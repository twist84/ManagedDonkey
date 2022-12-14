#pragma once

#include "interface/c_gui_widget.hpp"

struct s_text_widget_definition : s_core_widget_definition
{
	string_id value_override_list;
	string_id value_identifier;
	string_id text_color_preset;
	short_enum custom_font;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_text_widget_definition) == sizeof(s_core_widget_definition) + 0x10);

struct s_runtime_text_widget_definition : s_runtime_core_widget_definition
{
	string_id value_override_list;
	string_id value_identifier;
	real_argb_color text_color_preset;
	long custom_font;
};
static_assert(sizeof(s_runtime_text_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x1C);

struct c_gui_text_widget : public c_gui_widget
{
	struct s_text_source_data
	{
		long type;
		string_id id;
		long __unknown8;
	};
	static_assert(sizeof(s_text_source_data) == 0xC);

public:

	void __cdecl set_text(wchar_t const* text);

protected:
	long __unknownDC;
	s_text_source_data text_source_data;
	s_runtime_text_widget_definition m_core_definition;
	long __unknown13C;
};
static_assert(sizeof(c_gui_text_widget) == sizeof(c_gui_widget) + 0x64);

template<long k_maximum_count>
struct c_sized_user_interface_text : c_user_interface_text
{
	c_static_wchar_string<k_maximum_count> m_string1;
	c_static_wchar_string<k_maximum_count> m_string2;
	dword m_unknown3;
};
static_assert(sizeof(c_sized_user_interface_text<48>) == 0x120);
static_assert(sizeof(c_sized_user_interface_text<256>) == 0x460);
static_assert(sizeof(c_sized_user_interface_text<1024>) == 0x1060);

