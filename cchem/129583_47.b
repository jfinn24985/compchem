class publisher
!!!136367.cpp!!!	bind_library(inout a_application : gui_state) : void
// LOCAL
XmtSymbol l_symbol;
// DO
// Register a symbol
LOG("Creating symbol " + get_name ());
XmtVaRegisterSymbols(const_cast< String >(get_name ().c_str ())
		     , XtRInt
		     , sizeof(int)
		     , &index
		     , NULL);
l_symbol = XmtLookupSymbol(const_cast< String >(get_name ().c_str ()));
CHECK(NULL != l_symbol, "Could not find symbol " + get_name () = " just after registering it!");
XmtSymbolAddCallback(l_symbol
		     , &publisher_callback
                     , static_cast< XtPointer >(this));
!!!136495.cpp!!!	is_initialised() : bool
return (NULL != XmtLookupSymbol (const_cast< String > (get_name ().c_str ())));
