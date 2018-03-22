#ifndef COMPCHEM_MARKUP_HPP
#define COMPCHEM_MARKUP_HPP

// ------------------------------------------------------------
// class: markup
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <boost/format.hpp>


namespace report {

namespace filetype {

/**
 * An abstract interface to inserting some markup into a stream. This
 * markup may be modified using place-holder substitution using
 * the bind methods. The clear method resets any bound values.
 */
class basic_markup
{
  public:
    virtual ~basic_markup() throw() {}

    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    virtual basic_markup & bind(unsigned int a_index, unsigned int a_value) = 0;
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    virtual basic_markup & bind(unsigned int a_index, const std::string & a_value) = 0;
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    virtual basic_markup & bind(unsigned int a_index, double a_value) = 0;
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    virtual basic_markup & bind(unsigned int a_index, int a_value) = 0;
    /**
     * Unset any bound values.
     */
    virtual void clear() = 0;

    /**
     * Insert our mark into a_os. Return a reference to this.
     */
    virtual basic_markup & mark(std::ostream & a_os) = 0;


};
/**
 * Use a boost format object as the markup.
 */
class string_markup : public basic_markup
{
  private:
    /**
     * The markup string.
     */
    std::string m_str;


  public:
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, unsigned int a_value)
    {
     return *this;
    }
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, const std::string & a_value)
    {
     return *this;
    }
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, double a_value)
    {
      return *this;
    }
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, int a_value)
    {
      return *this;
    }
    /**
     * Unset any bound values.
     */
    void clear()
    {}

    /**
     * Insert our mark into a_os. Return a reference to this.
     */
    basic_markup & mark(std::ostream & a_os)
    {
      a_os << m_str;
      return *this;
    }

    /**
     * Create an object from a_str. Swallows a_str!
     */
    string_markup(std::string & a_str) throw ()
    : m_str ()
    {
      m_str.swap (a_str);
    }

    string_markup(const string_markup & a_source)
    : m_str (a_source.m_str)
    {}

    virtual ~string_markup() throw() {}

    string_markup & operator=(const string_markup & source)
    {
      if (this != &source) m_str.assign (source.m_str);
      return *this;
    }


};
/**
 * Use a boost format as the markup. Note that the 
 * format string uses "1" based counting and the bind
 * functions are given "0" based indices.  Therefore
 * here the bind operations all add 1 to their index 
 * arguments before applying them to the format object.
 */
class format_markup : public basic_markup
{
  private:
    /**
     * The markup format object.
     */
    boost::format m_format;


  public:
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, unsigned int a_value)
    {
      m_format.bind_arg (a_index + 1, a_value);
      return *this;
    }
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, const std::string & a_value)
    {
      m_format.bind_arg (a_index + 1, a_value);
      return *this;
    }
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, double a_value)
    {
      m_format.bind_arg (a_index + 1, a_value);
      return *this;
    }
    /**
     * Bind a_value to the nth markup position. Returns
     * a reference to this.
     */
    basic_markup & bind(unsigned int a_index, int a_value)
    {
      m_format.bind_arg (a_index + 1, a_value);
      return *this;
    }
    /**
     * Unset any bound values.
     */
    void clear()
    {
      m_format.clear_binds ();
    }

    /**
     * Insert our mark into a_os. Return a reference to this.
     */
    basic_markup & mark(std::ostream & a_os)
    {
      a_os << m_format;
      return *this;
    }

    /**
     * Create an object from a_str. Swallows a_str!
     */
    format_markup(std::string & a_str) throw ()
    : m_format (a_str)
    {}

    format_markup(const format_markup & a_source)
    : m_format (a_source.m_format)
    {}

    virtual ~format_markup() throw() {}

    format_markup & operator=(const format_markup & a_source)
    {
      if (this != &a_source) m_format = a_source.m_format;
      return *this;
    }


};

} // namespace report::filetype

} // namespace report
#endif
