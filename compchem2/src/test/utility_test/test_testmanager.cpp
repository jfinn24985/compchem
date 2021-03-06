// ------------------------------------------------------------
// class: test_testmanager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/utility_test/test_testmanager.hpp"

namespace utility {

/**
 * Class exemplar
 */
test_testmanager test_testmanager::exemplar;

/**
 * Run tests on test_manager exemplar
 */
void test_testmanager::main_test() const {
#line 101
  utility::test_manager & l_mgr (utility::test_manager::get_exemplar ());
  log () << WTCH << "Test manager title: " << l_mgr.title () << "\n";
  log () << WTCH << "Test manager size : " << l_mgr.size () << "\n";
  bool_test (l_mgr.has_tester (this), "Check we are in list of tests.");
  log () << COMM << "Attempt to add self to manager.\n";
  monitor_function (utility::contract_error::require
                    , boost::bind (&utility::test_manager::add_tester, &l_mgr, const_cast< test_testmanager * const >(this))
                    , "attempt to add self to manager again.");
  log () << WTCH << "Have tested that we can remove/add in earlier versions. However\n"
         << WTCH << "this causes havoc by reordering the test cases as the manager is\n"
         << WTCH << "over them\n";
  // log () << COMM << "Attempt to remove self from manager twice.\n";
  // l_mgr.remove_tester (const_cast< test_testmanager * const >(this));
  // monitor_function (utility::contract_error::require
  //                  , boost::bind (&utility::test_manager::remove_tester, &l_mgr, const_cast< test_testmanager * const >(this)));
  // bool_test (not l_mgr.has_tester (this), "Check we are NOT in list of tests.");
  // log () << COMM << "Reattempt to add self to manager.\n";
  monitor_function (utility::contract_error::require
                    , boost::bind (&utility::test_manager::remove_tester, &l_mgr, (const utility::base_test*)(NULL))
                    , "attempt to remove non-existent unit_test (nul) from manager.");
  bool_test (not l_mgr.has_tester (NULL), "Check nul unit_test is not in list of tests.");
}


} // namespace utility
