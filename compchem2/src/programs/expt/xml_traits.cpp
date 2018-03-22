// ------------------------------------------------------------
// class: xml_traits
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/xml_traits.hpp"

namespace expt {

//Find the first element that has a_name starting from a_start node (inclusive).
//
//\post Result.is_null () or (Result.node_name() = a_name and Result.node_type() = ELEMENT_NODE)

/**
 * Find the first element that has a_name starting from a_start node (inclusive).
 * 
 * \post Result.is_null () or (Result.node_name() = a_name and Result.node_type() = ELEMENT_NODE)
 */
purpletree::dom::node xml_traits::find_node_by_name(const purpletree::dom::node & a_start, const purpletree::dom::dom_string & a_name)
{
  // LOCAL
  purpletree::dom::node Result (a_start);
  // DO
  while (not Result.is_null ())
  {
    if (purpletree::dom::node::ELEMENT_NODE == Result.node_type ()
        and a_name == Result.node_name ())
      {
        break;
      }
    Result = Result.next_sibling ();
  }
  ENSURE (Result.is_null ()
  	or (purpletree::dom::node::ELEMENT_NODE == Result.node_type ()
  	    and a_name == Result.node_name ())
  	, "Invalid node found?");
  return Result;
}

//Find the last element that has a_name starting from a_start node (inclusive).
//
//\post Result.is_null () or (Result.node_name() = a_name and Result.node_type() = ELEMENT_NODE)

/**
 * Find the last element that has a_name starting from a_start node (inclusive).
 * 
 * \post Result.is_null () or (Result.node_name() = a_name and Result.node_type() = ELEMENT_NODE)
 */
purpletree::dom::node xml_traits::find_last_node_by_name(const purpletree::dom::node & a_start, const purpletree::dom::dom_string & a_name)
{
  // LOCAL
  purpletree::dom::node Result (a_start);
  // DO
  while (not Result.is_null ())
  {
    if (purpletree::dom::node::ELEMENT_NODE == Result.node_type ()
        and a_name == Result.node_name ())
      {
        break;
      }
    Result = Result.previous_sibling ();
  }
  ENSURE (Result.is_null ()
  	or (purpletree::dom::node::ELEMENT_NODE == Result.node_type ()
  	    and a_name == Result.node_name ())
  	, "Invalid node found?");
  return Result;
}

//Convert the ISO representation of a date in the attribute of a_cursor as a date.
//
//\pre not a_cursor.is_null
//\pre not a_attr_name.empty 
//\pre not a_cursor.get_attribute (a_attr_name).empty
//\post not Result.is_not_a_date ()

/**
 * Convert the ISO representation of a date in the attribute of a_cursor as a date.
 * 
 * \pre not a_cursor.is_null
 * \pre not a_attr_name.empty 
 * \pre not a_cursor.get_attribute (a_attr_name).empty
 * \post not Result.is_not_a_date ()
 */
boost::gregorian::date xml_traits::get_date_attribute(const purpletree::dom::element & a_cursor, const purpletree::dom::dom_string & a_attr_name)
{
  // PRE
  REQUIRE (not a_cursor.is_null (), "Cannot get an attribute from a null node.");
  REQUIRE (not a_attr_name.empty (), "Cannot get an attribute with no name.");
  purpletree::dom::dom_string l_date (a_cursor.get_attribute (a_attr_name));
  REQUIRE (not l_date.empty (), "Valid attribute is required to get date.");
  // DO
  // Get the date
  boost::gregorian::date Result (boost::gregorian::from_undelimited_string (purpletree::dom::get_utf8(l_date)));
  ENSURE (not Result.is_not_a_date (), "Invalid date attribute");
  return Result;
}

//Retrieve the text content of an element. It is the concatentation of all text nodes. Returned value may be an empty string
//
//\pre not a_node.is_null

/**
 * Retrieve the text content of an element. It is the concatentation of all text nodes. Returned value may be an empty string
 * 
 * \pre not a_node.is_null
 */
std::string xml_traits::get_node_content(const purpletree::dom::element & a_node)
{
  // LOCAL
  std::stringstream Result;
  // PRE
  REQUIRE (not a_node.is_null ()
  	 , "Cannot get node content from a null node.");
  // DO
  for (purpletree::dom::node l_node (a_node.first_child ());
       not l_node.is_null ();
       l_node = l_node.next_sibling ())
  {
    if (purpletree::dom::node::TEXT_NODE == l_node.node_type ())
      {
        Result << purpletree::dom::get_utf8 (l_node.node_value ());
      }
  }
  return Result.str ();
}

//Set attribute with a_attr_name of a_cursor to a_value.  If a_value is an empty string then attribute is removed. Return value indicates if attribute value was actually changed.
//
//\pre not a_cursor.is_null
//\post false implies a_value = (a_cursor_OLD).get_attribute (a_attr_name)
//\post a_value = a_cursor.get_attribute (a_attr_name)

/**
 * Set attribute with a_attr_name of a_cursor to a_value.  If a_value is an empty string then attribute is removed. Return value indicates if attribute value was actually changed.
 * 
 * \pre not a_cursor.is_null
 * \post false implies a_value = (a_cursor_OLD).get_attribute (a_attr_name)
 * \post a_value = a_cursor.get_attribute (a_attr_name)
 */
bool xml_traits::set_attribute(purpletree::dom::element & a_cursor, const purpletree::dom::dom_string & a_attr_name, const purpletree::dom::dom_string & a_value)
{
  // LOCAL
  bool Result(false);
  // PRE
  REQUIRE (not a_cursor.is_null (), "Cannot change a null node.");
  if (a_cursor.get_attribute (a_attr_name) != a_value)
  {
    if (a_value.empty ())
      {
        // Empty value means remove attribute
        a_cursor.remove_attribute (a_attr_name);
      }
    else
      {
        a_cursor.set_attribute (a_attr_name, a_value);
      }
    Result = true;
    ENSURE (a_cursor.get_attribute (a_attr_name) == a_value
  	  , "Real value should equal value given to set_attribute.")
  }
  return Result;
  
}

//Set an attribute of a_cursor to be a representation of a_date in ISO format.
//
//\pre not a_cursor.is_null and not a_attr_name.empty
//\pre not a_value.is_not_a_date
//\pre a_initial implies a_cursor.get_attribute (a_attr_name).empty()
//\post false implies a_value = get_date_attribute (a_cursor_OLD, a_attr_name)
//\post a_value = get_date_attribute (a_cursor, a_attr_name)

/**
 * Set an attribute of a_cursor to be a representation of a_date in ISO format.
 * 
 * \pre not a_cursor.is_null and not a_attr_name.empty
 * \pre not a_value.is_not_a_date
 * \pre a_initial implies a_cursor.get_attribute (a_attr_name).empty()
 * \post false implies a_value = get_date_attribute (a_cursor_OLD, a_attr_name)
 * \post a_value = get_date_attribute (a_cursor, a_attr_name)
 */
bool xml_traits::set_date_attribute(purpletree::dom::element & a_cursor, const purpletree::dom::dom_string & a_attr_name, const boost::gregorian::date & a_value, bool a_initial)
{
  // LOCAL
  bool Result (false);
  // PRE
  REQUIRE (not a_cursor.is_null (), "Cannot change a null node.");
  REQUIRE (not a_attr_name.empty (), "Cannot have an attribute with no name.");
  REQUIRE (not a_value.is_not_a_date ()
  	 , "Cannot set a date with value that is_not_a_date.");
  // DO
  REQUIRE (a_initial == a_cursor.get_attribute (a_attr_name).empty ()
  	 , "Can only set initial attribute when attribute is empty.");
  
  // Test if a change occurred
  if (a_initial or get_date_attribute (a_cursor, a_attr_name) != a_value)
  {
    a_cursor.set_attribute (a_attr_name
  			  , purpletree::dom::set_utf8(boost::gregorian::to_iso_string (a_value)));
    Result = true;
    ENSURE (a_value == get_date_attribute (a_cursor, a_attr_name)
  	  , "Current date does not equal value input to set_date_attribute");
  }
  return Result;
}

//Set the text content of the node.  If any text nodes exist, the content of the first will be deleted and all other text nodes will be removed.  If no text nodes exist, one will be created and appended to the node. If a_value is empty then all content is removed.
//
//\pre a_node.node_type () = ELEMENT_TYPE
//\post a_value = get_node_content (a_node)

/**
 * Set the text content of the node.  If any text nodes exist, the content of the first will be deleted and all other text nodes will be removed.  If no text nodes exist, one will be created and appended to the node. If a_value is empty then all content is removed.
 * 
 * \pre a_node.node_type () = ELEMENT_TYPE
 * \post a_value = get_node_content (a_node)
 */
bool xml_traits::set_node_content(purpletree::dom::element & a_node, const std::string & a_value)
{
  // LOCAL
  bool Result (false);
  // PRE
  REQUIRE (not a_node.is_null (), "Cannot change the content of an empty node.");
  REQUIRE (not a_node.owner_document ().is_null ()
  	 , "Cannot change content of node with no document.");
  // DO
  std::string l_old_value;
  //try
  //{
  l_old_value = get_node_content (a_node);
  //}
  //catch (const std::exception &a_err)
  //{
  //  ALWAYS(false, a_err.what ());
  //}
  
  // Check if content has changed.
  if (a_value != l_old_value)
  {
    purpletree::dom::dom_string l_value (purpletree::dom::set_utf8 (a_value));
    purpletree::dom::node l_node (a_node.first_child ());
    // find the first text node.
    while (not l_node.is_null ()
  	 and purpletree::dom::node::TEXT_NODE != l_node.node_type ())
      {
        l_node = l_node.next_sibling ();
      }
    
    // If there is a new value set the content of this first node
    // (creating it if necessary)
    if (not a_value.empty ())
      {
        if (l_node.is_null ())
  	{ 
  	  // When there is no text node, add one
  	  l_node = a_node.owner_document ().create_text_node (l_value);
  	  a_node.append_child (l_node);
  	}
        else
  	{
  	  // When there is a text node, set value.
  	  l_node.node_value (l_value);
  	}
        // Move to the next node (which may be null)
        l_node = l_node.next_sibling ();
      }
    // Delete any remaining TEXT_NODE nodes. When a_value is not empty
    // we moved from the first TEXT_NODE to its sibling above.  Below we
    // delete all TEXT_NODE including the current one.  The corollary of
    // this is that we delete all TEXT_NODEs when a_value is empty.
    while (not l_node.is_null ())
      {
        if (purpletree::dom::node::TEXT_NODE != l_node.node_type ())
  	{
  	  purpletree::dom::node l_temp = l_node.next_sibling ();
  	  l_node.parent_node ().remove_child (l_node);
  	  l_node = l_temp;
  	}
        else
  	{
  	  l_node = l_node.next_sibling ();
  	}
      }
    ENSURE (a_value == get_node_content (a_node)
  	  , "Set value is different to value given to set_node_content.");
    Result = true;
  }
  return Result;
}


} // namespace expt
