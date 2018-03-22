#ifndef PAGE_CTL_HPP
#define PAGE_CTL_HPP

// ------------------------------------------------------------
// class: page_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/item_ctl.hpp"
#include "programs/expt/item_iterator.hpp"
#include <string>

namespace expt {

/**
 * Control the page of the lab journal of an experimenter. The controlled model has identifying information such as starting date, title; categorising information such as expt template; cross reference information such as pageref, code name.
 * 
 * Controlled Model concepts:
 * * title
 * * pageref
 * * starting date (use date of first page_entry)
 * * code name, expt template id (use add_information/get_formation)
 * 
 * <page start-date="..." (end-date="...")? pageref="..."><title/><reference <??>*><log <entry>*></>
 */
class page_ctl : public item_ctl {
  public:
    typedef item_ctl base_t;

    /**
     * Create a new 'page' item, add it to the parent node and return an iterator to it.
     * 
     * \post Result != end () and not (*Result).is_null () and (*Result).node_name == get_node_name
     */
    item_iterator create_item();

    /**
     * Indicate the experiment on this page is complete, (sets the final_date.)
     * 
     * \post true = is_done()
     */
    static void finalise(item_iterator & a_cursor);

    /**
     * Get an iterator for a page that has the given pageref.  If pageref doesn't exist then returned iterator == end()
     * 
     * \post Result == end () or get_pageref (Result) = a_pageref
     */
    item_iterator find(const std::string & a_pageref) const;

    /**
     * Generate a new pageref based on the pageref_format string.  See documentation of pageref_format for how this method generates the new pageref.
     * 
     * \post not Result.empty ()
     */
    std::string generate_pageref();

    /**
     * Get the parent element of the entry list for the 'page' at a_cursor
     * 
     * \post not Result.is_null ()
     */
    static purpletree::dom::element get_entry_parent_node(item_iterator & a_cursor);

    /**
     * Get the final date associated with the current 'page'.  This requires the 'page' is finished, which is set using 'finalise()' and tested by 'is_done'.
     * 
     * \pre is_valid_iterator (a_cursor), test is a_cursor is a page iterator.
     * \pre is_done (a_cursor)
     * \post not Result.is_not_a_date
     */
    static boost::gregorian::date get_final_date(const item_iterator & a_cursor);

    /**
     * Get the parent element of the entry list for the 'page' at a_cursor
     * 
     * \post not Result.is_null ()
     */
    static purpletree::dom::element get_information_parent_node(item_iterator & a_cursor);

    /**
     * Return the DOM node name for the 'page' list elements.
     * 
     * \post not Result.empty
     */
    inline const purpletree::dom::dom_string & get_node_name() const {
          ENSURE (not PAGE_NODE_NAME.empty (), "Node name cannot be an empty string.");
          return PAGE_NODE_NAME;
        };

    /**
     * Get the page reference for the 'page' pointed to by a_cursor. 
     * 
     * \pre is_valid_iterator (a_cursor), test if a_cursor is a page iterator.
     * \post not Result.empty ()
     */
    static inline std::string get_pageref(const item_iterator & a_cursor) {
          require_valid_iterator (a_cursor);
          std::string Result (purpletree::dom::get_utf8 ((*a_cursor).get_attribute (PAGEREF_ATTR_NAME)));
          ENSURE (not Result.empty (), "A valid entry must have a non-empty page reference.");
          return Result;
        };

    /**
     * Get the start date associated with the current 'page'.
     * 
     * \pre is_valid_iterator (a_cursor), test is a_cursor is a page iterator.
     * \pre is_done (a_cursor)
     * \post not Result.is_not_a_date
     */
    static boost::gregorian::date get_start_date(const item_iterator & a_cursor);

    /**
     * Get the title for the 'page'.
     * 
     * \post not Result.empty()
     */
    static std::string get_title(const item_iterator & a_cursor);

    /**
     * Is there a 'page' item in the list with a page reference equal to a_pageref?
     * 
     * \post Result = (find(a_pageref) /= end())
     */
    inline bool has_pageref(const std::string & a_pageref) const {
          is_valid ();
          return end() != find (a_pageref);
        };

    /**
     * Is this 'page' finished, tested by seeing if the 'page' has a final date?
     */
    static inline bool is_done(const item_iterator & a_cursor) {
          require_valid_iterator (a_cursor);
          return not (*a_cursor).get_attribute (FINAL_DATE_ATTR_NAME).empty ();
        };

    /**
     * Test if this entry is from a "done" page
     */
    static bool is_done(const item_ctl * a_control);


  protected:
    /**
     * Class local test of invariants.
     */
    void is_valid() const;


  public:
    /**
     * Check the iterator is valid for 'page' item
     * 
     * \post (*a_cursor).node_name = get_node_name
     * \post (*a_cursor).has_attribute(START_DATE_ATTR_NAME)
     * \post [not tested] not get_title(a_cursor).empty
     * \post [not tested] not find_node_by_name(a_cursor, LOG_NODE_NAME).is_null
     * \post [not tested] not find_node_by_name(a_cursor, REFERENCE_NODE_NAME).is_null
     */
    static bool is_valid_iterator(const item_iterator & a_cursor);


  private:
    /**
     * Check the iterator has a valid 'page' item
     */
    static inline void require_valid_iterator(const item_iterator & a_cursor) {
          REQUIRE (is_valid_iterator (a_cursor), "Require iterator to 'page' type node");
        };

    /**
     * Not defined, added to stop copying.
     */
    page_ctl & operator =(const page_ctl & source);


  public:
    /**
     * General constructor.
     * 
     * \pre not a_parent_node.is_null ()
     */
    page_ctl(const purpletree::dom::element & a_parent_node);


  private:
    /**
     * Not defined, added to stop copying.
     */
    page_ctl(const page_ctl & source);


  public:
    inline ~page_ctl() {};

    /**
     * Set the current reference for the 'page' item at a_cursor. 
     * 
     * \pre is_valid_iterator (a_cursor), test if a_cursor is a page iterator.
     * \pre not a_value.empty: can not set page reference to an empty string
     * \post a_value = get_pageref (a_cursor)
     */
    static void set_pageref(item_iterator & a_cursor, const std::string & a_value);

    /**
     * Set the 'title' of the 'page' item at a_cursor.
     * 
     * \pre is_valid_iterator (a_cursor)
     * \pre not a_value.empty: can not set title to an empty string
     * \post a_value == get_title (a_cursor)
     */
    static void set_title(item_iterator & a_cursor, const std::string & a_value);


  protected:
    /**
     * [** Currently unimplemented **]
     * If an experiment has been finalised, this can be undone if the experiment has not been saved.
     * 
     * \pre true = is_done ()
     * \pre true = is_dirty ()
     * \post false = is_done ()
     */
    void unfinalise();


  public:
    static const purpletree::dom::dom_string FINAL_DATE_ATTR_NAME;

    static const purpletree::dom::dom_string LOG_NODE_NAME;

    static const purpletree::dom::dom_string PAGE_NODE_NAME;

    static const purpletree::dom::dom_string PAGEREF_ATTR_NAME;

    static const purpletree::dom::dom_string & PARENT_NODE_NAME;

    static const purpletree::dom::dom_string REFERENCE_NODE_NAME;

    static const purpletree::dom::dom_string START_DATE_ATTR_NAME;

    static const purpletree::dom::dom_string TITLE_NODE_NAME;


  private:
    static std::string experimenter_initials;

    static std::string pageref_format;

    /**
     * Get the configured experimenter intials
     */
    static inline const std::string get_experimenter_initials();

    /**
     * Get the configured page reference format string.
     */
    static inline const std::string get_pageref_format();

    /**
     * Set the configured experiment initials.
     */
    static void set_experimenter_initials(const std::string value);

    /**
     * Set the configured page reference format string.
     */
    static void set_pageref_format(const std::string value);

};
//Get the configured experimenter intials

/**
 * Get the configured experimenter intials
 */
inline const std::string page_ctl::get_experimenter_initials()
{
  return experimenter_initials;
}

//Get the configured page reference format string.

/**
 * Get the configured page reference format string.
 */
inline const std::string page_ctl::get_pageref_format()
{
  return pageref_format;
}


} // namespace expt
#endif
