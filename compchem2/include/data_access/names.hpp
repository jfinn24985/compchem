#ifndef COMPCHEM_NAMES_HPP
#define COMPCHEM_NAMES_HPP

// ------------------------------------------------------------
// class: names
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace data_access {

/**
 * This type is a container for names in the XML document schema and some DOM node helper functions additional to those provided in DOM.
 */
template<class T_xml_string>
class basic_names {
  public:
    typedef T_xml_string string_type;

    /**
     * Name of compound attribute in XML appl that allows compound grouping 
     */
    static const string_type & category_name()
    {
      const static string_type Result ("category");
      return Result;
    }

    /**
     * Special name for XML comments in application
     */
    static const string_type & comment_type()
    {
      const static string_type Result ("#CM");
      return Result;
    }

    /**
     * Name of chemical compound container element in XML appl
     */
    static const string_type & compound_name()
    {
      const static string_type Result ("compound");
      return Result;
    }

    /**
     * Name of chemical compound container element in XML appl
     */
    static const string_type & compound_ref_name()
    {
      const static string_type Result ("compound-ref");
      return Result;
    }

    /**
     * Name of starting date attribute in XML appl
     */
    static const string_type & date_start_name()
    {
      const static string_type Result ("date-start");
      return Result;
    }

    /**
     * Name of ending date attribute in XML appl
     */
    static const string_type & date_end_name()
    {
      const static string_type Result ("date-end");
      return Result;
    }

    /**
     * The name of datum elements in the XML appl.
     */
    static const string_type & datum_name()
    {
      const static string_type Result ("datum");
      return Result;
    }

    /**
     * The special name of filters that detect the mechanism of files.
     */
    static const string_type & detect_filter_name()
    {
      const static string_type Result ("detect");
      return Result;
    }

    /**
     * The datum object's dimension attribute name in the XML appl.
     */
    static const string_type & dimension_name()
    {
      const static string_type Result ("dim");
      return Result;
    }

    /**
     * Name of experiment container element in XML appl
     */
    static const string_type & experiment_name()
    {
      const static string_type Result ("experiment");
      return Result;
    }

    /**
     * Name of element containing details of filter that can be applied to output of enclosing mechanism
     */
    static const string_type & filter_name()
    {
      const static string_type Result ("filter");
      return Result;
    }

    /**
     * The URI "scheme" of a href attribute that indicates the target is a
     * local file.
     */
    static const string_type & file_scheme()
    {
      const static string_type Result ("file");
      return Result;
    }

    /**
     * The name of the datum format attribute in the XML appl.
     */
    static const string_type & format_name()
    {
      const static string_type Result ("format");
      return Result;
    }

    /**
     * Name of formula container element in XML appl
     */
    static const string_type & formula_name()
    {
      const static string_type Result ("formula");
      return Result;
    }

    /**
     * Name of URL link or filename attribute in XML appl
     */
    static const string_type & href_name()
    {
      const static string_type Result ("href");
      return Result;
    }

    /**
     * Value of [text/@name] for textual information relating to the containing element
     */
    static const string_type & info_name()
    {
      const static string_type Result ("info");
      return Result;
    }

    /**
     * Name of identity element in XML appl
     */
    static const string_type & identity_name()
    {
      const static string_type Result ("identity");
      return Result;
    }

    /**
     * Name of label attribute in XML appl
     */
    static const string_type & label_name()
    {
      const static string_type Result ("label");
      return Result;
    }

    /**
     * Name of a mechanism container element in XML appl
     */
    static const string_type & mechanism_name()
    {
      const static string_type Result ("mechanism");
      return Result;
    }

    /**
     * Name of element containing details of mechanism used in an experiment in XML appl
     */
    static const string_type & memo_name()
    {
      const static string_type Result ("memo");
      return Result;
    }

    /**
     * Name of chemical compound reference element in XML appl
     */
    static const string_type & memo_src_name()
    {
      const static string_type Result ("memo-src");
      return Result;
    }

    /**
     * The string for the name attribute of datum element in the XML appl.
     */
    static const string_type & name_name()
    {
      const static string_type Result ("name");
      return Result;
    }

    /**
     * The name of the bag element ordinal attribute in XML appl.
     */
    static const string_type & ordinal_name()
    {
      const static string_type Result ("ordinal");
      return Result;
    }

    /**
     * The name of attribute in XML appl.
     */
    static const string_type & outcome_name()
    {
      const static string_type Result ("outcome");
      return Result;
    }

    /**
     * The type name of filters that are dynamically loaded external code.
     */
    static const string_type & plugin_filter_name()
    {
      const static string_type Result ("plugin");
      return Result;
    }

    /**
     * Name of worker container element in XML appl
     */
    static const string_type & priority_name()
    {
      const static string_type Result ("priority");
      return Result;
    }

    /**
     * Special name for XML comments.
     */
    static const string_type & processing_instruction_type()
    {
      const static string_type Result ("#PI");
      return Result;
    }

    /**
     * Name of research program container element in XML appl
     */
    static const string_type & program_name()
    {
      const static string_type Result ("program");
      return Result;
    }

    /**
     * Name of project container element in XML appl
     */
    static const string_type & project_name()
    {
      const static string_type Result ("project");
      return Result;
    }

    /**
     * Name of project link element in XML appl
     */
    static const string_type & project_src_name()
    {
      const static string_type Result ("proj-src");
      return Result;
    }

    /**
     * Name of experiment (page) reference attribute in XML appl
     */
    static const string_type & reference_name()
    {
      const static string_type Result ("reference");
      return Result;
    }

    /**
     * The type name of internal filter that uses regular expressions.
     */
    static const string_type & regex_filter_name()
    {
      const static string_type Result ("regex");
      return Result;
    }

    /**
     * The name of the scale attribute in the XML appl.
     */
    static const string_type & scale_name()
    {
      const static string_type Result ("scale");
      return Result;
    }

    /**
     * Name of settings container element in XML appl
     */
    static const string_type & settings_name()
    {
      const static string_type Result ("settings");
      return Result;
    }

    /**
     * A white space.
     */
    static const string_type & space()
    {
      const static string_type Result (" ");
      return Result;
    }

    /**
     * Special name for XML text.
     */
    static const string_type & text_type()
    {
      const static string_type Result ("#TX");
      return Result;
    }

    /**
     * Name of the "type" attribute.
     */
    static const string_type & type_name()
    {
      const static string_type Result ("type");
      return Result;
    }

    /**
     * Name of worker container element in XML appl
     */
    static const string_type & worker_name()
    {
      const static string_type Result ("worker");
      return Result;
    }

    /**
     * convenience function to create a list.
     */
    static string_type list(string_type i1, const string_type & i2)
    {
      list_add (i1, i2);
      return i1;
    }

    /**
     * convenience function to create a list.
     */
    static string_type list(string_type i1, const string_type & i2, const string_type & i3)
    {
      list_add (i1, i2);
      list_add (i1, i3);
      return i1;
    }

    /**
     * convenience function to create a list.
     */
    static string_type list(string_type i1, const string_type & i2, const string_type & i3, const string_type & i4)
    {
      list_add (i1, i2);
      list_add (i1, i3);
      list_add (i1, i4);
      return i1;
    }

    /**
     * convenience function to create a list.
     */
    static string_type list(string_type i1, const string_type & i2, const string_type & i3, const string_type & i4, const string_type & i5)
    {
      list_add (i1, i2);
      list_add (i1, i3);
      list_add (i1, i4);
      list_add (i1, i5);
      return i1;
    }

    /**
     * convenience function to create a list.
     */
    static string_type list(string_type i1, const string_type & i2, const string_type & i3, const string_type & i4, const string_type & i5, const string_type & i6)
    {
      list_add (i1, i2);
      list_add (i1, i3);
      list_add (i1, i4);
      list_add (i1, i5);
      list_add (i1, i6);
      return i1;
    }
    

    /**
     * convenience function to append a single item to an existing list. Handles 
     * the case of empty arguments.
     */
    static void list_add(string_type & i1, const string_type & i2)
    {
      if (not i2.empty ())
      {
        if (i1.empty ())
        {
          i1.assign (i2);
        }
        else
        {
          i1.append (basic_names::space ());
          i1.append (i2);
        }
      }
    }

    /**
     * Is a_name one of the registered object names?
     */
    static bool is_element_name(const std::string& a_name);

    /**
     * Is a_name one of the registered attribute names?
     */
    static bool is_attribute_name(const std::string& a_attr);

    /**
     * Is a_name one of the registered special value names?
     */
    static bool is_special_name(const std::string& a_value);

    /**
     * Test if a_name is a type name.
     */
    static bool is_type_name(const std::string& a_name);

};
/**
 * Is a_name one of the registered object names?
 */
template<class T_xml_string>
bool basic_names<T_xml_string>::is_element_name(const std::string& a_name)

{
  return (a_name == basic_names<T_xml_string>::compound_name ()
      || a_name == basic_names<T_xml_string>::compound_ref_name ()
      || a_name == basic_names<T_xml_string>::datum_name ()
      || a_name == basic_names<T_xml_string>::experiment_name ()
      || a_name == basic_names<T_xml_string>::filter_name ()
      || a_name == basic_names<T_xml_string>::formula_name ()
      || a_name == basic_names<T_xml_string>::info_name ()
      || a_name == basic_names<T_xml_string>::identity_name ()
      || a_name == basic_names<T_xml_string>::mechanism_name ()
      || a_name == basic_names<T_xml_string>::memo_name ()
      || a_name == basic_names<T_xml_string>::memo_src_name ()
      || a_name == basic_names<T_xml_string>::program_name ()
      || a_name == basic_names<T_xml_string>::project_name ()
      || a_name == basic_names<T_xml_string>::project_src_name ()
      || a_name == basic_names<T_xml_string>::settings_name ()
      || a_name == basic_names<T_xml_string>::worker_name ());

}

/**
 * Is a_name one of the registered attribute names?
 */
template<class T_xml_string>
bool basic_names<T_xml_string>::is_attribute_name(const std::string& a_attr)

{
  return (a_attr == basic_names<T_xml_string>::category_name ()
      || a_attr == basic_names<T_xml_string>::compound_name ()
      || a_attr == basic_names<T_xml_string>::date_start_name ()
      || a_attr == basic_names<T_xml_string>::date_end_name ()
      || a_attr == basic_names<T_xml_string>::dimension_name ()
      || a_attr == basic_names<T_xml_string>::filter_name ()
      || a_attr == basic_names<T_xml_string>::format_name ()
      || a_attr == basic_names<T_xml_string>::formula_name ()
      || a_attr == basic_names<T_xml_string>::href_name ()
      || a_attr == basic_names<T_xml_string>::label_name ()
      || a_attr == basic_names<T_xml_string>::mechanism_name ()
      || a_attr == basic_names<T_xml_string>::name_name ()
      || a_attr == basic_names<T_xml_string>::ordinal_name ()
      || a_attr == basic_names<T_xml_string>::outcome_name ()
      || a_attr == basic_names<T_xml_string>::priority_name ()
      || a_attr == basic_names<T_xml_string>::reference_name ()
      || a_attr == basic_names<T_xml_string>::scale_name ()
      || a_attr == basic_names<T_xml_string>::type_name ()
      || a_attr == basic_names<T_xml_string>::worker_name ());

}

/**
 * Is a_name one of the registered special value names?
 */
template<class T_xml_string>
bool basic_names<T_xml_string>::is_special_name(const std::string& a_value)

{
  return (a_value == basic_names<T_xml_string>::detect_filter_name ()
      || a_value == basic_names<T_xml_string>::file_scheme ()
      || a_value == basic_names<T_xml_string>::plugin_filter_name ()
      || a_value == basic_names<T_xml_string>::regex_filter_name ());

}

/**
 * Test if a_name is a type name.
 */
template<class T_xml_string>
bool basic_names<T_xml_string>::is_type_name(const std::string& a_name)
{
  return (a_name == basic_names<T_xml_string>::comment_type()
    || a_name == basic_names<T_xml_string>::processing_instruction_type ()
    || a_name == basic_names<T_xml_string>::text_type ());
}

/**
 * This is the std::string implemenation of basic_names
 */
typedef basic_names< std::string > names;

} // namespace data_access
#endif
