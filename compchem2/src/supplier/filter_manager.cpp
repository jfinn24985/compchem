// ------------------------------------------------------------
// class: filter_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/filter_manager.hpp"
#include "data_scheme/content_visitor.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/experiment.hpp"
#include "utility/filesystem.hpp"
#include "supplier/matcher_filter.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_access/persistence.hpp"
#include "supplier/plugin_filter.hpp"
#include "data_scheme/scoped_open.hpp"
#include "supplier/supplier_helper.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/project.hpp"

// Manual includes
#include "data_access/xml_persistence.hpp"
#include <memory>
#include <tr1/memory>
#include <sstream>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/token_iterator.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
// -
namespace supplier {

/**
 * Instantiate the filters contained in a_mechanism, ignoring 'detect' filters.
 * 
 * \pre not a_mechanism.empty
 * \post 0 != m_filter.count (a_mechanism)
 */
void filter_manager::discover_filters(const data_scheme::mechanism & a_mechanism) 
{
  COMPCHEM_REQUIRE(not a_mechanism.empty_filter (), "Can not discover_filters with an empty mechanism object.");
  // Get reference to our filter set
  boost::ptr_vector< basic_filter > & l_set (m_filters[a_mechanism.label ()]);
  // Generate filters from a_mechanism 
  for (data_scheme::mechanism::const_filter_iterator l_filter = a_mechanism.begin_filter ();
        l_filter != a_mechanism.end_filter ();
        ++l_filter)
  {
    // Instantiate filters, skipping 'detect' filters.
    if (l_filter->outcomes ().find (data_access::names::detect_filter_name ()) >= l_filter->outcomes ().size ())
    {
      l_set.push_back (make_filter (*l_filter).release ());
    }
  }
  

  COMPCHEM_ENSURE(0 != m_filters.count (a_mechanism.label ()), "Failed to add filter set for target mechanism.");
}

/**
 * Instantiate the 'detect' filters for all mechanisms of a_prog.
 * 
 * \post 0 != m_filter.count ('detect')
 */
void filter_manager::discover_filters(data_scheme::program::const_mechanism_iterator a_begin, data_scheme::program::const_mechanism_iterator a_end) 
{
  // Get reference to our filter set
  boost::ptr_vector< basic_filter > & l_detect_filters (m_filters[data_access::names::detect_filter_name ()]);
  // Create vector of 'detect' filters.
  for (data_scheme::program::const_mechanism_iterator l_mech = a_begin; l_mech != a_end; ++l_mech)
  {
    data_scheme::mechanism::const_filter_iterator l_filt (l_mech->find_filter (data_access::names::detect_filter_name ()));
    if (l_filt != l_mech->end_filter ())
    {
      // Try filter.
      l_detect_filters.push_back (make_filter (*l_filt).release ());
    }
  }
  

}

/**
 * Construct a basic_filter object from the a_filter XML element.
 * 
 * * Detect filter type [regex:plugin(:python)] and instantiate relevant
 * filter type.
 */
std::auto_ptr< basic_filter > filter_manager::make_filter(const data_scheme::filter & a_filter)

{
  std::auto_ptr< basic_filter > Result;
  if (a_filter.filter_type () == data_access::names::plugin_filter_name ())
  {
    Result.reset (new plugin_filter (a_filter));
  }
  else if (a_filter.filter_type () == data_access::names::regex_filter_name ())
  {
    Result.reset (new matcher_filter (a_filter));
  }
  return Result;

}

/**
 * The main operation function to update a_memo with a possibly new result 
 * file.
 * 
 * Internally this:
 * 1: determines the mechanism of a_path and sets it on a_memo
 * 2: loads the filters for the mechanism.
 * 3: creates a working filter set by removing filters that have already been applied
 *     to a_memo or do not supply outcomes in the project.
 * 4: calls apply_filters
 * 
 * \pre a_memo.href (implies a_memo.target_mechanism) or exists (a_path)
 * \post a_memo.href implies a_memo.target_mechanism
 */
void filter_manager::process(data_scheme::memo & a_memo, const boost::filesystem::path & a_path) 
{
  // LOCAL
  /////////////////////////
  // Check memo
  // Open and close a_memo if necessary
  data_scheme::scoped_open < data_scheme::memo > l_open_memo (a_memo);
  /////////////////////////
  // Determine mechanism
  
  // Only test for href and target mechanism if no 'detect' filter has been applied
  if (a_memo.filters ().find (data_access::names::detect_filter_name ()) >= a_memo.filters ().size ())
  {
    // Check for href, set it from a_path if memo is without a href.
    if (a_memo.href ().empty ())
    {
      COMPCHEM_REQUIRE (not a_path.empty () and boost::filesystem::exists (a_path)
               , "Attempt to add results without a result filename or result file does not exist.");
      // href path is the path of the input file relative to the memo/project/database file
      boost::filesystem::path l_here (a_memo.uri_path ());
      a_memo.href (utility::filesystem::relative_path (l_here, a_path).string ());
    }
  
    // Create dummy set of outcomes
    std::set < std::string > l_outs;
    l_outs.insert (data_access::names::detect_filter_name ());
  
    // Create vector of 'detect' filters.
    if (0 == m_filters.count (data_access::names::detect_filter_name ()))
    {
      const data_scheme::program & l_prog (a_memo.page ().section ().site ());
      discover_filters (l_prog.begin_mechanism (), l_prog.end_mechanism ());
    }
    // Call apply_filters with find_first = true to perform the test for finding the mechanism
    // and test that mechanism has been set.
    if (CCHEM_SUPPLIER_SUCCESS != apply_filters (a_memo, l_outs.begin (), l_outs.end (), m_filters[data_access::names::detect_filter_name ()].begin (), m_filters[data_access::names::detect_filter_name ()].end (), true) or
         a_memo.filters ().find (data_access::names::detect_filter_name ()) >= a_memo.filters ().size ())
    {
      // Could not find mechanism.
      throw supplier_error ("Can not detect the mechanism for file " + a_memo.href (), CCHEM_SUPPLIER_BADDATA);
    }
  }
   
  // Create the filter set if not already present.
  if (0 == m_filters.count (a_memo.target_mechanism ()))
  {
    const data_scheme::program & l_prog (a_memo.page ().section ().site ());
    COMPCHEM_CHECK (l_prog.find_mechanism (a_memo.target_mechanism ()) != l_prog.end_mechanism ()
           , "Invalid mechanism label, no matching mechanism object.");
    discover_filters (*(l_prog.find_mechanism (a_memo.target_mechanism ())));
    COMPCHEM_CHECK (0 != m_filters.count (a_memo.target_mechanism ()), "Failed to add filter for target mechanism.");
  }
  
  ////////////////////////////////////
  // Split real outcomes into a set
  std::set < std::string > l_outs;
  boost::split (l_outs, a_memo.page ().section ().outcomes (), boost::is_any_of (data_access::names::space ()));
  
  // Now perform the main processing.
  apply_filters (a_memo, l_outs.begin (), l_outs.end (), m_filters[a_memo.target_mechanism ()].begin (), m_filters[a_memo.target_mechanism ()].end (), false);
  

}

/**
 * This method update the content of a_project with the project's current 
 * outcome  set. This scans through the memo objects and updates their 
 * results. For example this could be done after a project has added a new 
 * outcome label.
 * 
 * Internally for each memo in a_project
 * 1a: if memo.href but not memo.target_mechanism then call process (memo, "")
 * 1b: else loads the filters for memo.target_mechanism.
 *   2: creates a working filter set by removing filters that have already been applied
 *     to memo or do not supply outcomes in the project.
 *   3: calls apply_filters
 */
void filter_manager::process(data_scheme::project & a_project) 
{
  const data_scheme::program & l_program (a_project.site ());
  // Split real outcomes into a set
  std::set < std::string > l_outs;
  boost::split (l_outs, a_project.outcomes (), boost::is_any_of (data_access::names::space ()));
  
  for (data_scheme::project::experiment_iterator l_expt_iter (a_project.begin_experiment ());
        l_expt_iter != a_project.end_experiment ();
        ++l_expt_iter)
  {
    for (data_scheme::experiment::memo_iterator l_memo_iter (l_expt_iter->begin_memo ());
         l_memo_iter != l_expt_iter->end_memo ();
         ++l_memo_iter)
    {
      // If memo is a unopenned src type, then open (and close) it.
      const bool l_open_memo (l_memo_iter->is_src () and not l_memo_iter->is_open ());
      if (l_open_memo)
      {
        l_memo_iter->read ();
      }
      // Only consider memo objects that have href set
      if (not l_memo_iter->href ().empty ())
      {
        // Check the mechanism is set.
        if (l_memo_iter->target_mechanism ().empty ())
        {
          // Do a full process on the memo to set the mechanism and filter. 
          process (*l_memo_iter, boost::filesystem::path ());
        }
        else
        {
          COMPCHEM_CHECK (l_program.find_mechanism (l_memo_iter->target_mechanism ()) != l_program.end_mechanism ()
                          , "Invalid mechanism label, no matching mechanism object.");
          // Ensure we have a filter set for the target_mechanism.
          if (0 == m_filters.count (l_memo_iter->target_mechanism ()))
          {
            discover_filters (*(l_program.find_mechanism (l_memo_iter->target_mechanism ())));
          }
          ////////////////////////////////////
          // Now perform the main processing.
          apply_filters (*l_memo_iter, l_outs.begin (), l_outs.end (), m_filters[l_memo_iter->target_mechanism ()].begin (), m_filters[l_memo_iter->target_mechanism ()].end (), false);
        }
      }
      if (l_open_memo)
      {
        l_memo_iter->close ();
      }
    }
  }

}

/**
 * This method applies filters to the file named in a_twiglet.href then
 * adds the results to a_twiglet. It limits the range a_filter_begin to
 * a_filter_end to those with outcomes contained in a_out_begin to
 * a_out_end.
 * This method operates in two modes.  When a_find_first is true it 
 * is being called to detect the mechanism of a_twiglet.href.  In this 
 * mode it stops processing when the first filter object returns 
 * COMPLETE. The results from the filter are added to a_twiglet as
 * usual, however the a_twiglet.target_mechanism is also set. When 
 * a_find_first is false then it will process the entire file and filters and
 * add all the results to a_twiglet.
 */
int filter_manager::apply_filters(data_scheme::memo & a_twiglet, std::set< std::string >::const_iterator a_out_begin, std::set< std::string >::const_iterator a_out_end, filter_manager::iterator a_filter_begin, filter_manager::iterator a_filter_end, bool a_find_first) throw(supplier_error) 
{
#line 500
  int Result (CCHEM_SUPPLIER_SUCCESS);
  
  class helper__                  // Used to search for matching outcomes and reading file.
  {
  public:
    static bool match_outcome (const std::string & a_match, const std::string & a_word)
    {
      return a_match.size () > a_match.find (a_word, 0);
    }
    static char generate (std::istream & a_os, unsigned int &count)
    {
      return (not a_os.eof ()? ++count, a_os.get () : '\0');
    }
  };
  
  ////////////////////////////////////
  // Create the list of usable filters
  std::set < std::string > l_done;
  boost::split (l_done, a_twiglet.filters (), boost::is_any_of (data_access::names::space ()));
  
  // Get the initialise string (may be empty)
  std::string l_init_phrase;
  if (a_find_first)
  {
    l_init_phrase.assign (a_twiglet.href ());
  }
  else
  {
    data_scheme::memo::const_datum_iterator l_iter (a_twiglet.find_datum (data_access::names::detect_filter_name ()));
    if (l_iter != a_twiglet.end_datum ())
    {
      std::stringstream l_collect;
      data_scheme::content_visitor l_vstr (l_collect);
      l_iter->visit (l_vstr);
      l_init_phrase.assign (l_collect.str ());
    }
  }
  
  // Select usuable filters and split into line-, file- and buffer-oriented.
  std::vector < std::tr1::shared_ptr < basic_filter > >l_byline, l_bybuffer, l_byfile, l_complete;
  // functor to use with shared_ptr to ensure that all used filters 
  // will be reset however the function is exited.
  boost::function< void (basic_filter*) > l_reseter (boost::bind (&basic_filter::reset, _1));
  typedef std::vector < std::tr1::shared_ptr < basic_filter > >::iterator iter_t;
  
  for (iterator l_cursor (a_filter_begin); l_cursor != a_filter_end; ++l_cursor)
  {
    // Ignore already applied filters and filters that can not provide desired outcomes.
    if (l_done.end () == l_done.find (l_cursor->label ())and a_out_end != std::find_if (a_out_begin, a_out_end, boost::bind (&helper__::match_outcome, l_cursor->outcomes (), _1)))
    {
      if (CCHEM_SUPPLIER_FILE_ORIENTED & l_cursor->flags ())
      {
        DLOG ("FILE FILTER = ");
        DLOG (l_cursor->identity ());
        DLOG ("\n");
        if (CCHEM_SUPPLIER_SUCCESS == l_cursor->initial_test (l_init_phrase))
        {
          std::tr1::shared_ptr< basic_filter > l_tmp (&*l_cursor, l_reseter);
          l_byfile.push_back (l_tmp);
        }
      }
      else if (CCHEM_SUPPLIER_BUFFER_ORIENTED & l_cursor->flags ())
      {
        DLOG ("BUFFER FILTER = ");
        DLOG (l_cursor->identity ());
        DLOG ("\n");
        if (CCHEM_SUPPLIER_SUCCESS == l_cursor->initial_test (l_init_phrase))
        {
          std::tr1::shared_ptr< basic_filter > l_tmp (&*l_cursor, l_reseter);
          l_bybuffer.push_back (l_tmp);
        }
      }
      else                        // default (CCHEM_SUPPLIER_LINE_ORIENTED & l_cursor->flags ())
      {
        DLOG ("LINE FILTER = ");
        DLOG (l_cursor->identity ());
        DLOG ("\n");
        if (CCHEM_SUPPLIER_SUCCESS == l_cursor->initial_test (l_init_phrase))
        {
          std::tr1::shared_ptr< basic_filter > l_tmp (&*l_cursor, l_reseter);
          l_byline.push_back (l_tmp);
        }
      }
    }
  }
  
  if (l_byfile.empty ()and l_byline.empty ()and l_bybuffer.empty ())
  {
    Result = CCHEM_SUPPLIER_UNUSABLE;
  }
  else
  {
    /////////////////////////////
    // Begin processing the file
    boost::filesystem::path l_tmp (a_twiglet.uri_path () / boost::filesystem::path(a_twiglet.href ()));
    DLOG ("Memo path = \"");
    DLOG(l_tmp.string ());
    DLOG("\"\n");
    l_tmp.normalize ();
    DLOG ("Memo path (norm) = \"");
    DLOG(l_tmp.string ());
    DLOG("\"\n");
    COMPCHEM_CHECK (boost::filesystem::exists (l_tmp), "Cannot locate target file for memo, failed path is " + l_tmp.string ());
    // Open the result file for buffer and line oriented filters
    std::ifstream l_file (l_tmp.native_file_string ().c_str ());
    COMPCHEM_CHECK (l_file, "Unable to open file for reading :" + l_tmp.string ());
  
    // Buffer size
    const static unsigned int l_sz = 63 * 1024;
#ifndef NO_LOG
    unsigned int d_count (0);
#endif
    // NEW REPLACE boost::scoped_array< char > l_line (new char[l_sz]);
    std::string l_line (l_sz, '\0');      // The buffer
    std::string l_odd;
    while (l_file)
    {
#ifndef NO_LOG
      ++d_count;
#endif
      // Get part of buffer an put into l_part.
      unsigned int l_readsz (0);
      // NEw INSERT
      std::generate_n (l_line.begin (), l_sz, boost::bind (&helper__::generate, boost::ref (l_file), boost::ref (l_readsz)));
      if (l_readsz < l_line.size ())
      {
        --l_readsz;
        l_line.resize (l_readsz); // Only resize on last read.
      }
      // NEW EXCERT
      // l_file.read (l_line.get (), l_sz);
      // const unsigned int l_readsz (l_file.gcount ());
      // NEW END
      if ((l_file.fail ()and not l_file.eof ())or l_readsz > l_sz)
      {
        break;
      }
      const bool l_eof (l_file.eof ()or l_readsz < l_sz);
      // Give to buffer oriented filters
      if (not l_bybuffer.empty ())
      {
        for (iter_t l_i (l_bybuffer.begin ()); l_i != l_bybuffer.end (); ++l_i)
        {
          // Perform processing, test result was not an error
          const int l_result ((l_i->get ())->process_part (l_line));       // NEW REPLACE .get (), l_readsz));
          switch (l_result)
          {
          case CCHEM_SUPPLIER_SUCCESS:   // Do nothing
            break;
          case CCHEM_SUPPLIER_COMPLETE:  // Add to complete and remove (fall through)
            l_complete.push_back (*l_i);
          case CCHEM_SUPPLIER_UNUSABLE:  // Remove filter
          {
            iter_t l_tmp (l_i);
            --l_i;
            l_bybuffer.erase (l_tmp);
          }
          break;
          default:               // eg CCHEM_SUPPLIER_FATAL
            throw supplier_error ((l_i->get ())->error_message (), l_result);
            break;
          }
        }
      }
      // Break out if we have a completed filter.
      if (a_find_first and not l_complete.empty ())
      {
        break;
      }
      // Give to line oriented filters
      if (not l_byline.empty ())
      {
        // NEW REPLACE std::istringstream l_part (std::string (l_line.get (), l_readsz));
        std::istringstream l_part (l_line);
#ifndef NO_LOG
        unsigned int l_pcount (0);
#endif
        bool l_not_eob (true);
        while (l_not_eob and l_part)
        {
          std::string l_mline;
          std::getline (l_part, l_mline);
          // Handle when end of l_part is not end of line.
          l_not_eob = (l_readsz > l_part.tellg ());
          if (not l_not_eob and not l_eof)
          {
#ifndef NO_LOG
            DLOG ("[");
            DLOG (l_pcount);
            l_pcount = l_part.tellg ();
            DLOG ("..");
            DLOG (l_pcount);
            DLOG ("] ");
            DLOG ("PART LINE = ");
            DLOG (l_mline);
            DLOG ("\n");
#endif
            // Save the last part-line for the next iteration.
            l_odd.append (l_mline);
          }
          else
          {
            if (not l_odd.empty ())
            {
              // Have saved part-line from last iteration, insert into l_mline
              l_mline.insert (0, l_odd);
              l_odd.clear ();
            }
            // Pass segment to each plugin
#ifndef NO_LOG
            DLOG ("[");
            DLOG (l_pcount);
            l_pcount = l_part.tellg ();
            DLOG ("..");
            DLOG (l_pcount);
            DLOG ("] ");
#endif
            DLOG ("line = ");
            DLOG (l_mline);
            DLOG ("\n");
            for (iter_t l_i (l_byline.begin ()); l_i != l_byline.end (); ++l_i)
            {
              // Perform processing, test result was not an error
              const int
              l_result (l_i->get ()->process_part (l_mline));
              switch (l_result)
              {
              case CCHEM_SUPPLIER_SUCCESS:       // Do nothing
                break;
              case CCHEM_SUPPLIER_COMPLETE:      // Add to complete and remove (fall through)
                l_complete.push_back (*l_i);
              case CCHEM_SUPPLIER_UNUSABLE:      // Remove filter
              {
                iter_t l_tmp (l_i);
                --l_i;
                l_byline.erase (l_tmp);
              }
              break;
              default:           // eg CCHEM_SUPPLIER_FATAL
                throw supplier_error (l_i->get ()->error_message (), l_result);
                break;
              }
            }
          }
        }
      }
      // Break out if we have a completed filter.
      if (a_find_first and not l_complete.empty ())
      {
        break;
      }
    }
#ifndef NO_LOG
    DLOG ("Processed file in ");
    DLOG (d_count);
    DLOG (" pieces.\n");
#endif
  }
  
  {
  // Process file oriented filters
    const std::string l_filename (a_twiglet.href ());
    for (iter_t l_i (l_byfile.begin ()); l_i != l_byfile.end (); ++l_i)
    {
      // Perform processing, test result was not an error
      const int l_result (l_i->get ()->process_part (l_filename));
      switch (l_result)
      {
      case CCHEM_SUPPLIER_SUCCESS:       // Do nothing
        break;
      case CCHEM_SUPPLIER_COMPLETE:      // Add to complete and remove (fall through)
        l_complete.push_back (*l_i);
      case CCHEM_SUPPLIER_UNUSABLE:      // Remove filter
      {
        iter_t l_tmp (l_i);
        --l_i;
        l_byfile.erase (l_tmp);
      }
      break;
      default:                   // eg CCHEM_SUPPLIER_FATAL
        throw supplier_error (l_i->get ()->error_message (), l_result);
        break;
      }
      // Break out if we have a completed filter.
      if (a_find_first and not l_complete.empty ())
      {
        break;
      }
    }
  }
  
  ////////////////////////////////////
  // Gather the results.
  if (not a_find_first)
  {
    // For non-detect runs merge used filters together.
    l_complete.insert (l_complete.end (), l_byline.begin (), l_byline.end ());
    l_complete.insert (l_complete.end (), l_bybuffer.begin (), l_bybuffer.end ());
    l_complete.insert (l_complete.end (), l_byfile.begin (), l_byfile.end ());
  }
  
  {
    // Get output
    DLOG ("Getting results.\n");
    for (iter_t l_i = l_complete.begin (); l_i != l_complete.end (); ++l_i)
    {
      // Add filter labels to memo
      a_twiglet.add_filter_label (l_i->get ()->label ());
      // LOCAL
      DLOG ("Getting result from ");
      DLOG (l_i->get ()->identity ());
      DLOG ("\n");
      if (l_i->get ()->flags () & CCHEM_SUPPLIER_STRING_OUTPUT)
      {
        std::string l_outputstr;
        const int
        l_result (l_i->get ()->output_string (l_outputstr));
        DLOG ("Result for filter [");
        DLOG (l_i->get ()->identity ());
        DLOG ("](status = ");
        DLOG (l_result);
        DLOG (") = ");
        DLOG (l_outputstr);
        DLOG ("\n");
        // Test result
        if (CCHEM_SUPPLIER_FATAL <= l_result)
        {
          // Ensure nul terminated
          throw
          supplier_error (l_i->get ()->error_message (), l_result);
        }
        else
        {
          if (not l_outputstr.empty ())
          {
            try
            {
              data_access::xml_persistence::read_string (a_twiglet, l_outputstr);
            }
            catch (const std::exception & a_err)
            {
              throw
              supplier_error (a_err.what (), CCHEM_SUPPLIER_BADDATA);
            }
          }
        }
      }
      else
      {
        // CCHEM_SUPPLIER_DIRECT_OUTPUT
        const int
        l_result (l_i->get ()->output_direct (a_twiglet));
        DLOG ("Result for filter [");
        DLOG (l_i->get ()->identity ());
        DLOG ("](status = ");
        DLOG (l_result);
        DLOG (") = DIRECT\n");
        // Test result
        if (CCHEM_SUPPLIER_FATAL <= l_result)
        {
          throw
          supplier_error (l_i->get ()->error_message (), l_result);
        }
      }
      if (a_find_first)
      {
        // We also set the mechanism here
        a_twiglet.target_mechanism (l_i->get ()->mechanism_label ());
      }
    }
  }
  
  return Result;

}

/**
 * Construct a filter manager that uses a configuration file for finding 
 * filetypes and filters.
 * 
 * \param a_conf_filename - The name of the configuration file to use.
 */
filter_manager::filter_manager()
: m_filters ()
{
  

}


} // namespace supplier
