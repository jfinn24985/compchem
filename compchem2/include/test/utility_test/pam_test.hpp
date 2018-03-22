#ifndef PAM_TEST_HPP
#define PAM_TEST_HPP

// ------------------------------------------------------------
// class: pam_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include "utility/authentication_manager.hpp"
#include <string>

namespace utility {

/**
 * This class provides a unit test for the authentication manager derived classes.
 */
class authentication_test : public unit_test {
  private:
    /**
     * The singleton test object.
     */
    static authentication_test s_exemplar;

    /**
     * Ctor for exemplar.  Will add test-case to test_manager.
     */
    authentication_test()
    : unit_test ()
    {
      register_options ();
    }


  public:
    /**
     * Destructor, should remove test case?
     */
    ~authentication_test();


  private:
    /**
     * Undefined
     */
    authentication_test(const authentication_test & source);


  public:
    /**
     * The method that performs tests on the authentication manager exemplar
     */
    void main_test() const;


  private:
    /**
     * Undefined
     */
    authentication_test & operator=(const authentication_test & source);

    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();


  public:
    std::string title() const { return "Authentication Manager test"; }

};

} // namespace utility
#endif
