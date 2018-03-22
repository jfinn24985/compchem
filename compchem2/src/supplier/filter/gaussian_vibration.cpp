// ------------------------------------------------------------
// class: gaussian_vibration
// Orig author: Justin Finnerty
// $author:$
// Orig date:
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


// #define NO_LOG
#include "supplier/filter/gaussian_vibration.hpp"
#include "supplier/filter/datum_proxy.hpp"
#include "supplier/supplier_error.hpp"
#include "supplier/basic_filter_traits.hpp"

// Manual includes
#include <sstream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "utility/framework.hpp"
// --
// The symbol retrieved by the filter manager
extern "C" {
basic_filter_traits plugin_object = {
      "Gaussian Inc. log file vibration/peak table filter (C) 2008 V1.1.0"
      , NULL
      , NULL
      , &supplier::filter::gaussian_vibration::initial_test
      , &supplier::filter::gaussian_vibration::process
      , &supplier::filter::gaussian_vibration::output
      , &supplier::filter::gaussian_vibration::finalise
      , CCHEM_SUPPLIER_LINE_ORIENTED | CCHEM_SUPPLIER_DIRECT_OUTPUT
      , NULL
      , NULL
      , NULL
      , NULL
      , NULL
      , NULL
    };
}
namespace supplier {

namespace filter {

/**
 * Can this filter handle the given file type?
 */
int gaussian_vibration::initial_test(::basic_filter_traits * a_filter, char const *const a_phrase, int a_sz)

{
  // LOCAL
  int Result (CCHEM_SUPPLIER_UNUSABLE);
  if (NULL == a_filter)
  {
    Result = CCHEM_SUPPLIER_FATAL;
  }
  try
  {
    if (NULL != a_filter->m_internal_state)
    {
      set_error (a_filter, "Attempt to initialise and already initialised filter.");
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else
    {
      // build regular expression with case insensitive flag
      boost::regex l_command_re ("(freq[[:space:](=]|[[:space:](,]calcall[[:space:]),])", boost::regex::extended | boost::regex::icase);
      // DO
      // Test Gaussian "command" phrase matches our test regular expression
      if (boost::regex_search (a_phrase, a_phrase + a_sz, l_command_re))
      {
        a_filter->m_internal_state = new gaussian_vibration;
        Result = CCHEM_SUPPLIER_SUCCESS;
      }
    }
  }
  catch (const supplier_error &a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = a_err.level;
  }
  catch (const std::exception & a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  catch (...)
  {
    set_error (a_filter, "Unknown error type.");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  
  // END
  return Result;

}

/**
 * Output the result.
 */
int gaussian_vibration::output_default(::basic_filter_traits * a_filter, void * a_cb_data) 
{
  // LOCAL
  int Result = CCHEM_SUPPLIER_SUCCESS;
   // DO
  if (not m_table.empty () || m_state != END_SEARCH)
  {
    class helper__
    {
    public:
      static void set_vibration (basic_filter_traits * a_filter, void *a_parent, int &a_index, const chemistry::vibration & a_cursor)
      {
        datum_proxy l_vibration ("vibration");
        l_vibration.ordinal = a_index;
        void *l_top (l_vibration.output (a_filter, a_parent));
        datum_proxy l_frequency ("frequency", a_cursor.frequency ());
        l_frequency.output (a_filter, l_top);
        datum_proxy l_ir_intensity ("ir-intensity", a_cursor.intensity ());
        l_ir_intensity.output (a_filter, l_top);
        datum_proxy l_mode ("mode", a_cursor.mode ());
        l_mode.output (a_filter, l_top);
        if (not a_cursor.raman ().empty ())
        {
          datum_proxy l_raman_activity ("raman-activity", a_cursor.raman ());
          l_raman_activity.output (a_filter, l_top);
        }
        ++a_index;
      }
    };
    // Create an element to contain the table
    datum_proxy l_table ("vibration-table");
    void *l_table_node (l_table.output (a_filter, a_cb_data));
    int l_index (1); // Use one based counting for ordinals.
    std::for_each (m_table.begin (), m_table.end (), boost::bind (&helper__::set_vibration, a_filter, l_table_node, boost::ref (l_index), _1));
  }
  else
  {
    // Nothing to output, this is an error??
    set_error (a_filter, "No results in vibration table filter");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  
  // END
  return Result;

}

/**
 * Find the dipole and extract into state.
 */
int gaussian_vibration::process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  boost::cmatch l_found_data;
  // DO
  switch (m_state)
  {
  case START_SEARCH:
    DLOG (".");
    if (boost::regex_search (a_buffer, a_buffer + a_sz, m_start_re))
    {
      // Found beginning of table
      DLOG ("\nTable start found\n");
      m_state = TABLE_PROCESS;
    }
    break;
  case TABLE_PROCESS:
    {
      COMPCHEM_CHECK (m_table.empty ()or utility::in_irange (m_table.size () - m_vibr_base, 1U, 3U), "Can only have one to three vibrations!");
#ifndef NO_LOG
      const std::string l_buffer (a_buffer, a_sz);
#endif
      if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_dispvec_re))
      {
        DLOG ("DISP:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Have displacement vector.
        int l_atom_num = boost::lexical_cast < int >(l_found_data.str (2));
        const std::string l_atom_sym (chemistry::periodic_table::table ().at (l_atom_num).symbol ());
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].displacement_vector_append (l_atom_sym);
          m_table[m_vibr_base + 2].displacement_vector_append (",");
          m_table[m_vibr_base + 2].displacement_vector_append (l_found_data.str (12));
          m_table[m_vibr_base + 2].displacement_vector_append (",");
          m_table[m_vibr_base + 2].displacement_vector_append (l_found_data.str (13));
          m_table[m_vibr_base + 2].displacement_vector_append (",");
          m_table[m_vibr_base + 2].displacement_vector_append (l_found_data.str (14));
          m_table[m_vibr_base + 2].displacement_vector_append ("\\");
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].displacement_vector_append (l_atom_sym);
          m_table[m_vibr_base + 1].displacement_vector_append (",");
          m_table[m_vibr_base + 1].displacement_vector_append (l_found_data.str (8));
          m_table[m_vibr_base + 1].displacement_vector_append (",");
          m_table[m_vibr_base + 1].displacement_vector_append (l_found_data.str (9));
          m_table[m_vibr_base + 1].displacement_vector_append (",");
          m_table[m_vibr_base + 1].displacement_vector_append (l_found_data.str (10));
          m_table[m_vibr_base + 1].displacement_vector_append ("\\");
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].displacement_vector_append (l_atom_sym);
          m_table[m_vibr_base].displacement_vector_append (",");
          m_table[m_vibr_base].displacement_vector_append (l_found_data.str (4));
          m_table[m_vibr_base].displacement_vector_append (",");
          m_table[m_vibr_base].displacement_vector_append (l_found_data.str (5));
          m_table[m_vibr_base].displacement_vector_append (",");
          m_table[m_vibr_base].displacement_vector_append (l_found_data.str (6));
          m_table[m_vibr_base].displacement_vector_append ("\\");
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_atoms_re))
      {
        DLOG ("ATOM:");
        DLOG (l_buffer);
        DLOG ("\n");
      }                           // Vector list title line, ignore
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_depolar_re))
      {
        DLOG ("DPLR:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].depolar (l_found_data.str (7));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].depolar (l_found_data.str (5));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].depolar (l_found_data.str (3));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_raman_re))
      {
        DLOG ("RAMN:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].raman (l_found_data.str (6));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].raman (l_found_data.str (4));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].raman (l_found_data.str (2));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_intensity_re))
      {
        DLOG ("INTN:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].intensity (l_found_data.str (6));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].intensity (l_found_data.str (4));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].intensity (l_found_data.str (2));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_force_re))
      {
        DLOG ("FRCE:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].force (l_found_data.str (6));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].force (l_found_data.str (4));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].force (l_found_data.str (2));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_redms_re))
      {
        DLOG ("RDMS:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].reduced_mass (l_found_data.str (6));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].reduced_mass (l_found_data.str (4));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].reduced_mass (l_found_data.str (2));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_frequency_re))
      {
        DLOG ("FREQ:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Add the current data
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].frequency (l_found_data.str (6));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].frequency (l_found_data.str (4));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].frequency (l_found_data.str (2));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_ordinal_re))
      {
        DLOG ("ORDN:");
        DLOG (l_buffer);
        DLOG ("\n");
        // Got ordinal, determine how many vibs in this group.
#ifndef NO_LOG
        for (unsigned int i (0); i < l_found_data.size (); ++i)
        {
          std::cerr << "ORDN[" << i << "]: [" << l_found_data.str (i) << "]\n";
        }
#endif
        unsigned int l_vibr_count = 1;
        if (l_found_data[5].matched)
        {
          l_vibr_count = 3;
        }
        else if (l_found_data[3].matched)
        {
          l_vibr_count = 2;
        }
        // Extend m_table for new data
        m_vibr_base = m_table.size ();
        m_table.resize (m_vibr_base + l_vibr_count);
        // Capture values.
        switch (m_table.size () - m_vibr_base)
        {
        case 3:                  // Have three vibs
          m_table[m_vibr_base + 2].ordinal (l_found_data.str (5));
          // fall through
        case 2:                  // Have at least two vibs
          m_table[m_vibr_base + 1].ordinal (l_found_data.str (3));
          // fall through
        case 1:                  // Have at least one vibs
          m_table[m_vibr_base].ordinal (l_found_data.str (1));
          break;
        }
      }
      else if (boost::regex_search (a_buffer, a_buffer + a_sz, l_found_data, m_mode_re))
      {
        DLOG ("MODE:");
        DLOG (l_buffer);
        DLOG ("\n");
        // I think we got mode,if we have entries then we did get modes
        if (not m_table.empty ())
        {
          // Capture values.
          switch (m_table.size () - m_vibr_base)
          {
          case 3:                // Have three vibs
            m_table[m_vibr_base + 2].mode (l_found_data.str (5));
            // fall through
          case 2:                // Have at least two vibs
            m_table[m_vibr_base + 1].mode (l_found_data.str (3));
            // fall through
          case 1:                // Have at least one vibs
            m_table[m_vibr_base].mode (l_found_data.str (1));
            break;
          }
        }
      }
      else                        // if (0 == a_sz)
      {
        // End of table.
        // No match to any regex, if m_table is empty then still finding start
        DLOG ("NONE:");
        DLOG (l_buffer);
        DLOG ("\n");
        if (not m_table.empty ())
        {                         // - must be end of table
#ifndef NO_LOG
          std::copy (m_table.begin (), m_table.end (), std::ostream_iterator < chemistry::vibration > (std::cerr));
#endif
          m_state = END_SEARCH;
          Result = CCHEM_SUPPLIER_COMPLETE;
        }
      }
    }
    break;
  case END_SEARCH:               // Search completed, this is an error!
  default:
    std::stringstream l_err;
    l_err << "Invalid state indicator value [" << m_state << "]";
    set_error (a_filter, l_err.str ());
    Result = CCHEM_SUPPLIER_FATAL;
    break;
  }
  
  // END
  return Result;

}


} // namespace supplier::filter

} // namespace supplier
