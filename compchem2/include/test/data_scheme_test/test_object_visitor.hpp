#ifndef COMPCHEM_DATA_SCHEME_TEST_OBJECT_VISITOR_HPP
#define COMPCHEM_DATA_SCHEME_TEST_OBJECT_VISITOR_HPP

// ------------------------------------------------------------
// class: test_object_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/visitor.hpp"
#include "utility/unit_test.hpp"
#include "data_access/names.hpp"
#include <string>

namespace utility { class program_options; } 
namespace data_scheme { class formula; } 
namespace data_scheme { class basic_datum; } 
namespace data_scheme { class comment; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class compound_ref; } 
namespace data_scheme { class datum; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class filter; } 
namespace data_scheme { class identity; } 
namespace data_scheme { class info; } 
namespace data_scheme { class mechanism; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class processing_instruction; } 
namespace data_scheme { class program; } 
namespace data_scheme { class project; } 
namespace data_scheme { class settings; } 
namespace data_scheme { class text; } 
namespace data_scheme { class worker; } 
namespace data_scheme { class basic_text; } 

namespace data_scheme {

/**
 * This version of data_scheme::visitor performs type specific tests on every
 * object it encounters. It is the main test vehicle for the data_scheme classes.
 * 
 * This class is a test for 'visit' method of basic_datum objects.
 */
class test_object_visitor : public visitor, public utility::unit_test
{
  public:
    enum
    {
      s_size = 18

    };


  private:
    /**
     * Indicate that the visitor should only be called
     * for comment objects. (== 1)
     */
    static const unsigned char s_comment;

    /**
     * Indicate that the visitor should only be called
     * for compound objects. (== 2)
     */
    static const unsigned char s_compound;

    /**
     * Indicate that the visitor should only be called
     * for compound-ref objects. (== 3)
     */
    static const unsigned char s_compound_ref;

    /**
     * Indicate that the visitor should only be called
     * for datum objects. (== 4)
     */
    static const unsigned char s_datum;

    /**
     * Indicate that the visitor should only be called
     * for experiment objects. (== 5)
     */
    static const unsigned char s_experiment;

    /**
     * Indicate that the visitor should only be called
     * for filter objects. (== 6)
     */
    static const unsigned char s_filter;

    /**
     * Indicate that the visitor should only be called
     * for formula objects. (== 7)
     */
    static const unsigned char s_formula;

    /**
     * Indicate that the visitor should only be called
     * for identity objects. (== 8)
     */
    static const unsigned char s_identity;

    /**
     * Indicate that the visitor should only be called
     * for info objects. (== 9)
     */
    static const unsigned char s_info;

    /**
     * Indicate that the visitor should only be called
     * for mechanism objects. (== 10)
     */
    static const unsigned char s_mechanism;

    /**
     * Indicate that the visitor should only be called
     * for memo objects. (== 11)
     */
    static const unsigned char s_memo;

    /**
     * Indicate that the visitor should only be called
     * for p.i. objects. (== 12)
     */
    static const unsigned char s_pi;

    /**
     * Indicate that the visitor should only be called
     * for program objects. (== 13)
     */
    static const unsigned char s_program;

    /**
     * Indicate that the visitor should only be called
     * for project objects. (== 14)
     */
    static const unsigned char s_project;

    /**
     * Indicate that the visitor should only be called
     * for settings objects. (== 15)
     */
    static const unsigned char s_settings;

    /**
     * Indicate that the visitor should only be called
     * for text objects. (== 16)
     */
    static const unsigned char s_text;

    /**
     * Indicate that the visitor should only be called
     * for worker objects. (== 17)
     */
    static const unsigned char s_worker;

    /**
     * The class exemplar
     */
    static test_object_visitor s_exemplar;

    /**
     * The object-type counts
     */
    unsigned int m_type[s_size];

    /**
     * The filename of a database file.
     */
    std::string m_doc_filename;

    /**
     * Some formula
     */
    mutable const formula * m_formula;


  public:
    /**
     * Default ctor
     */
    test_object_visitor();

    ~test_object_visitor();


  private:
    /**
     * no copy
     */
    test_object_visitor(const test_object_visitor & source);

    /**
     * no assign
     */
    test_object_visitor & operator=(const test_object_visitor & source);


  public:
    /**
     * Test basic_datum methods for derived classes that can have children.
     */
    void visit_basic_datum(const basic_datum & a_datum);

    /**
     * Operate on comment objects.
     */
    void visit_comment(const comment & a_datum);

    /**
     * Operate on compound objects.
     */
    void visit_compound(const compound & a_datum);

    /**
     * Operate on compound ref objects.
     */
    void visit_compound_ref(const compound_ref & a_datum);

    /**
     * Operate on datum objects.
     */
    void visit_datum(const datum & a_datum);

    /**
     * Operate on experiment objects.
     */
    void visit_experiment(const experiment & a_datum);

    /**
     * Operate on filter objects.
     */
    void visit_filter(const filter & a_datum);

    /**
     * Operate on formula objects.
     */
    void visit_formula(const formula & a_datum);

    /**
     * Operate on identity objects.
     */
    void visit_identity(const identity & a_datum);

    /**
     * Operate on info objects.
     */
    void visit_info(const info & a_datum);

    /**
     * Operate on mechanism objects.
     */
    void visit_mechanism(const mechanism & a_datum);

    /**
     * Operate on memo objects.
     */
    void visit_memo(const memo & a_datum);

    /**
     * Operate on processing_instruction objects.
     */
    void visit_pi(const processing_instruction & a_datum);

    /**
     * Operate on program objects.
     */
    void visit_program(const program & a_datum);

    /**
     * Operate on project objects.
     */
    void visit_project(const project & a_datum);

    /**
     * Operate on settings objects.
     */
    void visit_settings(const settings & a_datum);

    /**
     * Operate on text objects.
     */
    void visit_text(const text & a_datum);

    /**
     * Operate on worker objects.
     */
    void visit_worker(const worker & a_datum);

    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();

    /**
     * Test a basic_text object
     */
    void test_basic_text(const basic_text & a_datum);


  public:
    std::string title() const
    {
      return std::string ("data_scheme schema class tests");
    }

    /**
     * Print the number of types analysed.
     */
    void show_coverage() const;


};

} // namespace data_scheme
#endif
