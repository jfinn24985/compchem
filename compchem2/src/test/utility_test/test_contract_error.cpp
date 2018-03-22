// ------------------------------------------------------------
// class: test_contract_error
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/utility_test/test_contract_error.hpp"

namespace utility {

/**
 * Override in derived classes to perform unit test.
 */
void test_contract_error::main_test() const {
  class helper 
  {
  public:
    static void test_always ()
    {
      COMPCHEM_ALWAYS (false, "Test of 'COMPCHEM_ALWAYS' DbC macro");
    }
    static void test_require ()
    {
      COMPCHEM_REQUIRE (false, "Test of 'COMPCHEM_REQUIRE' DbC macro");
    }
    static void test_ensure ()
    {
      COMPCHEM_ENSURE (false, "Test of 'COMPCHEM_ENSURE' DbC macro");
    }
    static void test_invariant ()
    {
      COMPCHEM_INVARIANT (false, "Test of 'COMPCHEM_INVARIANT' DbC macro");
    }
    static void test_check ()
    {
      COMPCHEM_CHECK (false, "Test of 'COMPCHEM_CHECK' DbC macro");
    }
  };
  log () << COMM << "Error codes:\n";
  bool_test (0 == contract_error::always, "Always    = [0]");
  bool_test (1 == contract_error::require, "Require   = [1]");
  bool_test (2 == contract_error::ensure, "Ensure    = [2]");
  bool_test (3 == contract_error::invariant, "Invariant = [3]");
  bool_test (4 == contract_error::check, "Check     = [4]");
  bool_test (5 == contract_error::max_level, "Max Level = [5]");
  bool_test (4000 == contract_error::s_size, "Buffer Sz = [4000]");
  log () << COMM << "Check macros:\n";
  monitor_function (contract_error::always, &helper::test_always, "Test of 'COMPCHEM_ALWAYS' DbC macro");
  monitor_function (contract_error::require, &helper::test_require, "Test of 'COMPCHEM_REQUIRE' DbC macro");
  monitor_function (contract_error::ensure, &helper::test_ensure, "Test of 'COMPCHEM_ENSURE' DbC macro");
  monitor_function (contract_error::invariant, &helper::test_invariant, "Test of 'COMPCHEM_INVARIANT' DbC macro");
  monitor_function (contract_error::check, &helper::test_check, "Test of 'COMPCHEM_CHECK' DbC macro");
  try
  {
    COMPCHEM_ALWAYS (false, "Test of copy and swap.");
  }
  catch (const contract_error &a_err)
  {
    const std::string l_what (a_err.what ());
    const unsigned int l_level (a_err.level);
    contract_error l_copy (a_err);
    bool_test (l_copy.what () == l_what, "copied message matchs original");
    bool_test (l_copy.level == l_level, "copied level matchs original");
    contract_error l_new ("msg1", "msg2", 4, __FILE__, __LINE__, "dummy::functionname");
    bool_test (l_new.level == 4, "made object level matchs");
    const std::string l_what2 (l_new.what ());
    l_copy.swap (l_new);
    bool_test (l_copy.what () == l_what2, "swapped object 1 has correct message");
    bool_test (l_copy.level == 4, "swapped object 1 has correct level");
    bool_test (l_new.what () == l_what, "swapped object 2 has correct message");
    bool_test (l_new.level == l_level, "swapped object 2 has correct level");
  }
}

/**
 * The class exemplar
 */
test_contract_error test_contract_error::exemplar;


} // namespace utility
