#ifndef COMPCHEM_DATA_SCHEME_TEST_XML2_URI_HPP
#define COMPCHEM_DATA_SCHEME_TEST_XML2_URI_HPP

// ------------------------------------------------------------
// class: test_xml2_uri
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include "xmlreader/xml2_uri.hpp"
#include <string>

namespace xmlreader {

class test_xml2_uri : public utility::unit_test
{
  private:
    static test_xml2_uri s_exemplar;


  public:
    /**
     * Override in derived classes to perform unit test.
     * 
     * Failed tests are indicated by an uncaught exception.
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("libxml2 URI/basic_uri test");
    }

    /**
     * Parse and print parts of a_uri
     */
    void print_uri(const std::string& a_uri) const;


};

} // namespace xmlreader
#endif
