#include "interface/user_interface_utilities.hpp"

#include "interface/c_gui_widget.hpp"
#include "memory/module.hpp"

#include <stdio.h>

HOOK_DECLARE(0x00AA4C80, tint_widget_to_change_color);

// hook to test for `c_gui_widget` struct correctness
void __cdecl tint_widget_to_change_color(c_gui_widget* widget, long a2, bool a3)
{
	char const* names[13]{};

	HOOK_INVOKE(, tint_widget_to_change_color, widget, a2, a3);

	names[0] = widget->m_name.get_string();

	if (widget->m_parent.__unknown0)    names[1] = widget->m_parent.__unknown0->m_name.get_string();
	if (widget->m_parent.__unknown4)    names[2] = widget->m_parent.__unknown4->m_name.get_string();
	if (widget->m_parent.__unknown8)    names[3] = widget->m_parent.__unknown8->m_name.get_string();

	if (widget->m_children.__unknown0)  names[4] = widget->m_children.__unknown0->m_name.get_string();
	if (widget->m_children.__unknown4)  names[5] = widget->m_children.__unknown4->m_name.get_string();
	if (widget->m_children.__unknown8)  names[6] = widget->m_children.__unknown8->m_name.get_string();

	if (widget->m_next.__unknown0)      names[7] = widget->m_next.__unknown0->m_name.get_string();
	if (widget->m_next.__unknown4)      names[8] = widget->m_next.__unknown4->m_name.get_string();
	if (widget->m_next.__unknown8)      names[9] = widget->m_next.__unknown8->m_name.get_string();

	if (widget->m_previous.__unknown0)  names[10] = widget->m_previous.__unknown0->m_name.get_string();
	if (widget->m_previous.__unknown4)  names[11] = widget->m_previous.__unknown4->m_name.get_string();
	if (widget->m_previous.__unknown8)  names[12] = widget->m_previous.__unknown8->m_name.get_string();

	printf("");
}
