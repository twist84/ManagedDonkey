#include "interface/c_gui_text_widget.hpp"

#include "interface/c_gui_screen_widget.hpp"

c_user_interface_text* __cdecl c_gui_text_widget::get_text_internal()
{
	return DECLFUNC(__vftable->__funcs[37], c_user_interface_text*, __thiscall, c_gui_text_widget*)(this);
}

dword __cdecl c_gui_text_widget::get_text_buffer_size() const
{
	return DECLFUNC(__vftable->__funcs[38], dword, __thiscall, c_gui_text_widget const*)(this);
}

void __cdecl c_gui_text_widget::set_text(wchar_t const* text)
{
	DECLFUNC(__vftable->__funcs[40], void, __thiscall, c_gui_text_widget*, wchar_t const*)(this, text);
}

void __cdecl c_gui_text_widget::set_text_from_string_id(c_gui_screen_widget* screen, long id)
{
	DECLFUNC(__vftable->__funcs[41], void, __thiscall, c_gui_text_widget*, c_gui_screen_widget*, long)(this, screen, id);
}

wchar_t const* __cdecl c_gui_text_widget::get_text()
{
	return DECLFUNC(__vftable->__funcs[42], wchar_t const*, __thiscall, c_gui_text_widget*)(this);
}

