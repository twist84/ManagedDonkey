#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "networking/online/online_service_record.hpp"

enum e_controller_index;
struct c_controller_input_message;

struct s_start_menu_breadcrumb
{
	long screen_id;
	long element_handle;
};
static_assert(sizeof(s_start_menu_breadcrumb) == 0x8);

struct c_start_menu_screen_widget :
	public c_gui_screen_widget
{
public:
	bool __thiscall handle_controller_input_message_(c_controller_input_message* message);
	static bool __cdecl handle_global_start_button_press_(c_controller_input_message* message);
	static bool __cdecl load_start_menu(e_controller_index controller_index, s_player_identifier const* player_identifier, qword const* player_xuid, s_service_record_identity const* identity, s_start_menu_breadcrumb const* breadcrumbs, long breadcrumb_count);

protected:
	s_player_identifier m_owner_player_id;
	s_player_identifier m_target_player_id;
	qword m_target_player_xuid;
	s_service_record_identity m_service_record_identity;
	real m_old_film_playback_speed;
	c_static_stack<s_start_menu_breadcrumb, 8> m_breadcrumbs;
	long m_requested_pane;
	bool m_breadcrumb_to_load_valid;
	s_start_menu_breadcrumb m_breadcrumb_to_load;
	bool m_is_rooted;
	bool m_wants_to_pause_game_time;
};
static_assert(sizeof(c_start_menu_screen_widget) == sizeof(c_gui_screen_widget) + 0x718);

