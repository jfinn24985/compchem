#ifndef COMPCHEM_DATA_SCHEME_PROGRAM_PY_HPP
#define COMPCHEM_DATA_SCHEME_PROGRAM_PY_HPP

// ------------------------------------------------------------
// class: program_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace data_scheme { class program; } 
namespace data_scheme { class visitor; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class project; } 
namespace data_scheme { class mechanism; } 
namespace data_scheme { class info; } 
namespace data_scheme { class worker; } 

namespace data_scheme {

class program_py
{
  public:
    /**
     * Make a getter function that is nicer for python.
     */
    static info * get_information_py(program & a_self);

    /**
     * Constructor for program objects.
     */
    static boost::shared_ptr< program > init_program(const std::string& a_uri);

    /**
     * Create the python definitions of the program data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
