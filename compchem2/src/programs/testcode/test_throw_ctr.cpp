#include <iostream>
#include <stdexcept>
// This program tests whether a pass-by-value function that
// throws in the copy-ctor of the argument is thrown by
// the function.

class test_throw
{
  int m_val;
public:
  test_throw (int a_val):m_val (a_val)
  {
  }
  ~test_throw ()
  {
  }

  void foo (test_throw a) throw ()
  {
    m_val = 0;
  }

  void bar (const test_throw & a) throw ()
  {
    foo (a);
  }

  int val () throw ()
  {
    return m_val;
  }

  test_throw (const test_throw & source):m_val (source.m_val)
  {
    throw std::logic_error ("Throwing error in the copy ctor");
  }
};

int
main (int argc, char **argv)
{
  try
  {
    test_throw l_a (3), l_b (40);
    std::cout << "A = " << l_a.val () << "\nB = " << l_b.val () << std::endl;
    try
    {
      std::cout << "Calling A.foo (B) should not be caught if copy-ctr called 'in' foo" << std::endl;
      l_a.foo (l_b);
    }
    catch (const std::exception & a_err)
    {
      std::cout << "Caught error: " << a_err.what () << std::endl;
    }
    try
    {
      std::cout << "Calling A.bar (B) should not be caught as copy-ctor called in no-throw method" << std::endl;
      l_a.bar (l_b);
    }
    catch (const std::exception & a_err)
    {
      std::cout << "Caught error: " << a_err.what () << std::endl;
    }
    std::cout << "A = " << l_a.val () << "\nB = " << l_b.val () << std::endl;
  }
  catch (const std::exception & a_err)
  {
    std::cout << "Caught error: " << a_err.what () << std::endl;
  }
  return 0;
}
