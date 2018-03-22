#ifndef COMPCHEM_DATA_SCHEME_CONTENT_VISITOR_HPP
#define COMPCHEM_DATA_SCHEME_CONTENT_VISITOR_HPP

// ------------------------------------------------------------
// class: content_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/visitor.hpp"

namespace data_scheme { class basic_datum; } 
namespace data_scheme { class text; } 

namespace data_scheme {

/**
 * This class collects the complete textual content of the current object and
 * all its descendents into the ostream object given in the constructor.
 */
class content_visitor : public visitor
{
  private:
    /**
     * Reference to a stream in which to insert the text.
     */
    std::ostream & m_os;


  public:
    content_visitor(std::ostream & a_os) throw ()
    : m_os (a_os)
    {}

    virtual ~content_visitor() throw () {}


  private:
    /**
     * no copy
     */
    content_visitor(const content_visitor & source);


  public:
    /**
     * Process children of any object.
     */
    void visit_basic_datum(const basic_datum & a_datum);

    /**
     * Operate on text objects.
     */
    void visit_text(const text & a_datum);


  private:
    /**
     * no assign
     */
    content_visitor & operator=(const content_visitor & source);


};

} // namespace data_scheme
#endif
