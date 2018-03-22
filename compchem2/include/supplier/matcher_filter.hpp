#ifndef COMPCHEM_MATCHER_FILTER_HPP
#define COMPCHEM_MATCHER_FILTER_HPP

// ------------------------------------------------------------
// class: matcher_filter
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter.hpp"
#include <string>
#include <memory>
#include <sstream>


namespace data_scheme { class content_visitor; } 
namespace supplier { class basic_matcher; } 
namespace data_scheme { class filter; } 
namespace data_scheme { class memo; } 

namespace supplier {

/**
 * A matcher filter uses plain-text, regular-expression or a counter to 
 * extract information from a file. It can have up to three submatch 
 * objects:
 * * start: signals the 'start' of the main match process. Optionally it can
 * collect top-level result information.
 * * main: collects the results. This can use a repetitive, first or last match
 * sequence.
 * * end: signals the end of the main match process, not results can 
 * be collected.
 * 
 * For 'simplicity' this type of filter is always LINE_ORIENTED and produces
 * STRING_OUTPUT.
 * 
 * The features of the submatchers are:
 * * plain-text exact match as substring of line, output uses printf like format string
 * * regular-expression match as subpart of line, output uses sed like re_format 
 * string.
 * * count: counts bytes or lines, no result match
 * 
 * Results:
 * * A top-level format string (if present) is used as container of the result set.
 * * With no top-level string AND multiple results a container element is 
 * automatically created using the filter object's outcome.
 * * Matchers use a format string to generate results. This must encode a 
 * 'datum' start tag but no ending tag. The system uses the boost regex library
 * so for regex matchers the format string is as per this library. It is similar to
 * a sed format string. The system uses the boost format lbrary for plain-text
 * searchs, this is similar to posix printf (ie has positional arguments that can be
 * ignored)
 * 
 * TODO: * The optional phrase '%COUNT%', if present
 * in the format string, will be replaced with the current number of matchs
 * (1 based).
 * 
 * The matcher can operate in three modes:
 * * first: in this mode the first match returns 'COMPLETE'. For the top
 * matcher_filter matching a start submatch this triggers the state change
 * to the main submatch and 'SUCCESS' is returned.
 * * accum: in this mode the results of the main submatch are collected.
 * * last: this mode is similar 'accum' except earlier results are discarded 
 * every time 'main' matches.
 * 
 * Notes: For logical reasons the start and end submatch must have 
 * mode = first.
 * 
 * 
 * An optional ON_NO_MATCH can be set to one of the default return values.
 * This is then the return value used when no match has occurred (you can
 * also set an error message string).
 * 
 * A matcher functor object is used to extract information from a buffer.  A
 * matcher is a combination of a base_matcher object for inspecting the
 * buffer and a starting and an ending offset.  When being fed a buffer 
 * piecemeal it uses the offsets to decide when to inspect the buffer for
 * a match.  If the ending offset is passed, no_match is true to indicate 
 * the base_matcher object will not be used on subsequent match()
 * operations.  When a match has occurred have_result is true and you 
 * can get the result.  The matcher's internal state can be reset() if 
 * the object is going to be reused.
 */
class matcher_filter : public basic_filter {
  public:
    enum mode_constant
    {
      first = 0,
      last,
      accum

    };


  private:
    /**
     * The currently in-use matcher.
     */
    basic_matcher * m_current;

    /**
     * The error message (if any).
     */
    std::string m_error;

    /**
     * The main matcher object
     */
    std::pair< mode_constant, std::auto_ptr<basic_matcher> > m_main;

    /**
     * The result (no match if empty)
     */
    std::stringstream m_result;

    /**
     * The (possible) first match.
     */
    std::pair< mode_constant, std::auto_ptr<basic_matcher> > m_start;

    /**
     * The result from the start matcher.
     */
    std::string m_start_result;


  public:
    /**
     * An error message created when, for example, a method returns FAIL_FATAL.
     * Generally this is only called internally to generate exceptions.
     */
    std::string error_message() const
    {
      return m_error;
    }

    /**
     * Indicate the parser/filter subtype:
     * * LINE_ORIENTED: process the file line-by-line.
     * 
     * Indicate how this object will return results.
     * * XMLSTRING: Return the result as a string containing 
     * XML datum elements. The string is then interpreted and
     * inserted into the memo object be the manager.
     * * or: Can accept a memo object and insert the
     * results directly.
     */
    int flags() const
    {
      return CCHEM_SUPPLIER_LINE_ORIENTED | CCHEM_SUPPLIER_STRING_OUTPUT;
    }
    /**
     * Information that identifies the filter/parser. This will
     * be placed into log messages from the manager and not in
     * the database. A typical message might contain a
     * descriptive name, version and author, eg.
     * "Nibble filter for Froggles: version 0.001: 2020/12/25: by P.J. Teeps"
     */
    std::string identity() const;

  private:
    /**
     * Parse a_line of text and create a matcher. Throw an error if 
     * a_line is invalid.
     * 
     * type,mode,match,format
     * 
     * Where
     * type = ('byte'|'lines'|'string'|'regex')
     * match = (string|integer)
     * format = string // Only allowed for string or regex type.
     * mode = ('first'|'last'|'accum') // Only allowed when format exists.
     * 
     * Up to three lines may be present which have the following meaning
     * 
     * 1 line: The 'main' matcher, this is used to generate results so it must 
     * have a format string.
     * 2 lines: The first line represents a 'start' indicator. When it matches
     * it is immediately replaced with a 'main' matcher generated from the 
     * following line (they get the matching line/buffer). It can contribute to
     * the final result if it has a format string. The second line is then the
     * 'main' matcher and must have a format string.
     * 3 lines: As for 2 lines plus the third line is an 'end' indicator. When it
     * matches the regex_filter returns 'COMPLETE' and further processing
     * is not necessary. Note this 'end' indicator is tested after the 'main'
     * matcher.
     * 
     * The format string should not include the closing XML node ("</datum>")
     * this is automatically added.
     * 
     * Programming note.  It was decided that this method should be part of 
     * matcher_filter not basic_matcher because the major function is to
     * decode the filter's description, which is really independent of the 
     * matcher types.
     */
    basic_matcher * make_matcher(const std::string & a_line, mode_constant & a_mode);


  public:
    /**
     * Constructor. Note that processing the content of the filter description
     * is delayed until the first process_* call.
     * 
     * \param a_filter, The filter object from which to derive this matcher.
     */
    matcher_filter(const data_scheme::filter & a_filter);

    /**
     * Destructor, does nothing special.
     */
    ~matcher_filter();

    /**
     * This filter can only return a string, this always leads to an error.
     */
    int output_direct(data_scheme::memo & a_memo)
    {
      m_error = "Can not operate on file directly.";
      return CCHEM_SUPPLIER_BADCONFIG;
    }

    /**
     * Put the results into a_result string containing XML elements.
     * 
     * \pre a_result.empty ()
     */
    int output_string(std::string & a_result);


  private:
    /**
     * * Process filter desc into matcher filters.
     * 
     * \post start = nul or start.mode = 0 (first)
     * \post main /= nul
     */
    void parse_filter();


  public:
    /**
     * This is not usable for this filter subtype so always returns an 
     * error.
     */
    int process_file(const std::string& a_filename)
    {
      m_error = "Can not operate on file directly.";
      return CCHEM_SUPPLIER_BADCONFIG;
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
    int process_part(const std::string& a_buffer);

    /**
     * Reset the matcher to perform another search.
     * 
     * * clears m_result and sets m_current
     */
    void reset();


  private:
    /**
     * no copy
     */
    matcher_filter(const matcher_filter & source);

    /**
     * no copy
     */
    matcher_filter & operator=(const matcher_filter & source);

};

} // namespace supplier
#endif
