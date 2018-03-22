// ------------------------------------------------------------
// class: basic_plugin_handler
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/basic_plugin_handler.hpp"
#include "supplier/supplier_error.hpp"
#include "supplier/supplier_helper.hpp"

// Manual includes
#include <dlfcn.h>
#include <boost/filesystem/operations.hpp>
// #include <boost/bind.hpp>
#include <sstream>
// --
namespace supplier {

/**
 * The symbol name of a static base_filter_traits object in a 
 * dynamic plugin module that is used to work with the module.  
 * This is placed here largely to document the name of the 
 * object that is searched for.
 */
const char basic_plugin_handler::plugin_object_name[]= "plugin_object";

/**
 * Attempt to create a plugin object for the library in a_filename. 
 * This does not set the callback members of basic_filter_traits.
 * 
 * \post nul /= m_dll_handle and nul /= plugin and nul /= plugin->identity
 * plugin->flags & LINE xor plugin->flags & BUFFER xor plugin->flags & FILE
 * plugin->flags & STRING xor plugin->flags & DIRECT
 * nul /= plugin->m_process and nul /= plugin->finalise
 */
basic_plugin_handler::basic_plugin_handler(const std::string& a_filename) 
: m_dll_handle (NULL)
, m_plugin (NULL)
{
  COMPCHEM_REQUIRE (not a_filename.empty (), "Filename cannot be empty string.");
  ////////////////
  // Load library
  boost::filesystem::path l_path (a_filename);
  if (not boost::filesystem::exists (l_path))
  {
    //Try default directory locations
    const supplier_helper & l_help (supplier_helper::helper ());
    for (supplier_helper::dir_iterator l_dir = l_help.begin_directory (); l_dir != l_help.end_directory (); ++l_dir)
    {
      if (boost::filesystem::exists ((*l_dir) / l_path))
      {
        l_path = (*l_dir) / l_path;
        break;
      }
    }
    if (not boost::filesystem::exists (l_path))
    {
      std::stringstream l_msg;
      l_msg << "Loading filter [" << a_filename << "] failed: Could not find file.";
      throw (supplier_error (l_msg.str (), CCHEM_SUPPLIER_NOFILE));
    }
  }
  
  // Attempt to open file
  m_dll_handle = dlopen (l_path.native_file_string ().c_str (), RTLD_NOW);
  if (NULL == m_dll_handle)
  {
    // "m_dll_handle" was null so a problem occured
    std::stringstream l_msg;
    l_msg << "Loading filter [" << a_filename << "] failed: ";
    const char *l_tmp = dlerror ();
    if (l_tmp != NULL)
    {
      l_msg << l_tmp;
    }
    throw (supplier_error (l_msg.str (), CCHEM_SUPPLIER_LOADERROR));
  }
  
  // Load plugin object from library
  basic_filter_traits *l_plugin = static_cast < basic_filter_traits * >(dlsym (m_dll_handle, plugin_object_name));
  // "l_plugin" should not be null, unless a problem occured
  if (NULL == l_plugin)
  {
    // Could not get object
    std::stringstream l_msg;
    l_msg << "Filter [" << a_filename << "] could not get plugin object!";
    const char *l_tmp = dlerror ();
    if (l_tmp != NULL)
    {
      l_msg << l_tmp;
    }
    unload_plugin ();
    throw (supplier_error (l_msg.str (), CCHEM_SUPPLIER_LOADERROR));
  }
  // Test if the plugin object is valid 
  
  //  nul /= plugin->identity
  //  plugin->flags & (LINE | BUFFER | FILE)
  //  plugin->flags & (STRING|DIRECT)
  //  plugin->flags & (LINE | BUFFER) and nul /= plugin->m_process_part
  //  plugin->flags & (FILE) and nul /= plugin->m_process_file
  //  plugin->flags & (STRING) and nul /= plugin->m_output_string
  //  plugin->flags & (DIRECT) and nul /= plugin->m_output_direct
  //  nul /= plugin->reset
  //  
  // Expect plugin object created was invalid!
  int l_test_plugin (CCHEM_SUPPLIER_SUCCESS);
  std::stringstream l_msg;
  l_msg << "Filter [" << a_filename << "] returned an invalid plugin object!";
  if (l_plugin->m_identity == NULL)
  {
    l_msg << " No identity information.\n";
    l_test_plugin = CCHEM_SUPPLIER_BADDATA;
  }
  else
  {
    l_msg << " Identity = \"" << l_plugin->m_identity << "\"\n";
  }
  
  if (not (l_plugin->m_flags & CCHEM_SUPPLIER_LINE_ORIENTED xor l_plugin->m_flags & CCHEM_SUPPLIER_BUFFER_ORIENTED xor l_plugin->m_flags & CCHEM_SUPPLIER_FILE_ORIENTED))
  {
    l_msg << " Flags contain no process orientation information.\n";
    l_test_plugin = CCHEM_SUPPLIER_BADDATA;
  }
  if (not (l_plugin->m_flags & CCHEM_SUPPLIER_STRING_OUTPUT xor l_plugin->m_flags & CCHEM_SUPPLIER_DIRECT_OUTPUT))
  {
    l_msg << " Flags contain no output orientation information.\n";
    l_test_plugin = CCHEM_SUPPLIER_BADDATA;
  }
  if (NULL == l_plugin->m_process)
  {
    l_msg << " Filter has no process function.\n";
    l_test_plugin = CCHEM_SUPPLIER_BADDATA;
  }
  if (NULL == l_plugin->m_output)
  {
    l_msg << " Filter has no output function.\n";
    l_test_plugin = CCHEM_SUPPLIER_BADDATA;
  }
  if (NULL == l_plugin->m_reset)
  {
    l_msg << " Filter has no finalise function.\n";
    l_test_plugin = CCHEM_SUPPLIER_BADDATA;
  }
  // Finalise
  if (l_test_plugin != CCHEM_SUPPLIER_SUCCESS)
  {
    unload_plugin ();
    throw (supplier_error (l_msg.str (), CCHEM_SUPPLIER_LOADERROR));
  }
  else
  {
    // Plugin is valid, set internal object.
    m_plugin = l_plugin;
  }

}

/**
 * Unload a plugin object.  This will also destroy the plugin object. Only called 
 * during destruction.
 * 
 * \pre m_dll_handle /= nul
 * \post m_dll_handle = nul && m_plugin = nul
 */
void basic_plugin_handler::unload_plugin() 
{
  // PRE
  COMPCHEM_REQUIRE (m_dll_handle != NULL, "Can not unload a plugin if no plugin is loaded!");
  // LOCAL
  // Perform finalisation of plugin and set to NULL
  if (m_plugin != NULL)
  {
    if (m_plugin->m_reset != NULL)
    {
      (*(m_plugin->m_reset)) (m_plugin);
    }
    m_plugin = NULL;
  }
  // Unload plugin
  dlclose (m_dll_handle);
  m_dll_handle = NULL;
  // POST
  COMPCHEM_ENSURE (m_dll_handle == NULL and m_plugin == NULL, "State objects remain after unload!");

}


} // namespace supplier
