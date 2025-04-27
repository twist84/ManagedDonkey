#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "tag_files/tag_groups.hpp"

enum e_screen_widget_definition_flags
{
	_screen_widget_definition_flag_b_back_shouldnt_dispose_screen_bit = k_core_widget_definition_flags,
	_screen_widget_definition_flag_overlays_other_screens_bit,

	k_screen_widget_definition_flags
};

struct s_screen_widget_definition :
	s_core_widget_definition
{
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> string_list;
	c_typed_tag_reference<GUI_SCREEN_WIDGET_DEFINITION_TAG, INVALID_TAG> screen_template;

	c_string_id initial_button_key_name;

	s_tag_block debug_datasources;
	s_tag_block groups;
	s_tag_block button_keys;

	// Sound Overrides
	// To override global sound effects for this particular screen, specify them here
	s_tag_reference sound_overrides;

	// On Load Command Script
	// To run a main menu scenario command script when this screen loads, specifiy script name here
	// NOTE: must be of type 'command script' and will not interrupt any currently running script of the same name
	char on_load_script_name[32];

	short script_index;
	uint8 scary[2];
};
static_assert(sizeof(s_screen_widget_definition) == sizeof(s_core_widget_definition) + 0x7C);

struct s_runtime_screen_widget_definition :
	s_runtime_core_widget_definition
{
	long string_list_index;
	c_string_id initial_button_key_name;
	s_tag_block debug_datasources;
	s_tag_block groups;
	s_tag_block button_keys;
	long sound_overrides_index;
	c_static_string<k_tag_string_length> on_load_script_name;
	short script_index;
	uint8 scary[2];
};
static_assert(sizeof(s_runtime_screen_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x54);

struct s_depth_sorted_render_widget
{
	e_gui_widget_type type;
	long render_data_offset;
	real32 depth;
	long depth_bias;
};
static_assert(sizeof(s_depth_sorted_render_widget) == 0x10);

struct s_window_manager_static_render_data
{
	enum
	{
		k_maximum_rendered_child_widgets_per_screen = 384
	};

	char render_data_buffer[0x14000];
	long render_data_buffer_count;
	s_depth_sorted_render_widget render_list[k_maximum_rendered_child_widgets_per_screen];
	long current_count;
};
static_assert(sizeof(s_window_manager_static_render_data) == 0x15808);

struct s_window_manager_screen_render_data
{
	enum
	{
		k_maximum_rendered_child_widgets_per_screen = 384
	};

	char* render_data_buffer;
	long render_data_buffer_length;
	long render_data_buffer_count;
	s_depth_sorted_render_widget render_list[k_maximum_rendered_child_widgets_per_screen];
	long current_count;
	rectangle2d built_for_viewport_bounds;
};
static_assert(sizeof(s_window_manager_screen_render_data) == 0x1818);

enum e_gui_game_mode;
enum e_user_interface_sound_effect;
enum e_window_index;
struct c_gui_data;
struct c_game_tag_parser;
struct c_gui_screen_widget :
	public c_gui_widget
{
public:
	c_gui_screen_widget(long name)
	{
		DECLFUNC(0x00AB02B0, c_gui_screen_widget*, __thiscall, c_gui_screen_widget*, long)(this, name);
	}

	//virtual ~c_gui_screen_widget();
	//virtual void initialize();
	//virtual void post_initialize();
	//virtual void dispose();
	//virtual void update(uint32 current_milliseconds);

	// get_render_data_size
	// get_gui_location
	// reconstruct
	// initialize_datasource
	// reload_assets
	// reload_next_frame
	// predict_immediate_resources? this could be bogus
	// predict_pending_resources? this could be bogus
	// can_receive_focus
	// update_render
	// process_message
	// handle_focused_widget_selected
	// handle_list_item_chosen
	// handle_dialog_result
	// submenu_invoked
	// transition_out_with_transition_type
	// __funcs53, function between `initialize_datasource` and `load_display_group` this means the name starts with i, j, k, or l
	// try_and_get_render_data_emblem_info

protected:
	void add_datasource(c_gui_data* datasource);

public:
	void add_game_tag_parser(c_game_tag_parser* parser);
	c_gui_data* get_data(long name, long* datasource_index);
	c_gui_widget* get_focused_widget();
	e_gui_game_mode get_gui_game_mode();
	e_window_index get_render_window();
	bool handle_list_item_chosen(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	void play_sound(e_user_interface_sound_effect sound_effect);
	bool running_in_codeless_mode();
	void transfer_focus(c_gui_widget* widget);
	void transfer_focus_to_list(c_gui_list_widget* list_widget, long element_handle, bool play_received_animation, bool play_lost_animation);

public:
	enum
	{
		k_maximum_number_of_game_tag_parsers = 20,
	};

	enum e_display_group_type
	{
		_display_group_type_default = 0,
		_display_group_type_button_key,

		k_display_group_type_count
	};

	long __unknownDC;
	long m_screen_index;
	uint32 m_creation_time_milliseconds;
	uint32 m_disposal_time_milliseconds;
	uint32 m_last_focus_change_time_milliseconds;
	c_gui_widget* m_current_focused_widget;
	bool m_suppress_focus;
	bool m_render_in_screenshot;
	long m_current_display_group_widgets[k_display_group_type_count];
	long m_previous_display_group_widgets[k_display_group_type_count];
	long m_current_display_group_indicies[k_display_group_type_count];
	bool m_reload_next_frame;
	bool m_responds_to_controller_events;
	long m_initial_focused_widget;
	long m_initial_focused_widget_element_handle;
	long m_initial_focused_widget_column_name;
	long m_initial_focused_widget_column_value;
	s_runtime_screen_widget_definition m_definition;
	c_gui_data* m_datasource[32];
	long m_datasource_count;
	c_game_tag_parser* m_game_tag_parsers[k_maximum_number_of_game_tag_parsers];
	long m_game_tag_parser_count;
	s_window_manager_screen_render_data m_render_data;
	bool m_running_in_codeless_mode;
};
static_assert(sizeof(c_gui_screen_widget) == 0x1AA0);