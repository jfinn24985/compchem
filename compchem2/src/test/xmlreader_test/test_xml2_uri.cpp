// ------------------------------------------------------------
// class: test_xml2_uri
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/xmlreader_test/test_xml2_uri.hpp"

// Manual includes
#include <sstream>
// -
namespace xmlreader {

test_xml2_uri test_xml2_uri::s_exemplar;

/**
 * Override in derived classes to perform unit test.
 * 
 * Failed tests are indicated by an uncaught exception.
 */
void test_xml2_uri::main_test() const 
{
  print_uri ("http://www.goldengirls.com/sitcom.asp?maude");
  print_uri ("/www.goldengirls.com/sitcom.asp");
  print_uri ("sftp://finnerty!123456@localhost:2001/mnt/home/finnerty/calc");

}

/**
 * Parse and print parts of a_uri
 */
void test_xml2_uri::print_uri(const std::string& a_uri) const 
{
  // [scheme]://[user]:[authority]@[server]:[port]/[path][#[fragment]|?[query]]
  
  xmlreader::uri l_tmp (a_uri);
  const xmlreader::uri l_t2 (l_tmp);
  xmlreader::uri l_t3;
  log () << COMM << "Parse URI [" << a_uri << "]\n";
  log () << COMM << "Scheme    : " << l_tmp.scheme () << "\n";
  log () << COMM << "User      : " << l_tmp.user () << "\n";
  log () << COMM << "Authority : " << l_tmp.authority () << "\n";
  log () << COMM << "Server    : " << l_tmp.server () << "\n";
  log () << COMM << "Port      : ";
  const int l_port (l_tmp.port ());
  if (0 != l_port)
  {
    log () << l_port;
  }
  log () << "\n";
  log () << COMM << "Path      : " << l_tmp.path () << "\n";
  log () << COMM << "Fragment  : " << l_tmp.fragment () << "\n";
  log () << COMM << "Query     : " << l_tmp.query () << "\n";
  l_t3 = l_tmp;
  log () << COMM << "Check copying. (orig, copy, assign)\n";
  std::ostringstream l_o1, l_o2, l_o3;
  l_o1 << l_tmp.scheme () << "://" << l_tmp.user () << l_tmp.authority ()
         << "[@]" << l_tmp.server () << "[:" << l_tmp.port () << "]"
         << l_tmp.path () << "[?]" << l_tmp.query () << "[#]" << l_tmp.fragment () << "\n";
  l_o2 << l_t2.scheme () << "://" << l_t2.user () << l_t2.authority ()
         << "[@]" << l_t2.server () << "[:" << l_t2.port () << "]"
         << l_t2.path () << "[?]" << l_t2.query () << "[#]" << l_t2.fragment () << "\n";
  l_o3 << l_t3.scheme () << "://" << l_t3.user () << l_t3.authority ()
         << "[@]" << l_t3.server () << "[:" << l_t3.port () << "]"
         << l_t3.path () << "[?]" << l_t3.query () << "[#]" << l_t3.fragment () << "\n";
  log () << l_o1.str () << l_o2.str () << l_o3.str ();
  bool_test (l_o1.str () == l_o2.str (), "original and copy match.");
  bool_test (l_o1.str () == l_o3.str (), "original and assignment copy match.");
  bool_test (l_tmp.scheme ().empty () or std::string::npos != a_uri.find (l_tmp.scheme ()), "scheme empty or found in original URI.");
  bool_test (l_tmp.user ().empty () or std::string::npos != a_uri.find (l_tmp.user ()), "user empty or found in original URI.");
  bool_test (l_tmp.authority ().empty () or std::string::npos != a_uri.find (l_tmp.authority ()), "authority empty or found in original URI.");
  bool_test (l_tmp.server ().empty () or std::string::npos != a_uri.find (l_tmp.server ()), "server empty or found in original URI.");
  bool_test (l_tmp.path ().empty () or std::string::npos != a_uri.find (l_tmp.path ()), "path empty or found in original URI.");
  bool_test (l_tmp.query ().empty () or std::string::npos != a_uri.find (l_tmp.query ()), "query empty or found in original URI.");
  bool_test (l_tmp.fragment ().empty () or std::string::npos != a_uri.find (l_tmp.fragment ()), "fragment empty or found in original URI.");
  

}


} // namespace xmlreader
