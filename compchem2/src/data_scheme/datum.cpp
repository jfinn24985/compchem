// ------------------------------------------------------------
// class: datum
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/datum.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
#include <boost/lexical_cast.hpp>
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
basic_datum & datum::add_child(const std::string & a_name, std::string & a_label) 
{
  if (data_access::names::datum_name () != a_name)
  {
    const bool bad_child_name (false);
    COMPCHEM_ALWAYS (bad_child_name, serial_name() + " class has no child with name " + a_name);
  }
  return add_datum (a_label);

}

/**
 * Create a basic_datum child of this node with the type tagname a_name.
 * Throw an error  if the a_name is not correct. Throw an error if no 
 * children are allowed.  Returns a reference to the new child object.
 * 
 * \return /= NULL
 */
datum & datum::add_datum(std::string a_label) 
{
  m_data.push_back (new datum (*this, a_label));
  append_child (m_data.back ());
  return m_data.back ();
}

/**
 * Add attributes to the XML element
 */
bool datum::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  bool Result (false);
  if (data_access::names::outcome_name () == a_name)
  {
    if (not m_outcome.empty ())
    {
      a_value.assign (m_outcome);
      Result = true;
    }
  }
  else if (data_access::names::format_name () == a_name)
  {
    if (not m_format.empty ())
    {
      a_value.assign (m_format);
      Result = true;
    }
  }
  else if (data_access::names::scale_name () == a_name)
  {
    if (not m_scale.empty ())
    {
      a_value.assign (m_scale);
      Result = true;
    }
  }
  else if (data_access::names::ordinal_name () == a_name)
  {
    if (m_ordinal != NO_ORDINAL)
    {
      a_value.assign (boost::lexical_cast< std::string >(m_ordinal));
      Result = true;
    }
  }
  return Result;

}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void datum::remove_child(datum & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a memo object.
 * 
 * \pre a_datum.parent = this
 */
void datum::remove_priv(datum & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (datum_iterator l_f = begin_datum (); l_f != end_datum (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_data.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key name for this class during serialisation.
 */
std::string datum::serial_name() const 
{
  return data_access::names::datum_name ();
}

/**
 * Write an attribute.  Throw an error if arguments are incorrect or if no 
 * attributes are allowed.
 */
void datum::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::outcome_name () == a_name) 
  {
    m_outcome.swap (a_value);
  }
  else if (data_access::names::dimension_name () == a_name)
  {
    m_dim.swap (a_value);
  }
  else if (data_access::names::format_name () == a_name)
  {
    m_format.swap (a_value);
  }
  else if (data_access::names::scale_name () == a_name)
  {
    m_scale.swap (a_value);
  }
  else if (data_access::names::ordinal_name () == a_name)
  {
    try
    {
      ordinal (boost::lexical_cast< int >(a_value));
    }
    catch (const boost::bad_lexical_cast& a_err)
    {
      const bool ordinal_value_is_not_an_integer (false);
      COMPCHEM_ALWAYS (ordinal_value_is_not_an_integer, a_err.what ());
    }
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
void datum::virt_remove_child(basic_datum * a_datum) 
{
  // datum*
  COMPCHEM_ALWAYS (not empty_datum (), "attempt to remove child from empty object");
  // a_datum should be a datum object!
  bool object_is_my_child (false);
  datum * l_tmp (dynamic_cast< datum * >(a_datum));
  if (NULL != l_tmp)
  {
    object_is_my_child = true;
    remove_priv (*l_tmp);
  }
  COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");

}

void datum::visit(visitor & a_visitor) const 
{
  a_visitor.visit_datum (*this);
}


} // namespace data_scheme
