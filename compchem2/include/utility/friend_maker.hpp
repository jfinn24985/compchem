#ifndef COMPCHEM_FRIEND_MAKER_HPP
#define COMPCHEM_FRIEND_MAKER_HPP

// ------------------------------------------------------------
// class: friend_maker
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace utility {

/**
 * Class to enable another template class to make a friend of one of its parameter
 * classes.
 */
template<class T_friend>
struct friend_maker
{
    typedef T_friend friend_type;


};

} // namespace utility
#endif
