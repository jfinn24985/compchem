#ifndef COMPCHEM_SUPPLIER_PLUGIN_FILTER_DIRECT_HPP
#define COMPCHEM_SUPPLIER_PLUGIN_FILTER_DIRECT_HPP

// ------------------------------------------------------------
// class: plugin_filter_direct
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_plugin_handler.hpp"

// Manual includes
#include <sstream>
// -
namespace supplier {

/**
 * This class plays the role of the plugin_manager for a single filter. It does not load 
 * the plugin dynamically, instead using the filter class directly.
 */
class plugin_filter_direct : public basic_plugin_handler {
  public:
    /**
     * Load the filtered result from the plugin into the filter manager provided a_twiglet.
     * 
     * \pre is_valid
     * \post error = true if an error occurred
     */
    inline void output(void * a_twiglet);


  private:
    /**
     * no copy
     */
    plugin_filter_direct & operator=(const plugin_filter_direct & source);


  public:
    /**
     * Create a filter to operate on the given file.
     * 
     * \post error = true if an error occurred
     */
    plugin_filter_direct();

    ~plugin_filter_direct() {}


  private:
    /**
     * no copy
     */
    plugin_filter_direct(const plugin_filter_direct & source);


  public:
    /**
     * Get a reference to the filter state object.
     */
    void * plugin_object() {
      REQUIRE (is_valid (), "Can not get any information with an invalid plugin.");
      return state (); }

    /**
     * 																													Process the file.
     * 
     * \pre is_valid
     * \post error /= 0 if an error occurred
     */
    inline void process(base_filetype & a_filetype);

};
/**
 * Load the filtered result from the plugin into the filter manager provided a_twiglet.
 * 
 * \pre is_valid
 * \post error = true if an error occurred
 */
inline void plugin_filter_direct::output(void * a_twiglet) {
  REQUIRE (is_valid (), "Can not get any information with an invalid plugin.");
  // Perform operation
  const int l_esize (256);
  char l_errbuf [l_esize];
  m_error = base_plugin_filter::output (a_twiglet, &l_errbuf[0], l_esize);
  // If an error occurred capture error message.
  if (0 != m_error) {
    l_errbuf[l_esize - 1] = '\0';
    m_error_msg << &l_errbuf[0]; }
}

/**
 * 																													Process the file.
 * 
 * \pre is_valid
 * \post error /= 0 if an error occurred
 */
inline void plugin_filter_direct::process(base_filetype & a_filetype) {
  // PRE
  REQUIRE(true == is_valid (), "Filter must be valid before processing.");
  // LOCAL
  const int l_size(256);
  char l_errbuf[l_size];
  char l_linebuf[l_size];
  // DO
  // Test is suitable
  m_error = base_plugin_filter::can_process (a_filetype, &l_errbuf[0], l_size);
  if (1 == m_error) {
    m_error_msg << name () << " is not usable for current file\n";
    finalise(); }
  else if (0 != m_error) {
    // Any other non-zero value is an error
    l_errbuf[l_size - 1] = '\0';
    m_error_msg << name () << ": " << &l_errbuf[0];
    finalise (); }
  // Set error is not suitable
  // Perform processing.
  for (int l_lsize = l_size;
       0 == m_error
  	 && a_filetype.next_segment (&l_linebuf[0], l_lsize);
       l_lsize = l_size) {
    m_error = base_plugin_filter::process (&l_linebuf[0], l_lsize
  		    , &l_errbuf[0], l_size);
    if (0 != m_error) {
      // Ensure nul terminated
      l_errbuf[l_size - 1] = '\0';
      m_error_msg << &l_errbuf[0]; }}
  // Set error if not end of file
  if (0 == m_error && not a_filetype.eof ()) {
    m_error = 1;
    m_error_msg << name () << "Reading of file terminated before end of file."; }
  // END
}


} // namespace supplier
#endif
