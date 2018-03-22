#ifndef COMPCHEM_DATA_SCHEME_INFO_PY_HPP
#define COMPCHEM_DATA_SCHEME_INFO_PY_HPP

// ------------------------------------------------------------
// class: info_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class info; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class info_py
{
  public:
    /**
     * Create the python definitions of the info data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
