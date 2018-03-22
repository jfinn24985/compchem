#ifndef COMPCHEM_TEST_FILE_HELPER_HPP
#define COMPCHEM_TEST_FILE_HELPER_HPP

// ------------------------------------------------------------
// class: test_file_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace data_access { template<class T_xml_string> class basic_names; } 
namespace data_access { class persistence; } 
namespace data_access { class xml_access_helper; } 

namespace data_access {

/**
 * Test of xml_access_helper
 * 
 * TODO: more rigorous testing.
 * 
 * --search-path	Start path
 * --search-base	Start base
 * --search-key	Search key
 * --autofix -A	(helper) Whether to automatically attempt to fix broken links
 * --search-dir -D (*) 	(helper) Add search directory.
 */
class test_file_helper : public utility::unit_test
{
  private:
    /**
     * The singleton
     */
    static test_file_helper exemplar;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("data_access::data_access_helper class test");
    }

  private:
    test_file_helper()
    : utility::unit_test ()
    {}


  public:
    ~test_file_helper();


};

} // namespace data_access
#endif
