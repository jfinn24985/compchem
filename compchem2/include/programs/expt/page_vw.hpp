#ifndef PAGE_VW_HPP
#define PAGE_VW_HPP

// ------------------------------------------------------------
// class: page_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/itemlist_vw.hpp"
#include "programs/expt/entry_vw.hpp"
#include "programs/expt/info_vw.hpp"
#include <memory>
#include "programs/expt/base_view.hpp"

namespace expt {

/**
 * Objects of this class manage a view of the current page.
 */
class page_vw : public itemlist_vw {
  public:
    typedef itemlist_vw base_t;

    class columns : public itemlist_vw::base_columns {
      public:
        typedef itemlist_vw::base_columns base_column_t;

        Gtk::TreeModelColumn<Glib::ustring> m_pageref;

        inline columns() : base_column_t ()
                , m_pageref ()
                {
                  add (m_pageref);
                };

        inline ~columns() {};

        /**
         * Append columns to the tree view.
         */
        virtual void set_columns(itemlist_vw::list_window_t a_tree_view);

    };
    

  private:
    entry_vw entry_view;

    info_vw info_view;

    Gtk::Entry * enddate_view;

    Gtk::Button * finalise_button;

    Gtk::Entry * pageref_view;

    Gtk::Entry * startdate_view;

    Gtk::Entry * title_view;


  protected:
    /**
     * Create a new list control.
     */
    virtual std::auto_ptr< control::item_ctl > create_list_control(const purpletree::dom::element & a_node) const;


  private:
    /**
     * [UI Callback] Called when the "Finalise" button is clicked.
     */
    void finalise_page();


  public:
    /**
     * This view is changed if it or it's children are changed
     */
    virtual bool is_changed() const;


  private:
    /**
     * Test invariant conditions
     */
    void is_valid() const;

    /**
     * Not defined, added to stop copying.
     */
    page_vw & operator =(const page_vw & source);


  public:
    page_vw(const base_view & a_journal_view);

    inline virtual ~page_vw() {};


  private:
    /**
     * Not defined, added to stop copying.
     */
    page_vw(const page_vw & source);


  protected:
    /**
     * Set the values in a_row from the data pointed to by a_cursor.
     */
    void set_row(const itemlist_vw::row_iterator_t & a_row, const control::item_iterator & a_cursor);

    /**
     * Populate the newly created item in a_cursor with queried data.  The base function does nothing.
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
    static const base_view::string_t PAGELIST_VIEW_KEY;

    static const base_view::string_t PAGE_VIEW_KEY;

    static const base_view::string_t TITLE_VIEW_KEY;

    static const base_view::string_t PAGEREF_VIEW_KEY;

    static const base_view::string_t STARTDATE_VIEW_KEY;

    static const base_view::string_t ENDDATE_VIEW_KEY;

    static const base_view::string_t FINALISE_BUTTON_KEY;

};

} // namespace expt
#endif
