#ifndef COMPCHEM_DATA_SCHEME_SETTINGS_PY_HPP
#define COMPCHEM_DATA_SCHEME_SETTINGS_PY_HPP

// ------------------------------------------------------------
// class: settings_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class settings; } 
namespace data_scheme { class visitor; } 
namespace data_scheme { class info; } 
namespace data_scheme { class datum; } 
namespace data_scheme { class mechanism; } 

namespace data_scheme {

class settings_py
{
  public:
    /**
     * Make a getter function that is nicer for python.
     */
    static datum * get_data_py(settings & a_self);

    /**
     * Make a getter function that is nicer for python.
     */
    static info * get_information_py(settings & a_self);

    /**
     * Create the python definition of the settings data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
