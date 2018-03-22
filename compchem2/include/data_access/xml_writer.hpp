#ifndef COMPCHEM_XML_WRITER_HPP
#define COMPCHEM_XML_WRITER_HPP

// ------------------------------------------------------------
// class: xml_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <stack>
#include <string>
#include <tr1/memory>

namespace data_access {

/**
 * This is a helper class for writing well-formed XML. It is a counter-
 * point to the xml_reader class.
 * 
 * It writes the document as it goes which simplifies the class although 
 * gives the following limitations.
 * * You must set all attributes before you use an object as a parent
 * * Using an object as a parent automatically closes the start tag 
 *   and sets have_children
 * 
 * Usage
 *   xml_writer l_writer (a_os, "nodename");
 *   l_writer.attribute (...);
 *   if (not object.empty ()) {
 *     l_writer.set_children ();
 *     write_children (a_os);
 *   }
 */
class xml_writer
{
  private:
    /**
     * The stack of element labels.
     */
    std::stack<std::string> m_element_stack;

    /**
     * Do we have children?
     */
    bool m_is_empty;

    /**
     * The stream to write to
     */
    std::ostream & m_os;


  public:
    void add_comment(const std::string& a_str);

    void add_pi(const std::string& a_name, const std::string& a_str);

    void add_text(const std::string& a_str);

    /**
     * Do we have children yet?
     */
    bool can_set_attribute() const
    {
      return m_is_empty;
    }

    void end_element();


  private:
    /**
     * no assign
     */
    xml_writer & operator=(const xml_writer & source);


  public:
    /**
     * Write an attribute to the stream. All attributes must be given before
     * this object is used as a parent.
     * 
     * \pre not have_children
     * \pre not a_label.empty and not a_value.empty
     */
    void set_attribute(const std::string& a_label, const std::string& a_value)
    {
      COMPCHEM_REQUIRE (m_is_empty, "Cannot set an attribute after adding children");
      COMPCHEM_REQUIRE (not a_label.empty (), "Cannot set an attribute with an empty label");
      COMPCHEM_REQUIRE (not a_value.empty (), "Can set attribute [" + a_label + "] with an empty value");
      m_os << " " << a_label << "=\"" << a_value << "\"";
    }

    /**
     * Let this object know it will have children.
     * 
     * \post not can_set_attribute
     */
    void set_children()
    {
      if (m_is_empty and not m_element_stack.empty ())
      {
        m_is_empty = false;
        m_os << ">";
      }
    }

    /**
     * Start a new element.
     */
    void start_element(const std::string& a_label);

    /**
     * Construct an xml_writer to write a document fragment.
     * 
     * \pre not a_label.empty
     */
    inline xml_writer(std::ostream & a_os);

    /**
     * Construct an xml_writer for a document element. Creates the <?xml?> tag, if a_pub_id 
     * and a_sys_id are not not both empty then a DOCTYPE element is also created with
     * a_label as the document tag.
     * 
     * \pre not a_label.empty
     */
    xml_writer(std::ostream & a_os, const std::string& a_label, const std::string& a_pub_id, const std::string& a_sys_id);

    inline ~xml_writer();


  private:
    /**
     * no copy
     */
    xml_writer(const xml_writer & source);


};
/**
 * Construct an xml_writer to write a document fragment.
 * 
 * \pre not a_label.empty
 */
inline xml_writer::xml_writer(std::ostream & a_os) 
: m_element_stack ()
, m_is_empty (true)
, m_os (a_os)
{}


inline xml_writer::~xml_writer() 
{
  set_children ();
  while (not m_element_stack.empty ()) end_element ();

}

/**
 * This is a helper class for writing well-formed XML. It 
 * collaborates with the xml_writer class to automate the
 * calling of start/end_element methods.
 * 
 * It has the following features.
 * * Automatic closure of element when proxy goes out of scope.
 * * Automatic closure of element(s) when parent (antecedents) 
 * is given a new child. [can be checked with is_closed]
 */
class xml_writer_proxy
{
  private:
    std::tr1::shared_ptr<xml_writer> m_rep;

    /**
     * Indicate that this element has been completed.
     */
    bool m_done;

    /**
     * Ptr to last child (nul if no prior child)
     */
    xml_writer_proxy * m_last_child;

    /**
     * Our parent node
     */
    xml_writer_proxy * m_parent;


  public:
    /**
     * Insert a comment. Returns a reference to itself.
     * 
     * \pre not a_str.empty
     */
    xml_writer_proxy & add_comment(const std::string& a_str)
    {
      COMPCHEM_REQUIRE (not is_closed (), "element is already closed.");
      m_rep->add_comment (a_str);
      return *this;
    }

    /**
     * Insert a processing instruction. Returns a reference to itself.
     * 
     * \pre not a_label.empty and not a_value.empty
     */
    xml_writer_proxy & add_pi(const std::string& a_label, const std::string& a_value)
    {
      COMPCHEM_REQUIRE (not is_closed (), "element is already closed.");
      m_rep->add_pi (a_label, a_value);
      return *this;
    }

    /**
     * Insert text. Returns a reference to itself.
     * 
     * \pre not a_str.empty
     */
    xml_writer_proxy & add_text(const std::string& a_str)
    {
      COMPCHEM_REQUIRE (not is_closed (), "element is already closed.");
      m_rep->add_text (a_str);
      return *this;
    }

    /**
     * Write an attribute to the stream. All attributes must be given before
     * this object is used as a parent. Returns a reference to itself.
     * 
     * \pre can_set_attribute
     * \pre not a_label.empty and not a_value.empty
     */
    xml_writer_proxy & attribute(const std::string& a_label, const std::string& a_value)
    {
      COMPCHEM_REQUIRE (not is_closed (), "element is already closed.");
      m_rep->set_attribute (a_label, a_value);
      return *this;
    }

    /**
     * Do we have children?
     */
    bool can_set_attribute() const
    {
      return m_rep->can_set_attribute () and not is_closed ();
    }


  private:
    void end_element();


  public:
    /**
     * Is this element already completed? 
     */
    bool is_closed() const
    {
      return m_done;
    }

    /**
     * Get the parent of this proxy (may be nul)
     */
    xml_writer_proxy * parent()
    {
      return m_parent;
    }


  private:
    /**
     * no assign
     */
    xml_writer_proxy & operator=(const xml_writer_proxy & source);

    inline void set_child(xml_writer_proxy * a_child);


  public:
    /**
     * Construct an xml_writer, letting a_parent know it has children
     * 
     * \pre not a_label.empty and not a_parent.is_closed
     * \post a_parent.have_children
     */
    inline xml_writer_proxy(xml_writer_proxy & a_parent, const std::string& a_label);

    /**
     * Construct a proxy for a document fragment.
     * 
     * \pre not a_label.empty
     */
    inline xml_writer_proxy(std::ostream & a_os, const std::string& a_label);

    /**
     * Construct a proxy from an already created xml_writer.
     * 
     * \pre not a_label.empty
     */
    inline xml_writer_proxy(std::tr1::shared_ptr< xml_writer > & a_rep, const std::string& a_label);

    /**
     * Construct a proxy for a document element. Creates the <?xml?> tag, if a_pub_id and a_sys_id 
     * are not not both empty then a DOCTYPE element is also created with a_label as the document
     * element.
     * 
     * \pre not a_label.empty
     */
    inline xml_writer_proxy(std::ostream & a_os, const std::string& a_label, const std::string& a_pub_id, const std::string& a_sys_id);

    ~xml_writer_proxy()
    {
      end_element ();
    }

  private:
    /**
     * no copy
     */
    xml_writer_proxy(const xml_writer_proxy & source);


};
inline void xml_writer_proxy::set_child(xml_writer_proxy * a_child) 
{
  COMPCHEM_REQUIRE (not is_closed (), "element is already closed.");
  if (NULL != m_last_child)
  {
    m_last_child->end_element ();
  }
  m_last_child = a_child;

}

/**
 * Construct an xml_writer, letting a_parent know it has children
 * 
 * \pre not a_label.empty and not a_parent.is_closed
 * \post a_parent.have_children
 */
inline xml_writer_proxy::xml_writer_proxy(xml_writer_proxy & a_parent, const std::string & a_label) 
: m_rep (a_parent.m_rep)
, m_done (false)
, m_last_child (NULL)
, m_parent (&a_parent)
{
  a_parent.set_child (this);
  m_rep->start_element (a_label);
}


/**
 * Construct a proxy for a document fragment.
 * 
 * \pre not a_label.empty
 */
inline xml_writer_proxy::xml_writer_proxy(std::ostream & a_os, const std::string& a_label) 
: m_rep (new xml_writer (a_os))
, m_done (false)
, m_last_child (NULL)
, m_parent (NULL)
{
  m_rep->start_element (a_label);
}


/**
 * Construct a proxy from an already created xml_writer.
 * 
 * \pre not a_label.empty
 */
inline xml_writer_proxy::xml_writer_proxy(std::tr1::shared_ptr< xml_writer > & a_rep, const std::string& a_label) 
: m_rep (a_rep)
, m_done (false)
, m_last_child (NULL)
, m_parent (NULL)
{
  m_rep->start_element (a_label);
}


/**
 * Construct a proxy for a document element. Creates the <?xml?> tag, if a_pub_id and a_sys_id 
 * are not not both empty then a DOCTYPE element is also created with a_label as the document
 * element.
 * 
 * \pre not a_label.empty
 */
inline xml_writer_proxy::xml_writer_proxy(std::ostream & a_os, const std::string& a_label, const std::string& a_pub_id, const std::string& a_sys_id) 
: m_rep (new xml_writer (a_os, a_label, a_pub_id, a_sys_id))
, m_done (false)
, m_last_child (NULL)
, m_parent (NULL)
{
  m_rep->start_element (a_label);
}



} // namespace data_access
#endif
