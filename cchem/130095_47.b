class newcalc
!!!141231.cpp!!!	do_initialise_application(in a_argc : int*, in a_argv : char**) : void
// Add notify method
class helper
{
public:
  static void on_write(newcalc* a_self, Widget, void*) { a_self->write_scripts (); }
  static void on_exit(Widget, void*) { exit (0); }
  static void on_change (newcalc* a_self, int a_value) 
  {
    std::cout << "Oniom changed to [" << a_value << "]" << std::endl;
    a_self->oniom.set_editable (a_value != 0);
    a_self->method.set_editable (a_value == 0);
    a_self->basis.set_editable (a_value == 0);
  }
};
// Add some functions
add_notify_binding ("data_write", boost::bind(&helper::on_write, this, _1, _2));
add_notify_binding ("application_close", &helper::on_exit);
// Connect some controls together
use_oniom.add_callback(boost::bind(&helper::on_change, this, _1));
!!!143407.cpp!!!	write_scripts() : void
// LOCAL
script_writer(*this).generate ();
