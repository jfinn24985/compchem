#ifndef COMPCHEM_DATA_SCHEME_WORKER_PY_HPP
#define COMPCHEM_DATA_SCHEME_WORKER_PY_HPP

// ------------------------------------------------------------
// class: worker_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class worker; } 
namespace data_scheme { class visitor; } 
namespace data_scheme { class program; } 
namespace data_scheme { class info; } 

namespace data_scheme {

class worker_py
{
  public:
    /**
     * Create the python definition of the worker data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
