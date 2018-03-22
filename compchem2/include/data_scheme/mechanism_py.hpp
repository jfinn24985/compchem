#ifndef COMPCHEM_DATA_SCHEME_MECHANISM_PY_HPP
#define COMPCHEM_DATA_SCHEME_MECHANISM_PY_HPP

// ------------------------------------------------------------
// class: mechanism_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class mechanism; } 
namespace data_scheme { class visitor; } 
namespace data_scheme { class info; } 
namespace data_scheme { class filter; } 
namespace data_scheme { class settings; } 
namespace data_scheme { class program; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class project; } 

namespace data_scheme {

class mechanism_py
{
  public:
    /**
     * Make a getter function that is nicer for python.
     */
    static info * get_information_py(mechanism & a_self);

    /**
     * Create the python definition of the mechanism data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
