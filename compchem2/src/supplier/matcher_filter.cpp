// ------------------------------------------------------------
// class: matcher_filter
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/matcher_filter.hpp"
#include "data_scheme/content_visitor.hpp"
#include "supplier/matcher_types.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/memo.hpp"

// Manual includes
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
// -
namespace supplier {

/**
 * Information that identifies the filter/parser. This will
 * be placed into log messages from the manager and not in
 * the database. A typical message might contain a
 * descriptive name, version and author, eg.
 * "Nibble filter for Froggles: version 0.001: 2020/12/25: by P.J. Teeps"
 */
std::string matcher_filter::identity() const 
{
  return get_filter ().href ();
}

/**
 * Parse a_line of text and create a matcher. Throw an error if 
 * a_line is invalid.
 * 
 * type,mode,match,format
 * 
 * Where
 * type = ('byte'|'lines'|'string'|'regex')
 * match = (string|integer)
 * format = string // Only allowed for string or regex type.
 * mode = ('first'|'last'|'accum') // Only allowed when format exists.
 * 
 * Up to three lines may be present which have the following meaning
 * 
 * 1 line: The 'main' matcher, this is used to generate results so it must 
 * have a format string.
 * 2 lines: The first line represents a 'start' indicator. When it matches
 * it is immediately replaced with a 'main' matcher generated from the 
 * following line (they get the matching line/buffer). It can contribute to
 * the final result if it has a format string. The second line is then the
 * 'main' matcher and must have a format string.
 * 3 lines: As for 2 lines plus the third line is an 'end' indicator. When it
 * matches the regex_filter returns 'COMPLETE' and further processing
 * is not necessary. Note this 'end' indicator is tested after the 'main'
 * matcher.
 * 
 * The format string should not include the closing XML node ("</datum>")
 * this is automatically added.
 * 
 * Programming note.  It was decided that this method should be part of 
 * matcher_filter not basic_matcher because the major function is to
 * decode the filter's description, which is really independent of the 
 * matcher types.
 */
basic_matcher * matcher_filter::make_matcher(const std::string & a_line, matcher_filter::mode_constant & a_mode) 
{
  // Set a_mode to default (first)
  a_mode = first;
  // Helper class to provide two internal conversion functions.
  class helper__
  {
  public:
    static int convert_type (std::string a_type)
    {
      COMPCHEM_REQUIRE (not a_type.empty (), "Invalid matcher phrase with no type");
      boost::to_lower (a_type); // Convert to lower case
      const int invalid_string (-1);
      int valid_matcher_type_string (invalid_string);
      if ("byte" == a_type) valid_matcher_type_string = 0;
      else if ("line" == a_type) valid_matcher_type_string = 1;
      else if ("string" == a_type) valid_matcher_type_string = 2;
      else if ("regex" == a_type) valid_matcher_type_string = 3;
      COMPCHEM_ALWAYS (valid_matcher_type_string != invalid_string, "Invalid matcher type \"" + a_type +  "\", can be 'byte', 'line', 'string' or 'regex'");
      return valid_matcher_type_string;
    }
    static mode_constant convert_mode (std::string a_mode_str)
    {
      COMPCHEM_REQUIRE (not a_mode_str.empty (), "Invalid matcher phrase with no type");
      boost::to_lower (a_mode_str); // Convert to lower case
      mode_constant Result;
      bool valid_mode_string (false);
      if ("first" == a_mode_str) { Result = first; valid_mode_string = true; }
      else if ("last" == a_mode_str) { Result = last; valid_mode_string = true; }
      else if ("accum" == a_mode_str) { Result = accum; valid_mode_string = true; }
      COMPCHEM_ALWAYS (valid_mode_string, "Invalid matcher mode \"" + a_mode_str +  "\", can be 'first', 'last' or 'accum'");
      return Result;
    }
  };
  //////////////////////
  // Split the content into tokens and process
  std::auto_ptr< basic_matcher > Result;
  boost::tokenizer < boost::escaped_list_separator < char > > l_defn_tokenizer (a_line);
  boost::tokenizer < boost::escaped_list_separator < char > >::iterator l_defn_iter (l_defn_tokenizer.begin ());
  COMPCHEM_ALWAYS (l_defn_iter != l_defn_tokenizer.end () and not l_defn_iter->empty ()
                   , "Invalid filter definition, is empty or has no type text.");
  // First element should be a type.
  // 'byte'|'lines'|'string'|'regex'
  
  switch (helper__::convert_type (*l_defn_iter))
  {
  case 0:
  {
    // Byte matcher. Next element should be a number
    ++l_defn_iter;
    COMPCHEM_ALWAYS (l_defn_iter != l_defn_tokenizer.end (), "Invalid filter 'byte' definition with byte count");
    unsigned int l_count (boost::lexical_cast< unsigned int >(*l_defn_iter));
    Result.reset (new matcher_bytes (l_count));
  }
  break;
  case 1:
  {
    // Byte matcher. Next element should be a number
    ++l_defn_iter;
    COMPCHEM_ALWAYS (l_defn_iter != l_defn_tokenizer.end (), "Invalid filter 'line' definition with line count");
    unsigned int l_count (boost::lexical_cast< unsigned int >(*l_defn_iter));
    Result.reset (new matcher_lines (l_count));
  }
  case 2:
  {
    // Have a string matcher
    std::string l_match;
    ++l_defn_iter;
    COMPCHEM_ALWAYS (l_defn_iter != l_defn_tokenizer.end (), "Invalid filter 'string' definition with match phrase");
    l_match.assign (*l_defn_iter);
    ++l_defn_iter;
    if (l_defn_iter == l_defn_tokenizer.end () and l_defn_iter->empty ())
    {
      // Search string type.
      Result.reset (new matcher_string_search (l_match));
    }
    else
    {
      std::string l_format (*l_defn_iter);
      ++l_defn_iter;
      if (l_defn_iter != l_defn_tokenizer.end () and l_defn_iter->empty ())
      {
        a_mode = helper__::convert_mode (*l_defn_iter);
      }
      Result.reset (new matcher_string (l_match, l_format));
    }
  }
  break;
  case 3:
  {
    // Have a regex matcher
    std::string l_match;
    ++l_defn_iter;
    COMPCHEM_ALWAYS (l_defn_iter != l_defn_tokenizer.end (), "Invalid filter 'regex' definition with match phrase");
    l_match.assign (*l_defn_iter);
    ++l_defn_iter;
    if (l_defn_iter == l_defn_tokenizer.end () and l_defn_iter->empty ())
    {
      // Search string type.
      Result.reset (new matcher_regex_search (l_match));
    }
    else
    {
      std::string l_format (*l_defn_iter);
      ++l_defn_iter;
      if (l_defn_iter != l_defn_tokenizer.end () and l_defn_iter->empty ())
      {
        a_mode = helper__::convert_mode (*l_defn_iter);
      }
      Result.reset (new matcher_regex (l_match, l_format));
    }
  }
  break;
  default:
    COMPCHEM_CHECK (false, "Should never get here, invalid type in filter definition.");
    break;
  }
  COMPCHEM_ENSURE (NULL != Result.get (), "Problem! Failed to create a new matcher object.");
  return Result.release ();

}

/**
 * Constructor. Note that processing the content of the filter description
 * is delayed until the first process_* call.
 * 
 * \param a_filter, The filter object from which to derive this matcher.
 */
matcher_filter::matcher_filter(const data_scheme::filter & a_filter) 
: basic_filter (a_filter)
, m_current (NULL)
, m_error ()
, m_main ()
, m_result ()
, m_start ()
, m_start_result ()
{}

/**
 * Destructor, does nothing special.
 */
matcher_filter::~matcher_filter() 
{}

/**
 * Put the results into a_result string containing XML elements.
 * 
 * \pre a_result.empty ()
 */
int matcher_filter::output_string(std::string & a_result) 
{
  int Result (CCHEM_SUPPLIER_SUCCESS);
  // Ensure a_result is empty
  COMPCHEM_REQUIRE (a_result.empty (), "Result string object should start empty.");
  if (NULL == m_main.second.get ())
  {
    // Have not processed anything!
    Result = CCHEM_SUPPLIER_BADCONFIG;
    m_error.assign ("Attempt to get results before processing data.");
  }
  else
  {
    const std::string l_tmp (m_result.str ());
    if (l_tmp.empty () or m_start_result.empty ()) // No results
    {
      Result = CCHEM_SUPPLIER_UNUSABLE;
    }
    else
    {
      if (not m_start_result.empty ())
      {
        a_result.assign (m_start_result);
      }
      else if (accum == m_main.first)
      {
        // first : one result, enclosing element not needed.
        // last : one result, enclosing element not needed
        // accum : multiple results, enclosing element needed
        a_result.assign ("<datum outcome=\"" + outcomes () + "\">");
      }
      if (not l_tmp.empty ())
      {
        // Only in mode = 2 will m_result be 'closed'
        a_result.append (l_tmp);
        // first : no ending tag
        // last : no ending tag
        // accum : ending tag already appended
        if (accum != m_main.first)
        {
          a_result.append ("</datum>");
        }
      }
      if (not m_start_result.empty () or accum == m_main.first)
      {
        // Add closing tag because we created opening tag for !m_start_result.empty () 
        // or accum == m_main->mode.
        a_result.append ("</datum>");
      }
    }
  }
  return Result;

}

/**
 * * Process filter desc into matcher filters.
 * 
 * \post start = nul or start.mode = 0 (first)
 * \post main /= nul
 */
void matcher_filter::parse_filter() 
{
  ////////////////////////////////////
  // Get the filter's text content.
  std::stringstream l_content;
  if (not get_filter ().empty ())
  {
    data_scheme::content_visitor l_visit (l_content);
    get_filter ().visit (l_visit);
  }
  else
  {
    // Try file
    COMPCHEM_ALWAYS (boost::filesystem::exists (get_filter ().href ())
       , "A filter of type 'regex' must contain the definition or @href must point to a file with the definition");
    std::ifstream l_is (get_filter ().href ().c_str ());
    std::copy (std::istream_iterator< char > (l_is)
              , std::istream_iterator< char > ()
              , std::ostream_iterator< char > (l_content));
  }
  // Test is l_content is empty.
  COMPCHEM_ALWAYS (not l_content.eof ()
       , "A filter of type 'regex' must contain the definition or @href must point to a file with the definition");
  //////////////////////
  // Split the content into tokens and process
  // Parse the content
  std::string l_line;
  std::getline (l_content, l_line);
  m_main.second.reset (make_matcher (l_line, m_main.first));
  // Current always starts out pointing to the first created matcher.
  m_current = m_main.second.get ();
  if (not l_content.eof ())
  {
    std::getline (l_content, l_line);
    m_start = m_main;
    m_main.second.reset (make_matcher (l_line, m_main.first));
  }
  if (not l_content.eof ())
  {
    std::getline (l_content, l_line);
    mode_constant l_ignored;
    m_main.second.reset (new matcher_end_pair (m_main.second.release (), make_matcher (l_line, l_ignored)));
  }
  COMPCHEM_ALWAYS (l_content.eof (), "Filter definition contains more than three lines.");
  // Now call 'reset' to initialise m_current.
  reset ();

}

/**
 * Process a piece of the file.  This will only be called
 * on LINE_ORIENTED and BUFFER_ORIENTED subtypes. The
 * return is one of SUCCESS, FAIL_FATAL or FAIL_UNUSABLE.
 * 
 * The result FAIL_UNUSABLE indicates this object is not
 * usuable on this file and can be removed from the current
 * filter/parser set.
 */
int matcher_filter::process_part(const std::string& a_buffer) 
{
  if (NULL == m_main.second.get ())
  {
     parse_filter ();
  }
  COMPCHEM_REQUIRE (NULL != m_current, "Can not call the filter once it has returned COMPLETE.");
  int Result (CCHEM_SUPPLIER_SUCCESS);
  std::string l_output;
  // Process current matcher
  if (m_current->process_part (a_buffer, l_output))
  {
    switch (m_current == m_main.second.get () ? m_main.first : m_start.first)
    {
    case accum:
      // In accumulate mode, append data
      m_result << l_output << "</datum>";
      break;
    case first:
      // In first mode switch processor or signal exit
      if (m_current == m_start.second.get ())
      {
        // Current matcher was start -> switch
        m_start_result.assign (l_output);
        m_current = m_main.second.get ();
      }
      else
      {
        // Invalidate processor and signal completion
        m_current = NULL;
        Result = CCHEM_SUPPLIER_COMPLETE;
      }
      break;
    case last:
      // In last mode only save the latest output
      m_result.str (l_output);
      break;
    }
  }
  return Result;

}

/**
 * Reset the matcher to perform another search.
 * 
 * * clears m_result and sets m_current
 */
void matcher_filter::reset() 
{
  if (NULL == m_main.second.get ())
  {
    // Need to initialise
    parse_filter ();
  }
  else
  {
    // Reset result stream.
    m_result.str (std::string ());
    // Reset cursor
    if (NULL != m_start.second.get ())
    {
      m_current = m_start.second.get ();
    }
    else
    {
      m_current = m_main.second.get ();
    }
  }

}


} // namespace supplier
