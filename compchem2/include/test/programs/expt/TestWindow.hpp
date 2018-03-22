#ifndef TESTWINDOW_HPP
#define TESTWINDOW_HPP

// ------------------------------------------------------------
// class: TestWindow
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

class TestWindow : public typedef1 {
  public:
    TestWindow();

    inline virtual ~TestWindow() {};


  protected:
    /**
     * Signal handlers:
     */
    virtual void on_button_info_clicked();

    virtual void on_button_question_clicked();

    Gtk::VButtonBox m_ButtonBox;

};

} // namespace expt
#endif
