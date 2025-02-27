#include "interface/c_gui_text_widget.hpp"

#include "interface/c_gui_screen_widget.hpp"

c_user_interface_text* c_gui_text_widget::get_text_internal()
{
	return INVOKE_CLASS_MEMBER((dword)__vftable->__funcs[37], c_gui_text_widget, get_text_internal);
}

dword c_gui_text_widget::get_text_buffer_size() const
{
	return INVOKE_CLASS_MEMBER((dword)__vftable->__funcs[38], c_gui_text_widget, get_text_buffer_size);
}

void c_gui_text_widget::set_text(wchar_t const* text)
{
	INVOKE_CLASS_MEMBER((dword)__vftable->__funcs[40], c_gui_text_widget, set_text, text);
}

void c_gui_text_widget::set_text_from_string_id(c_gui_screen_widget* screen, long id)
{
	INVOKE_CLASS_MEMBER((dword)__vftable->__funcs[41], c_gui_text_widget, set_text_from_string_id, screen, id);
}

wchar_t const* c_gui_text_widget::get_text()
{
	return INVOKE_CLASS_MEMBER((dword)__vftable->__funcs[42], c_gui_text_widget, get_text);
}

