#ifndef COMPCHEM_DATA_SCHEME_PROJECT_PY_HPP
#define COMPCHEM_DATA_SCHEME_PROJECT_PY_HPP

// ------------------------------------------------------------
// class: project_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace data_scheme { class compound_ref; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class info; } 
namespace data_scheme { class program; } 
namespace data_scheme { class project; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class project_py
{
  public:
    /**
     * Make a getter function that is nicer for python.
     */
    static info * get_information_py(project & a_self);

    /**
     * Make a src object.
     */
    static void make_src(project & a_proj, const std::string& a_path);

    /**
     * Create the python definitions for the project data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
