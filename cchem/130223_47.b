class script_writer
!!!144047.cpp!!!	generate() : void
// Local
// - A flag to indicate method can procede
bool l_not_done; 
// - The path of the base dir.
const std::string l_codename (state_object.get_code_name ().get_string ());
boost::filesystem::path l_dir (l_codename);

// Check options with user.
l_not_done = user_verify_options();

// If use wants to procede, attempt to create base directory.
if (l_not_done)
{
  // We want to create a directory called "state_object.getCodeName()".
  if (boost::filesystem::exists (l_dir))
    {
      // Have something with name.
      if (boost::filesystem::is_directory (l_dir))
	{
	  const std::string l_message("A directory called [" + l_dir.string() + "] already exists, continuing\nmay overwrite some of the contents of this directory.");
	  l_not_done = state_object.warning_and_ask (l_message, "Continue", "Cancel");
	}
      else
	{
	  const std::string l_message("A file called [" + l_dir.string() + "] already exists, continuing\nwill delete this file.");
	  l_not_done = state_object.warning_and_ask(l_message, "Continue", "Cancel");
	  // Remove file
	  if (l_not_done)
	    {
	      boost::filesystem::remove (l_dir);
	    }
	  // Create directory.
	  boost::filesystem::create_directory (l_dir);
	}
    }
  else
    {
      // Create directory.
      boost::filesystem::create_directory (l_dir);
    }
}

// Write the files.
if (l_not_done)
{
  // List of current algorithms
  std::vector< std::string > l_algorithm_set;
  // List of chain sets
  std::vector< std::string > l_chain_set;
  // - Resets header when set to true.
  bool l_firstfile = true;
  std::string l_current_filename;
  std::string l_next_filename;

  ALWAYS(boost::filesystem::is_directory (l_dir), "Unable to create dir.");

  // Get list of algorithms
  state_object.get_algorithms ().get_stringlist (l_algorithm_set);

  for (std::vector< std::string >::iterator l_algo_index = l_algorithm_set.begin ();
       l_algo_index != l_algorithm_set.end ();
       ++l_algo_index)
    {
      l_current_filename = make_filename (""
					  , l_codename
					  , ""
					  , ""
					  , *l_algo_index);
      output_file (*l_algo_index
		   , l_dir / l_current_filename
		   , ""
		   , l_firstfile);
      l_firstfile = false;
    }
  
  // Get list of chains.
  state_object.get_chain ().get_stringlist(l_chain_set);

  for (std::vector< std::string >::iterator l_chain_index = l_chain_set.begin ();
       l_chain_index != l_chain_set.end ();
       ++l_chain_index)
    {
      // Get algo list for this chain.
      config.get_stringlist (config.make_key("algorithm"
					     , "chain"
					     , *l_chain_index)
			     , l_algorithm_set);
      std::string l_next_filename;
      for (std::vector< std::string >::iterator l_algo_index = l_algorithm_set.begin ();
	   l_algo_index != l_algorithm_set.end ();
	   ++l_algo_index)
	{
	  // Only create filename once either for l_next_filename
	  // or l_current_filename.
	  if (not l_next_filename.empty ())
	    {
	      l_current_filename = l_next_filename;
	    }
	  else
	    {
	      l_current_filename = make_filename(*l_chain_index
						 , l_codename
						 , ""
						 , ""
						 , *l_algo_index);
	    }
	  if (l_algo_index + 1 != l_algorithm_set.end ())
	    {
	      l_next_filename = make_filename(*l_chain_index
					      , l_codename
					      , ""
					      , ""
					      , *(l_algo_index + 1));
	    }
	  else
	    {
	      l_next_filename.clear ();
	    }
	  // Create the output file
	  output_file (*l_algo_index
		       , l_dir / l_current_filename
		       , l_dir / l_next_filename
		       , l_firstfile);
	  l_firstfile = false;
	}
    }
}
!!!144175.cpp!!!	output_file(in a_algo : string, in a_filename : boost::filesystem::path, in a_chain_file : boost::filesystem::path, in a_reset_header : bool) : void
// LOCAL
std::ofstream l_os;
// PRE
REQUIRE(boost::filesystem::portable_name (a_filename.leaf ())
	, "Require a portable filename [" + a_filename.string () + "]");
REQUIRE(boost::filesystem::portable_name (a_chain_file.leaf ())
	, "Require a portable filename [" + a_chain_file.string () + "]");
// DO

// Open file for writing
l_os.open (a_filename.string ().c_str());
if (l_os)
{
  // Write header section
  if (a_reset_header || get_header ().empty ())
    {
      std::stringstream l_sos;
      write_header (l_sos);
      set_header (l_sos.str ());
    }
  l_os << get_header ();

  // Test is multi-part
  if (config.get_boolean (config.make_key ("algorithm", a_algo, "multi")))
    {
      // Have a multi-entry file
      int l_if_depth;
      std::vector< std::string > l_multipart_list;

      //  Get list
      config.get_stringlist (config.make_key ("algorithm", a_algo, "parts")
			     , l_multipart_list);
      // Create output for each part
      l_if_depth = -1;
      for (std::vector< std::string >::iterator l_cursor = l_multipart_list.begin ();
	   l_cursor != l_multipart_list.end ();
	   ++l_cursor, ++l_if_depth)
	{
  	  if (0 <= l_if_depth)
	    {
	      l_os << "if ( $status == 0 ) then\n\n";
	    }
	  // Write current body.
	  write_body (l_os, *l_cursor);
	}
      // Unwind if clauses
      for (; l_if_depth > 1; --l_if_depth)
	{
	  l_os << "endif\n\n";
	}
      // End file
      l_os << "exit( $status )\n\n";
    }
  else
    {
      // Have a single entry file
      write_body (l_os, a_algo);
    }
  // Add extra if a_chain_file
  if (not a_chain_file.empty ())
    {
      l_os << "if ( $status == 0 ) then\n\n";
      l_os << "  if ( ! -e \"" << a_chain_file.string () << ".pbs\" ) then\n\n";
      l_os << "  ${PREBATCH} " << a_chain_file.string () << "\n\n";
      l_os << "  endif\n\n";
      l_os << "  ${BATCH} " << a_chain_file.string () << ".pbs\n\n";
      l_os << "endif\n\n";
    }
  l_os.close ();
}
!!!144303.cpp!!!	replace_string(inout a_result : string, in a_search : string, in a_replace : string) : bool
// Local
bool Result = false;
const unsigned int l_index = a_result.find (a_search);
const unsigned int l_searchsize = a_search.size ();
// Do
if (l_index < a_result.size ())
{
  Result = true;
  // a_search found in template. If a_replace is not empty, replace
  if (not a_replace.empty ())
    {
      a_result.replace (l_index, l_searchsize, a_replace);
    }
  else
    {
      // a_replace is empty, remove string
      a_result.erase (l_index, l_searchsize);
    }
}
// End
return Result;
!!!144687.cpp!!!	make_filename(in a_chain : string, in a_code : string, in a_method : string, in a_basis : string, in a_algo : string) : string
// local
// - Set initial Result to template string
std::pair< bool, std::string > Result (config.get_string_attempt (config.make_key ("algorithm", "common", "filename")));
const std::string l_code ("${CODE}");
const std::string l_chain ("${CHAIN}");
const std::string l_method ("${METH}");
const std::string l_basis ("${BAS}");
const std::string l_algo ("${ALGO}");
CHECK (true == Result.first, "No filename template at algorithm/common/filename in configuration");
// Replace ${CODE} with a_code
replace_string (Result.second, l_code, a_code); 
// Replace ${CHAIN} with a_chain
replace_string (Result.second, l_chain, a_chain);
// Replace ${METH}
replace_string (Result.second, l_method, a_method);
// Replace ${BAS}
replace_string (Result.second, l_basis, a_basis);
// Replace ${METH}
replace_string (Result.second, l_algo, a_algo);
// Tidy up 
while(replace_string (Result.second, "--", "-")) { /* repeat until false */ }
while(replace_string (Result.second, "-.", ".")) { /* repeat until false */ }
while(replace_string (Result.second, ".-", ".")) { /* repeat until false */ }
while(replace_string (Result.second, "__", "_")) { /* repeat until false */ }
while(replace_string (Result.second, "_.", ".")) { /* repeat until false */ }
while(replace_string (Result.second, "._", ".")) { /* repeat until false */ }
while(replace_string (Result.second, "..", ".")) { /* repeat until false */ }

// End
return Result.second;
!!!144815.cpp!!!	user_verify_options() : bool
// Local
bool Result = 0;
std::ostringstream l_os;

// #ifdef DEBUG
// std::clog << "In input_file_factory::user_verify_option\n";
// #endif

// Construct a string with important options

// Method Options
if (state_object.is_oniom ())
{
  l_os << "Method = " << state_object.get_oniom ().get_string () << std::endl;
}
else
{
  l_os << "Method = " << state_object.get_method ().get_string () << std::endl;
  l_os << "Basis set = " << state_object.get_basis ().get_string () << std::endl;
};

// Transition state option
l_os << "Transition State = " << state_object.get_transitionstate ().get_string () << std::endl;

// Charge options
l_os << "Charge = " << state_object.get_charge ().get_string () << std::endl;

// Multiplicity options
l_os << "Multiplicity = " << state_object.get_multiplicity ().get_string () << std::endl;

// Machine options
l_os << "Machine = " << state_object.get_machine ().get_string () << std::endl;

// Code options
l_os << "Calculation Code = " << state_object.get_code_name ().get_string () << std::endl;

// Pageref Options
l_os << "Page Reference = " << state_object.get_pageref ().get_string () << std::endl;


// Query user
Result = state_object.warning_and_ask (l_os.str ()
				       , "Correct"
				       , "Cancel");
return Result;
!!!144943.cpp!!!	write_body(inout a_os : std::ostream, in a_algorithm : string) : void
// LOCAL
std::vector< std::string > l_part_list;
// DO

// This method writes out the gaussian input section of the script
// file.  The script is made by first getting a list of parts
// (l_name1).  Each part can have a "common" entry (l_name2) and/or an
// algorithm specific entry (l_name3).  These entries are written
// sequentially to disk.

//   Code options
a_os << "setenv ALGO " << a_algorithm << "\n\n";

// Get the job input sections.
config.get_stringlist (config.make_key ("algorithm", "options")
		       , l_part_list);

for (std::vector< std::string >::iterator l_cursor = l_part_list.begin ();
     l_cursor != l_part_list.end ();
     ++l_cursor)
{
  // Get the common part.
  std::pair< bool, std::string>
    l_value (config.get_string_attempt (config.make_key ("algorithm"
							 , "common"
							 , *l_cursor)));
  if (l_value.first)
    {
      //   Ouptut
      a_os << l_value.second << "\n";
    }
  // Get the algorithm part.
  l_value = config.get_string_attempt (config.make_key ("algorithm"
							, a_algorithm
							, *l_cursor));  
  if (l_value.first)
    {
      //   Ouptut
      a_os << l_value.second << "\n";
    }
}
!!!145071.cpp!!!	write_header(inout a_os : std::ostream) : void
// LOCAL
std::vector< std::string > l_machines;
const std::string l_default(state_object.get_machine ().get_string ());
// DO
// Standard start
a_os << "#!/bin/csh" << std::endl << std::endl;

// Standard section
write_section (a_os, "standard");
a_os << std::endl;
      
// Get the user section
write_options (a_os);
a_os << std::endl;
      
// Use the selected machine settings as the default
a_os << "# Default settings from machine == " << l_default << std::endl;
write_section (a_os, "machine", l_default);
a_os << std::endl << std::endl;
							 
// Get the machine sections
config.get_stringlist (config.make_key (state_object.get_machine ()
					, "valueStrings")
		       , l_machines);
for (std::vector< std::string >::iterator l_cursor = l_machines.begin ();
     l_cursor != l_machines.end ();
     ++l_cursor)
{
  // Test for existence of the 'domainname' resource setting.
  std::pair< bool, std::string >
    l_option_value(config.get_string_attempt (config.make_key ("machine"
							       , *l_cursor
							       , "domainname")));
  if (l_option_value.first)
    {
      // Write to script file
      a_os << "if ( \"`domainname`\" ==  \"" << l_option_value.second << "\" ) then\n";
    }
  else
    {
      a_os << "if ( \"`hostname`\" ==  \"" << *l_cursor << "\" ) then" << std::endl;
    }
  a_os << "echo Using configuration for machine = " << *l_cursor << std::endl;
  // Get this machines section
  write_section (a_os, "machine", *l_cursor);
  a_os << "endif" << std::endl << std::endl;
}
!!!145199.cpp!!!	write_options(inout a_os : std::ostream) : void
// DO
// Method Options
if (state_object.is_oniom ()) 
{
  a_os << "setenv METHOD \"ONIOM(" << state_object.get_oniom ().get_string () << ")\"\n";
  a_os << "setenv BASIS\n";
  a_os << "setenv XBASIS\n";
}
else
{
  std::vector< std::string > l_xbasis;
  config.get_stringlist (config.make_key (state_object.get_basis ()
					  , "xbasis")
			 , l_xbasis);
  // Do option lookup
  a_os << "setenv METHOD " << state_object.get_method ().get_string () << "\n";
  a_os << "setenv BASIS \"" << state_object.get_basis ().get_string () << "\"\n";
  ALWAYS(static_cast< int >(l_xbasis.size ()) >  state_object.get_basis ().get_index (), "Extended Basis set resource has too few entries");
  a_os << "setenv XBASIS \"" << l_xbasis.at(state_object.get_basis ().get_index ()) << "\"\n";
}

// Transition state option
a_os << "setenv OPTIONS " << state_object.get_transitionstate ().get_string () << "\n";
// Charge options
a_os << "setenv CHARGE " << state_object.get_charge ().get_string () << "\n";
// Multiplicity options
a_os << "setenv MULTIPLICITY " << state_object.get_multiplicity ().get_string () << "\n";
//   Code options
a_os << "setenv CODE " << state_object.get_code_name ().get_string () << "\n";
//   Pageref Options
a_os << "setenv PAGEREF " << state_object.get_pageref ().get_string () << "\n";
//  Extra Options
a_os << "setenv EXTRA " << state_object.get_extra ().get_string () << "\n";
!!!145327.cpp!!!	write_section(inout a_os : std::ostream, in a_section : string, in a_subsection : string) : void
// LOCAL
std::vector< std::string > l_option_set;

// DO
// Test is desired value exists.
config.get_stringlist (config.make_key (a_section, "options")
		       , l_option_set);

if (not l_option_set.empty ())
{
  for (std::vector< std::string >::iterator l_cursor = l_option_set.begin ();
       l_cursor != l_option_set.end ();
       ++l_cursor)
    {
      std::string l_option_name (*l_cursor);
      std::pair< bool, std::string > l_option_value;
      // Create an item name
      boost::to_lower (l_option_name);
      // Ensure item
      l_option_value = config.get_string_attempt (config.make_key (a_section
								   , a_subsection
								   , l_option_name));
      if (l_option_value.first)
	{
	      // Write to script file
	      a_os << "setenv " << *l_cursor << " " << l_option_value.second << "\n";
        }
    }
}
