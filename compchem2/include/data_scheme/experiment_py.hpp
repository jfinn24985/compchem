#ifndef COMPCHEM_DATA_SCHEME_EXPERIMENT_PY_HPP
#define COMPCHEM_DATA_SCHEME_EXPERIMENT_PY_HPP

// ------------------------------------------------------------
// class: experiment_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class experiment; } 
namespace data_scheme { class project; } 
namespace data_scheme { class info; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class experiment_py
{
  public:
    /**
     * Create the python definitions of the experiment data_scheme class.
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
