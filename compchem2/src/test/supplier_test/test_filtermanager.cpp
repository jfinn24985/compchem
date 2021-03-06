// ------------------------------------------------------------
// class: test_filtermanager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/supplier_test/test_filtermanager.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/program.hpp"
#include "utility/program_options.hpp"
#include "data_scheme/project.hpp"
#include "supplier/supplier_helper.hpp"
#include "utility/test_manager.hpp"
#include "data_access/xml_write_visitor.hpp"
#include "data_access/xml_access_helper.hpp"

namespace supplier {

/**
 * The class exemplar
 */
test_filtermanager test_filtermanager::s_exemplar;

/**
 * Run tests on filter_manager class
 */
void test_filtermanager::main_test() const 
{
  if (m_database.empty ()or not boost::filesystem::exists (m_database))
  {
    log () << "No database set for filter manager test.\n";
  }
  else
  {
    supplier::filter_manager l_manager;
    log () << WTCH << "Loading database \"" << m_database << "\"\n";
    data_scheme::program l_program;
    l_program.connect (m_database);
    if (l_program.begin_mechanism () == l_program.end_mechanism ())
    {
      log () << EXCP << "No mechanisms in database.\n";
    }
    else
    {
      log () << WTCH << "Loading detect filters.\n";
      l_manager.discover_filters (l_program.begin_mechanism (), l_program.end_mechanism ());
      log () << WTCH << "Have " << l_manager.count_filters (data_access::names::detect_filter_name ());
      log () << " mechanism type detect filter(s)\n";
    }
    const std::string l_expt_ref (supplier_helper::helper ().target_experiment ());
    const std::string l_memo_label (supplier_helper::helper ().target_memo ());
    if (l_expt_ref.empty () or l_memo_label.empty ())
    {
      log () << IFNO << "No target experiment and/or memo defined.\n";
    }
    else 
    {
      data_scheme::program::project_iterator l_project (l_program.target_project ());
      if (l_project != l_program.end_project ())
      {
        data_scheme::project::experiment_iterator l_expt (l_project->find_experiment (l_expt_ref));
        if (l_expt != l_project->end_experiment ())
        {
          data_scheme::experiment::memo_iterator l_memo (l_expt->find_memo (l_memo_label));
          if (l_memo != l_expt->end_memo ())
          {
            log () << WTCH << "Found existing memo!\n";
            // data_scheme::experiment & l_expt (l_project.add_experiment ("TEST-REF"));
            // data_scheme::experiment & l_memo (l_expt.add_memo_src ("test.gout"));
            // l_manager.main_single (l_memo);
          }
          else
          {
            l_expt->add_memo (l_memo_label);
            l_memo = l_expt->find_memo (l_memo_label);
            bool_test (l_memo != l_expt->end_memo (), "found memo immediately after adding it!");
            const std::string l_cmpd_label (supplier_helper::helper ().target_compound ());
            const std::string l_form_label (supplier_helper::helper ().target_formula ());
            if (l_cmpd_label.empty ())  
            {
              log () << IFNO << "No compound label given for the new memo. Memo object will\n";
              log () << "        not be valid.\n";
            }
            else
            {
              data_scheme::program::compound_iterator l_cmpd (l_program.find_compound (l_cmpd_label));
              if (l_cmpd == l_program.end_compound ())
              {
                l_program.add_compound (l_cmpd_label);
                l_cmpd = l_program.find_compound (l_cmpd_label);
                bool_test (l_cmpd != l_program.end_compound (), "found compound immediately after adding it!");
                log () << WTCH << "Added new compound with label: \"" << l_cmpd_label << "\"\n";
              }
              if (l_form_label.empty ())
              {
                log () << IFNO << "No formula label given for the new memo. Memo object will\n";
                log () << "        not be valid.\n";
              }
              else
              {
                data_scheme::compound::formula_iterator l_form (l_cmpd->find_formula (l_form_label));
                if (l_form == l_cmpd->end_formula ())
                {
                  l_memo->compound_formula (l_cmpd->add_formula (l_form_label));
                  log () << WTCH << "Added new formula with label: \"" << l_form_label << "\"\n";
                }
                else
                {
                  l_memo->compound_formula (*l_form);
                }
              }
            }
            // l_memo_ob.make_src (l_memo_label + ".xml");
            const boost::filesystem::path l_file (supplier_helper::helper ().target_file ());
            if (not l_file.empty ())
            {
              if (not boost::filesystem::exists (l_file))
              {
                log () << IFNO << "File \"" << l_file.string () << "\" does not exist.\n";
              }
              else
              {
                l_manager.process (*l_memo, l_file.string ());
                log () << IFYS << "Filtered \"" << l_file.string () << "\".\n";
                data_access::xml_write_visitor l_writer (log ()
                                                        , l_memo->serial_name ()
                                                        , data_access::xml_access_helper::helper ().public_id ()
                                                        , data_access::xml_access_helper::helper ().system_id ());
                l_writer.visit_basic_datum (*l_memo);
                log () << "\n";
              }
            }
          }
        }
      }
    }
    log () << "Current filter state:\n" << l_manager << "\n";
  }

}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_filtermanager::register_options() 
{
  // Add option
  utility::program_options::helper ().public_options ().add_options () ("database", boost::program_options::value< std::string >(&m_database), "Test database file");

}


} // namespace supplier
