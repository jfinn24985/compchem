#ifndef COMPCHEM_BASIC_FILTER_HPP
#define COMPCHEM_BASIC_FILTER_HPP

// ------------------------------------------------------------
// class: basic_filter
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/supplier_constants.hpp"
#include <string>

namespace data_scheme { class mechanism; } 
namespace data_scheme { class filter; } 
namespace data_scheme { class memo; } 

namespace supplier {

/**
 * Abstract base class for supplier filters managed by the filter manager
 * class.
 * 
 * The filters with outcome 'detect' should preferentially operate in BUFFER 
 * mode. When they have output for the other node sets the should return
 * SUCCESS to an output method which is also added to the result set. When
 * they have no output they return UNUSABLE to a output method.
 */
class basic_filter {
  private:
    /**
     * The data_scheme filter object this basic_filter object
     * is instantiated from.
     */
    const data_scheme::filter & m_filter;


  public:
    /**
     * Main Ctor
     */
    basic_filter(const data_scheme::filter & a_filter);

    virtual ~basic_filter() {}

    /**
     * An error message created when, for example, a method returns on of
     * the error return values (ie not SUCCESS, UNUSABLE or COMPLETE).
     */
    virtual std::string error_message() const = 0;

    /**
     * Indicate the parser/filter subtype:
     * * LINE_ORIENTED: process the file line-by-line.
     * * BUFFER_ORIENTED: process the file in 32kbyte buffered pieces.
     * * FILE_ORIENTED: work directly on the file.
     * 
     * Indicate how this object will return results.
     * * STRING_OUTPUT: Return the result as a string containing 
     * XML datum elements. The string is then interpreted and
     * inserted into the memo object be the manager.
     * * DIRECT_OUTPUT: Can accept a memo object and insert the
     * results directly.
     */
    virtual int flags() const = 0;

    /**
     * Information that identifies the filter/parser. This will
     * be placed into log messages from the manager and not in
     * the database. A typical message might contain a
     * descriptive name, version and author, eg.
     * "Nibble filter for Froggles: version 0.001: 2020/12/25: by P.J. Teeps"
     */
    virtual std::string identity() const = 0;

    /**
     * After creation of a filter object this method is called with a_phrase obtained 
     * from the 'detect' filter (or the filename for 'detect' filters) to prescreen the filter 
     * to see if they are usuable. The default implementation always returns 
     * SUCCESS.  When this returns anything other than SUCCESS the filter
     * must be in the same state as after a call to reset ().
     */
    virtual int initial_test(const std::string& a_phrase);

    /**
     * A unique identifier for this filter within the set of filters
     * that operate on the mechanism. This may not be empty.
     * 
     * (This is the the label of the data_scheme::filter object.)
     */
    const std::string & label() const;
    /**
     * The label of the mechanism associated with this filter.
     * 
     * (This is the the label of the mechanism parent of the 
     * data_scheme::filter object.)
     */
    const std::string & mechanism_label() const;
    /**
     * Output the results directly into the a_memo object.
     */
    virtual int output_direct(data_scheme::memo & a_memo) = 0;

    /**
     * Put the results into a_result string containing XML elements.
     */
    virtual int output_string(std::string & a_result) = 0;

    /**
     * Process the entire file. This is called for FILE_ORIENTED subtypes 
     * only. For mechanism discovery filters, the SUCCESS result indicates
     * that the file type was detected.
     */
    virtual int process_file(const std::string& a_filename) = 0;

    /**
     * Process a piece of the file.  This will only be called
     * on LINE_ORIENTED and BUFFER_ORIENTED subtypes. The
     * return is one of SUCCESS, FAIL_FATAL or FAIL_UNUSABLE.
     * 
     * The result FAIL_UNUSABLE indicates this object is not
     * usuable on this file and can be removed from the current
     * filter/parser set.
     */
    virtual int process_part(const std::string& a_buffer) = 0;

    /**
     * The identifier for the mechanism this filter operates with.
     */
    const std::string & outcomes() const;

    /**
     * Reset any internal result set.
     */
    virtual void reset() = 0;


  protected:
    /**
     * Return the defining filter object.
     */
    const data_scheme::filter & get_filter() const
    {
      return m_filter;
    }

};
/**
 * Write basic information about the plugin.  This 
   serialises the details of the plugin and is not 
   related to the filtering process.
 */
static inline std::ostream &
operator<< (std::ostream & a_os, const basic_filter & a_f)
{
  a_os << "Filter Identity: " << a_f.identity () << "\n";
  a_os << "       Provides: " << a_f.outcomes () << "\n";
  a_os << "   Process type: ";
  if (a_f.flags () & CCHEM_SUPPLIER_LINE_ORIENTED)
  {
    a_os << "LINE\n";
  }
  else if (a_f.flags () & CCHEM_SUPPLIER_BUFFER_ORIENTED)
  {
    a_os << "BUFFER\n";
  }
  else
  {
    a_os << "FILE\n";
  }
  a_os << "   Output style: ";
  if (a_f.flags () & CCHEM_SUPPLIER_STRING_OUTPUT)
  {
    a_os << "STRING\n";
  }
  else
  {
    a_os << "DIRECT\n";
  }
  return a_os;
}

} // namespace supplier
#endif
