class text_control
!!!137519.cpp!!!	get_string() : string
// LOCAL
std::string Result;
// DO
// 
if (XtIsSubclass(m_widget, xmtInputFieldWidgetClass))
{
  char *l_field = XmtInputFieldGetString (m_widget);
  if (NULL != l_field)
    {
      Result = l_field;
    }
}
else if (XtIsSubclass(m_widget, xmTextWidgetClass))
{
  char *l_field = XmTextGetString (m_widget);
  if (NULL != l_field)
    {
      Result = l_field;
      XtFree (l_field);
    }
}
return Result;
!!!137647.cpp!!!	set_string(in a_value : string) : void
// LOCAL
// DO
if (XtIsSubclass(m_widget, xmtInputFieldWidgetClass))
{
  XmtInputFieldSetString (m_widget, const_cast< char* >(a_value.c_str()));
}
else if (XtIsSubclass(m_widget, xmTextWidgetClass))
{
  XmTextSetString (m_widget, const_cast< char* >(a_value.c_str()));
}
