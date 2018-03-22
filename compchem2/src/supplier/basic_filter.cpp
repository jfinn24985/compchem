// ------------------------------------------------------------
// class: basic_filter
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/basic_filter.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/memo.hpp"

namespace supplier {

/**
 * Main Ctor
 */
basic_filter::basic_filter(const data_scheme::filter & a_filter) 
: m_filter (a_filter)
{}

/**
 * After creation of a filter object this method is called with a_phrase obtained 
 * from the 'detect' filter (or the filename for 'detect' filters) to prescreen the filter 
 * to see if they are usuable. The default implementation always returns 
 * SUCCESS.  When this returns anything other than SUCCESS the filter
 * must be in the same state as after a call to reset ().
 */
int basic_filter::initial_test(const std::string& a_phrase) 
{
  return CCHEM_SUPPLIER_SUCCESS;
}

/**
 * A unique identifier for this filter within the set of filters
 * that operate on the mechanism. This may not be empty.
 * 
 * (This is the the label of the data_scheme::filter object.)
 */
const std::string & basic_filter::label() const 
{
  return m_filter.label ();
}

/**
 * The label of the mechanism associated with this filter.
 * 
 * (This is the the label of the mechanism parent of the 
 * data_scheme::filter object.)
 */
const std::string & basic_filter::mechanism_label() const 
{
  return m_filter.parent ().label ();
}

/**
 * The identifier for the mechanism this filter operates with.
 */
const std::string & basic_filter::outcomes() const 
{
  return m_filter.outcomes (); 
}


} // namespace supplier
