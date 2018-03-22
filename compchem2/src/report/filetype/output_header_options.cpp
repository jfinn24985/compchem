// ------------------------------------------------------------
// class: output_header_options
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "report/filetype/output_header_options.hpp"

// Manual includes
#include <boost/bind.hpp>
// -
namespace report {

namespace filetype {

void output_header_options::register_options(boost::program_options::options_description & a_opt_set, output_header & a_header)
{
  // Add options to a_opt_set
  a_opt_set.add_options ()
    ("spectrum-title", boost::program_options::value< std::string >()->notifier (boost::bind (&output_header::title, &a_header, _1)), "the title shown in the output")
    ("spectrum-date", boost::program_options::value< std::string >()->notifier (boost::bind (&output_header::date_string, &a_header, _1)), "the date shown in the output (if not set elsewhere, default is today)")
    ("spectrum-source", boost::program_options::value< std::string >()->notifier (boost::bind (&output_header::origin, &a_header, _1)), "the source of the data in the output")
    ("spectrum-type", boost::program_options::value< std::string >()->notifier (boost::bind (&output_header::data_type, &a_header, _1)), "the spectrum data type")
    ("spectrum-owner", boost::program_options::value< std::string >()->notifier (boost::bind (&output_header::owner, &a_header, _1)), "the owner of the data in the output");
}


} // namespace report::filetype

} // namespace report
