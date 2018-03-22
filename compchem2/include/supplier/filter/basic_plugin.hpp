#ifndef COMPCHEM_FILTER_BASIC_PLUGIN_HPP
#define COMPCHEM_FILTER_BASIC_PLUGIN_HPP

// ------------------------------------------------------------
// class: basic_plugin
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter_traits.hpp"
#include "supplier/supplier_constants.hpp"
#include <string>

namespace supplier { class supplier_error; } 

namespace supplier {

namespace filter {

/**
 * Base class for building a plugin filter.
 */
class basic_plugin
{
  public:
    virtual ~basic_plugin() {}

    /**
     * Reset the plugin.
     * 
     * \pre nul /= a_plugin
     * \post a_plugin->internal_state = nul and a_plugin->error = nul
     */
    static void finalise(::basic_filter_traits * a_filter);
    /**
     * Output the result.
     */
    static int output(::basic_filter_traits * a_filter, void * a_cb_data);

    virtual int output_default(::basic_filter_traits * a_filter, void * a_cb_data) = 0;

    /**
     * Process a piece of the file.
     * 
     * \pre nul /= a_plugin
     */
    static int process(::basic_filter_traits *const a_filter, char const *const a_buffer, int a_sz);

    /**
     * This checks that the file is really a gaussian file and sets 
     * the filetype and subtype information.
     * 
     * \return SUCCESS if a Gaussian output, UNUSABLE if not.
     */
    virtual int process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz) = 0;

    /**
     * Set the plugin's error message from a_msg.
     */
    static void set_error(::basic_filter_traits *const a_filter, std::string a_msg);


};

} // namespace supplier::filter

} // namespace supplier
#endif
