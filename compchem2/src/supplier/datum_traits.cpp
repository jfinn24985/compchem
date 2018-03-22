// ------------------------------------------------------------
// class: datum_traits
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/datum_traits.hpp"

// Manual includes
#include <boost/lexical_cast.hpp>
#include "data_scheme/basic_datum.hpp"
#include "data_access/names.hpp"
// -
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
void * datum_traits::create(void * a_node, char const* const a_outcome, int a_sz)

{
  data_scheme::basic_datum * Result (NULL);
  if (NULL != a_node)
  {
    try
    {
      std::string l_outcome (a_outcome, a_sz);
      data_scheme::basic_datum * l_node (reinterpret_cast < data_scheme::basic_datum * >(a_node));
      Result = &(l_node->add_child (data_access::names::datum_name (), l_outcome));
    }
    catch (const std::exception & a_err)
    {
      Result = NULL;
      utility::implementation::log () << "Error in create_datum: " << a_err.what () << "\n";
    }
    catch (...)
    {
      Result = NULL;
      utility::implementation::log () << "Unknown error in create_datum.\n";
    }
  }
  return reinterpret_cast< void * > (Result);
  

}

/**
 * Set the text contents of the given node. This will insert the
 * text at the current "position".
 * 
 * \param The new content string
 * \return Error indicator (0 == success, 0 /= error)
 */
int datum_traits::add_content(void * a_node, char const * const a_content, int a_sz)

{
  int Result (0);
  if (NULL != a_node)
  {
    try
    {
      const std::string l_content (a_content, a_sz);
      data_scheme::basic_datum * l_node (static_cast < data_scheme::basic_datum * >(a_node));
      l_node->add_text (l_content);
    }
    catch (const std::exception & a_err)
    {
      Result = 1;
      utility::implementation::log () << "Error: " << a_err.what () << "\n";
    }
    catch (...)
    {
      Result = 1;
      utility::implementation::log () << "Unknown error\n";
    }
  }
  return Result;
  

}

/**
 * Set the format attribute of the given node. This will throw
 * an exception in C++ if a_node is not a datum node! 
 * 
 * \param The new format string
 * \return Error indicator (0 == success, 0 /= error)
 */
int datum_traits::set_format(void * a_node, char const * const a_format, int a_sz)

{
  int Result (0);
  if (NULL != a_node)
  {
    try
    {
      const std::string l_format (a_format, a_sz);
      data_scheme::basic_datum * l_node (static_cast < data_scheme::basic_datum * >(a_node));
      l_node->set_attribute (data_access::names::format_name (), l_format);
    }
    catch (const std::exception & a_err)
    {
      Result = 1;
      utility::implementation::log () << "Error: " << a_err.what () << "\n";
    }
    catch (...)
    {
      Result = 1;
      utility::implementation::log () << "Unknown error\n";
    }
  }
  return Result;
  

}

/**
 * Set the ordinal attribute of the given node. This will throw
 * an exception in C++ if a_node is not a datum node! 
 * 
 * \param The ordinal, (> 0)
 * \return Error indicator (0 == success, 0 /= error)
 */
int datum_traits::set_ordinal(void * a_node, int a_ordinal)

{
  int Result (0);
  if (NULL != a_node)
  {
    try
    {
      data_scheme::basic_datum * l_node (static_cast < data_scheme::basic_datum * >(a_node));
      l_node->set_attribute (data_access::names::ordinal_name (), boost::lexical_cast < std::string > (a_ordinal));
    }
    catch (const std::exception & a_err)
    {
      Result = 1;
      std::cout << "Error: " << a_err.what () << "\n";
    }
    catch (...)
    {
      Result = 1;
      std::cout << "Unknown error\n";
    }
  }
  return Result;
  

}

/**
 * Set the scale attribute of the given node. This will throw
 * an exception in C++ if a_node is not a datum node! 
 * 
 * \param The new scale string
 * \return Error indicator (0 == success, 0 /= error)
 */
int datum_traits::set_scale(void * a_node, char const * const a_scale, int a_sz)

{
  int Result (0);
  if (NULL != a_node)
  {
    try
    {
      const std::string l_scale (a_scale, a_sz);
      data_scheme::basic_datum * l_node (static_cast < data_scheme::basic_datum * >(a_node));
      l_node->set_attribute (data_access::names::scale_name (), l_scale);
    }
    catch (const std::exception & a_err)
    {
      Result = 1;
      std::cout << "Error: " << a_err.what () << "\n";
    }
    catch (...)
    {
      Result = 1;
      std::cout << "Unknown error\n";
    }
  }
  return Result;
  

}

