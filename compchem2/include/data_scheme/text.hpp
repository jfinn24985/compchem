#ifndef COMPCHEM_DATA_SCHEME_TEXT_HPP
#define COMPCHEM_DATA_SCHEME_TEXT_HPP

// ------------------------------------------------------------
// class: text
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <string>

namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This represents character data elements it can be interspersed with other elements.
 * 
 * This is an abstract base class of XML textual elements
 */
class basic_text : public basic_datum
{
  public:
    /**
     * The text string.
     */
    std::string the_content;


  private:
    /**
     * Our parent.
     */
    basic_datum & m_parent;


  public:
    /**
     * add_comment is an error for basic_text. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    comment & add_comment_swallow(std::string & a_value);

    /**
     * add_comment is an error for basic_text. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    processing_instruction & add_pi_swallow(const std::string & a_name, std::string & a_value);

    /**
     * add_comment is an error for basic_text. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    text & add_text_swallow(std::string & a_value);

    /**
     * Create a text element (copy version, copies text)
     */
    basic_text(basic_datum & a_parent, std::string & a_content)
    : basic_datum ()
    , the_content ()
    , m_parent (a_parent)
    {
      the_content.swap (a_content);
    }

    virtual ~basic_text() throw ()
    {}

  private:
    /**
     * no copy
     */
    basic_text(const basic_text & source);

    /**
     * no assign
     */
    basic_text & operator=(const basic_text & source);


  public:
    /**
     * Return a pointer to the parent object. Text objects always have parents.
     */
    const basic_datum & parent() const throw ()
    {
      return m_parent;
    }

    /**
     * Throw an error as text objects have no attributes.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);


};
/**
 * This represents character data elements it can be interspersed 
 * with other elements.
 */
class text : public basic_text
{
  private:
    /**
     * no assign
     */
    text & operator=(const text & source);


  public:
    /**
     * The XML element definition for this datum.
     */
    std::string serial_name() const;

    /**
     * Create a text element.
     * 
     * Swallows content!
     * \post a_content.empty
     */
    text(basic_datum & a_parent, std::string & a_content)
    : basic_text (a_parent, a_content)
    {}

    ~text() throw ()
    {}

  private:
    /**
     * no copy
     */
    text(const text & source);


  public:
    void visit(visitor & a_visitor) const;


};
/**
 * This represents character data elements it can be interspersed 
 * with other elements.
 */
class comment : public basic_text
{
  public:
    /**
     * Create a text element.
     * 
     * Swallows content!
     * \post a_content.empty
     */
    comment(basic_datum & a_parent, std::string & a_content)
    : basic_text (a_parent, a_content)
    {}


  private:
    /**
     * no copy
     */
    comment(const comment & source);


  public:
    ~comment() throw ()
    {}

  private:
    /**
     * no assign
     */
    comment & operator=(const comment & source);


  public:
    /**
     * The XML element definition for this datum.
     */
    std::string serial_name() const;

    void visit(visitor & a_visitor) const;


};
/**
 * This represents character data elements it can be interspersed 
 * with other elements.
 */
class processing_instruction : public basic_text
{
  public:
    /**
     * The name of the PI
     */
    const std::string name;

    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Create a processing_instruction object with a_parent, a_name and a_content.
     * 
     * Swallows content!
     * \post a_content.empty
     */
    processing_instruction(basic_datum & a_parent, const std::string & a_name, std::string & a_content)
    : basic_text (a_parent, a_content)
    , name (a_name)
    {}


  private:
    /**
     * no copy
     */
    processing_instruction(const processing_instruction & source);


  public:
    ~processing_instruction() throw ()
    {}
    void visit(visitor & a_visitor) const;


};

} // namespace data_scheme
#endif
