#ifndef JCAMPDX_DECOR_HPP
#define JCAMPDX_DECOR_HPP

// ------------------------------------------------------------
// class: jcampdx_decor
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

/**
 * This type decorates the data of its associated outfile_header object with JCAMP-DX formatting.
 */
class jcampdx_out_decor : public base_out_decor {
  public:
    void serialise_tail(std::ostream & a_os) const {}

    virtual void serialise_header(std::ostream & a_os) const;

    jcampdx_out_decor(const output_header & a_header): base_out_decor (a_header) {}

    virtual ~jcampdx_out_decor() {}

};
class jcampdx_in_decor : public base_in_decor {
  public:
    void deserialise_header(std::istream & a_is);

    void deserialise_tail(std::istream & a_is) {}

    jcampdx_in_decor(output_header & a_header): base_in_decor (a_header) {}

    ~jcampdx_in_decor() {}

};

} // namespace report::filetype

} // namespace report
#endif
