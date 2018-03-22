#ifndef PURPLETREE_XML2_READER_HPP
#define PURPLETREE_XML2_READER_HPP

// ------------------------------------------------------------
// class: xml2_reader
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include "xmlreader/xml_reader.hpp"

// Manual Includes
#include <iostream>
#include <stdexcept>
#include <tr1/memory>
extern "C"
{
// Use GNOME libxml2
#include <libxml/xmlreader.h>
}
// -
namespace xmlreader {

/**
 * Interface to the libxml2 XML reader type
 * 
 * The implementation can instantiate an object of this type by
 * calling the make_xml_reader function.
 * 
 * This class is to be used with the xml_reader template. Most methods 
 * do not test their preconditions here, assuming they have been tested 
 * in the xml_reader template.
 */
class xml2_reader_traits {
  public:
    typedef xmlTextReaderPtr reader_t;

    enum {
      START_ELEMENT= 1,
      ATTRIBUTE= 2,
      TEXT= 3,
      CDATA= 4,
      ENTITY= 5,
      ENTITY_DECL= 6,
      PI= 7,
      COMMENT= 8,
      DOCUMENT= 9,
      DOCTYPE= 10,
      DOCFRAG= 11,
      NOTATION= 12,
      END_ELEMENT= 15

    };

    /**
     * Constants that can be used to control optional aspects of the 
     * parser/reader.
     */
    enum
    {
      UNSUPPORTED= 0,/**
       * Value indicates a property is unsupported.
       */

      LOAD_DTD= XML_PARSER_LOADDTD,/**
       * Load the DTD while parsing the document.
       */

      DEFAULT_ATTRIBUTES= XML_PARSER_DEFAULTATTRS,/**
       * Will automatically set default attributes (implies
       * loading the DTD).
       */

      VALIDATE= XML_PARSER_VALIDATE,/**
       * Validate the document against the DTD (implies
       * loading the DTD.)
       */

      EXPAND_ENTITIES= XML_PARSER_SUBST_ENTITIES/**
       * Expand any entities into their final form (implies
       * loading the DTD)
       */


    };

    /**
     * Get value of attribute of give name from element
     * 
     * \pre (assumed) has_attribute (a_key)
     */
    static void attribute(const reader_t a_reader, const std::string & a_key, std::string & a_value)
    {
      to_string(xmlTextReaderGetAttribute (a_reader, reinterpret_cast< const xmlChar* > (a_key.c_str())), a_value);
    }

    /**
     * How many attributes are present?
     * 
     * \pre  (assumed) !is_attribute
     */
    static inline int attribute_count(const reader_t a_reader)
    {
      return xmlTextReaderAttributeCount (a_reader);
    }

    /**
     * Base URI of node
     */
    static void base_uri(const reader_t a_reader, std::string & a_value)
    {
      as_string (xmlTextReaderBaseUri (a_reader), a_value); }


  private:
    /**
     * Helpers for istream reader
     */
    static int close_callback(void * a_context) {
          // Do nothing
          return 0; }


  public:
    /**
     * Dump debugging information to error stream
     */
    static inline void debug(const reader_t a_reader);

    static void delete_reader(reader_t & a_reader) {
      if (a_reader != NULL) {
        xmlFreeTextReader (a_reader); }
      a_reader = NULL; }

    /**
     * How deep is this node?
     */
    static int depth(const reader_t a_reader) {
      return xmlTextReaderDepth (a_reader); }

    /**
     * Does element have no content?
     */
    static bool empty(const reader_t a_reader) {
      return (0 != xmlTextReaderIsEmptyElement (a_reader)); }

    /**
     * Move to first attribute of this element
     * 
     * \pre (assumed) has_attributes()
     * \post not has_attributes -> a_parser_okay = false
     */
    static void first_attribute(reader_t a_reader, bool & a_parser_okay) {
      const int l_result = xmlTextReaderMoveToFirstAttribute (a_reader);
      if (l_result != 1) {
        a_parser_okay = false;
        COMPCHEM_CHECK(false, "first_attribute() failed"); }}

    /**
     * Get the current setting of a parser property.
     */
    static bool get_parser_property(reader_t a_reader, int a_prop, bool & a_parser_okay)
    {
      bool Result (false);
      switch (xmlTextReaderGetParserProp(a_reader, a_prop))
      {
      case 0:
        break;
      case 1:
        Result = true;
        break;
      case -1: // fall through
      default:    
        a_parser_okay = false;
        COMPCHEM_CHECK(false, "get_parser_property failed");
      }
      return Result;
    }
    

    /**
     * Does element have attributes?
     * 
     * \pre (assumed) !is_attribute
     */
    static bool has_attributes(const reader_t a_reader) {
      return (1 == xmlTextReaderHasAttributes (a_reader)); }

    /**
     * Does element have attribute of give name?
     */
    static bool has_attribute(const reader_t a_reader, const std::string & a_key) {
      bool Result (false);
      xmlChar* l_value (xmlTextReaderGetAttribute (a_reader
                      , reinterpret_cast< const xmlChar* >(a_key.c_str())));
      if (NULL != l_value) {
        xmlFree(l_value);
        Result = true; }
      return Result; }

    /**
     * / Does element have a value?
     */
    static bool has_value(const reader_t a_reader) {
      return (1 == xmlTextReaderHasValue (a_reader)); }

    /**
     * Does the attribute have its default value
     * 
     * \pre is_attribute()
     */
    static bool is_default(const reader_t a_reader) {
      return (1 == xmlTextReaderIsDefault (a_reader)); }

    /**
     * local name of node
     */
    static void local_name(const reader_t a_reader, std::string & a_name)
    {
      as_string (xmlTextReaderConstLocalName (a_reader), a_name);
    }

    /**
     * name, possibly qualified, of node
     */
    static void name(const reader_t a_reader, std::string & a_name)
    {
      as_string (xmlTextReaderConstName (a_reader), a_name);
    }

    /**
     * URI of namespace of node
     */
    static void namespace_uri(const reader_t a_reader, std::string & a_value)
    {
      as_string (xmlTextReaderConstNamespaceUri (a_reader), a_value);
    }

    /**
     * Create a new reader
     */
    static reader_t new_reader(const std::string & a_filename) throw(std::logic_error) {
      reader_t Result (xmlNewTextReaderFilename (a_filename.c_str ()));
      if (Result == NULL) {
        throw std::logic_error ("Unable to open file: " + a_filename); }
      return Result; }

    static reader_t new_reader(std::istream & a_stream) throw(std::logic_error) {
      reader_t Result (xmlReaderForIO (&xml2_reader_traits::read_callback
    				   , &xml2_reader_traits::close_callback
    				   , (void*)(&a_stream)
    				   , "Xml2Reader/dummy/filename"
    				   , NULL 
    				   , 0));
      if (Result == NULL) {
        throw std::logic_error ("Unable to create reader for stream"); }
      return Result; }


  protected:
    static reader_t new_reader(const char * a_buffer, int a_sz) throw(std::logic_error) {
      reader_t Result (xmlReaderForMemory (a_buffer
    				      , a_sz
    				      , "Xml2Reader/dummy/filename"
    				      , NULL 
    				      , 0));
      if (Result == NULL) {
        throw std::logic_error ("Unable to create reader for buffer"); }
      return Result; }


  public:
    /**
     * Move to next attribute of an element
     * 
     * \pre is_attribute
     * 
     * \return false if no next attribute
     */
    static bool next_attribute(reader_t a_reader, bool & a_parser_okay) {
      const int Result (xmlTextReaderMoveToNextAttribute (a_reader));
      if (-1 == Result) {
        a_parser_okay = false; }
      return (Result == 1); }

    /**
     * Type of node
     */
    static int node_type(const reader_t a_reader) {
      return xmlTextReaderNodeType (a_reader); }

    /**
     * Prefix part of name
     */
    static void prefix(const reader_t a_reader, std::string & a_value)
    {
      as_string (xmlTextReaderConstPrefix (a_reader), a_value);
    }

    /**
     * Move the reader to the next node. 
     * 
     * \return false at end of document.
     * 
     * \pre !is_attribute
     */
    static bool read(reader_t a_reader, bool & a_parser_okay) {
      const int Result (xmlTextReaderRead(a_reader));
      if (-1 == Result) {
        a_parser_okay = false; }
      return (1 == Result); }


  private:
    /**
     * Helpers for istream reader -
     */
    static int read_callback(void * a_context, char * a_buffer, int a_len) {
      std::istream* l_in_stream = static_cast< std::istream* > (a_context);
      int Result(-1);
      // DEBUG fprintf (stderr, "Asked to put data in %p[0-%d]\n", a_buffer, a_len);
      l_in_stream->read (a_buffer, a_len);
      if (l_in_stream->good () || l_in_stream->eof ()) {
        Result = l_in_stream->gcount ();
        } // fprintf(stderr, "(%d)[%.*s]\n", Result, Result, a_buffer);
      return Result; }


  public:
    /**
     * Set a parser property.
     */
    static void set_parser_property(reader_t a_reader, int a_prop, bool a_set, bool & a_parser_okay)
    {
      if (0 != xmlTextReaderSetParserProp(a_reader, a_prop, (a_set?1:0)))
      {
        a_parser_okay = false;
        COMPCHEM_CHECK(false, "set_parser_property failed");
      }
    }
    

    /**
     * / \brief Move from attribute to enclosing element
     * /
     * / \pre is_attribute
     */
    static void to_element(reader_t a_reader, bool & a_parser_okay) {
      const int Result = xmlTextReaderMoveToElement (a_reader);
      if (Result != 1) {
        a_parser_okay = false;
        COMPCHEM_CHECK(false, "to_element() failed"); }}


  private:
    /**
     * Helper to convert xmlchar to string on returns. Free memory allocated for a_str.
     */
    static void to_string(xmlChar * a_str, std::string & a_value)
    {
      std::tr1::shared_ptr< xmlChar > l_tmp (a_str, xmlFree);
      if (a_str != NULL)
      {
        a_value.assign (reinterpret_cast< const char* > (a_str));
      }
    }
    

    /**
     * Helper to convert xmlchar to string on returns. *Does not* free a_str.
     */
    static void as_string(const xmlChar * a_str, std::string & a_value)
    {
      if (a_str != NULL)
      {
        a_value.assign (reinterpret_cast< const char* > (a_str));
      }
    }


  public:
    /**
     * The textual value of node
     */
    static void value(const reader_t a_reader, std::string & a_value)
    {
      as_string (xmlTextReaderConstValue (a_reader), a_value);
    }

    /**
     * XML:Lang thingy
     */
    static void xml_lang(const reader_t a_reader, std::string & a_value)
    {
      as_string (xmlTextReaderConstXmlLang (a_reader), a_value);
    }

};
/**
 * Dump debugging information to error stream
 */
inline void xml2_reader_traits::debug(const xml2_reader_traits::reader_t a_reader)

{
  xmlMemDisplay (stderr);
  fprintf (stderr, "Bytes parsed %ld\n", xmlTextReaderByteConsumed (a_reader));
  fprintf (stderr, "Line %d\n", xmlTextReaderGetParserLineNumber (a_reader));
  fprintf (stderr, "Column %d\n", xmlTextReaderGetParserColumnNumber (a_reader));
  xmlChar * l_inner = xmlTextReaderReadInnerXml (a_reader);
  if (NULL != l_inner)
  {
    fprintf (stderr, "Inner: %s\n", (char*)l_inner);
    xmlFree (l_inner);
  }
  

}

typedef basic_xml_reader<xml2_reader_traits> xml2_reader;
typedef basic_xml_reader<xml2_reader_traits> xml_reader;

} // namespace xmlreader
#endif
