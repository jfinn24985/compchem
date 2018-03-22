#ifndef TEXT_DECOR_HPP
#define TEXT_DECOR_HPP

// ------------------------------------------------------------
// class: text_decor
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "report/filetype/base_decor.hpp"

namespace report { namespace filetype { class output_header; }  } 

namespace report {

namespace filetype {

class text_in_decor : public base_in_decor {
  public:
    text_in_decor(output_header & a_header): base_in_decor (a_header) {}

    ~text_in_decor() {}

    void deserialise_header(std::istream & a_is);

    void deserialise_tail(std::istream & a_is) {}

};
class text_out_decor : public base_out_decor {
  public:
    text_out_decor(const output_header & a_header): base_out_decor (a_header) {}

    ~text_out_decor() {}

    void serialise_tail(std::ostream & a_os) const {}

    virtual void serialise_header(std::ostream & a_os) const;

};

} // namespace report::filetype

} // namespace report
#endif
