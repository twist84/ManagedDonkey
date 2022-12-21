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

extern void __cdecl sub_69D600();
extern bool __cdecl user_interface_join_remote_session(bool join_to_public_slots, long session_class, s_transport_secure_identifier* remote_session_id, s_transport_secure_address* remote_host_address, s_transport_secure_key* key);

