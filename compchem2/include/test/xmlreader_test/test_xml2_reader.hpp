#ifndef COMPCHEM_DATA_SCHEME_TEST_XML2_READER_HPP
#define COMPCHEM_DATA_SCHEME_TEST_XML2_READER_HPP

// ------------------------------------------------------------
// class: test_xml2_reader
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#include "xmlreader/xml2_reader.hpp"
#include <vector>
#include <string>

namespace utility { class program_options; } 

namespace xmlreader {

struct test_xml2_reader : public utility::unit_test {
  private:
    /**
     * A list of XML files to read.
     */
    std::vector<std::string> m_files;

    /**
     * The singleton test object.
     */
    static test_xml2_reader s_exemplar;


  public:
    void process_node(xml_reader & a_reader) const;

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
    virtual void register_options();


  public:
    std::string title() const
    {
      return "xmlreader class test";
    }


  private:
    test_xml2_reader()
    : m_files ()
    {
      register_options ();
    }


  public:
    ~test_xml2_reader()
    {}

};

} // namespace xmlreader
#endif
