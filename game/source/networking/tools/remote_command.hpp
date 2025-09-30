#pragma once

#include "camera/observer.hpp"

#define REMOTE_COMMAND_HEADER_TAG "RCN"
#define REMOTE_COMMAND_PACKET_VERSION 1

#define MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE 0x400
#define MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE 0x10000

enum e_remote_command
{
	_remote_command_map_reset = 0,
	_remote_command_map_synch,
	_remote_command_camera,
	_remote_command_add_object,
	_remote_command_modify_object,
	_remote_command_delete_object,
	_remote_command_sound_command,
	_remote_command_hs_expression,
	_remote_command_tag_placement,
	_remote_command_flag_placement,
	
	NUMBER_OF_REMOTE_COMMANDS
};

struct transport_endpoint;
struct s_remote_command_globals
{
	transport_endpoint* send_endpoint;
	transport_endpoint* receive_endpoint;
	transport_endpoint* listen_endpoint;

	int32 reception_header_size;
	char reception_header_buffer[32];

	int32 reception_packet_total_size;
	int32 reception_packet_payload_size;
	int32 reception_packet_received;
	char reception_packet_buffer[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE + MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE];

	int32 last_camera_sync_milliseconds;
	s_observer_result last_camera_synch_state;

	//int32 light_volume_index;
	//uns32 light_volume_send_time;
	//scenario_light_block light_volume; // scenario->light_volumes[light_volume_index];

	// int32 connections?
	bool connected;
};
static_assert(sizeof(s_remote_command_globals) == 0x104B4);

//-----------------------------------------------------------------------------

#define k_maximum_number_of_tokens 100
#define k_token_length 256

#define COMMAND_CALLBACK_DECLARE(_command) callback_result_t _command##_callback(const void* userdata, int32 token_count, tokens_t const tokens)
#define COMMAND_CALLBACK_REGISTER(_command, _parameter_count, _parameters, ...) { #_command, _command##_callback, _parameter_count, _parameters, __VA_ARGS__ }

#define COMMAND_CALLBACK_PARAMETER_CHECK                                      \
ASSERT(userdata != nullptr);                                                  \
ASSERT((token_count - 1) >= 0);                                               \
                                                                              \
const s_command& command = *static_cast<const s_command*>(userdata);          \
callback_result_t result = command.name;                                      \
result.append_line(": succeeded");                                            \
if ((token_count - 1) != command.parameter_count)                             \
{                                                                             \
    result = "Invalid usage. ";                                               \
    result.append_print_line("%s %s", command.name, command.parameter_types); \
    result.append(command.extra_info);                                        \
    return result;                                                            \
}

//-----------------------------------------------------------------------------

using _token_t = c_static_string<k_token_length>;
using token_t = _token_t*;
using tokens_t = c_static_array<token_t, k_maximum_number_of_tokens>;

using callback_result_t = c_static_string<4096>;
using callback_t = callback_result_t(const void*, int32, tokens_t const);

struct s_command
{
	const char* name;
	callback_t* callback;
	int32 parameter_count;
	const char* parameter_types;
	const char* extra_info;
};

//-----------------------------------------------------------------------------

COMMAND_CALLBACK_DECLARE(script_start);
COMMAND_CALLBACK_DECLARE(game_save_and_quit);
COMMAND_CALLBACK_DECLARE(game_save_unsafe);
COMMAND_CALLBACK_DECLARE(game_safe_to_save);
COMMAND_CALLBACK_DECLARE(game_safe_to_speak);
COMMAND_CALLBACK_DECLARE(game_all_quiet);
COMMAND_CALLBACK_DECLARE(game_save);
COMMAND_CALLBACK_DECLARE(game_save_cancel);
COMMAND_CALLBACK_DECLARE(game_save_no_timeout);
COMMAND_CALLBACK_DECLARE(game_save_immediate);
COMMAND_CALLBACK_DECLARE(game_save_cinematic_skip);
COMMAND_CALLBACK_DECLARE(game_saving);
COMMAND_CALLBACK_DECLARE(game_reverted);
COMMAND_CALLBACK_DECLARE(gui_print_active_screens);
COMMAND_CALLBACK_DECLARE(gui_print_active_screen_strings);
COMMAND_CALLBACK_DECLARE(gui_debug_screen_name);
COMMAND_CALLBACK_DECLARE(gui_debug_screen_animation);
COMMAND_CALLBACK_DECLARE(gui_debug_screen_bounds);
COMMAND_CALLBACK_DECLARE(gui_debug_screen_rotation);
COMMAND_CALLBACK_DECLARE(gui_debug_group_name);
COMMAND_CALLBACK_DECLARE(gui_debug_group_animation);
COMMAND_CALLBACK_DECLARE(gui_debug_group_bounds);
COMMAND_CALLBACK_DECLARE(gui_debug_group_rotation);
COMMAND_CALLBACK_DECLARE(gui_debug_list_name);
COMMAND_CALLBACK_DECLARE(gui_debug_list_animation);
COMMAND_CALLBACK_DECLARE(gui_debug_list_bounds);
COMMAND_CALLBACK_DECLARE(gui_debug_list_rotation);
COMMAND_CALLBACK_DECLARE(gui_debug_list_item_name);
COMMAND_CALLBACK_DECLARE(gui_debug_list_item_animation);
COMMAND_CALLBACK_DECLARE(gui_debug_list_item_bounds);
COMMAND_CALLBACK_DECLARE(gui_debug_list_item_rotation);
COMMAND_CALLBACK_DECLARE(gui_debug_text_name);
COMMAND_CALLBACK_DECLARE(gui_debug_text_animation);
COMMAND_CALLBACK_DECLARE(gui_debug_text_bounds);
COMMAND_CALLBACK_DECLARE(gui_debug_text_rotation);
COMMAND_CALLBACK_DECLARE(gui_debug_bitmap_name);
COMMAND_CALLBACK_DECLARE(gui_debug_bitmap_animation);
COMMAND_CALLBACK_DECLARE(gui_debug_bitmap_bounds);
COMMAND_CALLBACK_DECLARE(gui_debug_bitmap_rotation);
COMMAND_CALLBACK_DECLARE(net_session_create);
COMMAND_CALLBACK_DECLARE(net_session_add);
COMMAND_CALLBACK_DECLARE(net_test_ping);
COMMAND_CALLBACK_DECLARE(net_test_ping_directed);
COMMAND_CALLBACK_DECLARE(net_test_text_chat);
COMMAND_CALLBACK_DECLARE(net_test_text_chat_directed);
COMMAND_CALLBACK_DECLARE(net_test_player_color);
COMMAND_CALLBACK_DECLARE(net_test_map_name);
COMMAND_CALLBACK_DECLARE(net_test_variant);
COMMAND_CALLBACK_DECLARE(net_test_reset_objects);
COMMAND_CALLBACK_DECLARE(net_test_session_mode);
COMMAND_CALLBACK_DECLARE(net_test_ui_game_mode);
COMMAND_CALLBACK_DECLARE(net_test_advertisement_mode);
COMMAND_CALLBACK_DECLARE(net_test_game_variant_parameter);
COMMAND_CALLBACK_DECLARE(net_build_game_variant);
COMMAND_CALLBACK_DECLARE(net_verify_game_variant);
COMMAND_CALLBACK_DECLARE(net_load_and_use_game_variant);
COMMAND_CALLBACK_DECLARE(net_verify_packed_game_variant);
COMMAND_CALLBACK_DECLARE(net_load_and_use_packed_game_variant);
COMMAND_CALLBACK_DECLARE(net_build_map_variant);
COMMAND_CALLBACK_DECLARE(net_verify_map_variant);
COMMAND_CALLBACK_DECLARE(net_load_and_use_map_variant);
COMMAND_CALLBACK_DECLARE(net_verify_packed_map_variant);
COMMAND_CALLBACK_DECLARE(net_load_and_use_packed_map_variant);
COMMAND_CALLBACK_DECLARE(net_build_network_config);
COMMAND_CALLBACK_DECLARE(game_export_variant_settings);
COMMAND_CALLBACK_DECLARE(online_set_is_connected_to_live);
COMMAND_CALLBACK_DECLARE(online_user_set_name);
COMMAND_CALLBACK_DECLARE(mp_players_by_team);
COMMAND_CALLBACK_DECLARE(deterministic_end_game);
COMMAND_CALLBACK_DECLARE(mp_active_player_count_by_team);
COMMAND_CALLBACK_DECLARE(mp_respawn_override_timers);
COMMAND_CALLBACK_DECLARE(mp_ai_allegiance);
COMMAND_CALLBACK_DECLARE(mp_allegiance);
COMMAND_CALLBACK_DECLARE(mp_object_belongs_to_team);
COMMAND_CALLBACK_DECLARE(mp_weapon_belongs_to_team);
COMMAND_CALLBACK_DECLARE(mp_debug_goal_object_boundary_geometry);
COMMAND_CALLBACK_DECLARE(load_preferences_from_file);
COMMAND_CALLBACK_DECLARE(load_customization_from_file);
COMMAND_CALLBACK_DECLARE(cheat_all_chars);
COMMAND_CALLBACK_DECLARE(debug_menu_rebuild);
COMMAND_CALLBACK_DECLARE(drop_permutation);
COMMAND_CALLBACK_DECLARE(director_debug_camera);
COMMAND_CALLBACK_DECLARE(camera_control);
COMMAND_CALLBACK_DECLARE(camera_set_mode);
COMMAND_CALLBACK_DECLARE(debug_camera_save);
COMMAND_CALLBACK_DECLARE(debug_camera_load);
COMMAND_CALLBACK_DECLARE(crash);
COMMAND_CALLBACK_DECLARE(status);
COMMAND_CALLBACK_DECLARE(player_force_mode);
COMMAND_CALLBACK_DECLARE(test_download_storage_file);
COMMAND_CALLBACK_DECLARE(lsp_info_get);
COMMAND_CALLBACK_DECLARE(lsp_info_set);
COMMAND_CALLBACK_DECLARE(player_ragdoll);
COMMAND_CALLBACK_DECLARE(player_drop_weapon);
COMMAND_CALLBACK_DECLARE(player_add_weapon);
COMMAND_CALLBACK_DECLARE(levels_add_fake_map_solo);
COMMAND_CALLBACK_DECLARE(levels_add_map_solo);
COMMAND_CALLBACK_DECLARE(levels_add_fake_map_multi);
COMMAND_CALLBACK_DECLARE(levels_add_map_multi);
COMMAND_CALLBACK_DECLARE(ui_debug_text_font);
COMMAND_CALLBACK_DECLARE(xoverlapped_debug_render);
COMMAND_CALLBACK_DECLARE(overlapped_display_task_descriptions);
COMMAND_CALLBACK_DECLARE(overlapped_task_inject_error);
COMMAND_CALLBACK_DECLARE(overlapped_task_pause);
COMMAND_CALLBACK_DECLARE(sound_loop_spam);

COMMAND_CALLBACK_DECLARE(controller_set_background_emblem_color);
COMMAND_CALLBACK_DECLARE(controller_set_button_preset);
COMMAND_CALLBACK_DECLARE(controller_set_auto_center_look);
COMMAND_CALLBACK_DECLARE(controller_set_crouch_lock);
COMMAND_CALLBACK_DECLARE(controller_set_flight_stick_aircraft_controls);
COMMAND_CALLBACK_DECLARE(controller_set_look_inverted);
COMMAND_CALLBACK_DECLARE(controller_set_vibration_enabled);
COMMAND_CALLBACK_DECLARE(controller_set_emblem_info);
COMMAND_CALLBACK_DECLARE(controller_set_joystick_preset);
COMMAND_CALLBACK_DECLARE(controller_set_look_sensitivity);
COMMAND_CALLBACK_DECLARE(controller_set_player_character_type);
COMMAND_CALLBACK_DECLARE(controller_set_popup_message_index);
COMMAND_CALLBACK_DECLARE(controller_set_primary_change_color);
COMMAND_CALLBACK_DECLARE(controller_set_primary_emblem_color);
COMMAND_CALLBACK_DECLARE(controller_set_secondary_change_color);
COMMAND_CALLBACK_DECLARE(controller_set_secondary_emblem_color);
COMMAND_CALLBACK_DECLARE(controller_set_single_player_level_completed);
COMMAND_CALLBACK_DECLARE(controller_set_single_player_level_unlocked);
COMMAND_CALLBACK_DECLARE(controller_set_subtitle_setting);
COMMAND_CALLBACK_DECLARE(controller_set_tertiary_change_color);
COMMAND_CALLBACK_DECLARE(controller_set_voice_mask);
COMMAND_CALLBACK_DECLARE(controller_set_voice_output_setting);

//-----------------------------------------------------------------------------

s_command const k_registered_commands[] =
{
	COMMAND_CALLBACK_REGISTER(script_start, 1, "<string>", "debug script launching: starts a scenario script by name.\r\nNETWORK SAFE: No, for mainmenu only"),

	COMMAND_CALLBACK_REGISTER(game_save_and_quit, 0, "", "save & quit to the main menu\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_save_unsafe, 0, "", "saves right now, even if the game is in an immediate-loss state (NEVER USE THIS! EVER!)\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_safe_to_save, 0, "", "returns FALSE if it would be a bad idea to save the player's game right now\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_safe_to_speak, 0, "", "returns FALSE if it would be a bad idea to play mission dialog right now\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_all_quiet, 0, "", "returns FALSE if there are bad guys around, projectiles in the air, etc.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_save, 0, "", "checks to see if it is safe to save game, then saves (gives up after 8 seconds)\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_save_cancel, 0, "", "cancels any pending game_save, timeout or not\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_save_no_timeout, 0, "", "checks to see if it is safe to save game, then saves (this version never gives up)\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_save_immediate, 0, "", "disregards player's current situation and saves (BE VERY CAREFUL!)\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_save_cinematic_skip, 0, "", "don't use this, except in one place.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_saving, 0, "", "checks to see if the game is trying to save the map.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_reverted, 0, "", "don't use this for anything, you black-hearted bastards.\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(gui_print_active_screens, 0, "", "display list of active halox ui screens\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_print_active_screen_strings, 0, "display strings tag file name for current topmost screen", "\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_screen_name, 3, "<string_id> <boolean> <boolean>", "Toggle display of given screen's name, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_screen_animation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given screen animations, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_screen_bounds, 3, "<string_id> <boolean> <boolean>", "Toggle display of given screen's bounds, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_screen_rotation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given screen's rotation, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_group_name, 3, "<string_id> <boolean> <boolean>", "Toggle display of given group's name, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_group_animation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given group's animations, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_group_bounds, 3, "<string_id> <boolean> <boolean>", "Toggle display of given group's bounds, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_group_rotation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given group's rotation, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_name, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list's name, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_animation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list's animations, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_bounds, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list's bounds, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_rotation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list's rotation, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_item_name, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list item's name, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_item_animation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list item's animation, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_item_bounds, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list item's bounds, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_list_item_rotation, 3, "<string_id> <boolean> <boolean>", "Toggle display of given list item's rotation, optionally recursive\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_text_name, 2, "<string_id> <boolean>", "Toggle display of given text widget's name\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_text_animation, 2, "<string_id> <boolean>", "Toggle display of given text widget's animation state\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_text_bounds, 2, "<string_id> <boolean>", "Toggle display of given text widget's bounds\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_text_rotation, 2, "<string_id> <boolean>", "Toggle display of given text widget's rotation\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_bitmap_name, 2, "<string_id> <boolean>", "Toggle display of given bitmap widget's name\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_bitmap_animation, 2, "<string_id> <boolean>", "Toggle display of given bitmap widget's animation state\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_bitmap_bounds, 2, "<string_id> <boolean>", "Toggle display of given bitmap widget's bounds\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(gui_debug_bitmap_rotation, 2, "<string_id> <boolean>", "Toggle display of given bitmap widget's rotation\r\nNETWORK SAFE: No"),

	COMMAND_CALLBACK_REGISTER(net_session_create, 2, "<string> <string>", "<ui_game_mode> <advertisement_mode> creates a session to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_session_add, 1, "<string>", "<ip:port> adds a session from the given ip:port to the local games browser \r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_test_ping, 0, "", "network test: sends a ping\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_ping_directed, 1, "<string>", "<ip:port> network test: sends a ping to a specific address\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_text_chat, 1, "<string>", "<message> network test: sends a message\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_text_chat_directed, 2, "<string> <string>", "<ip:port> <message> network test: sends a message to a specific address\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_test_player_color, 1, "<long>", "network test: temporarily sets the color for all local players\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_map_name, 1, "<string>", "network test: sets the name of the scenario to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_variant, 1, "<string>", "network test: sets the game variant to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_reset_objects, 0, "", "network test: resets all objects on the map\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_session_mode, 1, "<string>", "network test: sets the session mode to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_ui_game_mode, 1, "<string>", "network test: sets the ui game mode to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_test_advertisement_mode, 1, "<string>", "network test: sets the advertisement mode to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_test_game_variant_parameter, 2, "<string> <long>", "network test: sets a parameter of the current game variant\r\nNETWORK SAFE: No, for mainmenu only"),

	COMMAND_CALLBACK_REGISTER(net_build_network_config, 0, "", "writes a new network configuration file\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_build_game_variant, 1, "<string>", "writes the current game variant to a file\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_verify_game_variant, 1, "<string>", "verifies the contents of a game variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_game_variant, 1, "<string>", "loads the contents of a game variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_verify_packed_game_variant, 1, "<string>", "verifies the contents of a packed game variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_packed_game_variant, 1, "<string>", "loads the contents of a packed game variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_build_map_variant, 1, "<string>", "writes the current map variant to a file\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_verify_map_variant, 1, "<string>", "verifies the contents of a map variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_map_variant, 1, "<string>", "loads the contents of a map variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_verify_packed_map_variant, 1, "<string>", "verifies the contents of a packed map variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_packed_map_variant, 1, "<string>", "loads the contents of a packed map variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(game_export_variant_settings, 1, "<string>", "export the current game engine variant settings to the specified text file\r\nNETWORK SAFE: No"),

	COMMAND_CALLBACK_REGISTER(online_set_is_connected_to_live, 1, "<boolean>", "sets connected to live\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(online_user_set_name, 2, "<long> <string>", "sets the name of a specific user\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(mp_players_by_team, 1, "<long>", "<mp_team> given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(deterministic_end_game, 0, "", "end game deterministically, by inserting a simulation queue event\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_active_player_count_by_team, 1, "<long>", "<mp_team> given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_respawn_override_timers, 1, "<short>", "<mp_team> causes all players on the specified team waiting to respawn (due to timer) to respawn immediately\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_ai_allegiance, 2, "<short> <short>", "<team> <mp_team> causes an allegiance to be formed between an AI squad team and a multiplayer team\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_allegiance, 2, "<short> <short>", "<mp_team> <mp_team> create an allegiance between two multiplayer teams\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_object_belongs_to_team, 2, "<long> <short>", "<object> <mp_team> causes specified object to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_weapon_belongs_to_team, 2, "<long> <short>", "<object> <mp_team> causes specified weapon to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(mp_debug_goal_object_boundary_geometry, 1, "<boolean>", "toggle debug geometry for multiplayer goal objects\r\nNETWORK SAFE: No"),

	COMMAND_CALLBACK_REGISTER(load_preferences_from_file, 1, "<string>", "<preferences filename> loads preferences from the specified file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(load_customization_from_file, 1, "<string>", "<customization filename> loads customization from the specified file\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(debug_menu_rebuild, 0, "", "Reparses the debug menu from the text file."),

	COMMAND_CALLBACK_REGISTER(drop_permutation, 2, "<string> <string>", "drops the named tag e.g. objects\\characters\\brute\\brute.biped using the specified permutation. permutations are specified as a comma-delimited string of region=permutation pairs (e.g. region1=permutation1,region2=permutation2).\r\n"),

	COMMAND_CALLBACK_REGISTER(director_debug_camera, 1, "<boolean>", "enable/disable camera debugging\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(camera_control, 1, "<boolean>", "toggles script control of the camera.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(camera_set_mode, 2, "<long> <long>", "<user_index> <camera_mode> sets user's camera perspective\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(debug_camera_save, 0, "", "saves the camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(debug_camera_load, 0, "", "loads the saved camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(crash, 1, "<string>", "crashes (for debugging).\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(status, 0, "", "prints the value of all global status variables.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(player_force_mode, 1, "<string_id>", "force your will upon the player\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(test_download_storage_file, 2, "<string> <string>", "<url> <filename> downloads a file from LSP to the client\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(lsp_info_get, 0, "", "gets the LSP server info\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(lsp_info_set, 1, "<string>", "<ip:port> sets the LSP server address and port\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(player_ragdoll, 0, "", "ragdolls the players biped.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(player_drop_weapon, 0, "", "drops the players held weapon.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(player_add_weapon, 2, "<string> <long>", "<weapon_definition_index> <weapon_addition_method> gives the player a weapon.\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(levels_add_fake_map_solo, 1, "<string>", "<scenario_path> adds a fake map for campaign\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(levels_add_map_solo, 2, "<long> <string>", "<map_id> <scenario_path> adds a map with the specified map id for campaign\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(levels_add_fake_map_multi, 1, "<string>", "<scenario_path> adds a fake map for multiplayer\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(levels_add_map_multi, 2, "<long> <string>", "<map_id> <scenario_path> adds a map with the specified map id for multiplayer\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(ui_debug_text_font, 1, "<boolean>", "toggle display of ui text font\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(xoverlapped_debug_render, 1, "<boolean>", "toggle display a list of active xoverlapped tasks\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(overlapped_display_task_descriptions, 0, "", "displays all tasks\r\nNETWORK SAFE: lol"),
	COMMAND_CALLBACK_REGISTER(overlapped_task_inject_error, 2, "<string> <boolean>", "inject error for tasks\r\nNETWORK SAFE: lol"),
	COMMAND_CALLBACK_REGISTER(overlapped_task_pause, 2, "<string> <boolean>", "pause tasks\r\nNETWORK SAFE: lol"),

	COMMAND_CALLBACK_REGISTER(sound_loop_spam, 0, "", "start all loaded looping sounds\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(controller_set_background_emblem_color, 2, "<controller> <player_color>", "set tertiary color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_button_preset, 2, "<controller> <button_preset>", "set button preset for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_auto_center_look, 2, "<controller> <boolean>", "set auto center look for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_crouch_lock, 2, "<controller> <boolean>", "set crouch lock for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_flight_stick_aircraft_controls, 2, "<controller> <boolean>", "set airrcraft flight stick controls for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_look_inverted, 2, "<controller> <boolean>", "set look inversion for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_vibration_enabled, 2, "<controller> <boolean>", "set vibration for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_emblem_info, 3, "<controller> <short> <short>", "set emblem for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_joystick_preset, 2, "<controller> <joystick_preset>", "set joystick preset for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_look_sensitivity, 2, "<controller> <short>", "set look sensitivity for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_player_character_type, 2, "<controller> <player_character_type>", "set player character type for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_popup_message_index, 2, "<controller> <long>", "<controller> <message index> set the highest popup message that the player has accepted\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(controller_set_primary_change_color, 2, "<controller> <player_color>", "set primary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_primary_emblem_color, 2, "<controller> <player_color>", "set primary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_secondary_change_color, 2, "<controller> <player_color>", "set secondary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_secondary_emblem_color, 2, "<controller> <player_color>", "set secondary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_single_player_level_completed, 5, "<controller> <long> <boolean> <game_difficulty> <boolean>", "<controller> <level index> <co-op> <difficulty> <completed> set the single player levels completed state for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_single_player_level_unlocked, 3, "<controller> <short> <boolean>", "<controller> <level index> <bool locked> set single player level locked state for specified controller\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(controller_set_subtitle_setting, 2, "<controller> <subtitle_setting>", "set subtitle setting for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_tertiary_change_color, 2, "<controller> <player_color>", "set tertiary color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_voice_mask, 2, "<controller> <voice_mask>", "set voice mask for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_voice_output_setting, 2, "<controller> <voice_output_setting>", "set voice output setting for specified controller\r\nNETWORK SAFE: No"),
};

extern void command_tokenize(const char* input, tokens_t& tokens, int32* token_count);
extern int32 token_try_parse_bool(const token_t& token);
extern bool load_preference(const char* name, const char* value);

extern s_remote_command_globals remote_command_globals;

extern void __cdecl remote_command_dispose();
extern void __cdecl remote_command_initialize();
extern bool __cdecl remote_command_connected();
extern void __cdecl remote_command_disconnect();
extern void __cdecl remote_command_process();
extern bool __cdecl remote_command_process_received_chunk(const char* buffer, int32 buffer_length);
extern bool __cdecl remote_command_send_encoded(int32 encoded_command_size, const void* encoded_command_buffer, int32 payload_size, const void* payload);
extern bool __cdecl remote_command_send(int32 command_type, const void* a2, int32 payload_size, const void* payload);
extern bool __cdecl remote_camera_update(int32 user_index, const s_observer_result* camera);

