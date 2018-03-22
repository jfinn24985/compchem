// ------------------------------------------------------------
// class: plugin_filter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/plugin_filter.hpp"
#include "supplier/datum_traits.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/filter.hpp"

// Manual includes
#include <dlfcn.h>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include <sstream>
// --
namespace supplier {

/**
 * Attempt to create a plugin object for the library in a_filename. 
 * 
 * \post is valid as follows
 * nul /= m_dll_handle
 * nul /= plugin
 * nul /= plugin->identity
 * plugin->flags & LINE xor plugin->flags & BUFFER xor plugin->flags & FILE
 * plugin->flags & (STRING|DIRECT)
 * plugin->flags & (LINE | BUFFER) and nul /= plugin->m_process_part
 * plugin->flags & (FILE) and nul /= plugin->m_process_file
 * plugin->flags & (STRING) and nul /= plugin->m_output_string
 * plugin->flags & (DIRECT) and nul /= plugin->m_output_direct
 * nul /= plugin->finalise
 */
plugin_filter::plugin_filter(const data_scheme::filter& a_filter) 
: basic_filter (a_filter)
, basic_plugin_handler (a_filter.href ())
{
  COMPCHEM_REQUIRE (not a_filter.label ().empty (), "Need a filter label to create a filter.");
  COMPCHEM_REQUIRE (not a_filter.outcomes ().empty (), "Need a set of outcomes to create a filter.");
  // Plugin is valid or a error would be thrown. set callbacks.
  m_plugin->m_output_cb = &plugin_filter::output_callback_;
  m_plugin->m_create_datum_cb = &datum_traits::create;
  m_plugin->m_add_content_cb = &datum_traits::add_content;
  m_plugin->m_set_format_cb = &datum_traits::set_format;
  m_plugin->m_set_ordinal_cb = &datum_traits::set_ordinal;
  m_plugin->m_set_scale_cb = &datum_traits::set_scale;

}

/**
 * Deconstruct the object, automatically unloading the plugin if necessary.
 */
plugin_filter::~plugin_filter() 
{}

/**
 * Callback to be set on the m_output_cb of base_plugin_traits
 */
void plugin_filter::output_callback_(void * a_obj, char const *const a_result, int a_sz)

{
  reinterpret_cast< std::string* >(a_obj)->assign (a_result, a_sz);
}


} // namespace supplier
