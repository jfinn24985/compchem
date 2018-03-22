// ------------------------------------------------------------
// class: xml_persistence
// $revision:$
// $log:$
// ------------------------------------------------------------


#define NO_LOG
#include "data_access/xml_persistence.hpp"
#include "data_scheme/experiment.hpp"
#include "data_access/manager.hpp"
#include "data_access/meta_datum.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/program.hpp"
#include "data_access/xml_access_helper.hpp"
#include "data_access/xml_write_visitor.hpp"
#include "data_scheme/basic_datum.hpp"

// Manual includes -
#include <stack>
#include <fstream>
#include <sstream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include "utility/filesystem.hpp"
#include "xmlreader/xml2_uri.hpp"
// -
namespace data_access {

/**
 * Is the given tag name an allowed document element?
 * 
 * \result implies a_tag_name = (program|project|memo)
 */
bool xml_persistence::allowed_document_element(const std::string & a_tag_name)
{
  return a_tag_name == names::program_name ()
      or a_tag_name == names::project_name ()
      or a_tag_name == names::memo_name ();
}

/**
 * The base part. This will be non-empty if path() is relative.
 * 
 * \pre exists(a_base)
 */
void xml_persistence::base(const boost::filesystem::path& a_base) 
{
  COMPCHEM_REQUIRE(boost::filesystem::exists (a_base), "Base path for xml_file object must already exist.");
  m_base = a_base;
}

/**
 * Attempt to find element with a_doc_elem and a_attr_name="a_attr_value"
 * in the file with a_filename.  Return true if found.
 */
bool xml_persistence::check_candidate(const boost::filesystem::path & a_filename, const std::string & a_doc_elem_name, const std::string & a_attr_name, const std::string & a_attr_value) 
{
  COMPCHEM_REQUIRE (boost::filesystem::exists (a_filename), "check_candidate requires a valid filename.");
  bool Result (false);
  bool l_fail (false);
  // Create an xml_reader.
  xml_reader_t l_tmp (a_filename.native_file_string ());
  // Check doctype and document element
  while (not l_fail and l_tmp.read ())
  {
    switch (l_tmp.node_type ())
    {
    case xml_reader_t::DOCTYPE:
      // set fail to true if DOCTYPE element is not a_doc_elem_name.
      l_fail = (l_tmp.name () != a_doc_elem_name);
      break;
    case xml_reader_t::START_ELEMENT:
      // Check first element for name and attribute.
      if  (l_tmp.name () == a_doc_elem_name)
      {
        Result = (l_tmp.has_attribute (a_attr_name) and a_attr_value == l_tmp.attribute (a_attr_name));
      }
      // set fail to true on first element.
      l_fail = true;
      break;
    default:
      // Do nothing in all other cases
      break;
    }
  }
  return Result;

}

/**
 * Attempt to convert m_uri into a filename.  May use parent filename as base. If
 * m_open is true and path is not found then will assume file is new. If m_open is
 * false and path is not found then will assume link is broken and will search for 
 * possible matches.
 */
boost::filesystem::path xml_persistence::find_file() 
{
  boost::filesystem::path Result;
  ////////////////////////////
  // Create two test paths
  // 1: The base m_url as a complete path
  const boost::filesystem::path l_url (utility::filesystem::standardize (m_url));
  // 3: The (parent_doc).branch_dir  (if parent_doc is a file)
  boost::filesystem::path l_parurl;
  // top () is either a memo, project or program
  const data_scheme::memo * l_memo (dynamic_cast< const data_scheme::memo * > (&(top ())));
  if (NULL != l_memo)
  {
    if (names::file_scheme () == l_memo->page ().section ().uri_scheme ())
    {
      l_parurl = l_memo->page ().section ().uri_path ();
      l_parurl = l_parurl.branch_path ();
      COMPCHEM_CHECK (boost::filesystem::exists (l_parurl) and boost::filesystem::is_directory (l_parurl)
                      , "Invalid path for parent document.");
    }
  }
  else
  {
    const data_scheme::project * l_project (dynamic_cast< const data_scheme::project * > (&(top ())));
    if (NULL != l_project)
    {
      if (names::file_scheme () == l_project->site ().uri_scheme ())
      {
        l_parurl = l_project->site ().uri_path ();
        l_parurl = l_parurl.branch_path ();
        COMPCHEM_CHECK (boost::filesystem::exists (l_parurl) and boost::filesystem::is_directory (l_parurl)
                        , "Invalid path for parent document.");
      }
    }
    else
    {
      COMPCHEM_CHECK (NULL != dynamic_cast< const data_scheme::program * > (&(top ()))
                      , "Invalid owner of persistence object, not a memo, project or program");
    }
  }
  /////////////////////////////
  // Test the three paths
  // 1: m_base / m_url.
  if (not m_base.empty () and boost::filesystem::exists (m_base / m_url))
  {
    // m_base and m_url are set correctly.
    Result = m_base / m_url;
    DLOG ("Found file at (base / path): ");
    DLOG (Result.string ());
    DLOG ("\n");
  }
  // 2: parent_doc.branch_dir / m_url
  else if (not l_parurl.empty () and boost::filesystem::exists (l_parurl / m_url))
  {
    // m_url is relative to parent, set m_base.
    m_base = l_parurl;
    Result = l_parurl / m_url;
    DLOG ("Found file at (parent_doc.dir / path): ");
    DLOG (Result.string ());
    DLOG ("\n");
  }
  // 3: m_url as a complete path.
  else if (boost::filesystem::exists (l_url))
  {
  // Complete path mactches.
    m_url = utility::filesystem::standardize (m_url);
  // Attempt to make relative to parent if it has the same scheme.
    if (not l_parurl.empty ())
    {
      boost::filesystem::path l_tmp = utility::filesystem::relative_path (l_parurl, m_url);
      // If l_tmp != m_url then we can be relative,
      if (l_tmp != m_url)
      {
        m_base = l_parurl;
        m_url = l_tmp;
      }
    }
    Result = l_url;
    DLOG ("Found file at (path): ");
    DLOG (Result.string ());
    DLOG ("\n");
  }
  // If not found then broken-link [find_file is never called for a new document]
  else
  {
    // This throws when the link can not be repaired. Note l_parurl was checked
    // for being an existing directory earlier.
    Result = find_link (l_parurl);
  }
  COMPCHEM_ALWAYS (not Result.empty (), "Can not find path from URL = " + m_url.string ());
  return Result.native_file_string ();

}

/**
 * Attempt to find an XML file with a document element that matches top(). 
 * Use a_start_path as the starting position (unless it is empty.)
 * 
 * \pre is_directory (a_start_path)
 * 
 * TODO: Cannot handle filenames ".a.b" because of boost::path exception with
 * default filename checker...
 */
boost::filesystem::path xml_persistence::find_link(const boost::filesystem::path & a_start_path) 
{
  COMPCHEM_REQUIRE (a_start_path.empty () or (boost::filesystem::exists (a_start_path) and boost::filesystem::is_directory (a_start_path)), "Start directory is not a directory.");
  // Initialise search.
  // Scan directories for relative_path/leaf_name match.
  // Scan directories for leaf_name match.
  // Scan directories for XML files.
  // On candidate filename, test file for match 
  boost::filesystem::path l_src (m_url);           // Path from our attribute
  boost::filesystem::path Result;
  std::deque < boost::filesystem::path > l_dirs;
  // -------------------------
  // -- Get initial paths.
  //If l_src is not complete, assume relative to a_start_path document.
  if (not l_src.is_complete () and not a_start_path.empty ())
  {
    l_src = a_start_path / l_src;
  }
  // Set branch path of src as first choice
  if (boost::filesystem::exists (l_src.branch_path ()) and boost::filesystem::is_directory (l_src.branch_path ()))
  {
    l_dirs.push_back (l_src.branch_path ());
  }
  // Add a_start_path directory too.
  if (not a_start_path.empty ())
  {
    l_dirs.push_back (a_start_path);
  }
  // Add any other search dirs..
  const xml_access_helper & l_help (xml_access_helper::helper ());
  if (not l_help.empty ())
  {
    for (xml_access_helper::const_iterator l_i (l_help.begin ()); l_i != l_help.end (); ++l_i)
    {
      boost::filesystem::path l_extra (*l_i);
      if (not l_extra.is_complete ())
      {
        l_extra = boost::filesystem::initial_path () / l_extra;
      }
      if (boost::filesystem::exists (l_extra) and boost::filesystem::is_directory (l_extra))
      {
        l_dirs.push_back (l_extra);
      }
    }
  }
  // --------------------------
  // -- Start search
  const boost::filesystem::directory_iterator l_end;
  // Get the values to search for...
  const std::string l_docelem_name (top ().serial_name ());
  const std::string l_keyattr_name (meta_datum::by_name (l_docelem_name).key_attribute);
  std::string l_keyattr_value;
  if (top ().get_attribute (l_keyattr_name, l_keyattr_value))
  {
    while (Result.empty () and not l_dirs.empty ())
    {
      boost::filesystem::directory_iterator l_iterator (l_dirs.front ());
      l_dirs.pop_front ();
      for (;Result.empty () and l_iterator != l_end; ++l_iterator)
      {
        try
        {
          if (boost::filesystem::exists (*l_iterator))
          {
            if (boost::filesystem::is_directory (*l_iterator))
            {
              // Push dirs. on front
      	  l_dirs.push_front (*l_iterator);
            }
            else
            {
      	    // Test files
              // TODO perform a filename match [include and exclude]
      	    if (check_candidate (l_iterator->native_file_string (), l_docelem_name, l_keyattr_name, l_keyattr_value))
              {
                Result = *l_iterator;
              }
            }
          }
        }
        catch (const boost::filesystem::filesystem_error & a_err) // Ignore filesystem errors.
        {
          utility::implementation::log () << a_err.what () << "\n Error code: " << a_err.error () << "\n";
        }
      }
    }
  }
  return Result;

}

/**
 * Deserialise XML document in a_reader to a_parent.
 * 
 * \pre not is_open
 * \post is_open
 */
void xml_persistence::parse(data_scheme::basic_datum & a_parent, xml_reader_t & a_reader, std::string & a_header, bool a_is_part)

{
  // The header string collector (should be empty for part docs)
  std::ostringstream l_header;
  // Object stack
  std::stack < data_scheme::basic_datum * >l_object_stack;
  // If not the full document then push parent element.
  if (a_is_part)
  {
    l_object_stack.push (&a_parent);
  }
  
  // Parse document
  while (a_reader.read ()and a_reader.is_doc_valid ())
  {
    DLOG ("Processing node name [");
    DLOG (a_reader.name ());
    DLOG ("] D = ");
    DLOG (a_reader.depth ());
    DLOG (" NT = ");
    DLOG (a_reader.node_type ());
    DLOG ("\n");
    switch (a_reader.node_type ())
    {
    case xml_reader_t::TEXT:
#ifdef XML_XML2
    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
    case XML_READER_TYPE_WHITESPACE:
#endif
      if (l_object_stack.empty ())
      {
        l_header << a_reader.value ();
      }
      else
      {
        std::string l_temp (a_reader.value ());
        l_object_stack.top ()->add_text_swallow (l_temp);
      }
      break;
    case xml_reader_t::PI:
      if (l_object_stack.empty ())
      {
        l_header << "<? ";
        l_header << a_reader.name ();
        l_header << " ";
        l_header << a_reader.value ();
        l_header << " ?>";
      }
      else
      {
        std::string l_temp (a_reader.value ());
        l_object_stack.top ()->add_pi_swallow (a_reader.name (), l_temp);
      }
      break;
    case xml_reader_t::COMMENT:
      if (l_object_stack.empty ())
      {
        l_header << "<!-- ";
        l_header << a_reader.value ();
        l_header << " -->";
      }
      else
      {
        std::string l_temp (a_reader.value ());
        l_object_stack.top ()->add_comment_swallow (l_temp);
      }
      break;
    case xml_reader_t::END_ELEMENT:
      COMPCHEM_CHECK (a_reader.name () == l_object_stack.top ()->serial_name (), "Node close name was not what was expected.");
      l_object_stack.pop ();
      DLOG ("Popped stack");
      if (l_object_stack.empty ())
      {
        DLOG (", stack empty.\n");
      }
      else
      {
        DLOG (", current top object is [");
        DLOG (l_object_stack.top ()->serial_name ());
        DLOG ("]\n");
      }
      break;
    case xml_reader_t::DOCTYPE:
    {
      // At document.
      const std::string l_name (a_reader.name ());
      COMPCHEM_CHECK (l_object_stack.empty (), "At document type, but have objects on stack.");
      COMPCHEM_ALWAYS (allowed_document_element (l_name), "Invalid XML document [" + l_name + "] document is not allowed.");
      COMPCHEM_ALWAYS (l_name == a_parent.serial_name (), "Incorrect XML document [" + l_name + "], expected [" + a_parent.serial_name () + "]");
      // COMPCHEM_CHECK (l_ok, "Incomplete document header without a public or system ID.");
      // TODO Check pub_id and sys_id
    }
    break;
    case xml_reader_t::START_ELEMENT:
    {
      if (l_object_stack.empty ())
      {
        // At document element.
        const std::string l_name (a_reader.name ());
        COMPCHEM_ALWAYS (allowed_document_element (l_name), "Invalid XML document element [" + l_name + "] document elment is incorrect.");
        COMPCHEM_ALWAYS (l_name == a_parent.serial_name (), "Incorrect XML document element [" + l_name + "], expected [" + a_parent.serial_name () + "]");
        l_object_stack.push (&a_parent);
      }
      else
      {
        const std::string l_name (a_reader.name ());
        std::string l_value (a_reader.attribute (meta_datum::by_name (l_name).key_attribute));
        l_object_stack.push (&(l_object_stack.top ()->add_child (l_name, l_value)));
      }
      if (a_reader.has_attributes ())	// Process attrs
      {
        a_reader.first_attribute ();
        do
        {
          l_object_stack.top ()->set_attribute (a_reader.name (), a_reader.value ());
        }
        while (a_reader.next_attribute ());
        a_reader.to_element ();
      }
      // Undo push for empty nodes as we get no END_ELEMENT.
      if (a_reader.empty ())
      {
        l_object_stack.pop ();
      }
    }
    break;
    default: // Everything else is ignored
      utility::implementation::log () << "Ignoring node with name [" << a_reader.name () << "] and type: " << a_reader.node_type () << "\n";
      break;
    }
  };
  // Copy header.
  if (a_is_part)
  {
    COMPCHEM_ALWAYS (l_header.str ().empty (), "Found header elements in a part document.");
  }
  else
  {
    if (not l_header.str ().empty ())
    {
      a_header.assign (l_header.str ());
    }
  }
  COMPCHEM_ALWAYS (l_object_stack.size () == (a_is_part ? 1 : 0), "Parsing failed, invalid document.");

}

/**
 * Read the document into a_top object.
 */
void xml_persistence::read() 
{
  // Find file
  boost::filesystem::path l_filename = find_file ();
  DLOG ("find_file returned : "); DLOG (l_filename.string ()); DLOG ("\n");
  COMPCHEM_CHECK(boost::filesystem::exists (l_filename), "find_file should return an existing filename.");
  // Create an xml_reader.
  xml_reader_t l_reader (l_filename.native_file_string ());
  
  // Parse document
  parse (top (), l_reader, m_header, false);
  
  DLOG("Header : \""); DLOG(header ()); DLOG("\"\n");
  // Check parsing was OK.
  COMPCHEM_ALWAYS (l_reader.is_doc_valid (), "Reading of XML document failed.");

}

/**
 * Parse the (part) document in the string. The parsed content 
 * is appended to a_parent.
 */
void xml_persistence::read_string(data_scheme::basic_datum & a_parent, const std::string& a_buffer, bool a_is_part)

{
  // Find file
  std::istringstream l_doc (a_buffer);
  std::string l_header;
  // Create an xml_reader.
  xml_reader_t l_reader (l_doc);
  
  // Parse document
  parse (a_parent, l_reader, l_header, a_is_part);
  
  // Check parsing was OK.
  COMPCHEM_ALWAYS (l_reader.is_doc_valid (), "Reading of XML document failed.");

}

/**
 * Write the objects descendent from top() onto the URL
 */
void xml_persistence::write() const 
{
  DLOG("Full path for xml_file::write is \""); DLOG(uri_path ()); DLOG("\"\n");
  DLOG("Path for xml_file::write is \""); DLOG(path ().string ()); DLOG("\"\n");
  DLOG("Base for xml_file::write is \""); DLOG(base ().string ()); DLOG("\"\n");
  if (boost::filesystem::exists (full_path ()))
  {
    // Use an output buffer and then test for changes.
    std::basic_ostringstream< std::string::value_type >  l_outstream;
    // Write header
    {
      // Write the data into a buffer.
      xml_write_visitor l_guest (l_outstream
                                 , top ().serial_name ()
                                 , xml_access_helper::helper ().public_id ()
                                 , xml_access_helper::helper ().system_id ());
      l_outstream << header ();
      l_guest.visit_basic_datum (top ());
    }
    // Compare files for changes
    const std::string& l_outstr (l_outstream.str ());
    if (boost::filesystem::file_size (full_path ()) != (l_outstr.size () * sizeof (std::string::value_type)))
    { 
      // Just overwrite old file if sizes are different.
      std::basic_ofstream< std::string::value_type > l_outfile (full_path ().native_file_string ().c_str ());
      l_outfile.write (l_outstr.data (), l_outstr.size ());
    }
    else
    {
      // Only really compare if files sizes are the same.
      std::basic_fstream< std::string::value_type > l_outfile (full_path ().native_file_string ().c_str ());
      COMPCHEM_ALWAYS (l_outfile, "Document exists but we can not read or write to it.");
      boost::scoped_array< std::string::value_type > l_buffer (new std::string::value_type [l_outstr.size ()]);
      const unsigned int l_read_size (l_outfile.readsome (l_buffer.get (), l_outstr.size ()));
      COMPCHEM_ALWAYS(l_read_size == l_outstr.size (), "Size of file read was different to reported file size.");
      if (0 != std::memcmp (l_buffer.get (), l_outstr.data (), l_outstr.size ()))
      {
        // Files are not equal so overwrite.
        l_outfile.write (l_outstr.data (), l_outstr.size ());
      }
    }
  }
  else
  {
    if (not boost::filesystem::exists (full_path ().branch_path ()))
    {
      // Create directory if necessary
      boost::filesystem::create_directory (full_path ().branch_path ());
    }
    // No existing file, write the file directly.
    std::basic_ofstream< std::string::value_type > l_outfile (full_path ().native_file_string ().c_str ());
    // Write the data into a buffer.
    xml_write_visitor l_guest (l_outfile
                              , top ().serial_name ()
                              , xml_access_helper::helper ().public_id ()
                              , xml_access_helper::helper ().system_id ());
    l_outfile << header ();
    l_guest.visit_basic_datum (top ());
  }

}

/**
 * This initialises the object but connecting to an actual file is delayed 
 * until read or write are called.
 */
xml_persistence::xml_persistence(data_scheme::basic_datum & a_top, const boost::filesystem::path & a_path) 
: persistence (a_top)
, m_base ()
, m_header ()
, m_url (a_path)
{
  COMPCHEM_REQUIRE(allowed_document_element (a_top.serial_name ()), "Invalid document element");
}

xml_persistence::~xml_persistence() throw () 
{}

static std::auto_ptr < persistence >
create_xml_persistence (data_scheme::basic_datum & a_top, const std::string &a_uri)
{
  const xmlreader::uri l_uri (a_uri);
  COMPCHEM_REQUIRE (l_uri.scheme ().empty () or l_uri.scheme () == names::file_scheme ()
                   , "Invalid URI scheme");
  return std::auto_ptr< persistence > (new xml_persistence (a_top, l_uri.path ()));
}
void initialise_xml_persistence_library_private ()
{
  manager::server ().register_creator (names::file_scheme (), &create_xml_persistence);
}


} // namespace data_access
