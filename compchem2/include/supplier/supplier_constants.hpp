#ifndef COMPCHEM_SUPPLIER_CONSTANTS_HPP
#define COMPCHEM_SUPPLIER_CONSTANTS_HPP

// ------------------------------------------------------------
// class: supplier_constants
// $revision:$
// $log:$
// ------------------------------------------------------------


/**
 * Set of flags for communicating state between plugin and application
 */
enum {
  CCHEM_SUPPLIER_LINE_ORIENTED= 1,/**
   * Indicate the plugin is designed to operate line-by-line.
   */

  CCHEM_SUPPLIER_BUFFER_ORIENTED= 2,/**
   * Indicate the plugin can act on fixed size segments of the file.
   */

  CCHEM_SUPPLIER_FILE_ORIENTED= 4,/**
   * Indicate the plugin takes the filename and manages reading
   * internally.
   */

  CCHEM_SUPPLIER_STRING_OUTPUT= 8,/**
   * Indicate the filter outputs results as a string
   * containing XML elements.
   */

  CCHEM_SUPPLIER_DIRECT_OUTPUT= 16/**
   * Indicate that the filter can create elements directly.
   */


};
/**
 * A set of return values from the various methods. (should be one of 
 * enum result_values; ON_ERROR, ON_LOADERROR, ON_NOFILE
 * or ON_BADDATA)
 */
enum {
  CCHEM_SUPPLIER_SUCCESS= 0,/**
   * Return value for when a function performed some task and is not in an error state.
   */

  CCHEM_SUPPLIER_COMPLETE,/**
   * This value is returned by a process method when the 
   * filter no longer needs to process the file to generate 
   * its result.
   */

  CCHEM_SUPPLIER_UNUSABLE,/**
   * Return value when a function fails but is not in an error state.  For example this is returned when a plugin filter can not process the given file type.
   */

  CCHEM_SUPPLIER_FATAL,/**
   * The lowest return value that indicates a function encountered an error.  This implies that the filter has writen an error message in the supplied error buffer.  Additionally, any number larger than this value also indicates an error.
   */

  CCHEM_SUPPLIER_LOADERROR,/**
   * Indicate that loading a dynamic library failed.
   */

  CCHEM_SUPPLIER_NOFILE,/**
   * Indicate that a file was not found. This is a type of fatal error
   */

  CCHEM_SUPPLIER_BADDATA,/**
   * Indicate that inconsistent data has been obtained. This is a
   * type of fatal error.
   */

  CCHEM_SUPPLIER_BADCONFIG/**
   * Indicate a problem with the system configuration. This is a type of
   * fatal error
   */


};
#endif
