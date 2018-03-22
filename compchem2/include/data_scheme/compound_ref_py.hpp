#ifndef COMPCHEM_DATA_SCHEME_COMPOUND_REF_PY_HPP
#define COMPCHEM_DATA_SCHEME_COMPOUND_REF_PY_HPP

// ------------------------------------------------------------
// class: compound_ref_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class compound_ref; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class compound_ref_py
{
  public:
    /**
     * Create python definition of the memo data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
