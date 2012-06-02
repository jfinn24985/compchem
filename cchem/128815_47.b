class widget_control
!!!138031.cpp!!!	add_widget(in a_widget : Widget) : void
// LOCAL
Widget* l_new;
// PRE
REQUIRE(allow_widget_type (widget_type (a_widget)), "Cannot accept widget of this type");
// DO
// Create new list.
l_new = new Widget[widget_count + 1];
// Copy old list
for (int l_index = 0; l_index < widget_count; ++l_index)
{
  l_new[l_index] = widget_list[l_index];
}
// Delete old list
if (NULL != widget_list)
{
  delete [] widget_list;
}
// Set new list.
widget_list = l_new;
// Add new widget
widget_list[widget_count] = a_widget;
++widget_count;
!!!138287.cpp!!!	bind_application(inout a_application : gui_state) : void
a_application.bind_control (*this);
