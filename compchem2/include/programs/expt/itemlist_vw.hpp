#ifndef ITEMLIST_VW_HPP
#define ITEMLIST_VW_HPP

// ------------------------------------------------------------
// class: itemlist_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/base_view.hpp"
#include <memory>

namespace expt {

/**
 * This view class manages the display of a list of items.  It reports the selection event back to its control object.
 */
class itemlist_vw : public base_view {
  public:
    class base_columns : public row_column_t {
      public:
        Gtk::TreeModelColumn<control::item_iterator> m_iterator;

        inline base_columns() : Gtk::TreeModel::ColumnRecord ()
                , m_iterator ()
                {
                  add (m_iterator);
                };

        inline virtual ~base_columns() {};

        /**
         * Append columns to the tree view.
         */
        virtual void set_columns(list_window_t a_tree_view) = 0;

    };
    
    typedef Gtk::TreeView*  list_window_t;

    typedef Gtk::TreeModel::iterator row_iterator_t;

    typedef Gtk::TreeModel::ColumnRecord row_column_t;

    typedef Gtk::Button*  button_t;

    typedef base_view base_t;


  protected:
    std::auto_ptr< control::item_ctl > list_control;

    Glib::RefPtr<Gtk::ListStore> m_model;

    std::auto_ptr<base_columns> m_columns;

  private:
    control::item_iterator cursor;


  protected:
    list_window_t m_list;

    button_t m_button;


  public:
    /**
     * Save any unsaved data and remove all data from the display and desensitize the widgets.  Then discard the control object.
     * 
     * \pre is_state_two () or is_state_three ()
     * \post is_state_one ()
     */
    void clear_parent_node();


  private:
    /**
     * [UI Callback] Create a new item and add it to the list.  Set the current item view to this item.
     */
    void create_new_item();


  protected:
    /**
     * Create a new list control.  Derived classes should return the list control type specific to their type.
     * 
     * \post Result /= null
     */
    virtual std::auto_ptr< control::item_ctl > create_list_control(const purpletree::dom::element & a_node) const = 0;

    /**
     * Find the corresponding item iterator for a_row_cursor.
     * 
     * \pre is_state_three or is_state_four
     * \post Result.get_parent = list_control
     * \post Result /= list_control.end ()
     */
    inline control::item_iterator find_row(const row_iterator_t & a_row_cursor) {
          REQUIRE (is_state_four () or is_state_three ()
                   , "Can only get a row in state three or four.");
          control::item_iterator Result ((*a_row_cursor)[m_columns->m_iterator]);
          ENSURE (Result.get_parent () == list_control.get ()
        	  , "Iterator is not from the current model.");
          ENSURE (Result != list_control->end ()
                  , "Iterator from a row should never equal end()");
          return Result;
        };

    /**
     * Find the corresponding row iterator for a_cursor.
     * 
     * \pre is_state_three or is_state_four
     * \pre a_cursor.get_parent = list_control
     * \post Result is a valid row
     */
    row_iterator_t find_row(const control::item_iterator & a_cursor);


  public:
    /**
     * Get a control iterator to the currently selected item (or list_control.end ())
     * 
     * \pre not is_state_one
     */
    inline const control::item_iterator & get_cursor() const {
          is_valid ();
          REQUIRE (not is_state_one (), "Cannot query an inactive list.");
          return cursor;
        };

    /**
     * Get a control iterator to the currently selected item (or list_control.end ())
     * 
     * \pre not is_state_one
     */
    inline control::item_iterator & get_cursor() {
          is_valid ();
          REQUIRE (not is_state_one (), "Cannot query an inactive list.");
          return cursor;
        };

    /**
     * Is the list in state one.  In this state the parent node is not set and view is insensitive
     */
    inline bool is_state_one() const {
          return NULL == list_control.get ();
        };

    /**
     * Is the list in state two.  In this state the parent node is set but is empty so the view is insensitive except the "add item" button.
     */
    inline bool is_state_two() const {
          return NULL != list_control.get () and m_model->children ().empty ();
        };

    /**
     * Is the list in state two.  In this state the parent node is set and has content, but there are no selected items so the view is insensitive.
     */
    inline bool is_state_three() const {
          return NULL != list_control.get () and not m_model->children ().empty () and cursor == list_control->end ();
        };

    /**
     * Is the list in state two.  In this state the parent node is set and has content, one of which is selected so the view is sensitive.
     */
    inline bool is_state_four() const {
          return NULL != list_control.get () and not m_model->children ().empty () and cursor != list_control->end ();
        };


  protected:
    /**
     * Test invariant conditions.
     * 
     * \inv m_columns /= nul, m_model /= nul, m_list /= nul, m_button /= nul
     * \inv not is_state_one --> list_control /= nul
     *                    , cursor /= nul or cursor == list_control.end
     *                    , m_list->model = m_model
     * \inv is_state_one --> list_control = nul, cursor = nul
     */
    void is_valid() const;


  public:
    /**
     * Create an object to manage a view with a list.
     */
    itemlist_vw(const base_view::string_t & a_key, const base_view::string_t & a_list_key, const base_view & a_window, base_columns * a_column);


  private:
    /**
     * Not defined, added to stop copying.
     */
    itemlist_vw(const itemlist_vw & source);


  public:
    inline virtual ~itemlist_vw() {};

    /**
     * Poll the view for the changed data.  This transfer data from the UI objects to the control object.
     * 
     * (The virtual part of the operation is in virt_poll, which should be overridden in derived classes.)
     * \post not is_changed ().
     */
    inline void poll() {
          is_valid ();
          virt_poll ();
          ENSURE (false == is_changed (), "After poll a view should have no changes.");
        };


  private:
    /**
     * Not defined, added to stop copying.
     */
    itemlist_vw & operator =(const itemlist_vw & source);

    /**
     * [UI Callback] This method is called (from the UI objects) to indicate that the selection in the UI object has been changed.
     */
    void select_item();


  public:
    /**
     * Set a_node as the data in the display and make the displayed objects sensitive.
     * 
     * \pre is_state_one()
     * \pre not a_node.is_null ()
     * \post is_state_two () or is_state_three ()
     * \post not is_changed ()
     */
    void set_parent_node(const purpletree::dom::element & a_node);


  protected:
    /**
     * Set the values in a_row from the data pointed to by a_cursor.  Derived classes define this method to insert data into the UI model object based on their column  definition.
     */
    virtual void set_row(const row_iterator_t & a_row, const control::item_iterator & a_cursor) = 0;


  public:
    /**
     * Update the display to view the current data. This transfers data from the control object into UI objects.
     * 
     * (The virtual part of the operation is in virt_update, which should be overridden in derived classes.)
     * \post not is_changed ().
     */
    virtual void update();


  protected:
    /**
     * Populate the newly created item in a_cursor with queried data.  The base function does nothing, derived types that need to perform extra operations to ensure a valid data item should do so here.
     */
    inline virtual void virt_create_item(control::item_iterator & a_cursor) {};

    /**
     * Poll the view for the changed data (Called from poll).  This does nothing with an itemlist view as the list is not editable.
     */
    virtual void virt_poll() = 0;

    /**
     * Update the display to view the current data (Creating a display if necessary).  This does not change the selection.
     * 
     * \pre false = is_changed ().
     * \post false = is_changed ().
     */
    virtual void virt_update() = 0;

};

} // namespace expt
#endif
