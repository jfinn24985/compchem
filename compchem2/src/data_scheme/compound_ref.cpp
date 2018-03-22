// ------------------------------------------------------------
// class: compound_ref
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/compound_ref.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/text.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
// -
namespace data_scheme {

/**
 * Throws an error as no children are allowed. 
 */
comment & compound_ref::add_comment_swallow(std::string & a_value) 
{
  const bool empty_element (false);
  COMPCHEM_ALWAYS(empty_element, "Can not add a comment to an EMPTY element");
}

void compound_ref::add_formula_label(const std::string & a_set) 
{
  data_access::names::list_add (m_formulae, a_set);
}

/**
 * Throws an error as no children are allowed. 
 */
processing_instruction & compound_ref::add_pi_swallow(const std::string & a_name, std::string & a_value) 
{
  const bool empty_element (false);
  COMPCHEM_ALWAYS(empty_element, "Can not add a processing-instruction to an EMPTY element");
}

/**
 * Throws an error as no children are allowed. 
 */
text & compound_ref::add_text_swallow(std::string & a_value) 
{
  const bool empty_element (false);
  COMPCHEM_ALWAYS(empty_element, "Can not add text to an EMPTY element");
}

/**
 * Construct a new compound-ref object, swallow a_label
 * 
 * \post a_label.empty
 */
compound_ref::compound_ref(project & a_parent, std::string & a_label) 
: basic_datum ()
, m_formulae ()
, m_importer (a_parent)
, m_label ()
{
  m_label.swap (a_label);
}

compound_ref::~compound_ref() throw ()
{}

/**
 * Add attributes to element writer.
 * 
 * No default implementation.
 */
bool compound_ref::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_label.empty (), "Attempting to write an invalid compound-ref object.");
  bool Result (false);
  // Process attributes.
  if (a_name == data_access::names::label_name ())
  {
    a_value.assign (m_label);
    Result = true;
  }
  else if (a_name == data_access::names::formula_name ())
  {
    if (not m_formulae.empty ())
    {
      a_value.assign (m_formulae);
      Result = true;
    }
  }
  return Result;

}

/**
 * The XML element definition for this datum.
 */
std::string compound_ref::serial_name() const 
{
  return data_access::names::compound_ref_name ();
}

/**
 * Write an attribute.  Throw an error if arguments are incorrect or if no 
 * attributes are allowed.
 */
void compound_ref::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::formula_name () == a_name) 
  {
    m_formulae.swap (a_value);
  }
  else if (data_access::names::label_name () == a_name) 
  {
    COMPCHEM_ALWAYS (a_value == m_label, serial_name () + " object labels cannot be changed after construction");
  }
  else
  {
    const bool bad_attribute_name (false);
    COMPCHEM_ALWAYS (bad_attribute_name, serial_name () + " objects do not have an attribute named " + a_name);
  }
  

}

void compound_ref::visit(visitor & a_visitor) const 
{
  a_visitor.visit_compound_ref (*this);
}


} // namespace data_scheme
