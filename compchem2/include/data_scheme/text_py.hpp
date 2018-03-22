#ifndef COMPCHEM_DATA_SCHEME_TEXT_PY_HPP
#define COMPCHEM_DATA_SCHEME_TEXT_PY_HPP

// ------------------------------------------------------------
// class: text_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/python.hpp>

#include "data_scheme/text.hpp"

namespace data_scheme { class visitor; } 

namespace data_scheme {

class basic_text_py : public boost::python::wrapper<basic_text_py>, public basic_text
{
  public:
    void visit(visitor & a_visitor) const;

    /**
     * Pythonise the text related element of data_scheme
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
