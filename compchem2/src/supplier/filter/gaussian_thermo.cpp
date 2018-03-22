// ------------------------------------------------------------
// class: gaussian_thermo
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


// Stop logging
#define NO_LOG
#include "supplier/filter/gaussian_thermo.hpp"
#include "supplier/filter/datum_proxy.hpp"
#include "supplier/supplier_error.hpp"
#include "supplier/basic_filter_traits.hpp"

// Manual include
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sstream>
// --
// The symbol retrieved by the filter manager
extern "C" {
basic_filter_traits plugin_object = {
      "Gaussian Inc log thermochemistry filter (C) 2008 V1.1.0"
      , NULL
      , NULL
      , &supplier::filter::gaussian_thermo::initial_test
      , &supplier::filter::gaussian_thermo::process
      , &supplier::filter::gaussian_thermo::output
      , &supplier::filter::gaussian_thermo::finalise
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
int gaussian_thermo::initial_test(::basic_filter_traits * a_filter, char const *const a_phrase, int a_sz)

{
  COMPCHEM_REQUIRE(NULL != a_filter, "Can not initialise a nul object.");
  COMPCHEM_REQUIRE(NULL == a_filter->m_internal_state, "Can not initialise a previously initialised object.");
  int Result (CCHEM_SUPPLIER_UNUSABLE);
  try
  {
    boost::regex l_command_re ("(freq[[:space:](=]|[[:space:](,]calcall[[:space:]),])", boost::regex::extended | boost::regex::icase);
    // Test Gaussian "command" phrase matches our test regular expression
    if (boost::regex_search (a_phrase, a_phrase + a_sz, l_command_re))
    {
      a_filter->m_internal_state = new gaussian_thermo;
      Result = CCHEM_SUPPLIER_SUCCESS;
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

int gaussian_thermo::output_default(::basic_filter_traits * a_filter, void * a_cb_data) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  // DO
  // Check if data was found
  if (2 != m_found || 10 > m_lines.size ())
  {
    std::stringstream l_err;
    l_err << "Unable to extract thermochemistry\n";
    l_err << "Found flag = " << m_found << ",\n";
    l_err << "Number of lines found = " << m_lines.size () << "\n";
    set_error (a_filter, l_err.str ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  else
  {
    // Generate output twiglet proxies
    datum_proxy l_thermo ("thermochemistry");
    datum_proxy l_zpve ("zero-point");
    datum_proxy l_zpcorr ("zero-point-corr");
    datum_proxy l_ecorr ("energy-corr");
    datum_proxy l_enth ("enthalpy-corr");
    datum_proxy l_free ("free-energy-corr");
    datum_proxy l_entropy ("entropy");
    // == Start processing cached lines ==
    std::vector < std::string >::iterator l_curs (m_lines.begin ());
    std::vector < std::string > l_words;
  
    // Extract zero point energy
    if (l_curs == m_lines.end ())
    {
      set_error (a_filter, "List begin = end for non-empty list.");
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else
    {
      boost::split (l_words, *l_curs, boost::is_space (), boost::token_compress_on);
      if (5 <= l_words.size ())
      {
        DLOG ("zero-point = [" + l_words.at (4) + "] from line = [" + *l_curs + "]");
        l_zpve.content.assign (l_words.at (4));
      }
      else
      {
        set_error (a_filter, "Processing thermochemistry failed, no zero-point found\n in line [" + *l_curs + "]\n");
        Result = CCHEM_SUPPLIER_FATAL;
      }
    }
    // Step along cached lines to zero-point correction
    if (CCHEM_SUPPLIER_SUCCESS == Result)
    {
      for (++l_curs;		// Step off first line
  	 l_curs != m_lines.end () && !boost::regex_search (*l_curs, m_start); ++l_curs)
      {
      }				// Do nothing in loop
  
      // Extract zero-point correction
      if (l_curs == m_lines.end ())
      {				// Check if at end of cached line
        set_error (a_filter, "Processing thermochemistry failed, no zero-point correction found\n");
        Result = CCHEM_SUPPLIER_FATAL;
      }
      else
      {
        boost::split (l_words, *l_curs, boost::is_space (), boost::token_compress_on);
        if (4 <= l_words.size ())
        {
  	DLOG ("zero-point corr = [" + l_words.at (3) + "] from line = [" + *l_curs + "]");
  	l_zpcorr.content.assign (l_words.at (3));
        }
        else
        {
  	std::stringstream l_err;
  	l_err << "Processing thermochemistry failed, no zero-point correction found\n";
  	l_err << " in line [" << *l_curs << "]\n";
  	set_error (a_filter, l_err.str ());
  	Result = CCHEM_SUPPLIER_FATAL;
        }
      }
    }
  
    // Extract energy correction
    if (CCHEM_SUPPLIER_SUCCESS == Result)
    {
      ++l_curs;
      if (l_curs == m_lines.end ())
      {				// Check if at end of cached line
        set_error (a_filter, "Processing thermochemistry failed, no energy correction found\n");
        Result = CCHEM_SUPPLIER_FATAL;
      }
      else
      {
        boost::split (l_words, *l_curs, boost::is_space (), boost::token_compress_on);
        if (6 <= l_words.size ())
        {
  	DLOG ("energy corr = [" + l_words.at (5) + "] from line = [" + *l_curs + "]");
  	l_ecorr.content.assign (l_words[5]);
        }
        else
        {
  	std::stringstream l_err;
  	l_err << "Processing thermochemistry failed, no energy correction found\n";
  	l_err << " in line [" << *l_curs << "]\n";
  	set_error (a_filter, l_err.str ());
  	Result = CCHEM_SUPPLIER_FATAL;
        }
      }
    }
  
    // Extract enthalpy correction
    if (CCHEM_SUPPLIER_SUCCESS == Result)
    {
      ++l_curs;
      if (l_curs == m_lines.end ())
      {				// Check if at end of cached line
        set_error (a_filter, "Processing thermochemistry failed, no enthalpy correction found\n");
        Result = CCHEM_SUPPLIER_FATAL;
      }
      else
      {
        boost::split (l_words, *l_curs, boost::is_space (), boost::token_compress_on);
        if (6 <= l_words.size ())
        {
  	DLOG ("enthalpy corr = [" + l_words.at (5) + "] from line = [" + *l_curs + "]");
  	l_enth.content.assign (l_words.at (5));
        }
        else
        {
  	std::stringstream l_err;
  	l_err << "Processing thermochemistry failed, no enthalpy correction found\n";
  	l_err << " in line [" << *l_curs << "]\n";
  	set_error (a_filter, l_err.str ());
  	Result = CCHEM_SUPPLIER_FATAL;
        }
      }
    }
  
    // Extract energy correction
    if (CCHEM_SUPPLIER_SUCCESS == Result)
    {
      ++l_curs;
      if (l_curs == m_lines.end ())
      {				// Check if at end of cached line
        set_error (a_filter, "Processing thermochemistry failed, no free energy correction found\n");
        Result = CCHEM_SUPPLIER_FATAL;
      }
      else
      {
        boost::split (l_words, *l_curs, boost::is_space (), boost::token_compress_on);
        if (8 <= l_words.size ())
        {
  	DLOG ("free energy corr = [" + l_words.at (7) + "] from line = [" + *l_curs + "]");
  	l_free.content.assign (l_words.at (7));
        }
        else
        {
  	std::stringstream l_err;
  	l_err << "Processing thermochemistry failed, no free energy correction found\n";
  	l_err << " in line [" << *l_curs << "]\n";
  	set_error (a_filter, l_err.str ());
  	Result = CCHEM_SUPPLIER_FATAL;
        }
      }
    }
  
    // Step along cached lines to entropy
    if (CCHEM_SUPPLIER_SUCCESS == Result)
    {
      boost::regex l_entropy_find ("cal/mol-kelvin", boost::regex::icase);
      for (++l_curs; l_curs != m_lines.end () && not boost::regex_search (*l_curs, l_entropy_find); ++l_curs)
      {
      }				// Do nothing in loop
      // Step to "Total" lines
      for (; l_curs != m_lines.end () && std::string::npos == l_curs->find ("otal"); ++l_curs)
      {
      }				// Do nothing in loop
      // Extract entropy correction
      if (l_curs == m_lines.end ())
      {				// Check if at end of cached line
        set_error (a_filter, "Processing thermochemistry failed, no entropy found\n");
        Result = CCHEM_SUPPLIER_FATAL;
      }
      else
      {
        boost::split (l_words, *l_curs, boost::is_space (), boost::token_compress_on);
        if (5 <= l_words.size ())
        {
  	DLOG ("Entropy = [" + l_words.at (4) + "] from line = [" + *l_curs + "]");
  	l_enth.content.assign (l_words.at (4));
        }
        else
        {
  	std::stringstream l_err;
  	l_err << "Processing thermochemistry failed, no entropy found\n";
  	l_err << " in line [" << *l_curs << "]\n";
  	set_error (a_filter, l_err.str ());
  	Result = CCHEM_SUPPLIER_FATAL;
        }
      }
    }
  
    // Add results to twiglet if no error
    if (CCHEM_SUPPLIER_SUCCESS == Result)
    {
      // Wrap twiglet
      const std::string l_hartree_label ("hartree");
      // zero point enrgy node
      l_zpve.scale.assign ("jmol");
      // zero point energy correction node
      l_zpcorr.scale.assign (l_hartree_label);
      // raw energy correction node
      l_ecorr.scale.assign (l_hartree_label);
      // enthalpy correction node
      l_enth.scale.assign (l_hartree_label);
      // free energy correction node
      l_free.scale.assign (l_hartree_label);
      // entropy node
      l_entropy.scale.assign ("cal-molK");
  
      void * l_thermo_node (l_thermo.output (a_filter, a_cb_data));
      l_zpve.output (a_filter, l_thermo_node);
      l_zpcorr.output (a_filter, l_thermo_node);
      l_ecorr.output (a_filter, l_thermo_node);
      l_enth.output (a_filter, l_thermo_node);
      l_free.output (a_filter, l_thermo_node);
      l_entropy.output (a_filter, l_thermo_node);
      DLOG ("Made thermo twig");
    }
  }
  
  return Result;

}

/**
 * Process a_segment of the file.
 */
int gaussian_thermo::process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz) 
{
  // LOCAL
  int Result (CCHEM_SUPPLIER_SUCCESS);
  //DO
  // short cut if 2 = m_energy_found
  if (2 != m_found)
  {
    if (m_found == 0 && boost::regex_search (a_buffer, a_buffer + a_sz, m_start))
    {
      // start grabbing lines
      m_found = 1;
    }
    if (1 == m_found)
    {
      if (boost::regex_search (a_buffer, a_buffer + a_sz, m_stop))
      {
        // end grabbing lines
        m_found = 2;
      }
      else
      {
        // add line
        m_lines.push_back (std::string (a_buffer, a_sz));
      }
    }
  }
  // DONE
  return Result;

}


} // namespace supplier::filter

} // namespace supplier
