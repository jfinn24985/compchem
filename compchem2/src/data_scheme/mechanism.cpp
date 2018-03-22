// ------------------------------------------------------------
// class: mechanism
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/mechanism.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/settings.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
#include <boost/bind.hpp>
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
basic_datum & mechanism::add_child(const std::string & a_name, std::string & a_label) 
{
  basic_datum * Result (NULL);
  if (data_access::names::info_name () == a_name)
  {
    COMPCHEM_ALWAYS (not has_information (), "You can only add one info object here");
    m_info.reset (new info (*this, a_label));
    Result = m_info.get ();
  }
  else if (data_access::names::filter_name () == a_name)
  {
    m_filter_set.push_back (new filter (*this, a_label));
    Result= &(m_filter_set.back ());
  }
  else if (data_access::names::settings_name () == a_name)
  {
    m_command_set.push_back (new settings (*this, a_label));
    Result = &(m_command_set.back ());
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
 * \pre not has_information
 * \return /= NULL
 */
info & mechanism::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_information (), "Attempt to add a second info object.");
  COMPCHEM_REQUIRE (not has_information (), "You can only add one info object here");
  m_info.reset (new info (*this, a_label));
  // Always first
  if (not m_filter_set.empty ())
  {
    insert_child (m_filter_set.front (), *(m_info.get ()));
  }
  else if (not m_command_set.empty ())
  {
    insert_child (m_command_set.front (), *(m_info.get ()));
  }
  else 
  {
    append_child (*(m_info.get ()));
  }
  return *(m_info.get ());

}

/**
 * Create and add a filter.
 * 
 * \pre find_filter(a_label) = end_label
 * \return /= NULL
 */
filter & mechanism::add_filter(std::string a_label) 
{
  m_filter_set.push_back (new filter (*this, a_label));
  if (not m_command_set.empty ())
  {
    insert_child (m_command_set.front (), m_filter_set.back ());
  }
  else
  {
    append_child (m_filter_set.back ());
  }
  return m_filter_set.back ();

}

/**
 * Create and add a settings child object.
 * 
 * \pre find_settings(a_label) = end_settings
 * \return /= NULL
 */
settings & mechanism::add_setting(std::string a_label) 
{
  m_command_set.push_back (new settings (*this, a_label));
  // Always last
  append_child (m_command_set.back ());
  return m_command_set.back ();

}

/**
 * Find filter with matching label
 */
mechanism::filter_iterator mechanism::find_filter(const std::string & a_label) 
{
  return std::find_if (m_filter_set.begin (), m_filter_set.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&filter::label, _1), a_label));

}

/**
 * Find filter with matching label
 */
mechanism::const_filter_iterator mechanism::find_filter(const std::string & a_label) const 
{
  return std::find_if (m_filter_set.begin (), m_filter_set.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&filter::label, _1), a_label));

}

/**
 * Add attributes to the XML element
 */
bool mechanism::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_label.empty (), "Attempting to get_attribute of an invalid mechanism object.");
  bool Result (false);
  if (data_access::names::label_name () == a_name)
  {
    a_value.assign (m_label);
    Result = true;
  }
  return Result;

}

/**
 * Construct a new mechanism object with a_parent and a_label. Swallow
 * content of a_label
 * 
 * \post a_label.empty
 */
mechanism::mechanism(program & a_parent, std::string & a_label) 
: basic_datum ()
, m_command_set ()
, m_label ()
, m_info ()
, m_filter_set ()
, m_site (a_parent)
{
  m_label.swap (a_label);
}

mechanism::~mechanism() throw ()
{}

/**
 * Return a pointer to the parent object, may be NULL.
 */
const basic_datum & mechanism::parent() const throw () 
{
  return m_site;
}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void mechanism::remove_child(filter & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void mechanism::remove_child(settings & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a filter object.
 * 
 * \pre a_datum.parent = this
 */
void mechanism::remove_priv(filter & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (filter_iterator l_f = begin_filter (); l_f != end_filter (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_filter_set.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * Remove a info object.
 * 
 * \pre a_datum.parent = this
 */
void mechanism::remove_priv(settings & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (settings_iterator l_f = begin_settings (); l_f != end_settings (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_command_set.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key for this class during serialisation.
 */
std::string mechanism::serial_name() const 
{
  return data_access::names::mechanism_name ();
}

/**
 * Throw an error as program has no attributes.
 */
void mechanism::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::label_name () == a_name) 
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
void mechanism::virt_remove_child(basic_datum * a_datum) 
{
  // info,filter*,setting*
  COMPCHEM_ALWAYS (not empty_filter () or not empty_settings () or has_information ()
                   , "attempt to remove child of empty object");
  if (has_information () and m_info.get () == a_datum)
  {
    m_info.reset ();
  }
  else
  {
    bool object_is_my_child (false);
    if (not empty_filter ())
    {
      filter * l_tmp (dynamic_cast< filter * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    if (not object_is_my_child and not empty_settings ())
    {
      settings * l_tmp (dynamic_cast< settings * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");
  }

}

void mechanism::visit(visitor & a_visitor) const 
{
  a_visitor.visit_mechanism (*this);
}


} // namespace data_scheme
