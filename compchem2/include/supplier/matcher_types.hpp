#ifndef COMPCHEM_MATCHER_TYPES_HPP
#define COMPCHEM_MATCHER_TYPES_HPP

// ------------------------------------------------------------
// class: matcher_types
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <memory>
#include <string>
#include <boost/regex.hpp>

#include <boost/format.hpp>


namespace supplier {

/**
 * A matcher object is used as a sub-part of the matcher_filter. This
 * enables a matcher_filter object to be composed of several steps. A
 * major assumption of this class heirarchy is that its members will 
 * be called in LINE_ORIENTED mode.
 * 
 */
class basic_matcher
{
  public:
    /**
     * Destructor. At the base level does nothing special.
     */
    virtual ~basic_matcher() {}

    /**
     * Clone self.
     * 
     * \return not result.get = nul
     */
    virtual std::auto_ptr< basic_matcher > clone() const = 0;

    /**
     * Perform a match against a_target, return true if
     * a match is made. Throw an exception on error. 
     * 
     * \return true on process event, may set a_result
     *   false --> BEGIN(a_result) == END(a_result)
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result) = 0;

    /**
     * Reinitialise the matcher
     */
    virtual void reset() {}


};
/**
 * base class for matcher_bytes and matcher_lines.
 */
class matcher_bytes : public basic_matcher
{
  public:
    /**
     * This is the current total number of bytes.
     */
    boost::uint64_t count;

    /**
     * The target offset.
     */
    const boost::uint64_t offset;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_bytes (*this));
    }

    /**
     * Perform a match against a_target.  If there is a match return true 
     * and set a_result.  This is a pure virtual method that will provided by 
     * derived classes.
     * 
     * \return Did a match occur?
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * throw: strong guarrantee
     */
    matcher_bytes(boost::uint64_t a_count)
    : count (0)
    , offset (a_count)
    {}

    /**
     * Base class virtual destructor, does nothing special.
     */
    virtual ~matcher_bytes() throw()
    {}

    matcher_bytes(const matcher_bytes & a_source)
    : count (a_source.count)
    , offset (a_source.offset)
    {}
    

    matcher_bytes & operator=(const matcher_bytes & a_source)
    {
      if (&a_source != this)
      {
        count = a_source.count;
        const_cast< uint64_t& > (offset) = a_source.offset;
      }
      return *this;
    }

    /**
     * Reinitialise the matcher
     */
    void reset()
    {
      count = 0;
    }

    void swap(matcher_bytes & a_source)
    {
      if (&a_source != this)
      {
        std::swap (count, a_source.count);
        std::swap (const_cast< uint64_t& > (offset), const_cast< uint64_t& > (a_source.offset));
      }
    }


};
/**
 * When searching in a file it is useful to define the possible start and end point
 * of a search.  This class provides the interface for managing a start/end point
 * as a fixed number of lines.
 */
class matcher_lines : public basic_matcher
{
  public:
    /**
     * This is the current total number of lines.
     */
    unsigned int count;

    /**
     * The target offset.
     */
    const unsigned int offset;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_lines (*this));
    }

    /**
     * Perform a match against a_target.  If there is a match return true .
     * 
     * \return Did a match occur?
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result);

    matcher_lines(unsigned int a_count)
    : count ()
    , offset (a_count)
    {}

    /**
     * Base class virtual destructor, does nothing special.
     */
    virtual ~matcher_lines() throw()
    {}

    matcher_lines(const matcher_lines & a_source)
    : count (a_source.count)
    , offset (a_source.offset)
    {}

    matcher_lines & operator=(const matcher_lines & a_source)
    {
      if (&a_source != this)
      {
        count = a_source.count;
        const_cast< unsigned int& > (offset) = a_source.offset;
      }
      return *this;
    }
    /**
     * Reinitialise the matcher
     */
    void reset()
    {
      count = 0;
    }

    void swap(matcher_lines & source)
    {
      if (&source != this)
      {
        std::swap (count, source.count);
        std::swap (const_cast< unsigned int& > (offset), const_cast< unsigned int& > (source.offset));
      }
    }


};
/**
 * Combine two matchers for use-case: accumulate data 
 * from 'main' until 'end' matches. Therefore main matcher 
 * is an accumulating matcher and the end matcher is a 
 * search scanner. 
 * 
 * * any results from end are ignored, 
 * * accumulating results from main are stored locally. 
 * * should be treated as a 'first' matcher.
 */
class matcher_end_pair : public basic_matcher
{
  public:
    /**
     * The accumulated results.
     */
    std::string accumulator;

    /**
     * The end matcher
     */
    std::auto_ptr<basic_matcher> end;

    /**
     * The main matcher
     */
    std::auto_ptr<basic_matcher> main;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_end_pair (*this));
    }

    /**
     * Equivalent to:
     * if main.process_part --> save result
     * if end.process_part --> set a_result to saved.
     * 
     * Supported use-case: accumulate data from 'main' until 'end'
     * matches.
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * Combine a pair or matchers.
     * 
     * \pre a_main /= nul, a_end /= nul
     */
    matcher_end_pair(basic_matcher * a_main, basic_matcher * a_end);

    ~matcher_end_pair();

    matcher_end_pair(const matcher_end_pair & a_source)
    : accumulator (a_source.accumulator)
    , end (a_source.end->clone ())
    , main (a_source.main->clone ())
    {}

    matcher_end_pair & operator=(const matcher_end_pair & a_source)
    {
      if (&a_source != this)
      {
        accumulator = a_source.accumulator;
        end = a_source.end->clone ();
        main = a_source.main->clone ();
      }
      return *this;
    }

    /**
     * Reinitialise the matcher
     */
    void reset()
    {
      accumulator.clear ();
      end.get ()->reset ();
      main.get ()->reset ();
    }

    void swap(matcher_end_pair & a_source)
    {
      if (&a_source != this)
      {
        accumulator.swap (a_source.accumulator);
        basic_matcher * l_temp (end.release ());
        end.reset (a_source.end.release ());
        a_source.end.reset (l_temp);
        l_temp = main.release ();
        main.reset (a_source.main.release ());
        a_source.main.reset (l_temp);
      }
    }


};
/**
 * This type will use a regular expression to inspect buffers.  On
 * a successful match a boost::regex 'format' string is used to 
 * construct a result. (Note this format string is not the same as
 * for matcher_string)
 * 
 * TODO: mode!
 */
class matcher_regex : public basic_matcher {
  public:
    /**
     * The regular expression.
     */
    const boost::regex finder;

    /**
     * The result format string.  The system uses boost::regex (std::tr1::regex?)
     * library and this string is the format string passed to a boost::regex_match
     * object. It should produce a string containing valid datum XML element(s).
     */
    const std::string format;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_regex (*this));
    }

    /**
     * Perform a match against a_buf (of a_sz).  This buffer is a_off from the beginning
     * of the file. If a match is made against the buffer the format string is used to 
     * extract text into a_result.
     * 
     * \return true is a match occurred.
     */
    bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * Construct a regular expression matcher. If a_line is true no flags
     * are set by default, if false then a default set of flags for buffer oriented
     * searches is used..
     * 
     * \param a_rgx:  The regular expression in Boost Perl-like syntax (see boost::regex)
     * \param a_lbl: a 'sed' like replacement string where \1, \2 etc represent 
     *     the sub-matches from the regular expression. (see format() in boost::regex_match)
     * 
     * \pre not a_rgx.empty and not a_lbl.empty
     * \pre a_rgx is a valid regular expression.
     */
    matcher_regex(const std::string& a_rgx, const std::string& a_lbl);

    /**
     * Construct a regular expression matcher with a set of regular expression flags.
     * 
     * \param a_rgx:  The regular expression in Boost Perl-like syntax (see boost::regex)
     * \param a_lbl: a 'sed' like replacement string where \1, \2 etc represent 
     *     the sub-matches from the regular expression. (see format() in boost::regex_match)
     * 
     * \pre not a_rgx.empty and not a_lbl.empty
     * \pre a_rgx is a valid regular expression.
     */
    matcher_regex(const std::string& a_rgx, unsigned int a_regex_flags, const std::string& a_lbl);

    /**
     * Destructor, does nothing special.
     */
    ~matcher_regex() {}

    matcher_regex(const matcher_regex & a_source)
    : finder (a_source.finder)
    , format (a_source.format)
    {}

    matcher_regex & operator=(const matcher_regex & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder and format reflect the object's property not
        // the object, therefore it is valid to cast off constness here.
        const_cast< boost::regex& > (finder) = a_source.finder;
        const_cast< std::string& > (format) = a_source.format;
      }
      return *this;
    }

    void swap(matcher_regex & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder/format is not part of object constness
        const_cast < boost::regex & > (finder).swap (const_cast< boost::regex & >(a_source.finder));
        const_cast < std::string & > (format).swap (const_cast < std::string & > (a_source.format));
      }
    }

};
/**
 * This type will use a regular expression to inspect buffers.  However
 * it can not produce results, only the fact the match occurs is
 * relevant.
 */
class matcher_regex_search : public basic_matcher {
  public:
    /**
     * The regular expression.
     */
    const boost::regex finder;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_regex_search (*this));
    }

    /**
     * Perform a match against a_buf (of a_sz).  This buffer is a_off from the beginning
     * of the file. If a match is made against the buffer the format string is used to 
     * extract text into a_result.
     * 
     * \return true is a match occurred.
     */
    bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * Construct a regular expression matcher. If a_line is true no flags
     * are set by default, if false then a default set of flags for buffer oriented
     * searches is used..
     * 
     * \param a_rgx:  The regular expression in POSIX syntax (see boost::regex)
     * \param a_lbl: a 'sed' like replacement string where \1, \2 etc represent 
     *     the sub-matches from the regular expression. (see format() in boost::regex_match)
     * 
     * \pre not a_rgx.empty and not a_lbl.empty
     * \pre a_rgx is a valid regular expression.
     */
    matcher_regex_search(const std::string& a_rgx);

    /**
     * Destructor, does nothing special.
     */
    ~matcher_regex_search() {}

    matcher_regex_search(const matcher_regex_search & a_source)
    : finder (a_source.finder)
    {}

    matcher_regex_search & operator=(const matcher_regex_search & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder reflects the object's property not
        // the object, therefore it is valid to cast off constness here.
        const_cast< boost::regex& > (finder) = a_source.finder;
      }
      return *this;
    }

    void swap(matcher_regex_search & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder/format is not part of object constness
        const_cast < boost::regex & > (finder).swap (const_cast< boost::regex & >(a_source.finder));
      }
    }

};
/**
 * Match a simple substring
 * 
 * This has two attributes.
 * * A 'phrase' that is matched directly with the target.
 * * A 'format' that is a boost::format string for processing the match. The format
 * is applied like printf (format, a_target, phrase) where format can take positional
 * arguments to use either a_target or phrase or neither.
 * 
 * TODO: mode!
 */
class matcher_string : public basic_matcher {
  public:
    /**
     * The phrase to match
     */
    const std::string phrase;

    /**
     * A boost format string that is used to build a datum XML element.
     */
    const std::string format;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_string (*this));
    }

    /**
     * Perform a match against a_buf (of a_sz).  This buffer is a_off from the beginning
     * of the file. If m_phrase is found in the buffer then a_result contains m_label if 
     * m_label is not empty, otherwise it contains m_phrase.
     * 
     * \result true if match occurred
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * Default ctor. Searches for literal text of a_phr.  If a match 
     * occurs a_lbl is used to format the result.
     * 
     * \param a_phr: what to search for
     * \param a_lbl: how to output result
     * \param a_flagset: operation mode (first (default), last or accumulating)
     * \pre not a_phr.empty and not a_lbl.empty
     *  (note a_lbl has a non-empty default)
     */
    matcher_string(const std::string & a_phr, const std::string & a_lbl = "<datum outcome=\"unknown\">%1%</datum>")
    : phrase (a_phr)
    , format (a_lbl)
    {
      COMPCHEM_REQUIRE (!a_phr.empty (), "Can not match an empty string!");
      COMPCHEM_REQUIRE (!a_lbl.empty (), "Can not format using an empty string!");
    }

    /**
     * Destructor, does nothing special.
     */
    ~matcher_string() {}

    matcher_string(const matcher_string & a_source)
    : phrase (a_source.phrase)
    , format (a_source.format)
    {}

    matcher_string & operator=(const matcher_string & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder and format reflect the object's property not
        // the object, therefore it is valid to cast off constness here.
        const_cast< std::string& > (phrase) = a_source.phrase;
        const_cast< std::string& > (format) = a_source.format;
      }
      return *this;
    }

    void swap(matcher_string & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder/format is not part of object constness
        const_cast < std::string & > (phrase).swap (const_cast < std::string & > (a_source.phrase));
        const_cast < std::string & > (format).swap (const_cast < std::string & > (a_source.format));
      }
    }

};
/**
 * Search a simple substring and return COMPLETE when found.
 * 
 * This has two attributes.
 * * A 'phrase' that is matched directly with the target.
 */
class matcher_string_search : public basic_matcher {
  public:
    /**
     * The phrase to match
     */
    const std::string phrase;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_string_search (*this));
    }

    /**
     * Perform a match against a_target.  Return COMPLETE on a match
     * 
     * \result SUCCESS on no match, COMPLETE on match.
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * Default ctor. Searches for literal text of a_phr.  If a match 
     * occurs a_lbl is used to format the result.
     * 
     * \pre not a_phr.empty and not a_lbl.empty
     *  (note a_lbl has a non-empty default)
     */
    matcher_string_search(const std::string & a_phr)
    : phrase (a_phr)
    {
      COMPCHEM_REQUIRE (!a_phr.empty (), "Can not match an empty string!");
    }

    /**
     * Destructor, does nothing special.
     */
    ~matcher_string_search() {}

    matcher_string_search(const matcher_string_search & a_source)
    : phrase (a_source.phrase)
    {}

    matcher_string_search & operator=(const matcher_string_search & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder and format reflect the object's property not
        // the object, therefore it is valid to cast off constness here.
        const_cast< std::string& > (phrase) = a_source.phrase;
      }
      return *this;
    }

    void swap(matcher_string_search & a_source)
    {
      if (&a_source != this)
      {
        // constness of finder/format is not part of object constness
        const_cast < std::string & > (phrase).swap (const_cast < std::string & > (a_source.phrase));
      }
    }

};
/**
 * Combine two matchers for use-case: scan document 
 * until 'start' matches then switch to 'main'.
 * 
 * * any results from start are ignored.
 * * should be treated as a 'first' matcher.
 */
class matcher_start_pair : public basic_matcher
{
  public:
    /**
     * Has start matched yet?
     */
    bool in_start;

    /**
     * The main matcher
     */
    std::auto_ptr<basic_matcher> main;

    /**
     * The end matcher
     */
    std::auto_ptr<basic_matcher> start;

    /**
     * Clone self.
     */
    virtual std::auto_ptr< basic_matcher > clone() const
    {
      return std::auto_ptr< basic_matcher > (new matcher_start_pair (*this));
    }

    /**
     * Equivalent to:
     * if main.process_part --> save result
     * if end.process_part --> set a_result to saved.
     * 
     * Supported use-case: accumulate data from 'main' until 'end'
     * matches.
     */
    virtual bool process_part(const std::string& a_target, std::string & a_result);

    /**
     * Combine a pair or matchers.
     * 
     * \pre a_main /= nul, a_end /= nul
     */
    matcher_start_pair(basic_matcher * a_start, basic_matcher * a_main);

    ~matcher_start_pair();

    matcher_start_pair(const matcher_start_pair & a_source)
    : in_start (a_source.in_start)
    , main (a_source.main->clone ())
    , start (a_source.start->clone ())
    {}

    matcher_start_pair & operator=(const matcher_start_pair & a_source)
    {
      if (&a_source != this)
      {
        in_start = a_source.in_start;
        main = a_source.main->clone ();
        start = a_source.start->clone ();
      }
      return *this;
    }

    /**
     * Reinitialise the matcher
     */
    void reset()
    {
      in_start = true;
      main.get ()->reset ();
      start.get ()->reset ();
    }

    void swap(matcher_start_pair & a_source)
    {
      if (&a_source != this)
      {
        std::swap (in_start, a_source.in_start);
        basic_matcher * l_temp = main.release ();
        main.reset (a_source.main.release ());
        a_source.main.reset (l_temp);
        l_temp = start.release ();
        start.reset (a_source.start.release ());
        a_source.start.reset (l_temp);
      }
    }


};

} // namespace supplier
#endif
