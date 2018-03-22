// ------------------------------------------------------------
// class: page_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/page_ctl.hpp"

namespace expt {

//Create a new 'page' item, add it to the parent node and return an iterator to it.
//
//\post Result != end () and not (*Result).is_null () and (*Result).node_name == get_node_name

/**
 * Create a new 'page' item, add it to the parent node and return an iterator to it.
 * 
 * \post Result != end () and not (*Result).is_null () and (*Result).node_name == get_node_name
 */
item_iterator page_ctl::create_item() {
  // PRE
  is_valid ();
  // DO
  const purpletree::dom::dom_string l_pref (purpletree::dom::set_utf8 (generate_pageref ()));
  purpletree::dom::document l_doc (get_parent_node ().owner_document ());
  purpletree::dom::element l_node (l_doc.create_element (get_node_name ()));
  
  ENSURE (xml_traits::is_element (l_node, get_node_name ())
  	, "Newly created node should be an element with correct node_name.");
  
  // Add to parent node
  l_node = get_parent_node ().append_child (l_node);
  
  // ** Make the 'page' node valid **
  
  // Set the start date attribute
  xml_traits::set_date_attribute (l_node
  				, START_DATE_ATTR_NAME
  				, boost::gregorian::day_clock::local_day()
  				, true);
  
  // Set the initial page reference
  xml_traits::set_attribute (l_node
  			   , PAGEREF_ATTR_NAME
  			   , l_pref);
  
  // NOTE: The addition sequence 'title node':'info list node':'entry
  // list node' is required for a valid 'page' node
  
  // Set a dummy title node
  purpletree::dom::element l_title (l_doc.create_element (TITLE_NODE_NAME));
  l_node.append_child (l_title);
  
  xml_traits::set_node_content (l_title, "Enter title");
  
  // Add info list parent node
  l_node.append_child (l_doc.create_element (REFERENCE_NODE_NAME));
  
  // Add entry list parent node
  l_node.append_child (l_doc.create_element (LOG_NODE_NAME));
  
  // Indicate that document is dirty.
  journal_ctl::make_dirty (l_node.owner_document ());
  
  // Create iterator
  item_iterator Result(*this, l_node);
  ENSURE (Result != end(), "Iterator for new item should not equal end()");
  // DONE
  return Result;
}

//Indicate the experiment on this page is complete, (sets the final_date.)
//
//\post true = is_done()

/**
 * Indicate the experiment on this page is complete, (sets the final_date.)
 * 
 * \post true = is_done()
 */
void page_ctl::finalise(item_iterator & a_cursor)
{
  REQUIRE (not is_done (a_cursor), "Cannot finalise a page that is already finalised.");
  xml_traits::set_date_attribute (*a_cursor
                                  , FINAL_DATE_ATTR_NAME
                                  , boost::gregorian::day_clock::local_day()
                                  , true);
  journal_ctl::make_dirty ((*a_cursor).owner_document ());
  ENSURE (is_done (a_cursor), "Finalised datum is not done.");
}

//Get an iterator for a page that has the given pageref.  If pageref doesn't exist then returned iterator == end()
//
//\post Result == end () or get_pageref (Result) = a_pageref

/**
 * Get an iterator for a page that has the given pageref.  If pageref doesn't exist then returned iterator == end()
 * 
 * \post Result == end () or get_pageref (Result) = a_pageref
 */
item_iterator page_ctl::find(const std::string & a_pageref) const {
  // PRE
  is_valid ();
  // LOCAL
  const purpletree::dom::dom_string l_reference (purpletree::dom::set_utf8(a_pageref));
  item_iterator Result (begin ());
  // DO
  for (
       ; Result != end () && (*Result).get_attribute (page_ctl::PAGEREF_ATTR_NAME) != l_reference
         ; ++Result) {} // Do nothing in loop
  ENSURE (Result == end()
  	or (*Result).get_attribute (page_ctl::PAGEREF_ATTR_NAME) == l_reference,
  	"Find function should return iterator to correct item OR end()");
  return Result;
}

//Generate a new pageref based on the pageref_format string.  See documentation of pageref_format for how this method generates the new pageref.
//
//\post not Result.empty ()

/**
 * Generate a new pageref based on the pageref_format string.  See documentation of pageref_format for how this method generates the new pageref.
 * 
 * \post not Result.empty ()
 */
std::string page_ctl::generate_pageref() {
  // LOCAL
  std::string Result(get_pageref_format ());
  std::ostringstream l_writer;
  unsigned int l_seq(size () + 1);
  // PRE
  is_valid ();
  // DO
  
  // A template/format string to use to automatically generate a new
  // pageref.  The string can contain any characters.  If the string
  // contains "YYYY" these will be replaced by the year in four digit
  // form.  If the string contains any sequence of "#"s they will be
  // replaced by a sequence number. This is padded with zeros if the
  // size of the sequence number does not equal or exceed the number of
  // "#". If the string contains "AA" this is replaced by the initials
  // of the current experimenter.  If there "YYYY" or "#" are not in the
  // string, the year and sequence number are appended to the string,
  // preceded by a dash:
  
  // "OLYYYY-####" --> "OL2006-0034"
  // "BNEYYYY-AA-#" --> "BNE2006-JJF-34"
  // "LON-AA" --> "LON-JJF-2006-34"
  // "HWK-####" --> "HWK-0034-2006"
  // If the string is empty, the initials, date and sequence number are used:
  // "" --> "JJF-2006-34"
  
  // TODO !!!
  if (Result.empty ())
  {
    // Empty string, construct simple pageref
    l_writer << get_experimenter_initials();
    l_writer << "-";
    l_writer << boost::gregorian::day_clock::local_day().year ();
    l_writer << "-";
    l_writer << l_seq;
    Result = l_writer.str ();
  }
  else
  {
    // Not empty, search for strings to replace
  
    // Search for YYYY
    unsigned int l_pos = Result.find ("YYYY");
    if (l_pos < Result.size ())
      {
        l_writer << boost::gregorian::day_clock::local_day().year ();
        Result.replace (l_pos, 4
    		      , l_writer.str ());
      }
    //  Search for AA
    l_pos = Result.find ("AA");
    if (l_pos < Result.size ())
      {
        Result.replace (l_pos, 2, get_experimenter_initials ());
      }
    //  Search for #
    l_pos = Result.find ('#');
    if (l_pos < Result.size ())
      {
        unsigned int l_last = Result.find_first_not_of ('#', l_pos);// find last
        l_writer << Result.substr (0, l_pos);
        l_writer << std::setw(l_last - l_pos) << std::setfill('0') << l_seq;
        l_writer << Result.substr (l_last + 1, Result.size ());
        Result = l_writer.str ();
      }
  }
  // POST
  is_valid ();
  // END
  return Result;
}

//Get the parent element of the entry list for the 'page' at a_cursor
//
//\post not Result.is_null ()

/**
 * Get the parent element of the entry list for the 'page' at a_cursor
 * 
 * \post not Result.is_null ()
 */
purpletree::dom::element page_ctl::get_entry_parent_node(item_iterator & a_cursor)
{
  // PRE
  require_valid_iterator (a_cursor);
  // DO
  purpletree::dom::element Result (xml_traits::find_node_by_name ((*a_cursor).first_child ()
  								, LOG_NODE_NAME));
  ENSURE (not Result.is_null ()
  	, "Invalid 'page' element without an entry list parent node.");
  return Result;
}

//Get the final date associated with the current 'page'.  This requires the 'page' is finished, which is set using 'finalise()' and tested by 'is_done'.
//
//\pre is_valid_iterator (a_cursor), test is a_cursor is a page iterator.
//\pre is_done (a_cursor)
//\post not Result.is_not_a_date

/**
 * Get the final date associated with the current 'page'.  This requires the 'page' is finished, which is set using 'finalise()' and tested by 'is_done'.
 * 
 * \pre is_valid_iterator (a_cursor), test is a_cursor is a page iterator.
 * \pre is_done (a_cursor)
 * \post not Result.is_not_a_date
 */
boost::gregorian::date page_ctl::get_final_date(const item_iterator & a_cursor)
{
  REQUIRE (is_done (a_cursor), "Cannot get final date from unfinalised page");
  boost::gregorian::date Result (xml_traits::get_date_attribute (*a_cursor, FINAL_DATE_ATTR_NAME));
  ENSURE (not Result.is_not_a_date ()
  	, "Final date must be a valid date.");
  return Result;
}

//Get the parent element of the entry list for the 'page' at a_cursor
//
//\post not Result.is_null ()

/**
 * Get the parent element of the entry list for the 'page' at a_cursor
 * 
 * \post not Result.is_null ()
 */
purpletree::dom::element page_ctl::get_information_parent_node(item_iterator & a_cursor)
{
  // PRE
  require_valid_iterator (a_cursor);
  // DO
  purpletree::dom::element Result (xml_traits::find_node_by_name ((*a_cursor).first_child ()
  								, REFERENCE_NODE_NAME));
  ENSURE (not Result.is_null ()
  	, "Invalid 'page' element without an information list parent node.");
  return Result;
}

//Get the start date associated with the current 'page'.
//
//\pre is_valid_iterator (a_cursor), test is a_cursor is a page iterator.
//\pre is_done (a_cursor)
//\post not Result.is_not_a_date

/**
 * Get the start date associated with the current 'page'.
 * 
 * \pre is_valid_iterator (a_cursor), test is a_cursor is a page iterator.
 * \pre is_done (a_cursor)
 * \post not Result.is_not_a_date
 */
boost::gregorian::date page_ctl::get_start_date(const item_iterator & a_cursor)
{
  require_valid_iterator (a_cursor);
  boost::gregorian::date Result (xml_traits::get_date_attribute (*a_cursor, START_DATE_ATTR_NAME));
  ENSURE (not Result.is_not_a_date (), "A page requires a valid start date.");
  return Result;
}

//Get the title for the 'page'.
//
//\post not Result.empty()

/**
 * Get the title for the 'page'.
 * 
 * \post not Result.empty()
 */
std::string page_ctl::get_title(const item_iterator & a_cursor)
{
  require_valid_iterator (a_cursor);
  purpletree::dom::node l_item (xml_traits::find_node_by_name ((*a_cursor).first_child ()
  							     , TITLE_NODE_NAME));
  REQUIRE (not l_item.is_null (), "A 'page' must have a 'title'.");
  std::string Result (xml_traits::get_node_content (l_item));
  ENSURE (not Result.empty (), "A 'page' must have a non-empty 'title'.");
  return Result;
}

//Test if this entry is from a "done" page

/**
 * Test if this entry is from a "done" page
 */
bool page_ctl::is_done(const item_ctl * a_control)
{
  // PRE
  REQUIRE (NULL != a_control, "Control must not be null");
  purpletree::dom::node l_node (a_control->get_parent_node ().parent_node ());
  REQUIRE (l_node.node_name () == PAGE_NODE_NAME
  	 , "List control should be for a sub-list of a \"page\" list.");
  // DO
  return not l_node.attributes ().named_item (FINAL_DATE_ATTR_NAME).is_null ();
}

//Class local test of invariants.

/**
 * Class local test of invariants.
 */
void page_ctl::is_valid() const {
  base_t::is_valid ();
  INVARIANT (xml_traits::is_element(get_parent_node (), PARENT_NODE_NAME)
             , "List control must have a parent element of right type.");
}

//Check the iterator is valid for 'page' item
//
//\post (*a_cursor).node_name = get_node_name
//\post (*a_cursor).has_attribute(START_DATE_ATTR_NAME)
//\post [not tested] not get_title(a_cursor).empty
//\post [not tested] not find_node_by_name(a_cursor, LOG_NODE_NAME).is_null
//\post [not tested] not find_node_by_name(a_cursor, REFERENCE_NODE_NAME).is_null

/**
 * Check the iterator is valid for 'page' item
 * 
 * \post (*a_cursor).node_name = get_node_name
 * \post (*a_cursor).has_attribute(START_DATE_ATTR_NAME)
 * \post [not tested] not get_title(a_cursor).empty
 * \post [not tested] not find_node_by_name(a_cursor, LOG_NODE_NAME).is_null
 * \post [not tested] not find_node_by_name(a_cursor, REFERENCE_NODE_NAME).is_null
 */
bool page_ctl::is_valid_iterator(const item_iterator & a_cursor)
{
  return xml_traits::is_element (*a_cursor, PAGE_NODE_NAME)
	and not (*a_cursor).get_attribute (START_DATE_ATTR_NAME).empty ();
}

/**
 * Not defined, added to stop copying.
 */
page_ctl & page_ctl::operator =(const page_ctl & source) {
}

//General constructor.
//
//\pre not a_parent_node.is_null ()

/**
 * General constructor.
 * 
 * \pre not a_parent_node.is_null ()
 */
page_ctl::page_ctl(const purpletree::dom::element & a_parent_node)
: item_ctl (a_parent_node)
{
  is_valid ();
}

/**
 * Not defined, added to stop copying.
 */
page_ctl::page_ctl(const page_ctl & source){
}

//Set the current reference for the 'page' item at a_cursor. 
//
//\pre is_valid_iterator (a_cursor), test if a_cursor is a page iterator.
//\pre not a_value.empty: can not set page reference to an empty string
//\post a_value = get_pageref (a_cursor)

/**
 * Set the current reference for the 'page' item at a_cursor. 
 * 
 * \pre is_valid_iterator (a_cursor), test if a_cursor is a page iterator.
 * \pre not a_value.empty: can not set page reference to an empty string
 * \post a_value = get_pageref (a_cursor)
 */
void page_ctl::set_pageref(item_iterator & a_cursor, const std::string & a_value)
{
  // PRE
  require_valid_iterator (a_cursor);
  REQUIRE (not a_value.empty (), "Can not set a page reference to an empty string.");
  // DO
  if (xml_traits::set_attribute (*a_cursor
  			       , PAGEREF_ATTR_NAME
  			       , purpletree::dom::set_utf8 (a_value)))
  {
    journal_ctl::make_dirty ((*a_cursor).owner_document ());
  }
  ENSURE (a_value == get_pageref (a_cursor)
  	, "Set value does not equal value input to set_pageref.");
}

//Set the 'title' of the 'page' item at a_cursor.
//
//\pre is_valid_iterator (a_cursor)
//\pre not a_value.empty: can not set title to an empty string
//\post a_value == get_title (a_cursor)

/**
 * Set the 'title' of the 'page' item at a_cursor.
 * 
 * \pre is_valid_iterator (a_cursor)
 * \pre not a_value.empty: can not set title to an empty string
 * \post a_value == get_title (a_cursor)
 */
void page_ctl::set_title(item_iterator & a_cursor, const std::string & a_value)
{
  // PRE
  require_valid_iterator (a_cursor);
  REQUIRE (not a_value.empty (), "Can not set title to an empty string");
  // DO
  purpletree::dom::element l_node (xml_traits::find_node_by_name ((*a_cursor).first_child (), TITLE_NODE_NAME));
  purpletree::dom::document l_doc ((*a_cursor).owner_document ());
  // if (l_node.is_null ())
  // {
  //   // There is no existing node, we need to add one.
  //   // A title node is at the start of the entry node.
  //   if ((*a_cursor).has_child_nodes ())
  //     {
  //       purpletree::dom::node l_first((*a_cursor).first_child ());
  //       // Has other child nodes, prepend to first
  //       l_node = (*a_cursor).insert_before (l_doc.create_element (TITLE_NODE_NAME), l_first);
  //     }
  //   else
  //     {
  //       // No other child nodes, simply append
  //       l_node = (*a_cursor).append_child (l_doc.create_element (TITLE_NODE_NAME));
  //     }
  // }
  CHECK (not l_node.is_null (), "Invalid 'page' element with no 'title' element.");
  if (xml_traits::set_node_content (l_node, a_value))
  {
    journal_ctl::make_dirty (l_doc);
  }
  // POST
  ENSURE (a_value == get_title (a_cursor)
  	, "Set title does not equal value input to set_title.");
}

/**
 * [** Currently unimplemented **]
 * If an experiment has been finalised, this can be undone if the experiment has not been saved.
 * 
 * \pre true = is_done ()
 * \pre true = is_dirty ()
 * \post false = is_done ()
 */
void page_ctl::unfinalise() {
}

const purpletree::dom::dom_string page_ctl::FINAL_DATE_ATTR_NAME(purpletree::dom::set_utf8 ("final-date"));

const purpletree::dom::dom_string page_ctl::LOG_NODE_NAME(purpletree::dom::set_utf8 ("log"));

const purpletree::dom::dom_string page_ctl::PAGE_NODE_NAME(purpletree::dom::set_utf8 ("page"));

const purpletree::dom::dom_string page_ctl::PAGEREF_ATTR_NAME(purpletree::dom::set_utf8 ("pageref"));

const purpletree::dom::dom_string & page_ctl::PARENT_NODE_NAME(journal_ctl::JOURNAL_DOC_NODE_NAME);

const purpletree::dom::dom_string page_ctl::REFERENCE_NODE_NAME(purpletree::dom::set_utf8 ("reference"));

const purpletree::dom::dom_string page_ctl::START_DATE_ATTR_NAME(purpletree::dom::set_utf8 ("start-date"));

const purpletree::dom::dom_string page_ctl::TITLE_NODE_NAME(purpletree::dom::set_utf8 ("title"));

std::string page_ctl::experimenter_initials("JJF");

std::string page_ctl::pageref_format("OLYYYY_AA_##");

//Set the configured experiment initials.

/**
 * Set the configured experiment initials.
 */
void page_ctl::set_experimenter_initials(const std::string value)
{
  experimenter_initials = value;
}

//Set the configured page reference format string.

/**
 * Set the configured page reference format string.
 */
void page_ctl::set_pageref_format(const std::string value)
{
  pageref_format = value;
}


} // namespace expt
