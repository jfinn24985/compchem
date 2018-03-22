// ------------------------------------------------------------
// class: worker
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/worker.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/visitor.hpp"

// -
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
basic_datum & worker::add_child(const std::string & a_name, std::string & a_label) 
{
  if (data_access::names::info_name () != a_name)
  {
    const bool bad_child_name (false);
    COMPCHEM_ALWAYS (bad_child_name, serial_name() + " class has no child with name " + a_name);
  }
  return add_info (a_label);

}

/**
 * add_child tests if this object can accept the tagname using the
 * meta_datum object. Only then is the virt_add_child method called. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
info & worker::add_info(std::string a_label) 
{
  m_information.push_back (new info (*this, a_label));
  // Always last
  append_child (m_information.back ());
  return m_information.back ();

}

/**
 * Add attributes to the XML element
 */
bool worker::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_label.empty (), "Attempting to get_attribute of an invalid worker object.");
  bool Result (false);
  if (data_access::names::label_name () == a_name)
  {
    a_value.assign (m_label);
    Result = true;
  }
  return Result;

}

/**
 * Return a pointer to the parent object, may be NULL.
 */
const basic_datum & worker::parent() const throw () 
{
  return m_site;
}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void worker::remove_child(info & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a info object.
 * 
 * \pre a_datum.parent = this
 */
void worker::remove_priv(info & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (info_iterator l_f = begin_info (); l_f != end_info (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_information.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The XML element definition for this datum.
 */
std::string worker::serial_name() const 
{
  return data_access::names::worker_name ();
}

/**
 * Write an attribute. Can assume that a_name and a_value are valid as per the
 * definition() object.
 */
void worker::set_attribute(const std::string & a_name, std::string a_value) 
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
void worker::virt_remove_child(basic_datum * a_datum) 
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

void worker::visit(visitor & a_visitor) const 
{
  a_visitor.visit_worker (*this);
}

/**
 * Construct a new worker object with a_parent and a_label.
 * Swallow the content of a_label.
 * 
 * \post a_label.empty
 */
worker::worker(program & a_parent, std::string & a_label)
: basic_datum()
, m_information ()
, m_label ()
, m_site (a_parent)
{
  m_label.swap (a_label);
}

worker::~worker() throw () 
{}


} // namespace data_scheme
