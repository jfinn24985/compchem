#ifndef COMPCHEM_CHEMISTRY_SPECTRUM_FROM_PEAKTABLE_HPP
#define COMPCHEM_CHEMISTRY_SPECTRUM_FROM_PEAKTABLE_HPP

// ------------------------------------------------------------
// class: spectrum_from_peaktable
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/program_options/options_description.hpp>


// Manual includes
#include <cmath>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/tuple/tuple.hpp>
// -
namespace chemistry {

    // Overload local_exp to get a type independent exponential
    // function.  Note this non-specialised version uses a
    // static_cast<double> on the value_type.
    template< class value_type >
    inline value_type local_exp(value_type a_value)
    {
      return exp (static_cast<double>(a_value));
    }
    
    inline double local_exp(double a_value)
    {
      return exp (a_value);
    }
    
    inline float local_exp(float a_value)
    {
      return exp (a_value);
    }
    
    inline long double local_exp(long double a_value)
    {
      return exp (a_value);
    }

/**
 * This type is used to generate a simulated spectrum from a peaktable.  It can
 * use either a Gaussian or a Lorentzian function to generate the spectrum.
 */
template<typename t_value>
class spectrum_from_peaktable {
  public:
    typedef t_value value_type;

    /**
     * The enumeration of generator types
     */
    enum generator_enum
    {
      LORENTZIAN= 0,/**
       * Use a Lorentzian generator function
       */

      GAUSSIAN= 1/**
       * Use a gaussian generator function
       */


    };


  private:
    value_type m_half_width;

    value_type m_increment;

    value_type m_lower_bound;

    value_type m_upper_bound;

    /**
     * Which generator function to use.
     */
    generator_enum m_generator;

    /**
     * This method converts the peaktable in the sequence (a_begin, a_end] into a spectrum saved starting at location a_destination.  All iterators must be convertible to std::pair< value_type, value_type >.  Exactly size () elements will be created and saved at a_destination.
     * 
     * \pre validate_input  (a_begin, a_end) and upper_bound /= lower_bound
     * 
     * Notes:
     * 
     * (1) This method will internally adjust increment such that (upper_bound - lower_bound)/increment is an integer value. ie increment_used ~= (upper_bound - lower_bound)/int((upper_bound - lower_bound)/increment) + 0.5). The output spectrum will thus have size() elements.  However, due to the limitations of floating point maths the last point may not exactly equal (or be less than!) upper_bound().  If this might be a problem the user can manually set the last point to upper_bound() after using the functor. 
     * 
     * (2) All simulated spectrum algorithms calculate the intensity at a particular point based on the intensity of nearby peaks in the peak table.  This makes the apparent peak intensity at the "top" of a peak heavily dependent on how close the location in the peak table is to a point in the spectrum.  To avoid this problem, the algorithm insures that the output value at the point closest to a location in the peak table will have intensity >= the intensity value from the peak table.
     * 
     */
    template <class ForwardIterator, class OutputIterator> inline void gaussian_generator(ForwardIterator a_begin, ForwardIterator a_end, OutputIterator a_destination) const;


  public:
    /**
     * Get the generator to use. Default is LORENTZIAN
     */
    generator_enum generator() const
    {
      return m_generator;
    }

    const value_type & half_width() const{  return m_half_width; }

    /**
     * Set the peak half-width
     * 
     * \pre a_width /= 0.0
     */
    void half_width(const value_type & value){
      REQUIRE (value > 0.0, "Require positive, non-zero half width");
      m_half_width = value;
    }

    const value_type & increment() const{  return m_increment; }

    /**
     * Set the distance between points of the digital spectrum
     * 
     * \pre a_inc /= 0.0
     */
    void increment(const value_type & value){
      REQUIRE (value > 0.0, "The increment must be a positive, non-zero value");
      m_increment = value;
    }


  private:
    /**
     * This method converts the peaktable in the sequence (a_begin, a_end] into a spectrum saved starting at location a_destination.  All iterators must be convertible to std::pair< value_type, value_type >.  Exactly size () elements will be created and saved at a_destination.
     * 
     * \pre validate_input  (a_begin, a_end) and upper_bound /= lower_bound
     * 
     * Notes:
     * 
     * (1) This method will internally adjust increment such that (upper_bound - lower_bound)/increment is an integer value. ie increment_used ~= (upper_bound - lower_bound)/int((upper_bound - lower_bound)/increment) + 0.5). The output spectrum will thus have size() elements.  However, due to the limitations of floating point maths the last point may not exactly equal (or be less than!) upper_bound().  If this might be a problem the user can manually set the last point to upper_bound() after using the functor. 
     * 
     * (2) All simulated spectrum algorithms calculate the intensity at a particular point based on the intensity of nearby peaks in the peak table.  This makes the apparent peak intensity at the "top" of a peak heavily dependent on how close the location in the peak table is to a point in the spectrum.  To avoid this problem, the algorithm insures that the output value at the point closest to a location in the peak table will have intensity >= the intensity value from the peak table.
     * 
     */
    template <class ForwardIterator, class OutputIterator> inline void lorentzian_generator(ForwardIterator a_begin, ForwardIterator a_end, OutputIterator a_destination) const;


  public:
    /**
     * The inclusive lower bound of a generated spectrum
     */
    const value_type & lower_bound() const{  return m_lower_bound; }

    /**
     * The inclusive lower bound of a generated spectrum
     */
    void lower_bound(const value_type & value) {  m_lower_bound = value; }


  private:
    /**
     * no copy
     */
    spectrum_from_peaktable<t_value> & operator=(const spectrum_from_peaktable<t_value> & source);


  public:
    /**
     * This method converts the peaktable in the sequence (a_begin, a_end] into a spectrum saved starting at location a_destination.  All iterators must be convertible to std::pair< value_type, value_type >.  Exactly size () elements will be created and saved at a_destination.
     * 
     * \pre validate_input  (a_begin, a_end) and upper_bound /= lower_bound
     * 
     * Notes:
     * 
     * (1) This method will internally adjust increment such that (upper_bound - lower_bound)/increment is an integer value. ie increment_used ~= (upper_bound - lower_bound)/int((upper_bound - lower_bound)/increment) + 0.5). The output spectrum will thus have size() elements.  However, due to the limitations of floating point maths the last point may not exactly equal (or be less than!) upper_bound().  If this might be a problem the user can manually set the last point to upper_bound() after using the functor. 
     * 
     * (2) All simulated spectrum algorithms calculate the intensity at a particular point based on the intensity of nearby peaks in the peak table.  This makes the apparent peak intensity at the "top" of a peak heavily dependent on how close the location in the peak table is to a point in the spectrum.  To avoid this problem, the algorithm insures that the output value at the point closest to a location in the peak table will have intensity >= the intensity value from the peak table.
     * 
     */
    template <class ForwardIterator, class OutputIterator> inline void operator()(ForwardIterator a_begin, ForwardIterator a_end, OutputIterator a_destination) const;

    /**
     * This adds all the configuration options for this object to the a_opt_set.
     */
    void register_options(boost::program_options::options_description & a_opt_set);

    /**
     * The number of data-points in the generated spectrum.  This equals (upper_bound - lower_bound)/increment + 1. For example a spectrum from 0.0 to 4000.0 at an increment of 1.0 has 4001 points.
     */
    size_t size() const { return abs (static_cast< long int > (nearbyint ((upper_bound () - lower_bound ())/increment ()))) + 1; }

    /**
     * Set the number of data-points in the generated spectrum.  This sets increment such that (upper_bound - lower_bound)/increment ~= a_count.  Therefore, if you are going to set the increment using size(x) then you must do it after setting the lower and upper bounds.
     * 
     * \pre a_count > 0 and upper_bound /= lower_bound
     * WARNING: Set upper and lower bounds before using this method.  
     */
    inline void size(const size_t & a_count) {
      REQUIRE (a_count > 0, "Number of points must be greater than zero");
      increment ((upper_bound () - lower_bound ())/value_type (a_count - 1));
    }

    spectrum_from_peaktable();


  private:
    /**
     * no copy
     */
    spectrum_from_peaktable(const spectrum_from_peaktable<t_value> & source);


  public:
    ~spectrum_from_peaktable() {}

    /**
     * The inclusive upper bound of a generated spectrum
     */
    const value_type & upper_bound() const{  return m_upper_bound; }

    /**
     * The inclusive upper bound of a generated spectrum
     */
    void upper_bound(const value_type & a_value){  m_upper_bound = a_value; }

    /**
     * Set the generator to use a Gaussian function.
     */
    void use_gaussian(bool a_value=true)
    {
      if (a_value) { m_generator = GAUSSIAN; }
    }
    

    /**
     * Set the generator to use a gaussian function.
     */
    void use_lorentzian(bool a_value=true)
    {
      if (a_value) { m_generator = LORENTZIAN; }
    }
    

    /**
     * Verify that the sequence represented by (a_begin, a_end] is in ascending wavenumber order.
     */
    template <class ForwardIterator> inline bool validate_input(ForwardIterator a_begin, ForwardIterator a_end) const;

};
/**
 * This method converts the peaktable in the sequence (a_begin, a_end] into a spectrum saved starting at location a_destination.  All iterators must be convertible to std::pair< value_type, value_type >.  Exactly size () elements will be created and saved at a_destination.
 * 
 * \pre validate_input  (a_begin, a_end) and upper_bound /= lower_bound
 * 
 * Notes:
 * 
 * (1) This method will internally adjust increment such that (upper_bound - lower_bound)/increment is an integer value. ie increment_used ~= (upper_bound - lower_bound)/int((upper_bound - lower_bound)/increment) + 0.5). The output spectrum will thus have size() elements.  However, due to the limitations of floating point maths the last point may not exactly equal (or be less than!) upper_bound().  If this might be a problem the user can manually set the last point to upper_bound() after using the functor. 
 * 
 * (2) All simulated spectrum algorithms calculate the intensity at a particular point based on the intensity of nearby peaks in the peak table.  This makes the apparent peak intensity at the "top" of a peak heavily dependent on how close the location in the peak table is to a point in the spectrum.  To avoid this problem, the algorithm insures that the output value at the point closest to a location in the peak table will have intensity >= the intensity value from the peak table.
 * 
 */
template<typename t_value>
template <class ForwardIterator, class OutputIterator> inline void spectrum_from_peaktable<t_value>::gaussian_generator(ForwardIterator a_begin, ForwardIterator a_end, OutputIterator a_destination) const {
  // PRE
  // Get working parameters
  // Ensure correct order of bounds and increase upper bound slightly in case of rounding
  const value_type l_upper (std::max (upper_bound (), lower_bound ()));
  const value_type l_lower_bound (std::min (upper_bound (), lower_bound ()));
  // TEST
  REQUIRE (l_lower_bound != l_upper, "Lower and Upper bounds must not be equal");
  // Peakwidth
  const value_type l_width (half_width ());
  // Working increment
  const value_type l_increment ((l_upper - l_lower_bound) / value_type (size () - 1));
  const value_type l_halfinc (l_increment / value_type (2.0));
  // Adjust working upper bound in case (size() - 1) * l_increment + l_lower_bound > l_upper
  // (which is possible for floating point maths.)
  const value_type l_upper_bound (l_upper + l_halfinc);
#ifdef LOCAL_DEBUG
  std::cout << "##UPPER= " << l_upper_bound << "\n";
  std::cout << "##LOWER= " << l_lower_bound << "\n";
  std::cout << "##INC=   " << increment () << "\n";
  std::cout << "##L INC= " << l_increment << "\n";
  std::cout << "##WIDTH= " << l_width << "\n";
  std::cout << "##SIZE=  " << size () << "\n";
#endif
  // Precompute inverse of divisor
  const value_type l_C (1 / (l_width * l_width * 2));
  
  // Compute output
  for (value_type l_location (lower_bound ()); l_location < l_upper_bound; l_location += l_increment, ++a_destination)
  {
    value_type l_maxpeak (0.0);
    value_type l_value (0.0);
    for (ForwardIterator l_begin = a_begin; l_begin != a_end; ++l_begin)
    {
      // Do gaussian
      //  f(x) = Ae-((x-B)^2/2C^2) 
      l_value += l_begin->second * local_exp (-1.0 * (l_location - l_begin->first) * (l_location - l_begin->first) * l_C);
  
      // Set max peak
      if (l_location - l_halfinc <= l_begin->first && l_begin->first <= l_location + l_halfinc)
      {
        l_maxpeak = std::max (l_maxpeak, l_begin->second);
      }
    }
    // Set output
    *a_destination = std::make_pair (l_location, std::max (l_maxpeak, l_value));
  }
}

/**
 * This method converts the peaktable in the sequence (a_begin, a_end] into a spectrum saved starting at location a_destination.  All iterators must be convertible to std::pair< value_type, value_type >.  Exactly size () elements will be created and saved at a_destination.
 * 
 * \pre validate_input  (a_begin, a_end) and upper_bound /= lower_bound
 * 
 * Notes:
 * 
 * (1) This method will internally adjust increment such that (upper_bound - lower_bound)/increment is an integer value. ie increment_used ~= (upper_bound - lower_bound)/int((upper_bound - lower_bound)/increment) + 0.5). The output spectrum will thus have size() elements.  However, due to the limitations of floating point maths the last point may not exactly equal (or be less than!) upper_bound().  If this might be a problem the user can manually set the last point to upper_bound() after using the functor. 
 * 
 * (2) All simulated spectrum algorithms calculate the intensity at a particular point based on the intensity of nearby peaks in the peak table.  This makes the apparent peak intensity at the "top" of a peak heavily dependent on how close the location in the peak table is to a point in the spectrum.  To avoid this problem, the algorithm insures that the output value at the point closest to a location in the peak table will have intensity >= the intensity value from the peak table.
 * 
 */
template<typename t_value>
template <class ForwardIterator, class OutputIterator> inline void spectrum_from_peaktable<t_value>::lorentzian_generator(ForwardIterator a_begin, ForwardIterator a_end, OutputIterator a_destination) const {
  // Get working parameters
  // Ensure correct order of bounds and increase upper bound slightly in case of rounding
  const value_type l_upper (std::max (upper_bound (), lower_bound ()));
  const value_type l_lower_bound (std::min (upper_bound (), lower_bound ()));
  // TEST
  REQUIRE (l_lower_bound != l_upper, "Lower and Upper bounds must not be equal");
  // Peakwidth
  const value_type l_width (half_width ());
  // Working increment
  const value_type l_increment ((l_upper - l_lower_bound) / value_type (size () - 1));
  const value_type l_halfinc (l_increment / value_type (2.0));
  // Adjust working upper bound in case (size() - 1) * l_increment + l_lower_bound > l_upper
  // (which is possible for floating point maths.)
  const value_type l_upper_bound (l_upper + l_halfinc);
#ifdef LOCAL_DEBUG
  std::cout << "##UPPER= " << l_upper_bound << "\n";
  std::cout << "##LOWER= " << l_lower_bound << "\n";
  std::cout << "##INC=   " << increment () << "\n";
  std::cout << "##L INC= " << l_increment << "\n";
  std::cout << "##WIDTH= " << l_width << "\n";
  std::cout << "##SIZE=  " << size () << "\n";
#endif
  // Precompute and store A0, A1
  std::vector < boost::tuple < value_type, value_type, value_type, value_type > >l_factor;
  for (ForwardIterator l_begin = a_begin; l_begin != a_end; ++l_begin)
  {
    const value_type l_factor_A1 (l_begin->second * l_width);
    const value_type l_factor_A0 (l_begin->second * l_factor_A1);
    l_factor.push_back (boost::make_tuple (l_begin->first, l_begin->second, l_factor_A0, l_factor_A1));
  }
  
  // Compute output (using precomputed values)
  for (value_type l_location (lower_bound ()); l_location < l_upper_bound; l_location += l_increment, ++a_destination)
  {
    value_type l_maxpeak (0.0);
    value_type l_value (0.0);
    for (typename std::vector < boost::tuple < value_type, value_type, value_type, value_type > >::const_iterator l_begin (l_factor.begin ()); l_begin != l_factor.end (); ++l_begin)
    {
      // Do lorentzian
      l_value += boost::get < 2 > (*l_begin) / (boost::get < 3 > (*l_begin) + (l_location - boost::get < 0 > (*l_begin)) * (l_location - boost::get < 0 > (*l_begin)));
  
      // Set max peak
      if (l_location - l_halfinc <= boost::get < 0 > (*l_begin) && boost::get < 0 > (*l_begin) <= l_location + l_halfinc)
      {
        l_maxpeak = std::max (l_maxpeak, boost::get < 1 > (*l_begin));
      }
    }
    // Set output
    *a_destination = std::make_pair (l_location, std::max (l_maxpeak, l_value));
  }
}

/**
 * This method converts the peaktable in the sequence (a_begin, a_end] into a spectrum saved starting at location a_destination.  All iterators must be convertible to std::pair< value_type, value_type >.  Exactly size () elements will be created and saved at a_destination.
 * 
 * \pre validate_input  (a_begin, a_end) and upper_bound /= lower_bound
 * 
 * Notes:
 * 
 * (1) This method will internally adjust increment such that (upper_bound - lower_bound)/increment is an integer value. ie increment_used ~= (upper_bound - lower_bound)/int((upper_bound - lower_bound)/increment) + 0.5). The output spectrum will thus have size() elements.  However, due to the limitations of floating point maths the last point may not exactly equal (or be less than!) upper_bound().  If this might be a problem the user can manually set the last point to upper_bound() after using the functor. 
 * 
 * (2) All simulated spectrum algorithms calculate the intensity at a particular point based on the intensity of nearby peaks in the peak table.  This makes the apparent peak intensity at the "top" of a peak heavily dependent on how close the location in the peak table is to a point in the spectrum.  To avoid this problem, the algorithm insures that the output value at the point closest to a location in the peak table will have intensity >= the intensity value from the peak table.
 * 
 */
template<typename t_value>
template <class ForwardIterator, class OutputIterator> inline void spectrum_from_peaktable<t_value>::operator()(ForwardIterator a_begin, ForwardIterator a_end, OutputIterator a_destination) const {
  // PRE
  REQUIRE (validate_input (a_begin, a_end), "Input must be in ascending wavenumber order");
  // Call generator
  switch (m_generator)
  {
  case LORENTZIAN:
    lorentzian_generator (a_begin, a_end, a_destination);
    break;
  case GAUSSIAN:
    gaussian_generator (a_begin, a_end, a_destination);
    break;
  }
}

/**
 * This adds all the configuration options for this object to the a_opt_set.
 */
template<typename t_value>
void spectrum_from_peaktable<t_value>::register_options(boost::program_options::options_description & a_opt_set) {
  // Add options to a_opt_set
  a_opt_set.add_options ()
    ("upper,u", boost::program_options::value< value_type > (&m_upper_bound), "upper bound of simulated spectrum")
    ("lower,l", boost::program_options::value< value_type > (&m_lower_bound), "lower bound of simulated spectrum")
    ("increment,i", boost::program_options::value< value_type > (&m_increment), "interval between points of simulated spectrum")
    ("gaussian,G", boost::program_options::bool_switch ()->notifier (boost::bind (&spectrum_from_peaktable::use_gaussian, this, _1)), "use a Gaussian function to generate the spectrum (do not use with --lorentzian)")
    ("lorentzian,L", boost::program_options::bool_switch ()->notifier (boost::bind (&spectrum_from_peaktable::use_lorentzian, this, _1)), "use a Lorentzian function to generate the spectrum (default, do not use with --gaussian)")
    ("size,s", boost::program_options::value< size_t > ()->notifier (boost::bind (&spectrum_from_peaktable::size, this, _1)), "total number of points in simulated spectrum")
    ("half-width,w", boost::program_options::value< value_type > (&m_half_width), "approx. peak half-width of simulated spectrum");
}

template<typename t_value>
spectrum_from_peaktable<t_value>::spectrum_from_peaktable()
  : m_half_width (1.0)
  , m_increment (0.1)
  , m_lower_bound (0.0)
  , m_upper_bound (4000.0)
  , m_generator (LORENTZIAN) {}


/**
 * Verify that the sequence represented by (a_begin, a_end] is in ascending wavenumber order.
 */
template<typename t_value>
template <class ForwardIterator> inline bool spectrum_from_peaktable<t_value>::validate_input(ForwardIterator a_begin, ForwardIterator a_end) const {
  ForwardIterator l_a (a_begin);
  ForwardIterator l_b (a_begin);
  if (l_b != a_end) // handle a_begin == a_end
  {  for (++l_b; l_b != a_end and l_b->first >= l_a->first; ++l_b, ++l_a)
    {
    }// No body
  }
  return (l_b == a_end);
}


} // namespace chemistry
#endif
