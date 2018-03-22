#ifndef COMPCHEM_DATA_SCHEME_DATUM_PY_HPP
#define COMPCHEM_DATA_SCHEME_DATUM_PY_HPP

// ------------------------------------------------------------
// class: datum_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class datum; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class datum_py
{
  public:
    /**
     * Create the python version of the datum C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
