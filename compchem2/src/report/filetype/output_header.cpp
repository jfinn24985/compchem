// ------------------------------------------------------------
// class: output_header
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "report/filetype/output_header.hpp"

// Manual Includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/regex.hpp>
// -
namespace report {

namespace filetype {

/**
 * The date from a ISO formatted string
 */
void output_header::date_string(const std::string & a_val)  { m_date = boost::lexical_cast <boost::gregorian::date> (a_val); }

/**
 * clears all member variables
 */
void output_header::clear() {
  m_data_type.clear ();
  m_date = boost::gregorian::date (boost::gregorian::not_a_date_time);
  m_header_map.clear ();
  m_origin.clear ();
  m_owner.clear ();
  m_title.clear ();
}


} // namespace report::filetype

} // namespace report
