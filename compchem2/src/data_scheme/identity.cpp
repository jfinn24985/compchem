// ------------------------------------------------------------
// class: identity
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/identity.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
// -
namespace data_scheme {

/**
 * Add attributes to the XML element
 */
bool identity::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_type.empty (), "Attempting to get_attribute of an invalid formula object.");
  bool Result (false);
  if (data_access::names::type_name () == a_name)
  {
    a_value.assign (m_type);
    Result = true;
  }
  return Result;

}

identity::~identity() throw () 
{}

/**
 * The key for this class during serialisation.
 */
std::string identity::serial_name() const 
{
  return data_access::names::identity_name ();
}

/**
 * Throw an error as program has no attributes.
 */
void identity::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::type_name () == a_name) 
  {
    COMPCHEM_ALWAYS (a_value == m_type, serial_name () + " object types cannot be changed after construction");
  }
  else
  {
    const bool bad_attribute_name (false);
    COMPCHEM_ALWAYS (bad_attribute_name, serial_name () + " objects do not have an attribute named " + a_name);
  }
  

}

void identity::visit(visitor & a_visitor) const 
{
  a_visitor.visit_identity (*this);
}


} // namespace data_scheme
