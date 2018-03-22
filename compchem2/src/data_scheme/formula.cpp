// ------------------------------------------------------------
// class: formula
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/formula.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/compound.hpp"
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
basic_datum & formula::add_child(const std::string & a_name, std::string & a_label) 
{
  basic_datum * Result (NULL);
  if (data_access::names::info_name () == a_name)
  {
    COMPCHEM_ALWAYS (not has_information (), "You can not add more than one info object here");
    m_info.reset (new info (*this, a_label));
    Result = m_info.get ();
  }
  else if (data_access::names::identity_name () == a_name)
  {
    m_identity.push_back (new identity (*this, a_label));
    Result = &(m_identity.back ());
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
 * Create and return an identity object with a_label as a child of this object.
 * 
 * \pre find_identity(a_label) = end_identity
 * \return /= NULL
 */
identity & formula::add_identity(std::string a_label) 
{
  m_identity.push_back (new identity (*this, a_label));
  // Always last
  append_child (m_identity.back ());
  return m_identity.back ();

}

/**
 * Add an info object as a child of this object.
 * 
 * \pre not has_information
 * \return /= NULL
 */
info & formula::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_information (), "Attempt to add a second info object.");
  COMPCHEM_REQUIRE (not has_information (), "You can not add more than one info object here");
  m_info.reset (new info (*this, a_label));
  // Always first
  if (m_identity.empty ())
  {
    append_child (*(m_info.get ()));
  }
  else
  {
    insert_child (m_identity.back (), *(m_info.get ()));
  }
  return *(m_info.get ());

}

/**
 * Construct a new formula object with a_parent and a_label. 
 * 
 * swallows a_label
 * \post a_label.empty
 */
formula::formula(compound & a_parent, std::string & a_label)
: basic_datum ()
, m_identity ()
, m_info ()
, m_label ()
, m_moiety (a_parent)
{
  m_label.swap (a_label);
}

formula::~formula() throw () 
{}

/**
 * Add attributes to the XML element
 */
bool formula::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not label ().empty (), "Attempting to get_attribute of an invalid formula object.");
  bool Result (false);
  if (data_access::names::label_name () == a_name)
  {
    a_value.assign (m_label);
    Result = true;
  }
  return Result;
}

/**
 * Return a pointer to the parent object.
 */
const basic_datum & formula::parent() const throw () 
{
  return m_moiety;
}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void formula::remove_child(identity & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}

/**
 * Remove a identity object.
 * 
 * \pre a_datum.parent = this
 */
void formula::remove_priv(identity & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (identity_iterator l_f = begin_identity (); l_f != end_identity (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_identity.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key for this class during serialisation.
 */
std::string formula::serial_name() const 
{
  return data_access::names::formula_name ();
}

/**
 * Throw an error as program has no attributes.
 */
void formula::set_attribute(const std::string & a_name, std::string a_value) 
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

void formula::visit(visitor & a_visitor) const 
{
  a_visitor.visit_formula (*this);
}

/**
 * Called when an object of a derived class will be removed.
 * 
 * throw exception if a_datum is not a child of derived class.
 */
void formula::virt_remove_child(basic_datum * a_datum) 
{
  // identity* or info?
  COMPCHEM_ALWAYS (not empty_identity () or has_information ()
                   , "attempt to remove child from empty object");
  if (m_info.get () == a_datum)
  {
    m_info.reset ();
  }
  else
  {
    bool object_is_my_child (false);
    if (not empty_identity ())
    {
      identity * l_tmp (dynamic_cast< identity * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");
  }

}


} // namespace data_scheme
