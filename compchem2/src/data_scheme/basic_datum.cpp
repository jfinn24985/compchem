// ------------------------------------------------------------
// class: basic_datum
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/basic_datum.hpp"
#include "data_scheme/text.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_access/names.hpp"
#include <algorithm>
#include <boost/bind.hpp>
// -
namespace data_scheme {

/**
 * add_child creates a new object corresponding to a_name and appends it
 * to the list of children.
 * 
 * Note: add_child always appends objects to list. It should therefore only be 
 * used by deserialisation classes.
 * 
 * throw is a_name is not a valid child.
 * base-class action is to throw an error!
 * 
 * \post a_label.empty
 * 
 * \return /= NULL
 */
basic_datum & basic_datum::add_child(const std::string & a_name, std::string & a_label) 
{
  COMPCHEM_ALWAYS(false, "Can not add children to a this object");
  // Dummy return value.
  return *this;
}

/**
 * Create a comment object and append to child list.  Swallow 
 * a_value. Throw an error if no children are allowed.  Returns a 
 * reference to the new child object.
 * 
 * Swallows text!
 * \post a_value.empty
 * 
 * \return /= NULL
 */
comment & basic_datum::add_comment_swallow(std::string & a_value) 
{
  std::auto_ptr< comment > l_child (new comment (*this, a_value));
  comment * Result (l_child.get ());
  m_base_elements.push_back (l_child.release ());
  m_children.push_back (Result);
  return *(Result);

}

/**
 * Create a processing_instruction and append it to list of children. Throw 
 * an error if no children are allowed.  Returns a reference to the new child 
 * object. Swallows a_value text!
 * 
 * \post a_value.empty
 * 
 * \return /= NULL
 */
processing_instruction & basic_datum::add_pi_swallow(const std::string & a_name, std::string & a_value) 
{
  std::auto_ptr< processing_instruction > l_child (new processing_instruction (*this, a_name, a_value));
  processing_instruction * Result (l_child.get ());
  m_base_elements.push_back (l_child.release ());
  m_children.push_back (Result);
  return *Result;

}

/**
 * Create a text object and append it to list of children. Throw 
 * an error if no children are allowed.  Returns a reference to the new child 
 * object. Swallows a_value text!
 * 
 * \post a_value.empty
 * 
 * \return /= NULL
 */
text & basic_datum::add_text_swallow(std::string & a_value) 
{
  std::auto_ptr< text > l_child (new text (*this, a_value));
  text * Result (l_child.get ());
  m_base_elements.push_back (l_child.release ());
  m_children.push_back (Result);
  return *Result;

}

basic_datum::basic_datum() 
: m_base_elements ()
, m_children ()
{}

basic_datum::~basic_datum() throw () 
{}

/**
 * Get attribute value. If the object has an attribute value corresponding to 
 * a_name then it is placed in a_value and true is returned. Otherwise false
 * is returned and a_value is unchanged.
 * 
 * \post return=false implies a_value.pre === a_value.post
 */
bool basic_datum::get_attribute(const std::string & a_name, std::string & a_value) const 
{
  return false;
}

/**
 * Remove the given iterator from the children list.  
 * 
 * If the removed object was owned by the basic_datum base class object 
 * then virt_remove_text will be called.
 * 
 * \pre (unchecked) a_item in [begin, end)
 * \post a_item is invalid.
 */
void basic_datum::remove(basic_datum::iterator a_item) 
{
  COMPCHEM_REQUIRE (a_item != end (), "iterator can not equal end iterator.");
  if (not m_base_elements.empty ())
  {
    basic_text * l_tmp (dynamic_cast< basic_text * >(*a_item));
    if (NULL != l_tmp)
    {
      for (boost::ptr_vector< basic_text >::iterator a_item2 (m_base_elements.begin ()); m_base_elements.end () != a_item2; ++a_item2)
      {
        if (&(*a_item2) == l_tmp)
        {
          m_base_elements.erase (a_item2);
          break;
        }
      }
    }
    else
    {
      virt_remove_child (*a_item);
    }
  }
  else
  {
    virt_remove_child (*a_item);
  }
  // Remove from child list
  erase (a_item);

}

/**
 * Called when an object of a derived class will be removed.
 * 
 * throw exception if a_datum is not a child of derived class.
 */
void basic_datum::virt_remove_child(basic_datum * a_datum) 
{
  const bool have_no_children (false);
  COMPCHEM_ALWAYS(have_no_children, "This type can have no children.");
}


} // namespace data_scheme
