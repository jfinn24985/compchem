#ifndef COMPCHEM_META_DATUM_PY_HPP
#define COMPCHEM_META_DATUM_PY_HPP

// ------------------------------------------------------------
// class: meta_datum_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_access { class meta_datum; } 

namespace data_access {

class meta_datum_py
{
  public:
    /**
     * Create the python definition of the meta_datum data_access C++ class
     */
    static void pythonise();


};

} // namespace data_access
#endif
