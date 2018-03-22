#ifndef PURPLETREE_XML_READER_HPP
#define PURPLETREE_XML_READER_HPP

// ------------------------------------------------------------
// class: xml_reader
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace xmlreader {

/**
 * / \brief Interface to an XML reader type
 * /
 * / The implementation can instantiate an object of this type by
 * / calling the make_xml_reader function.
 */
template<typename t_reader_traits>
class basic_xml_reader {
  public:
    typedef  typename t_reader_traits::reader_t reader_t;

    enum {
      START_ELEMENT= t_reader_traits::START_ELEMENT,
      ATTRIBUTE= t_reader_traits::ATTRIBUTE,
      TEXT= t_reader_traits::TEXT,
      CDATA= t_reader_traits::CDATA,
      ENTITY= t_reader_traits::ENTITY,
      PI= t_reader_traits::PI,
      ENTITY_DECL= t_reader_traits::ENTITY_DECL,
      COMMENT= t_reader_traits::COMMENT,
      DOCUMENT= t_reader_traits::DOCUMENT,
      DOCTYPE= t_reader_traits::DOCTYPE,
      NOTATION= t_reader_traits::NOTATION,
      DOCFRAG= t_reader_traits::DOCFRAG,
      END_ELEMENT= t_reader_traits::END_ELEMENT

    };

    /**
     * Constants that can be used to control optional aspects of the 
     * parser/reader.
     * 
     * If a property is unsupported the following will be true:
     * <prop> == UNSUPPORTED
     */
    enum parser_property
    {
      UNSUPPORTED= t_reader_traits::UNSUPPORTED,/**
       * Value to compare with another property value to
       * see if the property is unsupported.
       */

      LOAD_DTD= t_reader_traits::LOAD_DTD,/**
       * Load the DTD while parsing the document.
       */

      DEFAULT_ATTRIBUTES= t_reader_traits::DEFAULT_ATTRIBUTES,/**
       * Will automatically set default attributes (implies
       * loading the DTD).
       */

      VALIDATE= t_reader_traits::VALIDATE,/**
       * Validate the document against the DTD (implies
       * loading the DTD.)
       */

      EXPAND_ENTITIES= t_reader_traits::EXPAND_ENTITIES/**
       * Expand any entities into their final form (implies
       * loading the DTD)
       */


    };


  private:
    bool m_parser_okay;

    reader_t m_reader;


  public:
    /**
     * Get value of attribute of give name from element
     * 
     * \pre has_attribute (a_key)
     */
    std::string attribute(const std::string & a_key) const
    {
      COMPCHEM_REQUIRE(this->has_attribute (a_key), "No attribute with given name: " + a_key);
      std::string Result;
      t_reader_traits::attribute (this->m_reader, a_key, Result);
      return Result;
    }

    /**
     * How many attributes are present?
     * 
     * \pre !is_attribute
     */
    int attribute_count() const {
      COMPCHEM_REQUIRE(not this->is_attribute (), "Cannot get attribute from an attribute");
      return t_reader_traits::attribute_count (this->m_reader); }

    /**
     * Base URI of document
     */
    std::string base_uri() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::base_uri (this->m_reader, Result);
      return Result;
    }

    /**
     * Provide some debugging information.
     */
    void debug() const
    {
      t_reader_traits::debug (this->m_reader);
    }

    /**
     * Depth of node in tree
     */
    int depth() const {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
       return t_reader_traits::depth (this->m_reader); }

    /**
     * Does element have no content?
     */
    bool empty() const {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      return t_reader_traits::empty (this->m_reader); }

    /**
     * Move to first attribute of this element
     * 
     * \pre has_attributes()
     */
    void first_attribute() {
      COMPCHEM_REQUIRE(this->has_attributes ()
    		, "Cannot use first_attribute() on element with no attributes");
      t_reader_traits::first_attribute (this->m_reader, this->m_parser_okay); }

    /**
     * Does element have attributes?
     * 
     * \pre !is_attribute
     */
    bool has_attributes() const {
      COMPCHEM_REQUIRE(not this->is_attribute (), "Cannot get attribute from an attribute");
      return t_reader_traits::has_attributes (this->m_reader); }

    /**
     * Does element have attribute of give name?
     */
    bool has_attribute(const std::string & a_key) const {
      COMPCHEM_REQUIRE(not this->is_attribute (), "Cannot get attribute from an attribute");
      return t_reader_traits::has_attribute (this->m_reader, a_key); }

    /**
     * Does element have a value?
     */
    bool has_value() const {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      return t_reader_traits::has_value (this->m_reader); }

    /**
     * Is the current position an attribute node?
     */
    bool is_attribute() const {
      return (basic_xml_reader::ATTRIBUTE == this->node_type ()); }

    /**
     * Does the attribute have its default value
     * 
     * \pre is_attribute()
     */
    bool is_default() const {
      COMPCHEM_REQUIRE(this->is_attribute (), "Must be an attribute");
      return t_reader_traits::is_default (this->m_reader); }

    /**
     * Has the parser found the document valid so far
     * 
     * This should always be true until next() returns false. Then
     * this indicates whether the next() failed because the end of
     * document was reached or if the document was some how invalid.
     * 
     * This is implicit \pre condition to all methods
     */
    bool is_doc_valid() const { return this->m_parser_okay; }

    /**
     * local name of node
     */
    std::string local_name() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::local_name (this->m_reader, Result);
      return Result;
    }

    /**
     * name, possibly qualified, of node
     */
    std::string name() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::name (this->m_reader, Result);
      return Result;
    }

    /**
     * URI of namespace of node
     */
    std::string namespace_uri() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::namespace_uri (this->m_reader, Result);
      return Result;
    }

    /**
     * Move to next attribute of an element
     * 
     * \pre is_attribute
     * 
     * \return false if no next attribute
     */
    bool next_attribute() {
      COMPCHEM_REQUIRE(this->is_attribute ()
    		, "Cannot use next_attribute() on non-attribute");
      return t_reader_traits::next_attribute (this->m_reader, this->m_parser_okay); }

    /**
     * Type of node
     */
    int node_type() const {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      return t_reader_traits::node_type (this->m_reader); }

    /**
     * Attempt to get parser property corresponding to a_option.  
     * Returns the current state of the property if option is 
     * supported by the current implementation. Unsupported 
     * properties always return false. 
     * 
     * Throws an error if getting a valid option failed in CHECK mode
     */
    bool parser_property(int a_option) const
    {
      return t_reader_traits::get_parser_property(m_reader, a_option, m_parser_okay);
    }
    

    /**
     * Attempt to set parser attribute corresponding to a_option to
     * on or off.  Returns true if option is supported by the
     * current implementation and option was set, false if unsupported
     * or an error occured setting the property.
     * 
     * Throws an error if setting a valid option failed in CHECK mode
     */
    bool parser_property(int a_option, bool a_on)
    {
      bool Result (false);
      if (a_option != UNSUPPORTED)
      {
        t_reader_traits::set_parser_property(m_reader, a_option, a_on, m_parser_okay);
        Result = m_parser_okay;
      }
      return Result;
    }

    /**
     * Prefix part of name
     */
    std::string prefix() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::prefix (this->m_reader, Result);
      return Result;
    }
    /**
     * Move the reader to the next node. 
     * 
     * \return false at end of document.
     * 
     * \pre !is_attribute
     */
    bool read() {
      COMPCHEM_REQUIRE(not this->is_attribute (), "Cannot use read() on attribute");
      return t_reader_traits::read (this->m_reader, this->m_parser_okay); }

    /**
     * Move from attribute to enclosing element
     * 
     * \pre is_attribute
     */
    void to_element() {
      COMPCHEM_REQUIRE(this->is_attribute (), "Cannot use to_element() on non-attribute");
      t_reader_traits::to_element (this->m_reader, this->m_parser_okay); }

    /**
     * The textual value of node
     */
    std::string value() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::value (this->m_reader, Result);
      return Result;
    }

    /**
     * XML:Lang thingy
     */
    std::string xml_lang() const
    {
      COMPCHEM_REQUIRE(this->is_doc_valid (), "Cannot continue with invalid document");
      std::string Result;
      t_reader_traits::xml_lang (this->m_reader, Result);
    }

    /**
     * create an xml_reader from a file
     */
    basic_xml_reader(const std::string & a_filename)
    : m_parser_okay(false)
    , m_reader(t_reader_traits::new_reader (a_filename)) {
      this->m_parser_okay = (NULL != this->m_reader); }

    /**
     * create an xml_reader from a stream
     */
    basic_xml_reader(std::istream & a_stream)
    : m_parser_okay (false)
    , m_reader (t_reader_traits::new_reader (a_stream)) {
      this->m_parser_okay = (NULL != this->m_reader); }

    /**
     * create an xml_reader from a buffer.  This uses the buffer directly
     * and it is the responsibility of the creator to ensure that this
     * buffer exists for the duration of the reader.
     */
    basic_xml_reader(const char * a_buffer, int a_sz)
    : m_parser_okay(false)
    , m_reader(t_reader_traits::new_reader (a_buffer, a_sz)) {
      this->m_parser_okay = (NULL != this->m_reader); }

    ~basic_xml_reader() {
      if (this->m_reader != NULL){
        t_reader_traits::delete_reader (this->m_reader); }}


  private:
    /**
     * no default
     */
    explicit basic_xml_reader();

    /**
     * no copy
     */
    basic_xml_reader(const basic_xml_reader<t_reader_traits> & );

    /**
     * no copy
     */
    const basic_xml_reader<t_reader_traits> & operator =(const basic_xml_reader<t_reader_traits> & );

};

} // namespace xmlreader
#endif
