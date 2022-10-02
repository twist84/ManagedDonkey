#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_data.hpp"

struct s_runtime_core_widget_definition
{
	long __unknown0;
	c_string_id name;
	byte __data8[0x2C];
};
static_assert(sizeof(s_runtime_core_widget_definition) == 0x34);

struct s_runtime_screen_widget_definition : s_runtime_core_widget_definition
{
	byte __data34[0x4];
	c_string_id initial_button_key_name;
	byte __data3C[0x4C];
};
static_assert(sizeof(s_runtime_screen_widget_definition) == 0x88);

struct s_window_manager_screen_render_data
{
	byte __data[0x1818];
};
static_assert(sizeof(s_window_manager_screen_render_data) == 0x1818);

struct c_game_tag_parser;
struct c_gui_screen_widget : c_gui_widget
{
protected:
	long __unknownDC;
	long m_screen_index;
	dword __timeE4;
	long __unknownE8;
	long __unknownEC;
	c_gui_widget* m_focused_widget;
	bool m_suppress_focus;
	long m_display_groups[2][3];
	bool m_can_receive_focus;
	bool m_responds_to_controller_events;
	byte __data112[2];
	byte __data114[0x10];
	s_runtime_screen_widget_definition m_core_definition;
	c_gui_data* m_datasource[32];
	long m_datasource_count;
	c_game_tag_parser* m_game_tag_parser[20];
	long m_game_tag_parser_count;
	s_window_manager_screen_render_data m_screen_render_data;
	bool m_running_in_codeless_mode;
};
static_assert(sizeof(c_gui_screen_widget) == 0x1AA0);