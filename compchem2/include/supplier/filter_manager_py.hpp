#ifndef COMPCHEM_FILTER_MANAGER_PY_HPP
#define COMPCHEM_FILTER_MANAGER_PY_HPP

// ------------------------------------------------------------
// class: filter_manager_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>



namespace data_scheme { class compound; } 
namespace data_scheme { class experiment; } 
namespace supplier { class filter_manager; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class program; } 
namespace data_scheme { class project; } 
namespace supplier { class supplier_helper; } 
namespace utility { class time_stamp; } 

namespace supplier {

class filter_manager_py
{
  public:
    /**
     * Create a "unique" filename for the memo-src location.
     */
    std::string memo_src_location(const boost::filesystem::path & a_path, const std::string& a_memo_label);

    /**
     * Perform the configured filter operation on a_database.
     */
    void process(const std::string& a_database);


};

} // namespace supplier
#endif
