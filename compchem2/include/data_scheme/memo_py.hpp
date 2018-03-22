#ifndef COMPCHEM_DATA_SCHEME_MEMO_PY_HPP
#define COMPCHEM_DATA_SCHEME_MEMO_PY_HPP

// ------------------------------------------------------------
// class: memo_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace data_scheme { class visitor; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class datum; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class info; } 

namespace data_scheme {

class memo_py
{
  public:
    /**
     * Make a src object.
     */
    static void make_src(memo & a_memo, const std::string& a_path);

    /**
     * Create python definition of the memo data_scheme C++ class
     */
    static void pythonise();


};

} // namespace data_scheme
#endif
