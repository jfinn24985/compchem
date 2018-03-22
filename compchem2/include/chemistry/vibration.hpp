#ifndef COMPCHEM_CHEMISTRY_VIBRATION_HPP
#define COMPCHEM_CHEMISTRY_VIBRATION_HPP

// ------------------------------------------------------------
// class: vibration
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <sstream>
#include <string>

namespace chemistry {

/**
 * Simple structure to contain the data from a single row of vibration table.
 */
class vibration {
  public:
    /**
     * Set of flags that control the output of the object. Not there is no NO_FREQ flag as it is always assumed that this will be output.
     */
    enum flags {
      NO_ORDINAL= 1,
      NO_DEPOLAR= 2,
      NO_FORCE= 4,
      NO_INTENSITY= 8,
      NO_MODE= 16,
      NO_RAMAN= 32,
      NO_REDUCED_MASS= 64,
      NO_DISPLACEMENT_VEC= 128,
      PEAK_TABLE= NO_ORDINAL | NO_DEPOLAR | NO_FORCE | NO_MODE | NO_RAMAN | NO_REDUCED_MASS | NO_DISPLACEMENT_VEC /**
       * A mask for just outputting the frequency and intensity values.
       */


    };

    std::string m_ordinal;

    std::string m_depolar;

    std::string m_force;

    std::string m_frequency;

    std::string m_intensity;

    std::string m_mode;

    std::string m_raman;

    std::string m_redms;

    std::string m_dispvec;

    inline vibration();

    ~vibration() throw () {}

    inline vibration(const vibration & source);

    vibration & operator=(vibration source) throw ()
    {
      swap (source);
      return *this;
    }

friend std::ostream& operator<< (std::ostream&, const vibration&);

    const std::string & depolar() const throw ()
    {
      return m_depolar;
    }

    void depolar(std::string value) throw ()
    {
      m_depolar.swap (value);
    }

    const std::string& displacement_vector() const throw ()
    {
      return m_dispvec;
    }

    /**
     * Add to displacement vector,
     */
    void displacement_vector_append(const std::string& a_str)
    {
      m_dispvec.append (a_str);
    }

    /**
     * Reset the displacement vector to an empty string
     */
    void displacement_vector_reset(std::string a_str =std::string())
    {
      m_dispvec.swap (a_str);
    }
    

    const std::string & force() const throw ()
    {
      return m_force;
    }

    void force(std::string value) throw ()
    {
      m_force.swap (value);
    }
    const std::string & frequency() const throw ()
    {
      return m_frequency;
    }

    void frequency(std::string value) throw ()
    {
      m_frequency.swap (value);
    }

    const std::string & intensity() const throw ()
    {
      return m_intensity;
    }

    void intensity(std::string value)
    {
      m_intensity.swap (value);
    }

    const std::string & mode() const throw ()
    {
      return m_mode;
    }

    void mode(std::string value)
    {
      m_mode.swap (value);
    }

    const std::string & raman() const throw ()
    {
      return m_raman;
    }

    void raman(std::string value) throw()
    {
      m_raman.swap (value);
    }

    const std::string & ordinal() const throw ()
    {
      return m_ordinal;
    }

    void ordinal(std::string value) throw ()
    {
      m_ordinal.swap (value);
    }

    const std::string & reduced_mass() const throw ()
    {
      return m_redms;
    }

    void reduced_mass(std::string value) throw ()
    {
      m_redms.swap (value);
    }

    /**
     * Output the vibration.  This determines what to output from a_flags and separates the items with a _delim.
     */
    void serialise(std::ostream & a_os, std::string a_delim=std::string(", "), unsigned char a_flags=NO_DISPLACEMENT_VEC) const;

    /**
     * Output the vibration.  This determines what to output from a_flags and separates the items with a _delim.
     */
    static void serialise_header(std::ostream & a_os, std::string a_delim=std::string(", "), unsigned char a_flags=NO_DISPLACEMENT_VEC);

    /**
     * Non-throwing swap
     */
    void swap(vibration & source) throw ()
    {
      m_ordinal.swap (source.m_ordinal);
      m_depolar.swap (source.m_depolar);
      m_force.swap (source.m_force);
      m_frequency.swap (source.m_frequency);
      m_intensity.swap (source.m_intensity);
      m_mode.swap (source.m_mode);
      m_raman.swap (source.m_raman);
      m_redms.swap (source.m_redms);
      m_dispvec.swap (source.m_dispvec);
    }
};
inline vibration::vibration() 
  : m_ordinal ()
  , m_depolar ()
  , m_force ()
  , m_frequency ()
  , m_intensity ()
  , m_mode ()
  , m_raman ()
  , m_redms ()
  , m_dispvec () {}


inline vibration::vibration(const vibration & source) 
: m_ordinal (source.m_ordinal)
, m_depolar (source.m_depolar)
, m_force (source.m_force)
, m_frequency (source.m_frequency)
, m_intensity (source.m_intensity)
, m_mode (source.m_mode)
, m_raman (source.m_raman)
, m_redms (source.m_redms )
, m_dispvec (source.m_dispvec)
{}


inline void swap (vibration &a, vibration &b) throw ()
{
 a.swap (b);
}

} // namespace chemistry
namespace std
{
  template<>
  inline void swap<chemistry::vibration> (chemistry::vibration &a, chemistry::vibration &b) throw ()
  {
    a.swap (b);
  }
}
#endif
