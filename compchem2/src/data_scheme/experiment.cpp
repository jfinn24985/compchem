// ------------------------------------------------------------
// class: experiment
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/experiment.hpp"
#include "data_scheme/program.hpp"
#include "utility/program_options.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/project.hpp"
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
basic_datum & experiment::add_child(const std::string & a_name, std::string & a_label) 
{
  basic_datum * Result (NULL);
  if (data_access::names::memo_name () == a_name)
  {
    m_results.push_back (new memo (*this, a_label));
    Result = &(m_results.back ());
  }
  else if (data_access::names::memo_src_name () == a_name)
  {
    std::string l_dummy;
    std::auto_ptr< memo > l_temp (new memo (*this, l_dummy));
    l_temp.get ()->connect (a_label);
    m_results.push_back (l_temp.release ());
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
 * add_info child. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
info & experiment::add_info(std::string a_label) 
{
  m_info.push_back (new info (*this, a_label));
  // Infos are first
  if (m_results.empty ())
  {
    append_child (m_info.back ());
  }
  else
  {
    insert_child (m_results.front (), m_info.back ());
  } 
  return m_info.back ();

}

/**
 * add_child tests if this object can accept the tagname using the
 * meta_datum object. Only then is the virt_add_child method called. 
 * 
 * default action is to throw an error!
 * 
 * \pre find_memo (a_label) = end_memo
 * \return /= NULL
 */
memo & experiment::add_memo(std::string a_label) 
{
  m_results.push_back (new memo (*this, a_label));
  // Memos are last
  append_child (m_results.back ());
  return m_results.back ();

}

/**
 * add_child tests if this object can accept the tagname using the
 * meta_datum object. Only then is the virt_add_child method called. 
 * 
 * default action is to throw an error!
 * 
 * \return /= NULL
 */
memo & experiment::add_memo_src(const std::string& a_uri, std::string a_label) 
{
  std::auto_ptr < memo > Result (new memo (*this, a_label));
  Result.get ()->connect (a_uri);
  // Store successfully constructed memo
  m_results.push_back (Result.release ());
  // Memos are last
  append_child (m_results.back ());
  return m_results.back ();

}

void experiment::add_worker_label(const std::string & a_set) 
{
  data_access::names::list_add (m_workers, a_set);
}

/**
 * The starting date of the experiment
 */
boost::gregorian::date experiment::date_start() const 
{
  return (m_date_start.empty ()) ? boost::gregorian::date () : boost::gregorian::from_undelimited_string (m_date_start);
}


/**
 * The starting date of the experiment.  If a_date_str is empty then today's date is used.
 */
void experiment::date_start(const boost::gregorian::date& a_date) 
{
  m_date_start = boost::gregorian::to_iso_string (a_date);
}


/**
 * The finish date for the expt.
 */
boost::gregorian::date experiment::date_end() const 
{
  return (m_date_end.empty ()) ? date_start () : boost::gregorian::from_undelimited_string (m_date_end);
}
/**
 * The finish date for the expt. If a_date_str is empty, then value of date_start() is used.
 * 
 * \pre a_date >= date_start ()
 */
void experiment::date_end(const boost::gregorian::date& a_date) 
{
  const boost::gregorian::date l_start (date_start ());
  COMPCHEM_REQUIRE (l_start <= a_date, "End date must be the same or after start date.");
  if (l_start == a_date and not m_date_end.empty ())
  {
    m_date_end.clear ();
  }
  else
  {
    m_date_end = boost::gregorian::to_iso_string (a_date);
  }
}


/**
 * Construct a new experiment object as a child of a_parent. Swallow
 * a_reference
 * 
 * \post a_reference.empty
 */
experiment::experiment(project & a_parent, std::string & a_reference) 
: basic_datum ()
, m_date_start ()
, m_date_end ()
, m_info ()
, m_priority (0)
, m_reference ()
, m_results ()
, m_section (a_parent)
, m_workers ()
{
  m_reference.swap (a_reference);
}

experiment::~experiment() throw () 
{}

/**
 * memo [data_scheme]
 */
experiment::memo_iterator experiment::find_memo(const std::string & a_label) 
{
  return std::find_if (m_results.begin (), m_results.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&memo::label, _1), a_label));

}

/**
 * memo [data_scheme]
 */
experiment::const_memo_iterator experiment::find_memo(const std::string & a_label) const 
{
  return std::find_if (m_results.begin (), m_results.end (), boost::bind (std::equal_to< std::string >(), boost::bind (&memo::label, _1), a_label));

}

/**
 * Add attributes to the XML element
 */
bool experiment::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  COMPCHEM_REQUIRE (not reference ().empty (), "Attempting to get_attribute from an invalid experiment object.");
  bool Result (false);
  if (data_access::names::reference_name () == a_name)
  {
    a_value.assign (m_reference);
    Result = true;
  }
  else if (data_access::names::date_start_name () == a_name)
  {
    if (not m_date_start.empty ())
    {
      a_value.assign (m_date_start);
      Result = true;
    }
  }
  else if (data_access::names::date_end_name () == a_name)
  {
    if (not m_date_end.empty ())
    {
      a_value.assign (m_date_end);
      Result = true;
    }
  }
  else if (data_access::names::priority_name () == a_name)
  {
    if (0 != m_priority)
    {
      a_value.assign (boost::lexical_cast< std::string > (m_priority));
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
 * Fill in the default information for a newly created
 * object.
 */
void experiment::initialise() 
{
  // Set date-start and worker if not already set.
  if (m_workers.empty ())
  {
    m_workers = utility::program_options::helper ().current_worker ();
  }
  if (m_date_start.empty ())
  {
    date_start (boost::gregorian::day_clock::local_day());
  }
  

}

/**
 * Return a pointer to the parent object, may be NULL.
 */
const basic_datum & experiment::parent() const throw () 
{
  return m_section;
}

/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void experiment::remove_child(memo & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a_datum from our list of children.
 * 
 * \pre a_datum.parent = this
 */
void experiment::remove_child(info & a_datum) 
{
  remove_priv (a_datum); // performs validity checking.
  erase (find (&a_datum)); // Remove from base.
}
/**
 * Remove a memo object.
 * 
 * \pre a_datum.parent = this
 */
void experiment::remove_priv(memo & a_datum) 
{
  COMPCHEM_REQUIRE (&(a_datum.parent ()) == this, "attempt to remove an object that is not a child of this object.");
  bool object_is_child (false);
  for (memo_iterator l_f = begin_memo (); l_f != end_memo (); ++l_f)
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
void experiment::remove_priv(info & a_datum) 
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
 * The XML element definition for this datum.
 */
std::string experiment::serial_name() const 
{
  return data_access::names::experiment_name ();
}

/**
 * Write an attribute. Can assume that a_name and a_value are valid as per the
 * definition() object.
 */
void experiment::set_attribute(const std::string & a_name, std::string a_value) 
{
  if (data_access::names::date_start_name () == a_name)
  {
    m_date_start.swap (a_value);
  }
  else if (data_access::names::date_end_name () == a_name)
  {
    m_date_end.swap (a_value);
  }
  else if (data_access::names::priority_name () == a_name)
  {
    try
    {
      m_priority = boost::lexical_cast< unsigned int > (a_value);
    }
    catch (const boost::bad_lexical_cast &a_err)
    {
      const bool priority_value_is_not_a_positive_integer (false);
      COMPCHEM_ALWAYS (priority_value_is_not_a_positive_integer, a_err.what ());
    }
  }
  else if (data_access::names::worker_name () == a_name)
  {
    m_workers.swap (a_value);
  }
  else if (data_access::names::reference_name () == a_name) 
  {
    COMPCHEM_ALWAYS (a_value == reference (), serial_name () + " object reference labels cannot be changed after construction");
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
void experiment::virt_remove_child(basic_datum * a_datum) 
{
  // info*, memo*
  COMPCHEM_ALWAYS (not empty_memo () or not empty_info ()
                   , "attempt to remove child from empty object");
  bool object_is_my_child (false);
  if (not empty_memo ())
  {
    memo * l_tmp (dynamic_cast< memo * >(a_datum));
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

void experiment::visit(visitor & a_visitor) const 
{
  a_visitor.visit_experiment (*this);
}


} // namespace data_scheme
