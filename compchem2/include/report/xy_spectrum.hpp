#ifndef XY_SPECTRUM_HPP
#define XY_SPECTRUM_HPP

// ------------------------------------------------------------
// class: xy_spectrum
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "report/filetype/jcampdx_decor.hpp"
#include "report/filetype/csv_decor.hpp"
#include "report/filetype/text_decor.hpp"
#include "report/filetype/base_decor.hpp"
#include <string>
#include <boost/program_options/options_description.hpp>

#include "report/filetype/output_header.hpp"

// Manual includes
#include <boost/format.hpp>
#include <boost/bind.hpp>
// - 
namespace report {

template<class t_value>
class xy_spectrum {
  public:
    typedef t_value value_type;

    enum output_type {
      TEXT= 0,/**
       * Output as text,  the header line are preceded by a single '#' and data as two columns of numbers.
       */

      JCAMPDX= 1,/**
       * Output as JCAMP/DX
       */

      CSV/**
       * Output as Comma Separated Value file. Header is separated from data by a blank line.
       */


    };


  private:
    /**
     * The type of output. This should equal one of the output_type enum values.  In practise if it is not JCAMPDX or CSV then TEXT output is used.
     */
    output_type m_output_type;

    /**
     * Boilerplate method to output the header and data-set.
     */
    template< class t_set_type > static inline void do_serialise(std::ostream & a_os, const t_set_type & a_data_set, const report::filetype::base_out_decor & a_header, const std::string& a_format);

    /**
     * no copy
     */
    xy_spectrum<t_value> & operator=(const xy_spectrum<t_value> & source);


  public:
    /**
     * Should we output text?
     */
    bool output_csv() const { return m_output_type == CSV; }

    /**
     * Should we output text?
     */
    bool output_jcampdx() const { return m_output_type == JCAMPDX; }

    /**
     * Should we output text?
     */
    bool output_text() const { return m_output_type == TEXT; }

    virtual void register_options(boost::program_options::options_description & a_opt_set);

    /**
     * Output the header and data-set as a per the output type
     */
    template< class t_set_type > inline void serialise(std::ostream & a_os, const report::filetype::output_header & a_header, const t_set_type & a_data_set);

    /**
     * Set to output CSV
     */
    void set_csv(bool a_yes) { if (a_yes) m_output_type = CSV; }

    /**
     * Set to output text
     */
    void set_text(bool a_yes) { if (a_yes) m_output_type = TEXT; }

    /**
     * Set to output text
     */
    void set_jcampdx(bool a_yes){ if (a_yes) m_output_type = JCAMPDX; }

    xy_spectrum(): m_output_type (TEXT) {}


  private:
    /**
     * no copy
     */
    xy_spectrum(const xy_spectrum<t_value> & source);


  public:
    virtual ~xy_spectrum() {}

};
/**
 * Boilerplate method to output the header and data-set.
 */
template<class t_value>
template< class t_set_type > inline void xy_spectrum<t_value>::do_serialise(std::ostream & a_os, const t_set_type & a_data_set, const report::filetype::base_out_decor & a_header, const std::string& a_format)
{
  a_header.serialise_header (a_os);
  for (typename t_set_type::const_iterator l_i (a_data_set.begin ());
       l_i != a_data_set.end ();
       ++l_i) {
    a_os << boost::format (a_format) % (*l_i).first % (*l_i).second; }
  a_header.serialise_tail (a_os);
}

template<class t_value>
void xy_spectrum<t_value>::register_options(boost::program_options::options_description & a_opt_set) {
  // Add options to a_opt_set
  a_opt_set.add_options ()
    ("jcampdx,j", boost::program_options::bool_switch ()->notifier (boost::bind (&xy_spectrum::set_jcampdx, this, _1)), "output in JCAMP/DX format")
    ("csv,c", boost::program_options::bool_switch ()->notifier (boost::bind (&xy_spectrum::set_csv, this, _1)), "output in comma-separated-value format");
}

/**
 * Output the header and data-set as a per the output type
 */
template<class t_value>
template< class t_set_type > inline void xy_spectrum<t_value>::serialise(std::ostream & a_os, const report::filetype::output_header & a_header, const t_set_type & a_data_set) {
  std::auto_ptr< report::filetype::base_out_decor > l_header;
  std::string l_format;
  if (output_jcampdx ()) {
    l_header.reset (new report::filetype::jcampdx_out_decor (a_header));
    l_format.assign ("%d %d\n"); }
  else if (output_csv ()) {
    l_header.reset (new report::filetype::csv_out_decor (a_header));
    l_format.assign ("%d, %d\n"); }
  else {
    l_header.reset (new report::filetype::text_out_decor (a_header));
    l_format.assign ("%d %d\n"); }
  
  if (NULL != l_header.get ()) {
    do_serialise < t_set_type > (a_os, a_data_set, *(l_header.get ()), l_format);}
}


} // namespace report
#endif
