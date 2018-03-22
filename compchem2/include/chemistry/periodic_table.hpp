#ifndef COMPCHEM_CHEMISTRY_PERIODIC_TABLE_HPP
#define COMPCHEM_CHEMISTRY_PERIODIC_TABLE_HPP

// ------------------------------------------------------------
// class: periodic_table
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace chemistry {

/**
 * An object type to represent a chemistry element.
 */
class element {
  private:
    /**
     * The periodic number of element
     */
    unsigned int m_number;

    /**
     * Long name of element
     */
    std::string m_name;

    /**
     * Usual symbol for element
     */
    char m_symbol[4];


  public:
    element(unsigned int a_number, const std::string& a_name, const std::string& a_sym)
    : m_number (a_number)
    , m_name (a_name)
    , m_symbol ()
    {
      a_sym.copy (&m_symbol[0], 2);
      m_symbol [2] = '\0';
      m_symbol [3] = '\0';
    }

    element() throw ()
    : m_number (0)
    , m_name ()
    , m_symbol ()
    {
      m_symbol [0] = '\0';
      m_symbol [1] = '\0';
      m_symbol [2] = '\0';
      m_symbol [3] = '\0';
    }

    element(const element & source)
    : m_number (source.m_number)
    , m_name (source.m_name)
    , m_symbol ()
    {
      m_symbol [0] = source.m_symbol [0];
      m_symbol [1] = source.m_symbol [1];
      m_symbol [2] = source.m_symbol [2];
      m_symbol [3] = source.m_symbol [3];
    }

    ~element() throw() {}

    /**
     * get the atomic number
     */
    const std::string& name() const throw ()
    {
      return m_name;
    }

    /**
     * get the atomic number
     */
    unsigned int number() const throw ()
    {
      return m_number;
    }

    element & operator=(element source) throw ()
    {
      swap (source);
      return *this;
    }
    

    /**
     * Non-throwing swap
     */
    void swap(element & a_source) throw ()
    {
      std::swap (m_number, a_source.m_number);
      m_name.swap (a_source.m_name);
      std::swap (m_symbol[0], a_source.m_symbol[0]);
      std::swap (m_symbol[1], a_source.m_symbol[1]);
    }

    /**
     * get the atomic number
     */
    const char* const symbol() const throw ()
    {
      return &m_symbol[0];
    }

};
inline void swap (element &a, element &b) throw ()
{
 a.swap (b);
}
/**
 * A type to access simple information about the chemical periodic table.
 */
class periodic_table {
  private:
    /**
     * The index of the highest element in this periodic table (may not be highest element).
     */
    const unsigned int m_last_element;

    /**
     * The table of elements. Note that this is zero indexed so element 1 is at position 0, etc..
     */
    element m_elements[54];


  public:
    /**
     * Get an element in the table from its atomic number
     * 
     * \pre a_number > 0 && a_number <= get_last_element ()
     */
    const element& at(unsigned int a_number) const
    {
      COMPCHEM_REQUIRE (0 < a_number && a_number <= m_last_element, "No element at this position in this table.");
      return m_elements [a_number - 1];
    }


  private:
    /**
     * no copy
     */
    periodic_table & operator=(const periodic_table & source);

    /**
     * Private Singleton ctor
     */
    periodic_table();


  public:
    ~periodic_table() throw ()
    {}


  private:
    /**
     * no copy
     */
    periodic_table(const periodic_table & source);


  public:
    /**
     * Get the periodic table.
     */
    static const periodic_table& table();

    /**
     * The last element in this periodic table.  The periodic table may be truncated and not have elements up to atomic number 109.
     */
    unsigned int size() const throw ()
    {
      return m_last_element;
    }

};

} // namespace chemistry
namespace std
{
  template<>
  inline void swap<chemistry::element> (chemistry::element &a, chemistry::element &b) throw ()
  {
    a.swap (b);
  }
}

#endif
