// ------------------------------------------------------------
// class: vibration
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#define NO_LOG
#include "chemistry/vibration.hpp"

namespace chemistry {

std::ostream& operator<< (std::ostream& a_os, const chemistry::vibration& a_vib)
{
  a_os << "Ordinal:    [" << a_vib.ordinal() << "]\n";
  a_os << "Depolar:    [" << a_vib.depolar() << "]\n";
  a_os << "Force:      [" << a_vib.force() << "]\n";
  a_os << "Frequency:  [" << a_vib.frequency() << "]\n";
  a_os << "Intensity:  [" << a_vib.intensity() << "]\n";
  a_os << "Mode:       [" << a_vib.mode() << "]\n";
  a_os << "Raman Act:  [" << a_vib.raman() << "]\n";
  a_os << "Red. Mass:  [" << a_vib.reduced_mass() << "]\n";
  a_os << "Disp. Vec.: [" << a_vib.displacement_vector () << "]\n";
  return a_os;
}

/**
 * Output the vibration.  This determines what to output from a_flags and separates the items with a _delim.
 */
void vibration::serialise(std::ostream & a_os, std::string a_delim, unsigned char a_flags) const 
{
  class helper
  {
  public:
    // Helper function to output the values.
    static inline void output (std::ostream & A_os, const std::string & A_delim, const unsigned char A_flags, const std::string & A_value, const unsigned char A_flag_select, const unsigned char A_flag_follow_mask)
    {
      DLOG ("FLAG[");
      DLOG (int (A_flag_select));
        DLOG ("][");
        DLOG ((int) (~A_flags & A_flag_follow_mask));
        DLOG ("]: ");
        DLOG (A_value);
        DLOG ("\n");
      if (not (A_flags & A_flag_select))
      {
        if (not A_value.empty ())
        {
          A_os << A_value;
        }
        if (~A_flags & A_flag_follow_mask)
        {
          A_os << A_delim;
        }
      }
    }
  };
  unsigned int l_mask (255);
  l_mask ^= NO_ORDINAL;
  helper::output (a_os, a_delim, a_flags, ordinal (), NO_ORDINAL, l_mask);
  helper::output (a_os, a_delim, a_flags, frequency (), 0, l_mask);
  l_mask ^= NO_INTENSITY;
  helper::output (a_os, a_delim, a_flags, intensity (), NO_INTENSITY, l_mask);
  l_mask ^= NO_RAMAN;
  helper::output (a_os, a_delim, a_flags, raman (), NO_RAMAN, l_mask);
  l_mask ^= NO_FORCE;
  helper::output (a_os, a_delim, a_flags, force (), NO_FORCE, l_mask);
  l_mask ^= NO_DEPOLAR;
  helper::output (a_os, a_delim, a_flags, depolar (), NO_DEPOLAR, l_mask);
  l_mask ^= NO_MODE;
  helper::output (a_os, a_delim, a_flags, mode (), NO_MODE, l_mask);
  l_mask ^= NO_REDUCED_MASS;
  helper::output (a_os, a_delim, a_flags, reduced_mass (), NO_REDUCED_MASS, l_mask);
  helper::output (a_os, a_delim, a_flags, displacement_vector (), NO_DISPLACEMENT_VEC, 0);
  a_os << "\n";

}

/**
 * Output the vibration.  This determines what to output from a_flags and separates the items with a _delim.
 */
void vibration::serialise_header(std::ostream & a_os, std::string a_delim, unsigned char a_flags)

{
  class helper
  {
  public:
    // Helper function to output the values.
    static inline void output (std::ostream & A_os, const std::string & A_delim, const unsigned char A_flags, const std::string & A_value, const unsigned char A_flag_select)
    {
      if (not A_flags & A_flag_select)
      {
        A_os << A_value;
        if (not A_flags & !(unsigned char) ((A_flag_select >> 1) - 1))
        {
          A_os << A_delim;
        }
      }
  }};
  
  helper::output (a_os, a_delim, a_flags, "Ordinal", NO_ORDINAL);
  helper::output (a_os, a_delim, a_flags, "Frequency", 0);
  helper::output (a_os, a_delim, a_flags, "Intensity", NO_INTENSITY);
  helper::output (a_os, a_delim, a_flags, "Raman Activity", NO_RAMAN);
  helper::output (a_os, a_delim, a_flags, "Force Constants", NO_FORCE);
  helper::output (a_os, a_delim, a_flags, "Depolar", NO_DEPOLAR);
  helper::output (a_os, a_delim, a_flags, "Mode", NO_MODE);
  helper::output (a_os, a_delim, a_flags, "Reduced Mass", NO_REDUCED_MASS);
  helper::output (a_os, a_delim, a_flags, "Displacement Vector", NO_DISPLACEMENT_VEC);

}


} // namespace chemistry
