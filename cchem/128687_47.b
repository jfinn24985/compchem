class list_control
!!!131759.cpp!!!	bind_application(inout a_application : ) : void
a_application.bind_control (*this);
if (is_mask())
{
  configuration l_config (a_application);
  l_config.get_stringlist (l_config.make_key (*this, "valueStrings")
			   , *m_string_list);
}
!!!131887.cpp!!!	do_allow_widget(in a_widget : Widget) : bool
// LOCAL
bool Result(false);
// DO
if (True == XtIsSubclass(a_widget, xmtChooserWidgetClass))
{
  XmtChooserType l_type;
  XtVaGetValues (a_widget, XmtNchooserType, &l_type, NULL);
  // XmtChooserButtonBox is an illegal value for list_control
  Result = (l_type != XmtChooserButtonBox);
}
return Result;
!!!132015.cpp!!!	do_add_widget(in a_widget : Widget) : void
// LOCAL
XmtChooserType l_type;
// DO
m_widget = a_widget;
XtVaGetValues (m_widget, XmtNchooserType, &l_type, NULL);

// Is 'false' when the XmtNchooserType resource is set to one of the
// ``single selection'' types: XmtChooserRadioBox,
// XmtChooserRadioPalette, XmtChooserRadioList, or
// XmtChooserOption. It is 'true' for XmtChooserCheckBox,
// XmtChooserCheckPalette, or XmtChooserCheckList types.
switch (l_type)
{
 case XmtChooserRadioBox:
 case XmtChooserRadioPalette:
 case XmtChooserRadioList:
 case XmtChooserOption:
   break;
 case XmtChooserCheckBox:
 case XmtChooserCheckPalette:
 case XmtChooserCheckList:
   m_string_list = new std::vector< std::string >;
   break;
 case XmtChooserButtonBox: // Illegal value for list_control
 default:
   // We should NEVER get here
   ALWAYS (false, "Illegal XmtChooser type for list_control object");
   break;
}
!!!132911.cpp!!!	get_stringlist(inout a_list : std::vector< std::string >) : void
// LOCAL
int l_index(get_index());
// PRE
REQUIRE(true == is_mask(), "Can not get list of strings for single selection lists");
REQUIRE(false == m_string_list->empty(), "List of strings is empty");
// DO
// clear string set.
a_list.clear();
// Scan index for selected items.
for (unsigned int l_bit = 0;
     l_bit < m_string_list->size();
     ++l_bit)
{
  if (0 != (l_index & (1 << l_bit)))
    {
      a_list.push_back (std::string (m_string_list->at(l_bit)));
    }
}

