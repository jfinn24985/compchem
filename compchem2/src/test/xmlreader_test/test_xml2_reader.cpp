// ------------------------------------------------------------
// class: test_xml2_reader
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/xmlreader_test/test_xml2_reader.hpp"
#include "utility/program_options.hpp"

namespace xmlreader {

/**
 * The singleton test object.
 */
test_xml2_reader test_xml2_reader::s_exemplar;

void test_xml2_reader::process_node(xml_reader & a_reader) const 
{
  log () << WTCH << "Depth: [" << a_reader.depth () << "]\tType: [" << a_reader.node_type () << "]\tName: [" << a_reader.name () << "]\tIs Empty: [" << a_reader.empty () << "]\n";
  log () << WTCH << "Prefix: [" << a_reader.prefix () << "]\tLocal name: [" << a_reader.local_name () << "]\n";
  if (not a_reader.is_attribute ())
  {
    if (a_reader.has_attribute ("PUBLIC"))
    {
      log () << WTCH << "PUBLIC: [" << a_reader.attribute ("PUBLIC") << "]\n";
    }
    if (a_reader.has_attribute ("SYSTEM"))
    {
      log () << WTCH << "SYSTEM: [" << a_reader.attribute ("PUBLIC") << "]\n";
    }
  }
  if (a_reader.node_type () == xmlreader::xml_reader::START_ELEMENT && a_reader.has_attributes ())
  {
    static const std::string l_name ("chkcalc-config");
    static const std::string l_attr_name ("version");
    if (l_name == a_reader.name ())
    {
      if (a_reader.has_attribute (l_attr_name))
      {
        log () << WTCH << "Special attribute: " << l_attr_name << "=\"" << a_reader.attribute (l_attr_name) << "\"\n";
      }
    }
    log () << WTCH << "start attributes\n";
    a_reader.first_attribute ();
    do
    {
      process_node (a_reader);
    }
    while (a_reader.next_attribute ());
    a_reader.to_element ();
    log () << WTCH << "end attributes\n";
  }
  if (a_reader.node_type () == xmlreader::xml_reader::DOCTYPE && a_reader.has_attributes ())
  {
    log () << WTCH << "start attributes\n";
    a_reader.first_attribute ();
    do
    {
      process_node (a_reader);
    }
    while (a_reader.next_attribute ());
    a_reader.to_element ();
    log () << WTCH << "end attributes\n";
  }
  if (a_reader.has_value ())
  {
    log () << WTCH << "value = \"" << a_reader.value () << "\"\n";
  }

}

/**
 * Override in derived classes to perform unit test.
 */
void test_xml2_reader::main_test() const 
{
  // This initialize the library and check potential ABI mismatches
  // between the version it was compiled for and the actual shared
  // library used.
  LIBXML_TEST_VERSION
  
  for (std::vector < std::string >::const_iterator l_filename (m_files.begin ()); l_filename != m_files.end (); ++l_filename)
  {
    log () << COMM << "Test reading XML filename " << *l_filename << "\n";
    if (boost::filesystem::exists (*l_filename))
    {
      xmlreader::xml_reader l_reader (*l_filename);
      while (l_reader.read ())
      {
        process_node (l_reader);
      }
      if (not l_reader.is_doc_valid ())
      {
        log () << IFNO << "Error occurred while processing XML filename " << *l_filename << "\n";
      }
    }
    else
    {
      log () << EXCP << "ERROR, could not find input file \"" << *l_filename << "\"\n";
    }
    log () << COMM << "End reading XML filename " << *l_filename << "\n";
  }

}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_xml2_reader::register_options() 
{
  utility::program_options::helper ().public_options ().add_options ()("xmlfile,i", boost::program_options::value< std::vector< std::string > > (&m_files)->composing (), "A file to test using the xmlreader (may be used multiple times)");
  

}


} // namespace xmlreader
