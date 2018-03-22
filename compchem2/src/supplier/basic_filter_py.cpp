// ------------------------------------------------------------
// class: basic_filter_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/basic_filter_py.hpp"
#include "data_scheme/memo.hpp"

namespace supplier {

/**
 * An error message created when, for example, a method returns FAIL_FATAL.
 * Generally this is only called internally to generate exceptions.
 */
std::string basic_filter_py::error_message() const 
{
  return this->get_override("error_message")();
}

/**
 * Indicate the parser/filter subtype:
 * * LINE_ORIENTED: process the file line-by-line.
 * * BUFFER_ORIENTED: process the file in 32kbyte buffered pieces.
 * * FILE_ORIENTED: work directly on the file.
 * 
 * The first two are managed by a parser/filter manager
 * 
 * Indicate how this object will return results.
 * * XMLSTRING: Return the result as a string containing 
 * XML datum elements. The string is then interpreted and
 * inserted into the memo object be the manager.
 * * or: Can accept a memo object and insert the
 * results directly.
 */
int basic_filter_py::flags() const 
{
  return this->get_override("flags")();
}

/**
 * Information that identifies the filter/parser. This will
 * be placed into log messages from the manager and not in
 * the database. A typical message might contain a
 * descriptive name, version and author, eg.
 * "Nibble filter for Froggles: version 0.001: 2020/12/25: by P.J. Teeps"
 */
std::string basic_filter_py::identity() const 
{
  return this->get_override("identity")();
}

/**
 * Output the results directly into the a_memo object.
 */
int basic_filter_py::output_direct(data_scheme::memo & a_memo) 
{
  return this->get_override("output_direct")(a_memo);
}

/**
 * Put the results into a_result string containing XML elements.
 */
int basic_filter_py::output_string(std::string & a_result) 
{
  return this->get_override("output_string")(a_result);
}

/**
 * Process the entire file. This is called for FILE_ORIENTED subtypes 
 * only. For mechanism discovery filters, the SUCCESS result indicates
 * that the file type was detected.
 */
int basic_filter_py::process_file(const std::string& a_filename) 
{
  return this->get_override("process_file")(a_filename);
}

/**
 * Process a piece of the file.  This will only be called
 * on LINE_ORIENTED and BUFFER_ORIENTED subtypes. The
 * return is one of SUCCESS, FAIL_FATAL or FAIL_UNUSABLE.
 * 
 * The result FAIL_UNUSABLE indicates this object is not
 * usuable on this file and can be removed from the current
 * filter/parser set.
 */
int basic_filter_py::process_part(const std::string& a_buffer) 
{
  return this->get_override("process_part")(a_buffer);
}

/**
 * Reset any internal result set.
 */
void basic_filter_py::reset() 
{
  this->get_override("reset")();
}

/**
 * Generate the python version of the basic_filter C++ class.
 */
void basic_filter_py::pythonise()

{
  boost::python::class_< basic_filter_py, boost::noncopyable >("basic_filter"
        , boost::python::no_init)
      .def("error_message", boost::python::pure_virtual(&basic_filter::error_message))
      .def("flags", boost::python::pure_virtual(&basic_filter::flags))
      .def("identity", boost::python::pure_virtual(&basic_filter::identity))
      .def("output_direct", boost::python::pure_virtual(&basic_filter::output_direct))
      .def("output_string", boost::python::pure_virtual(&basic_filter::output_string))
      .def("process_file", boost::python::pure_virtual(&basic_filter::process_file))
      .def("process_part", boost::python::pure_virtual(&basic_filter::process_part))
      .def("reset", boost::python::pure_virtual(&basic_filter::reset))
      .add_property("label", boost::python::make_function (&basic_filter::label, boost::python::return_value_policy< boost::python::return_by_value >()))
      .add_property("outcomes", boost::python::make_function (&basic_filter::outcomes, boost::python::return_value_policy< boost::python::return_by_value >()))
      ;

}


} // namespace supplier
