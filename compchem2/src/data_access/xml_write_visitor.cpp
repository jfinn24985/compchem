// ------------------------------------------------------------
// class: xml_write_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/xml_write_visitor.hpp"
#include "data_access/meta_datum.hpp"
#include "data_access/xml_writer.hpp"
#include "data_scheme/basic_datum.hpp"
#include "data_scheme/text.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/project.hpp"

// Manual includes
#include <boost/bind.hpp>
//-
namespace data_access {

/**
 * Write well-formed XML for this node (and its children) into a_os.
 * 
 * Note that this should only be overriden for types like text, comment and PI
 * 
 * \final
 */
void xml_write_visitor::visit_basic_datum(const data_scheme::basic_datum & a_datum) 
{
  // Start element
  const meta_datum & l_defn (meta_datum::by_name (a_datum.serial_name ()));
  if (m_writer.empty ())
  {
    m_writer.push_back (new xml_writer_proxy (m_writer_rep, l_defn.name));
  }
  else
  {
    m_writer.push_back (new xml_writer_proxy (m_writer.back (), l_defn.name));
  }
  // Process attributes.
  if (l_defn.has_attributes ())
  {
    for (meta_datum::const_iterator l_i (l_defn.begin_attr ());
         l_i != l_defn.end_attr ();
         ++l_i)
    {
      std::string l_value;
      if (a_datum.get_attribute (*l_i, l_value))
      {
        m_writer.back ().attribute (*l_i, l_value);
      }
    }
  }
  // Now process children
  if (not a_datum.empty ())
  {
    std::for_each (a_datum.begin (), a_datum.end (), boost::bind (&data_scheme::basic_datum::visit, _1, boost::ref(*this)));
  }
  m_writer.pop_back ();

}

/**
 * Write a comment.
 */
void xml_write_visitor::visit_comment(const data_scheme::comment & a_comment) 
{
  COMPCHEM_REQUIRE (not m_writer.empty (), "first element can not be a comment.");
  m_writer.back ().add_comment (a_comment.the_content);
}

/**
 * Write well-formed XML for processing instruction.
 */
void xml_write_visitor::visit_pi(const data_scheme::processing_instruction & a_pi) 
{
  COMPCHEM_REQUIRE (not m_writer.empty (), "first element can not be a P.I.");
  m_writer.back ().add_pi (a_pi.name, a_pi.the_content);
}

/**
 * Write well-formed XML for this node (and its children) into a_os.
 * 
 * No default implementation.
 */
void xml_write_visitor::visit_text(const data_scheme::text & a_text) 
{
  COMPCHEM_REQUIRE (not m_writer.empty (), "first element can not be a comment.");
  m_writer.back ().add_text (a_text.the_content);
}

void xml_write_visitor::visit_memo(const data_scheme::memo & a_datum) 
{
  if (a_datum.is_src ())
  {
    COMPCHEM_REQUIRE (not m_writer.empty (), "first element can not be a memo-src.");
    // Start element
    xml_writer_proxy l_writer (m_writer.back (), a_datum.serial_src_name ());
    // Process attributes.
    l_writer.attribute (names::compound_name (), a_datum.compound_label ());
    l_writer.attribute (names::formula_name (), a_datum.formula_label ());
    l_writer.attribute (names::label_name (), a_datum.label ());
    l_writer.attribute (names::href_name (), a_datum.uri ());
    // Get object to write itself.
    if (a_datum.is_open ()) a_datum.write ();
  }
  else
  {
    // For non-src use std writer.
    visit_basic_datum (a_datum);
  }

}

void xml_write_visitor::visit_project(const data_scheme::project & a_datum) 
{
  if (a_datum.is_src ())
  {
    COMPCHEM_REQUIRE (not m_writer.empty (), "first element can not be a proj-src.");
    // Start element
    xml_writer_proxy l_writer (m_writer.back (), a_datum.serial_src_name ());
    // Process attributes.
    l_writer.attribute (names::label_name (), a_datum.label ());
    l_writer.attribute (names::href_name (), a_datum.uri ());
    // Get object to write itself.
    if (a_datum.is_open ()) a_datum.write ();
  }
  else
  {
    // Not non-src use std method
    visit_basic_datum (a_datum);
  }

}

/**
 * Construct and initialise the xml_write_visitor.
 * 
 * After this method the xml header (<?xml ..> + <!DOCTYPE..>)
 * is written to the stream but not the document element. If 
 * you want to add extra header information you can write these
 * directly to the stream before initialising the write process.
 */
 xml_write_visitor::xml_write_visitor(std::ostream & a_os, const std::string & a_label, const std::string& a_pub_id, const std::string& a_sys_id) 
: m_writer ()
, m_writer_rep (new xml_writer (a_os, a_label, a_pub_id, a_sys_id))
{}

xml_write_visitor::~xml_write_visitor() throw () 
{}


} // namespace data_access
