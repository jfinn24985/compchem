// ------------------------------------------------------------
// class: jcampdx_decor
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "report/filetype/jcampdx_decor.hpp"
#include "report/filetype/output_header.hpp"

// Manual Includes
#include <boost/regex.hpp>
// - 
namespace report {

namespace filetype {

void jcampdx_out_decor::serialise_header(std::ostream & a_os) const {
  a_os << "##TITLE= " << m_header.title() << "\n";
  a_os << "##JCAMP-DX= 4.24\n";
  a_os << "##DATA TYPE= " << m_header.data_type () << "\n";
  a_os << "##ORIGIN= " << m_header.origin () << "\n";
  a_os << "##OWNER= " << m_header.owner () << "\n";
  a_os << "##DATE= " << m_header.date_string () << "\n";
  for (output_header::const_iterator l_i = m_header.get_header_map ().begin ();
       l_i != m_header.get_header_map ().end ();
       ++l_i) {
    std::string l_temp (boost::algorithm::to_upper_copy (l_i->first));
    a_os << "##" << l_temp << "= " << l_i->second << "\n"; }
}

void jcampdx_in_decor::deserialise_header(std::istream & a_is) {
  const boost::regex l_jcamp_rx ("^##([^=]*)=(.*)$", boost::regex::extended);
  const std::string l_title ("TITLE");
  const std::string l_version ("JCAMP-DX");
  const std::string l_origin ("ORIGIN");
  const std::string l_owner ("OWNER");
  const std::string l_date ("DATE");
  const std::string l_data_type ("DATA_TYPE");
  m_header.clear ();
  while (a_is) {
    std::string l_line;
    std::getline (a_is, l_line);
    boost::smatch l_match;
    if (not boost::regex_match (l_line, l_match, l_jcamp_rx)) {
      // End of header
      break; }
    if (l_match.size () < 3) {
      // Error processing file?
      throw std::exception (); }
    const std::string l_key (boost::to_upper_copy (l_match.str (1)));
    if (l_key == l_title) { m_header.title (l_match.str (2)); }
    else if (l_key == l_version) { } // do nothing (TODO)
    else if (l_key == l_origin) { m_header.origin (l_match.str (2)); }
    else if (l_key == l_owner) { m_header.owner (l_match.str (2)); }
    else if (l_key == l_date) { m_header.date_string (l_match.str (2)); }
    else if (l_key == l_data_type) { m_header.data_type (l_match.str (2)); }
    else { m_header.key (l_key, l_match.str (2)); }}
}


} // namespace report::filetype

} // namespace report
