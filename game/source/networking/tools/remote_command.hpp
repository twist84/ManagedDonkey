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

#define COMMAND_CALLBACK_PARAMETER_CHECK \
ASSERT(userdata != nullptr); \
ASSERT((token_count - 1) >= 0); \
 \
const s_command& command = *static_cast<const s_command*>(userdata); \
callback_result_t result = command.name; \
result.append_line(": succeeded"); \
if ((token_count - 1) != command.parameter_count) \
{ \
	result = "Invalid usage. "; \
	result.append_print_line("%s %s", command.name, command.parameter_types); \
	result.append(command.extra_info); \
	return result; \
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
COMMAND_CALLBACK_DECLARE(game_safe_to_save);
COMMAND_CALLBACK_DECLARE(game_safe_to_speak);
COMMAND_CALLBACK_DECLARE(game_all_quiet);
COMMAND_CALLBACK_DECLARE(game_saving);
COMMAND_CALLBACK_DECLARE(game_reverted);
COMMAND_CALLBACK_DECLARE(net_session_create);
COMMAND_CALLBACK_DECLARE(net_session_add);
COMMAND_CALLBACK_DECLARE(net_test_ping_directed);
COMMAND_CALLBACK_DECLARE(net_test_text_chat);
COMMAND_CALLBACK_DECLARE(net_test_text_chat_directed);
COMMAND_CALLBACK_DECLARE(net_test_player_color);
COMMAND_CALLBACK_DECLARE(net_test_variant);
COMMAND_CALLBACK_DECLARE(net_test_session_mode);
COMMAND_CALLBACK_DECLARE(net_test_ui_game_mode);
COMMAND_CALLBACK_DECLARE(net_test_advertisement_mode);
COMMAND_CALLBACK_DECLARE(net_test_game_variant_parameter);
COMMAND_CALLBACK_DECLARE(online_set_is_connected_to_live);
COMMAND_CALLBACK_DECLARE(online_user_set_name);
COMMAND_CALLBACK_DECLARE(cheat_all_chars);
COMMAND_CALLBACK_DECLARE(drop_permutation);
COMMAND_CALLBACK_DECLARE(lsp_info_get);
COMMAND_CALLBACK_DECLARE(lsp_info_set);
COMMAND_CALLBACK_DECLARE(player_ragdoll);
COMMAND_CALLBACK_DECLARE(player_drop_weapon);
COMMAND_CALLBACK_DECLARE(player_add_weapon);
COMMAND_CALLBACK_DECLARE(levels_add_fake_map_solo);
COMMAND_CALLBACK_DECLARE(levels_add_map_solo);
COMMAND_CALLBACK_DECLARE(levels_add_fake_map_multi);
COMMAND_CALLBACK_DECLARE(levels_add_map_multi);

COMMAND_CALLBACK_DECLARE(controller_set_background_emblem_color);
COMMAND_CALLBACK_DECLARE(controller_set_emblem_info);
COMMAND_CALLBACK_DECLARE(controller_set_primary_change_color);
COMMAND_CALLBACK_DECLARE(controller_set_primary_emblem_color);
COMMAND_CALLBACK_DECLARE(controller_set_secondary_change_color);
COMMAND_CALLBACK_DECLARE(controller_set_secondary_emblem_color);
COMMAND_CALLBACK_DECLARE(controller_set_tertiary_change_color);

//-----------------------------------------------------------------------------

s_command const k_registered_commands[] =
{
	COMMAND_CALLBACK_REGISTER(script_start, 1, "<string>", "debug script launching: starts a scenario script by name.\r\nNETWORK SAFE: No, for mainmenu only"),

	COMMAND_CALLBACK_REGISTER(game_safe_to_save, 0, "", "returns FALSE if it would be a bad idea to save the player's game right now\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_safe_to_speak, 0, "", "returns FALSE if it would be a bad idea to play mission dialog right now\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_all_quiet, 0, "", "returns FALSE if there are bad guys around, projectiles in the air, etc.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_saving, 0, "", "checks to see if the game is trying to save the map.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(game_reverted, 0, "", "don't use this for anything, you black-hearted bastards.\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_session_create, 2, "<string> <string>", "<ui_game_mode> <advertisement_mode> creates a session to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_session_add, 1, "<string>", "<ip:port> adds a session from the given ip:port to the local games browser \r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(net_test_ping_directed, 1, "<string>", "<ip:port> network test: sends a ping to a specific address\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_text_chat, 1, "<string>", "<message> network test: sends a message\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_text_chat_directed, 2, "<string> <string>", "<ip:port> <message> network test: sends a message to a specific address\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(net_test_player_color, 1, "<long>", "network test: temporarily sets the color for all local players\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_variant, 1, "<string>", "network test: sets the game variant to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_session_mode, 1, "<string>", "network test: sets the session mode to play\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(net_test_ui_game_mode, 1, "<string>", "network test: sets the ui game mode to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_test_advertisement_mode, 1, "<string>", "network test: sets the advertisement mode to play\r\nNETWORK SAFE: No, for mainmenu only"),
	COMMAND_CALLBACK_REGISTER(net_test_game_variant_parameter, 2, "<string> <long>", "network test: sets a parameter of the current game variant\r\nNETWORK SAFE: No, for mainmenu only"),

	COMMAND_CALLBACK_REGISTER(online_set_is_connected_to_live, 1, "<boolean>", "sets connected to live\r\nNETWORK SAFE: Yes"),
	COMMAND_CALLBACK_REGISTER(online_user_set_name, 2, "<long> <string>", "sets the name of a specific user\r\nNETWORK SAFE: Yes"),

	COMMAND_CALLBACK_REGISTER(drop_permutation, 2, "<string> <string>", "drops the named tag e.g. objects\\characters\\brute\\brute.biped using the specified permutation. permutations are specified as a comma-delimited string of region=permutation pairs (e.g. region1=permutation1,region2=permutation2).\r\n"),

	COMMAND_CALLBACK_REGISTER(lsp_info_get, 0, "", "gets the LSP server info\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(lsp_info_set, 1, "<string>", "<ip:port> sets the LSP server address and port\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(player_ragdoll, 0, "", "ragdolls the players biped.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(player_drop_weapon, 0, "", "drops the players held weapon.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(player_add_weapon, 2, "<string> <long>", "<weapon_definition_index> <weapon_addition_method> gives the player a weapon.\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(levels_add_fake_map_solo, 1, "<string>", "<scenario_path> adds a fake map for campaign\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(levels_add_map_solo, 2, "<long> <string>", "<map_id> <scenario_path> adds a map with the specified map id for campaign\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(levels_add_fake_map_multi, 1, "<string>", "<scenario_path> adds a fake map for multiplayer\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(levels_add_map_multi, 2, "<long> <string>", "<map_id> <scenario_path> adds a map with the specified map id for multiplayer\r\nNETWORK SAFE: Unknown, assumed unsafe"),

	COMMAND_CALLBACK_REGISTER(controller_set_background_emblem_color, 2, "<controller> <player_color>", "set tertiary color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_emblem_info, 3, "<controller> <short> <short>", "set emblem for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_primary_change_color, 2, "<controller> <player_color>", "set primary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_primary_emblem_color, 2, "<controller> <player_color>", "set primary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_secondary_change_color, 2, "<controller> <player_color>", "set secondary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_secondary_emblem_color, 2, "<controller> <player_color>", "set secondary change color for specified controller\r\nNETWORK SAFE: No"),
	COMMAND_CALLBACK_REGISTER(controller_set_tertiary_change_color, 2, "<controller> <player_color>", "set tertiary color for specified controller\r\nNETWORK SAFE: No"),
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

