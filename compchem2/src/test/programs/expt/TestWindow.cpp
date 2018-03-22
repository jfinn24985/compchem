// ------------------------------------------------------------
// class: TestWindow
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/programs/expt/TestWindow.hpp"

namespace expt {

TestWindow::TestWindow()
  : m_Button_Info("Show Info MessageDialog"),
    m_Button_Question("Show Question MessageDialog")
{
  set_title("Gtk::MessageDialog example");

  add(m_ButtonBox);
  
  m_ButtonBox.pack_start(m_Button_Info);
  m_Button_Info.signal_clicked().connect( sigc::mem_fun(*this, &TestWindow::on_button_info_clicked) );

  m_ButtonBox.pack_start(m_Button_Question);
  m_Button_Question.signal_clicked().connect( sigc::mem_fun(*this, &TestWindow::on_button_question_clicked) );

  show_all_children();
}

/**
 * Signal handlers:
 */
void TestWindow::on_button_info_clicked() {
  Gtk::MessageDialog dialog(*this, "This is an INFO MessageDialog");
  dialog.set_secondary_text("And this is the secondary text that explains things.");

  dialog.run();
}

void TestWindow::on_button_question_clicked() {
  Gtk::MessageDialog dialog(*this
			    , "This is a QUESTION MessageDialog"
			    , false /* use_markup */
			    , Gtk::MESSAGE_QUESTION
			    , Gtk::BUTTONS_OK_CANCEL);
  dialog.set_secondary_text("And this is the secondary text that explains things.");
  
  // Add an entry widget
  Gtk::HBox l_part;
  Gtk::Entry l_text;
  Gtk::Label l_label ("Enter text here");
  std::string a_value ("hello world");
  dialog.get_vbox ()->add (l_part);
  l_part.add (l_label);
  l_part.add (l_text);
  l_text.set_text (a_value);
  dialog.get_vbox ()->show_all ();
  int result = dialog.run();

  //Handle the response:
  switch(result)
    {
    case(Gtk::RESPONSE_OK):
      {
	std::cout << "OK clicked." << std::endl;
	a_value =l_text.get_text ();
	std::cout << "Entered text: " << a_value << std::endl;
	break;
      }
    case(Gtk::RESPONSE_CANCEL):
      {
	std::cout << "Cancel clicked." << std::endl;
	break;
      }
    default:
      {
	std::cout << "Unexpected button clicked." << std::endl;
	break;
      }
    }
  
}


} // namespace expt
