// ------------------------------------------------------------
// class: matcher_types
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/matcher_types.hpp"

namespace supplier {

/**
 * Perform a match against a_target.  If there is a match return true 
 * and set a_result.  This is a pure virtual method that will provided by 
 * derived classes.
 * 
 * \return Did a match occur?
 */
bool matcher_bytes::process_part(const std::string& a_target, std::string & a_result) 
{
  count += a_target.size ();
  return count >= offset;
}

/**
 * Perform a match against a_target.  If there is a match return true .
 * 
 * \return Did a match occur?
 */
bool matcher_lines::process_part(const std::string& a_target, std::string & a_result) 
{
  ++count;
  return count >= offset;
}

/**
 * Equivalent to:
 * if main.process_part --> save result
 * if end.process_part --> set a_result to saved.
 * 
 * Supported use-case: accumulate data from 'main' until 'end'
 * matches.
 */
bool matcher_end_pair::process_part(const std::string& a_target, std::string & a_result) 
{
  std::string l_result;
  if (main->process_part (a_target, l_result))
  {
    this->accumulator.append (l_result);
    l_result.clear ();
  }
  const bool Result (end->process_part (a_target, l_result));
  if (Result)
  {
    a_result.assign (this->accumulator);
  }
  return Result;

}

/**
 * Combine a pair or matchers.
 * 
 * \pre a_main /= nul, a_end /= nul
 */
matcher_end_pair::matcher_end_pair(basic_matcher * a_main, basic_matcher * a_end) 
: accumulator ()
, end (a_end)
, main (a_main)
{
  COMPCHEM_REQUIRE(NULL != a_main, "Main submatch must not be nul");
  COMPCHEM_REQUIRE(NULL != a_end, "End submatch must not be nul");
}

matcher_end_pair::~matcher_end_pair() 
{

}

/**
 * Perform a match against a_buf (of a_sz).  This buffer is a_off from the beginning
 * of the file. If a match is made against the buffer the format string is used to 
 * extract text into a_result.
 * 
 * \return true is a match occurred.
 */
bool matcher_regex::process_part(const std::string& a_target, std::string & a_result) 
{
  // LOCAL
  boost::smatch l_result;
  const bool Result (boost::regex_search (a_target, l_result, finder));
  // DO
  if (Result)
  {
    l_result.format (format).swap (a_result);
  }
  return Result;

}

/**
 * Construct a regular expression matcher. If a_line is true no flags
 * are set by default, if false then a default set of flags for buffer oriented
 * searches is used..
 * 
 * \param a_rgx:  The regular expression in Boost Perl-like syntax (see boost::regex)
 * \param a_lbl: a 'sed' like replacement string where \1, \2 etc represent 
 *     the sub-matches from the regular expression. (see format() in boost::regex_match)
 * 
 * \pre not a_rgx.empty and not a_lbl.empty
 * \pre a_rgx is a valid regular expression.
 */
matcher_regex::matcher_regex(const std::string & a_rgx, const std::string & a_lbl)
: finder (a_rgx)
, format (a_lbl)
{
  COMPCHEM_REQUIRE (!a_rgx.empty (), "Can not match an empty string!");
  COMPCHEM_REQUIRE (!a_lbl.empty (), "Can not use an empty format string!");
}

/**
 * Construct a regular expression matcher with a set of regular expression flags.
 * 
 * \param a_rgx:  The regular expression in Boost Perl-like syntax (see boost::regex)
 * \param a_lbl: a 'sed' like replacement string where \1, \2 etc represent 
 *     the sub-matches from the regular expression. (see format() in boost::regex_match)
 * 
 * \pre not a_rgx.empty and not a_lbl.empty
 * \pre a_rgx is a valid regular expression.
 */
matcher_regex::matcher_regex(const std::string& a_rgx, unsigned int a_regex_flags, const std::string& a_lbl)
: basic_matcher ()
, finder (a_rgx, a_regex_flags)
, format (a_lbl)
{
  COMPCHEM_REQUIRE (!a_rgx.empty (), "Can not match an empty string!");
  COMPCHEM_REQUIRE (!a_lbl.empty (), "Can not use an empty format string!");
}

/**
 * Perform a match against a_buf (of a_sz).  This buffer is a_off from the beginning
 * of the file. If a match is made against the buffer the format string is used to 
 * extract text into a_result.
 * 
 * \return true is a match occurred.
 */
bool matcher_regex_search::process_part(const std::string& a_target, std::string & a_result) 
{
  // DO
  return boost::regex_search (a_target, finder);

}

/**
 * Construct a regular expression matcher. If a_line is true no flags
 * are set by default, if false then a default set of flags for buffer oriented
 * searches is used..
 * 
 * \param a_rgx:  The regular expression in POSIX syntax (see boost::regex)
 * \param a_lbl: a 'sed' like replacement string where \1, \2 etc represent 
 *     the sub-matches from the regular expression. (see format() in boost::regex_match)
 * 
 * \pre not a_rgx.empty and not a_lbl.empty
 * \pre a_rgx is a valid regular expression.
 */
matcher_regex_search::matcher_regex_search(const std::string & a_rgx)
: finder (a_rgx)
{
  COMPCHEM_REQUIRE (!a_rgx.empty (), "Can not match an empty string!");
}

/**
 * Perform a match against a_buf (of a_sz).  This buffer is a_off from the beginning
 * of the file. If m_phrase is found in the buffer then a_result contains m_label if 
 * m_label is not empty, otherwise it contains m_phrase.
 * 
 * \result true if match occurred
 */
bool matcher_string::process_part(const std::string& a_target, std::string & a_result) 
{
  bool Result (a_target.size () > a_target.find (phrase));
  // DO
  if (Result)
  {
    boost::format l_fmter (format);
    // Set formatter to ignore excess arguments.
    l_fmter.exceptions (boost::io::all_error_bits ^ (boost::io::too_many_args_bit));
    l_fmter % a_target % phrase;
    l_fmter.str ().swap (a_result);
  }
  return Result;

}

/**
 * Perform a match against a_target.  Return COMPLETE on a match
 * 
 * \result SUCCESS on no match, COMPLETE on match.
 */
bool matcher_string_search::process_part(const std::string& a_target, std::string & a_result) 
{
  // DO
  return a_target.size () > a_target.find (phrase);

}

/**
 * Equivalent to:
 * if main.process_part --> save result
 * if end.process_part --> set a_result to saved.
 * 
 * Supported use-case: accumulate data from 'main' until 'end'
 * matches.
 */
bool matcher_start_pair::process_part(const std::string& a_target, std::string & a_result) 
{
  std::string l_result;
  bool Result (false);
  if (in_start)
  {
    if (start->process_part (a_target, l_result))
    {
      in_start = false;
      Result = main->process_part (a_target, a_result);
    }
  }
  else
  {
    Result = main->process_part (a_target, a_result);
  }
  return Result;

}

/**
 * Combine a pair or matchers.
 * 
 * \pre a_main /= nul, a_end /= nul
 */
matcher_start_pair::matcher_start_pair(basic_matcher * a_start, basic_matcher * a_main) 
: in_start (true)
, main (a_main)
, start (a_start)
{
  COMPCHEM_REQUIRE(NULL != a_start, "Main submatch must not be nul");
  COMPCHEM_REQUIRE(NULL != a_main, "End submatch must not be nul");
}

matcher_start_pair::~matcher_start_pair() 
{

}


} // namespace supplier
