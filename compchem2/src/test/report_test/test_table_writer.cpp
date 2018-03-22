// ------------------------------------------------------------
// class: test_table_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/report_test/test_table_writer.hpp"
#include "report/filetype/markup.hpp"
#include "report/filetype/table_writer.hpp"

// Manual includes
#include <vector>
#include <sstream>
#include <boost/variant.hpp>
// -
namespace report {

struct html_filter
{
  typedef char                   char_type;
  typedef boost::iostreams::output_filter_tag  category;

  template<typename Sink>
  bool put(Sink& snk, char c) 
  {
    char Result (c);
    switch (Result)
    {
    case '&':
      boost::iostreams::put(snk, '&');
      boost::iostreams::put(snk, 'a');
      boost::iostreams::put(snk, 'm');
      boost::iostreams::put(snk, 'p');
      Result = ';';
      break;
    case '<':
      boost::iostreams::put(snk, '&');
      boost::iostreams::put(snk, 'l');
      boost::iostreams::put(snk, 't');
      Result = ';';
      break;
    case '>':
      boost::iostreams::put(snk, '&');
      boost::iostreams::put(snk, 'g');
      boost::iostreams::put(snk, 't');
      Result = ';';
      break;
    default: // do nothing
      break;
    }
    return boost::iostreams::put (snk, Result);
  }
};/**
 * Override in derived classes to perform unit test.
 * 
 * Failed tests are indicated by an uncaught exception.
 */
void test_table_writer::main_test() const 
{
  std::ostringstream l_log;
  std::vector< boost::variant< int, std::string, double > > l_temp;
  l_temp.push_back (1);
  l_temp.push_back (10);
  l_temp.push_back (std::string ("Hello & World"));
  l_temp.push_back (4.5);
  l_temp.push_back (std::string ("<b>lacy</b>"));
  l_temp.push_back (std::string ("jocstrap"));
  const std::string l_compare1 ("110Hello & World4.5<b>lacy</b>jocstrap");
  const std::string l_compare2 ("1, 10\nHello & World, 4.5\n<b>lacy</b>, jocstrap\n");
  const std::string l_compare3 ("<table columns=\"3\" rows=\"2\">\n<tr><td>1</td><td>10</td><td>Hello &amp; World</td></tr>\n<tr><td>4.5</td><td>&lt;b&gt;lacy&lt;/b&gt;</td><td>jocstrap</td></tr>\n</table>\n");
  const std::string l_compare4 ("<table>\n<tr>\n  <td>1</td>\n  <td>10</td>\n  <td>Hello &amp; World</td>\n  <td>4.5</td>\n  <td>&lt;b&gt;lacy&lt;/b&gt;</td>\n  <td>jocstrap</td>\n</tr>\n</table>\n");
  log () << COMM << "List of values.\n";
  for (unsigned int index (0); index < l_temp.size (); ++index)
  {
    log () << l_temp[index] << "\n";
  }
  report::filetype::table_writer l_tablerator;
  log () << WTCH << "Print using table_writer {table 6x1 = list} with no markup-set.\n";
  l_tablerator.write_table (l_temp.begin (), l_temp.end (), 6, 1, l_log);
  log () << l_log.str ();
  log () << COMM << "Canonical output 1\n";
  log () << l_compare1 << "\n";
  bool_test (l_compare1 == l_log.str (), "Output 1 matches canonical value.");
  l_log.str ("");
  {
    std::string l_endline ("\n");
    std::string l_endelem (", ");
    l_tablerator.replace_end_row_markup (new report::filetype::string_markup (l_endline));
    l_tablerator.replace_end_element_markup (new report::filetype::string_markup (l_endelem));
  }
  log () << "\n" << WTCH << "Print using simple CSV-like style {table 3x2}\n";
  l_tablerator.write_table (l_temp.begin (), l_temp.end (), 3, 2, l_log);
  log () << l_log.str ();
  log () << COMM << "Canonical output 2\n";
  log () << l_compare2;
  bool_test (l_compare2 == l_log.str (), "Output 2 matches canonical value.");
  l_log.str ("");
  {
    std::string l_stable ("<table columns=\"%2%\" rows=\"%1%\">\n"), l_etable ("</table>\n"), l_srow("<tr><td>");
    std::string l_erow ("</td></tr>\n"), l_sdat ("<td>"), l_edat("</td>");
    l_tablerator.replace_start_table_markup (new report::filetype::format_markup (l_stable));
    l_tablerator.replace_end_table_markup (new report::filetype::string_markup (l_etable));
    l_tablerator.replace_start_row_markup (new report::filetype::string_markup (l_srow));
    l_tablerator.replace_end_row_markup (new report::filetype::string_markup (l_erow));
    l_tablerator.replace_start_element_markup (new report::filetype::string_markup (l_sdat));
    l_tablerator.replace_end_element_markup (new report::filetype::string_markup (l_edat));
  }
  // Use a filter for the following
  html_filter l_filt;
  l_tablerator.set_filter (l_filt);
  log () << WTCH << "Print using HTML-like style with filtering {table 2x3}\n";
  l_tablerator.write_table (l_temp.begin (), l_temp.end (), 2, 3, l_log);
  log () << l_log.str ();
  log () << COMM << "Canonical output 3\n";
  log () << l_compare3;
  bool_test (l_compare3 == l_log.str (), "Output 3 matches canonical value.");
  l_log.str ("");
  log () << WTCH << "Print using HTML-like style from table_writer {table 1x6}\n";
  std::auto_ptr< report::filetype::table_writer > l_tempwriter (report::filetype::table_writer::html_table_writer ());
  l_tempwriter->write_table (l_temp.begin (), l_temp.end (), 1, 6, l_log);
  log () << l_log.str ();
  log () << COMM << "Canonical output 4\n";
  log () << l_compare4;
  bool_test (l_compare4 == l_log.str (), "Output 3 matches canonical value.");

}

/**
 * Singleton for this test class.
 */
test_table_writer test_table_writer::s_exemplar;


} // namespace report
