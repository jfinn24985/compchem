#ifndef UI_TRAITS_HPP
#define UI_TRAITS_HPP

// ------------------------------------------------------------
// class: ui_traits
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/base_view.hpp"
#include <string>

namespace expt {

class ui_traits {
  public:
    typedef Gtk::Widget*  window_t;

    typedef Gtk::Widget*const  const_window_t;

    typedef Glib::ustring string_t;

    class ask_info {
      public:
        const base_view::string_t message;

        const base_view::string_t help_message;

        const base_view::string_t yes;

        const base_view::string_t no;

        const base_view::string_t cancel;

        inline ask_info(const base_view::string_t & a_message, const base_view::string_t & a_help, const base_view::string_t & a_yes, const base_view::string_t & a_no, const base_view::string_t & a_cancel) : message(a_message)
                , help_message(a_help)
                , yes(a_yes)
                , no(a_no)
                , cancel(a_cancel)
                {};

    };
    
    /**
     * Ask the user for a piece of information with initial value given by and returned in a_value.  If you do not specify (or specify an empty string) Yes/No/Cancel buttons titles then the corresponding button is not diplayed. If not titles are set then a default "Ok" button will be used (which returns YES).
     * 
     * Note that the NO value is returned if the dialog is closed by the window manager.
     * \post result == YES, NO or CANCEL
     */
    static unsigned int ask_query(const ask_info & a_message, std::string & a_value);

    /**
     * Ask the user for a filename or URL.  Use a_message to inform the user the reason for request.  If a_message is an empty string then a default message is displayed.
     * 
     * The a_filename is used to obtain an initial filename.  It is used as the starting location presented to the user.  If it is empty then the current working directory is displayed.  When a filename is selected a_filename set to the value.
     * 
     * TODO: ensure a_filename is a valid path.  If a_filename is an empty string, use the current working directory.
     * 
     * If Yes or Cancel button titles are empty strings then default buttons will be used.  If No button title is empty then no corresponding button will be displayed. 
     * 
     * \param a_is_read  Whether the file will be read or writen to.  When a_is_read is true then the document must already exist.  When false then a non-existant document may be chosen. (Default is false)
     * \post result == YES, NO or CANCEL
     */
    static unsigned int ask_url(const ask_info & a_message, std::string & a_filename, bool a_is_read);

    /**
     * Ask the user a yes/no question with option to cancel.  For displaying information messages do not set the Yes/No/Cancel buttons titles then a default "Ok" button will be used and the return value will be relatively meaningless.
     * 
     * Note that the NO value is returned if the dialog is closed by the window manager.
     * \post result == YES, NO or CANCEL 
     */
    static unsigned int ask_user(const ask_info & a_message);

    /**
     * Convert a date into a UI string.
     */
    static base_view::string_t date_to_text(const boost::gregorian::date & a_date);

    /**
     * Find or create a UI object based on a_key.  The UI object will be created relative to a_parent.
     * 
     * \pre a_key.empty () /= true
     * \pre a_parent /= nul
     */
    static base_view::window_t find_representation(const base_view::string_t & a_key, base_view::const_window_t a_parent);

    /**
     * Instantiate UI library and load UI from a_url.  Instantiate window located using a_key as the top-level window.  Return the window.
     * 
     * \pre a_key.empty () /= true amd a_url.empty ()
     * 
     * \post Result /= nul
     */
    static base_view::window_t find_representation(const base_view::string_t & a_key, const base_view::string_t & a_url);


  private:
    /**
     * This converts a Gtk::Response value into Yes/No/Cancel.
     * 
     * \post Result == YES or NO or CANCEL
     */
    static unsigned int response_conv(int a_response);


  public:
    /**
     * Convert a UI string into a date.
     */
    static boost::gregorian::date text_to_date(const base_view::string_t & a_str_date);

    static const unsigned int NO;

    static const unsigned int CANCEL;

    static const unsigned int YES;

};

} // namespace expt
#endif
