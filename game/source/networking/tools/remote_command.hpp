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

	long reception_header_size;
	char reception_header_buffer[32];

	long reception_packet_total_size;
	long reception_packet_size;
	long reception_packet_received;
	char reception_packet[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE + MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE];

	dword camera_send_time;
	s_observer_result camera;

	//long light_volume_index;
	//dword light_volume_send_time;
	//scenario_light_block light_volume; // scenario->light_volumes[light_volume_index];

	// long connections?
	bool connected;
};
static_assert(sizeof(s_remote_command_globals) == 0x104B4);

//-----------------------------------------------------------------------------

#define k_maximum_number_of_tokens 100
#define k_token_length 256

#define COMMAND_CALLBACK_DECLARE(_command) callback_result_t _command##_callback(void const* userdata, long token_count, tokens_t const tokens)
#define COMMAND_CALLBACK_REGISTER(_command, _parameter_count, _parameters, ...) { #_command, _command##_callback, _parameter_count, _parameters, __VA_ARGS__ }

#define COMMAND_CALLBACK_PARAMETER_CHECK                                      \
ASSERT(userdata != nullptr);                                                  \
ASSERT((token_count - 1) >= 0);                                               \
                                                                              \
s_command const& command = *static_cast<s_command const*>(userdata);          \
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
using callback_t = callback_result_t(void const*, long, tokens_t const);

struct s_command
{
	char const* name;
	callback_t* callback;
	long parameter_count;
	char const* parameter_types;
	char const* extra_info;
};

//-----------------------------------------------------------------------------

COMMAND_CALLBACK_DECLARE(help);
COMMAND_CALLBACK_DECLARE(script_doc);
COMMAND_CALLBACK_DECLARE(breakpoint);
COMMAND_CALLBACK_DECLARE(set);
COMMAND_CALLBACK_DECLARE(exit_game);
COMMAND_CALLBACK_DECLARE(script_start);
COMMAND_CALLBACK_DECLARE(map_name);
COMMAND_CALLBACK_DECLARE(game_multiplayer);
COMMAND_CALLBACK_DECLARE(game_splitscreen);
COMMAND_CALLBACK_DECLARE(game_difficulty);
COMMAND_CALLBACK_DECLARE(game_coop_players);
COMMAND_CALLBACK_DECLARE(game_initial_bsp);
COMMAND_CALLBACK_DECLARE(game_tick_rate);
COMMAND_CALLBACK_DECLARE(game_start);
COMMAND_CALLBACK_DECLARE(game_won);
COMMAND_CALLBACK_DECLARE(main_menu);
COMMAND_CALLBACK_DECLARE(net_session_create);
COMMAND_CALLBACK_DECLARE(net_session_add);
COMMAND_CALLBACK_DECLARE(net_test_ping);
COMMAND_CALLBACK_DECLARE(net_test_ping_directed);
COMMAND_CALLBACK_DECLARE(net_test_text_chat);
COMMAND_CALLBACK_DECLARE(net_test_text_chat_directed);
COMMAND_CALLBACK_DECLARE(net_test_map_name);
COMMAND_CALLBACK_DECLARE(net_test_variant);
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
COMMAND_CALLBACK_DECLARE(game_export_variant_settings);
COMMAND_CALLBACK_DECLARE(alert_carry);
COMMAND_CALLBACK_DECLARE(online_set_is_connected_to_live);
COMMAND_CALLBACK_DECLARE(online_user_set_name);
COMMAND_CALLBACK_DECLARE(mp_game_won);
COMMAND_CALLBACK_DECLARE(mp_debug_goal_object_boundary_geometry);
COMMAND_CALLBACK_DECLARE(load_preferences_from_file);
COMMAND_CALLBACK_DECLARE(load_customization_from_file);
COMMAND_CALLBACK_DECLARE(cheat_all_powerups);
COMMAND_CALLBACK_DECLARE(cheat_all_vehicles);
COMMAND_CALLBACK_DECLARE(cheat_all_weapons);
COMMAND_CALLBACK_DECLARE(cheat_teleport_to_camera);
COMMAND_CALLBACK_DECLARE(debug_menu_rebuild);
COMMAND_CALLBACK_DECLARE(drop);
COMMAND_CALLBACK_DECLARE(drop_variant);
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

//-----------------------------------------------------------------------------

s_command const k_registered_commands[] =
{
	COMMAND_CALLBACK_REGISTER(help, 0, "", "prints this output.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(script_doc, 0, "", "saves a file called hs_doc.txt with parameters for all script commands.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(breakpoint, 1, "<string>", "If breakpoints are enabled, pause execution when this statement is hit (displaying the given message).\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(set, 0, "", "set the value of a global variable.\r\nNETWORK SAFE: Yes (depending on result)"),
	COMMAND_CALLBACK_REGISTER(exit_game, 0, "", "exits the game.\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(script_start, 1, "<string>", "debug script launching: starts a scenario script by name.\r\nNETWORK SAFE: No, for mainmenu only"),

	COMMAND_CALLBACK_REGISTER(map_name, 1, "<string>", "debug map launching: sets the multiplayer engine for the next map.\r\nNo, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_multiplayer, 1, "<string>", "debug map launching: sets the multiplayer engine for the next map.\r\nNo, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_splitscreen, 1, "<long>", "debug map launching: sets the number of multiplayer splitscreen players for the next map.\r\nNETWORK SAFE: No, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_difficulty, 1, "<game_difficulty>", "debug map launching: sets the difficulty of the next map.\r\nNETWORK SAFE: No, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_coop_players, 1, "<long>", "debug map launching: sets the number of coop players for the next map.\r\nNETWORK SAFE: No, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_initial_bsp, 1, "<long>", "debug map launching: sets the initial bsp for the next map.\r\nNETWORK SAFE: No, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_tick_rate, 1, "<long>", "debug map launching: sets the tick rate for the next map.\r\nNETWORK SAFE: No, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_start, 1, "<string>", "debug map launching: starts a game on the specified map.\r\nNETWORK SAFE: No, for init.txt only"),
	COMMAND_CALLBACK_REGISTER(game_won, 0, "", "causes the player to successfully finish the current level and move to the next\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(main_menu, 0, "", "goes back to the main menu\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_session_create, 2, "<string> <string>", "<ui_game_mode> <advertisement_mode> creates a session to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_session_add, 1, "<string>", "<ip:port> adds a session from the given ip:port to the local games browser \r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_test_ping, 0, "", "network test: sends a ping\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_ping_directed, 1, "<string>", "<ip:port> network test: sends a ping to a specific address\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_text_chat, 1, "<string>", "<message> network test: sends a message\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_text_chat_directed, 2, "<string> <string>", "<ip:port> <message> network test: sends a message to a specific address\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_test_map_name, 1, "<string>", "network test: sets the name of the scenario to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_variant, 1, "<string>", "network test: sets the game variant to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_session_mode, 1, "<string>", "network test: sets the session mode to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_ui_game_mode, 1, "<string>", "network test: sets the ui game mode to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_test_advertisement_mode, 1, "<string>", "network test: sets the advertisement mode to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_test_game_variant_parameter, 2, "<string> <long>", "network test: sets a parameter of the current game variant\r\nNETWORK SAFE: No, for mainmenu only"),

	COMMAND_CALLBACK_REGISTER(net_build_game_variant, 1, "<string>", "writes the current game variant to a file\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_verify_game_variant, 1, "<string>", "verifies the contents of a game variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_game_variant, 1, "<string>", "loads the contents of a game variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_verify_packed_game_variant, 1, "<string>", "verifies the contents of a packed game variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_packed_game_variant, 1, "<string>", "loads the contents of a packed game variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_build_map_variant, 1, "<string>", "writes the current map variant to a file\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_verify_map_variant, 1, "<string>", "verifies the contents of a packed map variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_map_variant, 1, "<string>", "loads the contents of a packed map variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_verify_packed_map_variant, 1, "<string>", "verifies the contents of a packed map variant file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(net_load_and_use_packed_map_variant, 1, "<string>", "loads the contents of a packed map variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(game_export_variant_settings, 1, "<string>", "export the current game engine variant settings to the specified text file\r\nNETWORK SAFE: No"),

	COMMAND_CALLBACK_REGISTER(alert_carry, 1, "<long>", "<user_index> raise/lower player weapon toggle\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(online_set_is_connected_to_live, 1, "<boolean>", "sets connected to live\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(online_user_set_name, 1, "<string>", "sets the name of the first user\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(mp_game_won, 1, "<long>", "<mp_team> given a team index, declares the game a victory for that team and a loss for all others\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(mp_debug_goal_object_boundary_geometry, 1, "<bool>", "toggle debug geometry for multiplayer goal objects\r\nNETWORK SAFE: No"),

	COMMAND_CALLBACK_REGISTER(load_preferences_from_file, 1, "<string>", "<preferences filename> loads preferences from the specified file\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(load_customization_from_file, 1, "<string>", "<customization filename> loads customization from the specified file\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(cheat_all_powerups, 0, "", "drops all powerups near player\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(cheat_all_vehicles, 0, "", "drops all vehicles on player\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(cheat_all_weapons, 0, "", "drops all weapons near player\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(cheat_teleport_to_camera, 0, "", "teleports player to camera location\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(debug_menu_rebuild, 0, "", "Reparses the debug menu from the text file."),

	COMMAND_CALLBACK_REGISTER(drop, 1, "<string>", "drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle\r\nNETWORK SAFE: Yes, for objects"),
	COMMAND_CALLBACK_REGISTER(drop_variant, 2, "<string> <string>", "drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle using the specified variant name\r\n"),
	COMMAND_CALLBACK_REGISTER(drop_permutation, 2, "<string> <string>", "drops the named tag e.g. objects\\characters\\brute\\brute.biped using the specified permutation. permutations are specified as a comma-delimited string of region=permutation pairs (e.g. region1=permutation1,region2=permutation2).\r\n"),

	COMMAND_CALLBACK_REGISTER(director_debug_camera, 1, "<boolean>", "enable/disable camera debugging\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(camera_control, 1, "<boolean>", "toggles script control of the camera.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(camera_set_mode, 2, "<user_index> <mode_index>", "sets user's camera perspective\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(debug_camera_save, 0, "", "saves the camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(debug_camera_load, 0, "", "loads the saved camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(crash, 1, "<string>", "crashes (for debugging).\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(status, 0, "", "prints the value of all global status variables.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(player_force_mode, 1, "<string_id>", "force your will upon the player\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(test_download_storage_file, 2, "<string> <string>", "<url> <filename> downloads a file from LSP to the client\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(lsp_info_get, 0, "", "gets the LSP server info\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(lsp_info_set, 1, "<string>", "<ip:port> sets the LSP server address and port\r\nNETWORK SAFE: Unknown, assumed unsafe"),
};

extern void command_tokenize(char const* input, tokens_t& tokens, long* token_count);
extern long token_try_parse_bool(token_t const& token);

extern s_remote_command_globals remote_command_globals;

extern void patch_remote_command();

extern void __cdecl remote_command_initialize();
extern void __cdecl remote_command_dispose();
extern bool __cdecl remote_command_connected();
extern void __cdecl remote_command_disconnect();
extern void __cdecl remote_command_process();
extern bool __cdecl remote_command_process_received_chunk(char const* buffer, long buffer_length);
extern bool __cdecl remote_command_send_encoded(long encoded_command_size, void const* encoded_command_buffer, long payload_size, void const* payload);
extern bool __cdecl remote_command_send(long command_type, void const* a2, long payload_size, void const* payload);
extern bool __cdecl remote_camera_update(long user_index, s_observer_result const* camera);

