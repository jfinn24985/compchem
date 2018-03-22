// ------------------------------------------------------------
// class: content_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/content_visitor.hpp"
#include "data_scheme/basic_datum.hpp"
#include "data_scheme/text.hpp"

// Manual includes
#include <boost/bind.hpp>
// -
namespace data_scheme {

/**
 * Process children of any object.
 */
void content_visitor::visit_basic_datum(const basic_datum & a_datum) 
{
  // Now process children
  if (not a_datum.empty ())
  {
    std::for_each (a_datum.begin (), a_datum.end (), boost::bind (&data_scheme::basic_datum::visit, _1, boost::ref(*this)));
  }

}

/**
 * Operate on text objects.
 */
void content_visitor::visit_text(const text & a_datum) 
{
  m_os << a_datum.the_content;
}


} // namespace data_scheme
