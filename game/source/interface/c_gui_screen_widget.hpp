#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "tag_files/tag_groups.hpp"

class c_dialog_result_message;
class c_game_tag_parser;
class c_gui_bitmap_widget;
class c_gui_data;
class c_message;
struct s_emblem_info;

enum e_screen_widget_definition_flags
{
	_screen_widget_definition_flag_b_back_shouldnt_dispose_screen_bit = k_core_widget_definition_flags,
	_screen_widget_definition_flag_overlays_other_screens_bit,

	k_screen_widget_definition_flags
};

struct s_screen_widget_definition :
	s_core_widget_definition
{
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> string_list_tag_reference;
	c_typed_tag_reference<GUI_SCREEN_WIDGET_DEFINITION_TAG, INVALID_TAG> template_reference;

	string_id initial_button_key_name;

	s_tag_block datasource_blocks;
	s_tag_block group_blocks;
	s_tag_block button_key_blocks;

	// Sound Overrides
	// To override global sound effects for this particular screen, specify them here
	s_tag_reference sound_override_reference;

	// On Load Command Script
	// To run a main menu scenario command script when this screen loads, specifiy script name here
	// NOTE: must be of type 'command script' and will not interrupt any currently running script of the same name
	char script_name[32];

	int16 script_index;
	int16 pad;
};
COMPILE_ASSERT(sizeof(s_screen_widget_definition) == sizeof(s_core_widget_definition) + 0x7C);

struct s_runtime_screen_widget_definition :
	s_runtime_core_widget_definition
{
	s_runtime_screen_widget_definition();

	int32 string_list_tag_reference_index;
	int32 initial_button_key_name;
	s_tag_block datasource_blocks;
	s_tag_block group_blocks;
	s_tag_block button_key_blocks;
	int32 sound_override_reference_index;
	char script_name[32];
	int16 script_index;
	int16 pad;
};
COMPILE_ASSERT(sizeof(s_runtime_screen_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x54);

struct s_integer_data_nugget
{
	int32 name;
	int32 value;
};
COMPILE_ASSERT(sizeof(s_integer_data_nugget) == 0x8);

struct s_string_data_nugget
{
	int32 name;
	char string[32];
};
COMPILE_ASSERT(sizeof(s_string_data_nugget) == 0x24);

struct s_string_id_data_nugget
{
	int32 name;
	int32 value;
};
COMPILE_ASSERT(sizeof(s_string_id_data_nugget) == 0x8);

struct s_datasource_block
{
	c_typed_tag_block<s_integer_data_nugget> integer_nuggets;
	c_typed_tag_block<s_string_data_nugget> text_nuggets;
	c_typed_tag_block<s_string_id_data_nugget> string_id_nuggets;
	int32 submenu_control_name;
};
COMPILE_ASSERT(sizeof(s_datasource_block) == 0x28);

struct s_datasource_definition
{
	int32 name;
	int32 pad[3];
	c_typed_tag_block<s_datasource_block> elements;
};
COMPILE_ASSERT(sizeof(s_datasource_definition) == 0x1C);

struct s_datasource_definition_block
{
	s_tag_reference datasource_tag_reference;
};
COMPILE_ASSERT(sizeof(s_datasource_definition_block) == sizeof(s_tag_reference));

struct s_depth_sorted_render_widget
{
	e_gui_widget_type type;
	int32 render_data_offset;
	real32 depth;
	int32 depth_bias;
};
COMPILE_ASSERT(sizeof(s_depth_sorted_render_widget) == 0x10);

struct s_window_manager_static_render_data
{
	enum
	{
		k_maximum_rendered_child_widgets_per_screen = 384
	};

	byte render_data_buffer[0x14000];
	int32 render_data_buffer_count;
	s_depth_sorted_render_widget render_list[k_maximum_rendered_child_widgets_per_screen];
	int32 current_count;
};
COMPILE_ASSERT(sizeof(s_window_manager_static_render_data) == 0x15808);

struct s_window_manager_screen_render_data
{
	enum
	{
		k_maximum_rendered_child_widgets_per_screen = 384
	};

	s_window_manager_screen_render_data();
	void clear();
	void dispose();
	bool initialize(int32 buffer_size);
	bool valid() const;

	char* render_data_buffer;
	int32 render_data_buffer_length;
	int32 render_data_buffer_count;
	s_depth_sorted_render_widget render_list[k_maximum_rendered_child_widgets_per_screen];
	int32 current_count;
	rectangle2d built_for_viewport_bounds;
};
COMPILE_ASSERT(sizeof(s_window_manager_screen_render_data) == 0x1818);

class c_gui_screen_widget :
	public c_gui_widget
{
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

public:
	virtual ~c_gui_screen_widget() = default;
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual void dispose() override;
	virtual s_runtime_core_widget_definition* get_core_definition() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual bool get_string_by_string_id(int32 string_identifier, c_static_wchar_string<1024>* buffer) override;

protected:
	virtual e_render_data_size get_render_data_size();

public:
	virtual e_gui_location get_gui_location() const;
	virtual void reconstruct();
	virtual void initialize_datasource();
	virtual void reload_assets();
	virtual void reload_next_frame();
	virtual void predict_immediate_resources(int32 a1);
	virtual void predict_pending_resources(int32 a1);
	virtual bool can_receive_focus() const;
	virtual void update_render();
	virtual bool process_message(const c_message* message);
	virtual bool handle_focused_widget_selected(const c_controller_input_message* message, c_gui_widget* widget);
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	virtual bool handle_dialog_result(const c_dialog_result_message* dialog_result_message);
	virtual void submenu_invoked(c_gui_list_widget* submenu_widget);
	virtual void transition_out_with_transition_type(e_transition_out_type transition_out, e_screen_transition_type transition_type);
	virtual bool __funcs53();
	virtual bool try_and_get_render_data_emblem_info(c_gui_bitmap_widget* bitmap_widget, s_emblem_info* emblem_info);

public:
	c_gui_screen_widget(int32 name);

protected:
	void add_datasource(c_gui_data* datasource);
	void add_definition_fields(const s_screen_widget_definition* definition, bool was_templated);

public:
	void add_game_tag_parser(c_game_tag_parser* parser);
	bool button_key_to_controller_component(c_gui_text_widget* widget, e_controller_component* component);

private:
	void clear_display_groups();
	void create_and_add_child_widgets_from_definition(s_runtime_screen_widget_definition* definition);

public:
	void dispose_child_screens();

private:
	void dispose_datasource();

protected:
	c_gui_list_item_widget* find_list_item(int32 list_name, int32 list_column, int32 data_to_find);

public:
	c_gui_group_widget* get_button_key_widget();
	int32 get_constants_datasource_integer(int32 constant_name);
	c_gui_data* get_data(int32 name, int32* datasource_index);

protected:
	int32 get_datasource_count() const;

public:
	c_gui_widget* get_focused_widget();
	int32 get_multilingual_unicode_string_list_index();
	const char* get_multilingual_unicode_string_list_tag_name();
	s_window_manager_screen_render_data* get_render_state();
	e_window_index get_render_window();
	int32 get_screen_index() const;

private:
	bool handle_controller_alt_stick_attempt(const c_controller_input_message* message);
	bool handle_controller_alt_tab_attempt(const c_controller_input_message* message);
	bool handle_controller_selection_attempt(const c_controller_input_message* message);
	bool handle_controller_tab_attempt(const c_controller_input_message* message);
	bool handle_mouse_click(const c_controller_input_message* message, c_gui_widget* widget);

public:
	void load_display_group_by_name(e_display_group_type group_type, int32 group_name);
	void play_sound(e_user_interface_sound_effect sound_effect);

protected:
	void reposition_for_splitscreen();

public:
	bool running_in_codeless_mode();

private:
	void set_initial_focused_widget();

public:
	void set_responds_to_controller_events(bool value);
	void set_text_widget_string_id(int32 widget_name, int32 widget_text);
	void set_text_widget_string_id_from_focused_list_item(int32 text_widget_name, int32 list_name, int32 column_name);
	void set_text_widget_text(int32 widget_name, const wchar_t* widget_text);
	void transfer_focus(c_gui_widget* new_focused_widget);
	void transfer_focus_to_list(c_gui_list_widget* list_widget, int32 element_handle, bool play_received_animation, bool play_lost_animation);
	void transfer_focus_without_animations(c_gui_widget* new_focused_widget, bool play_received_animation, bool play_lost_animation);
	void transition_out(e_transition_out_type transition_out);

public:
//protected:
	int32 m_screen_index;
	uns32 m_creation_time_milliseconds;
	uns32 m_disposal_time_milliseconds;
	uns32 m_last_focus_change_time_milliseconds;
	c_gui_widget* m_current_focused_widget;
	bool m_suppress_focus;
	bool m_render_in_screenshot;
	int32 m_current_display_group_widgets[k_display_group_type_count];
	int32 m_previous_display_group_widgets[k_display_group_type_count];
	int32 m_current_display_group_indicies[k_display_group_type_count];
	bool m_reload_next_frame;
	bool m_responds_to_controller_events;
	int32 m_initial_focused_widget;
	int32 m_initial_focused_widget_element_handle;
	int32 m_initial_focused_widget_column_name;
	int32 m_initial_focused_widget_column_value;
	s_runtime_screen_widget_definition m_definition;
	c_gui_data* m_datasource[32];
	int32 m_datasource_count;
	c_game_tag_parser* m_game_tag_parsers[k_maximum_number_of_game_tag_parsers];
	int32 m_game_tag_parser_count;
	s_window_manager_screen_render_data m_render_data;
	bool m_running_in_codeless_mode;
};
COMPILE_ASSERT(sizeof(c_gui_screen_widget) == 0x1AA0);
COMPILE_ASSERT(sizeof(c_gui_screen_widget) == sizeof(c_gui_widget) + 0x19C0);

