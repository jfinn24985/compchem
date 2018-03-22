#ifndef COMPCHEM_DATUM_TRAITS_HPP
#define COMPCHEM_DATUM_TRAITS_HPP

// ------------------------------------------------------------
// class: datum_traits
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
/**
 * This provides a series of functions for managing datum objects that can 
 * be called from "C". These are given to the plugin as the m_create_datum_cb
 * ... m_set_scale_cb and enable the implementation of the OUTPUT_DIRECT 
 * mode. The creation, attributes and content can all be set. Note that the 
 * navigation of the tree is not supported so must be managed by the caller.
 */
class datum_traits
{
  public:
    /**
     * This method creates and returns a new datum object as a child
     * of the given object.
     * 
     * \param a_node The parent node of the new datum.
     * \param a_outcome The value of the new datum's outcome attribute.
     * \return The newly create datum object (ptr owned by parent).
     * 
     * \errors Will return NULL if an error occurs.
     */
    static void * create(void * a_node, char const* const a_outcome, int a_sz);

    /**
     * Set the text contents of the given node. This will insert the
     * text at the current "position".
     * 
     * \param The new content string
     * \return Error indicator (0 == success, 0 /= error)
     */
    static int add_content(void * a_node, char const * const a_content, int a_sz);

    /**
     * Set the format attribute of the given node. This will throw
     * an exception in C++ if a_node is not a datum node! 
     * 
     * \param The new format string
     * \return Error indicator (0 == success, 0 /= error)
     */
    static int set_format(void * a_node, char const * const a_format, int a_sz);

    /**
     * Set the ordinal attribute of the given node. This will throw
     * an exception in C++ if a_node is not a datum node! 
     * 
     * \param The ordinal, (> 0)
     * \return Error indicator (0 == success, 0 /= error)
     */
    static int set_ordinal(void * a_node, int a_ordinal);

    /**
     * Set the scale attribute of the given node. This will throw
     * an exception in C++ if a_node is not a datum node! 
     * 
     * \param The new scale string
     * \return Error indicator (0 == success, 0 /= error)
     */
    static int set_scale(void * a_node, char const * const a_scale, int a_sz);


};
#endif
