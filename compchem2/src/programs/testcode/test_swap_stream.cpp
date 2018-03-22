// ------------------------------------------------------------
// Test swapping of std::stringstream buffers.
// $revision:$
// $log:$
// ------------------------------------------------------------

#include <iostream>
#include <sstream>

int main (int argc, char** argv)
{
  std::stringstream s1, s2;
  s1 << "Hello World.";
  s2 << "Goodbye cruel world.";
  std::cout << "S1 = \"" << s1.str () << "\"\n";
  std::cout << "S2 = \"" << s2.str () << "\"\n";
  // std::basic_streambuf<std::stringstream::char_type> * l_tmp = s1.rdbuf ();
  // s1.init (s2.rdbuf ());
  // s2.init (l_tmp);
  std::cout << "Swapped stream buffers is not possible.\n";
  std::cout << "S1 = \"" << s1.str () << "\"\n";
  std::cout << "S2 = \"" << s2.str () << "\"\n";
  return 0;
}
