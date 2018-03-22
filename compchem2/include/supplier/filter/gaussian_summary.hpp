#ifndef COMPCHEM_FILTER_GAUSSIAN_SUMMARY_HPP
#define COMPCHEM_FILTER_GAUSSIAN_SUMMARY_HPP

// ------------------------------------------------------------
// class: gaussian_summary
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
#include <sstream>

namespace supplier { class supplier_error; } 
namespace supplier { namespace filter { class datum_proxy; }  } 
struct basic_filter_traits;

namespace supplier {

namespace filter {

/**
 * Datum Outcomes: summary (title method basis-set point-group nimag
 * exit-status state calc-type dipole) energy (type..) geometry (inc charge/multi)
 * Info objects: title
 * 
 * This is one of the more interesting gaussian filters.  It demonstrates how you 
 * can dynamically choose between different "filters".  The functions and state 
 * object with suffix A are the default filter system when there will be a Gaussian 
 * archive.  The functions and state object with suffix B are when there will be a valid
 * calculation but no archive (only IRC calculations detected). The initial_test method 
 * detects when the default A system will not work and cuts-over the filter 
 * methods and internal state to use the B system
 * 
 * The A system of the ArcStanza filter is used when there will be an archive 
 * section. It gathers all the information it requires from this section. This 
 * makes the processLine() method simply a matter of checking for the 
 * archive to start then collecting the information. The interesting part of the 
 * filter is the post processing. Most of this is straightforward, simply saving 
 * the various parts into the results attribute. The difficult part involves storing 
 * the final geometry. This requires adding adding a new DOM element that 
 * is the sibling of the experimentRootNode.
 * 
 * The B system of the ArcStanza filters attempts to extract the information 
 * when there is not going to be an archive section.  The original (and main) 
 * use was to extract data from 'IRC' calculations. It tries to find most of the 
 * information from the beginning sections of the Gaussian output file. It 
 * searches for the methods and titles by attempting to decode this header 
 * section. The approach is fairly simplistic and may not be terribly robust, 
 * although it has worked in our hands successfully.
 */
class gaussian_summary : public basic_plugin {
  protected:
    /**
     * Constants used in this filter 
     */
    enum constants {
      VERSION_SEARCH = 0,/**
       * Searching for the version indicator
       */

      VERSION_FOUND,/**
       * Searching for the version termination indicator
       */

      COMMAND_SEARCH,/**
       * Searching for the command indicator
       */

      COMMAND_FOUND,/**
       * Searching for the command termination indicator
       */

      TITLE_SEARCH,/**
       * Searching for the title indicator
       */

      TITLE_FOUND,/**
       * Searching for the title termination indicator (and collecting title)
       */

      BASIS_SEARCH,/**
       * Searching for basis-set information
       */

      METHOD_SEARCH,/**
       * searching for method information
       */

      DIPOLE_SEARCH,/**
       * Searching for the dipole moment
       */

      ARCHIVE_SEARCH,/**
       * Indicate state is searching for beginning of archive.
       */

      TERM_SEARCH,/**
       * Searching for the termination status.
       */

      ARCHIVE_END_SEARCH,/**
       * Indicate state is searching for end of archive
       */

      COMPLETED_SEARCH/**
       * Indicate no more searching is required
       */


    };

    /**
     * The indices of the m_results_set
     */
    enum indices
    {
      BASIS_SET,/**
       * Index of basis set in processed result list used by the output methods
       */

      CALC_METHOD,/**
       * Index of calculation algorithm in processed result list used by the output methods
       */

      CALC_TYPE,/**
       * The calc type, eg Freq, Opt, IRC etc.
       */

      DIPOLE,/**
       * Index of dipole moment
       */

      ENERGY_HF,/**
       * The HF energy, usually collected for jobs with archives. This is meaningless
       * for ONIOM calculations.
       */

      ENERGY_MP2,/**
       * The MP2 energy (not always present)
       */

      EXIT_STATUS,
      NIMAG,/**
       * The index of the number of imaginary freqs
       */

      POINT_GROUP,/**
       * The index of the point-group.
       */

      STATE,/**
       * The molecular state
       */

      TITLE,/**
       * Index of gaussian title string in processed result list used by the output methods
       */

      GEOM_CHARGE,/**
       * The charge of the geometry
       */

      GEOM_DATA,/**
       * The geometry itself.
       */

      GEOM_MULT/**
       * The multiplicity of the geometry
       */


    };


  private:
    /**
     * The regular expression to use to find the dipole section
     */
    const boost::regex m_find_dipole;

    /**
     * RE to find the termination line.
     */
    boost::regex m_find_term;


  protected:
    /**
     * The results of this session.
     */
    std::vector<datum_proxy> m_result_set;

    /**
     * Flags of what has been found so far
     */
    int m_state;


  private:
    /**
     * no copy
     */
    gaussian_summary(const gaussian_summary & other);


  public:
    /**
     * Default constructor sets the starting state
     */
    gaussian_summary(int a_state);

    /**
     * Does nothing special here.
     */
    virtual ~gaussian_summary();

    /**
     * This filter is always appropriate.  This method actually selects 
     * system B if necessary.
     */
    static int initial_test(::basic_filter_traits * a_filter, char const *const a_phrase, int a_sz);


  private:
    /**
     *  not copy
     */
    gaussian_summary & operator =(const gaussian_summary & other);


  public:
    /**
     * Find the dipole and extract into state.
     */
    int process_dipole(::basic_filter_traits *const a_filter, char const *const a_segment, int a_sz);

    /**
     * Find the termination status.
     */
    int process_termination(::basic_filter_traits * a_filter, char const *const a_segment, int a_sz);

    /**
     * This adds the m_result_set contents to the output twiglet.
     * 
     * Programming notes:
     * * This assigns the outcomes to all the result set.
     * * For ONIOM calculations, HF/MP2 are not output as they will be incorrect.
     */
    int output_default(::basic_filter_traits * a_filter, void * a_cb_data);

};
/**
 * Subtype for the gaussian_summary type that understands the Gaussian
 * archive section.
 */
struct state_a : public gaussian_summary {
  private:
    /**
     * Cached archive section.
     */
    std::stringstream m_archive;

    /**
     * Find the start of the archive section
     */
    boost::regex m_start_re;

    /**
     * Find the end of the archive section
     */
    boost::regex m_end_re;

    /**
     * Process the archive string into the result set. This should
     * return COMPLETE on successful conversion.
     */
    int process_archive(::basic_filter_traits * a_filter);

    /**
     * Process the geometry segments of the archive section.
     * 
     * \\ POST a_begin is at Version segment
     * \return CCHEM_SUPPLIER_COMPLETE on success.
     */
    template < class t_iter > int process_geometry(::basic_filter_traits * a_filter, t_iter & a_begin, t_iter a_end);


  public:
    /**
     * Find the dipole and extract into state.
     */
    int process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz);

    state_a();

    ~state_a() throw() {}

};
/**
 * Subtype for the gaussian_summary type that is used when there will
 * be no Gaussian archive section. Currently this processes results for 
 * IRC calculations only.
 */
struct state_b : public gaussian_summary {
  private:
    /**
     * Find the basis set
     */
    const boost::regex m_basis_re;

    /**
     * Find the method
     */
    const boost::regex m_method_re;

    /**
     * Find the line that defines the start of the title
     */
    const boost::regex m_title_divider_re;

    /**
     * Capture the version
     */
    boost::regex m_version_re;


  public:
    /**
     * Find the various pieces as the file progresses.
     */
    int process_default(::basic_filter_traits *const a_filter, char const *const a_buffer, int a_sz);

    state_b();

    ~state_b() throw() {}

};

} // namespace supplier::filter

} // namespace supplier
#endif
