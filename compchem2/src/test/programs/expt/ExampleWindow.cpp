// ------------------------------------------------------------
// class: ExampleWindow
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/programs/expt/ExampleWindow.hpp"

namespace expt {

ExampleWindow::ExampleWindow()
  : m_count(0)
    , m_Button_Quit("Quit")
    , m_Button_Add("Add")
    , m_Button_Reset("Reset")
{
  set_title("Gtk::TreeView (ListStore) example");
  set_border_width(5);
  set_default_size(400, 200);

  add(m_VBox);

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TreeView);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_VBox.pack_start(m_ScrolledWindow);
  m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

  m_ButtonBox.pack_start(m_Button_Add, Gtk::PACK_SHRINK);
  m_ButtonBox.pack_start(m_Button_Reset, Gtk::PACK_SHRINK);
  m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
  m_ButtonBox.set_border_width(5);
  m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
  m_Button_Quit.signal_clicked ().connect (sigc::mem_fun (*this, &ExampleWindow::on_button_quit));
  m_Button_Reset.signal_clicked ().connect (sigc::mem_fun (*this, &ExampleWindow::on_button_reset));
  m_Button_Add.signal_clicked ().connect (sigc::mem_fun (*this, &ExampleWindow::on_button_add));

  //Create the Tree model:
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);

  //Add the TreeView's view columns:
  m_TreeView.append_column("ID", m_Columns.m_col_id); //This number will be shown with the default numeric formatting.
  m_TreeView.append_column("Name", m_Columns.m_col_name);
 
  m_TreeView.append_column_numeric("Formatted number", m_Columns.m_col_number, "%010d" /* 10 digits, using leading zeroes. */);
   
  //Display a progress bar instead of a decimal number:
  Gtk::CellRendererProgress* cell = new Gtk::CellRendererProgress;
  int cols_count = m_TreeView.append_column("Some percentage", *cell);
  Gtk::TreeViewColumn* pColumn = m_TreeView.get_column(cols_count - 1);
  if(pColumn)
    pColumn->add_attribute(cell->property_value(), m_Columns.m_col_percentage);

 
  //Make all the columns reorderable:
  //This is not necessary, but it's nice to show the feature.
  //You can use TreeView::set_column_drag_function() to more
  //finely control column drag and drop.
  for(guint i = 0; i < 2; i++)
    {
      Gtk::TreeView::Column* pColumn = m_TreeView.get_column(i);
      pColumn->set_reorderable();
    }

  show_all_children();
}

ExampleWindow::~ExampleWindow(){
}

/**
 * Signal handlers:
 */
void ExampleWindow::on_button_quit() {
  hide();
}

void ExampleWindow::on_button_add() {
  //Fill the TreeView's model
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = m_count + 1;
  row[m_Columns.m_col_number] = (m_count + 1) * 10;
  switch (m_count)
    {
    case 0:
      row[m_Columns.m_col_name] = "Billy Bob";
      row[m_Columns.m_col_percentage] = 15;
      break;
    case 1:
      row[m_Columns.m_col_name] = "Joey Jojo";
      row[m_Columns.m_col_percentage] = 40;
      break;
    case 2:
      row[m_Columns.m_col_name] = "Rob McRoberts";
      row[m_Columns.m_col_percentage] = 70;
      break;
    default:
      row[m_Columns.m_col_name] = "Jane Doe";
      row[m_Columns.m_col_percentage] = 55;
      break;
    }
  //
  ++m_count;
  std::cout << "Added row " << m_count << " to model" << std::endl;
}

void ExampleWindow::on_button_reset() {
  m_refTreeModel->clear();
  m_count = 0;
  std::cout << "Removed all rows from to model" << std::endl;
}


} // namespace expt
