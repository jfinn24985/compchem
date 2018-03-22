#ifndef EXAMPLEWINDOW_HPP
#define EXAMPLEWINDOW_HPP

// ------------------------------------------------------------
// class: ExampleWindow
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "test/programs/expt/typedef1.hpp"

namespace expt {

class ExampleWindow : public typedef1 {
  public:
    ExampleWindow();

    virtual ~ExampleWindow();


  protected:
    /**
     * Signal handlers:
     */
    virtual void on_button_quit();

    virtual void on_button_add();

    virtual void on_button_reset();


  public:
    class ModelColumns : public typedef2 {
      public:
        inline ModelColumns() { add(m_col_id); add(m_col_name); add(m_col_number); add(m_col_percentage);};

        Gtk::TreeModelColumn<unsigned int> m_col_id;

        Gtk::TreeModelColumn<Glib::ustring> m_col_name;

        Gtk::TreeModelColumn<short> m_col_number;

        Gtk::TreeModelColumn<int> m_col_percentage;

    };
    
    typedef Gtk::TreeModel::ColumnRecord typedef2;


  protected:
    ModelColumns m_Columns;

    unsigned int m_count;

    Gtk::VBox m_VBox;

    Gtk::ScrolledWindow m_ScrolledWindow;

    Gtk::TreeView m_TreeView;

    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

    Gtk::HButtonBox m_ButtonBox;

    Gtk::Button m_Button_Quit;

    Gtk::Button m_Button_Add;

    Gtk::Button m_Button_Reset;

};

} // namespace expt
#endif
