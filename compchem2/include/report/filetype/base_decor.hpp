#ifndef BASE_DECOR_HPP
#define BASE_DECOR_HPP

// ------------------------------------------------------------
// class: base_decor
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace report { namespace filetype { class output_header; }  } 

namespace report {

namespace filetype {

/**
 * Abstract base class for header sections (and tail sections) of report files.  This class comes about because most report formats consist of the data set preceded by some header section and followed by some tail section.  This class heirarchy is responsible for writing out the header section of some data file.
 */
class base_out_decor {
  protected:
    /**
     * The output header to serialise.
     */
    const output_header & m_header;


  public:
    base_out_decor(const output_header & a_header): m_header (a_header) {}

    virtual ~base_out_decor() {}


  private:
    /**
     * no copy
     */
    base_out_decor(const base_out_decor & source);

    /**
     * no copy
     */
    base_out_decor & operator=(const base_out_decor & source);


  public:
    virtual void serialise_header(std::ostream & a_os) const = 0;

    virtual void serialise_tail(std::ostream & a_os) const = 0;

};
/**
 * Abstract base class for header sections (and tail sections) of report files.  This class comes about because most report formats consist of the data set preceded by some header section and followed by some tail section.  This class heirarchy is responsible for reading in the header section of some data file.
 */
class base_in_decor {
  protected:
    output_header & m_header;


  public:
    base_in_decor(output_header & a_header): m_header (a_header) {}


  private:
    /**
     * no copy
     */
    base_in_decor(const base_in_decor & source);


  public:
    virtual ~base_in_decor() {}

    virtual void deserialise_header(std::istream & a_is) = 0;

    virtual void deserialise_tail(std::istream & a_is) = 0;


  private:
    /**
     * no copy
     */
    base_in_decor & operator=(const base_in_decor & source);

};

} // namespace report::filetype

} // namespace report
#endif
