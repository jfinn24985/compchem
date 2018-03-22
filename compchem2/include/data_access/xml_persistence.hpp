#ifndef COMPCHEM_XML_PERSISTENCE_HPP
#define COMPCHEM_XML_PERSISTENCE_HPP

// ------------------------------------------------------------
// class: xml_persistence
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "xmlreader/xml2_reader.hpp"
#include "data_access/persistence.hpp"
#include "utility/call_once.hpp"
#include "data_access/names.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#include <string>

namespace data_scheme { class experiment; } 
namespace data_access { class manager; } 
namespace data_access { class meta_datum; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class project; } 
namespace data_scheme { class program; } 
namespace data_access { class xml_access_helper; } 
namespace data_access { class xml_write_visitor; } 
namespace data_scheme { class basic_datum; } 

namespace data_access {

typedef xmlreader::xml_reader xml_reader_t;
/**
 * The base class for the XML-based object persistence system.
 */
class xml_persistence : public persistence
{
  private:
    /**
     * The base path for the parent file. This is only present if m_url is a relative path.
     */
    boost::filesystem::path m_base;

    /**
     * This contains a literal representation of non-element
     * nodes at the document level.  Note that on writing these
     * nodes will be located between the DOCTYPE node and the
     * document element regardless of their original position.
     */
    std::string m_header;

    /**
     * The possibly relative path of the file.
     */
    boost::filesystem::path m_url;


  public:
    /**
     * Is the given tag name an allowed document element?
     * 
     * \result implies a_tag_name = (program|project|memo)
     */
    static bool allowed_document_element(const std::string & a_tag_name);
    /**
     * The base part. This is non-empty if path() is relative.
     */
    const boost::filesystem::path& base() const throw ()
    {
      return m_base;
    }

    /**
     * The base part. This will be non-empty if path() is relative.
     * 
     * \pre exists(a_base)
     */
    void base(const boost::filesystem::path& a_base);


  private:
    /**
     * Attempt to find element with a_doc_elem and a_attr_name="a_attr_value"
     * in the file with a_filename.  Return true if found.
     */
    bool check_candidate(const boost::filesystem::path & a_filename, const std::string & a_doc_elem_name, const std::string & a_attr_name, const std::string & a_attr_value);

    /**
     * Attempt to convert m_uri into a filename.  May use parent filename as base. If
     * m_open is true and path is not found then will assume file is new. If m_open is
     * false and path is not found then will assume link is broken and will search for 
     * possible matches.
     */
    boost::filesystem::path find_file();

    /**
     * Attempt to find an XML file with a document element that matches top(). 
     * Use a_start_path as the starting position (unless it is empty.)
     * 
     * \pre is_directory (a_start_path)
     * 
     * TODO: Cannot handle filenames ".a.b" because of boost::path exception with
     * default filename checker...
     */
    boost::filesystem::path find_link(const boost::filesystem::path & a_start_path);


  public:
    /**
     * This returns the fully qualified filename of the xml file. This may involve
     * prepending the base() path to path().
     */
    boost::filesystem::path full_path() const
    {
      return (m_base.empty () ? m_url : m_base / m_url);
    }

    /**
     * Get the header section of the XML document
     */
    const std::string & header() const throw ()
    {
      return m_header;
    }


  private:
    /**
     * no assign
     */
    persistence & operator=(const persistence & source);

    /**
     * Deserialise XML document in a_reader to a_parent.
     * 
     * \pre not is_open
     * \post is_open
     */
    static void parse(data_scheme::basic_datum & a_parent, xml_reader_t & a_reader, std::string & a_header, bool a_is_part);


  public:
    /**
     * Get the current path
     */
    const boost::filesystem::path& path() const throw ()
    {
      return m_url;
    }

    /**
     * Set the current path
     */
    void path(const boost::filesystem::path& a_path)
    {
      m_url = a_path;
    }

    /**
     * Read the document into a_top object.
     */
    void read();

    /**
     * Parse the (part) document in the string. The parsed content 
     * is appended to a_parent.
     */
    static void read_string(data_scheme::basic_datum & a_parent, const std::string& a_buffer, bool a_is_part=true);

    /**
     * The full path.
     */
    std::string uri_path() const
    {
      return full_path ().string ();
    }

    /**
     * The URI scheme of the persistent storage. Currently this will be 
     * 'file' for XML file. Future extensions to 'odbc' for databases is planned.
     */
    std::string uri_scheme() const
    {
      return std::string("file");
    }

    /**
     * The combination of the URI scheme and a (possibly relative) path to the 
     * persistent storage. 
     */
    std::string uri() const
    {
      std::string Result;
      if (m_url.is_complete ())
      {
        Result.reserve (m_url.string ().size () + 8);
        Result.assign (uri_scheme ());
        Result.append ("://");
        Result.append (m_url.string ());
      }
      else
      {
        Result.assign (m_url.string ());
      }
      return Result;
    }

    /**
     * Write the objects descendent from top() onto the URL
     */
    void write() const;

    /**
     * This initialises the object but connecting to an actual file is delayed 
     * until read or write are called.
     */
    xml_persistence(data_scheme::basic_datum & a_top, const boost::filesystem::path & a_path);

    ~xml_persistence() throw ();


  private:
    /**
     * no copy
     */
    xml_persistence(const persistence & source);

    /**
     * no copy
     */
    xml_persistence(const xml_persistence & source);


};

// This function initialises the XML persistence library. It is
// automatically called only once via the call_once template
// instantiation below. It is never necessary to call this
// function in any other way.
void initialise_xml_persistence_library_private ();

// Create dummy object to initialise xml_persistence lib.
static utility::call_once s_xml_persistence_initialise_dummy (&initialise_xml_persistence_library_private);


} // namespace data_access
#endif
