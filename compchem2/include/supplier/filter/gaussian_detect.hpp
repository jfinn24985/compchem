#ifndef COMPCHEM_SUPPLIER_GAUSSIAN_DETECT_HPP
#define COMPCHEM_SUPPLIER_GAUSSIAN_DETECT_HPP

// ------------------------------------------------------------
// class: gaussian_detect
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/filter/basic_plugin.hpp"
#include <boost/regex.hpp>

#include <string>

namespace supplier { class supplier_error; } 
struct basic_filter_traits;

namespace supplier {

namespace filter {

/**
 * Outcome: detect
 * 
 * The initialise function will be called with the base filename of the file 
 * (if it has one).
 * 
 * A detect filter should operate in BUFFER mode.
 * 
 * Returns a datum with the route section to be passed to other filters.
 * 
 *  A FILE mode filter that determines if the file is a Gaussian output file.
 */
class gaussian_detect : public basic_plugin {
  private:
    /**
     * The gaussian route/command section from the top of the log file.
     */
    std::string m_route;

    /**
     * The Gaussian subversion, eg g03/g98/g94..
     */
    std::string m_subversion;

    /**
     * The gaussian version information from the top of the log file.  This will be empty if the file is not a gaussian log file.
     */
    std::string m_version;

    /**
     * Error message when getting results before processing a file.
     */
    static const char* const s_no_result;


  public:
    /**
     * Main ctor.
     */
    gaussian_detect();


  private:
    /**
     * no copy
     */
    gaussian_detect(const gaussian_detect & source);


  public:
    /**
     * Initialise the plugin object. Creates the internal state object.
     * 
     * \pre nul /= a_plugin
     */
    static int initial_test(::basic_filter_traits * a_filter, char const *const a_filename, int a_sz);

    int output_default(::basic_filter_traits *const a_filter, void * a_cb_data);

    /**
     * This checks that the file is really a gaussian file and sets 
     * the filetype and subtype information.
     * 
     * \return SUCCESS if a Gaussian output, UNUSABLE if not.
     */
    int process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz);

    virtual ~gaussian_detect() {}


  private:
    /**
     * no assign
     */
    gaussian_detect & operator=(const gaussian_detect & source);

};

} // namespace supplier::filter

} // namespace supplier
#endif
