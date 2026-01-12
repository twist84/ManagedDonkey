#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "networking/online/online_service_record.hpp"

class c_controller_input_message;

struct s_start_menu_breadcrumb
{
	enum
	{
		k_maximum_breadcrumbs = 8,
	};

	int32 screen_id;
	int32 element_handle;
};
static_assert(sizeof(s_start_menu_breadcrumb) == 0x8);

class c_start_menu_screen_widget :
	public c_gui_screen_widget
{
public:
	void __thiscall back_out_current_pane_();
	void __thiscall dispose_();
	bool __thiscall handle_controller_input_message_(c_controller_input_message* message);
	static bool __cdecl handle_global_start_button_press_(c_controller_input_message* message);
	void __thiscall initialize_();

public:
	virtual ~c_start_menu_screen_widget() = default;
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual void dispose() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;

protected:
	virtual e_render_data_size get_render_data_size() override;

public:
	virtual void reconstruct() override;
	virtual void initialize_datasource() override;
	virtual bool process_message(const c_message* message) override;
	virtual void transition_out_with_transition_type(e_transition_out_type transition_out, e_screen_transition_type transition_type) override;
	virtual bool __funcs53() override;

private:
	void update_pane();

public:
	c_start_menu_screen_widget(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00ADFE60, void, __thiscall, c_start_menu_screen_widget*, int32)(this, name);
	}

private:
	void back_out_current_pane();

public:
	static int32 __cdecl get_current_game_pane_name();

private:
	c_gui_screen_widget* get_current_pane();

public:
	static bool __cdecl handle_global_start_button_press(const c_controller_input_message* message);
	void load_pane(int32 pane_to_load, bool is_top_level_pane, e_screen_transition_type transition_type, int32 focus_on_load_list_name, int32 focus_on_load_element_handle, real32 appearance_camera_zoom, real32 appearance_camera_yaw);

private:
	void load_pane_direct(int32 pane_to_load, e_screen_transition_type transition_type, int32 focus_on_load_list_name, int32 focus_on_load_element_handle, real32 camera_zoom, real32 camera_yaw);

public:
	static bool __cdecl load_start_menu(e_controller_index controller_index, const s_player_identifier* player_identifier, const uns64* player_xuid, const s_service_record_identity* identity, const s_start_menu_breadcrumb* breadcrumbs, int32 breadcrumb_count);

//protected:
	s_player_identifier m_owner_player_id;
	s_player_identifier m_target_player_id;
	uns64 m_target_player_xuid;
	s_service_record_identity m_service_record_identity;
	real32 m_old_film_playback_speed;
	c_static_stack<s_start_menu_breadcrumb, s_start_menu_breadcrumb::k_maximum_breadcrumbs> m_breadcrumbs;
	int32 m_requested_pane;
	bool m_breadcrumb_to_load_valid;
	s_start_menu_breadcrumb m_breadcrumb_to_load;
	bool m_is_rooted;
	bool m_wants_to_pause_game_time;
};
static_assert(sizeof(c_start_menu_screen_widget) == 0x21B8);
static_assert(sizeof(c_start_menu_screen_widget) == sizeof(c_gui_screen_widget) + 0x718);
static_assert(0x1AA0 == offsetof(c_start_menu_screen_widget, m_owner_player_id));
static_assert(0x1AA8 == offsetof(c_start_menu_screen_widget, m_target_player_id));
static_assert(0x1AB0 == offsetof(c_start_menu_screen_widget, m_target_player_xuid));
static_assert(0x1AB8 == offsetof(c_start_menu_screen_widget, m_service_record_identity));
static_assert(0x2158 == offsetof(c_start_menu_screen_widget, m_old_film_playback_speed));
static_assert(0x215C == offsetof(c_start_menu_screen_widget, m_breadcrumbs));
static_assert(0x21A0 == offsetof(c_start_menu_screen_widget, m_requested_pane));
static_assert(0x21A4 == offsetof(c_start_menu_screen_widget, m_breadcrumb_to_load_valid));
static_assert(0x21A8 == offsetof(c_start_menu_screen_widget, m_breadcrumb_to_load));
static_assert(0x21B0 == offsetof(c_start_menu_screen_widget, m_is_rooted));
static_assert(0x21B1 == offsetof(c_start_menu_screen_widget, m_wants_to_pause_game_time));

