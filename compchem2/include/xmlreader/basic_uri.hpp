#ifndef PURPLETREE_BASIC_URI_HPP
#define PURPLETREE_BASIC_URI_HPP

// ------------------------------------------------------------
// class: basic_uri
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <memory>

namespace xmlreader {

/**
 * Base class for URLs
 * 
 * [scheme]://[user]:[authority]@[server]:[port]/[path][#[fragment]|?[query]]
 */
template<class T_traits>
class basic_uri
{
  public:
    /**
     * Traits type for this instantiation
     */
    typedef T_traits traits_type;

    /**
     * Pointer type of rep object.
     */
    typedef typename T_traits::rep_type rep_type;


  private:
    rep_type m_rep;


  public:
    std::string authority() const
    {
      return traits_type::authority (m_rep);
    }

    /**
     * Create empty URI
     * 
     * \post empty
     */
    basic_uri()
    : m_rep (0)
    {}

    /**
     * Attempt to create a URI for a_uri string.  Will be empty on failure
     * 
     * \pre valid_uri (a_uri)
     * \post empty -> bad URI
     */
    basic_uri(const std::string& a_url)
    : m_rep (traits_type::parse (a_url))
    {
      COMPCHEM_ALWAYS(rep_type (0) != m_rep, "Failed to parse URI object or out-of-memory");
    }

    ~basic_uri()
    {
      reset ();
    }

    basic_uri(const basic_uri<T_traits> & source)
    : m_rep (rep_type (0) == source.m_rep ? 0 : traits_type::copy (source.m_rep))
    {}

    /**
     * Does this contain a valid URL?
     */
    bool empty() const
    {
      return m_rep == rep_type (0);
    }

    std::string fragment() const
    {
      return traits_type::fragment (m_rep);
    }

    basic_uri<T_traits> & operator=(basic_uri<T_traits> source)
    {
      swap (source);
      return *this;
    }

    std::string path() const
    {
      return traits_type::path (m_rep);
    }

    /**
     * Return the port number, a value of 0 indicates no port.
     */
    int port() const
    {
      return traits_type::port (m_rep);
    }

    std::string query() const
    {
      return traits_type::query (m_rep);
    }


  private:
    /**
     * Delete the rep object if non-nul
     */
    void reset()
    {
      if (m_rep)
      {
        traits_type::reset (m_rep);
        COMPCHEM_ENSURE(m_rep == rep_type (0), "Traits reset method failed to set pointer to nul");
      }
    }

  public:
    std::string scheme() const
    {
      return traits_type::scheme (m_rep);
    }

    std::string server() const
    {
      return traits_type::server (m_rep);
    }

    void swap(basic_uri<T_traits> & a_source)
    {
      rep_type l_tmp (m_rep);
      m_rep = a_source.m_rep;
      a_source.m_rep = l_tmp;
    }

    std::string user() const
    {
      return traits_type::user (m_rep);
    }

    /**
     * Test if a_uri is a vlaid (parsable) URI.
     */
    static bool valid_uri(const std::string& a_uri)
    {
      return not a_uri.empty () and traits_type::is_valid (a_uri);
    }

    /**
     * Attempt to make a URI object from a_uri string. Returns
     * nul object on failure.
     */
    static std::auto_ptr< basic_uri<T_traits> > make_uri(const std::string& a_uri)
    {
      rep_type l_trial (traits_type::parse (a_uri));
      std::auto_ptr< basic_uri<T_traits> > Result;
      if (NULL != l_trial)
      {
        Result.reset (new basic_uri<T_traits>);
        Result.get ()->m_rep = l_trial;
      }
      return Result;
    }

};

} // namespace xmlreader
#endif
