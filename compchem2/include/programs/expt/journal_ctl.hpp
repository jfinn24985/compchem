#ifndef JOURNAL_CTL_HPP
#define JOURNAL_CTL_HPP

// ------------------------------------------------------------
// class: journal_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <memory>
#include <string>

namespace expt {

/**
 * This classes manages the application task and manages the experiment log document.  This includes log openning/closing and configuration.
 * 
 * Control the model of the laboratory journal (lab-book) of an experimenter.  The role of the type is to manage the textual observations for a set of experiments (an experiment is modelled by the journal_page class)
 * 
 * In this model of a journal, each user has a single journal.  Thus each instance of an application will only have a single journal.
 * 
 * Features not implemented in this release: 
 * * Link to experimenter.
 * * Link to result data.
 */
class journal_ctl {
  private:
    bool dirty;

    purpletree::dom::document document_node;

    boost::filesystem::path filename;

    sigc::signal1< void, bool > is_dirty_signal;


  public:
    /**
     * Create a new empty journal document object.
     */
    static std::auto_ptr<journal_ctl> create();

    /**
     * Get the document level DOM element (const version)
     */
    inline const purpletree::dom::element get_document_element() const {
          is_valid ();
          return document_node.document_element ();
        };

    /**
     * Get the document level DOM object
     */
    inline purpletree::dom::element get_document_element() {
          is_valid ();
          return document_node.document_element ();
        };

    /**
     * Get the filename of the currently open document.
     * 
     * \pre has_filename ()
     */
    inline const std::string get_filename() const {
          return filename.string();
        };

    /**
     * Get a handler for the is_dirty signal.  This signal is emitted when the document becomes dirty due to editting or clean due to saving.  There is no 'undo' operation...
     */
    inline sigc::signal1< void, bool > get_is_dirty_signal();

    /**
     * Does the current journal document have a filename?
     * 
     * This will only be false for new documents.
     */
    inline bool has_filename() const {
          is_valid ();
          return not filename.empty ();
        };

    /**
     * Is there unsaved modifications in the model document?
     */
    inline bool is_dirty() const {
          is_valid ();
          return dirty;
        };

    /**
     * Does the file exist and contain an XML file of the right type?
     * 
     * \post true implies boost::filesystem::exists (a_filename) and
     * dom::implementation::open (a_filename).doctype () == JOURNAL_DOCTYPE
     * 
     * \note Note that our DOM library doesn't support comparison operators. The only test that we can use for comparing doctype nodes is therefore to compare the names of the base elements. This is a necessary but not sufficient condition.  More rigorous tests would be a better, such as a proper comparison operator and/or a method that allows validation of a DOM tree against a given doctype node.
     */
    static bool is_journal_document(const std::string & a_filename);


  private:
    /**
     * Test invariant conditions
     */
    inline void is_valid() const {
          INVARIANT (not document_node.is_null (), "Journal must have a valid document.");
        };


  public:
    /**
     * Indicate that changes have been made to the document.  This uses a map of document to journal_ctl pointers.
     */
    static void make_dirty(const purpletree::dom::document & a_doc);

    /**
     * Indicate that changes have been made to the document.
     */
    inline void make_dirty() {
          is_valid ();
          if (not dirty)
            {
              dirty = true;
              is_dirty_signal.emit (true);
              ENSURE (is_dirty (), "When a document is made dirty it should be dirty.");
            }
        };

    /**
     * Construct a valid document object from the file/document identified by a_filename.
     * 
     * \pre is_journal_document (a_filename) = true
     */
    static std::auto_ptr<journal_ctl> open(const std::string & a_filename);

    /**
     * Attempt to save the document.
     * 
     * \pre is_dirty ()
     * \pre has_filename ()
     * \result true if document successfully saved.
     * 
     * \post not is_dirty ()
     */
    bool serialise();

    /**
     * Set the filename of the current document.
     */
    void set_filename(const std::string value);


  private:
    /**
     * Constructor for a journal object.
     * 
     * \pre not a_doc_node.is_null ()
     */
    journal_ctl(purpletree::dom::document & a_doc_node, const std::string & a_filename);


  public:
    ~journal_ctl();


  private:
    /**
     * Not defined, added to stop copying.
     */
    journal_ctl(const journal_ctl & source);

    /**
     * Not defined, added to stop copying.
     */
    journal_ctl & operator =(const journal_ctl & source);


  public:
    static const purpletree::dom::dom_string JOURNAL_DOC_NODE_NAME;

    static const purpletree::dom::dom_string JOURNAL_DOC_PUBLIC_ID;

    static const purpletree::dom::dom_string JOURNAL_DOC_SYSTEM_ID;

};
//Get a handler for the is_dirty signal.  This signal is emitted when the document becomes dirty due to editting or clean due to saving.  There is no 'undo' operation...

/**
 * Get a handler for the is_dirty signal.  This signal is emitted when the document becomes dirty due to editting or clean due to saving.  There is no 'undo' operation...
 */
inline sigc::signal1< void, bool > journal_ctl::get_is_dirty_signal() {
  return is_dirty_signal;
}


} // namespace expt
#endif
