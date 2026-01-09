#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

struct s_data_array;
struct s_event_record;
struct s_window_manager_screen_render_data;
struct s_window_manager_static_render_data;
class c_gui_screen_widget;
class c_gui_widget;
class c_load_screen_message;

class c_window_manager
{
public:
	enum
	{
		k_maximum_number_of_channels_per_render_window = 10
	};

	enum e_widget_debug_state
	{
		_debug_name = 0,
		_debug_bounds,
		_debug_animation_state,
		_debug_rotation_origin,
	};

	struct s_screen_handle_datum :
		s_datum_header
	{
		c_gui_screen_widget* screen;
	};

	s_data_array* m_active_screens;
	uns32 m_last_milliseconds;
	c_gui_screen_widget* m_channels[k_number_of_render_windows][k_maximum_number_of_channels_per_render_window];
	c_synchronized_long m_current_channel_count[k_number_of_render_windows];
	rectangle2d m_last_known_viewport_bounds[k_number_of_render_windows];
	int32 m_had_ui_frames_ago[4]; // k_number_of_player_windows?
	int32 m_hs_thread_index;
	real32 m_fade_amount;
	bool m_fade_in;
	bool m_fade_out_and_quit_campaign;
	bool m_render_fade;
	uns8 __unknown127;

	c_gui_screen_widget* allocate_codeless_screen(int32 screen_name);
	c_gui_screen_widget* allocate_named_screen(int32 screen_name);
	void begin_transition_out(c_gui_screen_widget* screen, e_screen_transition_type transition_type);
	void close_all_screens(const c_gui_screen_widget** except_these, int32 except_these_count);
	void debug_widget_state(e_gui_widget_type type, int32 name, e_widget_debug_state state, bool activate, bool include_children);
	void debug_widget_state_for_screen(c_gui_screen_widget* screen, e_gui_widget_type type, int32 name, e_widget_debug_state state, bool activate, bool include_children);
	void debug_widget_state_for_widget(c_gui_widget* widget, e_widget_debug_state state, bool activate, bool include_children);
	void dispose();
	void dispose_from_old_map();
	void dispose_screen(c_gui_screen_widget* screen);
	bool get_fading() const;
	real32 get_fading_amount() const;
	bool get_fading_max() const;
	bool get_fading_min() const;
	e_window_index get_render_window_for_screen(c_gui_screen_widget* screen);
	c_gui_screen_widget* get_screen_above(e_window_index window_index, const c_gui_screen_widget* screen);
	c_gui_screen_widget* get_screen_below(e_window_index window_index, const c_gui_screen_widget* screen_widget);
	c_gui_screen_widget* get_screen_by_name(e_window_index window_index, int32 screen_name);
	c_gui_screen_widget* get_screen_by_screen_index(int32 screen_index);
	c_gui_screen_widget* get_topmost_screen(e_window_index window_index);
	void handle_global_controller_event(s_event_record* event_record);
	c_gui_screen_widget* load_screen(e_controller_index controller_index, bool load_as_error, const c_load_screen_message* screen_message, int32 window_index);
	static int32 __cdecl locate_screen_definition_tag_by_name(int32 name);
	static bool __cdecl named_screen_defined_in_code(int32 screen_name);
	static void __cdecl play_controller_sound(int32 optional_sound_tag_override_index, e_event_type event_type, e_controller_component controller_component);
	static void __cdecl print_active_screen_strings_tag_name();
	void print_active_screens();
	void render(e_window_index window_index, int32 user_index, const rectangle2d* viewport_bounds, bool is_screenshot);
	void render_fade();
	void reset_screens();
	void run_screen_hs_script(int32 script_index);
	void set_fade_out_and_quit_campaign(bool value, bool render_fade);
	void set_fading(bool fade_state);
	void update(uns32 milliseconds);
};
static_assert(sizeof(c_window_manager) == 0x128);

extern c_window_manager& g_window_manager;

extern void __cdecl window_manager_add_widgets_to_render_list_recursive(const rectangle2d* window_bounds, c_gui_widget* root_widget, s_window_manager_screen_render_data* render_data, e_controller_index local_controller_index);
extern void __cdecl window_manager_build_render_data_for_screen(const rectangle2d* viewport_bounds, c_gui_screen_widget* screen, s_window_manager_screen_render_data* render_data);
extern c_window_manager* __cdecl window_manager_get();
extern void __cdecl window_manager_load_screen_hs(int32 screen_name);
extern void __cdecl window_manager_render_screen_internal(s_window_manager_static_render_data* render_data, int32 user_index, const rectangle2d* viewport_bounds, bool is_screenshot);
extern void __cdecl window_manager_reset_screens();
