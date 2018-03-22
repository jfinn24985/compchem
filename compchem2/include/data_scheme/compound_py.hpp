#ifndef COMPCHEM_DATA_SCHEME_COMPOUND_PY_HPP
#define COMPCHEM_DATA_SCHEME_COMPOUND_PY_HPP

// ------------------------------------------------------------
// class: compound_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class compound; } 
namespace data_scheme { class identity; } 
namespace data_scheme { class info; } 
namespace data_scheme { class formula; } 
namespace data_scheme { class program; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class compound_py
{
  public:
    /**
     * Make a getter function that is nicer for python.
     */
    static info * get_information_py(compound & a_self);

    /**
     * Create python definitions of the compound C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
