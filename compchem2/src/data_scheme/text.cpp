// ------------------------------------------------------------
// class: text
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/text.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
// -
namespace data_scheme {

/**
 * add_comment is an error for basic_text. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
comment & basic_text::add_comment_swallow(std::string & a_value) 
{
  COMPCHEM_ALWAYS(false, "Can not add comments to a basic_text objects");
}

/**
 * add_comment is an error for basic_text. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
processing_instruction & basic_text::add_pi_swallow(const std::string & a_name, std::string & a_value) 
{
  COMPCHEM_ALWAYS(false, "Can not add processing instructions to a basic_text objects");
}

/**
 * add_comment is an error for basic_text. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
text & basic_text::add_text_swallow(std::string & a_value) 
{
  COMPCHEM_ALWAYS(false, "Can not add text object to a basic_text objects");
}

/**
 * Throw an error as text objects have no attributes.
 */
void basic_text::set_attribute(const std::string & a_name, std::string a_value) 
{
  COMPCHEM_ALWAYS(false, "Text classes have no settable attributes");
}

/**
 * The XML element definition for this datum.
 */
std::string text::serial_name() const 
{
  return data_access::names::text_type ();
}

void text::visit(visitor & a_visitor) const 
{
  a_visitor.visit_text (*this);
}

/**
 * The XML element definition for this datum.
 */
std::string comment::serial_name() const 
{
  return data_access::names::comment_type ();
}

void comment::visit(visitor & a_visitor) const 
{
  a_visitor.visit_comment (*this);
}

/**
 * The key for this class during serialisation.
 */
std::string processing_instruction::serial_name() const 
{
  return data_access::names::processing_instruction_type ();
}

void processing_instruction::visit(visitor & a_visitor) const 
{
  a_visitor.visit_pi (*this);
}


} // namespace data_scheme
