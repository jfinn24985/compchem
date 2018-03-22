// ------------------------------------------------------------
// class: project
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/project.hpp"
#include "data_scheme/data_scheme_helper.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/compound_ref.hpp"
#include "data_access/persistence.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/program.hpp"
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
basic_datum & project::add_child(const std::string & a_name, std::string & a_label) 
{
  COMPCHEM_ALWAYS (not is_src () or is_open (), "can not add children to closed proj-src objects");
  basic_datum * Result (NULL);
  if (data_access::names::experiment_name () == a_name)
  {
    m_activities.push_back (new experiment (*this, a_label));
    Result = &(m_activities.back ());
  }
  else if (data_access::names::info_name () == a_name)
  {
    COMPCHEM_ALWAYS (not has_information (), "Can not add more than one info object here");
    m_info.reset (new info (*this, a_label));
    Result = m_info.get ();
  }
  else if (data_access::names::compound_ref_name () == a_name)
  {
    m_imports.push_back (new compound_ref (*this, a_label));
    Result = &(m_imports.back ());
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
 * create and return a compound_ref, with a_label, as a child of
 * this project.
 * 
 * \return /= NULL
 */
compound_ref & project::add_compound_ref(std::string a_label) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add children to closed proj-src objects");
  m_imports.push_back (new compound_ref (*this, a_label));
  // add before any experiment
  if (empty_experiment ())
  {
    append_child (m_imports.back ());
  }
  else
  {
    insert_child (m_activities.front (), m_imports.back ());
  }
  return m_imports.back ();

}

/**
 * Add an info object.
 * 
 * \pre not has_information
 * \return /= NULL
 */
info & project::add_info(std::string a_label) 
{
  COMPCHEM_REQUIRE(not has_information (), "Attempt to add a second info object.");
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add children to closed proj-src objects");
  COMPCHEM_REQUIRE (not has_information (), "Can not more than one info object here");
  m_info.reset (new info (*this, a_label));
  // First
  if (not m_activities.empty ())
  {
    insert_child (m_activities.front (), *(m_info.get ()));
  }
  else
  {
    append_child (*(m_info.get ()));
  }
  return *(m_info.get ());

}

/**
 * add_child tests if this object can accept the tagname using the
 * meta_datum object. Only then is the virt_add_child method called. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
experiment & project::add_experiment(std::string a_label) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add children to closed proj-src objects");
  m_activities.push_back (new experiment (*this, a_label));
  // Always last
  append_child (m_activities.back ());
  return m_activities.back ();

}

void project::add_outcome_label(const std::string & a_set) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add children to closed proj-src objects");
  data_access::names::list_add (m_outcomes, a_set);
}

/**
 * worker   [data_scheme]
 */
void project::add_worker_label(const std::string & a_worker) 
{
  COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add children to closed proj-src objects");
  data_access::names::list_add (m_workers, a_worker);
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
void project::close() 
{

}

/**
 * Second part of constructor for proj-src derived objects. This
 * must be called after the contructor to create the document.
 */
void project::connect(const std::string & a_uri) 
{
  m_is_open = false;
  m_document = data_access::manager::server ().connect (*this, a_uri);

}

/**
 * experiment [data_scheme]
 */
project::experiment_iterator project::find_experiment(const std::string & a_label) 
{
  return std::find_if (m_activities.begin (), m_activities.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&experiment::reference, _1), a_label));

}

/**
 * experiment [data_scheme]
 */
project::const_experiment_iterator project::find_experiment(const std::string & a_label) const 
{
  return std::find_if (m_activities.begin (), m_activities.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&experiment::reference, _1), a_label));

}

/**
 * Add attributes to the XML element
 */
bool project::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not m_label.empty (), "Attempting to get_attribute of an invalid project object.");
  bool Result (false);
  if (data_access::names::label_name () == a_name)
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
  else if (data_access::names::worker_name () == a_name)
  {
    if (not m_workers.empty ())
    {
      a_value.assign (m_workers);
      Result = true;
    }
  }
  return Result;

}

/**
 * Convert the memo into a src type.
 * 
 * \pre not is_src and ((a_base.empty and exists(parent.database_path)) or exists(a_base))
 * \post is_src
 */
void project::make_src(const boost::filesystem::path & a_path) 
{
  COMPCHEM_REQUIRE(not is_src (), "Object is already a src type object.");
  m_is_open = true;
  m_document = data_access::manager::server ().connect (*this, a_path.string ());

  COMPCHEM_ENSURE(is_src (), "Failed to convert object into a src type object.");
}

/**
 * Reference to the parent object.
 */
const basic_datum & project::parent() const throw () 
{
  return m_site;
}

/**
 * Constructor for project object (from project elements) with a_program
 * parent and a_label. Swallows content of a_label.
 * 
 * \post a_label.empty
 */
project::project(program & a_program, std::string & a_label)
: basic_datum ()
, m_activities ()
, m_document ()
, m_imports ()
, m_info ()
, m_is_open (false)
, m_label ()
, m_outcomes ()
, m_site (a_program)
, m_workers ()
{
  m_label.swap (a_label);
}

project::~project() throw () 
{}

/**
 * Open a src type
 * 
 * \pre is_src
 */
void project::read() 
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
void project::remove_child(experiment & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void project::remove_child(compound_ref & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a info object.
 * 
 * \pre a_datum.parent = this
 */
void project::remove_priv(experiment & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (experiment_iterator l_f = begin_experiment (); l_f != end_experiment (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_activities.erase (l_f);
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
void project::remove_priv(compound_ref & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.section ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (compound_ref_iterator l_f = begin_compound_ref (); l_f != end_compound_ref (); ++l_f)
  {
    if (&(*l_f) == &a_datum)
    {
      m_imports.erase (l_f);
      object_is_child = true;
      break;
    }
  }
  COMPCHEM_ENSURE (true == object_is_child, "child object not found in child list");

}

/**
 * The key for this class during serialisation.
 */
std::string project::serial_name() const 
{
  return data_access::names::project_name ();
}

/**
 * The key for this class during serialisation.
 */
std::string project::serial_src_name() const 
{
  return data_access::names::project_src_name ();
}

/**
 * Write an attribute. Can assume that a_name and a_value are valid as per the
 * definition() object.
 */
void project::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::worker_name () == a_name)
  {
    m_workers.swap (a_value);
  }
  else if (data_access::names::outcome_name () == a_name)
  {
    m_outcomes.swap (a_value);
  }
  else if (data_access::names::label_name () == a_name)
  {
    if (m_label.empty ())
    {
      m_label.swap (a_value);
      if (is_src () and data_scheme_helper::helper ().target_project () == a_value)
      {
        read ();
      }
    }
    else
    {
      COMPCHEM_ALWAYS (m_label == a_value, serial_name () + " object labels cannot be changed once set.");
    }
  }
  else if (data_access::names::href_name () == a_name) 
  {
    COMPCHEM_ALWAYS (std::string::npos != uri ().find (a_value), "can not change the project-src uri using set_attribute");
  }
  else
  {
    const bool bad_attribute_name (false);
    COMPCHEM_ALWAYS (bad_attribute_name, serial_name () + " objects do not have an attribute named " + a_name);
  }
  

}

/**
 * Get the full path to the document/source that contains the current element
 */
std::string project::uri_path() const 
{
  return (is_src () ? m_document->uri_path () : site ().uri_path ());
}

/**
 * Get the scheme used to connect to the document/source that contains 
 * the current element.
 */
std::string project::uri_scheme() const 
{
  return (is_src () ? m_document->uri_scheme () : site ().uri_scheme ());
}

/**
 * The URI of the document (proj-src nodes)
 * 
 * \pre is_src
 */
std::string project::uri() const 
{
  COMPCHEM_REQUIRE(is_src (), "Can not get url from non-source object.");
  return m_document->uri ();
}

/**
 * Called when an object of a derived class will be removed.
 * 
 * throw exception if a_datum is not a child of derived class.
 */
void project::virt_remove_child(basic_datum * a_datum) 
{
  // info?,compound-ref*,experiment*
  COMPCHEM_ALWAYS (not empty_compound_ref () or not empty_experiment () or has_information ()
                   , "attempt to remove child from empty object");
  if (has_information () and m_info.get () == a_datum)
  {
    m_info.reset ();
  }
  else
  {
    bool object_is_my_child (false);
    if (not empty_experiment ())
    {
      experiment * l_tmp (dynamic_cast< experiment * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    if (not object_is_my_child and not empty_compound_ref ())
    {
      compound_ref * l_tmp (dynamic_cast< compound_ref * >(a_datum));
      if (NULL != l_tmp)
      {
        object_is_my_child = true;
        remove_priv (*l_tmp);
      }
    }
    COMPCHEM_ALWAYS (true == object_is_my_child, "attempt to remove non-child object");
  }

}

void project::visit(visitor & a_visitor) const 
{
  a_visitor.visit_project (*this);
}

/**
 * Trigger document to write itself.
 * 
 * \pre is_src
 */
void project::write() const 
{
  COMPCHEM_REQUIRE(is_src () and is_open (), "Can not call write on a non-source/closed element.");
  if (m_is_open) m_document->write ();
}


} // namespace data_scheme
