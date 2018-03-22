// ------------------------------------------------------------
// class: compound
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/compound.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
#include <boost/bind.hpp>
// -
namespace data_scheme {

/**
 * Create a basic_datum child of this node with the type tagname a_name.
 * Throw an error not an allowed child.  Returns a reference to the new 
 * child object.
 */
basic_datum & compound::add_child(const std::string & a_name, std::string & a_label) 
{
  basic_datum * Result (NULL);
  if (data_access::names::formula_name () == a_name)
  {
    m_instances.push_back (new formula (*this, a_label));
    Result = &m_instances.back ();
  }
  else if (data_access::names::identity_name () == a_name)
  {
    m_identity.push_back (new identity (*this, a_label));
    Result = &m_identity.back ();
  }
  else if (data_access::names::info_name () == a_name)
  {
    COMPCHEM_ALWAYS (NULL == m_info.get (), "Can not add more than one info object");
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
 * Create and insert a formula element. Return a pointer to the created formula.
 * 
 * \pre find_formula(a_label) = end_formula
 * \post return /= nul
 */
formula & compound::add_formula(std::string a_label) 
{
  // LOCAL
  m_instances.push_back (new formula (*this, a_label));
  // formulae are always last
  append_child (m_instances.back ());
  return m_instances.back ();

}

/**
 * Create and insert a identity element. Return a pointer to the created formula.
 * 
 * \pre find_identity(a_label) = end_identity
 * \post return /= nul
 */
identity & compound::add_identity(std::string a_label) 
{
  // LOCAL
  // Create new object.
  m_identity.push_back (new identity (*this, a_label));
  // Insert: Identity is first elements.
  if (has_information ())
  {
    insert_child (*(m_info.get ()), m_identity.back ());
  }
  else if (not m_instances.empty ())
  {
    insert_child (m_instances.front (), m_identity.back ());
  }
  else
  {
    append_child (m_identity.back ());
  }
  return m_identity.back ();

}

/**
 * Create and add an information object. Return the object.
 * 
 * \pre not has_information
 * \post return /= nul
 */
info & compound::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_information (), "Attempt to add a second info object.");
  COMPCHEM_REQUIRE (not has_information (), "You can not add more than one info object");
  // To set a node we check if node exists, and replace if not equal OR we insert in the right place.
  m_info.reset (new info (*this, a_label));
  // Insert: Info is before formula elements.
  if (not m_instances.empty ())
  {
    insert_child (m_instances.front (), *(m_info.get ()));
  }
  else
  {
    append_child (*(m_info.get ()));
  }
  return *(m_info.get ());

}

/**
 * A space delimited set of category IDs that allow formulae to be grouped in meaningful ways.  Order is not
 * important, though priority can be flagged by ending an ID with the ":" and a number (lower
 * number is higher priority).  For example if you are studying a reaction with several
 * steps you would identify each step with an ID, and if you are making a comparison across
 * substituents then they could be IDed.  It would then be possible for a report to
 * generate a table of reaction v. substituent.
 */
void compound::add_category_label(const std::string& a_cat_set) 
{
  data_access::names::list_add (m_categories, a_cat_set);
}
/**
 * Construct a new compound object, swallow a_label
 * 
 * \post a_label.empty
 */
compound::compound(program & a_parent, std::string & a_label) 
: basic_datum ()
, m_categories ()
, m_identity ()
, m_info ()
, m_instances ()
, m_label ()
, m_parent (a_parent)
{
  m_label.swap (a_label);
}

compound::~compound() throw () 
{}

/**
 * Search for a formula with the given label
 */
compound::formula_iterator compound::find_formula(const std::string & a_label) 
{
  return std::find_if (begin_formula (), end_formula ()
      , boost::bind (std::equal_to< std::string >(), boost::bind (&formula::label, _1), a_label));

}

/**
 * Search for a formula with the given label
 */
compound::const_formula_iterator compound::find_formula(const std::string & a_label) const 
{
  return std::find_if (begin_formula (), end_formula ()
      , boost::bind (std::equal_to< std::string >(), boost::bind (&formula::label, _1), a_label));

}

/**
 * Add attributes to element writer.
 * 
 * No default implementation.
 */
bool compound::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not label ().empty (), "Attempting to write an invalid compound object.");
  bool Result (false);
  // Process attributes.
  if (a_name == data_access::names::label_name ())
  {
    a_value.assign (m_label);
    Result = true;
  }
  else if (a_name == data_access::names::category_name ())
  {
    if (not m_categories.empty ())
    {
      a_value.assign (m_categories);
      Result = true;
    }
  }
  return Result;

}

/**
 * Return a pointer to the parent object, may be NULL.
 */
const basic_datum & compound::parent() const throw () 
{
  return m_parent;
}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void compound::remove_child(formula & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void compound::remove_child(identity & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}

/**
 * Remove a formula object.
 * 
 * \pre a_datum.parent = this
 */
void compound::remove_priv(formula & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (formula_iterator l_f = begin_formula (); l_f != end_formula (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_instances.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * Remove a identity object.
 * 
 * \pre a_datum.parent = this
 */
void compound::remove_priv(identity & a_datum) 
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
 * The XML element name for this datum.
 */
std::string compound::serial_name() const 
{
  return data_access::names::compound_name ();
}

/**
 * Write an attribute.  Throw an error if arguments are incorrect or if no 
 * attributes are allowed.
 */
void compound::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::category_name () == a_name) 
  {
    m_categories.swap (a_value);
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
void compound::virt_remove_child(basic_datum * a_datum) 
{
  // formula*, identity* or info?
  COMPCHEM_ALWAYS (not empty_formula () or not empty_identity () or has_information ()
                   , "attempt to remove child from empty object");
  if (has_information () and m_info.get () == a_datum)
  {
    m_info.reset ();
  }
  else
  {
    bool object_is_my_child (false);
    if (not empty_formula ())
    {
      formula * l_tmp (dynamic_cast< formula * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    if (not object_is_my_child and not empty_identity ())
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

void compound::visit(visitor & a_visitor) const 
{
  a_visitor.visit_compound (*this);
}


} // namespace data_scheme
