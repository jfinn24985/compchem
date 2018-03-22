// ------------------------------------------------------------
// class: basic_plugin
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/filter/basic_plugin.hpp"
#include "supplier/supplier_error.hpp"

namespace supplier {

namespace filter {

/**
 * Reset the plugin.
 * 
 * \pre nul /= a_plugin
 * \post a_plugin->internal_state = nul and a_plugin->error = nul
 */
void basic_plugin::finalise(::basic_filter_traits * a_filter)

{
  if (NULL != a_filter)
  {
    if (NULL != a_filter->m_internal_state)
    {
      delete static_cast < basic_plugin * >(a_filter->m_internal_state);
      a_filter->m_internal_state = NULL;
    }
    if (NULL != a_filter->m_error)
    {
      delete[]a_filter->m_error;
      a_filter->m_error = NULL;
    }
  }

}

/**
 * Output the result.
 */
int basic_plugin::output(::basic_filter_traits * a_filter, void * a_cb_data)

{
  // DO
  int Result (CCHEM_SUPPLIER_SUCCESS);
  if (NULL == a_filter)
  {
    Result = CCHEM_SUPPLIER_FATAL;
  }
  else if (NULL == a_filter->m_internal_state)
  {
    set_error (a_filter, "Invalid state, attempt to get result from nul or uninitialised filter.");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  else
  {
    try
    {
      Result = static_cast < basic_plugin * >(a_filter->m_internal_state)->output_default (a_filter, a_cb_data);
    }
    catch (const supplier_error &a_err)
    {
      set_error (a_filter, a_err.what ());
      Result = a_err.level;
    }
     catch (const std::exception &a_err)
    {
      set_error (a_filter, a_err.what ());
      Result = CCHEM_SUPPLIER_FATAL;
    }
    catch (...)
    {
      set_error (a_filter, "Unknown error");
      Result = CCHEM_SUPPLIER_FATAL;
    }
  }
  return Result;

}

/**
 * Process a piece of the file.
 * 
 * \pre nul /= a_plugin
 */
int basic_plugin::process(::basic_filter_traits *const a_filter, char const *const a_buffer, int a_sz)

{
  int Result (CCHEM_SUPPLIER_SUCCESS);
  try
  {
    if (NULL == a_filter)
    {
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else if (NULL == a_filter->m_internal_state)
    {
      set_error (a_filter, "Uninitialised plugin object passed as argument to process.");
      Result = CCHEM_SUPPLIER_FATAL;
    }
    else
    { 
      Result = static_cast< basic_plugin * >(a_filter->m_internal_state)->process_default (a_filter, a_buffer, a_sz);
    }
  }
  catch (const supplier_error &a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = a_err.level;
  }
   catch (const std::exception a_err)
  {
    set_error (a_filter, a_err.what ());
    Result = CCHEM_SUPPLIER_FATAL;
  }
  catch (...)
  {
    set_error (a_filter, "Unknown error.");
    Result = CCHEM_SUPPLIER_FATAL;
  }
  return Result;
;
}

/**
 * Set the plugin's error message from a_msg.
 */
void basic_plugin::set_error(::basic_filter_traits *const a_filter, std::string a_msg)

{
  if (not a_msg.empty ())
  {
    if (NULL != a_filter->m_error)
    {
      a_msg.insert (0, "\n");
      a_msg.insert (0, a_filter->m_error); // Combine error messages
      delete [] a_filter->m_error;
      a_filter->m_error = NULL;
    } 
    char *l_error = new char[a_msg.size () + 1];
    a_msg.copy (l_error, a_msg.size (), 0);
    *(l_error + a_msg.size ()) = '\0';
    a_filter->m_error = l_error;
  }

}


} // namespace supplier::filter

} // namespace supplier
