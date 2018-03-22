#ifndef COMPCHEM_TEST_XML_WRITER_HPP
#define COMPCHEM_TEST_XML_WRITER_HPP

// ------------------------------------------------------------
// class: test_xml_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace data_access { class xml_writer_proxy; } 

namespace data_access {

class test_xml_writer : public utility::unit_test
{
  private:
    /**
     * The representative for this unit-test
     */
    static test_xml_writer s_exemplar;


  public:
    /**
     * Test the xml_writer class.
     * 
     * Failed tests are indicated by an uncaught exception.
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("Test data_access/test_xml_writer");
    }


};

} // namespace data_access
#endif
