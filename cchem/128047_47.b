class configuration
!!!129199.cpp!!!	get_boolean_attempt(in a_resource : configuration::key) : std::pair< bool, bool >
// LOCAL
std::pair< bool, std::string > l_value (get_string_attempt (a_resource));
std::pair< bool, bool > Result;
// DO
Result.first = l_value.first;
if (Result.first)
{
  // Convert to lower case.
  boost::to_lower (l_value.second);
  if ("true" == l_value.second)
    {
      Result.second = true;
    }
  else if ("false" == l_value.second)
    {
      Result.second = false;
    }
  else
    {
      try
	{
	  Result.second = boost::lexical_cast< bool, std::string > (l_value.second);
	}
      catch (const boost::bad_lexical_cast &err)
	{
	  LOG(err.what());
	  // See if it is "true" or "false"
	  Result.first = false;
	}
    }
}
return Result;
!!!129711.cpp!!!	get_string_attempt(in a_resource : configuration::key) : std::pair< bool , std::string  >
// LOCAL
XrmValue l_value; /* { int size, void* addr } */
std::pair< bool, std::string > Result;
char* l_type;
// DO
REQUIRE(NULL != state.get_top_level (), "Non-null widget is required to access resource database");
REQUIRE(false == a_resource.get_value ().empty(), "A non-empty resource string is required to access resource database");
REQUIRE(false == state.get_class ().empty(), "A non-empty X11 class name string is required to access resource database");
Result.first = (True == XrmGetResource (XrmGetDatabase (XtDisplay(state.get_top_level ()))
			 , const_cast<String>(a_resource.get_value ().c_str ())
			 , const_cast<String>(state.get_class ().c_str ())
			 , &l_type
			 , &l_value));
if (Result.first && NULL != l_value.addr && NULL != l_type)
{
  CHECK(0 == strcmp (l_type, XtRString), "Got configuration type " + std::string(l_type) + " but wanted a string");
  if (0 == strcmp (l_type, XtRString))
    {
      Result.second = (char*)l_value.addr;
      LOG("Got configuration [" + a_resource.get_value () + "] = [" + Result.second + "]");
    }
  else
    {
      LOG("Got configuration type " + std::string(l_type) + " but wanted a string");
      Result.first = false;
    }
}
else
{
  LOG("Failed to get configuration [" + a_resource.get_value () + "]");
  Result.first = false;
}
return Result;
!!!129967.cpp!!!	get_stringlist_attempt(in a_resource : configuration::key, inout a_list : configuration::stringlist_type) : bool
// LOCAL
std::pair< bool, std::string > l_list (get_string_attempt (a_resource));
// DO
a_list.clear ();
if (l_list.first && false == l_list.second.empty ())
{
  // Use boost tokenizer to split up string
  boost::tokenizer < boost::escaped_list_separator< char > > l_tok(l_list.second);
  for (boost::tokenizer < boost::escaped_list_separator< char > >::iterator l_cursor = l_tok.begin();
	l_cursor != l_tok.end();
	++l_cursor)
    {
        // Remove leading and trailing spaces. The problem is that for the
        // string ["a", "b"] we get tokens [a] and [ b]. It is hard to know if
	// this is a "bug" or a "feature". 
        const std::string::size_type l_start (l_cursor->find_first_not_of (" "));
	const std::string::size_type l_end (l_cursor->find_last_not_of (" "));
        if (l_start < l_cursor->size ())
	  {
	    a_list.push_back (l_cursor->substr (l_start, l_end - l_start + 1));
	  }
    }  
}
return l_list.first;
!!!130095.cpp!!!	get_other_attempt(in a_resource : configuration::key) : std::pair< bool , Type  >
// LOCAL
std::pair< bool, std::string > l_value (get_string_attempt (a_resource));
std::pair< bool, Type > Result;
// DO
Result.first = l_value.first;
if (Result.first)
{
  try
    {
	Result.second = boost::lexical_cast< Type, std::string > (l_value.second);
    }
  catch (const boost::bad_lexical_cast &err)
    {
	LOG("Conversion failed:" + std::string (err.what()));
	Result.first = false;
   }
}
return Result;
