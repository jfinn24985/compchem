#ifndef COMPCHEM_SUPPLIER_PLUGIN_FILTER_HPP
#define COMPCHEM_SUPPLIER_PLUGIN_FILTER_HPP

// ------------------------------------------------------------
// class: plugin_filter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter.hpp"
#include "supplier/basic_plugin_handler.hpp"
#include <string>

// Manual includes
#include <vector>
// #include <sstream>
// --
class datum_traits;
namespace data_scheme { class memo; } 
namespace data_scheme { class filter; } 

namespace supplier {

/**
 * A basic_filter class for handling dynamically loaded filter object.
 */
class plugin_filter : public basic_filter, public basic_plugin_handler {
  public:
    /**
     * An error message created when, for example, a method returns an error
     * indicator.
     */
    std::string error_message() const
    {
      return basic_plugin_handler::error_message();
    }

    /**
     * What are the flags for this filter. A combination of:
     * LINE_ORIENTED or BUFFER_ORIENTED or FILE_ORIENTED
     * with
     * DIRECT_OUTPUT or STRING_OUTPUT
     */
    int flags() const
    {
      return basic_plugin_handler::flags();
    }

    /**
     * A descriptive name for this plugin object.  It may not be an 
     * empty string.
     */
    std::string identity() const
    {
      return basic_plugin_handler::identity();
    }

    /**
     * After creation of a filter object this method is called with a_phrase obtained 
     * from the 'detect' filter (or the filename for 'detect' filters) to prescreen the filter 
     * to see if they are usuable. This calls the m_initialise functor of the 
     * basic_filter_traits object.
     */
    int initial_test(const std::string& a_phrase)
    {
      return initialise (a_phrase);
    }

  private:
    /**
     * no copy
     */
    plugin_filter & operator=(const plugin_filter & source);


  public:
    /**
     * Return the current results in a_string. 
     * 
     * \return indicates if a_result is usable
     */
    int output_string(std::string & a_result)
    {
      return output (&a_result);
    }
    

    /**
     * Output the results directly into the a_memo object. 
     * 
     * \return indicates if addition was successful. Filter must guarrantee
     * a_memo will remain valid in the event errors.
     */
    int output_direct(data_scheme::memo & a_memo)
    {
      return output (&a_memo);
    }
    

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
    plugin_filter(const data_scheme::filter& a_filter);


  private:
    /**
     * no copy
     */
    plugin_filter(const plugin_filter & source);


  public:
    /**
     * Deconstruct the object, automatically unloading the plugin if necessary.
     */
    ~plugin_filter();
    /**
     * Process the entire file. This is called for FILE_ORIENTED subtypes 
     * only. For mechanism discovery filters, the SUCCESS result indicates
     * that the file type was detected.
     * 
     * \pre flags & FILE_ORIENTED
     */
    int process_file(const std::string& a_filename)
    {
      return process(a_filename.c_str (), a_filename.size ());
    }

    /**
     * Process a piece of the file.  This will only be called
     * on LINE_ORIENTED and BUFFER_ORIENTED subtypes. The
     * return is one of SUCCESS, FAIL_FATAL or FAIL_UNUSABLE.
     * 
     * The result FAIL_UNUSABLE indicates this object is not
     * usuable on this file and can be removed from the current
     * filter/parser set.
     * 
     * \pre flags & (LINE_ORIENTED | BUFFER_ORIENTED)
     */
    int process_part(const std::string& a_buffer)
    {
      return process(a_buffer.data (), a_buffer.size ());
    }
    

    /**
     * Reset any internal result state.
     */
    void reset()
    {
      return basic_plugin_handler::reset();
    }

  private:
    /**
     * Callback to be set on the m_output_cb of base_plugin_traits
     */
    static void output_callback_(void * a_obj, char const *const a_result, int a_sz);

};

} // namespace supplier
#endif
