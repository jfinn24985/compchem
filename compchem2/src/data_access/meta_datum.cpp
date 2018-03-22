// ------------------------------------------------------------
// class: meta_datum
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/meta_datum.hpp"

// Manual includes
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <algorithm>
// -- 
namespace data_access {

/**
 * The map of names to meta_datum objects.
 */
std::auto_ptr<meta_datum::name_map_t> meta_datum::s_map;

/**
 * Get the key attribute for the tag a_name.
 * 
 * I this returned the key_attribute then the reader would have to 
 * find the attribute and meta_datum would be independent of the
 * reader type.
 * 
 * \pre names::is_element_name (a_name) 
 */
const meta_datum & meta_datum::by_name(const std::string & a_name)

{
  if (NULL == s_map.get ())
  {
    initialise_map ();
  }
  const name_map_t * l_map (s_map.get ());
  name_map_t::const_iterator l_defn (l_map->find (a_name));
  COMPCHEM_ALWAYS(l_map->end () != l_defn, "No definition object for " + a_name);
  return *l_defn;

}

/**
 * Initialise map with data_scheme types.
 */
void meta_datum::initialise_map()

{
#line 100
  // Create the map object.
  if (NULL == s_map.get ())
  {
    s_map.reset (new name_map_t ());
  }
  // Some name refs
  const std::string l_category_name (data_access::names::category_name ());
  const std::string l_compound_name (data_access::names::compound_name ());
  const std::string l_compound_ref_name = data_access::names::compound_ref_name ();
  const std::string l_datum_name (data_access::names::datum_name ());
  const std::string l_dim_name (data_access::names::dimension_name ());
  const std::string l_experiment_name (data_access::names::experiment_name ());
  const std::string l_filter_name (data_access::names::filter_name ());
  const std::string l_format_name (data_access::names::format_name ());
  const std::string l_formula_name (data_access::names::formula_name ());
  const std::string l_href_name (data_access::names::href_name ());
  const std::string l_label_name (data_access::names::label_name ());
  const std::string l_identity_name (data_access::names::identity_name ());
  const std::string l_info_name (data_access::names::info_name ());
  const std::string l_mech_name (data_access::names::mechanism_name ());
  const std::string l_memo_name (data_access::names::memo_name ());
  const std::string l_memo_src_name (data_access::names::memo_src_name ());
  const std::string l_outcome_name (data_access::names::outcome_name ());
  const std::string l_ord_name (data_access::names::ordinal_name ());
  const std::string l_program_name (data_access::names::program_name ());
  const std::string l_project_name (data_access::names::project_name ());
  const std::string l_project_src_name (data_access::names::project_src_name ());
  const std::string l_ref_name (data_access::names::reference_name ());
  const std::string l_scale_name (data_access::names::scale_name ());
  const std::string l_settings_name (data_access::names::settings_name ());
  const std::string l_type_name (data_access::names::type_name ());
  const std::string l_worker_name (data_access::names::worker_name ());
  
  const std::string l_nul;
  /**
   * The definition object for the compound type
   */
  {
    std::string l_key (l_compound_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_compound_name, l_label_name
                                               , data_access::names::list (l_identity_name, l_info_name, l_formula_name)
                                               , l_category_name));
  }
  
  /**
   * The definition object for the compound-ref type
   */
  {
    std::string l_key (l_compound_ref_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_compound_ref_name, l_label_name, l_nul, l_formula_name));
  }
  
  /**
   * The definition object for the datum type.
   */
  {
    std::string l_key (l_datum_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_datum_name, l_outcome_name, l_datum_name
                                               , data_access::names::list (l_dim_name, l_format_name, l_scale_name, l_ord_name)));
  }
  
  /**
   * The definition object for experiment type.
   */
  {
    std::string l_key (l_experiment_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_experiment_name, l_ref_name
                                               , data_access::names::list (l_info_name, l_memo_name, l_memo_src_name)
                                               , data_access::names::list (data_access::names::date_start_name (), data_access::names::date_end_name (), data_access::names::priority_name (), l_worker_name)));
  }
  
  /**
   * The definition object for filter type
   */
  {
    std::string l_key (l_filter_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_filter_name, l_label_name, l_nul
                                               , data_access::names::list (l_href_name, l_outcome_name, l_type_name)));
  }
  
  /**
   * The definition object for formula type
   */
  {
    std::string l_key (l_formula_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_formula_name
                                               , l_label_name
                                               , data_access::names::list (l_info_name, l_identity_name)
                                               , l_nul));
  }
  
  /**
   * The definition object for identity type.
   */
  {
    std::string l_key (l_identity_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_identity_name, l_type_name, l_nul, l_nul));
  }
  
  /**
   * The definition object for info type
   */
  {
    std::string l_key (l_info_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_info_name, data_access::names::name_name (), l_nul, l_nul));
  }
  
  /**
   * The definition object for mechanism type
   */
  {
    std::string l_key (l_mech_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_mech_name, l_label_name
                                               , data_access::names::list (l_info_name, l_filter_name, l_settings_name)
                                               , l_nul));
  }
  
  /**
   * The definition object for memo type
   */
  // {
  //   std::string l_key (l_memo_name);
  //   s_map->insert(l_key,
  //                 new data_access::meta_datum (l_memo_name, l_label_name
  //                                              , data_access::names::list (l_datum_name, l_info_name)
  //                                              , data_access::names::list (l_mech_name, l_href_name, l_filter_name, l_formula_name, l_compound_name)));
  // }
  {
    std::string l_key (l_memo_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_memo_name, l_label_name
                                               , data_access::names::list (l_datum_name, l_info_name)
                                               , data_access::names::list (l_mech_name, l_filter_name, l_formula_name, l_compound_name)));
  }
  
  /**
   * The definition object for memo-src type
   */
  {
    std::string l_key (l_memo_src_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_memo_src_name, l_href_name, l_nul
                                               , data_access::names::list (l_label_name, l_formula_name, l_compound_name)));
  }
  
  /**
   * The definition object for program type
   */
  {
    std::string l_key (l_program_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_program_name, l_nul
                                               , data_access::names::list (l_info_name, l_worker_name, l_mech_name, l_compound_name, l_project_name, l_project_src_name)
                                               , l_nul));
  }
  
  /**
   * The definition object for project type
   */
  {
    std::string l_key (l_project_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_project_name, l_label_name
                                               , data_access::names::list (l_info_name, l_experiment_name)
                                               , data_access::names::list (l_outcome_name, l_worker_name, l_href_name)));
  }
  
  /**
   * The definition object for project-src type
   */
  {
    std::string l_key (l_project_src_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_project_src_name
                                               , l_href_name, l_nul, l_label_name));
  }
  
  /**
   * The definition object for settings type
   */
  {
    std::string l_key (l_settings_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_settings_name, l_label_name
                                               , data_access::names::list (l_datum_name, l_info_name)
                                               , data_access::names::list (l_outcome_name, l_filter_name)));
  }
  
  /**
   * The definition object for worker type
   */
  {
    std::string l_key (l_worker_name);
    s_map->insert(l_key,
                  new data_access::meta_datum (l_worker_name, l_label_name, l_info_name, l_nul));
  }

  COMPCHEM_ENSURE(NULL != s_map.get (), "Meta-datum map still undefined after creating data-scheme objects.");
}

/**
 * Main constructor. The tag name for this object is a_name, the list of 
 * child tag names is a_child_list and the list of attribute names is 
 * a_attribute_list. The attribute name a_key is a key attribute (#REQUIRED)
 * for the element, it will automatically be added to a_attribute_list.
 * Lists are space separated list of words.
 * 
 * \pre names::is_element_name (a_name) and names::is_element_name (ALL in a_child_list) and names::is_attribute_name (ALL in a_attribute_list)
 * 
 * PROGRAM NOTE, List parsing is:
 * m_children.insert(istream_iterator<..>(istringstream(a_child_list)), istream_iterator<...>())
 */
meta_datum::meta_datum(const std::string & a_name, const std::string & a_key, const std::string & a_child_list, const std::string & a_attribute_list)
: key_attribute (a_key)
, m_attributes ()
, m_children ()
, name (a_name)
{
  if (not a_child_list.empty ())
  {
    boost::algorithm::split (m_children, a_child_list, boost::algorithm::is_any_of (" "), boost::algorithm::token_compress_on);
    COMPCHEM_REQUIRE(static_cast<std::set< std::string >::size_type>(std::count_if (m_children.begin (), m_children.end (), &names::is_element_name)) == m_children.size (), ("For " + name + " at least one child element name was invalid in \"" + a_child_list + "\""));
  }
  if (not a_attribute_list.empty ())
  {
    boost::algorithm::split (m_attributes, a_attribute_list, boost::algorithm::is_any_of (" "), boost::algorithm::token_compress_on);
  }
  if (not key_attribute.empty () and not has_attr_name (key_attribute))
  {
  //  DLOG("Inserting key attribute "); DLOG (key_attribute); DLOG(" for "); DLOG(name); DLOG ("\n"); 
    m_attributes.insert (key_attribute);
  }
  COMPCHEM_REQUIRE(static_cast<std::set< std::string >::size_type>(std::count_if (m_attributes.begin (), m_attributes.end (), &names::is_attribute_name)) == m_attributes.size (), ("For " + name + " at least one attribute name was invalid in \"" + a_attribute_list + " " + key_attribute + "\""));

}

meta_datum::~meta_datum() throw() 
{}


} // namespace data_access
