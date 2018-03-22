// ------------------------------------------------------------
// class: scope
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/newcalc/scope.hpp"

namespace cchem {

namespace newcalc {

/**
 * Whether to produce log output of the configuration searches.
 */
bool scope::m_log= false;

/**
 * A test version of value() where a_result contains a meaningful value if
 * the return value is true.  This should be used when you want to retrieve
 * a value only if it is truly present.
 */
bool scope_local::has_value_virt(const resource_path & a_path, std::string & a_result) const throw(std::bad_alloc) {
  const_iterator l_ix (m_local.find (a_path.string ()));
  bool Result;
  if (m_local.end () == l_ix) { // No key
    Result = m_up.has_value_virt (a_path, a_result);
    if (Result) {
      m_local.insert (std::make_pair (a_path.string (), a_result)); }}
  else {
    Result = true;
    a_result.assign (l_ix->second); }
  return Result; 
}


} // namespace cchem::newcalc

} // namespace cchem
