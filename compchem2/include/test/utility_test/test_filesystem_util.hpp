#ifndef COMPCHEM_TEST_FILESYSTEM_UTIL_HPP
#define COMPCHEM_TEST_FILESYSTEM_UTIL_HPP

// ------------------------------------------------------------
// class: test_filesystem_util
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/filesystem.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace utility { class program_options; } 

namespace utility {

class test_filesystem_util : public unit_test {
  private:
    /**
     * The name of a file to test the 'compare' method
     */
    std::string m_compare_fname;

    static test_filesystem_util exemplar;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("Test filesystem utilities.");
    }


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();


  public:
    test_filesystem_util()
    : unit_test ()
    , m_compare_fname ()
    {
      register_options ();
    }

    virtual ~test_filesystem_util()
    {}

};

} // namespace utility
#endif
