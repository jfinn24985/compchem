// ------------------------------------------------------------
// class: test_data_manager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/data_access_test/test_data_manager.hpp"
#include "data_access/manager.hpp"

namespace data_access {

test_data_manager test_data_manager::s_exemplar;

/**
 * Override in derived classes to perform unit test.
 * 
 * Failed tests are indicated by an uncaught exception.
 */
void test_data_manager::main_test() const 
{
  data_access::manager &l_man (data_access::manager::server ());
  
  // Get Manager attributes
  bool_test (1 == l_man.size (), "found one persistence back-end.");
  // Test is_supported
  const std::string l_u1;
  const std::string l_u2 ("http://localhost/cchem/base.xml");
  const std::string l_u3 ("file:///usr/local/lib/cchem/base.xml");
  const std::string l_u4 ("/usr/local/lib/cchem/base.xml");
  bool_test (false == l_man.is_supported (l_u1), "reject empty URI [" + l_u1 + "]");
  bool_test (false == l_man.is_supported (l_u2), "reject unsupported scheme URI [" + l_u2 + "]");
  bool_test (true == l_man.is_supported (l_u3), "accept 'file' scheme URI [" + l_u3 + "]");
  bool_test (true == l_man.is_supported (l_u4), "accept plain path [" + l_u4 + "]");

}


} // namespace data_access
