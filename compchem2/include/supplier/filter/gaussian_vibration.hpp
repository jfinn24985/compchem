#ifndef COMPCHEM_FILTER_GAUSSIAN_VIBRATION_HPP
#define COMPCHEM_FILTER_GAUSSIAN_VIBRATION_HPP

// ------------------------------------------------------------
// class: gaussian_vibration
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/filter/basic_plugin.hpp"
#include "chemistry/vibration.hpp"
#include "chemistry/periodic_table.hpp"
#include <boost/regex.hpp>


// Manual includes
#include <sstream>
#include <vector>
// -
namespace supplier { namespace filter { class datum_proxy; }  } 
namespace supplier { class supplier_error; } 
struct basic_filter_traits;

namespace supplier {

namespace filter {

/**
 * Outcome ir-peak-table
 * 
 * Collect the vibration table from a gaussian log file. It collects all the
 * information from the vibration section of the output log file but only
 * the header lines (freq, intn, raman, mode) are output.
 */
class gaussian_vibration : public basic_plugin {
  public:
    typedef std::vector< chemistry::vibration > table_t;

    typedef table_t::iterator iterator;

    typedef table_t::const_iterator const_iterator;

    /**
     * Where are we in the search sequence?
     */
    enum indexer {
      START_SEARCH= 0,/**
       * Indicate we are searching for the start of the vibration table.
       */

      TABLE_PROCESS,
      END_SEARCH

    };

    /**
     * Capture vibration atoms
     */
    boost::regex m_atoms_re;

    /**
     * Capture vibration depolar value
     */
    boost::regex m_depolar_re;

    /**
     * Capture vibration displacement vector
     */
    boost::regex m_dispvec_re;

    /**
     * Capture vibration force constant
     */
    boost::regex m_force_re;

    /**
     * Capture vibration frequency
     */
    boost::regex m_frequency_re;

    /**
     * Capture vibration intensity
     */
    boost::regex m_intensity_re;

    /**
     * Capture vibration mode
     */
    boost::regex m_mode_re;

    /**
     * Capture vibration ordinals.
     */
    boost::regex m_ordinal_re;

    /**
     * Capture vibration raman intensity
     */
    boost::regex m_raman_re;

    /**
     * Capture vibration reduced mass
     */
    boost::regex m_redms_re;

    /**
     * Find start of table
     */
    boost::regex m_start_re;

    /**
     * The current point in the filter sequence (one START_SEARCH, TABLE_PROCESS or END_SEARCH)
     */
    int m_state;

    /**
     * The base index of the current vibrations.  The current vibs are:
     * m_table[m_vibr_base] .. m_table[m_vibr_base + m_vibr_count - 1]
     */
    unsigned int m_vibr_base;


  private:
    /**
     * The table of vibrations.
     */
    std::vector<chemistry::vibration> m_table;


  public:
    iterator begin() throw() { return m_table.begin (); }

    const_iterator begin() const throw() { return m_table.begin (); }

    /**
     * Can this filter handle the given file type?
     */
    static int initial_test(::basic_filter_traits * a_filter, char const *const a_phrase, int a_sz);

    bool empty() const throw() { return m_table.empty (); }

    iterator end() throw() { return m_table.end (); }

    const_iterator end() const throw() { return m_table.end (); }

    /**
     * no copy
     */
    gaussian_vibration(const gaussian_vibration & source);

    inline gaussian_vibration();

    ~gaussian_vibration() throw() {}


  private:
    /**
     *  not defined
     */
    gaussian_vibration & operator =(const gaussian_vibration & );


  public:
    /**
     * Output the result.
     */
    int output_default(::basic_filter_traits * a_filter, void * a_cb_data);

    /**
     * Find the dipole and extract into state.
     */
    int process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz);

    unsigned int size() const throw() { return m_table.size (); }

};
inline gaussian_vibration::gaussian_vibration() 
  : m_atoms_re ("[[:space:]]+(Atom[[:space:]]+AN[[:space:]]+)(X[[:space:]]+Y[[:space:]]+Z)([[:space:]]+X[[:space:]]+Y[[:space:]]+Z)?([[:space:]]+X[[:space:]]+Y[[:space:]]+Z)?[[:space:]]*", boost::regex::extended)
  , m_depolar_re ("[[:space:]]+Depolar[[:space:]]+([(][PU][)][[:space:]]+)?--([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?[[:space:]]*", boost::regex::extended)
  , m_dispvec_re ("[[:space:]]+([[:digit:]]+)[[:space:]]+([[:digit:]]+)([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+)[[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+)[[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+)[[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+)[[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+)[[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+)[[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?[[:space:]]*", boost::regex::extended)
  , m_force_re ("[[:space:]]+Frc consts[[:space:]]+--([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?[[:space:]]*$", boost::regex::extended)
  , m_frequency_re ("[[:space:]]+Frequencies[[:space:]]+--([[:space:]]+(-?[[:digit:]]+.[[:digit:]]+))([[:space:]]+(-?[[:digit:]]+.[[:digit:]]+))?([[:space:]]+(-?[[:digit:]]+.[[:digit:]]+))?[[:space:]]*", boost::regex::extended)
  , m_intensity_re ("[[:space:]]+IR Inten[[:space:]]+--([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?[[:space:]]*", boost::regex::extended)
  , m_mode_re ("[[:space:]]+([^[:space:]]+)([[:space:]]+([^[:space:]]+))?([[:space:]]+([^[:space:]]+))?[[:space:]]*", boost::regex::extended)
  , m_ordinal_re ("^[[:space:]]+([[:digit:]]+)([[:space:]]+([[:digit:]]+))?([[:space:]]+([[:digit:]]+))?[[:space:]]*", boost::regex::extended)
  , m_raman_re ("[[:space:]]+Raman[[:space:]]+Activ[[:space:]]+--([[:space:]]+([[:digit:]]+\\.[[:digit:]]+))([[:space:]]+([[:digit:]]+\\.[[:digit:]]+))?([[:space:]]+([[:digit:]]+\\.[[:digit:]]+))?[[:space:]]*", boost::regex::extended)
  , m_redms_re ("[[:space:]]+Red\\. masses[[:space:]]+--([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?([[:space:]]+(-?[[:digit:]]+\\.[[:digit:]]+))?[[:space:]]*$", boost::regex::extended)
  , m_start_re ("educed m")
  , m_state (START_SEARCH)
  , m_vibr_base (0)
  , m_table () {}



} // namespace supplier::filter

} // namespace supplier
#endif
