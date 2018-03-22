#ifndef ITEM_CTL_HPP
#define ITEM_CTL_HPP

// ------------------------------------------------------------
// class: item_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/item_iterator.hpp"

namespace expt {

/**
 * In several situations you have a list of DOM element objects that you want to be able scan and to manipulate individually.  This type has an associated list view to display the objects as a list and maintains a cursor to the currently selected item. Derived classes can use this cursor to display an item in detail.
 * 
 * The list is publicly available through an STL like iterator interface.
 * 
 * The type is abstract and requires derived classes to provide the following:
 * * create_item - A method to create a new DOM element.
 * * get_node_name - Return a DOM string of the name of the DOM element.
 * * on_change - Signal method called when the cursor is changed.
 * * The UI object to be managed by the item list view.
 */
class item_ctl {
  private:
    purpletree::dom::element parent_node;

    item_iterator m_end;


  public:
    /**
     * Get an interator to the first item in the list.
     * 
     * \post Result == end() or (*Result).node_name == get_node_name
     */
    item_iterator begin() const;

    /**
     * Create a new DOM node, add it to the parent element and return an iterator to it.  Derived classes must create nodes with node_name () == get_node_name () and ensure that the created element is a valid twiglet in the DTD/Schema
     * 
     * \post Result != end () and not (*Result).is_null () and (*Result).node_name == get_node_name
     */
    virtual item_iterator create_item() = 0;

    /**
     * Are there any items?
     */
    inline bool empty() const {
          is_valid ();
          return begin () == end ();
        };

    /**
     * Get an iterator to one-past-end of list of items.
     */
    inline const item_iterator & end() const {
          is_valid ();
          return m_end;
        };

    /**
     * The DOM node name for the represented elements.
     * 
     * \post not Result.empty ()
     */
    virtual const purpletree::dom::dom_string & get_node_name() const = 0;

    /**
     * The DOM element that is parent to all the items.
     * 
     * \post not Result.is_null()
     */
    inline const purpletree::dom::element & get_parent_node() const {
          is_valid ();
          return parent_node;
        };

    /**
     * The DOM element that is parent to all the items.
     * 
     * \post not Result.is_null()
     */
    inline purpletree::dom::element & get_parent_node() {
      is_valid ();
      return parent_node;
    };


  protected:
    /**
     * Test type invariants
     */
    inline void is_valid() const {
          INVARIANT (not parent_node.is_null (), "List control must have a non-null parent element.");
        };


  public:
    /**
     * Construct a new object that manages a set of children of a_parent.
     */
    inline item_ctl(const purpletree::dom::element & a_parent) : parent_node (a_parent)
        , m_end (*this)
        {
          REQUIRE (not a_parent.is_null (), "Cannot create list control with null DOM element.");
          REQUIRE (not a_parent.owner_document ().is_null (), "Cannot create list control with null DOM document.");
          is_valid ();
        };


  private:
    /**
     * Not defined, added to stop copying.
     */
    item_ctl(const item_ctl & source);


  public:
    /**
     * Destructor, this class has no special actions here.
     */
    inline virtual ~item_ctl() {};


  private:
    /**
     * Not defined, added to stop copying.
     */
    item_ctl & operator =(const item_ctl & source);


  public:
    /**
     * The number of items in the list.  This iterates over the list so has time O(N).
     */
    inline unsigned int size() const {
          // LOCAL
          unsigned int Result(0);
          is_valid ();
          // DO
          for (item_iterator cursor(begin()); cursor != end(); ++cursor, ++Result) {} // Do nothing in loop
          return Result;
        };

};

} // namespace expt
#endif
