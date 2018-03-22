// ------------------------------------------------------------
// class: text_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/text_py.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

void basic_text_py::visit(visitor & a_visitor) const 
{
  this->get_override("visit")(a_visitor);
}

/**
 * Pythonise the text related element of data_scheme
 */
void basic_text_py::pythonise()

{
  boost::python::class_< basic_text_py, boost::noncopyable >("basic_text",
          "Base class for textual data_scheme components.",
          boost::python::no_init
          )
   .add_property("the_content", boost::python::make_getter(&basic_text::the_content, boost::python::return_value_policy< boost::python::return_by_value >()),
        "The text content of a comment, processing_instruction or text object.")
   ;
  
  boost::python::class_< comment, boost::python::bases< basic_text_py >, boost::noncopyable >("comment",
          "XML comment in data_scheme components.",
          boost::python::no_init
          )
   ;
  
  boost::python::class_< processing_instruction, boost::python::bases< basic_text_py >, boost::noncopyable >("processing_instruction",
          "XML processing_instruction in data_scheme components.",
          boost::python::no_init
          )
   .add_property("name", boost::python::make_getter(&processing_instruction::name, boost::python::return_value_policy< boost::python::return_by_value >()),
         "The name part of the processing_instruction object.")
   ;
  
  boost::python::class_< text, boost::python::bases< basic_text_py >, boost::noncopyable >("text",
          "XML text in data_scheme components.",
          boost::python::no_init
          )
   ;
  
  

}


} // namespace data_scheme
