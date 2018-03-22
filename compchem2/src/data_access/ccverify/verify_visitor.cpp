// ------------------------------------------------------------
// class: verify_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/ccverify/verify_visitor.hpp"
#include "data_scheme/basic_datum.hpp"

namespace data_access {

/**
 * Print a report on the status of cross-references.
 */
void verify_visitor::report_cross_references(std::ostream & a_log) 
{
  std::cout << "**Warning** Currently no test of cross references.\n";

}

verify_visitor::verify_visitor() 
{

}

verify_visitor::~verify_visitor() 
{

}

/**
 * Method to operate on objects as basic_datum. (does nothing)
 */
void verify_visitor::visit_basic_datum(const data_scheme::basic_datum & a_datum) 
{}


} // namespace data_access
