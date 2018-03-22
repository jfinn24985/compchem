#ifndef COMPCHEM_TEST_TABLE_WRITER_HPP
#define COMPCHEM_TEST_TABLE_WRITER_HPP

// ------------------------------------------------------------
// class: test_table_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace report { namespace filetype { class basic_markup; }  } 
namespace report { namespace filetype { class table_writer; }  } 

namespace report {

class test_table_writer : public utility::unit_test
{
  private:
    /**
     * Ctor
     */
    test_table_writer() {}


  public:
    ~test_table_writer() {}

    /**
     * Override in derived classes to perform unit test.
     * 
     * Failed tests are indicated by an uncaught exception.
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("test report/table_writer");
    }


  private:
    /**
     * Singleton for this test class.
     */
    static test_table_writer s_exemplar;


};

} // namespace report
#endif
