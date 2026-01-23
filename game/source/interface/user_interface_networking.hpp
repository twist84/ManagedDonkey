#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

class c_game_results;
struct s_transport_qos_result;

struct s_user_interface_networking_globals
{
	int16 minutes_spent_in_current_state;
	int16 minutes_remaining_in_current_state;
	uns32 time_entered_current_state;
	bool showing_join_dialog;
	c_string_id previous_join_alert;
	bool start_game_when_ready;
	e_last_game_was_started_from_main_menu_mode last_game_was_started_from_main_menu_mode;
	e_controller_index start_game_when_ready_controller_index;
	bool start_game_when_ready_from_test_command;
	int32 start_game_when_ready_user_count;
	int32 countdown_timer;
	e_network_interface_user_state current_error_priority_state;
	e_controller_index current_error_controller_index;
	c_string_id current_screen_name;
	c_string_id active_alert;
	c_string_id active_dialog;
	bool guide_was_open;
	uns32 time_the_guide_closed;
	bool do_not_reset_errored_users_on_dialog_close;
	bool do_not_show_dialog_until_guide_comes_up;
	struct
	{
		c_string_id dialog_name;
		bool join_to_public_slots;
		int32 session_class;
		s_transport_secure_identifier remote_session_id;
		s_transport_secure_address remote_host_address;
		s_transport_secure_key key;
	} current_join;
	bool last_known_good_matchmaking_team_setting;
};
COMPILE_ASSERT(sizeof(s_user_interface_networking_globals) == 0x88);

extern s_user_interface_networking_globals& user_interface_networking_globals;

extern void __cdecl sub_69D600();
extern bool __cdecl user_interface_join_remote_session(bool join_to_public_slots, int32 session_class, s_transport_secure_identifier* remote_session_id, s_transport_secure_address* remote_host_address, s_transport_secure_key* key);
extern void __cdecl user_interface_network_update_live_service_qos(const s_transport_qos_result* qos_result);
extern void __cdecl user_interface_networking_dispose();
extern void __cdecl user_interface_networking_dispose_from_old_map();
extern bool __cdecl user_interface_networking_enter_pregame_location(e_gui_game_mode game_mode);
extern e_gui_location __cdecl user_interface_networking_get_current_location();
extern const c_game_results* __cdecl user_interface_networking_get_final_game_results();
extern int32 __cdecl user_interface_networking_get_maximum_party_size();
extern int32 __cdecl user_interface_networking_get_name_from_gui_game_mode(e_gui_game_mode gui_game_mode);
extern e_controller_index __cdecl user_interface_networking_get_saved_game_controller();
extern e_gui_network_session_advertisement_mode __cdecl user_interface_networking_get_session_advertisement();
extern bool __cdecl user_interface_networking_get_start_game_when_ready();
extern void __cdecl user_interface_networking_memory_dispose();
extern void __cdecl user_interface_networking_memory_initialize(e_map_memory_configuration configuration);
extern bool __cdecl user_interface_networking_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode);
extern void __cdecl user_interface_networking_set_start_game_when_ready(bool value, int32 controller_index);
extern void __cdecl user_interface_networking_set_ui_upload_quota(int32 ui_upload_quota);
extern bool __cdecl user_interface_networking_should_allow_team_switching();
extern bool __cdecl user_interface_networking_should_show_skill();
extern bool __cdecl user_interface_networking_should_show_teams();
extern void __cdecl user_interface_networking_show_signed_out_while_in_live_alert();
extern void __cdecl user_interface_networking_start_game_when_ready_update();
extern void __cdecl user_interface_networking_update();
extern bool __cdecl user_interface_networking_xbox_guide_was_active_recently();
extern bool __cdecl user_interface_reset_networking_to_pregame();
extern void __cdecl user_interface_sanitize_game_setup_preferences_for_remote_squad_join();
extern int32 __cdecl user_interface_session_get_player_netdebug_filled_bar_count(int32 player_index);
extern void __cdecl user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode desired_multiplayer_mode);

