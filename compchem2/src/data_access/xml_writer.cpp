// ------------------------------------------------------------
// class: xml_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/xml_writer.hpp"

namespace data_access {

void xml_writer::add_comment(const std::string& a_str) 
{
  COMPCHEM_REQUIRE (not a_str.empty (), "can not create an empty comment.");
  set_children ();
  m_os << "<!-- " << a_str << " -->";

}

void xml_writer::add_pi(const std::string& a_name, const std::string& a_str) 
{
  COMPCHEM_REQUIRE (not a_name.empty () and not a_str.empty (), "can not set P.I. with empty content.");
  set_children ();
  m_os << "<?" << a_name << " " << a_str << "?>";

}

void xml_writer::add_text(const std::string& a_str) 
{
  COMPCHEM_REQUIRE (not a_str.empty (), "can not add empty text.");
  set_children ();
  m_os << a_str;

}

void xml_writer::end_element() 
{
  if (m_is_empty)
  {
    m_os << "/>";
  }
  else
  {
    m_os << "</" << m_element_stack.top () << ">";
  }
  // At end, indicate popped element is non-empty.
  if (m_is_empty) m_is_empty = false;
  m_element_stack.pop ();

}

/**
 * Start a new element.
 */
void xml_writer::start_element(const std::string& a_label) 
{
  set_children ();
  m_is_empty = true;
  m_element_stack.push (a_label);
  m_os << "<" << a_label;

}

/**
 * Construct an xml_writer for a document element. Creates the <?xml?> tag, if a_pub_id 
 * and a_sys_id are not not both empty then a DOCTYPE element is also created with
 * a_label as the document tag.
 * 
 * \pre not a_label.empty
 */
xml_writer::xml_writer(std::ostream & a_os, const std::string& a_label, const std::string& a_pub_id, const std::string& a_sys_id) 
: m_element_stack ()
, m_is_empty (true)
, m_os (a_os)
{
  COMPCHEM_REQUIRE (not a_label.empty (), "can not start an XML document without a document element label.");
  COMPCHEM_REQUIRE (not a_sys_id.empty (), "can not start an XML document without a system ID.");
  a_os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
  a_os << "<!DOCTYPE " << a_label;
  if (not a_pub_id.empty ())
  {
    a_os << " PUBLIC \"" << a_pub_id << "\"";
  }
  a_os << " \"" << a_sys_id << "\">\n";
}


void xml_writer_proxy::end_element() 
{
  if (not m_done)
  {
    if (NULL != m_last_child)
    {
      m_last_child->end_element ();
      COMPCHEM_CHECK (NULL == m_last_child, "child should remove itself.");
    }
    m_rep->end_element ();
    m_done = true;
    if (NULL != m_parent and m_parent->m_last_child == this)
    {
      // detach from parent
       m_parent->m_last_child = NULL;
    }
  }

}


} // namespace data_access
