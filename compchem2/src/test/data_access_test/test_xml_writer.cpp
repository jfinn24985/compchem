// ------------------------------------------------------------
// class: test_xml_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/data_access_test/test_xml_writer.hpp"
#include "data_access/xml_writer.hpp"

// Manual includes
#include <sstream>
// -
namespace data_access {

/**
 * The representative for this unit-test
 */
test_xml_writer test_xml_writer::s_exemplar;

/**
 * Test the xml_writer class.
 * 
 * Failed tests are indicated by an uncaught exception.
 */
void test_xml_writer::main_test() const 
{
  // Want to produce the following xml document
  std::string l_default (
    "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
    "<!DOCTYPE a PUBLIC \"//my-doctype//version//1.0.0\" \"mydoctype.dtd\">\n"
    "<a><b x=\"1\" y=\"2\"><c>Hello World<d/></c><c><d/></c></b></a>"
  );
  {
    std::ostringstream l_ios;
    {
      xml_writer_proxy l_top (l_ios, "a", "//my-doctype//version//1.0.0", "mydoctype.dtd");
      {
        xml_writer_proxy l_b1 (l_top, "b");
        l_b1.attribute ("x", "1");
        l_b1.attribute ("y", "2");
        {
          xml_writer_proxy l_c1 (l_b1, "c");
          l_c1.add_text ("Hello World");
          xml_writer_proxy l_d1 (l_c1, "d");
        }
        xml_writer_proxy l_c2 (l_b1, "c");
        {
          xml_writer_proxy l_d2 (l_c2, "d");
        }
      }
    }
    log () << COMM << "Trying for : ---\n" << l_default << "\n---\n";
    log () << WTCH << "Got: ---\n" << l_ios.str () << "\n---\n";
    bool_test (l_default == l_ios.str (), "target and result match");
  }
  {
    std::ostringstream l_ios;
    {
      xml_writer_proxy l_top (l_ios, "a", "//my-doctype//version//1.0.0", "mydoctype.dtd");
      xml_writer_proxy l_b1 (l_top, "b");
      l_b1.attribute ("x", "1");
      l_b1.attribute ("y", "2");
      xml_writer_proxy l_c1 (l_b1, "c");
      l_c1.add_text ("Hello World");
      xml_writer_proxy l_d1 (l_c1, "d");
      xml_writer_proxy l_c2 (l_b1, "c");
      xml_writer_proxy l_d2 (l_c2, "d");
    }
    log () << COMM << "Trying for : ---\n" << l_default << "\n---\n";
    log () << WTCH << "Got: ---\n" << l_ios.str () << "\n---\n";
    bool_test (l_default == l_ios.str (), "target and result match");
  }

}


} // namespace data_access
