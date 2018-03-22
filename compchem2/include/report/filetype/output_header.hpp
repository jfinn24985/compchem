#ifndef OUTPUT_HEADER_HPP
#define OUTPUT_HEADER_HPP

// ------------------------------------------------------------
// class: output_header
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <map>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>


// Manual includes
namespace report {

namespace filetype {

/**
 * Type for the header section of JCAMP-DX format data file.
 */
class output_header {
  public:
    typedef std::map< std::string, std::string > map_type;

    typedef map_type::const_iterator const_iterator;


  private:
    /**
     * The "data type" header value.  This is a general description of the contents.
     */
    std::string m_data_type;

    /**
     * The date the data was created on.
     */
    boost::gregorian::date m_date;

    map_type m_header_map;

    /**
     * The source of the data
     */
    std::string m_origin;

    /**
     * The organisation or individual who owns the data
     */
    std::string m_owner;

    /**
     * The user supplied title for the data
     */
    std::string m_title;


  public:
    inline output_header();

    inline output_header(const output_header & source);

    virtual ~output_header() {}

    const std::string& data_type() const { return m_data_type; }

    void data_type(const std::string & a_val) { m_data_type.assign (a_val); }

    const boost::gregorian::date& date() const {
      return m_date;
    }

    void date(const boost::gregorian::date & a_val) { m_date = a_val; }

    /**
     * The date as a ISO formatted string
     */
    std::string date_string() const { return boost::gregorian::to_iso_extended_string (m_date); }

    /**
     * The date from a ISO formatted string
     */
    void date_string(const std::string & a_val);

    const std::map< std::string, std::string >& get_header_map() const {
      return m_header_map;
    }

    std::map< std::string, std::string >& get_header_map() {
      return m_header_map;
    }

    /**
     * Is the given key in the header map (this excludes the standard keys for title, version etc).
     */
    bool has_key(const std::string& a_key) const { return (0 != m_header_map.count (a_key)); }

    /**
     * Set value of map[a_key] = a_value
     */
    void key(const std::string& a_key, const std::string& a_value) { m_header_map.insert (std::make_pair (a_key, a_value)); }

    /**
     * Get value of map.at (a_key)
     * 
     * \pre has_key (a_key)
     */
    const std::string & key(const std::string& a_key) const { 
     REQUIRE (has_key (a_key), "Key is not in header map");
     return m_header_map.find (a_key)->second; }

    inline output_header & operator=(const output_header & source);

    const std::string& origin() const { return m_origin; }

    void origin(const std::string & a_val) { m_origin.assign (a_val); }

    const std::string& owner() const { return m_owner; }

    void owner(const std::string & a_val) { m_owner.assign (a_val); }

    const std::string& title() const { return m_title; }

    void title(const std::string & a_val) { m_title.assign (a_val); }

    /**
     * clears all member variables
     */
    virtual void clear();

};
inline output_header::output_header()
  : m_data_type ()
  , m_date (boost::gregorian::day_clock::universal_day())
  , m_header_map ()
  , m_origin ()
  , m_owner ()
  , m_title ()
  {}


inline output_header::output_header(const output_header & source) 
  : m_data_type (source.m_data_type)
  , m_date (source.m_date)
  , m_header_map (source.m_header_map)
  , m_origin (source.m_origin)
  , m_owner (source.m_owner)
  , m_title (source.m_title) {}


inline output_header & output_header::operator=(const output_header & source) {
  m_data_type.assign (source.m_data_type);
  m_date = source.m_date;
  m_header_map.clear ();
  m_header_map.insert (source.m_header_map.begin (), source.m_header_map.end ());
  m_origin.assign (source.m_origin);
  m_owner.assign (source.m_owner);
  m_title.assign (source.m_title);
  return *this;
}


} // namespace report::filetype

} // namespace report
#endif
