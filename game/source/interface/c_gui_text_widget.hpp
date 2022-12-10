#pragma once

#include "interface/c_gui_widget.hpp"

struct s_runtime_text_widget_definition : s_runtime_core_widget_definition
{
	byte __data[0x20];
};
static_assert(sizeof(s_runtime_text_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x20);

struct c_gui_text_widget : c_gui_widget
{
	struct s_text_source_data
	{
		long type;
		string_id id;
		long __unknown8;
	};
	static_assert(sizeof(s_text_source_data) + 0xC);

	byte __dataDC[0x4];
	s_text_source_data text_source_data;
	s_runtime_text_widget_definition m_core_definition;
};
static_assert(sizeof(c_gui_text_widget) == sizeof(c_gui_widget) + 0x64);

