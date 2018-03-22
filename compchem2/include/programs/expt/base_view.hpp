#ifndef BASE_VIEW_HPP
#define BASE_VIEW_HPP

// ------------------------------------------------------------
// class: base_view
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace expt {

/**
 * This is the base class for view types.  The main responsibility assigned to view types in the design is to transfer data between one or more UI objects and a control object.
 */
class base_view {
  public:
    typedef Gtk::Widget*  window_t;

    typedef Gtk::Widget*const  const_window_t;

    typedef Glib::ustring string_t;


  private:
    window_t rep;

    mutable bool change_flag;


  public:
    /**
     * Constructor for UI application object.  a_url gives the location of the XML document containing the UI description. a_key gives the name of the window to instantiate as the top-level window. For simple applications this will only be invoked once.
     * 
     * \pre is_readable_file (a_url) [not directly tested]
     * \pre is_valid_key (a_key) [not directly tested]
     */
    base_view(const string_t & a_key, const string_t & a_url);

    /**
     * General constructor for sub-windows.
     */
    base_view(const string_t & a_key, const base_view & a_parent);


  private:
    /**
     * Not defined, added to stop copying.
     */
    base_view(const base_view & source);


  public:
    inline virtual ~base_view() {};

    /**
     * Get the GUI-object.
     */
    inline window_t get_rep() {
          return rep;
        };

    /**
     * Get the GUI-object in constant form
     */
    inline const_window_t get_rep() const {
          return const_window_t (rep);
        };

    /**
     * Are there edits to UI content that has not been passed to a control object?
     * 
     * \post Result
     */
    inline virtual bool is_changed() const {
          return change_flag;
        };


  protected:
    /**
     * Test class level invariant conditions 
     * 
     * \inv get_rep() /= nul
     */
    inline void is_valid() const {
          INVARIANT (rep != NULL, "Cannot have a view object without a valid UI object.");
        };


  public:
    /**
     * Indicate what data has been changed with a_flag.  The given value is ORed to the change_flag value.  This can be connected to a signal on a UI object that indicates editing.
     */
    inline void on_change() {
           change_flag = true;
         };


  protected:
    /**
     * Indicate that data has been changed.  This is the version that can be called during the is_changed method if it performs tests for changes..
     */
    inline void on_change() const {
           change_flag = true;
         };


  private:
    /**
     * Not defined, added to stop copying.
     */
    base_view & operator =(const base_view & source);


  public:
    /**
     * Reset the change flag (to zero) to indicate the UI object hold the same data as the control object.
     */
    inline void reset_change_flag() {
          change_flag = false;
        };

};

} // namespace expt
#endif
