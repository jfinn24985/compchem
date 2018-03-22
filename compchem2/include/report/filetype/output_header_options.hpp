#ifndef OUTPUT_HEADER_OPTIONS_HPP
#define OUTPUT_HEADER_OPTIONS_HPP

// ------------------------------------------------------------
// class: output_header_options
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/program_options/options_description.hpp>

#include "report/filetype/output_header.hpp"

namespace report {

namespace filetype {

/**
 * Sub-type that allows some of the header section of JCAMP-DX format data file to be set from program options.
 */
class output_header_options {
  public:
    static void register_options(boost::program_options::options_description & a_opt_set, output_header & a_header);

};

} // namespace report::filetype

} // namespace report
#endif
