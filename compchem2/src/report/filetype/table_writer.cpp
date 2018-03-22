// ------------------------------------------------------------
// class: table_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "report/filetype/table_writer.hpp"
#include "report/filetype/markup.hpp"

namespace report {

namespace filetype {

/**
 * Insert end-element mark into a_os.
 */
void table_writer::end_element(std::ostream & a_os) 
{
  if (not this->m_marks.is_null< end_element_index > ())
  {
    this->m_marks.at< end_element_index > ().mark (a_os);
  }

}

/**
 * Insert end-element mark into a_os.
 */
void table_writer::end_row(std::ostream & a_os, unsigned int a_width) 
{
  if (not this->m_marks.is_null< end_row_index > ())
  {
    this->m_marks.at< end_row_index > ().bind (0, a_width).mark (a_os).clear ();
  }

}

/**
 * Insert end-element mark into a_os.
 */
void table_writer::end_table(std::ostream & a_os, unsigned int a_length, unsigned int a_width) 
{
  if (not this->m_marks.is_null< end_table_index > ())
  {
    this->m_marks.at< end_table_index > ().bind (0, a_length).bind (1, a_width).mark (a_os).clear ();
  }

}

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
 * Get a table_writer for simple html table output.
 */
std::auto_ptr< table_writer > table_writer::html_table_writer()

{
  std::auto_ptr< table_writer > Result (new table_writer);
  std::string l_stable ("<table>\n");
  std::string l_etable ("</table>\n");
  std::string l_srow("<tr>\n  <td>");
  std::string l_erow ("</td>\n</tr>\n");
  std::string l_sdat ("  <td>");
  std::string l_edat("</td>\n");
  html_filter l_filt;
  Result->replace_start_table_markup (new report::filetype::string_markup (l_stable));
  Result->replace_end_table_markup (new report::filetype::string_markup (l_etable));
  Result->replace_start_row_markup (new report::filetype::string_markup (l_srow));
  Result->replace_end_row_markup (new report::filetype::string_markup (l_erow));
  Result->replace_start_element_markup (new report::filetype::string_markup (l_sdat));
  Result->replace_end_element_markup (new report::filetype::string_markup (l_edat));
  Result->set_filter (l_filt);
  return Result;

}

/**
 * Insert end-element mark into a_os.
 */
void table_writer::start_element(std::ostream & a_os) 
{
  if (not this->m_marks.is_null (start_element_index))
  {
    this->m_marks.at< start_element_index > ().mark (a_os);
  }

}

/**
 * Insert end-element mark into a_os.
 */
void table_writer::start_row(std::ostream & a_os, unsigned int a_width) 
{
  if (not this->m_marks.is_null< start_row_index > ())
  {
    this->m_marks.at < start_row_index > ().bind (0, a_width).mark (a_os).clear ();
  }

}

/**
 * Insert end-element mark into a_os.
 */
void table_writer::start_table(std::ostream & a_os, unsigned int a_length, unsigned int a_width) 
{
  if (not this->m_marks.is_null< start_table_index > ())
  {
    this->m_marks.at< start_table_index > ().bind (0, a_length).bind (1, a_width).mark (a_os).clear ();
  }

}

/**
 * Default ctor
 */
table_writer::table_writer() 
: m_marks ()
, m_filter ()
{}

/**
 * Dtor
 */
table_writer::~table_writer() 
{}


} // namespace report::filetype

} // namespace report
