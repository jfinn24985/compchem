// ------------------------------------------------------------
// class: csv_decor
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "report/filetype/csv_decor.hpp"
#include "report/filetype/output_header.hpp"

// Manual includes
#include <boost/tokenizer.hpp>
//-
namespace report {

namespace filetype {

void csv_in_decor::deserialise_header(std::istream & a_is) {
  // The default I/O is csv.  This should use the boost CSV reader.
  const std::string l_title ("TITLE");
  const std::string l_origin ("ORIGIN");
  const std::string l_owner ("OWNER");
  const std::string l_date ("DATE");
  const std::string l_data_type ("DATA_TYPE");
  m_header.clear ();
  while (a_is) {
    std::string l_line;
    std::getline (a_is, l_line);
    if (l_line.empty ()) {
      break; }
    else {
      boost::tokenizer<boost::escaped_list_separator<char> > l_tok (l_line);
      boost::tokenizer<boost::escaped_list_separator<char> >::iterator l_cur = l_tok.begin();
      if (l_cur != l_tok.end()) {
        const std::string l_key (boost::to_upper_copy (*l_cur));
        ++l_cur;
        if (l_cur != l_tok.end()) {
  	const std::string l_value (*l_cur);
  	if (l_key == l_title) { m_header.title (l_value); }
  	else if (l_key == l_origin) { m_header.origin (l_value); }
  	else if (l_key == l_owner) { m_header.owner (l_value); }
  	else if (l_key == l_date) { m_header.date_string (l_value); }
  	else if (l_key == l_data_type) { m_header.data_type (l_value); }
  	else { m_header.key (l_key, l_value); }}
        else { // No second field? Add empty string
  	m_header.key (l_key, ""); }}
      else { // No tokens? End of header...
        break; }}}
}

void csv_out_decor::serialise_header(std::ostream & a_os) const {
  a_os << "\"TITLE\", \"" << escape_string(m_header.title()) << "\"\n";
  a_os << "\"DATA TYPE\", \"" << escape_string(m_header.data_type ()) << "\"\n";
  a_os << "\"ORIGIN\", \"" << escape_string(m_header.origin ()) << "\"\n";
  a_os << "\"OWNER\", \"" << escape_string(m_header.owner ()) << "\"\n";
  a_os << "\"DATE\", \"" << escape_string(m_header.date_string ()) << "\"\n";
  for (output_header::const_iterator l_i = m_header.get_header_map ().begin ();
       l_i != m_header.get_header_map ().end ();
       ++l_i) {
    std::string l_temp (boost::algorithm::to_upper_copy (l_i->first));
    a_os << "\"" << escape_string(l_temp) << "\", \"" << escape_string(l_i->second) << "\"\n"; }
  a_os << "\n"; // Output empty line to indicate end of header
}

/**
 * This method adds '\' before those characters in a_str that need them for CSV encoding.
 */
std::string csv_out_decor::escape_string(const std::string& a_str)
{
  // LOCAL
  std::stringstream Result;
  const std::string l_to_esc ("\", ");
  // DO
  for (std::string::const_iterator l_i = a_str.begin ();
       l_i != a_str.end ();
       ++l_i) {
    // Add '\' if character needs to be escaped
    if (std::string::npos != l_to_esc.find (*l_i)) { Result << '\\'; }
    Result << *l_i; }
  return Result.str ();
}


} // namespace report::filetype

} // namespace report
