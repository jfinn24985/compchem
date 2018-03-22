// ------------------------------------------------------------
// class: journal_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/journal_ctl.hpp"

namespace expt {

//Create a new empty journal document object.

/**
 * Create a new empty journal document object.
 */
std::auto_ptr<journal_ctl> journal_ctl::create()
{
  // LOCAL
  purpletree::dom::document l_doc (purpletree::dom::implementation::create_document (purpletree::dom::dom_string()
  										   , JOURNAL_DOC_NODE_NAME
  										   , JOURNAL_DOC_PUBLIC_ID
  										   , JOURNAL_DOC_SYSTEM_ID));
  // DO
  ALWAYS (not l_doc.is_null (), "Unable to create a valid empty document");
  return std::auto_ptr< journal_ctl > (new journal_ctl (l_doc));
}

//Does the file exist and contain an XML file of the right type?
//
//\post true implies boost::filesystem::exists (a_filename) and
//dom::implementation::open (a_filename).doctype () == JOURNAL_DOCTYPE
//
//\note Note that our DOM library doesn't support comparison operators. The only test that we can use for comparing doctype nodes is therefore to compare the names of the base elements. This is a necessary but not sufficient condition.  More rigorous tests would be a better, such as a proper comparison operator and/or a method that allows validation of a DOM tree against a given doctype node.

/**
 * Does the file exist and contain an XML file of the right type?
 * 
 * \post true implies boost::filesystem::exists (a_filename) and
 * dom::implementation::open (a_filename).doctype () == JOURNAL_DOCTYPE
 * 
 * \note Note that our DOM library doesn't support comparison operators. The only test that we can use for comparing doctype nodes is therefore to compare the names of the base elements. This is a necessary but not sufficient condition.  More rigorous tests would be a better, such as a proper comparison operator and/or a method that allows validation of a DOM tree against a given doctype node.
 */
bool journal_ctl::is_journal_document(const std::string & a_filename)
{
  // LOCAL
  bool Result(false);
  // DO
  
  // We do not test for symbolic_link_exists as we are only interested
  // in readable files.
  if (boost::filesystem::exists (a_filename))
  {
    purpletree::dom::document l_doc (purpletree::dom::implementation::deserialise (a_filename));
    // Simply check that the document has the right document element.
    // To have got a document means that the document was valid.
    if (not l_doc.is_null ())
      {
        purpletree::dom::element l_node (l_doc.document_element ());
        if (not l_node.is_null ())
  	{
  	  Result = (l_node.tag_name () == journal_ctl::JOURNAL_DOC_NODE_NAME);
  	}
      }
  }
  return Result;
}

//Indicate that changes have been made to the document.  This uses a map of document to journal_ctl pointers.

/**
 * Indicate that changes have been made to the document.  This uses a map of document to journal_ctl pointers.
 */
void journal_ctl::make_dirty(const purpletree::dom::document & a_doc)
{
  // LOCAL
  // (find_doc ensures a valid iterator)
  // Make associated document dirty
  find_doc (a_doc)->second->make_dirty ();
}

//Construct a valid document object from the file/document identified by a_filename.
//
//\pre is_journal_document (a_filename) = true

/**
 * Construct a valid document object from the file/document identified by a_filename.
 * 
 * \pre is_journal_document (a_filename) = true
 */
std::auto_ptr<journal_ctl> journal_ctl::open(const std::string & a_filename)
{
  // LOCAL
  // PRE
  REQUIRE (boost::filesystem::exists (a_filename)
  	 , std::string ("No file with name [" + a_filename + "]").c_str ());
  // We do not test for symbolic_link_exists as we are only interested
  // in readable files.
  purpletree::dom::document l_doc (purpletree::dom::implementation::deserialise (a_filename));
  // Simply check that the document has the right document element.
  // To have got a document means that the document was valid.
  REQUIRE (not l_doc.is_null ()
  	 and not l_doc.document_element ().is_null ()
  	 and l_doc.document_element ().tag_name () == journal_ctl::JOURNAL_DOC_NODE_NAME
  	 , std::string ("Document read from file [" + a_filename + "] is not valid").c_str ());
  // Document was opened successfully.
  return std::auto_ptr< journal_ctl > (new journal_ctl (l_doc, a_filename));
}

//Attempt to save the document.
//
//\pre is_dirty ()
//\pre has_filename ()
//\result true if document successfully saved.
//
//\post not is_dirty ()

/**
 * Attempt to save the document.
 * 
 * \pre is_dirty ()
 * \pre has_filename ()
 * \result true if document successfully saved.
 * 
 * \post not is_dirty ()
 */
bool journal_ctl::serialise() {
  // PRE
  is_valid ();
  REQUIRE (has_filename (), "Cannot save document into an unnamed file");
  // DO
  purpletree::dom::implementation::serialise (document_node, filename.string ());
  
  dirty = false;
  is_dirty_signal.emit (false);
  return true;
}

//Set the filename of the current document.

/**
 * Set the filename of the current document.
 */
void journal_ctl::set_filename(const std::string value) {
  filename = boost::filesystem::path(value, boost::filesystem::native);
}

//Constructor for a journal object.
//
//\pre not a_doc_node.is_null ()

/**
 * Constructor for a journal object.
 * 
 * \pre not a_doc_node.is_null ()
 */
journal_ctl::journal_ctl(purpletree::dom::document & a_doc_node, const std::string & a_filename) 
: dirty (false)
, document_node (a_doc_node)
, filename (a_filename, boost::filesystem::native)
, is_dirty_signal ()
{
  REQUIRE (not a_doc_node.is_null (), "Journal requires a valid document node when being created.");
  S_map.push_back (std::make_pair (document_node, this));
  is_valid ();
}

journal_ctl::~journal_ctl(){
  S_map.erase (find_doc (document_node));
}

/**
 * Not defined, added to stop copying.
 */
journal_ctl::journal_ctl(const journal_ctl & source){
}

/**
 * Not defined, added to stop copying.
 */
journal_ctl & journal_ctl::operator =(const journal_ctl & source) {
}

const purpletree::dom::dom_string journal_ctl::JOURNAL_DOC_NODE_NAME(purpletree::dom::set_utf8("journal"));

const purpletree::dom::dom_string journal_ctl::JOURNAL_DOC_PUBLIC_ID(purpletree::dom::set_utf8("http://cchem.org/journal/version 1.0/2006"));

const purpletree::dom::dom_string journal_ctl::JOURNAL_DOC_SYSTEM_ID(purpletree::dom::set_utf8("journal.dtd"));


} // namespace expt
