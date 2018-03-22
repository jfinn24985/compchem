#ifndef BASIC_FILTER_TRAITS_HPP
#define BASIC_FILTER_TRAITS_HPP

// ------------------------------------------------------------
// class: basic_filter_traits
// $revision:$
// $log:$
// ------------------------------------------------------------



#ifdef __cplusplus
extern "C" {
#endif
struct basic_filter_traits;
/**
 * The function type for setting the text content of
 * datum objects.
 */
typedef int (*add_content_t)(void *, char const * const, int);
/**
 * The function type for creating a new datum object that is the child
 * of another object (memo or datum).
 */
typedef void * (*create_datum_t)(void *, char const* const, int a_bufsize);
/**
 * A method with this signature is used to feed the file being processed to the filter.
 */
typedef int (*initialise_t)(basic_filter_traits* a_filter, char const *const a_buffer, int a_bufsize);
/**
 * A method with this signature is used to indicate that the parsing
 * stage is complete.  It should finalise the outputbuf object with any cached 
 * results and call the outputbuf callback before returning.  If called multiple 
 * times the function should short-circuit after the first call.
 * 
 * \param a_cb_data: An object to use as the first parameter to m_output_cb
 */
typedef int (*output_method_t)(basic_filter_traits * a_filter, void * a_data);
/**
 * A method with this signature is used to feed the file being processed to the filter.
 */
typedef int (*process_method_t)(basic_filter_traits* a_filter, char const *const a_buffer, int a_bufsize);
/**
 * A method with this signature is called to clean up filter state before 
 * deleting the object.  This **MUST** set the m_internal_state attribute 
 * and all the callback function pointers of the passed object to NULL. 
 * It can not report any errors.
 */
typedef void (*reset_method_t)(basic_filter_traits*);
/**
 * The function type for setting the hint and ordinal of
 * datum objects.
 */
typedef int (*set_int_attribute_t)(void *, int);
/**
 * The function type for setting the format and scale of
 * datum objects.
 */
typedef int (*set_string_attribute_t)(void *, char const* const, int a_bufsize);
/**
 * The output callback function type.  Object of this type are supplied 
 * by the application to the filter.  Such a function should be called 
 * by the filter in response to the application calling an output_method_t 
 * function at the end of the filtering process.
 */
typedef void (*set_string_output_t)(void * a_parent, char const *const a_result, int a_sz);
/**
 * This structure is used to communicate between the filter manager and 
 * the filter libraries.  It is a "C" structure that contains "C" style callback
 *  functions and a pointer to filter state.  This pointer is considered 
 * opaque by the filter manager and can be used by the library to hold its 
 * state object.
 * 
 * DIRECT_OUTPUT is always the preferred output method.
 * 
 * The contract between the application and plugin are:
 * * The data from m_identity to m_flags is owned and controlled by the 
 * plugin. The application may read these variables but will not change them.
 * * The data from m_output_cb to m_set_scale_cb is owned and controlled
 * by the application. The plugin may read these variable but must not
 * change them.
 * * Only m_error and m_internal_state may contain allocated memory.
 * * Only after calling m_initialise may m_internal_state be non-nul.
 * * After calling m_finalise m_internal_state and m_error must be nul.
 */
struct basic_filter_traits {
    /**
     * A string containing information about the plugin.
     */
    const char* m_identity;

    /**
     * When an error occurs, this will point to a textual 
     * message about the error.
     */
    const char* m_error;

    /**
     * This pointer should be used by the filter library to hold its state
     * information. It is not accessed by the filter manager.
     * 
     * This is the only pointer in the structure that may be NULL.
     */
    void * m_internal_state;

    /**
     * This initialises the plugin. It gets the text from a 'detect' filter as
     * its argument and should initialise itself accordingly. If it is not 
     * suitable for this file it can return UNUSABLE and remain in the
     * 'nul' state.
     */
    initialise_t m_initialise;

    /**
     * A pointer to the method to use to filter the input. When this
     * is LINE_ORIENTED or BUFFER_ORIENTED then the passed
     * character pointer contains a piece of the target file. When this
     * is FILE_ORIENTED then the passed character pointer is the
     * the filename
     */
    process_method_t m_process;

    /**
     * The method to transfer the filtered data from the filter plugin to the filter 
     * manager. When this is a STRING_OUTPUT method then the second 
     * argument to the function should be passed back as the first argument 
     * of m_output_cb. When this is a DIRECT_OUTPUT method then the 
     * second rgument is the memo object to add results to.
     */
    output_method_t m_output;

    /**
     * A method to clean-up the plugin state. This resets the plugin
     * object to its 'nul' state.
     */
    reset_method_t m_reset;

    /**
     * A set of state flags of type plugin_flag
     */
    int m_flags;

    /**
     * This function pointer is set by the application when the plugin has mode 
     * OUTPUT_STRING.  It should be called by the filter to return the 
     * result of a filter operation. It will be nul in OUTPUT_DIRECT mode.
     */
    set_string_output_t m_output_cb;

    /**
     * This function pointer is set by the application when the plugin has mode 
     * OUTPUT_DIRECT.  It should be called by the filter to create a new datum
     * child of its first argument. It will be nul in OUTPUT_STRING mode.
     */
    create_datum_t m_create_datum_cb;

    /**
     * This function pointer is set by the application when the plugin has mode 
     * OUTPUT_DIRECT.  It should be called by the filter to create add text 
     * content to a datum object created by m_create_datum_cb. It will be nul 
     * in OUTPUT_STRING mode.
     */
    add_content_t m_add_content_cb;

    /**
     * This function pointer is set by the application when the plugin has mode 
     * OUTPUT_DIRECT.  It should be called by the filter to set the format 
     * attribute of a datum object created by m_create_datum_cb. It will be nul 
     * in OUTPUT_STRING mode.
     */
    set_string_attribute_t m_set_format_cb;

    /**
     * This function pointer is set by the application when the plugin has mode 
     * OUTPUT_DIRECT.  It should be called by the filter to set the ordinal
     * attribute of a datum object created by m_create_datum_cb. It will be nul 
     * in OUTPUT_STRING mode.
     */
    set_int_attribute_t m_set_ordinal_cb;

    /**
     * This function pointer is set by the application when the plugin has mode 
     * OUTPUT_DIRECT.  It should be called by the filter to set the scale
     * attribute of a datum object created by m_create_datum_cb. It will be nul 
     * in OUTPUT_STRING mode.
     */
    set_string_attribute_t m_set_scale_cb;

};
#ifdef __cplusplus
}
#endif
#endif
