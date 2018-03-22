#ifndef UI_TRAITS_TEST_HPP
#define UI_TRAITS_TEST_HPP

// ------------------------------------------------------------
// class: ui_traits_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/ui_traits.hpp"

namespace expt {

/**
 * This class consists of various test sequences on the ui_traits class.
 */
class ui_traits_test {
  public:
    /**
     * This function tests if a date can be read to and from a UI string to return the same value.
     */
    void test_dates();

    /**
     * Test the "ask_query" method
     */
    void test_ask_query();

    /**
     * Test the "ask_user" method
     */
    void test_ask_url();

    /**
     * Test the "ask_user" method
     */
    void test_ask_user();

    int main(int argc, char * * argv);

    /**
     * Pretty-print the content of a_info on a_os
     */
    void show_ask_info(std::ostream & a_os, const ui_traits::ask_info & a_info);

};

} // namespace expt
#endif
