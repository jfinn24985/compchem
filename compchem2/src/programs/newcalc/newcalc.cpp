// ------------------------------------------------------------
// class: newcalc
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/newcalc/newcalc.hpp"
#include "programs/newcalc/scope.hpp"

// Manual include
#include <boost/filesystem/path.hpp>
#include <Xmt/Xmt.h> 
#include <Xmt/Procedures.h> 
#include "guistate/gui.hpp"
// -
namespace cchem {

namespace newcalc {

/**
 * The application object.
 */
newcalc * newcalc::m_application(NULL);

/**
 * The suppposed class name (blank here)
 */
const std::string newcalc::m_class;

/**
 * The application name (derived from the name the program was called as.
 */
const std::string newcalc::m_name("NewCalc");

/**
 * Write the script file.
 */
void newcalc::data_write() throw(std::bad_alloc) {
  COMPCHEM_CHECK(NULL != m_parent, "Should not call this method without guistate.");
  // The path of the directory to create files in.
  boost::filesystem::path l_dir;
  bool is_continue (true);
  COMPCHEM_LOG ("Entering function");
  // PREP
  cchem::guistate::resource_path l_path ("*." + application_name() + ".configuration");
  // Create a working scope for
  scope_working l_scope1 (l_path, *m_parent);
  // Create a local scope on top of the working scope
  scope_local l_scope (l_scope1);
  // Add the attribute data to the scope
  for (unsigned int l_attr (0);
       l_attr < m_parent->upper_bound ();
       ++l_attr) {
    const std::string l_val (m_parent->get_attribute_as_string (l_attr));
    COMPCHEM_DLOG("[");COMPCHEM_DLOG(l_attr);COMPCHEM_DLOG("] = \"");COMPCHEM_DLOG(l_val);
    l_scope.add_local (boost::lexical_cast< std::string > (l_attr), l_val); }
  
  // Determine directory name and check create 
  l_dir = make_substitutions (l_scope.value ("directory", l_scope.base ()), l_scope);
  if (boost::filesystem::exists(l_dir)) {
    COMPCHEM_DLOG("Directory exists\n");
    if (not boost::filesystem::is_directory(l_dir)) {
      // A file with the name of the intended directory exists..
      COMPCHEM_ALWAYS(false, "TODO: Add warning for 'file exists'"); }
    else {
      std::stringstream l_msg;
      l_msg << "A directory called [" <<  l_dir.string ()
            << "] already exists, continuing\nmay overwrite some of the contents of this directory.";
      if (not m_parent->warning_and_ask (std::string(), l_msg.str(), "Continue", "Cancel", std::string())) {
        // Signal exit if we don't want to continue.
        is_continue = false; }}}
  else {
    boost::filesystem::create_directory (l_dir); }
  if (is_continue) {
    // Generate files
    generate (l_scope, l_dir); }
}

/**
 * Find occurences of '%%var%%' and return "var" and set a_start and a_end
 */
std::string newcalc::find_variable(const std::string& a_str, unsigned int & a_start, unsigned int & a_end) throw(std::bad_alloc) {
  // Find occurences of '%var%' and return "var" and set a_start and a_end
  std::string Result;
  a_start = 0;
  do {
    if (0 != a_start) { ++a_start; } // To actually skip single '%'s
    a_start = a_str.find ('%', a_start); }
  while (std::string::npos != a_start
        and '%' != a_str[a_start + 1]); // Skip single '%'s
  if (std::string::npos != a_start) { // Found '%%'
    a_end = a_str.find ('%', a_start + 2);
    ++a_end;
    COMPCHEM_ALWAYS ('%' == a_str[a_end], "Parse error, expected '%%[var]%%' but got '%%[var]%'");
    ++a_end;
    COMPCHEM_CHECK(a_start + 2 < a_end - 2, "No variable");
    Result = a_str.substr (a_start + 2, a_end - a_start - 4); 
    COMPCHEM_DLOG("Variable = \""); COMPCHEM_DLOG(Result); COMPCHEM_DLOG("\"\n"); }
  return Result;
}

/**
 * This method creates the result files. We create the result directory and 
 * filenames on-the-fly. 
 */
void newcalc::generate(scope & a_scope, const boost::filesystem::path & a_dir) throw(std::bad_alloc) {
  // Indicator 
  COMPCHEM_LOG ("Entering function");
  cchem::guistate::string_list_t l_multi_list;
  std::string l_value;
  if (a_scope.has_value (a_scope.base () / "scripts", l_value)) {
    cchem::guistate::utility::convert_string_to_list (make_substitutions(l_value, a_scope), l_multi_list);
    for (cchem::guistate::string_list_t::const_iterator l_part_cursor (l_multi_list.begin ());
         l_part_cursor != l_multi_list.end ();
         ++l_part_cursor) {
      COMPCHEM_DLOG("List entry [");COMPCHEM_DLOG(*l_part_cursor);COMPCHEM_DLOG("]\n");
      scope_local l_local (a_scope);
      l_local.add_local ("scriptname", *l_part_cursor); 
      boost::filesystem::path l_filename (a_dir / make_substitutions(a_scope.value ("filename", a_scope.base ()), l_local));
      std::ofstream l_os (l_filename.native_file_string ().c_str ());
      if (l_os) {
        l_os << make_substitutions (a_scope.value ("shell", a_scope.base ()), l_local) << "\n\n";
        write_body (a_scope.base () / *l_part_cursor, l_local, l_os); }
      else {
        std::cerr << "Unable to open/create file [" << l_filename.native_file_string () << "]\n"; }}}
  else {
    std::cerr << "Unable to find any scripts to output.\n"; }
}

/**
 * Convert the input string into final form by making substitutions from a_scope
 * and elsewhere.
 */
std::string newcalc::make_substitutions(const std::string& a_str, const scope & a_scope) throw(std::bad_alloc) {
  unsigned l_start (0), l_end (0);
  std::string Result (a_str);
  COMPCHEM_DLOG("Transforming [");COMPCHEM_DLOG(Result);COMPCHEM_DLOG("]\n");
  do {
    std::string l_var (find_variable (Result, l_start, l_end));
    if (not l_var.empty ()) {
      std::string l_subs; // The substitution string
      unsigned int l_pos (l_var.find ('$'));
      if (std::string::npos != l_pos) { // Has subpart, %%??$0%%
        // The subpart reference indicates that the value is a stringlist we want
        // the indexed position.
        unsigned int l_ix (boost::lexical_cast< unsigned int >(l_var.substr (l_pos + 1)));
        l_var.resize (l_pos);
        cchem::guistate::string_list_t l_parts;
        cchem::guistate::utility::convert_string_to_list (a_scope.value (l_var), l_parts);
        COMPCHEM_CHECK (l_ix < l_parts.size (), "Not enough elements substituting vars"); 
        COMPCHEM_DLOG ("List index is ["); COMPCHEM_DLOG (l_ix);
        COMPCHEM_DLOG ("] Val ["); COMPCHEM_DLOG (l_parts.at (l_ix)); COMPCHEM_DLOG ("]\n");
        l_subs = l_parts.at (l_ix); }
      else {
        l_subs = a_scope.value (l_var); }
      // Make substitution
      Result.replace (l_start, l_end - l_start, l_subs); }}
  while (std::string::npos != l_start);
  COMPCHEM_DLOG("Transform result [");COMPCHEM_DLOG(Result);COMPCHEM_DLOG("]\n");
  return Result;
}

newcalc::newcalc(int & a_argc, char ** a_argv) throw()
: m_parent (NULL) {
  COMPCHEM_REQUIRE (NULL == m_application, "Cannot create two application instances.");
  m_application = this;
  // m_name.assign(boost::filesystem::path (a_argv[0], &boost::filesystem::native).leaf ());
}

/**
 * This method is registered with the Xmt system and acts as trigger callback.
 */
void newcalc::notifier(widget_t a_widg, void * client_data, void * calldata) throw()
{
  try {
    if (NULL == memchr(client_data, '\0', 20)) {
      std::stringstream l_msg;
      std::string l_call_phrase ((char*)client_data, 20);
      l_msg << "The resource 'Notify' method was called with \"" << l_call_phrase << "\"\n";
      m_application->m_parent->warning_and_ask ("", l_msg.str ().c_str (), "Okay", "", ""); }
    else {
      std::string l_call_phrase ((char*)client_data);
      COMPCHEM_LOG("In Notify with client_data = [" + l_call_phrase + "]\n");
      // The data_write action means to create the gaussian input files. 
      if (l_call_phrase == "data_write") {
        m_application->data_write (); }
      else if (l_call_phrase == "application_close") {
        m_application->close (); }}}
  catch (const std::exception &a_err) {
    COMPCHEM_ALWAYS(false, a_err.what ()); } 
}

/**
 * This method is called before attempting to create an X11 connection. It should be
 * used to register static information with the library.  For example this is the 
 * place to register functions and widget types with the Xmt resource converter.
 */
void newcalc::register_with_x11(newcalc::guistate_type * a_self) throw() {
  COMPCHEM_REQUIRE (NULL != a_self, "Cannot work with null parent");
  if (NULL == m_parent) {
    m_parent = a_self;
    // Register the callback function "Notify" for use in the resource file converters
    XmtRegisterCallbackProcedure("Notify"
  			, (XtCallbackProc)&newcalc::notifier
  			, XtRString); }
  
}

/**
 * Write a subsection starting from a_base resource path onto a_os stream.  Substitutions should be made from a_scope.
 */
void newcalc::write_body(const resource_path & a_base, scope & a_scope, std::ostream & a_os) throw(std::bad_alloc) {
  const cchem::guistate::resource_path l_for ("foreach");
  const cchem::guistate::resource_path l_part ("parts");
  const cchem::guistate::resource_path l_val ("value");
  const cchem::guistate::resource_path l_if ("if");
  cchem::guistate::string_list_t l_stages;
  cchem::guistate::string_list_t l_steps;
  std::string l_value;
  bool l_afterok (false), l_is_if (false);
  // Test for value (overrides parts etc)
  if (a_scope.has_value (a_base / l_val, l_value)) {
    a_os << make_substitutions (l_value, a_scope) << "\n"; }
  else { // process stages and steps.
    // Test for looping constructs
    COMPCHEM_LOG ("Checking for 'foreach' clause\n");
    if (a_scope.has_value (a_base / l_for, l_value)) {
      cchem::guistate::utility::convert_string_to_list (make_substitutions (l_value, a_scope), l_stages);
      // Set stage progress type
      l_afterok = ("afterok" == a_scope.value (a_base / "type")); }
    // Test for parts
    COMPCHEM_LOG ("Checking for 'parts' clause\n");
    if (a_scope.has_value (a_base / l_part, l_value)) {
      cchem::guistate::utility::convert_string_to_list (make_substitutions (l_value, a_scope), l_steps); }
    
    // Test for "if" and add if necessary [## IF ##]
    if (a_scope.has_value (a_base / l_if, l_value)) {
      l_is_if = true;
      scope_local l_local (a_scope);
      l_local.add_local ("predicate", l_value);
      a_os << make_substitutions (a_scope.value ("if", a_scope.base ()), l_local); }
  
    cchem::guistate::string_list_t::const_iterator l_stage_cr (l_stages.begin ());
    unsigned int l_depth (0);
    do {
      cchem::guistate::resource_path l_specific (a_base);
      if (l_stage_cr != l_stages.end ()) { // Stage loops
        l_specific /= *l_stage_cr; 
        // What sort of stage loop?
        if (l_afterok) {
          if (l_depth > 0) {
            scope_local l_local (a_scope);
            l_local.add_local ("predicate", a_scope.value ("afterok", a_scope.base ()));
            a_os << make_substitutions (a_scope.value ("if", a_scope.base ()), l_local); }
          ++l_depth; }}
      // Process parts
      for (cchem::guistate::string_list_t::const_iterator l_job_opt_cursor (l_steps.begin ());
        l_job_opt_cursor != l_steps.end ();
        ++l_job_opt_cursor) {
        // recurse
        write_body (l_specific / *l_job_opt_cursor, a_scope, a_os); }
      if (l_stage_cr != l_stages.end ()) {
        ++l_stage_cr; }}
    while (l_stage_cr != l_stages.end ()); 
    // Now undo any ifs
    for (;
         l_depth > 1;
         --l_depth) {
       a_os << make_substitutions (a_scope.value ("endif", a_scope.base ()), a_scope); }}
  // Do we need a closing "endif"? [## IF ##]
  if (l_is_if) {
    a_os << make_substitutions (a_scope.value ("endif", a_scope.base ()), a_scope); }
  
}


} // namespace cchem::newcalc

} // namespace cchem

