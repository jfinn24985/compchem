// ------------------------------------------------------------
// class: filter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/filter.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
// -
namespace data_scheme {

void filter::add_outcome_label(const std::string & a_set) 
{
  data_access::names::list_add (m_outcomes, a_set);
}

/**
 * Construct a new filter object with a_parent and a_label. Swallows
 * a_label.
 * 
 * \post a_label
 */
filter::filter(mechanism & a_parent, std::string & a_label)
: basic_datum ()
, m_label ()
, m_href ()
, m_outcomes ()
, m_target (a_parent)
, m_type ()
{
  m_label.swap (a_label);  
}

filter::~filter() throw () 
{}

const std::string & filter::filter_type() const throw () 
{
  return (m_type.empty () ? data_access::names::plugin_filter_name () : m_type);
}

/**
 * Add attributes to the XML element
 */
bool filter::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not label ().empty (), "Attempting to get_attribute of an invalid filter object.");
  bool Result (false);
  if (data_access::names::href_name () == a_name)
  {
    if (not m_href.empty ())
    {
      a_value.assign (m_href);
      Result = true;
    }
  }
  else if (data_access::names::label_name () == a_name)
  {
    a_value.assign (m_label);
    Result = true;
  }
  else if (data_access::names::outcome_name () == a_name)
  {
    if (not m_outcomes.empty ())
    {
      a_value.assign (m_outcomes);
      Result = true;
    }
  }
  else if (data_access::names::type_name () == a_name)
  {
    if (not m_type.empty ())
    {
      a_value.assign (m_type);
      Result = true;
    }
  }
  return Result;

}

/**
 * Return a pointer to the parent object
 */
const mechanism & filter::parent() const throw () 
{
  return m_target;
}
/**
 * The key for this class during serialisation
 */
std::string filter::serial_name() const 
{
  return data_access::names::filter_name ();
}

/**
 * Write an attribute. Can assume that a_name and a_value are valid as per the
 * definition() object.
 */
void filter::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::href_name () == a_name)
  {
    m_href.swap (a_value);
  }
  else if (data_access::names::outcome_name () == a_name)
  {
    m_outcomes.swap (a_value);
  }
  else if (data_access::names::type_name () == a_name)
  {
    if (data_access::names::plugin_filter_name () == a_name)
    {
      if (not m_type.empty ()) m_type.clear ();
    }
    else
    {
      m_type.swap (a_value);
    }
  }
  else if (data_access::names::label_name () == a_name) 
  {
    COMPCHEM_ALWAYS (a_value == label (), "filter object labels cannot be changed after construction");
  }
  else
  {
    const bool bad_attribute_name (false);
    COMPCHEM_ALWAYS (bad_attribute_name, "filter objects do not have an attribute named " + a_name);
  }
  

}

void filter::visit(visitor & a_visitor) const 
{
  a_visitor.visit_filter (*this);
}


} // namespace data_scheme
