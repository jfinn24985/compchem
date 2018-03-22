#ifndef PURPLETREE_XML2_URI_HPP
#define PURPLETREE_XML2_URI_HPP

// ------------------------------------------------------------
// class: xml2_uri
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
extern "C"
{
// Use GNOME libxml2
#include <libxml/uri.h>
}
#include <string>
#include "xmlreader/basic_uri.hpp"

namespace xmlreader {

class xml2_uri_traits
{
  public:
    typedef xmlURIPtr rep_type;

    static std::string authority(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->authority) : std::string ();
    }

    static inline rep_type copy(rep_type a_rep);

    static std::string fragment(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->fragment) : std::string ();
    }

    static bool is_valid(const std::string& a_uri)
    {
      rep_type l_trial (parse (a_uri));
      const bool Result (NULL != l_trial);
      if (Result) 
      {
        reset (l_trial);
      }
      return Result;
    }
    static rep_type parse(const std::string& a_uri)
    {
      return (a_uri.empty ()? NULL : xmlParseURI (a_uri.c_str ()));
    }
    static std::string path(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->path) : std::string ();
    }

    static int port(const rep_type & a_rep)
    {
      return NULL != a_rep ? a_rep->port : 0;
    }

    static std::string query(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->query_raw) : std::string ();
    }

    static void reset(rep_type & a_rep)
    {
      xmlFreeURI (a_rep);
      a_rep = NULL;
    }

    static std::string scheme(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->scheme) : std::string ();
    }

    static std::string server(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->server) : std::string ();
    }

    static std::string user(const rep_type & a_rep)
    {
      return NULL != a_rep ? as_string (a_rep->user) : std::string ();
    }


  private:
    /**
     * Helper to convert char* to string on returns. *Does not* free a_str.
     */
    static std::string as_string(const char * a_str)
    {
      return a_str != NULL ? std::string (a_str) : std::string ();
    }


};
inline xml2_uri_traits::rep_type xml2_uri_traits::copy(xml2_uri_traits::rep_type a_rep)

{
  rep_type Result;
  if (a_rep != rep_type (0))
  {
    xmlChar * l_tmp (xmlSaveUri (a_rep));
    if (NULL != l_tmp)
    {
      Result = xmlParseURI (reinterpret_cast<char*>(l_tmp));
      xmlFree (l_tmp);
    }
  }
  return Result;

}

/**
 * libxml2 URL type
 */
typedef basic_uri<xml2_uri_traits> uri;

} // namespace xmlreader
#endif
