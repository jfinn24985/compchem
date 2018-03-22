#ifndef CSV_DECOR_HPP
#define CSV_DECOR_HPP

// ------------------------------------------------------------
// class: csv_decor
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "report/filetype/base_decor.hpp"
#include <string>

namespace report { namespace filetype { class output_header; }  } 

namespace report {

namespace filetype {

class csv_in_decor : public base_in_decor {
  public:
    csv_in_decor(output_header & a_header): base_in_decor (a_header) {}

    ~csv_in_decor() {}

    void deserialise_header(std::istream & a_is);

    void deserialise_tail(std::istream & a_is) {}

};
class csv_out_decor : public base_out_decor {
  public:
    csv_out_decor(const output_header & a_header): base_out_decor (a_header) {}

    ~csv_out_decor() {}

    void serialise_tail(std::ostream & a_os) const {}

    virtual void serialise_header(std::ostream & a_os) const;

    /**
     * This method adds '\' before those characters in a_str that need them for CSV encoding.
     */
    static std::string escape_string(const std::string& a_str);

};

} // namespace report::filetype

} // namespace report
#endif
