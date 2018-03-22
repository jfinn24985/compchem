#ifndef COMPCHEM_DATA_SCHEME_VISITOR_HPP
#define COMPCHEM_DATA_SCHEME_VISITOR_HPP

// ------------------------------------------------------------
// class: visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme { class basic_datum; } 
namespace data_scheme { class comment; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class compound_ref; } 
namespace data_scheme { class datum; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class filter; } 
namespace data_scheme { class formula; } 
namespace data_scheme { class identity; } 
namespace data_scheme { class info; } 
namespace data_scheme { class mechanism; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class processing_instruction; } 
namespace data_scheme { class program; } 
namespace data_scheme { class project; } 
namespace data_scheme { class settings; } 
namespace data_scheme { class text; } 
namespace data_scheme { class worker; } 

namespace data_scheme {

/**
 * This class defines an abstract interface to an object that can 
 * visit the data objects in document-order. 
 */
class visitor
{
  public:
    /**
     * Pure virtual method to operate on objects as basic_datum.
     */
    virtual void visit_basic_datum(const basic_datum & a_datum) = 0;

    /**
     * Operator on comment objects.
     */
    virtual void visit_comment(const comment & a_datum);

    /**
     * Operate on compound objects.
     */
    virtual void visit_compound(const compound & a_datum);

    /**
     * Operate on compound objects.
     */
    virtual void visit_compound_ref(const compound_ref & a_datum);

    /**
     * Operate on datum objects.
     */
    virtual void visit_datum(const datum & a_datum);

    /**
     * Operate on experiment objects.
     */
    virtual void visit_experiment(const experiment & a_datum);

    /**
     * Operate on filter objects.
     */
    virtual void visit_filter(const filter & a_datum);

    /**
     * Operate on formula objects.
     */
    virtual void visit_formula(const formula & a_datum);

    /**
     * Operate on identity objects.
     */
    virtual void visit_identity(const identity & a_datum);

    /**
     * Operate on info objects.
     */
    virtual void visit_info(const info & a_datum);

    /**
     * Operate on mechanism objects.
     */
    virtual void visit_mechanism(const mechanism & a_datum);

    /**
     * Operate on memo objects.
     */
    virtual void visit_memo(const memo & a_datum);

    /**
     * Operate on processing_instruction objects.
     */
    virtual void visit_pi(const processing_instruction & a_datum);

    /**
     * Operate on program objects.
     */
    virtual void visit_program(const program & a_datum);

    /**
     * Operate on project objects.
     */
    virtual void visit_project(const project & a_datum);

    /**
     * Operate on settings objects.
     */
    virtual void visit_settings(const settings & a_datum);

    /**
     * Operate on text objects.
     */
    virtual void visit_text(const text & a_datum);

    /**
     * Operate on worker objects.
     */
    virtual void visit_worker(const worker & a_datum);

    virtual ~visitor() {}


};

} // namespace data_scheme
#endif
