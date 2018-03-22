// ------------------------------------------------------------
// class: filter_manager_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/filter_manager_py.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/experiment.hpp"
#include "supplier/filter_manager.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/project.hpp"
#include "supplier/supplier_helper.hpp"
#include "utility/time_stamp.hpp"

// Manual includes
#include <boost/python.hpp>
// -
namespace supplier {

/**
 * Create a "unique" filename for the memo-src location.
 */
std::string filter_manager_py::memo_src_location(const boost::filesystem::path & a_path, const std::string& a_memo_label) 
{
  boost::filesystem::path Result (supplier::supplier_helper::helper ().target_memo_src ());
  if (Result.empty ())
  {
    Result = a_path.branch_path ();
  }
  // If Result is a directory or existing filename.
  if (boost::filesystem::exists (Result))
  {
    // Need to create a unique name, first set Result to the closest directory
    if (not boost::filesystem::is_directory (Result))
    {
      Result = Result.branch_path ();
    }
    // Now use a simple algorithm to create the filename. First use the memo-label
    // then use the memo-label with numbers from 00 to 99 appended.
    if (not boost::filesystem::exists (Result / (a_memo_label + ".xml")))
    {
      Result /= a_memo_label + ".xml";
    }
    else
    {
      for (unsigned int l (0); l < 100; ++l)
      {
        std::stringstream l_tmp (a_memo_label);
        l_tmp << std::setfill ('0') << std::setw (2) << l << ".xml";
        if (not boost::filesystem::exists (Result / l_tmp.str ()))
        {
          Result /= l_tmp.str ();
          break;
        }
        COMPCHEM_ALWAYS (l < 99, "Fatal error creating filename for memo-src");
      }
    }
  }
  else
  {
    if (not boost::filesystem::exists (Result.branch_path ()))
    {
      // Need to create some directories
      boost::filesystem::create_directory (Result.branch_path ());
    }
  }
  return Result.string ();

}

/**
 * Perform the configured filter operation on a_database.
 */
void filter_manager_py::process(const std::string& a_database) 
{
  const std::string l_expt_ref (supplier::supplier_helper::helper ().target_experiment ());
  const std::string l_memo_label (supplier::supplier_helper::helper ().target_memo ());
  const boost::filesystem::path l_file (supplier::supplier_helper::helper ().target_file ());
  utility::time_stamp l_time ("main", "CCfilter application start", std::cout);
  // Test for database, experiment reference, input-file and memo
  if (l_file.empty ())
  {
    std::cerr << "Error: No input file, please supply one on the command line.\n";
  }
  else if (not boost::filesystem::exists (l_file))
  {
    std::cerr << "Error: Supplied input-file \"" << l_file.string () << "\" does not exist.\n";
  }
  else if (l_expt_ref.empty ())
  {
    std::cerr << "Error: No target experiment, please supply one on the command line.\n";
  }
  else if (l_memo_label.empty ())
  {
    std::cerr << "Error: No target memo, please supply one on the command line.\n";
  }
  else if (a_database.empty ())
  {
    std::cerr << "Error: No database set, please set a default or supply one on the command line.\n";
  }
  else
  {
    supplier::filter_manager l_manager;
    l_time.log ("Initialised application");
    data_scheme::program l_program;
    l_program.connect (a_database);
    if (l_program.begin_mechanism () == l_program.end_mechanism ())
    {
      std::cerr << "Invalid database with no mechanisms.\n";
    }
    else
    {
      l_time.log ("Loaded database \"" + a_database + "\"");
      l_manager.discover_filters (l_program.begin_mechanism (), l_program.end_mechanism ());
      l_time.log ("Loaded [" + boost::lexical_cast < std::string > (l_manager.count_filters (data_access::names::detect_filter_name ())) + "] detect filters.");
      data_scheme::program::project_iterator l_project (l_program.target_project ());
      if (l_project != l_program.end_project ())
      {
        data_scheme::project::experiment_iterator l_expt (l_project->find_experiment (l_expt_ref));
        if (l_expt != l_project->end_experiment ())
        {
          data_scheme::experiment::memo_iterator l_memo (l_expt->find_memo (l_memo_label));
          if (l_memo != l_expt->end_memo ())
          {
            l_time.log ("Found existing memo!");
          }
          else
          {
            l_expt->add_memo (l_memo_label);
            l_memo = l_expt->find_memo (l_memo_label);
            COMPCHEM_ALWAYS (l_memo != l_expt->end_memo (), "Could not find memo immediately after adding it!");
            const
              std::string
            l_cmpd_label (supplier::supplier_helper::helper ().target_compound ());
            const
              std::string
            l_form_label (supplier::supplier_helper::helper ().target_formula ());
            COMPCHEM_ALWAYS (not l_cmpd_label.empty (), "No compound label given for the new memo. Memo object will not be valid.");
            data_scheme::program::compound_iterator l_cmpd (l_program.find_compound (l_cmpd_label));
            if (l_cmpd == l_program.end_compound ())
            {
              l_program.add_compound (l_cmpd_label);
              l_cmpd = l_program.find_compound (l_cmpd_label);
              COMPCHEM_ALWAYS (l_cmpd != l_program.end_compound (), "Could not find compound immediately after adding it!");
              l_time.log ("Added new compound with label: \"" + l_cmpd_label + "\"");
            }
            COMPCHEM_ALWAYS (not l_form_label.empty (), "No formula label given for the new memo. Memo object will not be valid.");
            data_scheme::compound::formula_iterator l_form (l_cmpd->find_formula (l_form_label));
            if (l_form == l_cmpd->end_formula ())
            {
              l_memo->compound_formula (l_cmpd->add_formula (l_form_label));
              l_time.log ("Added new formula with label: \"" + l_form_label + "\"");
            }
            else
            {
              l_memo->compound_formula (*l_form);
            }
            // memo-src location = user-supplied or sub-directory that contains l_file
            l_memo->make_src (memo_src_location (l_file, l_memo_label));
          }
          // We now have a valid memo object, procede to processing the target file;
          l_manager.process (*l_memo, l_file.string ());
          l_time.log ("Filtered \"" + l_file.string () + "\".");
          l_program.write ();
          l_time.log ("Database saved, program completed successfully.");
        }
      }
    }
  }

}

// This file defines the python wrappers for the data_scheme namespace
// into the data_scheme python module.
//
// * meta_datum
// * xml_file
// * xml_write_visitor

BOOST_PYTHON_MODULE(supplier)
{
  boost::python::class_< supplier::filter_manager_py, boost::noncopyable >("filter_manager",
        "Manage the filter process.",
        boost::python::init<>()
        )
  .def("process", &supplier::filter_manager_py::process
       , "Perform the configured filter process into the named database")
   ;
}


} // namespace supplier
