#ifndef COMPCHEM_DATA_SCHEME_VISITOR_PY_HPP
#define COMPCHEM_DATA_SCHEME_VISITOR_PY_HPP

// ------------------------------------------------------------
// class: visitor_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/python.hpp>

#include "data_scheme/visitor.hpp"

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
namespace data_scheme { class basic_datum; } 

namespace data_scheme {

class visitor_py : public boost::python::wrapper<visitor_py>, public visitor
{
  public:
    void visit_comment(const comment & a_datum);

    void def_visit_comment(const comment & a_datum);

    void visit_compound(const compound & a_datum);

    void def_visit_compound(const compound & a_datum);

    void visit_compound_ref(const compound_ref & a_datum);

    void def_visit_compound_ref(const compound_ref & a_datum);

    void visit_datum(const datum & a_datum);

    void def_visit_datum(const datum & a_datum);

    void visit_experiment(const experiment & a_datum);

    void def_visit_experiment(const experiment & a_datum);

    void visit_filter(const filter & a_datum);

    void def_visit_filter(const filter & a_datum);

    void visit_formula(const formula & a_datum);

    void def_visit_formula(const formula & a_datum);

    void visit_identity(const identity & a_datum);

    void def_visit_identity(const identity & a_datum);

    void visit_info(const info & a_datum);

    void def_visit_info(const info & a_datum);

    void visit_mechanism(const mechanism & a_datum);

    void def_visit_mechanism(const mechanism & a_datum);

    void visit_memo(const memo & a_datum);

    void def_visit_memo(const memo & a_datum);

    virtual void visit_pi(const processing_instruction & a_datum);

    void def_visit_pi(const processing_instruction & a_datum);

    void visit_program(const program & a_datum);

    void def_visit_program(const program & a_datum);

    void visit_project(const project & a_datum);

    void def_visit_project(const project & a_datum);

    void visit_settings(const settings & a_datum);

    void def_visit_settings(const settings & a_datum);

    void visit_text(const text & a_datum);

    void def_visit_text(const text & a_datum);

    void visit_worker(const worker & a_datum);

    void def_visit_worker(const worker & a_datum);

    void visit_basic_datum(const basic_datum & a_datum);

    /**
     * Pythonise the visitor class
     */
    static void pythonise();

    /**
     * Dtor (does nothing in this class)
     */
    virtual ~visitor_py();


};

} // namespace data_scheme
#endif
