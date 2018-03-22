// ------------------------------------------------------------
// class: gaussian_summary
// Orig author: Justin Finnerty
// $author:$
// Orig date:
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


//#define NO_LOG
#include "supplier/filter/gaussian_summary.hpp"
#include "supplier/supplier_error.hpp"
#include "supplier/filter/datum_proxy.hpp"
#include "supplier/basic_filter_traits.hpp"

// Manual includes
// #include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
// #include <iterator>
// --
// The symbol retrieved by the filter manager
extern "C" {
basic_filter_traits plugin_object = {
      "Gaussian Inc. Log file/archive decoder (C) 2008 V1.1.0"
      , NULL
      , NULL
      , &supplier::filter::gaussian_summary::initial_test
      , &supplier::filter::gaussian_summary::process
      , &supplier::filter::gaussian_summary::output
      , &supplier::filter::gaussian_summary::finalise
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
 * Default constructor sets the starting state
 */
gaussian_summary::gaussian_summary(int a_state) 
: m_find_dipole ("Tot=")
, m_find_term ("termination")
, m_result_set (GEOM_MULT + 1) //Preallocate enough space in vector.
, m_state (a_state)
{}

/**
 * Does nothing special here.
 */
gaussian_summary::~gaussian_summary() 
{}

/**
 * This filter is always appropriate.  This method actually selects 
 * system B if necessary.
 */
int gaussian_summary::initial_test(::basic_filter_traits * a_filter, char const *const a_phrase, int a_sz)

{
  COMPCHEM_REQUIRE(NULL != a_filter, "Can not initialise a nul object.");
  COMPCHEM_REQUIRE(NULL == a_filter->m_internal_state, "Can not initialise a previously initialised object.");
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);	// This filter is always appropriate
  if (NULL == a_filter)
  {
    Result = CCHEM_SUPPLIER_FATAL;
  }
  else
  {
    try
    {
      boost::regex l_command_re ("[[:space:]]irc[[:space:](=]", boost::regex::extended | boost::regex::icase);
  // DO
  // If the Gaussian "command" phrase matches our test regular expression then
  // switch to B system.  Otherwise just create the A system state object.
      if (boost::regex_search (a_phrase, a_phrase + a_sz, l_command_re))
      {
        // Match RE so we must cut-over to use filter system B
        DLOG ("Using B system\n");
        a_filter->m_internal_state = new state_b;
      }
      else
      {
        DLOG ("Using A system\n");
        a_filter->m_internal_state = new state_a;
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
      set_error (a_filter, "Unknown error.");
      Result = CCHEM_SUPPLIER_FATAL;
    }
  }
  
  // END
  return Result;

}

/**
 * Find the dipole and extract into state.
 */
int gaussian_summary::process_dipole(::basic_filter_traits *const a_filter, char const *const a_segment, int a_sz) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  COMPCHEM_CHECK (m_result_set [DIPOLE].empty (), "Invalid state, searching for dipole after it was found.");
  if (68 == a_sz		// Skip lines with incorrect size
      and boost::regex_search (a_segment, a_segment + a_sz, m_find_dipole))
  {
    Result = CCHEM_SUPPLIER_COMPLETE; 
    DLOG ("Dipole found with line size ");DLOG(a_sz);DLOG("\n");
    // This regex looks for "X= 0.0 Y= 0.0 Z= 0.0 Tot= 0.0"
    boost::regex l_extract ("[XYZt]=[[:space:]]*([-.[:digit:]]+)", boost::regex::extended);
    unsigned int l_counter (0);
    std::stringstream l_output;
    // Perform match
    for (boost::regex_iterator<const char*> l_matcher (a_segment, a_segment + a_sz, l_extract), l_end; l_matcher != l_end; ++l_matcher, ++l_counter)
    {
      // Get result string
      if (l_counter > 0)
      {
        l_output << ",";
      }
      l_output << l_matcher->str (1);
    }
    if (4 <= l_counter)
    {
      // Add to result twiglet.
      m_result_set [DIPOLE].scale.assign ("debye");
      m_result_set [DIPOLE].format.assign ("x,y,z,tot");
      m_result_set [DIPOLE].content.assign (l_output.str ());
    }
    else
    {
      // ERROR: Could not find what we wanted in result string.
      set_error (a_filter, "Error extracting dipole from string [" + std::string (a_segment, a_sz) + "].");
      Result = CCHEM_SUPPLIER_BADDATA;
    }
  }
  // END
  return Result;

}

/**
 * Find the termination status.
 */
int gaussian_summary::process_termination(::basic_filter_traits * a_filter, char const *const a_segment, int a_sz) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  // DO
  COMPCHEM_CHECK (m_result_set [EXIT_STATUS].empty (), "Invalid state, searching for dipole after it was found.");
  if (boost::regex_search (a_segment, a_segment + a_sz, m_find_term))
  {
    m_result_set[EXIT_STATUS].content.assign (a_segment, a_sz);
    if (m_result_set[EXIT_STATUS].content.find ("Normal") == std::string::npos)
    {
      // Gaussian did not terminate normally, set error indicators and do not save status.
      set_error (a_filter, "Invalid result with program termination [" + std::string (a_segment, a_sz) + "]");
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else
    {
      boost::trim (m_result_set[EXIT_STATUS].content);
      Result = CCHEM_SUPPLIER_COMPLETE;
    }
  }
  // END
  return Result;

}

/**
 * This adds the m_result_set contents to the output twiglet.
 * 
 * Programming notes:
 * * This assigns the outcomes to all the result set.
 * * For ONIOM calculations, HF/MP2 are not output as they will be incorrect.
 */
int gaussian_summary::output_default(::basic_filter_traits * a_filter, void * a_cb_data) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  // First lets check we have the calc-method and
  // basis-set. It is not a big problem to not have other data.
  if (m_result_set[CALC_METHOD].content.empty ())
  {
    Result = CCHEM_SUPPLIER_BADDATA;
    set_error (a_filter, "No calculation method data found.");
  }
  else if (m_result_set[BASIS_SET].content.empty ())
  {
    Result = CCHEM_SUPPLIER_BADDATA;
    set_error (a_filter, "No basis set information found.");
  }
  else
  {
    // Set the outcomes on the various datum proxy objects
    m_result_set[BASIS_SET].outcome.assign ("basis-set");
    m_result_set[CALC_METHOD].outcome.assign ("calc-method");
    m_result_set[CALC_TYPE].outcome.assign ("calc-type");
    m_result_set[DIPOLE].outcome.assign ("dipole");
    m_result_set[ENERGY_HF].outcome.assign ("energy-HF");
    m_result_set[ENERGY_MP2].outcome.assign ("energy-MP2");
    m_result_set[EXIT_STATUS].outcome.assign ("exit-status");
    m_result_set[NIMAG].outcome.assign ("nimag");
    m_result_set[POINT_GROUP].outcome.assign ("point-group");
    m_result_set[STATE].outcome.assign ("state");
    m_result_set[TITLE].outcome.assign ("title");
    m_result_set[GEOM_DATA].outcome.assign ("geometry");
    m_result_set[GEOM_CHARGE].outcome.assign ("charge");
    m_result_set[GEOM_MULT].outcome.assign ("multiplicity");
    // Create summary node as child of the top-level node 
    datum_proxy l_top ("summary");
    void *l_top_node (l_top.output (a_filter, a_cb_data));
    // Add non-geometry data as children of the summary node
    for (unsigned int l_i = 0; l_i <= TITLE; ++l_i)
    {
      if (not m_result_set[l_i].empty ())
      {
        m_result_set[l_i].output (a_filter, l_top_node);
      }
    }
    // Now do the geometry.
    if (not m_result_set[GEOM_DATA].empty ())
    {
      // Create a geometry node as a child of the top-level.
      l_top_node = m_result_set[GEOM_DATA].output (a_filter, a_cb_data);
      if (m_result_set[GEOM_CHARGE].empty () or m_result_set[GEOM_MULT].empty ())
      {
        set_error (a_filter, "Invalid geometry data without charge and/or multiplicity.");
        Result = CCHEM_SUPPLIER_BADDATA;
      }
      else
      {
        // Now add the sub-nodes.
        m_result_set[GEOM_CHARGE].output (a_filter, l_top_node);
        m_result_set[GEOM_MULT].output (a_filter, l_top_node);
      }
    }
  }
  // END
  return Result;

}

/**
 * Process the archive string into the result set. This should
 * return COMPLETE on successful conversion.
 */
int state_a::process_archive(::basic_filter_traits * a_filter) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_COMPLETE);	// Error indicator and return value.
  try
  {
    // DO
    const std::string l_archive (m_archive.str ());	// Cache a copy of result
    DLOG ("Archive is: [" + l_archive + "]\n");
    COMPCHEM_CHECK (not l_archive.empty (), "No archive section found but call to process_archive");
  
    // Split archive into segments of the archive using regex.
    const boost::regex l_barbar ("\\\\");
    boost::sregex_token_iterator l_segment (boost::make_regex_token_iterator (l_archive, l_barbar, -1));
    boost::sregex_token_iterator l_end_segment;
    // Look for calculation "type" ( Operating mode, here eg opt, freq, irc )
    std::advance (l_segment, 3);	// Go to 4th segment
    m_result_set[CALC_TYPE].content = (l_segment->str ().empty ()? "unknown" : l_segment->str ());
    DLOG ("MECH_MODE = " + m_result_set[CALC_TYPE].content + "\n");
    ++l_segment;			// Go to 5th segment
    // Look for calculation "method"
    if (l_segment->str ().empty ())
    {
      // unknown method and basis
      m_result_set[CALC_METHOD].content = "unknown";
      m_result_set[BASIS_SET].content = "unknown";
      // Step to 6th segment
      ++l_segment;
    }
    else
    {
      // Check for ONIOM
      const boost::regex l_oniom_check ("ONIOM(.*)$", boost::regex::icase | boost::regex::extended);
      boost::smatch l_match;
      if (boost::regex_search (l_segment->str (), l_match, l_oniom_check))
      {
        // method is oniom
        m_result_set[CALC_METHOD].content = "ONIOM";
        m_result_set[BASIS_SET].content = l_match.str (1);
      }
      else
      {
        // method, eg b3lyp 
        m_result_set[CALC_METHOD].content = l_segment->str ();
        // Should be "normal" basis set in 6th segment
        ++l_segment;
        m_result_set[BASIS_SET].content = (l_segment->str ().empty ()? "unknown" : l_segment->str ());
      }
    }
  
    DLOG ("CALC_METHOD = " + m_result_set[CALC_METHOD].content + "\n");
    DLOG ("BASIS_SET = " + m_result_set[BASIS_SET].content + "\n");
    // Step from 6th to 13th
    std::advance (l_segment, 7);
    if (not l_segment->str ().empty ())
    {
      set_error (a_filter, "Non-empty string at split archive [13].");
      Result = CCHEM_SUPPLIER_BADDATA;
    }
    else
    {
      // Step to 14th
      ++l_segment;
      // Title
      m_result_set[TITLE].content = (l_segment->str ().empty ()? "unknown" : l_segment->str ());
      DLOG ("TITLE = " + m_result_set[TITLE].content + "\n");
      // "blank" before compounds's structure definition
      ++l_segment;
    }
    if (CCHEM_SUPPLIER_COMPLETE == Result and not l_segment->str ().empty ())
    {
      set_error (a_filter, "Non-empty string at split archive [15].");
      Result = CCHEM_SUPPLIER_BADDATA;
    }
    else
    {
      // Step to 16th
      ++l_segment;
      // Call method to process geometry
      Result = process_geometry (a_filter, l_segment, l_end_segment);
    }
    if (CCHEM_SUPPLIER_COMPLETE == Result)
    {
      // What we have now is a section of name=value pairs. We can loop over this.
      for (++l_segment;		// Step off version
  	 l_segment != l_end_segment and not l_segment->str ().empty (); ++l_segment)
      {
        // Position of '=' in 'name=value' segments.
        const unsigned int l_nv_pos = l_segment->str ().find ("=");
        // If no '=' then in error
        if (std::string::npos == l_nv_pos)
        {
          DLOG ("Name=Value without '=' " + l_segment->str () + "\n");
  	set_error (a_filter, "All text in archive pair section must have '='");
  	Result = CCHEM_SUPPLIER_BADDATA;
  	break;
        }
        else
        {
  	// We only want STATE, PG, NImag, HF and MP2
  	const std::string l_label (l_segment->str ().substr (0, l_nv_pos));
  	const std::string l_state ("State");
  	const std::string l_pg ("PG");
  	const std::string l_nimag ("NImag");
  	const std::string l_hf ("HF");
  	const std::string l_mp2 ("MP2");
  	if (l_state == l_label)
  	{
  	  m_result_set[STATE].content.assign (l_segment->str ().substr (l_nv_pos + 1));
  	}
  	else if (l_pg == l_label)
  	{
  	  m_result_set[POINT_GROUP].content.assign (l_segment->str ().substr (l_nv_pos + 1));
  	}
  	else if (l_nimag == l_label)
  	{
  	  m_result_set[NIMAG].content.assign (l_segment->str ().substr (l_nv_pos + 1));
  	}
  	else if (l_hf == l_label)
  	{
  	  m_result_set[ENERGY_HF].content.assign (l_segment->str ().substr (l_nv_pos + 1));
  	  m_result_set[ENERGY_HF].scale.assign ("hartree");
  	}
  	else if (l_mp2 == l_label)
  	{
  	  m_result_set[ENERGY_MP2].content.assign (l_segment->str ().substr (l_nv_pos + 1));
  	  m_result_set[ENERGY_MP2].scale.assign ("hartree");
  	}
        }
      }
    }
  }
  catch (const std::exception & a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  // END
  return Result;

}

/**
 * Process the geometry segments of the archive section.
 * 
 * \\ POST a_begin is at Version segment
 * \return CCHEM_SUPPLIER_COMPLETE on success.
 */
template < class t_iter > int state_a::process_geometry(::basic_filter_traits * a_filter, t_iter & a_begin, t_iter a_end) 
{
  // LOCAL
  // Static assertion
  int Result (CCHEM_SUPPLIER_COMPLETE);
  // PRE
  COMPCHEM_REQUIRE (a_begin != a_end, "process_geometry method needs data");
  
  // The charge and multiplicity come from the first arc_line
  {
    const std::string l_phrase (a_begin->str ());
    const unsigned int pos = l_phrase.find (",");
    if (std::string::npos != pos)
    {
      m_result_set[GEOM_CHARGE].content.assign (l_phrase.substr (0, pos));
      m_result_set[GEOM_MULT].content.assign (l_phrase.substr (pos + 1));
    }
    else
    {
      Result = CCHEM_SUPPLIER_BADDATA;
      set_error (a_filter, "Unable to determine the charge or multiplicity for this structure");
    }
  }
  
  if (CCHEM_SUPPLIER_COMPLETE == Result)
  {
    // Now we need to get the geometry data 
    ++a_begin;
  
    // start collecting geometry info
    std::stringstream l_geom_stream;
    l_geom_stream << a_begin->str ();
    for (++a_begin; a_begin != a_end and not a_begin->str ().empty (); ++a_begin)
    {
      l_geom_stream << "\\" << a_begin->str ();
    }
    // Check for zmt or version
    ++a_begin;
    if (not a_begin->str ().empty ()and std::string::npos != a_begin->str ().find ("Version"))
    {
      // a_begin is at Version string -> Appears to be XYZ format
      m_result_set[GEOM_DATA].format.assign ("axyz");
    }
    else
    {
      // Have more geometry -> ZMT format;
      m_result_set[GEOM_DATA].format.assign ("alad");
      // Continue collecting...
      l_geom_stream << "\\";
      for (++a_begin; a_begin != a_end && not a_begin->str ().empty (); ++a_begin)
      {
        l_geom_stream << "\\" << a_begin->str ();
      }
      // Step to Version string: Note there is sometimes extra empty lines at the end
      // of the geometry, this will loop to the end of the section. 
      for (; a_begin != a_end and std::string::npos == a_begin->str ().find ("Version"); ++a_begin)
      {
      }				// do nothing in loop
    }
    // Add content to node
    m_result_set[GEOM_DATA].content.assign (l_geom_stream.str ());
  }
  DLOG ("At [" + (a_begin->str ()) + "]\n");
  
  // Test we are not at the end-point.
  if (a_begin == a_end)
  {
    set_error (a_filter, "After processing the geometry we should not be at the end.");
    Result = CCHEM_SUPPLIER_BADDATA;
  }
  return Result;

}

/**
 * Find the dipole and extract into state.
 */
int state_a::process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  try
  {
    // DO
    switch (m_state)
    {
    case DIPOLE_SEARCH:
      {
        Result = process_dipole (a_filter, a_buffer, a_sz);
        if (CCHEM_SUPPLIER_COMPLETE == Result)
        {
  	Result = CCHEM_SUPPLIER_SUCCESS;
  	m_state = ARCHIVE_SEARCH;
        }
      }
      break;
    case ARCHIVE_SEARCH:		// Searching for first line of archive
      if (70 <= a_sz		// We assume archive will be more than one line, so skip short lines
  	&& boost::regex_search (a_buffer, a_buffer + a_sz, m_start_re))
      {
        // cache line (stripping whitespace) and indicate search for end of archive
        m_state = ARCHIVE_END_SEARCH;
        m_archive.write (a_buffer + 1, 70);
      }
      break;
    case ARCHIVE_END_SEARCH:	// Searching for last line of archive
      //cache line (stripping whitespace)
      m_archive.write (a_buffer + 1, std::min (70, a_sz - 1));
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_end_re))
      {
        m_state = TERM_SEARCH;
      }
      break;
    case TERM_SEARCH:
      {
        Result = process_termination (a_filter, a_buffer, a_sz);
        if (CCHEM_SUPPLIER_COMPLETE == Result)
        {
  	// Convert the archive section now we know the calculation terminated normally. 
  	Result = process_archive (a_filter);
  	m_state = COMPLETED_SEARCH;
        }
      }
      break;
    case COMPLETED_SEARCH:	//Fall through to error
    default:
      // error
      {
        // The default case is an error
        Result = CCHEM_SUPPLIER_FATAL;
        set_error (a_filter, "Unknown or invalid state in gaussian_summary filter");
      }
      break;
    }
  }
  catch (const std::exception & a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  
  catch (...)
  {
    set_error (a_filter, "Unknown error");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  
  // END
  return Result;

}

state_a::state_a()
  : gaussian_summary (DIPOLE_SEARCH)
  , m_archive ()
  , m_start_re ("1\\\\1\\\\")
  , m_end_re ("@$") {}


/**
 * Find the various pieces as the file progresses.
 */
int state_b::process_default(::basic_filter_traits *const a_filter, char const *const a_buffer, int a_sz) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  try
  {
  // DO
  // The b filter is called when there will not be a valid
  // archive at the end of the file. The filter therefore needs to get
  // as much information as possible from beginning section of the
  // file.
  
  // The strings passed to the function contains the header section of
  // the Gaussian file. This is everything from the first line to the
  // line beginning "Leave Link 101...". We search through it
  // gathering bits of information as we go. We use a simple case
  // statement to help order the sequence of events. Note that this
  // really has no error detection, so failure to find one part might
  // cause subsequent sections to fail. Hopefully, this section of the
  // output file is reasonable ordered so this should never be a
  // problem.
  
  // When m_state == 7 then we have finished the header section
    DLOG ("[");
    DLOG (m_state);
    DLOG ("]: ");
    DLOG (a_buffer);
    DLOG ("\n");
    switch (m_state)
    {
    case VERSION_SEARCH:
      // In this state we are searching for the version information indicator which
      // starts with a series of "******" stars.
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_version_re))
      {
        m_state = COMMAND_SEARCH;
      }
      break;
    case COMMAND_SEARCH:		// fall through
      // We have got the version indicator now we are looking for
      // the command line(s) indicator, this starts with the next line of
      // "------------"
    case COMMAND_FOUND:		// fall through
      // We are in the gaussian command line(s), now search for the end of the command line
      // before searching for the title.
    case TITLE_SEARCH:
      // We are afte the gaussian command line(s), now search for the indicator line
      // for the title.
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_title_divider_re))
      {
        ++m_state;
      }
      break;
    case TITLE_FOUND:
      // We are in the gaussian title line. Lets keep it.
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_title_divider_re))
      {
        // Found second divider line, end of title line(s)
        DLOG ("TITLE = " + m_result_set[TITLE].content);
        m_state = BASIS_SEARCH;
      }
      else
      {
        // We have the title string now, lets store it in the results
        m_result_set[TITLE].content.append (a_buffer, a_sz);
      }
      break;
    case BASIS_SEARCH:
      // Finished parsing the header section.
      // Find the basis-set by looking for the string "Standard basis"
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_basis_re))
      {
        // Now we need to add the basis-set to the results
        std::vector < std::string > l_split_line;
        const std::string l_temp (a_buffer, a_sz);
        boost::split (l_split_line, l_temp, boost::is_space (), boost::token_compress_on);
        for (unsigned int l_index (3); l_index < l_split_line.size (); ++l_index)
        {
  	m_result_set[BASIS_SET].content.append (l_split_line[l_index]);
        }
        DLOG ("BASIS_SET = " + m_result_set[BASIS_SET].content);
        // Set to search for the method.
        m_state = METHOD_SEARCH;
      }
      break;
    case METHOD_SEARCH:
      // Find the method searching for "SCF Done" line
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_method_re))
      {
        // Now we need to add the method-set to the results
        std::vector < std::string > l_split_line;
        const std::string l_temp (a_buffer, a_sz);
        boost::split (l_split_line, l_temp, boost::is_space (), boost::token_compress_on);
        m_result_set[CALC_METHOD].content.assign (l_split_line[3]);
        DLOG ("CALC_METHOD = " + m_result_set[CALC_METHOD].content);
        // Search for dipole
        m_state = DIPOLE_SEARCH;
      }
      break;
    case TERM_SEARCH:
      {
        Result = process_termination (a_filter, a_buffer, a_sz);
        if (CCHEM_SUPPLIER_COMPLETE == Result)
        {
  	m_state = COMPLETED_SEARCH;
        }
      }
      break;
    case COMPLETED_SEARCH:	//Fall through to error
    default:
      {
        // The default case is an error
        set_error (a_filter, "Unknown or invalid state in gaussian_summary filter");
        Result = CCHEM_SUPPLIER_FATAL;
      }
      break;
    }
  }
  catch (const std::exception & a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  catch (...)
  {
    set_error (a_filter, "Unknown error.");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  // END
  return Result;

}

state_b::state_b()
  : gaussian_summary (VERSION_SEARCH)
  , m_basis_re ("standard basis", boost::regex::icase)
  , m_method_re ("SCF Done")
  , m_title_divider_re ("-----+", boost::regex::extended)
  , m_version_re ("\\*{20,}", boost::regex::extended)
  {
    m_result_set [CALC_TYPE].content.assign ("IRC");
  }



} // namespace supplier::filter

} // namespace supplier
