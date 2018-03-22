#ifndef COMPCHEM_VERIFY_VISITOR_HPP
#define COMPCHEM_VERIFY_VISITOR_HPP

// ------------------------------------------------------------
// class: verify_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/visitor.hpp"

namespace data_scheme { class basic_datum; } 

namespace data_access {

/**
 * Perform the verification actions on a database.
 * 
 * for each project
 *    open project // will auto-fix link
 * 
 * for each memo
 *    open memo // will auto-fix link
 * 
 * Use ID/IDREF tables to verify that the following reference attributes have a target object:
 *  * compound-ref -> compound/formula
 *  * experiment -> worker
 *  * memo -> mechanism/filter, compound/formula
 *  * project -> worker
 *  * settings -> (parent)/filter
 */
class verify_visitor : public data_scheme::visitor
{
  private:
    /**
     * no assign
     */
    verify_visitor & operator=(const verify_visitor & source);


  public:
    /**
     * Print a report on the status of cross-references.
     */
    void report_cross_references(std::ostream & a_log);

    verify_visitor();

    ~verify_visitor();


  private:
    /**
     * no copy
     */
    verify_visitor(const verify_visitor & source);


  public:
    /**
     * Method to operate on objects as basic_datum. (does nothing)
     */
    void visit_basic_datum(const data_scheme::basic_datum & a_datum);

};

} // namespace data_access
#endif
