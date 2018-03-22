#ifndef COMPCHEM_XML_WRITE_VISITOR_HPP
#define COMPCHEM_XML_WRITE_VISITOR_HPP

// ------------------------------------------------------------
// class: xml_write_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/visitor.hpp"
#include "data_access/names.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <tr1/memory>
#include <string>

namespace data_access { class meta_datum; } 
namespace data_access { class xml_writer_proxy; } 
namespace data_access { class xml_writer; } 
namespace data_scheme { class basic_datum; } 
namespace data_scheme { class comment; } 
namespace data_scheme { class processing_instruction; } 
namespace data_scheme { class text; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class project; } 

namespace data_access {

/**
 * This class is a visitor class that writes the database tree into an
 * XML document.
 * 
 * Standard usage is something like the following. Note we use the
 * visit_basic_datum as memo-src and proj-src objects would get
 * into a loop as visit_memo and visit_project woudl call a_datum.write
 * 
 *   xml_write_visitor l_obj (os, doc_elem_name, pub_id, sys_id); 
 *   os << extra_doc_headers;
 *   l_obj.visit_basic_datum (a_datum);
 */
class xml_write_visitor : public data_scheme::visitor
{
  private:
    /**
     * The wrapped output stream.
     */
    boost::ptr_vector<xml_writer_proxy> m_writer;

    /**
     * The xml_writer.
     */
    std::tr1::shared_ptr<xml_writer> m_writer_rep;


  public:
    /**
     * Write well-formed XML for this node (and its children) into a_os.
     * 
     * Note that this should only be overriden for types like text, comment and PI
     * 
     * \final
     */
    virtual void visit_basic_datum(const data_scheme::basic_datum & a_datum);

    /**
     * Write a comment.
     */
    virtual void visit_comment(const data_scheme::comment & a_comment);

    /**
     * Write well-formed XML for processing instruction.
     */
    virtual void visit_pi(const data_scheme::processing_instruction & a_pi);

    /**
     * Write well-formed XML for this node (and its children) into a_os.
     * 
     * No default implementation.
     */
    virtual void visit_text(const data_scheme::text & a_text);

    virtual void visit_memo(const data_scheme::memo & a_datum);

    virtual void visit_project(const data_scheme::project & a_datum);


  private:
    /**
     * no assign
     */
    xml_write_visitor & operator=(const xml_write_visitor & source);


  public:
    /**
     * Construct and initialise the xml_write_visitor.
     * 
     * After this method the xml header (<?xml ..> + <!DOCTYPE..>)
     * is written to the stream but not the document element. If 
     * you want to add extra header information you can write these
     * directly to the stream before initialising the write process.
     */
     xml_write_visitor(std::ostream & a_os, const std::string & a_label, const std::string& a_pub_id, const std::string& a_sys_id);


  private:
    /**
     * no copy
     */
    xml_write_visitor(const xml_write_visitor & source);


  public:
    virtual ~xml_write_visitor() throw ();


};

} // namespace data_access
#endif
