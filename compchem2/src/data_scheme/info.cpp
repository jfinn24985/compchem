// ------------------------------------------------------------
// class: info
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/info.hpp"
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
basic_datum & info::add_child(const std::string & a_name, std::string & a_label) 
{
  COMPCHEM_ALWAYS (data_access::names::info_name () == a_name, "info class has no child with name " + a_name);
  return add_info (a_label);

}

/**
 * add_child tests if this object can accept the tagname using the
 * meta_datum object. Only then is the virt_add_child method called. 
 * 
 * \return /= NULL
 */
info & info::add_info(std::string a_label) 
{
  m_content.push_back (new info (*this, a_label));
  // Always last
  append_child (m_content.back ());
  return m_content.back ();
}

/**
 * Add attributes to the XML element
 */
bool info::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_name.empty (), "Attempting to get_attribute of an invalid info object.");
  bool Result (false);
  if (data_access::names::name_name () == a_name)
  {
    a_value.assign (m_name);
    Result = true;
  }
  return Result;

}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void info::remove_child(info & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a info object.
 * 
 * \pre a_datum.parent = this
 */
void info::remove_priv(info & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (info_iterator l_f = begin_info (); l_f != end_info (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_content.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key for this class during serialisation.
 */
std::string info::serial_name() const 
{
  return data_access::names::info_name ();
}

/**
 * Write an attribute.  Throw an error if arguments are incorrect.
 */
void info::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::name_name () == a_name) 
  {
    m_name.swap (a_value);
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
void info::virt_remove_child(basic_datum * a_datum) 
{
  COMPCHEM_ALWAYS (not empty_info (), "attempt to remove child from empty object");
  // a_datum should be an info object!
  bool object_is_my_child (false);
  info * l_tmp (dynamic_cast< info * >(a_datum));
  if (NULL != l_tmp)
  {
    object_is_my_child = true;
    remove_priv (*l_tmp);
  }
  COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");

}

void info::visit(visitor & a_visitor) const 
{
  a_visitor.visit_info (*this);
}


} // namespace data_scheme
