#ifndef ENTRY_VW_HPP
#define ENTRY_VW_HPP

// ------------------------------------------------------------
// class: entry_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/itemlist_vw.hpp"
#include <memory>
#include "programs/expt/base_view.hpp"

namespace expt {

/**
 * An abstract base class for objects that provide a view on an entry object.
 */
class entry_vw : public itemlist_vw {
  public:
    typedef itemlist_vw base_t;

    class columns : public itemlist_vw::base_columns {
      public:
        typedef itemlist_vw::base_columns base_column_t;

        Gtk::TreeModelColumn<Glib::ustring> m_entrydate;

        inline columns() : base_column_t ()
                , m_entrydate ()
                {
                  add (m_entrydate);
                };

        inline ~columns() {};

        /**
         * Append columns to the tree view.
         */
        virtual void set_columns(itemlist_vw::list_window_t a_tree_view);

    };
    

  private:
    Gtk::Entry * date_view;

    Gtk::Entry * sequence_view;

    Gtk::TextView * observation_view;

    Gtk::TextView * action_view;


  protected:
    /**
     * Create a new list control.
     */
    virtual std::auto_ptr< control::item_ctl > create_list_control(const purpletree::dom::element & a_node) const;


  public:
    /**
     * PART TODO
     * 
     * \pre a_entry != NULL
     */
    entry_vw(const base_view & a_journal_view);


  private:
    /**
     * Not defined, added to stop copying.
     */
    entry_vw(const entry_vw & source);


  public:
    inline virtual ~entry_vw() {};

    /**
     * Are there edits to UI content that has not been passed to a control object?
     * 
     * \post Result implies 0 /= get_change_flag ()
     */
    virtual bool is_changed() const;


  private:
    /**
     * Test invariant conditions.
     */
    inline void is_valid() const {
          INVARIANT (NULL != date_view, "Wrong type or non-existent window for date view.");
          INVARIANT (NULL != sequence_view, "Wrong type or non-existent window for sequence view.");
          INVARIANT (NULL != observation_view, "Wrong type or non-existent window for observation view.");
          INVARIANT (NULL != action_view, "Wrong type or non-existent window for action view.");
        };

    /**
     * Not defined, added to stop copying.
     */
    entry_vw & operator =(const entry_vw & source);


  protected:
    /**
     * Set the values in a_row from the data pointed to by a_cursor.
     */
    void set_row(const itemlist_vw::row_iterator_t & a_row, const control::item_iterator & a_cursor);

    /**
     * Poll the view for the changed data (Called from poll)
     */
    virtual void virt_poll();

    /**
     * Update the display to view the current data (Creating a display if necessary)
     */
    void virt_update();


  private:
    static const base_view::string_t ACTION_VIEW_KEY;

    static const base_view::string_t DATE_VIEW_KEY;

    static const base_view::string_t ENTRY_VIEW_KEY;

    static const base_view::string_t ENTRYLIST_VIEW_KEY;

    static const base_view::string_t OBSERVATION_VIEW_KEY;

    static const base_view::string_t SEQUENCE_VIEW_KEY;

};

} // namespace expt
#endif
