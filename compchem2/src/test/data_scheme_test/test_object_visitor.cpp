// ------------------------------------------------------------
// class: test_object_visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/data_scheme_test/test_object_visitor.hpp"
#include <boost/bind.hpp>

#include "utility/program_options.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/basic_datum.hpp"
#include "data_scheme/text.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/compound_ref.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/settings.hpp"
#include "data_scheme/worker.hpp"

// Manual includes
#include <boost/lexical_cast.hpp>
// - 
namespace data_scheme {

/**
 * Indicate that the visitor should only be called
 * for comment objects. (== 1)
 */
const unsigned char test_object_visitor::s_comment(1);

/**
 * Indicate that the visitor should only be called
 * for compound objects. (== 2)
 */
const unsigned char test_object_visitor::s_compound(2);

/**
 * Indicate that the visitor should only be called
 * for compound-ref objects. (== 3)
 */
const unsigned char test_object_visitor::s_compound_ref(3);

/**
 * Indicate that the visitor should only be called
 * for datum objects. (== 4)
 */
const unsigned char test_object_visitor::s_datum(4);

/**
 * Indicate that the visitor should only be called
 * for experiment objects. (== 5)
 */
const unsigned char test_object_visitor::s_experiment(5);

/**
 * Indicate that the visitor should only be called
 * for filter objects. (== 6)
 */
const unsigned char test_object_visitor::s_filter(6);

/**
 * Indicate that the visitor should only be called
 * for formula objects. (== 7)
 */
const unsigned char test_object_visitor::s_formula(7);

/**
 * Indicate that the visitor should only be called
 * for identity objects. (== 8)
 */
const unsigned char test_object_visitor::s_identity(8);

/**
 * Indicate that the visitor should only be called
 * for info objects. (== 9)
 */
const unsigned char test_object_visitor::s_info(9);

/**
 * Indicate that the visitor should only be called
 * for mechanism objects. (== 10)
 */
const unsigned char test_object_visitor::s_mechanism(10);

/**
 * Indicate that the visitor should only be called
 * for memo objects. (== 11)
 */
const unsigned char test_object_visitor::s_memo(11);

/**
 * Indicate that the visitor should only be called
 * for p.i. objects. (== 12)
 */
const unsigned char test_object_visitor::s_pi(12);

/**
 * Indicate that the visitor should only be called
 * for program objects. (== 13)
 */
const unsigned char test_object_visitor::s_program(13);

/**
 * Indicate that the visitor should only be called
 * for project objects. (== 14)
 */
const unsigned char test_object_visitor::s_project(14);

/**
 * Indicate that the visitor should only be called
 * for settings objects. (== 15)
 */
const unsigned char test_object_visitor::s_settings(15);

/**
 * Indicate that the visitor should only be called
 * for text objects. (== 16)
 */
const unsigned char test_object_visitor::s_text(16);

/**
 * Indicate that the visitor should only be called
 * for worker objects. (== 17)
 */
const unsigned char test_object_visitor::s_worker(17);

/**
 * The class exemplar
 */
test_object_visitor test_object_visitor::s_exemplar;

/**
 * Default ctor
 */
test_object_visitor::test_object_visitor() 
: visitor ()
, unit_test ()
, m_type ()
, m_doc_filename ()
, m_formula (NULL)
{
  register_options ();
}

test_object_visitor::~test_object_visitor() 
{

}

/**
 * Test basic_datum methods for derived classes that can have children.
 */
void test_object_visitor::visit_basic_datum(const basic_datum & a_datum) 
{
  log () << COMM << "entering on_visit (basic_datum)\n";
  // Count basic_datum objects
  ++m_type[0];
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  // Test object
  {
    basic_datum & l_temp (const_cast< basic_datum & >(a_datum));
    {
      // add_comment
      const static std::string l_s1 ("A stitch in time");
      comment & l_comm (l_temp.add_comment (l_s1));
      // the_content
      bool_test (l_comm.the_content == l_s1, "text content and string match");
      // parent
      bool_test (&a_datum == &(l_comm.parent ()), "parent object matches");
      // find (const)
      bool_test (a_datum.find (&l_comm) != a_datum.end (), "found added comment");
      bool_test (l_orig_size + 1 == l_temp.size (), "additional child object increase size by 1");
      // remove
      l_temp.remove (l_temp.find (&l_comm));
      bool_test (a_datum.find (&l_comm) == a_datum.end (), "did not find removed comment");
    }
    {
      // add_pi
      const static std::string l_s2 ("stylesheet");
      const static std::string l_s3 ("way-to-go");
      processing_instruction & l_pi (l_temp.add_pi (l_s2, l_s3));
      // the_content
      bool_test (l_pi.the_content == l_s3, "text content and string match");
      bool_test (l_pi.name == l_s2, "text content and string match");
      // parent
      bool_test (&a_datum == &(l_pi.parent ()), "parent object matches");
      // find (const)
      bool_test (a_datum.find (&l_pi) != a_datum.end (), "found added p.i.");
      bool_test (l_orig_size + 1 == l_temp.size (), "additional child object increase size by 1");
      // remove
      l_temp.remove (l_temp.find (&l_pi));
      bool_test (a_datum.find (&l_pi) == a_datum.end (), "did not find removed p.i.");
    }
    {
      // add_text
      const static std::string l_s4 ("some text here");
      text & l_text (l_temp.add_text (l_s4));
      // find (const)
      bool_test (a_datum.find (&l_text) != a_datum.end (), "found added text");
      bool_test (l_orig_size + 1 == l_temp.size (), "additional child object increase size by 1");
      // remove
      l_temp.remove (l_temp.find (&l_text));
      bool_test (a_datum.find (&l_text) == a_datum.end (), "did not find removed text");
    }
    {
      // set_attribute
      const static std::string l_s5 ("N1fqpLsv9");
      const static std::string l_s6 ("some text here");
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, l_s5, l_s6), "can not set attribute with bad name.");
    }
  }
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  
  // Visit all children
  if (not a_datum.empty ())
  {
    for (basic_datum::const_iterator l_i (a_datum.begin ()); l_i != a_datum.end (); ++l_i)
    {
      (*l_i)->visit (*this);
    }
  }

}

/**
 * Operate on comment objects.
 */
void test_object_visitor::visit_comment(const comment & a_datum) 
{
  log () << COMM << "entering on_visit (comment)\n";
  // Increment counter
  ++m_type[s_comment];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::comment_type (), "type name matches");
  // Do basic_text tests
  test_basic_text (a_datum);
  // Count basic_datum objects
  ++m_type[0];

}

/**
 * Operate on compound objects.
 */
void test_object_visitor::visit_compound(const compound & a_datum) 
{
  log () << COMM << "entering on_visit (compound) [" << a_datum.label () << "]\n";
  // Count compound objs.
  ++m_type[s_compound];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::compound_name (), "type name matches");
  // Check attributes
  {
    std::string l_value;
    bool_test (a_datum.get_attribute ("label", l_value) and a_datum.label () == l_value
               , "get_attribute('label') matches label()");
  }
  {
    std::string l_value;
    bool_test (not a_datum.get_attribute ("category", l_value) or a_datum.categories () == l_value
               , "no categories or get_attribute('category') matches categories()");
  }
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_form_size (a_datum.size_formula ());
  const unsigned int l_iden_size (a_datum.size_identity ());
  const bool l_have_info (a_datum.has_information ());
  // Test add_formula
  {
    compound & l_temp (const_cast< compound & >(a_datum));
    const static std::string l_str ("N6fy6syV");
    std::string l_str1 (l_str);
    basic_datum & l_form (l_temp.add_child (data_access::names::formula_name (), l_str1));
    // find (const)
    bool_test (a_datum.find (&l_form) != a_datum.end (), "found added formula");
    bool_test (&l_form == &(*(a_datum.find_formula (l_str))), "find by label found added formula");
    bool_test (NULL != dynamic_cast< formula * >(&l_form), "add child is the right derived type");
    // remove
    l_temp.remove (l_temp.find (&l_form));
    bool_test (a_datum.find (&l_form) == a_datum.end (), "did not find removed formula");
    // add child using specific method
    formula & l_form2 (l_temp.add_formula (l_str));
    // find (const)
    bool_test (a_datum.find (&l_form2) != a_datum.end (), "found added formula");
    bool_test (&l_form2 == &(*(a_datum.find_formula (l_str))), "find by label found added formula");
    bool_test (&(l_form2.parent ()) == &a_datum, "am parent of added object.");
    // remove
    l_temp.remove_child (l_form2);
    bool_test (a_datum.find (&l_form2) == a_datum.end (), "did not find removed formula");
  }
  
  // Test add_identity
  {
    compound & l_temp (const_cast< compound & >(a_datum));
    const static std::string l_str ("IisP9em1");
    std::string l_str1 (l_str);
    basic_datum & l_ident (l_temp.add_child (data_access::names::identity_name (), l_str1));
    // find (const)
    bool_test (a_datum.find (&l_ident) != a_datum.end (), "found added identity");
    bool_test (NULL != dynamic_cast< identity * >(&l_ident), "add child is the right derived type");
    // remove
    l_temp.remove (l_temp.find (&l_ident));
    bool_test (a_datum.find (&l_ident) == a_datum.end (), "did not find removed identity");
    // add child using specific method
    identity & l_ident2 (l_temp.add_identity (l_str));
    // find (const)
    bool_test (a_datum.find (&l_ident2) != a_datum.end (), "found added identity");
    bool_test (&(l_ident2.parent ()) == &a_datum, "am parent of added object.");
    // remove
    l_temp.remove_child (l_ident2);
    bool_test (a_datum.find (&l_ident2) == a_datum.end (), "did not find removed identity");
  }
  // Test add_info
  {
    const static std::string l_str ("x5Kvop3X");
    if (a_datum.has_information ())
    {
      bool_test (&(a_datum.information ().parent ()) == &a_datum, "am parent of info object.");
      // You cannot add second info
      compound & l_temp (const_cast< compound & >(a_datum));
      monitor_function (utility::contract_error::require, boost::bind (&compound::add_info, &l_temp, l_str), "can not add second info to a compound.");
    }
    else
    {
      compound & l_temp (const_cast< compound & >(a_datum));
      basic_datum & l_info (l_temp.add_info (l_str));
      bool_test (a_datum.has_information (), "has info object after adding.");
      // find (const)
      bool_test (&l_info == &(a_datum.information ()), "found added info");
      bool_test (&(a_datum.information ().parent ()) == &a_datum, "am parent of added object.");
      // remove
      l_temp.remove (l_temp.find (&l_info));
      bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info");
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_form_size == a_datum.size_formula (), "have original number of formula objects");
  bool_test (l_iden_size == a_datum.size_identity (), "have original number of identity objects");
  bool_test (l_have_info == a_datum.has_information (), "have original number of info objects");
  bool_test (l_orig_size >= (a_datum.size_formula () + a_datum.size_identity () + (a_datum.has_information ()?1:0))
             , "number of children >= to number of special children.");
  // Compound is a child of its parent.
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "compound is a child of its parent.");
  // Change category
  {
    const static std::string l_str ("NMhb7tl7y");
    const std::string l_orig (a_datum.categories ());
    compound & l_temp (const_cast< compound & >(a_datum));
    l_temp.add_category_label (l_str);
    if (l_orig.empty ())
    {
      bool_test (l_str == a_datum.categories (), "category value matches set value");
    }
    else
    {
      bool_test (data_access::names::list (l_orig, l_str) == a_datum.categories (), "category value matches set value");
    }
    l_temp.set_attribute (data_access::names::category_name (), l_orig);
    bool_test (l_orig == a_datum.categories (), "category value matches reset value");
  }
  // Attempt to set label.
  {
    const static std::string l_str1 ("a5FowHsd8");
    const static std::string l_str2 ("zDf62ioeS");
    const std::string & l_str (l_str1 != a_datum.label () ? l_str1 : l_str2);
    compound & l_temp (const_cast< compound & >(a_datum));
    monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_str), "can not set label attribute");
  }
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on compound ref objects.
 */
void test_object_visitor::visit_compound_ref(const compound_ref & a_datum) 
{
  log () << COMM << "entering on_visit (compound_ref) [" << a_datum.compound_label () << "]\n";
  // Count compound objs.
  ++m_type[s_compound_ref];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::compound_ref_name (), "type name matches");
  // Check attributes
  {
    std::string l_value;
    bool_test (a_datum.get_attribute ("label", l_value) and a_datum.compound_label () == l_value
               , "get_attribute('label') matches compound_label().");
  }
  {
    std::string l_value;
    bool_test (a_datum.get_attribute ("formula", l_value) and a_datum.formulae () == l_value
               , "get_attribute('formula') matches formulae().");
  }
  /////////////////////
  // Modifying methods
  {
    compound_ref & l_temp (const_cast< compound_ref & >(a_datum));
    const std::string l_orig (a_datum.formulae ());
    l_temp.add_formula_label ("qa1QH2gi");
    bool_test (a_datum.formulae () == data_access::names::list (l_orig, "qa1QH2gi")
               , "successfully added formula label.");
    l_temp.set_attribute ("formula", l_orig);
    bool_test (a_datum.formulae () == l_orig, "successfully reset formulae.");
  }
  // Empty elements can not contain anything!
  bool_test (a_datum.empty (), "compound-ref objects always empty.");
  // Are we a child of our parent
  bool_test (a_datum.section ().find (&a_datum) != a_datum.section ().end (), "object is a child of its parent.");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on datum objects.
 */
void test_object_visitor::visit_datum(const datum & a_datum) 
{
  log () << COMM << "entering on_visit (datum) [" << a_datum.outcome () << "]\n";
  // Count compound objs.
  ++m_type[s_datum];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::datum_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_data_size (a_datum.size_datum ());
  // Test add_formula
  {
    datum & l_temp (const_cast< datum & > (a_datum));
    {
      const static std::string l_str ("N6fy6syV");
      std::string l_str1 (l_str);
      datum & l_datum2 (l_temp.add_datum (l_str));
      // find (const)
      bool_test (a_datum.find (&l_datum2) != a_datum.end (), "found added datum");
      // remove
      l_temp.remove_child (l_datum2);
      bool_test (a_datum.find (&l_datum2) == a_datum.end (), "did not find removed datum");
      basic_datum & l_datum (l_temp.add_child (data_access::names::datum_name (), l_str1));
      // find (const)
      bool_test (a_datum.find (&l_datum) != a_datum.end (), "found added datum 2");
      // remove
      l_temp.remove (l_temp.find (&l_datum));
      bool_test (a_datum.find (&l_datum) == a_datum.end (), "did not find removed datum 2");
    }
    // Check attributes
    const static std::string l_new_value ("yjH1yE5xl");
    {
      std::string l_value;
      const std::string l_orig (a_datum.outcome ());
      bool_test (a_datum.get_attribute (data_access::names::outcome_name (), l_value) and l_orig == l_value
                 , "get_attribute('outcome') matches outcome()");
      l_temp.outcome (l_new_value);
      bool_test (l_new_value == a_datum.outcome (), "outcome () matches set value");
      l_temp.set_attribute (data_access::names::outcome_name (), l_orig);
      bool_test (l_orig == a_datum.outcome (), "outcome () matches reset value");
    }
    {
      std::string l_value;
      const std::string l_orig (a_datum.dim ());
      bool_test (not a_datum.get_attribute (data_access::names::dimension_name (), l_value) or l_orig == l_value
                 , "no dimension or get_attribute('dim') matches dim()");
      l_temp.dim (l_new_value);
      bool_test (l_new_value == a_datum.dim (), "dim () matches set value");
      l_temp.set_attribute (data_access::names::dimension_name (), l_orig);
      bool_test (l_orig == a_datum.dim (), "dim () matches reset value");
    }
    {
      std::string l_value;
      const std::string l_orig (a_datum.format ());
      bool_test (not a_datum.get_attribute (data_access::names::format_name (), l_value) or l_orig == l_value
                 , "no format or get_attribute('format') matches format()");
      l_temp.format (l_new_value);
      bool_test (l_new_value == a_datum.format (), "format () matches set value");
      l_temp.set_attribute (data_access::names::format_name (), l_orig);
      bool_test (l_orig == a_datum.format (), "format () matches reset value");
    }
    {
      std::string l_value;
      const std::string l_orig (a_datum.scale ());
      bool_test (not a_datum.get_attribute (data_access::names::scale_name (), l_value) or a_datum.scale () == l_value
                 , "no scale or get_attribute('scale') matches scale()");
      l_temp.scale (l_new_value);
      bool_test (l_new_value == a_datum.scale (), "scale () matches set value");
      l_temp.set_attribute (data_access::names::scale_name (), l_orig);
      bool_test (l_orig == a_datum.scale (), "scale () matches reset value");
    }
    if (a_datum.has_ordinal ())
    {
      std::string l_value;
      bool l_has_ordinal (a_datum.has_ordinal ());
      const int l_new_ord (123);
      int l_orig (0);
      if (l_has_ordinal)
      {
        l_orig = a_datum.ordinal ();
      }
      l_temp.ordinal (l_new_ord);
      bool_test (l_new_ord == a_datum.ordinal (), "ordinal () matches set value");
      bool_test (a_datum.get_attribute (data_access::names::ordinal_name (), l_value) and l_value == std::string ("123")
                 , "get_attribute('ordinal') matches set value");
      if (l_has_ordinal)
      {
        l_temp.ordinal (l_orig);
        bool_test (l_orig == a_datum.ordinal (), "ordinal () matches reset value");
      }
      else
      {
        l_temp.unset_ordinal ();
      }
      bool_test (l_has_ordinal == a_datum.has_ordinal (), "status of ordinal reset");
    }
  }
  
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_data_size == a_datum.size_datum (), "have original number of datum objects");
  bool_test (l_orig_size >= a_datum.size_datum (), "number of children >= to number of special children.");
  // Are we a child of our parent
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is a child of its parent.");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on experiment objects.
 */
void test_object_visitor::visit_experiment(const experiment & a_datum) 
{
  log () << COMM << "entering on_visit (experiment) [" << a_datum.reference () << "]\n";
  // Count experiment objs.
  ++m_type[s_experiment];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::experiment_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_info_size (a_datum.size_info ());
  const unsigned int l_memo_size (a_datum.size_memo ());
  {
    experiment & l_temp (const_cast< experiment & >(a_datum));
    // Test add_memo
    {
      const static std::string l_name ("7y-3jUXip");
      std::string l_name1 (l_name);
      memo & l_memo2 (l_temp.add_memo (l_name));
      // find (const)
      bool_test (a_datum.find (&l_memo2) != a_datum.end (), "found added memo");
      bool_test (&l_memo2 == &(*(a_datum.find_memo (l_name))), "find by label found added memo");
      // remove
      l_temp.remove_child (l_memo2);
      bool_test (a_datum.find (&l_memo2) == a_datum.end (), "did not find removed memo");
      basic_datum & l_memo (l_temp.add_child (data_access::names::memo_name (), l_name1));
      // find (const)
      bool_test (a_datum.find (&l_memo) != a_datum.end (), "found added memo 2");
      bool_test (&l_memo == &(*(a_datum.find_memo (l_name))), "find by label found added memo 2");
      // remove
      l_temp.remove (l_temp.find (&l_memo));
      bool_test (a_datum.find (&l_memo) == a_datum.end (), "did not find removed memo 2");
    }
  
    // Test add_info
    {
      const static std::string l_name ("WTgl3qk3");
      std::string l_name1 (l_name);
      info & l_info2 (l_temp.add_info (l_name));
      // find (const)
      bool_test (a_datum.find (&l_info2) != a_datum.end (), "found added info");
      // remove
      l_temp.remove_child (l_info2);
      bool_test (a_datum.find (&l_info2) == a_datum.end (), "did not find removed info");
      basic_datum & l_info (l_temp.add_child (data_access::names::info_name (), l_name1));
      // find (const)
      bool_test (a_datum.find (&l_info) != a_datum.end (), "found added info 2");
      // remove
      l_temp.remove (l_temp.find (&l_info));
      bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info 2");
    }
    // Check attributes
    const static std::string l_new_value ("kdv81ulSA");
    {
      std::string l_value;
      const std::string l_orig (a_datum.reference ());
      const static std::string l_nv2 ("5MtvvhJ7o");
      const std::string & l_str (l_new_value != l_orig ? l_new_value : l_nv2);
      bool_test (a_datum.get_attribute (data_access::names::reference_name (), l_value) and l_orig == l_value
                 , "get_attribute('reference') matches reference()");
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::reference_name (), l_str), "can not set reference attribute of experiment");
    }
    {
      std::string l_value;
      const std::string l_orig (a_datum.worker_list ());
      bool_test (a_datum.get_attribute (data_access::names::worker_name (), l_value) and l_orig == l_value
                 , "get_attribute('worker') matches worker()");
      l_temp.add_worker_label (l_new_value);
      bool_test (a_datum.worker_list () == data_access::names::list (l_orig, l_new_value), "worker list equals set value");
      l_temp.set_attribute (data_access::names::worker_name (), l_orig);
      bool_test (a_datum.worker_list () == l_orig, "worker list equals reset value");
    }
    {
      std::string l_value;
      const bool l_has_pri (a_datum.has_priority ());
      const unsigned int l_new_pri (123);
      unsigned int l_orig;
      if (l_has_pri)
      {
        l_orig = a_datum.priority ();
        bool_test (not a_datum.get_attribute (data_access::names::priority_name (), l_value) or l_orig == boost::lexical_cast< unsigned int > (l_value)
                   , "no priority or get_attribute('priority') matches priority()");
      }
      l_temp.priority (l_new_pri);
      bool_test (a_datum.priority () == l_new_pri, "priority value changed to set value.");
      if (l_has_pri)
      {
        l_temp.set_attribute (data_access::names::priority_name (), l_value);
        bool_test (a_datum.priority () == l_orig, "priority value reset.");
      }
      else
      {
        l_temp.unset_priority ();
      }
      bool_test (l_has_pri == a_datum.has_priority (), "reset priority status");
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_info_size == a_datum.size_info (), "have original number of info objects");
  bool_test (l_memo_size == a_datum.size_memo (), "have original number of memo objects");
  bool_test (l_orig_size >= (a_datum.size_info () + a_datum.size_memo ())
             , "number of children >= to number of special children.");
  // Are we a child of our parent
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is a child of its parent.");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on filter objects.
 */
void test_object_visitor::visit_filter(const filter & a_datum) 
{
  log () << COMM << "entering on_visit (filter) [" << a_datum.label () << "]\n";
  // Count filter objs.
  ++m_type[s_filter];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::filter_name (), "type name matches");
  {
    filter & l_temp (const_cast < filter & >(a_datum));
    const static std::string l_new_value ("udxMS5up0");
    // Check attributes
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value
                , "get_attribute('label') matches label()");
      const static std::string l_nv2 ("r5uoccVG6");
      const std::string & l_nv_ref (l_new_value != l_value ? l_new_value : l_nv2);
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_nv_ref)
                        , "can not change label value");
    }
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::href_name (), l_value) and a_datum.href () == l_value
                , "get_attribute('href') matches href()");
      l_temp.href (l_new_value);
      bool_test (l_new_value == l_temp.href (), "set href value");
      l_temp.set_attribute (data_access::names::href_name (), l_value);
      bool_test (l_value == l_temp.href (), "reset href value");
    }
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::outcome_name (), l_value) and a_datum.outcomes () == l_value
                , "get_attribute('outcome') matches outcomes()");
      l_temp.add_outcome_label (l_new_value);
      bool_test (data_access::names::list(l_value, l_new_value) == l_temp.outcomes (), "added outcome value");
      l_temp.set_attribute (data_access::names::outcome_name (), l_value);
      bool_test (l_value == l_temp.outcomes (), "reset outcome value");
    }
    {
      std::string l_value;
      const bool l_is_default (a_datum.is_default_type ());
      if (not l_is_default)
      {
        bool_test (a_datum.get_attribute (data_access::names::type_name (), l_value) and a_datum.filter_type () == l_value
                  , "get_attribute('type') matches filter_type()");
        bool_test (l_value == data_access::names::plugin_filter_name () or l_value == data_access::names::regex_filter_name ()
                   , "filter_type is one of allowed names.");
      }
      l_temp.filter_type (l_new_value);
      bool_test (l_new_value == l_temp.filter_type (), "set plugin value");
      if (not l_is_default)
      {
        l_temp.set_attribute (data_access::names::type_name (), l_value);
        bool_test (l_value == l_temp.filter_type (), "reset plugin value");
      }
      else
      {
        l_temp.reset_type ();
      }
      bool_test (l_is_default == a_datum.is_default_type (), "status of filter type reset");
    }
  }
  // test parent
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on formula objects.
 */
void test_object_visitor::visit_formula(const formula & a_datum) 
{
  log () << COMM << "entering on_visit (formula) [" << a_datum.label () << "]\n";
  // Capture the first formula!
  if (NULL == m_formula)
  {
    m_formula = &a_datum;
  }
  // Count formula objs.
  ++m_type[s_formula];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::formula_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_iden_size (a_datum.size_identity ());
  const bool l_has_info (a_datum.has_information ());
  {
    formula & l_temp (const_cast< formula & >(a_datum));
    // Check attributes
    {
      const static std::string l_s1 ("Ed44cRgix");
      const static std::string l_s2 ("c3TvchpG1");
      const static std::string & l_str (l_s1 != a_datum.label () ? l_s1 : l_s2);
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value
                 , "get_attribute('label') matches label()");
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_str)
                        , "can not change label attribute");
    }
    // Test add_identity
    {
      const static std::string l_label ("fogRdvS73");
      std::string l_label2 (l_label);
      identity & l_ident (l_temp.add_identity (l_label));
      // find (const)
      bool_test (a_datum.find (&l_ident) != a_datum.end (), "found added identity");
      // remove
      l_temp.remove_child (l_ident);
      bool_test (a_datum.find (&l_ident) == a_datum.end (), "did not find removed identity");
      basic_datum & l_ident2 (l_temp.add_child (data_access::names::identity_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_ident2) != a_datum.end (), "found added identity 2");
      // remove
      l_temp.remove (l_temp.find (&l_ident2));
      bool_test (a_datum.find (&l_ident2) == a_datum.end (), "did not find removed identity 2");
    }
    {
      const static std::string l_label ("0Xki6Xgj");
      // Test add_info
      if (l_has_info)
      {
        // You cannot add second info
        monitor_function (utility::contract_error::require, boost::bind (&formula::add_info, const_cast< formula* > (&a_datum), l_label), "can not add second info to a compound.");
      }
      else
      {
        formula & l_temp (const_cast< formula & >(a_datum));
        info & l_info (l_temp.add_info (l_label));
        bool_test (a_datum.has_information (), "has info object after adding.");
        // find (const)
        bool_test (&l_info == &(a_datum.information ()), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_info));
        bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info");
        std::string l_label2 (l_label);
        basic_datum & l_info2 (l_temp.add_child (data_access::names::info_name (), l_label2));
        bool_test (a_datum.has_information (), "has info object 2 after adding.");
        // find (const)
        bool_test (&l_info2 == &(a_datum.information ()), "found added info 2");
        // remove
        l_temp.remove (l_temp.find (&l_info2));
        bool_test (a_datum.find (&l_info2) == a_datum.end (), "did not find removed info 2");
      }
      bool_test (l_has_info == a_datum.has_information (), "info status unchanged.");
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_iden_size == a_datum.size_identity (), "have original number of identity objects");
  bool_test (l_has_info == a_datum.has_information (), "have original number of info objects");
  bool_test (l_orig_size >= (a_datum.size_identity () + (a_datum.has_information ()?1:0))
             , "number of children >= to number of special children.");
  // parent
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on identity objects.
 */
void test_object_visitor::visit_identity(const identity & a_datum) 
{
  log () << COMM << "entering on_visit (identity) [" << a_datum.identity_type () << "]\n";
  // Count identity objs.
  ++m_type[s_identity];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::identity_name (), "type name matches");
  // Check attributes
  {
    identity & l_temp (const_cast< identity & >(a_datum));
    std::string l_value;
    const static std::string l_name ("o5gjtc6XA");
    bool_test (a_datum.get_attribute (data_access::names::type_name (), l_value) and a_datum.identity_type () == l_value
               , "get_attribute('type') matches identity_type()");
    monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::type_name (), l_name)
                     , "can not change type attribute");
  }
  // check parent/child
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on info objects.
 */
void test_object_visitor::visit_info(const info & a_datum) 
{
  log () << COMM << "entering on_visit (info) [" << a_datum.name () << "]\n";
  // Count info objs.
  ++m_type[s_info];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::info_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_info_size (a_datum.size_info ());
  {
    info & l_temp (const_cast< info & >(a_datum));
    // Test add_info
    {
      const static std::string l_label ("W9aTlm2u");
      info & l_form (l_temp.add_info (l_label));
      // find (const)
      bool_test (a_datum.find (&l_form) != a_datum.end (), "found added info");
      // remove
      l_temp.remove_child (l_form);
      bool_test (a_datum.find (&l_form) == a_datum.end (), "did not find removed info");
      std::string l_label2 (l_label);
      basic_datum & l_form2 (l_temp.add_child (data_access::names::info_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_form2) != a_datum.end (), "found added info 2");
      // remove
      l_temp.remove (l_temp.find (&l_form2));
      bool_test (a_datum.find (&l_form2) == a_datum.end (), "did not find removed info 2");
    }
    // Check attributes
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::name_name (), l_value) and a_datum.name () == l_value
                 , "get_attribute('name') matches name()");
      const static std::string l_label ("C3TvchpG1");
      l_temp.name (l_label);
      bool_test (a_datum.name () == l_label, "name changed to set value");
      l_temp.set_attribute (data_access::names::name_name (), l_value);
      bool_test (a_datum.name () == l_value, "name value reset");
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_info_size == a_datum.size_info (), "have original number of info objects");
  bool_test (l_orig_size >= a_datum.size_info (), "number of children >= to number of special children.");
  // check parent/child
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on mechanism objects.
 */
void test_object_visitor::visit_mechanism(const mechanism & a_datum) 
{
  log () << COMM << "entering on_visit (mechanism) [" << a_datum.label () << "]\n";
  // Count mechanism objs.
  ++m_type[s_mechanism];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::mechanism_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_filt_size (a_datum.size_filter ());
  const unsigned int l_sett_size (a_datum.size_settings ());
  const bool l_has_info (a_datum.has_information ());
  {
    mechanism & l_temp (const_cast< mechanism & >(a_datum));
    // Test add_filter
    {
      const static std::string l_label ("xvk92kT");
      filter & l_form (l_temp.add_filter (l_label));
      // find (const)
      bool_test (a_datum.find (&l_form) != a_datum.end (), "found added filter");
      bool_test (&l_form == &(*(a_datum.find_filter (l_label))), "find by label found added filter");
      // remove
      l_temp.remove_child (l_form);
      bool_test (a_datum.find (&l_form) == a_datum.end (), "did not find removed filter");
      std::string l_label2 (l_label);
      basic_datum & l_form2 (l_temp.add_child (data_access::names::filter_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_form2) != a_datum.end (), "found added filter 2");
      bool_test (&l_form2 == &(*(a_datum.find_filter (l_label))), "find by label found added filter 2");
      // remove
      l_temp.remove (l_temp.find (&l_form2));
      bool_test (a_datum.find (&l_form2) == a_datum.end (), "did not find removed filter 2");
    }
    // Test add_setting
    {
      const static std::string l_label ("4bc7msUL");
      settings & l_ident (l_temp.add_setting (l_label));
      // find (const)
      bool_test (a_datum.find (&l_ident) != a_datum.end (), "found added setting");
      // remove
      l_temp.remove_child (l_ident);
      bool_test (a_datum.find (&l_ident) == a_datum.end (), "did not find removed setting");
      std::string l_label2 (l_label);
      basic_datum & l_ident2 (l_temp.add_child (data_access::names::settings_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_ident2) != a_datum.end (), "found added setting");
      // remove
      l_temp.remove (l_temp.find (&l_ident2));
      bool_test (a_datum.find (&l_ident2) == a_datum.end (), "did not find removed setting");
    }
    // Test add_info
    {
      const static std::string l_str ("mk4zGDr");
      const bool l_has_info (a_datum.has_information ());
      if (l_has_info)
      {
        // You cannot add second info
        monitor_function (utility::contract_error::require, boost::bind (&mechanism::add_info, &l_temp, l_str), "can not add second info to a mechanism.");
      }
      else
      {
        info & l_info (l_temp.add_info (l_str));
        bool_test (a_datum.has_information (), "has info object after adding.");
        // find (const)
        bool_test (&l_info == &(a_datum.information ()), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_info));
        bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info");
        std::string l_str1 (l_str);
        basic_datum & l_info2 (l_temp.add_child (data_access::names::info_name (), l_str1));
        bool_test (a_datum.has_information (), "has info object after adding.");
        // find (const)
        bool_test (&l_info2 == &(a_datum.information ()), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_info2));
        bool_test (a_datum.find (&l_info2) == a_datum.end (), "did not find removed info");
      }
      bool_test (l_has_info == a_datum.has_information (), "reset information status to original");
    }
    // Check attributes
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value
                 , "get_attribute('label') matches label()");
      const static std::string l_str1 ("wuf4LGou7");
      const static std::string l_str2 ("raaJBl63q");
      const std::string & l_str (l_str1 != a_datum.label () ? l_str1 : l_str2);
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_str)
                        , "can not change label attribute of mechanism object");
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_filt_size == a_datum.size_filter (), "have original number of filter objects");
  bool_test (l_sett_size == a_datum.size_settings (), "have original number of settings objects");
  bool_test (l_has_info == a_datum.has_information (), "have original number of information objects");
  bool_test (l_orig_size >= (a_datum.size_filter () + a_datum.size_settings () + (a_datum.has_information ()?1:0))
             , "number of children >= to number of special children.");
  // check parent/child
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on memo objects.
 */
void test_object_visitor::visit_memo(const memo & a_datum) 
{
  log () << COMM << "entering on_visit (memo)[" << a_datum.label () << "]\n";
  // Count memo objs.
  ++m_type[s_memo];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::memo_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_data_size (a_datum.size_datum ());
  const unsigned int l_info_size (a_datum.size_info ());
  {
    memo & l_temp (const_cast< memo & >(a_datum));
    // Check attributes
    const static std::string l_new_value ("b6Ag8fvZe");
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value
                , "get_attribute('label') matches label()");
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_new_value)
                       , "can not change label of memo object.");
    }
    {
      std::string l_form_value;
      bool_test (a_datum.get_attribute (data_access::names::formula_name (), l_form_value) and a_datum.formula_label () == l_form_value
                , "get_attribute('formula') matches formula_label()");
      std::string l_cmpd_value;
      bool_test (a_datum.get_attribute (data_access::names::compound_name (), l_cmpd_value) and a_datum.compound_label () == l_cmpd_value
                , "get_attribute('compound') matches compound_label()");
      // Need to find original compound/formula to reset
      program::const_compound_iterator l_cmpd_iter (l_temp.page ().section ().site ().find_compound (l_cmpd_value));
      bool_test (l_cmpd_iter != l_temp.page ().section ().site ().end_compound (), "found compound for memo object");
      compound::const_formula_iterator l_form_iter (l_cmpd_iter->find_formula (l_form_value));
      bool_test (l_form_iter != l_cmpd_iter->end_formula (), "found formula for memo object");
      if (NULL != m_formula)
      {
        const std::string l_form_label (m_formula->label ());
        const std::string l_cmpd_label (m_formula->moiety ().label ());
        l_temp.compound_formula (*m_formula);
        bool_test (l_form_label == a_datum.formula_label (), "set formula_label value matches");
        bool_test (l_cmpd_label == a_datum.compound_label (), "set compound_label value matches");
        l_temp.compound_formula (*l_form_iter);
        bool_test (l_form_value == a_datum.formula_label (), "reset formula_label value matches");
        bool_test (l_cmpd_value == a_datum.compound_label (), "reset compound_label value matches");
      }
    }
    // tests only if memo or opened memo-src
    if (not a_datum.is_src () or a_datum.is_open ())
    {
      {
        std::string l_value;
        bool_test (a_datum.get_attribute (data_access::names::mechanism_name (), l_value) and a_datum.target_mechanism () == l_value
                  , "get_attribute('mechanism') matches target_mechanism()");
        l_temp.target_mechanism (l_new_value);
        bool_test (l_new_value == a_datum.target_mechanism (), "set target_mechanism value matches");
        l_temp.set_attribute (data_access::names::mechanism_name (), l_value);
        bool_test (l_value == a_datum.target_mechanism (), "reset target_mechanism value matches");
      }
      {
        std::string l_value;
        bool_test (not a_datum.get_attribute (data_access::names::filter_name (), l_value) or a_datum.filters () == l_value
                  , "no categories or get_attribute('filter') matches filters()");
        l_temp.add_filter_label (l_new_value);
        bool_test (data_access::names::list (l_value, l_new_value) == a_datum.filters (), "added filter label value matches");
        l_temp.set_attribute (data_access::names::filter_name (), l_value);
        bool_test (l_value == a_datum.filters (), "reset filters value matches");
      }
      // Test add_datum
      {
        const static std::string l_name ("vNUv32qt");
        datum & l_form (l_temp.add_datum (l_name));
        // find (const)
        bool_test (a_datum.find (&l_form) != a_datum.end (), "found added datum");
        bool_test (&l_form == &(*(a_datum.find_datum (l_name))), "find by label found added datum");
        // remove
        l_temp.remove_child (l_form);
        bool_test (a_datum.find (&l_form) == a_datum.end (), "did not find removed datum");
        std::string l_name2 (l_name);
        basic_datum & l_form2 (l_temp.add_child (data_access::names::datum_name (), l_name2));
        // find (const)
        bool_test (a_datum.find (&l_form2) != a_datum.end (), "found added datum 2");
        bool_test (&l_form2 == &(*(a_datum.find_datum (l_name))), "find by label found added datum 2");
        // remove
        l_temp.remove (l_temp.find (&l_form2));
        bool_test (a_datum.find (&l_form2) == a_datum.end (), "did not find removed datum 2");
      }
      // Test add_info
      {
        const static std::string l_name ("wNmj17aB");
        info & l_ident (l_temp.add_info (l_name));
        // find (const)
        bool_test (a_datum.find (&l_ident) != a_datum.end (), "found added info");
        // remove
        l_temp.remove_child (l_ident);
        bool_test (a_datum.find (&l_ident) == a_datum.end (), "did not find removed info");
        std::string l_name2 (l_name);
        basic_datum & l_ident2 (l_temp.add_child (data_access::names::info_name (), l_name2));
        // find (const)
        bool_test (a_datum.find (&l_ident2) != a_datum.end (), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_ident2));
        bool_test (a_datum.find (&l_ident2) == a_datum.end (), "did not find removed info");
      }
      {
        std::string l_value;
        bool_test (not a_datum.get_attribute (data_access::names::href_name (), l_value) or a_datum.href () == l_value
                  , "no href or get_attribute('href') matches href()");
        l_temp.href (l_new_value);
        bool_test (l_new_value == a_datum.href (), "set href label value matches");
        l_temp.set_attribute (data_access::names::href_name (), l_value);
        bool_test (l_value == a_datum.href (), "reset href value matches");
      }
    }
    else
    {
      const static std::string l_name ("wNmj17aB");
      std::string l_name2 (l_name);
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::add_child, &l_temp, data_access::names::datum_name (), l_name2)
                        , "can not add datum child to closed memo-src object.");
      monitor_function (utility::contract_error::require, boost::bind (&memo::add_datum, &l_temp, l_name)
                        , "can not add datum child to closed memo-src object.");
      std::string l_name3 (l_name);
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::add_child, &l_temp, data_access::names::info_name (), l_name3)
                        , "can not add info child to  memo-src object.");
      monitor_function (utility::contract_error::require, boost::bind (&memo::add_info, &l_temp, l_name)
                        , "can not add info child to closed memo-src object.");
      {
        std::string l_value;
        bool_test (not a_datum.get_attribute (data_access::names::href_name (), l_value) or a_datum.href () == l_value
                  , "no href or get_attribute('href') matches href()");
      }
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_data_size == a_datum.size_datum (), "have original number of datum objects");
  bool_test (l_info_size == a_datum.size_info (), "have original number of info objects");
  bool_test (l_orig_size >= (a_datum.size_datum () + a_datum.size_info ())
             , "number of children >= to number of special children.");
  // check parent/child
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on processing_instruction objects.
 */
void test_object_visitor::visit_pi(const processing_instruction & a_datum) 
{
  log () << COMM << "entering on_visit (pi)\n";
  // Increment counter
  ++m_type[s_pi];
  // Test name attribute
  bool_test (not a_datum.name.empty (), "P.I. may not have empty name.");
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::processing_instruction_type (), "type name matches");
  // Do basic_text tests
  test_basic_text (a_datum);
  // Count basic_datum objects
  ++m_type[0];

}

/**
 * Operate on program objects.
 */
void test_object_visitor::visit_program(const program & a_datum) 
{
  log () << COMM << "entering on_visit (program)\n";
  // Count program objs.
  ++m_type[s_program];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::program_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_work_size (a_datum.size_worker ());
  const unsigned int l_mech_size (a_datum.size_mechanism ());
  const unsigned int l_cmpd_size (a_datum.size_compound ());
  const unsigned int l_proj_size (a_datum.size_project ());
  const bool l_has_info (a_datum.has_information ());
  {
    program & l_temp (const_cast< program & >(a_datum));
    // Test add_worker
    {
      const static std::string l_label ("bfrlFO99");
      worker & l_element (l_temp.add_worker (l_label));
      // find (const)
      bool_test (a_datum.find (&l_element) != a_datum.end (), "found added worker");
      bool_test (&l_element == &(*(a_datum.find_worker (l_label))), "find by label found added worker");
      // remove
      l_temp.remove_child (l_element);
      bool_test (a_datum.find (&l_element) == a_datum.end (), "did not find removed worker");
      std::string l_label2 (l_label);
      basic_datum & l_element2 (l_temp.add_child (data_access::names::worker_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_element2) != a_datum.end (), "found added worker 2");
      bool_test (&l_element2 == &(*(a_datum.find_worker ("bfrlFO99"))), "find by label found added worker 2");
      // remove
      l_temp.remove (l_temp.find (&l_element2));
      bool_test (a_datum.find (&l_element2) == a_datum.end (), "did not find removed worker 2");
    }
    // Test add_mechanism
    {
      const static std::string l_label ("plr6PX5lm");
      mechanism & l_element (l_temp.add_mechanism (l_label));
      // find (const)
      bool_test (a_datum.find (&l_element) != a_datum.end (), "found added mechanism");
      bool_test (&l_element == &(*(a_datum.find_mechanism (l_label))), "find by label found added mechanism");
      // remove
      l_temp.remove_child (l_element);
      bool_test (a_datum.find (&l_element) == a_datum.end (), "did not find removed mechanism");
  
      std::string l_label2 (l_label);
      basic_datum & l_element2 (l_temp.add_child (data_access::names::mechanism_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_element2) != a_datum.end (), "found added mechanism 2");
      bool_test (&l_element2 == &(*(a_datum.find_mechanism (l_label))), "find by label found added mechanism 2");
      // remove
      l_temp.remove (l_temp.find (&l_element2));
      bool_test (a_datum.find (&l_element2) == a_datum.end (), "did not find removed mechanism 2");
    }
    // Test add_compound
    {
      const static std::string l_label ("56enfbMA");
      compound & l_element (l_temp.add_compound (l_label));
      // find (const)
      bool_test (a_datum.find (&l_element) != a_datum.end (), "found added compound");
      bool_test (&l_element == &(*(a_datum.find_compound (l_label))), "find by label found added compound");
      // remove
      l_temp.remove_child (l_element);
      bool_test (a_datum.find (&l_element) == a_datum.end (), "did not find removed compound");
  
      std::string l_label2 (l_label);
      basic_datum & l_element2 (l_temp.add_child (data_access::names::compound_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_element2) != a_datum.end (), "found added compound 2");
      bool_test (&l_element2 == &(*(a_datum.find_compound (l_label))), "find by label found added compound 2");
      // remove
      l_temp.remove (l_temp.find (&l_element2));
      bool_test (a_datum.find (&l_element2) == a_datum.end (), "did not find removed compound 2");
    }
    // Test add_project
    {
      const static std::string l_label ("J8eM4rhw");
      project & l_element (l_temp.add_project (l_label));
      // find (const)
      bool_test (a_datum.find (&l_element) != a_datum.end (), "found added project");
      bool_test (&l_element == &(*(a_datum.find_project (l_label))), "find by label found added project");
      // remove
      l_temp.remove_child (l_element);
      bool_test (a_datum.find (&l_element) == a_datum.end (), "did not find removed project");
  
      std::string l_label2 (l_label);
      basic_datum & l_element2 (l_temp.add_child (data_access::names::project_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_element2) != a_datum.end (), "found added project 2");
      bool_test (&l_element2 == &(*(a_datum.find_project (l_label))), "find by label found added project 2");
      // remove
      l_temp.remove (l_temp.find (&l_element2));
      bool_test (a_datum.find (&l_element2) == a_datum.end (), "did not find removed project 2");
    }
    // Test add_project_src
    {
      const static std::string l_fname ("somefile.xml");
      const static std::string l_label ("df33iAiA");
      project & l_element (l_temp.add_project_src (l_fname, l_label));
      // find (const)
      bool_test (a_datum.find (&l_element) != a_datum.end (), "found added project_src");
      bool_test (&l_element == &(*(a_datum.find_project (l_label))), "find by label found added project_src");
      // remove
      l_temp.remove_child (l_element);
      bool_test (a_datum.find (&l_element) == a_datum.end (), "did not find removed project_src");
  
      std::string l_fname2 (l_fname);
      basic_datum & l_element2 (l_temp.add_child (data_access::names::project_src_name (), l_fname2));
      l_element2.set_attribute (data_access::names::label_name (), l_label);
      // find (const)
      bool_test (a_datum.find (&l_element2) != a_datum.end (), "found added project_src 2");
      bool_test (&l_element2 == &(*(a_datum.find_project (l_label))), "find by label found added project_src 2");
      // remove
      l_temp.remove (l_temp.find (&l_element2));
      bool_test (a_datum.find (&l_element2) == a_datum.end (), "did not find removed project_src 2");
    }
    // Test add_info
    {
      const static std::string l_label ("x5Kvop3X");
      if (a_datum.has_information ())
      {
        // You cannot add second info
        std::string l_str (l_label);
        monitor_function (utility::contract_error::require
                          , boost::bind (&program::add_info, &l_temp, l_str)
                          , "can not add second info to a program.");
        monitor_function (utility::contract_error::always
                          , boost::bind (&basic_datum::add_child, &l_temp, data_access::names::info_name (), l_str)
                          , "can not add second info to a program.");
      }
      else
      {
        basic_datum & l_info (l_temp.add_info (l_label));
        bool_test (a_datum.has_information (), "has info object after adding.");
        // find (const)
        bool_test (&l_info == &(a_datum.information ()), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_info));
        bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info");
  
        std::string l_label2 (l_label);
        basic_datum & l_element2 (l_temp.add_child (data_access::names::info_name (), l_label2));
        bool_test (a_datum.has_information (), "has info object 2 after adding.");
        // find (const)
        bool_test (&l_element2 == &(a_datum.information ()), "found added info 2");
        // remove
        l_temp.remove (l_temp.find (&l_element2));
        bool_test (a_datum.find (&l_element2) == a_datum.end (), "did not find removed info 2");
      }
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_work_size == a_datum.size_worker (), "have original number of worker objects");
  bool_test (l_mech_size == a_datum.size_mechanism (), "have original number of mechanism objects");
  bool_test (l_cmpd_size == a_datum.size_compound (), "have original number of compound objects");
  bool_test (l_proj_size == a_datum.size_project (), "have original number of project objects");
  bool_test (l_has_info == a_datum.has_information (), "have original status of info objects");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on project objects.
 */
void test_object_visitor::visit_project(const project & a_datum) 
{
  log () << COMM << "entering on_visit (project) [" << a_datum.label () << "]\n";
  // Count project objs.
  ++m_type[s_project];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::project_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_expt_size (a_datum.size_experiment ());
  const unsigned int l_cmpd_size (a_datum.size_compound_ref ());
  const bool l_has_info (a_datum.has_information ());
  {
    project & l_temp (const_cast< project & >(a_datum));
    {
      const static std::string l_new_value ("eMqmqoO44");
      // Check attributes
      {
        std::string l_value;
        bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value and not l_value.empty ()
                  , "get_attribute('label') matches label()");
        const static std::string l_nv2 ("tca7VprI8");
        const std::string l_str (l_new_value != l_value ? l_new_value : l_nv2);
        monitor_function (utility::contract_error::always
                          , boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_str)
                          , "can not change the label attribute of project.");
      }
      if (not a_datum.is_src () or a_datum.is_open ())
      {
        {
          std::string l_value;
          bool_test (not a_datum.get_attribute (data_access::names::worker_name (), l_value) or a_datum.workers () == l_value
                    , "no workers or get_attribute('worker') matches workers()");
          l_temp.add_worker_label (l_new_value);
          bool_test (data_access::names::list (l_value, l_new_value) == a_datum.workers (), "added worker label");
          l_temp.set_attribute (data_access::names::worker_name (), l_value);
          bool_test (l_value == a_datum.workers (), "reset worker list");
        }
        {
          std::string l_value;
          bool_test (not a_datum.get_attribute (data_access::names::outcome_name (), l_value) or a_datum.outcomes () == l_value
                    , "no outcomes or get_attribute('outcome') matches outcomes()");
          l_temp.add_outcome_label (l_new_value);
          bool_test (data_access::names::list (l_value, l_new_value) == a_datum.outcomes (), "added outcome label");
          l_temp.set_attribute (data_access::names::outcome_name (), l_value);
          bool_test (l_value == a_datum.outcomes (), "reset outcome list");
        }
      }
      else
      {
        std::string l_value;
        bool_test (not a_datum.get_attribute (data_access::names::worker_name (), l_value)
                  , "no workers in closed project");
        bool_test (not a_datum.get_attribute (data_access::names::outcome_name (), l_value)
                  , "no outcomes in closed project");
      }
      {
          std::string l_value;
          bool_test (not a_datum.get_attribute (data_access::names::href_name (), l_value)
                    , "no href in project");
      }
    }
    // Test add_experiment
    if (not a_datum.is_src () or a_datum.is_open ())
    {
      const static std::string l_label ("f5ekcPLl7");
      experiment & l_expt (l_temp.add_experiment (l_label));
      // find (const)
      bool_test (a_datum.find (&l_expt) != a_datum.end (), "found added experiment");
      bool_test (&l_expt == &(*(a_datum.find_experiment (l_label))), "find by label found added experiment");
      // remove
      l_temp.remove_child (l_expt);
      bool_test (a_datum.find (&l_expt) == a_datum.end (), "did not find removed experiment");
  
      std::string l_label2 (l_label);
      basic_datum & l_expt2 (l_temp.add_child (data_access::names::experiment_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_expt2) != a_datum.end (), "found added experiment 2");
      bool_test (&l_expt2 == &(*(a_datum.find_experiment (l_label))), "find by label found added experiment 2");
      // remove
      l_temp.remove (l_temp.find (&l_expt2));
      bool_test (a_datum.find (&l_expt2) == a_datum.end (), "did not find removed experiment 2");
    }
    // Test add_compound_ref
    if (not a_datum.is_src () or a_datum.is_open ())
    {
      const static std::string l_label ("ZXhn78rda");
      compound_ref & l_cmpdref (l_temp.add_compound_ref (l_label));
      // find (const)
      bool_test (a_datum.find (&l_cmpdref) != a_datum.end (), "found added compound_ref");
      // remove
      l_temp.remove_child (l_cmpdref);
      bool_test (a_datum.find (&l_cmpdref) == a_datum.end (), "did not find removed compound_ref");
  
      std::string l_label2 (l_label);
      basic_datum & l_cmpdref2 (l_temp.add_child (data_access::names::compound_ref_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_cmpdref2) != a_datum.end (), "found added compound_ref 2");
      // remove
      l_temp.remove (l_temp.find (&l_cmpdref2));
      bool_test (a_datum.find (&l_cmpdref2) == a_datum.end (), "did not find removed compound_ref 2");
    }
    // Test add_info
    if (not a_datum.is_src () or a_datum.is_open ())
    {
      const static std::string l_label ("aaQchIo96");
      if (a_datum.has_information ())
      {
        // You cannot add second info
        std::string l_str (l_label);
        monitor_function (utility::contract_error::require
                          , boost::bind (&project::add_info, &l_temp, l_str)
                          , "can not add second info to a project.");
        monitor_function (utility::contract_error::always
                          , boost::bind (&project::add_child, &l_temp, data_access::names::info_name (), l_str)
                          , "can not add second info to a project.");
      }
      else
      {
        info & l_info (l_temp.add_info (l_label));
        bool_test (a_datum.has_information (), "has info object after adding.");
        // find (const)
        bool_test (&l_info == &(a_datum.information ()), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_info));
        bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info");
  
        std::string l_label2 (l_label);
        basic_datum & l_info2 (l_temp.add_child (data_access::names::info_name (), l_label2));
        bool_test (a_datum.has_information (), "has info object 2 after adding.");
        // find (const)
        bool_test (&l_info2 == &(a_datum.information ()), "found added info 2");
        // remove
        l_temp.remove (l_temp.find (&l_info2));
        bool_test (a_datum.find (&l_info2) == a_datum.end (), "did not find removed info 2");
      }
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_expt_size == a_datum.size_experiment (), "have original number of experiment objects");
  bool_test (l_cmpd_size == a_datum.size_compound_ref (), "have original number of compound-ref objects");
  bool_test (l_has_info == a_datum.has_information (), "have original number of info objects");
  bool_test (l_orig_size >= (a_datum.size_experiment () + a_datum.size_compound_ref () + (a_datum.has_information ()?1:0))
             , "number of children >= to number of special children.");
  // check child/parent
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end  (), "object is child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on settings objects.
 */
void test_object_visitor::visit_settings(const settings & a_datum) 
{
  log () << COMM << "entering on_visit (settings) [" << a_datum.label () << "]\n";
  // Count settings objs.
  ++m_type[s_settings];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::settings_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const bool l_has_data (a_datum.has_data ());
  const bool l_has_info (a_datum.has_information ());
  {
    settings & l_temp (const_cast< settings & >(a_datum));
    const static std::string l_label ("tca7VprI8");
    // Check attributes
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value and not l_value.empty ()
                , "get_attribute('label') matches label()");
      const static std::string l_label2 ("Vnq4okeO2");
      const std::string & l_str (l_label != l_value ? l_label : l_label2);
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_str)
                        , "can not change label attribute of settings");
    }
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::outcome_name (), l_value) and a_datum.outcomes () == l_value
                , "get_attribute('outcome') matches outcomes()");
      l_temp.add_outcome_label (l_label);
      bool_test (data_access::names::list (l_value, l_label) == a_datum.outcomes (), "added new outcome label");
      l_temp.set_attribute (data_access::names::outcome_name (), l_value);
      bool_test (a_datum.outcomes () == l_value, "reset outcomes");
    }
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::filter_name (), l_value) and a_datum.filter_list () == l_value
                , "get_attribute('filter') matches filter_list()");
      l_temp.add_filter_label (l_label);
      bool_test (data_access::names::list (l_value, l_label) == a_datum.filter_list (), "added new filter label");
      l_temp.set_attribute (data_access::names::filter_name (), l_value);
      bool_test (a_datum.filter_list () == l_value, "reset filters");
    }
    // Test add_data
    {
      const static std::string l_label2 ("p1ug6gzCD");
      if (a_datum.has_data ())
      {
        // You cannot add second data
        std::string l_str (l_label2);
        monitor_function (utility::contract_error::require, boost::bind (&settings::add_datum, &l_temp, l_str)
                          , "can not add second data to a settings.");
        monitor_function (utility::contract_error::require, boost::bind (&basic_datum::add_child, &l_temp, data_access::names::datum_name (), l_str)
                          , "can not add second data to a settings.");
      }
      else
      {
        datum & l_data (l_temp.add_datum (l_label2));
        bool_test (a_datum.has_data (), "has data object after adding.");
        // find (const)
        bool_test (&l_data == &(a_datum.data ()), "found added data");
        // remove
        l_temp.remove (l_temp.find (&l_data));
        bool_test (a_datum.find (&l_data) == a_datum.end (), "did not find removed data");
        std::string l_label3 (l_label2);
        basic_datum & l_data2 (l_temp.add_child (data_access::names::datum_name (), l_label3));
        bool_test (a_datum.has_data (), "has data 2 object after adding.");
        // find (const)
        bool_test (&l_data2 == &(a_datum.data ()), "found added data 2");
        // remove
        l_temp.remove (l_temp.find (&l_data2));
        bool_test (a_datum.find (&l_data2) == a_datum.end (), "did not find removed data 2");
      }
    }
    // Test add_info
    {
      const static std::string l_label2 ("l9xAuh5wJ");
      if (a_datum.has_information ())
      {
        // You cannot add second info
        std::string l_str (l_label2);
        monitor_function (utility::contract_error::require, boost::bind (&settings::add_info, &l_temp, l_str)
                          , "can not add second info to a settings.");
        monitor_function (utility::contract_error::require, boost::bind (&settings::add_child, &l_temp, data_access::names::info_name (), l_str)
                          , "can not add second info to a settings.");
      }
      else
      {
        info & l_info (l_temp.add_info (l_label2));
        bool_test (a_datum.has_information (), "has info object after adding.");
        // find (const)
        bool_test (&l_info == &(a_datum.information ()), "found added info");
        // remove
        l_temp.remove (l_temp.find (&l_info));
        bool_test (a_datum.find (&l_info) == a_datum.end (), "did not find removed info");
        std::string l_label3 (l_label2);
        basic_datum & l_info2 (l_temp.add_child (data_access::names::info_name (), l_label3));
        bool_test (a_datum.has_information (), "has info object 2 after adding.");
        // find (const)
        bool_test (&l_info2 == &(a_datum.information ()), "found added info 2");
        // remove
        l_temp.remove (l_temp.find (&l_info2));
        bool_test (a_datum.find (&l_info2) == a_datum.end (), "did not find removed info 2");
      }
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_has_data == a_datum.has_data (), "have original number of datum objects");
  bool_test (l_has_info == a_datum.has_information (), "have original number of info objects");
  bool_test (l_orig_size >= (unsigned int)((a_datum.has_data ()?1:0) + (a_datum.has_information ()?1:0))
             , "number of children >= to number of special children.");
  // Check parent/child
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is a child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Operate on text objects.
 */
void test_object_visitor::visit_text(const text & a_datum) 
{
  log () << COMM << "entering on_visit (text)\n";
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::text_type (), "type name matches");
  // Do basic_text tests
  test_basic_text (a_datum);
  // Count basic_datum objects
  ++m_type[0];
  ++m_type[s_text];

}

/**
 * Operate on worker objects.
 */
void test_object_visitor::visit_worker(const worker & a_datum) 
{
  log () << COMM << "entering on_visit (worker) [" << a_datum.label () << "]\n";
  // Count worker objs.
  ++m_type[s_worker];
  // Check serial_name
  bool_test (a_datum.serial_name () == data_access::names::worker_name (), "type name matches");
  /////////////////////
  // Modifying methods
  const unsigned int l_orig_size (a_datum.size ());
  const unsigned int l_info_size (a_datum.size_info ());
  {
    worker & l_temp (const_cast< worker & >(a_datum));
    const static std::string l_label ("gO19juzbM");
    // Check attributes
    {
      std::string l_value;
      bool_test (a_datum.get_attribute (data_access::names::label_name (), l_value) and a_datum.label () == l_value
                , "get_attribute('label') matches label()");
      const static std::string l_label2 ("Vnq4okeO2");
      const std::string & l_str (l_label != l_value ? l_label : l_label2);
      monitor_function (utility::contract_error::always, boost::bind (&basic_datum::set_attribute, &l_temp, data_access::names::label_name (), l_str)
                        , "can not set label attribute when it is already set.");
    }
    // Test add_info
    {
      info & l_ident (l_temp.add_info (l_label));
      // find (const)
      bool_test (a_datum.find (&l_ident) != a_datum.end (), "found added info");
      // remove
      l_temp.remove_child (l_ident);
      bool_test (a_datum.find (&l_ident) == a_datum.end (), "did not find removed info");
      std::string l_label2 (l_label);
      basic_datum & l_info2 (l_temp.add_child (data_access::names::info_name (), l_label2));
      // find (const)
      bool_test (a_datum.find (&l_info2) != a_datum.end (), "found added info 2");
      // remove
      l_temp.remove (l_temp.find (&l_info2));
      bool_test (a_datum.find (&l_info2) == a_datum.end (), "did not find removed info 2");
    }
  }
  // Test number of children is unchanged.
  bool_test (l_orig_size == a_datum.size (), "have original number of child objects");
  bool_test (l_info_size == a_datum.size_info (), "have original number of info objects");
  bool_test (l_orig_size >= a_datum.size_info (), "number of children >= to number of special children.");
  // check parent/child
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "object is a child of parent");
  // Call parent method
  visit_basic_datum (a_datum);

}

/**
 * Override in derived classes to perform unit test.
 */
void test_object_visitor::main_test() const 
{
  // DO
  int Result (0);
  m_formula = NULL;
  log () << COMM << "Getting test database\n";
  if (m_doc_filename.empty ())
  {
    log () << IFNO << "No data filename set, please provide a database name\n";
    Result = 1;
  }
  if (not boost::filesystem::exists (m_doc_filename))
  {
    log () << IFNO << "Data filename: [" << m_doc_filename << "]\n does not exist, please provide a valid database name\n";
    Result = 1;
  }
  
  if (0 == Result)
  {
    data_scheme::program l_program;
    // Read in the document..
    l_program.connect (m_doc_filename);
    // Perform tests on data_scheme objects.
    l_program.visit (const_cast< test_object_visitor & >(*this));
    log () << WTCH << "visit finished" << std::endl;
    // Move old version
    const boost::filesystem::path l_dir (m_doc_filename);
    const boost::filesystem::path l_base (l_dir.branch_path ());
    const boost::filesystem::path l_old (l_base.branch_path () / (l_base.leaf () + ".old"));
    const boost::filesystem::path l_new (l_base.branch_path () / (l_base.leaf () + ".new"));
    log () << COMM << "Base path is :" << l_base.string () << "\n";
    log () << COMM << "Old path is :" << l_old.string () << "\n";
    log () << COMM << "New path is :" << l_new.string () << "\n";
    boost::filesystem::rename (l_base, l_old);
    // Attempt to save 'new' version
    log () << COMM << "Attempting to write Program.\n";
    l_program.write ();
    if (boost::filesystem::exists (l_new))
    {
      boost::filesystem::remove_all (l_new);
    }
    boost::filesystem::rename (l_base, l_new);
    boost::filesystem::rename (l_old, l_base);
  }
  
  show_coverage ();

}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_object_visitor::register_options() 
{
  if (NULL == utility::program_options::helper ().public_options ().find_nothrow ("database", true))
  {
    // Add option
    utility::program_options::helper ().public_options ().add_options () ("database", boost::program_options::value< std::string >(&(this->m_doc_filename)), "Test database file");
  }

}

/**
 * Test a basic_text object
 */
void test_object_visitor::test_basic_text(const basic_text & a_datum) 
{
  const static std::string l_str ("some-text");
  // set_attribute
  monitor_function (utility::contract_error::always, boost::bind (&basic_text::set_attribute, const_cast< basic_text* > (&a_datum), l_str, l_str), "can not set attribute of text objects.");
  // basic_text is always empty
  bool_test (a_datum.empty () and 0 == a_datum.size () and a_datum.begin () == a_datum.end ()
             , "basic_text object is always empty.");
  // basic_text has no attributes (no-testable)
  // You cannot add children to a basic_text
  monitor_function (utility::contract_error::always, boost::bind (&basic_text::add_child, const_cast< basic_text* > (&a_datum), l_str, l_str), "can not add children to text objects.");
  // Cannot add PIs, Comments or Text
  monitor_function (utility::contract_error::always, boost::bind (&basic_text::add_comment, const_cast< basic_text* > (&a_datum), l_str), "can not add comment to text objects.");
  monitor_function (utility::contract_error::always, boost::bind (&basic_text::add_pi, const_cast< basic_text* > (&a_datum), l_str, l_str), "can not add P.I. to text objects.");
  monitor_function (utility::contract_error::always, boost::bind (&basic_text::add_text, const_cast< basic_text* > (&a_datum), l_str), "can not add text to text objects.");
  // basic_text is a child of its parent.
  bool_test (a_datum.parent ().find (&a_datum) != a_datum.parent ().end (), "text is a child of its parent.");

}

/**
 * Print the number of types analysed.
 */
void test_object_visitor::show_coverage() const 
{
  log () << TTLE << "Type coverage\n"
         << WTCH << "comments      : " << m_type[s_comment] << "\n"
         << WTCH << "compounds     : " << m_type[s_compound] << "\n"
         << WTCH << "compound_refs : " << m_type[s_compound_ref] << "\n"
         << WTCH << "datums        : " << m_type[s_datum] << "\n"
         << WTCH << "experiments   : " << m_type[s_experiment] << "\n"
         << WTCH << "filters       : " << m_type[s_filter] << "\n"
         << WTCH << "formulae      : " << m_type[s_formula] << "\n"
         << WTCH << "identitys     : " << m_type[s_identity] << "\n"
         << WTCH << "infos         : " << m_type[s_info] << "\n"
         << WTCH << "mechanisms    : " << m_type[s_mechanism] << "\n"
         << WTCH << "memos         : " << m_type[s_memo] << "\n"
         << WTCH << "P.I.s         : " << m_type[s_pi] << "\n"
         << WTCH << "programs      : " << m_type[s_program] << "\n"
         << WTCH << "projects      : " << m_type[s_project] << "\n"
         << WTCH << "settings      : " << m_type[s_settings] << "\n"
         << WTCH << "texts         : " << m_type[s_text] << "\n"
         << WTCH << "workers       : " << m_type[s_worker] << "\n"
         << WTCH << "basic_datums  : " << m_type[0] << "\n";
  int l_total (0);
  for (unsigned int i (1); i < s_size; ++i)
  {
    l_total += m_type [i];
  }
  log () << WTCH << "total         : " << l_total << "\n";
  

}


} // namespace data_scheme
