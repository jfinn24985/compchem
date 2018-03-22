#ifndef COMPCHEM_DATA_SCHEME_FILTER_PY_HPP
#define COMPCHEM_DATA_SCHEME_FILTER_PY_HPP

// ------------------------------------------------------------
// class: filter_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class filter; } 
namespace data_scheme { class mechanism; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class filter_py
{
  public:
    /**
     * Create the python definitions of the filter data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
