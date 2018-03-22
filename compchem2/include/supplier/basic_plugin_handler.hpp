#ifndef COMPCHEM_BASIC_PLUGIN_HANDLER_HPP
#define COMPCHEM_BASIC_PLUGIN_HANDLER_HPP

// ------------------------------------------------------------
// class: basic_plugin_handler
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter_traits.hpp"
#include "supplier/supplier_constants.hpp"
#include <string>

namespace supplier { class supplier_error; } 
namespace supplier { class supplier_helper; } 

namespace supplier {

/**
 * Base class for handling a dynamically loaded external library containing 
 * a basic_filter_traits based object. This provides methods for the 
 * basic_filter virtual functions but is not a descendent of that class and 
 * does not set the callback functions of m_plugin.
 */
class basic_plugin_handler
{
  private:
    /**
     * A pointer to the DLL information returned by the dynamic loader.
     */
    void * m_dll_handle;


  protected:
    /**
     * The plugin object (reference).
     * 
     * \always  m_plugin /= nul -> m_dll_handle /= nul
     */
    ::basic_filter_traits * m_plugin;


  public:
    /**
     * The symbol name of a static base_filter_traits object in a 
     * dynamic plugin module that is used to work with the module.  
     * This is placed here largely to document the name of the 
     * object that is searched for.
     */
    static const char plugin_object_name[];

    /**
     * Attempt to create a plugin object for the library in a_filename. 
     * This does not set the callback members of basic_filter_traits.
     * 
     * \post nul /= m_dll_handle and nul /= plugin and nul /= plugin->identity
     * plugin->flags & LINE xor plugin->flags & BUFFER xor plugin->flags & FILE
     * plugin->flags & STRING xor plugin->flags & DIRECT
     * nul /= plugin->m_process and nul /= plugin->finalise
     */
    basic_plugin_handler(const std::string& a_filename);


  private:
    /**
     * no copy
     */
    basic_plugin_handler(const basic_plugin_handler & source);


  public:
    /**
     * Deconstruct the object, automatically unloading the plugin if necessary.
     */
    virtual ~basic_plugin_handler()
    {
      if (m_dll_handle != NULL)
      {
        unload_plugin ();
      }
    }

  private:
    /**
     * no copy
     */
    basic_plugin_handler & operator=(const basic_plugin_handler & source);


  public:
    /**
     * An error message created when, for example, a method returns FATAL.
     */
    std::string error_message() const
    {
      std::string Result;
      if (NULL != m_plugin->m_error)
      {
        Result.assign (m_plugin->m_error);
      }
      return Result;
    }

    /**
     * What are the flags for this filter. A combination of:
     * LINE_ORIENTED or BUFFER_ORIENTED or FILE_ORIENTED
     * with
     * DIRECT_OUTPUT or STRING_OUTPUT
     */
    int flags() const
    {
      return m_plugin->m_flags;
    }

    /**
     * A descriptive name for this plugin object.  It may not be an 
     * empty string.
     */
    std::string identity() const
    {
      return std::string (m_plugin->m_identity);
    }

    /**
     * After creation of a filter object this method is called with a_phrase obtained 
     * from the 'detect' filter (or the filename for 'detect' filters) to prescreen the filter 
     * to see if they are usuable. This calls the m_initialise functor of the 
     * basic_filter_traits object.
     * 
     * \pre not is_initialised
     */
    int initialise(const std::string& a_phrase)
    {
      COMPCHEM_REQUIRE(NULL == (m_plugin->m_internal_state), "Plugin is already initialised.");
      // Perform processing.
      return (*(m_plugin->m_initialise))(m_plugin, a_phrase.data (), a_phrase.size ());
    }
    /**
     * Check whether this plugin has been intialised
     * 
     * \return nul /= m_plugin->m_internal_state
     */
    bool is_initialised()
    {
      return NULL != m_plugin->m_internal_state;
    }

    /**
     * Get the plugin to output the results. For STRING this will pass a_result
     * to the m_output_cb member of m_pluign. For DIRECT this will pass
     * a_result to m_create_datum_cb etc.
     * 
     * \pre is_initialised
     * \return indicates if a_result is usable
     */
    int output(void * a_result)
    {
      COMPCHEM_REQUIRE(NULL != (m_plugin->m_internal_state), "Plugin is not initialised.");
      // Perform operation
      return (*(m_plugin->m_output))(m_plugin, a_result);
    }
    

    /**
     * Process the file.  For LINE_ORIENTED and BUFFER_ORIENTED 
     * subtypes a_buffer contains a piece of a file. For FILE_ORIENTED
     * subtypes a_buffer contains the filename.
     * 
     * The result UNUSABLE indicates this object is not
     * usuable on this file and can be removed from the current
     * filter/parser set.
     * 
     * The result COMPLETE indicates this filter has finished gathering 
     * results and 'process' should not be called until after a reinitialising
     * the object.
     * 
     * \pre is_initialised
     */
    int process(char const* const a_buffer, int a_sz)
    {
      COMPCHEM_REQUIRE(NULL != (m_plugin->m_internal_state), "Plugin is not initialised.");
      // Perform processing.
      return (*(m_plugin->m_process))(m_plugin, a_buffer, a_sz);
    }
    

    /**
     * Reset any internal result state.
     */
    void reset()
    {
      (*(m_plugin->m_reset))(m_plugin);
    }


  protected:
    /**
     * Get the plugin state object.  This is to allow standalone filter programs
     * to access the data set so they can produce different outputs.  
     * 
     * \pre is_initialised
     */
    template< class t_type >t_type * state()
    {
      COMPCHEM_REQUIRE(NULL != (m_plugin->m_internal_state), "Plugin is not initialised.");
      return static_cast< t_type * >(m_plugin->m_internal_state);
    }


  private:
    /**
     * Unload a plugin object.  This will also destroy the plugin object. Only called 
     * during destruction.
     * 
     * \pre m_dll_handle /= nul
     * \post m_dll_handle = nul && m_plugin = nul
     */
    void unload_plugin();


};

} // namespace supplier
#endif
