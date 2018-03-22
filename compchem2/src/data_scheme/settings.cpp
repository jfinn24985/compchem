// ------------------------------------------------------------
// class: settings
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/settings.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
// -
namespace data_scheme {

/**
 * add_child tests if this object can accept the tagname using the
 * meta_datum object. Only then is the virt_add_child method called. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
basic_datum & settings::add_child(const std::string & a_name, std::string & a_label) 
{
  basic_datum * Result (NULL);
  if (data_access::names::datum_name () == a_name)
  {
    COMPCHEM_ALWAYS (not has_data (), "Can not add more than one datum object here");
    m_datum.reset (new datum (*this, a_label));
    Result = m_datum.get ();
  }
  else if (data_access::names::info_name () == a_name)
  {
    COMPCHEM_ALWAYS (not has_information (), "Can not add more than one info object here");
    m_info.reset (new info (*this, a_label));
    Result = m_info.get ();
  }
  else
  {
    const bool bad_child_name (false);
    COMPCHEM_ALWAYS (bad_child_name, serial_name() + " class has no child with name " + a_name);
  }
  append_child (*Result);
  return *Result;

}

/**
 * Add an info object.
 * 
 * \pre not has_data
 * \return /= NULL
 */
datum & settings::add_datum(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_data (), "Attempt to add a second datum object.");
  COMPCHEM_REQUIRE (not has_data (), "Can not add more than one datum object here");
  m_datum.reset (new datum (*this, a_label));
  // Always last
  append_child (*(m_datum.get ()));
  return *(m_datum.get ());

}

/**
 * Add an info object.
 * 
 * \pre not has_information
 * \return /= NULL
 */
info & settings::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_information (), "Attempt to add a second info object.");
  COMPCHEM_REQUIRE (not has_information (), "Can not add more than one info object here");
  m_info.reset (new info (*this, a_label));
  // Always last
  append_child (*(m_info.get ()));
  return *(m_info.get ());

}

/**
 * Add a filter label to list of what extracted data into database.
 */
void settings::add_filter_label(const std::string & a_filter) 
{
  data_access::names::list_add (m_filters, a_filter);
}

/**
 * Add a filter label to list of what extracted data into database.
 */
void settings::add_outcome_label(const std::string & a_filter) 
{
  data_access::names::list_add (m_outcomes, a_filter);
}

/**
 * Add attributes to the XML element
 */
bool settings::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_label.empty (), "Attempting to get_attribute of an invalid filter object.");
  bool Result (false);
  if (data_access::names::label_name () == a_name)
  {
    a_value.assign (m_label);
    Result = true;
  }
  else if (data_access::names::filter_name () == a_name)
  {
    if (not m_filters.empty ())
    {
      a_value.assign (m_filters);
      Result = true;
    }
  }
  else if (data_access::names::outcome_name () == a_name)
  {
    if (not m_outcomes.empty ())
    {
      a_value.assign (m_outcomes);
      Result = true;
    }
  }
  return Result;

}

/**
 * Return a pointer to the parent object, may be NULL.
 */
const basic_datum & settings::parent() const throw () 
{
  return m_target;
}
/**
 * Construct a new setting object with a_parent and a_label.
 * Swallows the content of a_label
 * 
 * \post a_label.empty
 */
settings::settings(mechanism & a_parent, std::string & a_label)
: basic_datum ()
, m_datum ()
, m_filters ()
, m_info ()
, m_label ()
, m_outcomes ()
, m_target (a_parent)
{
  m_label.swap (a_label);
}

settings::~settings() throw () 
{}

/**
 * The key for this class during serialisation.
 */
std::string settings::serial_name() const 
{
  return data_access::names::settings_name ();
}

/**
 * Write an attribute.  Can assume that a_name and a_value are valid as
 * per the definition() object.
 */
void settings::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::outcome_name () == a_name)
  {
    m_outcomes.swap (a_value);
  }
  else if (data_access::names::filter_name () == a_name)
  {
    m_filters.swap (a_value);
  }
  else if (data_access::names::label_name () == a_name) 
  {
    COMPCHEM_ALWAYS (a_value == label (), serial_name () + " object labels cannot be changed after construction");
  }
  else
  {
    const bool bad_attribute_name (false);
    COMPCHEM_ALWAYS (bad_attribute_name, serial_name () + " objects do not have an attribute named " + a_name);
  }
  

}

/**
 * Called when an object of a derived class will be removed.
 * 
 * throw exception if a_datum is not a child of derived class.
 */
void settings::virt_remove_child(basic_datum * a_datum) 
{
  // formula*, identity* or info?
  COMPCHEM_ALWAYS (has_data () or has_information (), "attempt to remove child from empty object");
  // a_datum should be an info object!
  if (m_info.get () == a_datum)
  {
    m_info.reset ();
  }
  else
  {
    COMPCHEM_ALWAYS (m_datum.get () == a_datum, "attempt to remove non-child object");
    m_datum.reset ();
  }

}

void settings::visit(visitor & a_visitor) const 
{
  a_visitor.visit_settings (*this);
}


} // namespace data_scheme
