#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "tag_files/tag_groups.hpp"

enum e_screen_widget_definition_flags
{
	_screen_widget_definition_flag_b_back_shouldnt_dispose_screen_bit = k_core_widget_definition_flag_count,
	_screen_widget_definition_flag_overlays_other_screens_bit,

	k_screen_widget_definition_flag_count
};

struct s_screen_widget_definition : s_core_widget_definition
{
	c_typed_tag_reference<'unic'> string_list;
	c_typed_tag_reference<'scn3'> screen_template;

	c_string_id initial_button_key_name;

	tag_block debug_datasources;
	tag_block groups;
	tag_block button_keys;

	// Sound Overrides
	// To override global sound effects for this particular screen, specify them here
	tag_reference sound_overrides;

	// On Load Command Script
	// To run a main menu scenario command script when this screen loads, specifiy script name here
	// NOTE: must be of type 'command script' and will not interrupt any currently running script of the same name
	char on_load_script_name[32];

	short script_index;
	byte scary[2];
};
static_assert(sizeof(s_screen_widget_definition) == 0xA8);

struct s_runtime_screen_widget_definition : s_runtime_core_widget_definition
{
	long string_list_index;
	c_string_id initial_button_key_name;
	tag_block debug_datasources;
	tag_block groups;
	tag_block button_keys;
	long sound_overrides_index;
	string on_load_script_name;
	short script_index;
	byte scary[2];
};
static_assert(sizeof(s_runtime_screen_widget_definition) == 0x88);

struct s_window_manager_screen_render_data
{
	struct s_render_list
	{
		long type;
		long render_data_offset;
		real rendered_depth;
		long render_depth_bias;
	};
	static_assert(sizeof(s_render_list) == 0x10);

	char* render_data_buffer;
	long __unknown4;
	long render_data_buffer_count;
	s_render_list render_list[384];
	long current_count;
	short_rectangle2d window_bounds;
};
static_assert(sizeof(s_window_manager_screen_render_data) == 0x1818);

struct c_game_tag_parser;
struct c_gui_screen_widget : c_gui_widget
{
public:
	c_gui_data* get_data(long name, long* datasource_index);

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
	byte __unknown112;
	byte __unknown113;
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