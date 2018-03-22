// ------------------------------------------------------------
// class: test_offsetter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/supplier_test/test_offsetter.hpp"

// Manual includes
#include <boost/scoped_array.hpp>
// -
namespace supplier {

/**
 * The class exemplar.
 */
test_offsetter test_offsetter::s_exmplr;

/**
 * Override in derived classes to perform unit test.
 */
void test_offsetter::main_test() const {
  offsetter l_test ("0", "160");
  log () << TTLE << "Testing numerical first/last offsetter [0, 160]\n";
  bool_test (0 == l_test.first (NULL, 256, 0), "Start is at 0 for 'initial' buffer");
  bool_test (160 == l_test.last (NULL, 256, 0), "End is at 160 for 'initial' buffer");
  bool_test (0 == l_test.first (NULL, 256, 170), "Start is 0 for past-last buffer.");
  bool_test (0 == l_test.last (NULL, 256, 170), "End is 0 for past-end buffer.");
  bool_test (80 == l_test.last (NULL, 256, 80), "End is 80 for buffer 80 from global start.");
  offsetter l_test2 ("first", "last");
  log () << TTLE << "Testing always first/last offsetter\n";
  bool_test (0 == l_test.first (NULL, 256, 0), "Start is at 0 for 'initial' buffer");
  bool_test (257 == l_test.last (NULL, 256, 0), "End is at 257 for 'initial' buffer [size = 256]");
  bool_test (0 == l_test.first (NULL, 256, 170), "Start is still 0 for next buffer.");
  bool_test (257 == l_test.last (NULL, 256, 170), "End is still 257 for next buffer.");
  try
  {
    log () << TTLE << "Attempting to make a bad offsetter ('start, -10')\n";
    offsetter l_test ("start", "-10");
    log () << IFNO << "Should not get here\n";
  }
  catch (const std::exception & a_err)
  {
    log () << IFYS << "Caught error: " << a_err.what () << "\n";
  }
  try
  {
    log () << TTLE << "Attempting to make a bad offsetter ('StaRt, 10')\n";
    offsetter l_test ("StaRt", "10");
    log () << IFNO << "Should not get here\n";
  }
  catch (const std::exception & a_err)
  {
    log () << IFYS << "Caught error: " << a_err.what () << "\n";
  }
  const unsigned int l_step (128);
  const unsigned int l_sz (4 * l_step);
  std::string l_buf;
  l_buf.resize (l_sz);
  for (unsigned int l_ix = 0; l_ix < l_sz; ++l_ix)
  {
    l_buf[l_ix] = ((rand () % 30) + 'a' - 4);
  }
  log () << COMM << "Test buffer.\n";
  log () << l_buf << "\n";
  log () << COMM << "Testing first and last\n";
  for (unsigned int l_ix = 0, l_end = l_step;
        l_ix < l_sz and l_ix < l_end; // Note l_sz is multiple of l_step
        l_ix += l_step)
  {
    switch (l_test.first (l_buf.data () + l_ix, l_step, l_ix))
    {
    case 0:
      log () << WTCH << "At offset [" << l_ix << "] first was 0\n";
      break;
    default:
      log () << WTCH << "At offset [" << l_ix << "] first was not 0\n";
      break;
    }
    l_end = l_test.last (l_buf.data () + l_ix, l_step, l_ix);
    switch (l_end)
    {
    case l_step + 1:
      log () << WTCH << "At offset [" << l_ix << "] last was step + 1\n";
      break;
    default:
      log () << WTCH << "At offset [" << l_ix << "] last was not step + 1\n";
      break;
    }
    if (l_ix + l_step >= l_end)
    {
      log () << WTCH << "Reached end at [" << l_end << "] at offset [" << l_ix << "]\n";
    }
  }
}


} // namespace supplier
