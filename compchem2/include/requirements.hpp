#ifndef COMPCHEM_REQUIREMENTS_HPP
#define COMPCHEM_REQUIREMENTS_HPP

// ------------------------------------------------------------
// class: requirements
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * This header file defines macros that indicate what is available in the current compilation environment.  These are modelled on the macros found in the "config.h" file created by Configure scripts etc. with a view to later adding this capability.
 */

// This should be defined for all POSIX compliant systems
#define HAVE_UNISTD_H

// This should be defined for systems with "stat" libc function.
#define HAVE_SYS_STAT_H 

// This should be defined for systems with a backtrace function
#define HAVE_BACKTRACE

// For systems with snprintf
#define HAVE_SNPRINTF

// For systems with strerror_r
#define HAVE_STRERROR_R

// Std requirements
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

// Can the libc internally malloc memory for getcwd?
#define HAVE_GETCWD_EXT

#endif
