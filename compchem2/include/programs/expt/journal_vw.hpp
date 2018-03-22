#ifndef JOURNAL_VW_HPP
#define JOURNAL_VW_HPP

// ------------------------------------------------------------
// class: journal_vw
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
#include "programs/expt/page_vw.hpp"
#include <string>

namespace expt {

/**
 * Objects of this class manage UI Window that displays a journal document. This is the object that maintains the top level window.
 */
class journal_vw : public base_view {
  public:
    typedef base_view base_t;


  private:
    std::auto_ptr<control::journal_ctl> journal;

    page_vw page_view;

    sigc::signal1< void, bool > open_document_signal;

    sigc::signal1< void, bool > dirty_document_signal;

    /**
     * [UI callback] Optionally closes the currently viewed journal (and any associated views).  This will cause this object to be deconstructed.  If there are unsaved changes then the user is queried and the user may elect to cancel the close process.
     * 
     * \pre is_open ()
     */
    void close_document();


  public:
    /**
     * [UI callback] This function attempts to exit the application.  This function will test to see if there are any unsaved changes.  If there are unsaved changes and then a_unstoppable is true a back-up version of the journal will be saved otherwise the user will be queried for the save action.  This method should never return.
     */
    void exit();


  private:
    inline const control::journal_ctl & get_journal() const {
          return *journal;
        };


  public:
    /**
     * Indicate if there is an open document
     */
    inline bool is_open() const {
          return NULL != journal.get ();
        };


  private:
    /**
     * Test invariant conditions
     */
    void is_valid() const;

    /**
     * This function attempts to exit the application.  This function will test to see if there are any unsaved changes.  If there are unsaved changes and then a_unstoppable is true a back-up version of the journal will be saved otherwise the user will be queried for the save action.  This method should never return.
     */
    void no_stop_exit();


  public:
    /**
     * [UI callback] Open and display an existing journal document.  This will close a document if one is open.  The user is asked several questions which can lead to cancelling the process, so there may not be an open document at the end.
     * 
     * The user requests a journal stored in a permanent storage be loaded into the current application.  This version looks at the case when only a single journal can be opened at a time. If is_open () returns true then "Close Journal" use-case is called before ask_url().
     */
    void open_document();

    /**
     * [UI callback] Open and display an newly created journal document. This will close a document if one is open.  The user is asked several questions if a document is closed, so the document may be the existing document.
     * 
     * \post is_open ()
     */
    void open_new_document();

    /**
     * Main constructor.  
     */
    journal_vw();

    virtual ~journal_vw();


  private:
    /**
     * Not defined, added to stop copying.
     */
    journal_vw(const journal_vw & source);

    /**
     * Not defined, added to stop copying.
     */
    journal_vw & operator =(const journal_vw & source);

    /**
     * [UI callback] Save the journal document.  This may fail to save the document.
     */
    void save_as_document();

    /**
     * [UI callback] Save the journal document.  This may fail.
     */
    void save_document();

    /**
     * [impl] Used to set the senstivity of a "save" widget to indicate when a document (is_open and) is_dirty and has_filename.
     * 
     * \pre is_open ()
     * \pre a_widget /= nul
     */
    void save_sensitivity_test(base_view::window_t & a_item, bool a_is_dirty);

    static const base_view::string_t JOURNAL_VIEW_KEY;

    static const base_view::string_t UI_SOURCE_URL;

    static std::string backup_suffix;


  public:
    static inline const std::string & get_backup_suffix();

    static void set_backup_suffix(const std::string value);

};
inline const std::string & journal_vw::get_backup_suffix()
{
  return backup_suffix;
}


} // namespace expt
#endif
