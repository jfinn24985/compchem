#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

// ------------------------------------------------------------
// class: spectrum
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "report/xy_spectrum.hpp"
#include "chemistry/spectrum_from_peaktable.hpp"
#include <vector>
#include "report/filetype/output_header.hpp"
#include "chemistry/vibration.hpp"
#include <boost/program_options/options_description.hpp>


// Manual includes
#include <boost/lexical_cast.hpp>
#include "report/filetype/output_header_options.hpp"
#include <algorithm>
// -
namespace report {

/**
 * Generate a spectrum from a peaktable.
 */
template<class t_value>
class spectrum : public xy_spectrum<t_value> {
  public:
    typedef t_value value_type;

    typedef chemistry::spectrum_from_peaktable<t_value> functor_t;

    typedef std::vector< std::pair< t_value, t_value > > data_vector_t;


  private:
    /**
     * The peaktable to spectrum functor.
     */
    functor_t m_converter;

    /**
     * The header section of the spectrum
     */
    report::filetype::output_header m_header;

    /**
     * The peak table as (freq.,intensity) pairs
     */
    data_vector_t m_peaktable;


  public:
    void add_peak(const chemistry::vibration & a_vib);

    /**
     * Add all peaks to spectrum. ~= std::for_each(a_begin, a_end, add_peak)
     */
    template < typename InputIterator > inline void add_peak_table(InputIterator a_begin, InputIterator a_end);

    /**
     * Reset the peak table.
     */
    void clear() { m_peaktable.clear (); }

    const functor_t& get_convert_options() const {
      return m_converter;
    }

    functor_t& get_convert_options() {
      return m_converter;
    }

    const report::filetype::output_header& get_header() const {
      return m_header;
    }

    report::filetype::output_header& get_header(){
      return m_header;
    }

    /**
     * Read the peak table from the istream, assuming the data is [ freq intn ] pairs.
     */
    void read_peak_table(std::istream & a_in);

    virtual void register_options(boost::program_options::options_description & a_opt_set);

    /**
     * Generate and output the spectrum to a_os
     */
    void serialise(std::ostream & a_os);

    spectrum();

    ~spectrum();

};
template<class t_value>
void spectrum<t_value>::add_peak(const chemistry::vibration & a_vib) {
  m_peaktable.push_back (std::make_pair (boost::lexical_cast< double >(a_vib.frequency ())
      , boost::lexical_cast< double >(a_vib.intensity ())));
}

/**
 * Add all peaks to spectrum. ~= std::for_each(a_begin, a_end, add_peak)
 */
template<class t_value>
template < typename InputIterator > inline void spectrum<t_value>::add_peak_table(InputIterator a_begin, InputIterator a_end) {
  std::for_each (a_begin, a_end, boost::bind (&spectrum::add_peak, this, _1));
}

/**
 * Read the peak table from the istream, assuming the data is [ freq intn ] pairs.
 */
template<class t_value>
void spectrum<t_value>::read_peak_table(std::istream & a_in) 
{
  // Ensure peaktable is empty 
  if (not m_peaktable.empty ())
  {
    clear ();
  }
  // Read in data
  while (a_in)
  {
    std::pair < value_type, value_type > l_pair;
    a_in >> l_pair.first >> l_pair.second;
    m_peaktable.push_back (l_pair);
  }
   

}

template<class t_value>
void spectrum<t_value>::register_options(boost::program_options::options_description & a_opt_set) {
  // Add options to a_opt_set
  xy_spectrum< value_type >::register_options (a_opt_set);
  m_converter.register_options (a_opt_set);
  report::filetype::output_header_options::register_options (a_opt_set, m_header);
}

/**
 * Generate and output the spectrum to a_os
 */
template<class t_value>
void spectrum<t_value>::serialise(std::ostream & a_os) {
  std::vector <std::pair < value_type, value_type > > l_spectrum (m_converter.size ());
  // Ensure peaktable is sorted properly
  std::stable_sort (m_peaktable.begin (), m_peaktable.end ());
  // Create the result
  m_converter (m_peaktable.begin (), m_peaktable.end (), l_spectrum.begin ());
  // Update the header with actual data.
  m_header.key ("FIRSTX", boost::lexical_cast< std::string >(m_converter.lower_bound ()));
  m_header.key ("LASTX", boost::lexical_cast< std::string >(m_converter.upper_bound ()));
  m_header.key ("NPOINTS", boost::lexical_cast< std::string >(m_converter.size ()));
  // Serialise output using xy_spectrum
  xy_spectrum< value_type >::serialise (a_os, m_header, l_spectrum);
}

template<class t_value>
spectrum<t_value>::spectrum()
  : m_converter ()
  , m_header ()
  , m_peaktable () {
    // Set the header options to useful defaults.
    m_header.title ("Simulated Spectrum");
    m_header.owner ("unknown");
    m_header.data_type ("Infrared Spectrum");
    m_header.key ("XUNITS", "1/CM");
    m_header.key ("YUNITS", "ABSORBANCE");
    m_header.key ("XFACTOR", "1");
    m_header.key ("YFACTOR", "1"); }


template<class t_value>
spectrum<t_value>::~spectrum() {
}


} // namespace report
#endif
