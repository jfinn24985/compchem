// ------------------------------------------------------------
// class: test_filter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/supplier_test/test_filter.hpp"

namespace supplier {

/**
 * The exemplar
 */
test_filter test_filter::s_exmplr;

/**
 * Override in derived classes to perform unit test.
 */
void test_filter::main_test2() const {
  // monitor_function (utility::contract_error::require
  //     , boost::bind (&make_matcher
  //       , "A match"
  //       , "A format"
  //       , "A type"
  //       , true)
  //     , "Attempt to make matcher with invalid type.");
  // monitor_function (utility::contract_error::require
  //     , boost::bind (&make_matcher
  //       , ""
  //       , "A format"
  //       , "regex"
  //       , true)
  //     , "Attempt to make regex matcher with an empty regular expression.");
  // monitor_function (utility::contract_error::require
  //     , boost::bind (&make_matcher
  //       , "A regex"
  //       , ""
  //       , "regex"
  //       , true)
  //     , "Attempt to make regex matcher with an empty format expression.");
  // monitor_function (utility::contract_error::require
  //     , boost::bind (&make_matcher
  //       , "A\\(\\((regex\\)))"
  //       , "A format"
  //       , "regex"
  //       , true)
  //     , "Attempt to make regex matcher with an invalid regular expression.");
  // monitor_function (utility::contract_error::require
  //     , boost::bind (&make_matcher
  //       , ""
  //       , "A format"
  //       , "line"
  //       , true)
  //     , "Attempt to make string matcher with an empty search string.");
  // if (0 == utility::test_manager::get_exemplar ().option_map ().count ("gout"))
  // {
  //   log () << EXCP << "No gaussian output file given.\n";
  // }
  // else
  // {
  //   matcher l_test1 ("0", "160", "Entering Gaussian System, Link 0=g03", "g03", "line", true);
  //   matcher l_test2 ("3000", "", "Gaussian [[:alnum:]]+: +([[:alnum:]-]+)/([.[:alnum:]-]+) +([[:alnum:]-]+)", "\\1 : \\2 : \\3", "regex", true);
  //   matcher l_test3 ("3000", "", "----------------------------------------------------------------------[[:space:]]+#((-[^-\n]+|[^-\n]+)+)\n[[:space:]]+(((-[^-\n]+|[^-\n]+)+)\n[[:space:]]+)?(((-[^-\n]+|[^-\n]+)+)\n[[:space:]]+)?---------------------------------------", "\\1\\4\\7", "regex", false);
  //   // Unit tests
  //   bool_test (not l_test1.have_result (), "Matcher one starts without result after ctor.");
  //   bool_test (not l_test2.have_result (), "Matcher two starts without result after ctor.");
  //   bool_test (not l_test3.have_result (), "Matcher three starts without result after ctor.");
  //   bool_test (l_test1.line_oriented (), "Matcher one is line oriented.");
  //   bool_test (l_test2.line_oriented (), "Matcher two is line oriented.");
  //   bool_test (not l_test3.line_oriented (), "Matcher three is not line oriented.");
  //   bool_test (not l_test1.no_match (), "Matcher one can match after ctor.");
  //   bool_test (not l_test2.no_match (), "Matcher two can match after ctor.");
  //   bool_test (not l_test3.no_match (), "Matcher three can match after ctor.");
  //   log () << WTCH << "Attempt to get result one when matcher one has no result.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::result, &l_test1));
  //   log () << WTCH << "Attempt to get result two when matcher two has no result.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::result, &l_test2));
  //   log () << WTCH << "Attempt to get result three when matcher three has no result.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::result, &l_test3));
  //   log () << WTCH << "Attempt to match with nul buffer which has 'size' > 0.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::match, &l_test3, (const char*)NULL, 10U, 15U));
  //   log () << WTCH << "Attempt to match with nul buffer which has 'size' = 0.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::match, &l_test3, (const char*)this, 0U, 10U));     
  //   // Acceptance tests
  //   const std::string l_filenm (utility::test_manager::get_exemplar ().option_map ()["gout"].as < std::string > ());
  //   std::ifstream l_is (l_filenm.c_str ());
  //   if (not l_is)
  //   {
  //     log () << EXCP << "Could not open input stream for '" << l_filenm << "'.\n";
  //   }
  //   else
  //   {
  //     const unsigned int l_step (256);
  //     const unsigned int l_sz (32 * l_step);
  //     boost::scoped_array < char >l_buf (new char[l_sz]);
  //     l_is.read (l_buf.get (), l_sz - 1);
  //     const unsigned int l_read (l_is.gcount ());
  //     l_buf[l_read] = '\0';	// Ensure null terminated.
  //     log () << COMM << "Test buffer. Size = " << l_read << "\n";
  //     log () << l_buf.get () << "\n";
  // 
  //     log () << COMM << "String matcher 1 (\"Entering Gaussian System, Link 0=g03\", \"g03\")\n";
  //     test_matching (l_test1, l_buf.get (), l_read);
  // 
  //     log () << COMM << "Regex matcher 2 (\"Gaussian 03: ...\")\n";
  //     test_matching (l_test2, l_buf.get (), l_read);
  // 
  //     log () << COMM << "Regex matcher 3 (\"--- #.. ---\")\n";
  //     test_matching (l_test3, l_buf.get (), l_read);
  // 
  //     log () << COMM << "Regex matcher 3 (\"--- #.. ---\") all in one.\n";
  //     l_test3.reset ();
  //     if (not l_test3.match (l_buf.get (), l_sz, 0))
  //     {
  //       log () << WTCH << "No match\n";
  //     }
  //     else
  //     {
  //       if (l_test3.have_result ())
  //       {
  //         log () << WTCH << "Match made, result = \"" << l_test3.result () << "\"\n";
  //       }
  //       else
  //       {
  //         log () << WTCH << "Match but no result\n";
  //       }
  //     }
  //   }
  //   // Unit tests
  //   log () << TTLE << "Unit tests after match.\n";
  //   bool_test (l_test1.have_result (), "Matcher one may have result after match.");
  //   bool_test (l_test2.have_result (), "Matcher two may have result after match.");
  //   bool_test (l_test3.have_result (), "Matcher three may have result after match.");
  //   bool_test (l_test1.line_oriented (), "Matcher one is line oriented.");
  //   bool_test (l_test2.line_oriented (), "Matcher two is line oriented.");
  //   bool_test (not l_test3.line_oriented (), "Matcher three is not line oriented.");
  //   l_test1.reset ();
  //   l_test2.reset ();
  //   l_test3.reset ();
  //   log () << TTLE << "Unit tests after reset (should be the sames as directly after ctor).\n";
  //   bool_test (not l_test1.have_result (), "Matcher one starts without result after ctor.");
  //   bool_test (not l_test2.have_result (), "Matcher two starts without result after ctor.");
  //   bool_test (not l_test3.have_result (), "Matcher three starts without result after ctor.");
  //   bool_test (l_test1.line_oriented (), "Matcher one is line oriented.");
  //   bool_test (l_test2.line_oriented (), "Matcher two is line oriented.");
  //   bool_test (not l_test3.line_oriented (), "Matcher three is not line oriented.");
  //   bool_test (not l_test1.no_match (), "Matcher one can match after ctor.");
  //   bool_test (not l_test2.no_match (), "Matcher two can match after ctor.");
  //   bool_test (not l_test3.no_match (), "Matcher three can match after ctor.");
  //   log () << WTCH << "Attempt to get result one when matcher one has no result.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::result, &l_test1));
  //   log () << WTCH << "Attempt to get result two when matcher two has no result.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::result, &l_test2));
  //   log () << WTCH << "Attempt to get result three when matcher three has no result.\n";
  //   monitor_function (utility::contract_error::require
  //                   , boost::bind (&matcher::result, &l_test3));
  // }
}


} // namespace supplier
