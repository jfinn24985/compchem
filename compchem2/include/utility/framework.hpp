#ifndef COMPCHEM_FRAMEWORK_HPP
#define COMPCHEM_FRAMEWORK_HPP

// ------------------------------------------------------------
// This file contains helpful free functions in the framework namespace.
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * This file contains helpful free functions in the framework namespace.
 */
namespace utility {


/// \brief Ensure a nul character at end of buffer.
///
/// After copying a_copy characters to a_bf ensure that a_bf is nul terminated by
/// setting a nul at position a_copy or a_size - 1 which ever is smaller.
inline void nul_terminate (char* a_bf, unsigned long a_size, unsigned long a_copy) {
 *(a_bf + ((a_size > a_copy)?a_copy:a_size - 1)) = '\0'; }

/// \brief Is first argument in the inclusive range of second and third argument?
///
/// Result implies arg2 <= arg1 <= arg3
template < typename t_number >
inline bool in_irange (t_number a1, t_number a2, t_number a3) {
  return a2 <= a1 && a1 <= a3; }

/// \brief Is first argument in the exclusive range of second and third argument?
///
/// Result implies arg2 < arg1 < arg3
template < typename t_number >
inline bool in_xrange (t_number a1, t_number a2, t_number a3) {
  return a2 < a1 && a1 < a3; }


} // namespace utility
#endif
