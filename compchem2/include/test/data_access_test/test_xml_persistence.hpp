#ifndef COMPCHEM_TEST_XML_PERSISTENCE_HPP
#define COMPCHEM_TEST_XML_PERSISTENCE_HPP

// ------------------------------------------------------------
// class: test_xml_persistence
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#include <string>

namespace data_scheme { class experiment; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class program; } 
namespace utility { class program_options; } 
namespace data_scheme { class project; } 
namespace data_access { class xml_persistence; } 
namespace data_access { class xml_write_visitor; } 
namespace data_access { class xml_access_helper; } 

namespace data_access {

/**
 * Test the xml_persistence and xml_write_visitor classes
 */
class test_xml_persistence : public utility::unit_test
{
  private:
    /**
     * The search "base" path
     */
    boost::filesystem::path m_base;

    /**
     * The search key
     */
    std::string m_key;

    /**
     * The search filename
     */
    boost::filesystem::path m_path;

    /**
     * The file name of the file to compare our result to.
     */
    std::string m_test_file;

    /**
     * Singleton instance.
     */
    static test_xml_persistence s_exemplar;


  public:
    /**
     * Override in derived classes to perform unit test.
     * 
     * Failed tests are indicated by an uncaught exception.
     */
    void main_test() const;


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();

    void set_base(const std::string& a_path)
    {
      m_base = a_path;
    }

    void set_path(const std::string& a_path)
    {
      m_path = a_path;
    }


  public:
    std::string title() const
    {
      return std::string ("Test xml_persistence class");
    }


  private:
    test_xml_persistence()
    : utility::unit_test ()
    , m_base ()
    , m_key ()
    , m_path ()
    , m_test_file ()
    {
      register_options ();
    }

  public:
    ~test_xml_persistence();


};

} // namespace data_access
#endif
