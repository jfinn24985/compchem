#ifndef COMPCHEM_DATA_SCHEME_DATA_SCHEME_PY_MAIN_HPP
#define COMPCHEM_DATA_SCHEME_DATA_SCHEME_PY_MAIN_HPP

// ------------------------------------------------------------
// class: data_scheme_py_main
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * Call the definitions for the individual data_scheme classes
 */
#include "config.hpp"
#include <string>

namespace data_scheme { class basic_datum_py; } 
namespace data_scheme { class basic_text_py; } 
namespace data_scheme { class compound_py; } 
namespace data_scheme { class compound_ref_py; } 
namespace data_scheme { class datum_py; } 
namespace data_scheme { class experiment_py; } 
namespace data_scheme { class filter_py; } 
namespace data_scheme { class formula_py; } 
namespace data_scheme { class identity_py; } 
namespace data_scheme { class info_py; } 
namespace data_scheme { class mechanism_py; } 
namespace data_scheme { class memo_py; } 
namespace data_scheme { class program_py; } 
namespace data_scheme { class project_py; } 
namespace data_scheme { class settings_py; } 
namespace data_scheme { class visitor_py; } 
namespace data_scheme { class worker_py; } 
namespace data_scheme { class data_scheme_helper; } 

namespace data_scheme {

class data_scheme_py
{
  public:
    bool get_auto_open_memo();

    void set_auto_open_memo(bool a_proj);

    bool get_auto_project();

    void set_auto_project(bool a_proj);

    const std::string & get_target_project();

    void set_target_project(const std::string& a_proj);


};

} // namespace data_scheme
#endif
