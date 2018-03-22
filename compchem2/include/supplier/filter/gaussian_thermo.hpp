#ifndef COMPCHEM_FILTER_GAUSSIAN_THERMO_HPP
#define COMPCHEM_FILTER_GAUSSIAN_THERMO_HPP

// ------------------------------------------------------------
// class: gaussian_thermo
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

#include <vector>
#include <string>

namespace supplier { namespace filter { class datum_proxy; }  } 
namespace supplier { class supplier_error; } 
struct basic_filter_traits;

namespace supplier {

namespace filter {

/**
 * Outcomes: thermochemistry
 * 
 * This filter works by first collecting the relevant segments during the 
 * line processing.  It then extracts the results from the cached lines 
 * during the output process.
 */
class gaussian_thermo : public basic_plugin {
  public:
    /**
     * A tristate flag to indicate where in the line collection process we are.
     */
    int m_found;

    /**
     * The cached segments
     */
    std::vector< std::string > m_lines;

    /**
     * Matches the start of the energy section.
     */
    const boost::regex m_start;

    /**
     * Matches the end of the energy section.
     */
    const boost::regex m_stop;

    /**
     * Can this filter handle the given file type?
     */
    static int initial_test(::basic_filter_traits * a_filter, char const *const a_phrase, int a_sz);


  private:
    /**
     * no copy
     */
    gaussian_thermo(const gaussian_thermo & );

    /**
     * no copy
     */
    gaussian_thermo & operator =(const gaussian_thermo & );


  public:
    int output_default(::basic_filter_traits * a_filter, void * a_cb_data);

    /**
     * Process a_segment of the file.
     */
    int process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz);

    gaussian_thermo()
    : m_found (0)
    , m_lines ()
    , m_start ("^ Zero-point", boost::regex::extended)
    , m_stop ("[Ll]og10", boost::regex::extended) {}
    ~gaussian_thermo() throw() {}

};

} // namespace supplier::filter

} // namespace supplier
#endif
