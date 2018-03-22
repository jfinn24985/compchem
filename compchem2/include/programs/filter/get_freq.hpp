#ifndef COMPCHEM_SUPPLIER_GET_FREQ_HPP
#define COMPCHEM_SUPPLIER_GET_FREQ_HPP

// ------------------------------------------------------------
// class: get_freq
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/plugin_filter_direct.hpp"
#include "report/spectrum.hpp"
#include <string>
#include <vector>

namespace cchem {

namespace programs {

/**
 * This class plays the role of the plugin_manager for a single filter. It does not load the a plugin dynamically, instead using the filter class directly.
 * 
 * gaussian_vibration type to be usable as a stand-alone filter to extract the vibration table.  By default it prints the frequencies and intensities in a text table.  Other outputs are possible.
 */
class get_freq {
  private:
    /**
     * The filter adaptor object
     */
    supplier::plugin_filter_direct m_filter;

    report::spectrum<t_value> m_spectrum;

    /**
     * Output a spectrum, outputing just a peak table is the default.
     */
    bool m_use_spectrum;

    /**
     * Whether to output raman activity and infrared intensity
     */
    bool m_use_raman;

    /**
     * Whether to output raman activity and not infrared intensity
     */
    bool m_only_raman;


  public:
    int main(int argc, char ** argv);

    /**
     * The name of the app.
     */
    std::string name() { return "get_freq"; }

    void output(const std::string& a_filename);

    void serialise(std::ostream & a_os);

    void output_xml(std::ostream & a_os);

    get_freq(): m_filter (), m_spectrum (), m_use_spectrum (false), m_use_raman (false), m_only_raman (false) {}

    ~get_freq() {}


  private:
    /**
     * no copy
     */
    get_freq(const get_freq & source);

    /**
     * no copy
     */
    get_freq & operator=(const get_freq & source);


  public:
    int process_files(const std::vector< std::string > & a_fileset);

    /**
     * Set whether to output a spectrum. Default is a peak table.,
     */
    void use_spectrum(bool a_spec) { m_use_spectrum = a_spec; }

};

} // namespace cchem::programs

} // namespace cchem
#endif
