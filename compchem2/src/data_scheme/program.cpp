// ------------------------------------------------------------
// class: program
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/program.hpp"
#include "data_scheme/data_scheme_helper.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/worker.hpp"
#include "data_access/persistence.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
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
basic_datum & program::add_child(const std::string & a_name, std::string & a_label) 
{
  basic_datum * Result (NULL);
  if (data_access::names::compound_name () == a_name)
  {
    m_catalog.push_back (new compound (*this, a_label));
    Result = &(m_catalog.back ());
  }
  else if (data_access::names::project_name () == a_name)
  {
    m_projects.push_back (new project (*this, a_label));
    Result = &(m_projects.back ());
  }
  else if (data_access::names::project_src_name () == a_name)
  {
    std::string l_dummy;
    std::auto_ptr < project > l_temp (new project (*this, l_dummy));
    l_temp.get ()->connect (a_label);
    m_projects.push_back (l_temp.release ());
    Result = &(m_projects.back ());
  }
  else if (data_access::names::mechanism_name () == a_name)
  {
    m_equipment.push_back (new mechanism (*this, a_label));
    Result = &(m_equipment.back ());
  }
  else if (data_access::names::worker_name () == a_name)
  {
    m_staff.push_back (new worker (*this, a_label));
    Result = &(m_staff.back ());
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
 * Create and add a worker object.
 * 
 * \return /= nul
 */
compound & program::add_compound(std::string a_label) 
{
  m_catalog.push_back (new compound (*this, a_label));
  // Before projects
  if (not m_projects.empty ())
  {
    insert_child (m_projects.front (), m_catalog.back ());
  }
  else
  {
    append_child (m_catalog.back ());
  }
  return m_catalog.back ();
  

}

/**
 * Add an info object.
 * 
 * \pre not has_information
 * \return /= NULL
 */
info & program::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_information (), "Attempt to add a second info object.");
  COMPCHEM_ALWAYS (not has_information (), "Can not add more than one info object here");
  m_info.reset (new info (*this, a_label));
  // First! (worker+,mechanism*,compound*,project*)
  if (not m_staff.empty ())
  {
    insert_child (m_staff.front (), *(m_info.get ()));
  }
  else if (not m_equipment.empty ())
  {
    insert_child (m_equipment.front (), *(m_info.get ()));
  }
  else if (not m_catalog.empty ())
  {
    insert_child (m_catalog.front (), *(m_info.get ()));
  }
  else if (not m_projects.empty ())
  {
    insert_child (m_projects.front (), *(m_info.get ()));
  }
  else
  {
    append_child (*(m_info.get ()));
  }
  return *(m_info.get ());

}

/**
 * Create and add a worker object.
 * 
 * \return /= nul
 */
mechanism & program::add_mechanism(std::string a_label) 
{
  m_equipment.push_back (new mechanism (*this, a_label));
  // Before (compound*,project*)
  if (not m_catalog.empty ())
  {
    insert_child (m_catalog.front (), m_equipment.back ());
  }
  else if (not m_projects.empty ())
  {
    insert_child (m_projects.front (), m_equipment.back ());
  }
  else
  {
    append_child (m_equipment.back ());
  }
  return m_equipment.back ();
  

}

/**
 * Create and add a worker object.
 * 
 * \return /= nul
 */
project & program::add_project(std::string a_label) 
{
  m_projects.push_back (new project (*this, a_label));
  // Always last
  append_child (m_projects.back ());
  return m_projects.back ();
  

}

/**
 * Create and add a worker object.
 * 
 * \return /= nul
 */
project & program::add_project_src(const std::string& a_uri, std::string a_label) 
{
  std::auto_ptr< project > Result (new project (*this, a_label));
  Result.get ()->connect (a_uri);
  // Save successfully created project.
  m_projects.push_back (Result.release ());
  // Always last
  append_child (m_projects.back ());
  return m_projects.back ();

}

/**
 * Create and add a worker object.
 * 
 * \return /= nul
 */
worker & program::add_worker(std::string a_label) 
{
  m_staff.push_back (new worker (*this, a_label));
  // Before mechanism*,compound*,project*
  if (not m_equipment.empty ())
  {
    insert_child (m_equipment.front (), m_staff.back ());
  }
  else if (not m_catalog.empty ())
  {
    insert_child (m_catalog.front (), m_staff.back ());
  }
  else if (not m_projects.empty ())
  {
    insert_child (m_projects.front (), m_staff.back ());
  }
  else
  {
    append_child (m_staff.back ());
  }
  return m_staff.back ();

}

/**
 * Second part of constructor connects the program object to the serialisation source.
 */
void program::connect(const std::string & a_uri)
{
  m_document = data_access::manager::server ().connect (*this, a_uri);
  m_document->read ();
}

/**
 * compound   [data_scheme]
 */
program::compound_iterator program::find_compound(const std::string & a_label) 
{
  return std::find_if (m_catalog.begin (), m_catalog.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&compound::label, _1), a_label));

}

/**
 * compound   [data_scheme]
 */
program::const_compound_iterator program::find_compound(const std::string & a_label) const 
{
  return std::find_if (m_catalog.begin (), m_catalog.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&compound::label, _1), a_label));

}

/**
 * mechanism [data_scheme]
 */
program::mechanism_iterator program::find_mechanism(const std::string & a_label) 
{
  return std::find_if (m_equipment.begin (), m_equipment.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&mechanism::label, _1), a_label));

}

/**
 * mechanism [data_scheme]
 */
program::const_mechanism_iterator program::find_mechanism(const std::string & a_label) const 
{
  return std::find_if (m_equipment.begin (), m_equipment.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&mechanism::label, _1), a_label));

}

/**
 * Find an existing project element. This either returns a null element or a 'project' element.  This requires that a 'project-src' element will be followed.
 */
program::project_iterator program::find_project(const std::string & a_label) 
{
  return std::find_if (m_projects.begin (), m_projects.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&project::label, _1), a_label));

}

/**
 * Find an existing project element. This either returns a null element or a 'project' element.  This requires that a 'project-src' element will be followed.
 */
program::const_project_iterator program::find_project(const std::string & a_label) const 
{
  return std::find_if (m_projects.begin (), m_projects.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&project::label, _1), a_label));

}

/**
 * worker   [data_scheme]
 */
program::worker_iterator program::find_worker(const std::string & a_label) 
{
  return std::find_if (m_staff.begin (), m_staff.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&worker::label, _1), a_label));

}

/**
 * worker   [data_scheme]
 */
program::const_worker_iterator program::find_worker(const std::string & a_label) const 
{
  return std::find_if (m_staff.begin (), m_staff.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&worker::label, _1), a_label));

}

/**
 * Default program constructor has no arguments?
 */
program::program()
: basic_datum ()
, m_catalog ()
, m_document ()
, m_equipment ()
, m_info ()
, m_projects ()
, m_staff ()
{}

program::~program() throw () 
{}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_child(compound & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_child(mechanism & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_child(project & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_child(worker & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a formula object.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_priv(compound & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (compound_iterator l_f = begin_compound (); l_f != end_compound (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_catalog.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * Remove a formula object.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_priv(mechanism & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (mechanism_iterator l_f = begin_mechanism (); l_f != end_mechanism (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_equipment.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * Remove a formula object.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_priv(project & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (project_iterator l_f = begin_project (); l_f != end_project (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_projects.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * Remove a formula object.
 * 
 * \pre a_datum.parent = this
 */
void program::remove_priv(worker & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (worker_iterator l_f = begin_worker (); l_f != end_worker (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_staff.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key for this class during serialisation.
 */
std::string program::serial_name() const 
{
  return data_access::names::program_name ();
}

/**
 * Throw an error as program has no attributes.
 */
void program::set_attribute(const std::string & a_name, std::string a_value) 
{
  COMPCHEM_ALWAYS(false, "Class 'program' has no settable attributes");
}

/**
 * Attempt to find the project named by data_scheme_helper.target_project.
 * Return end_project when project is not found (also auto_project)
 */
program::project_iterator program::target_project() 
{
  return std::find_if (m_projects.begin (), m_projects.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&project::label, _1), data_scheme_helper::helper ().target_project ()));

}

/**
 * Get the full path to the document/source that contains the current element 
 * (also see basic_datum::uri_path)
 */
std::string program::uri_path() const 
{
  return m_document->uri_path ();
}

/**
 * Get the scheme used to connect to the document/source that contains 
 * the current element. (also see basic_datum::uri_scheme)
 */
std::string program::uri_scheme() const 
{
  return m_document->uri_scheme ();
}

/**
 * Called when an object of a derived class will be removed.
 * 
 * throw exception if a_datum is not a child of derived class.
 */
void program::virt_remove_child(basic_datum * a_datum) 
{
  // info?,worker+,mechanism*,compound*,(project|proj-src)*
  COMPCHEM_ALWAYS (not empty_worker () or not empty_mechanism () or not empty_project () or not empty_compound () or has_information ()
                   , "attempt to remove child from empty object");
  if (has_information () and m_info.get () == a_datum)
  {
    m_info.reset ();
  }
  else
  {
    bool object_is_my_child (false);
    if (not empty_compound ())
    {
      compound * l_tmp (dynamic_cast< compound * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    if (not object_is_my_child and not empty_project ())
    {
      project * l_tmp (dynamic_cast< project * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    if (not object_is_my_child and not empty_mechanism ())
    {
      mechanism * l_tmp (dynamic_cast< mechanism * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    if (not object_is_my_child and not empty_worker ())
    {
      worker * l_tmp (dynamic_cast< worker * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");
  }

}

void program::visit(visitor & a_visitor) const 
{
  a_visitor.visit_program (*this);
}

/**
 * Trigger document to write itself.
 * 
 * \pre is_src
 */
void program::write() const 
{
  m_document->write ();
}


} // namespace data_scheme
