#ifndef ITEM_ITERATOR_HPP
#define ITEM_ITERATOR_HPP

// ------------------------------------------------------------
// class: item_iterator
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/item_ctl.hpp"

namespace expt {

/**
 * A type for iterating over a list of items.
 */
class item_iterator {
  private:
    const item_ctl * parent;

    purpletree::dom::element cursor;


  public:
    /**
     * Get the owning list control object.
     */
    inline const item_ctl * const get_parent() const {
          return parent;
        };

    /**
     * Test if the element the iterator points to is_null.  Note that for all lists: end().is_null ()
     */
    inline bool is_null() const {
          return NULL == parent or cursor.is_null ();
        };

    /**
     * Empty constructor for creating iterators that will be assigned a new value (This should be avoided.)
     * 
     * \post is_null()
     */
    inline explicit item_iterator() : parent (NULL)
        , cursor ()
        {
          ENSURE (is_null (), "No-argument constructor should produce a null node.");
        };

    /**
     * Main constructor used to create a useful iterator.
     * 
     * \pre not a_start.is_null()
     * \post not is_null ()
     */
    inline item_iterator(const item_ctl & a_parent, const purpletree::dom::element & a_start) : parent (&a_parent)
        , cursor (a_start) 
        {
          REQUIRE (not a_start.is_null (), "Require a non-null element to create a valid iterator.");
          ENSURE (not is_null (), "Main constructor should create a non-null iterator.");
        };

    /**
     * Constructor for creating list end() iterators.
     * 
     * \post is_null ()
     */
    inline item_iterator(const item_ctl & a_parent) : parent (&a_parent)
        , cursor ()
        {
          ENSURE (is_null (), "One argument constructor should create a null iterator.");
        };

    inline ~item_iterator() {};

    /**
     * Copy constructor.
     * 
     * \post *this == source
     */
    inline item_iterator(const item_iterator & source) : parent (source.parent)
        , cursor (source.cursor)
        {
          ENSURE (source == *this, "Copied operator should equal original.");
        };

    /**
     * Return the DOM element pointed to by this iterator.  Result may be null.
     */
    inline purpletree::dom::element & operator *() { return cursor; };

    /**
     * Return the DOM element pointed to by this iterator.  Result may be null. (const version)
     */
    inline const purpletree::dom::element & operator *() const { return cursor; };

    /**
     * Increment the iterator.
     */
    const item_iterator & operator ++();

    /**
     * Convert to boolean.
     * 
     * \post Result == not is_null()
     */
    inline operator bool() const {
          return not is_null ();
        };

    /**
     * Assignment operator.
     * 
     * \post *this == source
     */
    inline item_iterator & operator =(const item_iterator & source) {
          parent = source.parent;
          cursor = source.cursor;
          ENSURE (source == *this, "Copied operator should equal original.");
          return *this;
        };

    /**
     * Compare two iterators for equality.
     * \post true implies both have the same parent and either both are null or have the same cursor element.
     */
    inline bool is_equal(const item_iterator & a_other) const { return (parent == a_other.parent) and ((cursor.is_null () and a_other.cursor.is_null ()) or cursor.is_equal (a_other.cursor)); };

    friend inline bool operator ==(const item_iterator & a_lhs, const item_iterator & a_rhs) { return a_lhs.is_equal (a_rhs); };

    friend inline bool operator !=(const item_iterator & a_lhs, const item_iterator & a_rhs) { return not a_lhs.is_equal (a_rhs); };

};

} // namespace expt
#endif
