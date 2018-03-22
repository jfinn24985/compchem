#ifndef TEST_POSIX_HPP
#define TEST_POSIX_HPP

// ------------------------------------------------------------
// class: test_posix
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <boost/bind.hpp>

#include <vector>
#include <string>

namespace utility { class epoch_time; } 
namespace utility { class program_options; } 
namespace utility { class filesystem_properties; } 

namespace utility {

/**
 * A set of test cases for the POSIX Framework classes
 */
class test_posix : public unit_test {
  private:
    /**
     * The set of files to look at.
     */
    std::vector<std::string> m_filelist;

    /**
     * Create a static instance to register this set of test cases.
     */
    static test_posix s_exemplar;


  public:
    /**
     * Perform unit test on POSIX helpers
     */
    void main_test() const;

    explicit test_posix()
    : unit_test ()
    , m_filelist ()
    {
      register_options ();
    }

    virtual ~test_posix()
    {}

    /**
     * Pretty print the file properties to a_stream.
     */
    void print_file_properties(const std::string& a_filename, const filesystem_properties & a_props, std::ostream & a_os) const;


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();


  public:
    virtual std::string title() const { return "POSIX helpers"; }

};

} // namespace utility
#endif
