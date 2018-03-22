#ifndef COMPCHEM_TEST_FILTERMANAGER_HPP
#define COMPCHEM_TEST_FILTERMANAGER_HPP

// ------------------------------------------------------------
// class: test_filtermanager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <boost/bind.hpp>

#include "supplier/filter_manager.hpp"
#include <string>

namespace data_scheme { class compound; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class program; } 
namespace utility { class program_options; } 
namespace data_scheme { class project; } 
namespace supplier { class supplier_helper; } 
namespace utility { class test_manager; } 
namespace data_access { class xml_write_visitor; } 
namespace data_access { class xml_access_helper; } 

namespace supplier {

/**
 * Test run the filter_manager.
 */
class test_filtermanager : public utility::unit_test
{
  private:
    /**
     * The path to the test database.
     */
    std::string m_database;

    /**
     * The class exemplar
     */
    static test_filtermanager s_exemplar;


  public:
    /**
     * Run tests on filter_manager class
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("Test the /compchem/supplier//filter_manager class");
    }
    test_filtermanager()
    : unit_test ()
    , m_database ()
    {
      register_options ();
    }

    virtual ~test_filtermanager()
    {}


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();


};

} // namespace supplier
#endif
