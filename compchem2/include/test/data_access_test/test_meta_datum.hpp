#ifndef COMPCHEM_TEST_META_DATUM_HPP
#define COMPCHEM_TEST_META_DATUM_HPP

// ------------------------------------------------------------
// class: test_meta_datum
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * Test meta_datum
 */
#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace data_access { class meta_datum; } 

namespace data_access {

/**
 * Unit test for meta_datum class.
 */
class test_meta_datum : public utility::unit_test {
  private:
    /**
     * Class exemplar
     */
    static test_meta_datum exemplar;


  public:
    void main_test() const;

    test_meta_datum()
    : utility::unit_test ()
    {}

    ~test_meta_datum()
    {}

    std::string title() const
    {
      return std::string ("data_access::meta_datum class test");
    }

};

} // namespace data_access
#endif
