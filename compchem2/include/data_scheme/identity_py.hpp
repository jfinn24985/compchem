#ifndef COMPCHEM_DATA_SCHEME_IDENTITY_PY_HPP
#define COMPCHEM_DATA_SCHEME_IDENTITY_PY_HPP

// ------------------------------------------------------------
// class: identity_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class identity; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class identity_py
{
  public:
    /**
     * Create the python definitions of the identity data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
