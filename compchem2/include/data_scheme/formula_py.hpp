#ifndef COMPCHEM_DATA_SCHEME_FORMULA_PY_HPP
#define COMPCHEM_DATA_SCHEME_FORMULA_PY_HPP

// ------------------------------------------------------------
// class: formula_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class formula; } 
namespace data_scheme { class info; } 
namespace data_scheme { class identity; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class formula_py
{
  public:
    /**
     * Make a getter function that is nicer for python.
     */
    static info * get_information_py(formula & a_self);

    /**
     * Create the python definitions of the formula data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
