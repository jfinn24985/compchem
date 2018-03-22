// ------------------------------------------------------------
// class: gaussian_detect
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#define NO_LOG
#include "supplier/filter/gaussian_detect.hpp"
#include "supplier/supplier_error.hpp"
#include "supplier/basic_filter_traits.hpp"

// Manual includes
#include <boost/algorithm/string/trim.hpp>
#include <sstream>
// --
// --
// The symbol retrieved by the filter manager
extern "C" {
struct basic_filter_traits plugin_object = {
      "Gaussian Inc. Output file type detect filter (C) 2008, V1.1.0"
      , NULL // error message
      , NULL // state object
      , &supplier::filter::gaussian_detect::initial_test
      , &supplier::filter::gaussian_detect::process
      , &supplier::filter::gaussian_detect::output
      , &supplier::filter::gaussian_detect::finalise
      , CCHEM_SUPPLIER_BUFFER_ORIENTED | CCHEM_SUPPLIER_DIRECT_OUTPUT
      , NULL // output_cb
      , NULL // create datum cb
      , NULL // add contect cb
      , NULL // set format cb
      , NULL // set ordinal cb
      , NULL // set scale cb
    };
}
namespace supplier {

namespace filter {

/**
 * Error message when getting results before processing a file.
 */
const char* const gaussian_detect::s_no_result("Attempt to get result before a successfully processing a file.");

/**
 * Main ctor.
 */
gaussian_detect::gaussian_detect() 
: m_route ()
, m_subversion ()
, m_version ()
{}

/**
 * Initialise the plugin object. Creates the internal state object.
 * 
 * \pre nul /= a_plugin
 */
int gaussian_detect::initial_test(::basic_filter_traits * a_filter, char const *const a_filename, int a_sz)

{
  COMPCHEM_REQUIRE(NULL != a_filter, "Nul plugin object passed as argument to process.");
  int Result (CCHEM_SUPPLIER_SUCCESS);
  try
  {
    if (NULL == a_filter)
    {
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else if (NULL != a_filter->m_internal_state)
    {
      set_error (a_filter, "Attempt to initialise an already initialised filter.");
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else
    { 
      a_filter->m_internal_state = static_cast< void * >(new gaussian_detect ());
    }
  }
  catch (const supplier_error &a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = a_err.level;
  }
   catch (const std::exception &a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  catch (...)
  {
    set_error (a_filter, "Unknown error");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  return Result;

}

int gaussian_detect::output_default(::basic_filter_traits *const a_filter, void * a_cb_data) 
{
  // plugin = a_filter,
  // top (memo) = a_cb_data
  int Result (CCHEM_SUPPLIER_SUCCESS);
  if (m_version.empty () or m_route.empty ())
  {
    Result = CCHEM_SUPPLIER_FATAL;
    a_filter->m_error = s_no_result;
  }
  else
  {
    DLOG ("Entering main part of output_direct.\n");
    COMPCHEM_CHECK (NULL != a_filter->m_create_datum_cb, "Create datum callback is nul");
    COMPCHEM_CHECK (NULL != a_filter->m_add_content_cb, "Add content callback is nul");
    // Create the top-level datum
    const std::string l_detect ("detect");
    void * l_top_child ((*a_filter->m_create_datum_cb)(a_cb_data, l_detect.data (), l_detect.size ()));
    // Create the version datum
    const std::string l_version ("version");
    void * l_child ((*a_filter->m_create_datum_cb)(l_top_child, l_version.data (), l_version.size ()));
    // Set text on the child datum.
    (*a_filter->m_add_content_cb)(l_child, m_version.data (), m_version.size ());
    // Create the subversion datum
    const std::string l_subversion ("subversion");
    l_child = (*a_filter->m_create_datum_cb)(l_top_child, l_subversion.data (), l_subversion.size ());
    // Set text on the second child datum.
    (*a_filter->m_add_content_cb)(l_child, m_subversion.data (), m_subversion.size ());
    // Add route text to top-level datum
    (*a_filter->m_add_content_cb)(l_top_child, m_route.data (), m_route.size ());
    DLOG ("Exiting main part of output_direct.\n");
  }
  return CCHEM_SUPPLIER_SUCCESS;

}

/**
 * This checks that the file is really a gaussian file and sets 
 * the filetype and subtype information.
 * 
 * \return SUCCESS if a Gaussian output, UNUSABLE if not.
 */
int gaussian_detect::process_default(::basic_filter_traits * a_filter, char const *const a_buffer, int a_sz) 
{
  // LOCAL
  // This routine gets the header section of the 'gaussian' file and
  // finds the section of text that was the original 'gaussian'
  // control statement. We then pass this statement to each filter and
  // ask them if they can handle it.
  //
  // The first line of the file should read (XX is version):
  //  Entering Gaussian System, Link 0=gXX
  // The version line is something like:
  //  Gaussian 98:  SGI64-G98RevA.7 11-Apr-1999
  //  Gaussian 03:  DEC-AXP-OSF/1-G03RevB.03 4-May-2003
  // The command section is follows the third set of "------+" and ends with
  // another line of "-----+"
  boost::regex l_version_re ("Gaussian ([0-9]*):  [^ ]{1,30}-G\\1Rev[^ ]* [[:digit:]]{1,2}-[[:alpha:]]{3}-[[:digit:]]{4}"); // Version is match 0, subtype is "g" + match 1
  boost::cmatch l_tmp;
  DLOG("Entering gaussian_detect\n");
  const std::string d_tmp (a_buffer, std::min (80, a_sz));
  DLOG("First part of buffer is :"); DLOG(d_tmp); DLOG ("\n");
  if (boost::regex_search< const char * >(a_buffer, a_buffer + a_sz, l_tmp, l_version_re))
  {
    DLOG ("Match found to version re: ");DLOG(l_tmp[0]);DLOG("\n");
    m_version.assign (l_tmp[0]);
    m_subversion.assign ("g");
    m_subversion.append (l_tmp[1]);
  }
  boost::regex l_route_re ("(-{4,80})(.+?)\\1"); // Route is match 2 of second match
  int l_count (0);
  for (boost::regex_iterator< const char * > l_M (a_buffer, a_buffer + a_sz, l_route_re); 
       l_M != boost::regex_iterator< const char * > (); ++l_M)
  {
    DLOG ("Match found to route re: ");DLOG((*l_M)[0]);DLOG("\n");
    if (1 == l_count)
    {
      std::istringstream l_is ((*l_M)[2]);
      if (not m_route.empty ()) m_route.clear ();
      while (l_is)
      {
        std::string l_tmp;
        std::getline (l_is, l_tmp);
        unsigned int l_start (0);
        unsigned int l_end (l_tmp.size ());
        if (' ' == l_tmp [l_start]) ++l_start;
        if (' ' == l_tmp [l_end - 1]) --l_end;
        m_route.append (l_tmp.substr (l_start, l_end - l_start));
      }
      break;
    }
    ++l_count;
  }
  DLOG ("VERSION: " + m_version);DLOG("\n");
  DLOG ("COMMAND: " + m_route);DLOG("\n");
  // If we have not found the version and route in the first buffer, then the detect fails...
  return (not m_version.empty () and not m_route.empty ()) ? CCHEM_SUPPLIER_COMPLETE : CCHEM_SUPPLIER_UNUSABLE;

}


} // namespace supplier::filter

} // namespace supplier
