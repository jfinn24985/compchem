#ifndef COMPCHEM_FILTER_DATUM_PROXY_HPP
#define COMPCHEM_FILTER_DATUM_PROXY_HPP

// ------------------------------------------------------------
// class: datum_proxy
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/supplier_constants.hpp"
#include <string>
#include "supplier/basic_filter_traits.hpp"
#include "supplier/supplier_error.hpp"
namespace supplier { class supplier_error; } 

namespace supplier {

namespace filter {

/**
 * This class holds details of a datum object and can output these
 * using the direct methods of basic_datum_traits. Nesting of
 * datum objects via this proxy is not supported.
 */
class datum_proxy
{
  public:
    /**
     * The text content.
     */
    std::string content;

    /**
     * The datum's format
     */
    std::string format;

    /**
     * The ordinal of the datum (default = -1 (unset))
     */
    int ordinal;

    /**
     * The outcome of the object.
     */
    std::string outcome;

    /**
     * The scale attribute for the datum
     */
    std::string scale;

    /**
     * Does this have a result to save (outcome and content /= empty)?
     */
    bool empty() const
    {
      return outcome.empty () or content.empty ();
    }

    datum_proxy()
    : content ()
    , format ()
    , ordinal (-1)
    , outcome ()
    , scale ()
    {}

    datum_proxy(const std::string & a_outcome)
    : content ()
    , format ()
    , ordinal (-1)
    , outcome (a_outcome)
    , scale ()
    {}

    datum_proxy(const std::string & a_outcome, const std::string & a_content)
    : content (a_content)
    , format ()
    , ordinal (-1)
    , outcome (a_outcome)
    , scale ()
    {}

    ~datum_proxy()
    {}

    datum_proxy(const datum_proxy & source)
    : content (source.content)
    , format (source.format)
    , ordinal (source.ordinal)
    , outcome (source.outcome)
    , scale (source.scale)
    {}

    datum_proxy & operator=(const datum_proxy & source)
    {
      content.assign (source.content);
      format.assign (source.format);
      ordinal = source.ordinal;
      outcome.assign (source.outcome);
      scale.assign (source.scale);
      return *this;
    }
    

    /**
     * Write the data directly as a child node of a_twiglet.
     * 
     * \pre not outcome.empty
     * \return /= nul.
     */
    inline void * output(::basic_filter_traits *const a_filter, void * a_twiglet);


};
/**
 * Write the data directly as a child node of a_twiglet.
 * 
 * \pre not outcome.empty
 * \return /= nul.
 */
inline void * datum_proxy::output(::basic_filter_traits *const a_filter, void * a_twiglet) 
{
  void *Result (NULL);
  if (outcome.empty ())
  {
    throw supplier_error ("Cannot create a datum with no outcome.", CCHEM_SUPPLIER_BADDATA);
  }
  else
  {
    Result = ((*a_filter->m_create_datum_cb) (a_twiglet, outcome.data (), outcome.size ()));
    if (NULL == Result)
    {
      throw supplier_error ("Failed to create a datum object.", CCHEM_SUPPLIER_FATAL);
    }
    if (not format.empty ())
    {
      (*a_filter->m_set_format_cb) (Result, format.data (), format.size ());
    }
    if (0 <= ordinal)
    {
      (*a_filter->m_set_ordinal_cb) (Result, ordinal);
    }
    if (not scale.empty ())
    {
      (*a_filter->m_set_scale_cb) (Result, scale.data (), scale.size ());
    }
    if (not content.empty ())
    {
      (*a_filter->m_add_content_cb) (Result, content.data (), content.size ());
    }
  }
  
  return Result;

}


} // namespace supplier::filter

} // namespace supplier
#endif
