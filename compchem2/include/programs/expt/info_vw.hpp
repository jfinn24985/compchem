#ifndef INFO_VW_HPP
#define INFO_VW_HPP

// ------------------------------------------------------------
// class: info_vw
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
 * Manage the display of the information items of a journal page.
 */
class info_vw : public itemlist_vw {
  public:
    typedef itemlist_vw base_t;

    class columns : public itemlist_vw::base_columns {
      public:
        typedef itemlist_vw::base_columns base_column_t;

        Gtk::TreeModelColumn<Glib::ustring> m_name;

        Gtk::TreeModelColumn<Glib::ustring> m_value;

        inline columns() : base_column_t ()
                , m_name ()
                , m_value ()
                {
                  add (m_name);
                  add (m_value);
                };

        inline ~columns() {};

        /**
         * Append columns to the tree view.
         */
        virtual void set_columns(itemlist_vw::list_window_t a_tree_view);

    };
    

  private:
    Gtk::Entry * name_view;

    Gtk::Entry * value_view;


  protected:
    /**
     * Create a new list control.
     */
    virtual std::auto_ptr< control::item_ctl > create_list_control(const purpletree::dom::element & a_node) const;


  public:
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
          INVARIANT (NULL != name_view, "Viewer must have a valid name entry object.");
          INVARIANT (NULL != value_view, "Viewer must have a valid value entry object.");
        };


  public:
    /**
     * Main constructor.
     */
    info_vw(const base_view & a_view);

    inline virtual ~info_vw() {};


  private:
    /**
     * Not defined, added to stop copying.
     */
    info_vw(const info_vw & source);

    /**
     * Not defined, added to stop copying.
     */
    info_vw & operator =(const info_vw & source);


  protected:
    /**
     * Set the values in a_row from the data pointed to by a_cursor.
     */
    void set_row(const itemlist_vw::row_iterator_t & a_row, const control::item_iterator & a_cursor);

    /**
     * Populate the newly created item in a_cursor with queried data.  The base function does nothing, derived types that need to perform extra operations to ensure a valid data item should do so here.
     */
    virtual void virt_create_item(control::item_iterator & a_cursor);

    /**
     * Poll the view for the changed data (Called from poll)
     */
    virtual void virt_poll();

    /**
     * Update the display to view the current data (Creating a display if necessary)
     * 
     * \pre false = is_changed ().
     */
    virtual void virt_update();


  private:
    static const base_view::string_t INFOLIST_VIEW_KEY;

    static const base_view::string_t INFO_VIEW_KEY;

    static const base_view::string_t NAME_VIEW_KEY;

    static const base_view::string_t VALUE_VIEW_KEY;

};

} // namespace expt
#endif
