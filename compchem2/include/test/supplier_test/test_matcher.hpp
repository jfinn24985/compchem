#ifndef COMPCHEM_TEST_MATCHER_HPP
#define COMPCHEM_TEST_MATCHER_HPP

// ------------------------------------------------------------
// class: test_matcher
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include "supplier/matcher_types.hpp"
#include <string>

namespace supplier { class matcher_string; } 
namespace utility { class program_options; } 
namespace supplier { class matcher_bytes; } 
namespace supplier { class matcher_end_pair; } 
namespace supplier { class matcher_lines; } 
namespace supplier { class matcher_regex_search; } 
namespace supplier { class matcher_start_pair; } 
namespace supplier { class matcher_string_search; } 
namespace supplier { class basic_matcher; } 

namespace supplier {

class test_matcher : public utility::unit_test {
  private:
    /**
     * The name of the gaussian file to scan
     */
    std::string m_gaussian_file;

    /**
     * The class exemplar
     */
    static test_matcher s_exmplr;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();

    /**
     * Test a matcher_bytes object.
     */
    void test_bytes(matcher_bytes & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_string_search object.
     */
    void test_end_pair(matcher_end_pair & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_lines object.
     */
    void test_lines(matcher_lines & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_string_search object.
     */
    void test_regex(matcher_regex & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_string_search object.
     */
    void test_regex_search(matcher_regex_search & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_string_search object.
     */
    void test_start_pair(matcher_start_pair & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_string_search object.
     */
    void test_string(matcher_string & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    /**
     * Test a matcher_string_search object.
     */
    void test_string_search(matcher_string_search & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    void test_first_matching(basic_matcher & a_matcher, const char *const a_buf, unsigned int a_sz, bool a_line_oriented) const;

    void test_first_matching_priv(basic_matcher & a_matcher, char const *const a_buf, unsigned int a_sz, bool a_line_oriented, std::string & a_result) const;


  public:
    test_matcher()
    : unit_test ()
    , m_gaussian_file ()
    {
      register_options ();
    }

    ~test_matcher() throw()
    {}

    std::string title() const
    {
      return std::string ("Test matcher_? classes");
    }

};

} // namespace supplier
#endif
