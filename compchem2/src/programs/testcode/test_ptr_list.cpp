#include <boost/ptr_container/ptr_list.hpp>
#if defined(USE_LAMBDA)
#include <boost/lambda/lambda.hpp>
#elif defined(USE_BIND)
#include <boost/bind.hpp>
#else
#endif
#include <string>
#include <iostream>
#include <algorithm>

class A
{
private:
  std::string m_what;
public:
  A(const std::string & w)
  : m_what (w)
  {
    std::cout << "Constructor [" << m_what << "]\n";
  }
  ~A()
  {
    std::cout << "Destructor [" << m_what << "]\n";
  }
  const std::string & what () const { return m_what; }
  void what (const std::string & w) { m_what = w; }
private:
  A(const A &);
  A & operator= (const A &);
};

#if defined(USE_LAMBDA)
#elif defined(USE_BIND)
bool compare (const A & a1, const A & a2)
{
  return &a1 == &a2;
}
#else
bool operator== (const A & a1, const A & a2)
{
  return &a1 == &a2;
}
#endif
int main (int argc, char** argv)
{
#if defined(USE_LAMBDA) 
using namespace boost::lambda;
#endif
  std::cout << "main entry\n";
  boost::ptr_list<A> l_vec;
  boost::ptr_list<A>::iterator l_i;
  std::cout << "created vector\n";
  l_vec.push_back (new A("one"));
  std::cout << "added element to vector\n";
  A * l_aptr (new A("two"));
  l_vec.push_back (l_aptr);
  std::cout << "added second element to vector\n";
  std::cout << "Second element is = " << l_aptr->what () << "\n";
  std::cout << "About to remove second element.\n";
#if defined(USE_LAMBDA)
  l_i = std::find_if (l_vec.begin (), l_vec.end (), &_1 == l_aptr);
#elif defined (USE_BIND)
  l_i = std::find_if (l_vec.begin (), l_vec.end (), boost::bind(&compare, boost::ref(*l_aptr), _1));
#else
  l_i = std::find (l_vec.begin (), l_vec.end (), *l_aptr);
#endif
  if (l_vec.end () != l_i)
  {
    std::cout << "found second element.\n";
    l_vec.erase (l_i);
    std::cout << "erased second element.\n";
  }
  A l_a1 ("three");
  std::cout << "try to find a non element.\n";
#if defined(USE_LAMBDA)
  l_i = std::find_if (l_vec.begin (), l_vec.end (), &_1 == &l_a1);
#elif defined (USE_BIND)
  l_i = std::find_if (l_vec.begin (), l_vec.end (), boost::bind(&compare, boost::ref(l_a1), _1));
#else
  l_i = std::find (l_vec.begin (), l_vec.end (), l_a1);
#endif
  if (l_vec.end () == l_i)
  {
    std::cout << "did not find unknown element.\n";
  }
  std::cout << "done.\n";
  // This segmentation faults as the object l_aptr points to is deleted when erased.
  // std::cout << "Second element is = " << l_aptr->what () << "\n";
  return 0;
} 

