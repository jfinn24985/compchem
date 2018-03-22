// ------------------------------------------------------------
// class: memo
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/memo.hpp"
#include "data_scheme/data_scheme_helper.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/info.hpp"
#include "data_access/persistence.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/visitor.hpp"

// Manual source includes
#include "data_access/names.hpp"
#include "data_access/manager.hpp"
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
basic_datum & memo::add_child(const std::string & a_name, std::string & a_label) 
{
  COMPCHEM_ALWAYS (not is_src () or is_open (), "can not add child to closed memo-src");
  basic_datum * Result (NULL);
  if (data_access::names::datum_name () == a_name)
  {
    m_results.push_back (new datum (*this, a_label));
    Result = &(m_results.back ());
  }
  else if (data_access::names::info_name () == a_name)
  {
    m_info.push_back (new info (*this, a_label));
    Result = &(m_info.back ());
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
 * Create and add a datum object.
 * 
 * \return /= NULL
 */
datum & memo::add_datum(std::string a_label) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add child to closed memo-src");
  m_results.push_back (new datum (*this, a_label));
  // Datum before info
  if (not m_info.empty ())
  {
    insert_child (m_info.front (), m_results.back ());
  }
  else
  {
    append_child (m_results.back ());
  }
  return m_results.back ();

}

/**
 * Add a filter label to list of what extracted data into database.
 */
void memo::add_filter_label(const std::string & a_filter) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not filters to closed memo-src objects");
  data_access::names::list_add (m_filters, a_filter);
}

/**
 * Create and add an info object.
 * 
 * \return /= NULL
 */
info & memo::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add child to closed memo-src");
  m_info.push_back (new info (*this, a_label));
  // Always last
  append_child (m_info.back ());
  return m_info.back ();

}

/**
 * TODO Flush data to disk and revert to unopenned state. Freeing
 * any allocated resources.
 * 
 * \pre is_src and is_open
 * (\post not is_open)
 * 
 * TODO: currently does nothing!
 */
void memo::close() 
{

}

/**
 * Set the compound and formula attributes. This is the only
 * way to change the compound/formula reference of a memo
 * object.
 */
void memo::compound_formula(const formula & a_form) 
{
  m_compound = a_form.moiety ().label ();
  m_formula = a_form.label ();

}

/**
 * Second part of constructor for memo-src objects. This sets the document
 * from a_uri and optionally reads the referenced document depending on
 * data_access::helper().auto_open_memo ()
 * 
 * \post a_uri.empty
 */
void memo::connect(const std::string & a_uri) 
{
  m_is_open = false;
  m_document = data_access::manager::server ().connect (*this, a_uri);
  if (data_scheme_helper::helper().auto_open_memo ())
  {
    read ();
  }

}

/**
 * Search for the first datum that has a_outcome. Start search from the
 * optional a_iter (defaults to begin_datum)
 */
memo::datum_iterator memo::find_datum(const std::string& a_outcome, const memo::datum_iterator & a_iter) 
{
  datum_iterator Result (a_iter);
  while (Result != end_datum ())
  {
    if (Result->outcome ().find (a_outcome) < Result->outcome ().size ())
    {
      break;
    }
    ++Result;
  }
  return Result;

}

/**
 * Search for the first datum that has a_outcome. Start search from the
 * optional a_iter (defaults to begin_datum)
 */
memo::const_datum_iterator memo::find_datum(const std::string& a_outcome, const memo::const_datum_iterator & a_iter) const 
{
  const_datum_iterator Result (a_iter);
  while (Result != end_datum ())
  {
    if (Result->outcome ().find (a_outcome) < Result->outcome ().size ())
    {
      break;
    }
    ++Result;
  }
  return Result;

}

/**
 * Get attributes by name. This returns values without checking
 * whether the object is a memo or memo-src object.
 */
bool memo::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_label.empty (), "Attempting to get_attribute of an invalid memo object.");
  bool Result (false);
  if (data_access::names::label_name () == a_name)
  {
    a_value.assign (m_label);
    Result = true;
  }
  else if (data_access::names::mechanism_name () == a_name)
  {
    if (not m_mechanism.empty ())
    {
      a_value.assign (m_mechanism);
      Result = true;
    }
  }
  else if (data_access::names::href_name () == a_name)
  {
    if (not m_href.empty ())
    {
      a_value.assign (m_href);
      Result = true;
    }
  }
  else if (data_access::names::filter_name () == a_name)
  {
    if (not m_filters.empty ())
    {
      a_value.assign (m_filters);
      Result = true;
    }
  }
  else if (data_access::names::formula_name () == a_name)
  {
    if (not m_formula.empty ())
    {
      a_value.assign (m_formula);
      Result = true;
    }
  }
  else if (data_access::names::compound_name () == a_name)
  {
    if (not m_compound.empty ())
    {
      a_value.assign (m_compound);
      Result = true;
    }
  }
  return Result;

}

/**
 * Convert the memo into a src type. This requires that the target file
 * does not exist. The action of this method is similar to 'connect'
 * except it sets the reference document as new (and doesn't auto-read!)
 * 
 * \pre not is_src and ((a_base.empty and exists(parent.database_path)) or exists(a_base))
 * \post is_src
 */
void memo::make_src(const boost::filesystem::path & a_path) 
{
  COMPCHEM_REQUIRE(not is_src (), "Object is already a src type object.");
  m_is_open = true;
  m_document = data_access::manager::server ().connect (*this, a_path.string ());

  COMPCHEM_ENSURE(is_src (), "Failed to convert object into a src type object.");
}

/**
 * Constructor for memo objects (from memo elements) using a_parent and
 * a_label. Swallows a_label content.
 * 
 * \post a_label.empty
 */
memo::memo(experiment & a_parent, std::string & a_label)
: basic_datum ()
, m_compound ()
, m_document ()
, m_filters ()
, m_formula ()
, m_href ()
, m_info ()
, m_is_open (false)
, m_label ()
, m_mechanism ()
, m_page (a_parent)
, m_results ()
{
  m_label.swap (a_label);
}

memo::~memo() throw () 
{}

/**
 * The parent object. The result of this is the same object as page().
 */
const basic_datum & memo::parent() const throw () 
{
  return m_page;
}

/**
 * Open a src type
 * 
 * \pre is_src and not is_open
 */
void memo::read() 
{
  COMPCHEM_REQUIRE(is_src () and not is_open (), "Can not open a non-src or already open src element.");
  m_is_open = true;
  m_document->read ();
}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void memo::remove_child(datum & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void memo::remove_child(info & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a info object.
 * 
 * \pre a_datum.parent = this
 */
void memo::remove_priv(datum & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (datum_iterator l_f = begin_datum (); l_f != end_datum (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_results.erase (l_f);
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
void memo::remove_priv(info & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (info_iterator l_f = begin_info (); l_f != end_info (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_info.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key for this class during serialisation.
 */
std::string memo::serial_name() const 
{
  return data_access::names::memo_name ();
}
/**
 * The key for this class during serialisation.
 */
std::string memo::serial_src_name() const 
{
  return data_access::names::memo_src_name ();
}

/**
 * Write an attribute. Can assume that a_name and a_value are valid as per the
 * definition() object.
 * 
 * NOTE: This is mainly useful during deserialisation. This method has the following
 * restrictions.
 * label, compound
 */
void memo::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::mechanism_name () == a_name)
  {
    COMPCHEM_REQUIRE (not is_src () or is_open (), "can not set mechanism attribute of memo-src object.");
    m_mechanism.swap (a_value);
  }
  else if (data_access::names::href_name () == a_name)
  {
    // Do not use the memo-src/@href as the memo/@href
    if (not is_src () or is_open ())
    {
      m_href.swap (a_value);
    }
    else
    {
      COMPCHEM_ALWAYS (std::string::npos != uri ().find (a_value), "can not change the memo-src uri using set_attribute");
    }
  }
  else if (data_access::names::filter_name () == a_name)
  {
    COMPCHEM_REQUIRE (not is_src () or is_open (), "can not set filter attribute of memo-src object.");
    m_filters.swap (a_value);
  }
  else if (data_access::names::formula_name () == a_name)
  {
    if (m_formula.empty ())
    {
      m_formula.swap (a_value);
    }
    else
    {
      COMPCHEM_ALWAYS (a_value == m_formula, "memo object formula labels cannot be changed once set.");
    }
  }
  else if (data_access::names::compound_name () == a_name)
  {
    if (m_compound.empty ())
    {
      m_compound.swap (a_value);
    }
    else
    {
      COMPCHEM_ALWAYS (a_value == m_compound, "memo object compound labels cannot be changed once set.");
    }
  
  }
  else if (data_access::names::label_name () == a_name)
  {
    if (m_label.empty ())
    {
      m_label.swap (a_value);
    }
    else
    {
      COMPCHEM_ALWAYS (m_label == a_value, "memo object labels cannot be changed once set.");
    }
  }
  else
  {
    const bool bad_attribute_name (false);
    COMPCHEM_ALWAYS (bad_attribute_name, "memo objects do not have an attribute named " + a_name);
  }
  

}

/**
 * Get the full path to the document/source that contains the current element 
 * (also see basic_datum::uri_path)
 */
std::string memo::uri_path() const 
{
  return (is_src () ? m_document->uri_path () : page ().section ().uri_path ());
}

/**
 * Get the scheme used to connect to the document/source that contains 
 * the current element. (also see basic_datum::uri_scheme)
 */
std::string memo::uri_scheme() const 
{
  return (is_src () ? m_document->uri_scheme () : page ().section ().uri_scheme ());
}

/**
 * The URI of the document (memo-src nodes)
 * 
 * \pre is_src
 */
std::string memo::uri() const 
{
  COMPCHEM_REQUIRE(is_src (), "Can not get url from non-source object.");
  return m_document->uri ();
}

/**
 * Called when an object of a derived class will be removed.
 * 
 * throw exception if a_datum is not a child of derived class.
 */
void memo::virt_remove_child(basic_datum * a_datum) 
{
  // datum*,info*
  COMPCHEM_ALWAYS (not empty_datum () or not empty_info ()
                   , "attempt to remove child from empty object");
  // a_datum should be an info object!
  bool object_is_my_child (false);
  if (not empty_datum ())
  {
    datum * l_tmp (dynamic_cast< datum * >(a_datum));
    if (NULL != l_tmp)
    {
      object_is_my_child = true;
      remove_priv (*l_tmp);
    }
  }
  if (not object_is_my_child and not empty_info ())
  {
    info * l_tmp (dynamic_cast< info * >(a_datum));
    if (NULL != l_tmp)
    {
      object_is_my_child = true;
      remove_priv (*l_tmp);
    }
  }
  COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");

}

/**
 * Call the on_visit(memo) method of the visitor
 */
void memo::visit(visitor & a_visitor) const 
{
  a_visitor.visit_memo (*this);
}

/**
 * Trigger a memo-src document to write itself.
 * 
 * \pre is_src (and is_open?)
 */
void memo::write() const 
{
  COMPCHEM_REQUIRE(is_src () and is_open (), "Can not call write on a non-source/closed element.");
  if (m_is_open) m_document->write ();
}


} // namespace data_scheme
