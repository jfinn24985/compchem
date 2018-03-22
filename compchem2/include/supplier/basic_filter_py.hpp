#ifndef COMPCHEM_BASIC_FILTER_PY_HPP
#define COMPCHEM_BASIC_FILTER_PY_HPP

// ------------------------------------------------------------
// class: basic_filter_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter.hpp"
#include <boost/python.hpp>

#include <string>

namespace data_scheme { class memo; } 

namespace supplier {

/**
 * This class virtualises the C++ filter base class into Python. Python
 * filters derive and instantiate a version of this class. The python
 * source file must have a method "initialise" that returns an object
 * derived from the Python version of this class.
 */
class basic_filter_py : public basic_filter, public boost::python::wrapper<basic_filter>
{
  public:
    /**
     * An error message created when, for example, a method returns FAIL_FATAL.
     * Generally this is only called internally to generate exceptions.
     */
    std::string error_message() const;

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
    int flags() const;

    /**
     * Information that identifies the filter/parser. This will
     * be placed into log messages from the manager and not in
     * the database. A typical message might contain a
     * descriptive name, version and author, eg.
     * "Nibble filter for Froggles: version 0.001: 2020/12/25: by P.J. Teeps"
     */
    std::string identity() const;

    /**
     * Output the results directly into the a_memo object.
     */
    int output_direct(data_scheme::memo & a_memo);

    /**
     * Put the results into a_result string containing XML elements.
     */
    int output_string(std::string & a_result);

    /**
     * Process the entire file. This is called for FILE_ORIENTED subtypes 
     * only. For mechanism discovery filters, the SUCCESS result indicates
     * that the file type was detected.
     */
    int process_file(const std::string& a_filename);

    /**
     * Process a piece of the file.  This will only be called
     * on LINE_ORIENTED and BUFFER_ORIENTED subtypes. The
     * return is one of SUCCESS, FAIL_FATAL or FAIL_UNUSABLE.
     * 
     * The result FAIL_UNUSABLE indicates this object is not
     * usuable on this file and can be removed from the current
     * filter/parser set.
     */
    int process_part(const std::string& a_buffer);

    /**
     * Reset any internal result set.
     */
    void reset();

    /**
     * Generate the python version of the basic_filter C++ class.
     */
    static void pythonise();


};

} // namespace supplier
#endif
