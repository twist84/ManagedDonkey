#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

struct s_user_interface_networking_globals
{
	byte __data0[0x8];

	bool join_in_progress;
	c_string_id join_failed_alert_name;

	bool start_game_when_ready;
	long start_game_last_started;
	long start_game_controller_index;

	bool __unknown1C;
	long player_count;
	long countdown_timer;

	long local_user_state;

	long current_screen_controller_index;
	c_string_id current_screen_name;
	c_string_id active_alert;
	c_string_id active_dialog;

	bool xbox_guide_active;
	dword xbox_guide_last_active_time;
	bool __unknown44;
	bool __unknown45;

	c_string_id join_dialog_name;
	bool join_to_public_slots;
	long session_class;
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_secure_address;
	s_transport_secure_key session_key;

	bool should_show_teams;
};
static_assert(sizeof(s_user_interface_networking_globals) == 0x88);

extern s_user_interface_networking_globals& user_interface_networking_globals;

enum e_gui_game_mode;
struct c_game_results;
enum e_gui_network_session_advertisement_mode;
enum e_desired_multiplayer_mode;

extern void __cdecl sub_69D600();
extern bool __cdecl user_interface_join_remote_session(bool join_to_public_slots, long session_class, s_transport_secure_identifier* remote_session_id, s_transport_secure_address* remote_host_address, s_transport_secure_key* key);
extern bool __cdecl user_interface_networking_enter_pregame_location(e_gui_game_mode game_mode);
extern c_game_results const* __cdecl user_interface_networking_get_final_game_results();
extern e_gui_network_session_advertisement_mode __cdecl user_interface_networking_get_session_advertisement();
extern void __cdecl user_interface_networking_set_start_game_when_ready(bool a1, long controller_index);
extern void __cdecl user_interface_networking_set_ui_upload_quota(long ui_upload_quota);
extern bool __cdecl user_interface_networking_should_allow_team_switching();
extern bool __cdecl sub_A7FB70();
extern bool __cdecl user_interface_networking_should_show_teams();
extern void __cdecl user_interface_networking_show_signed_out_while_in_live_alert();
extern void __cdecl user_interface_networking_start_game_when_ready_update();
extern void __cdecl user_interface_networking_update();
extern bool __cdecl user_interface_networking_xbox_guide_was_active_recently();
extern bool __cdecl user_interface_reset_networking_to_pregame();
extern void __cdecl user_interface_sanitize_game_setup_preferences_for_remote_squad_join();
extern long __cdecl sub_A7FFA0(long player_index);
extern void __cdecl user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode desired_multiplayer_mode);

