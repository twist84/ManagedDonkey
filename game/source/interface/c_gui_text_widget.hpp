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

	long __unknownDC;
	s_text_source_data text_source_data;
	s_runtime_text_widget_definition m_core_definition;
	long __unknown13C;
};
static_assert(sizeof(c_gui_text_widget) == sizeof(c_gui_widget) + 0x64);

