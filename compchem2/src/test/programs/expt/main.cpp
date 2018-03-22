int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  ExampleWindow window;
  Gtk::Main::run(window); //Shows the window and returns when it is closed.

  return 0;
}
