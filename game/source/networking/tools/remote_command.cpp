#include "networking/tools/remote_command.hpp"

#include "ai/ai.hpp"
#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "editor/editor_stubs.hpp"
#include "game/cheats.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_scripting.hpp"
#include "game/game_time.hpp"
#include "game/multiplayer_game_hopper.hpp"
#include "game/player_mapping.hpp"
#include "hf2p/hf2p.hpp"
#include "interface/c_controller.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/gui_screens/game_browser/gui_game_browser.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "interface/user_interface_window_manager_debug.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_game_launch.hpp"
#include "memory/data_packet_groups.hpp"
#include "memory/data_packets.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_messages_text_chat.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_globals.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "networking/online/online.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"
#include "objects/multiplayer_game_objects.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "shell/shell.hpp"
#include "sound/game_sound.hpp"
#include "test/test_functions.hpp"
#include "text/font_loading.hpp"
#include "units/bipeds.hpp"
#include "xbox/xnet.hpp"

#include <string>

//HOOK_DECLARE(0x014E2A60, remote_command_dispose);
//HOOK_DECLARE(0x014E2A70, remote_command_initialize);

s_remote_command_globals remote_command_globals;

void __cdecl remote_command_transport_shutdown(void*)
{
	remote_command_dispose();
}

void __cdecl remote_command_dispose()
{
	remote_command_disconnect();
	if (remote_command_globals.listen_endpoint)
	{
		transport_endpoint_delete(remote_command_globals.listen_endpoint);
		remote_command_globals.listen_endpoint = 0;
	}
}

void __cdecl remote_command_initialize()
{
	remote_command_globals.last_camera_sync_milliseconds = network_time_get();
	remote_command_globals.reception_header_size = -1;
	remote_command_globals.connected = false;
	if (!remote_command_globals.listen_endpoint)
	{
		remote_command_globals.listen_endpoint = transport_endpoint_create(_transport_type_tcp);
		if (remote_command_globals.listen_endpoint)
		{
			transport_address listen_address{};
			transport_register_transition_functions(NULL, remote_command_transport_shutdown, NULL, NULL);
			transport_get_listen_address(&listen_address, 1030);
			if (!transport_endpoint_bind(remote_command_globals.listen_endpoint, &listen_address) || !transport_endpoint_listen(remote_command_globals.listen_endpoint))
			{
				event(_event_error, "remote command client couldn't listen for incoming commands");
				transport_endpoint_delete(remote_command_globals.listen_endpoint);
				remote_command_globals.listen_endpoint = NULL;
			}
		}
		else
		{
			event(_event_error, "remote command client couldn't create_transport_endpoint() for incoming commands");
		}
	}
}

bool __cdecl remote_command_connected()
{
	return remote_command_globals.send_endpoint && transport_endpoint_connected(remote_command_globals.send_endpoint);
}

void __cdecl remote_command_disconnect()
{
	// Delete the receive endpoint if it exists
	if (remote_command_globals.receive_endpoint)
	{
		// If the receive and send endpoints are the same, set the send endpoint to NULL
		if (remote_command_globals.send_endpoint == remote_command_globals.receive_endpoint)
		{
			remote_command_globals.send_endpoint = NULL;
		}

		transport_endpoint_delete(remote_command_globals.receive_endpoint);
		remote_command_globals.receive_endpoint = NULL;
	}

	// Delete the send endpoint if it exists
	if (remote_command_globals.send_endpoint)
	{
		transport_endpoint_delete(remote_command_globals.send_endpoint);
		remote_command_globals.send_endpoint = NULL;
	}
}

void command_handler(char* buffer, int32 buffer_length);

void __cdecl remote_command_process()
{
	// Check if there's a pending incoming connection request
	if (remote_command_globals.listen_endpoint && transport_endpoint_readable(remote_command_globals.listen_endpoint))
	{
		transport_endpoint* endpoint = transport_endpoint_accept(remote_command_globals.listen_endpoint);
		if (endpoint)
		{
			// If we're already connected to a remote host, disconnect from it first
			if (remote_command_globals.receive_endpoint)
			{
				event(_event_warning, "### remote connection attempt causing us to drop existing connection to a host");
				remote_command_disconnect();
			}

			// Assign the new connection endpoint as the receive endpoint
			ASSERT(remote_command_globals.receive_endpoint == NULL);
			remote_command_globals.receive_endpoint = endpoint;

			// If the new endpoint is writeable, set it as the send endpoint too
			if (transport_endpoint_writeable(endpoint))
			{
				remote_command_globals.send_endpoint = remote_command_globals.receive_endpoint;
			}

			event(_event_warning, "received a connection from a remote host!");
		}
	}

	// Check if we've lost connection to the remote host
	if (remote_command_globals.send_endpoint && (!transport_endpoint_connected(remote_command_globals.send_endpoint) || !transport_endpoint_writeable(remote_command_globals.send_endpoint)))
	{
		event(_event_warning, "### lost connection to remote xbox");
		remote_command_disconnect();
	}

	// Process incoming data from the remote host
	if (remote_command_globals.receive_endpoint)
	{
		bool continue_processing = true;

		while (continue_processing)
		{
			// Check if the connection has been closed or there's no data to read
			if (!transport_endpoint_connected(remote_command_globals.receive_endpoint) || !transport_endpoint_readable(remote_command_globals.receive_endpoint))
			{
				continue_processing = false;
				break;
			}

			char buffer[4096] = {};
			int16 buffer_length = transport_endpoint_read(remote_command_globals.receive_endpoint, buffer, sizeof(buffer));

			// If there's no data to read or an error occurred, stop processing
			if (buffer_length <= 0)
			{
				continue_processing = false;
				break;
			}

			command_handler(buffer, buffer_length);

			// Process the received data
			if (!remote_command_process_received_chunk(buffer, buffer_length))
			{
				event(_event_error, "remote command client couldn't process received command");

				// Disconnect if there's an error
				event(_event_warning, "### lost connection to remote host");
				remote_command_disconnect();
				continue_processing = false;
				break;
			}

			// If there's no more data to read, stop processing
			if (buffer_length < sizeof(buffer))
			{
				continue_processing = false;
				break;
			}
		}
	}

	// Update remote cameras if the game is in progress
	if (game_in_progress())
	{
		for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
		{
			if (!player_mapping_output_user_is_active(user_index))
			{
				continue;
			}

			if (const s_observer_result* camera = observer_try_and_get_camera(user_index))
			{
				remote_camera_update(user_index, camera);
			}
		}
	}
}

bool __cdecl remote_command_process_received_chunk(const char* buffer, int32 buffer_length)
{
	ASSERT(buffer);
	ASSERT(buffer_length > 0);

	if (csstrcmp(buffer, "disconnect") == 0)
	{
		return false;
	}

	// $TODO while loop...

	return true;
}

bool __cdecl remote_command_send_encoded(int32 encoded_command_size, const void* encoded_command_buffer, int32 payload_size, const void* payload)
{
	// Ensure that the input is valid
	ASSERT((encoded_command_size > 0) && (encoded_command_size <= MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE));
	ASSERT(encoded_command_buffer);
	ASSERT(((payload_size == 0) && (payload == NULL)) || ((payload_size > 0) && (payload_size <= MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE) && (payload != NULL)));

	// Check if the remote command is connected
	if (!remote_command_connected())
	{
		return false;
	}

	// Create a buffer for the encoded packet and construct the packet header
	static char encode_packet[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE + MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE]{};
	int32 header_length = sprintf_s(encode_packet, 32, "%c%s%04d:%04d%c", '>', REMOTE_COMMAND_HEADER_TAG, encoded_command_size + payload_size, payload_size, '/');

	// Copy the encoded command buffer to the encoded packet buffer
	csmemcpy(encode_packet + header_length, encoded_command_buffer, encoded_command_size);

	// If there is a payload, copy it to the encoded packet buffer
	int32 encode_packet_size = encoded_command_size + header_length;
	if (payload_size > 0)
	{
		csmemcpy(encode_packet + encode_packet_size, payload, payload_size);
		encode_packet_size += payload_size;
	}

	// Write the encoded packet to the send endpoint and check for errors
	ASSERT(remote_command_globals.send_endpoint != NULL);
	int16 bytes_written = transport_endpoint_write(remote_command_globals.send_endpoint, encode_packet, static_cast<int16>(encode_packet_size));
	if (bytes_written <= 0)
	{
		// If there was an error, disconnect the remote command
		event(_event_warning, "### lost connection to remote xbox");
		remote_command_disconnect();
	}

	// Return true if the packet was successfully written, false otherwise
	return bytes_written > 0;
}

DATA_PACKET_DEFINITION(remote_command_packet_map_reset,        0x0 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_map_synch,        0x4 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_camera,          0x70 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_add_object,       0xC /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_modify_object,  0x114 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_delete_object,   0x10 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_sound_command,    0x4 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_hs_expression,  0x3FF /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_tag_placement,  0x104 /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_flag_placement, 0x12C /* Halo 3 PC 64-bit size */, 1, /* $TODO add fields */);

data_packet_group_packet remote_command_packets[NUMBER_OF_REMOTE_COMMANDS]
{
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_map_reset),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_map_synch),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_camera),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_add_object),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_modify_object),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_delete_object),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_sound_command),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_hs_expression),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_tag_placement),
	DATA_PACKET_GROUP_PACKET(0, remote_command_packet_flag_placement)
};

DATA_PACKET_GROUP_DEFINITION(remote_command_packets_group, NUMBER_OF_REMOTE_COMMANDS, 1, MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE, MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE, remote_command_packets);

bool __cdecl remote_command_send(int32 command_type, const void* a2, int32 payload_size, const void* payload)
{
	ASSERT((command_type >= 0) && (command_type < NUMBER_OF_REMOTE_COMMANDS));

	if (remote_command_connected())
	{
		int16 encoded_command_size = MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE;
		char encoded_command_buffer[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE]{};

		if (data_packet_group_encode_packet(&remote_command_packets_group, const_cast<void*>(a2), encoded_command_buffer, &encoded_command_size, static_cast<int16>(command_type), REMOTE_COMMAND_PACKET_VERSION))
		{
			return remote_command_send_encoded(encoded_command_size, encoded_command_buffer, payload_size, payload);
		}

		event(_event_error, "remote command couldn't encode packet type %d (%s)", command_type, remote_command_packets_group.packets[command_type].definition->name);
	}

	return false;
}

bool __cdecl remote_camera_update(int32 user_index, const s_observer_result* camera)
{
	// Check if the game is being run in the editor or if the user index is not the first active user.
	if (!game_in_editor() || user_index != player_mapping_first_active_input_user())
	{
		return false;
	}

	// If less than 4 seconds have passed since the last update, store the updated camera information and return false.
	if (network_time_since(remote_command_globals.last_camera_sync_milliseconds) < 4000)
	{
		remote_command_globals.last_camera_synch_state = *camera;
		return false;
	}

	// Send the updated camera information to the remote endpoint.
	bool result = remote_command_send(_remote_command_camera, camera, 0, NULL);

	// Store the current time and updated camera information.
	remote_command_globals.last_camera_sync_milliseconds = network_time_get();
	remote_command_globals.last_camera_synch_state = *camera;

	return result;
}

//-----------------------------------------------------------------------------

void command_tokenize(const char* input, tokens_t& tokens, int32* token_count)
{
	bool in_quotes = false;
	int32 num_chars = strlen(input);
	char current_token[k_token_length] = { 0 };

	for (int i = 0; i < num_chars; i++)
	{
		char c = input[i];

		if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == 0)
		{
			if (in_quotes)
			{
				current_token[strlen(current_token)] = c;
			}
			else if (strlen(current_token) > 0)
			{
				token_t& token = tokens[*token_count] = new _token_t();
				token->set(current_token);
				(*token_count)++;
				csmemset(current_token, 0, k_token_length);
			}
		}
		else if (c == '"')
		{
			if (in_quotes)
			{
				in_quotes = false;
			}
			else
			{
				in_quotes = true;
			}
		}
		else
		{
			current_token[strlen(current_token)] = c;
		}
	}

	if (strlen(current_token) > 0)
	{
		token_t& token = tokens[*token_count] = new _token_t();
		token->set(current_token);
		(*token_count)++;
	}
}

void command_execute(int32 token_count, tokens_t& tokens, int32 command_count, const s_command* commands)
{
	if (token_count == 0)
	{
		return;
	}

	callback_result_t output;

	for (int32 i = 0; i < command_count; i++)
	{
		if (tokens[0]->is_equal(commands[i].name))
		{
			output = commands[i].callback(&commands[i], token_count, tokens);
			output.append_line();
			transport_endpoint_write(remote_command_globals.send_endpoint, output.get_string(), static_cast<int16>(output.length()));
			return;
		}
	}

	output.print_line("Unknown command: '%s'", tokens[0]);
	output.append_line("For a list of command use 'help'");
	output.append_line();
	//output = help_callback(NULL, 1, {});
	transport_endpoint_write(remote_command_globals.send_endpoint, output.get_string(), static_cast<int16>(output.length()));
}

void command_handler(char* buffer, int32 buffer_length)
{
	tokens_t tokens{};
	int32 token_count = 0;
	command_tokenize(buffer, tokens, &token_count);
	command_execute(token_count, tokens, NUMBEROF(k_registered_commands), k_registered_commands);
}

//-----------------------------------------------------------------------------

callback_result_t script_start_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name = tokens[1]->get_string();
	user_interface_start_hs_script_by_name(name);

	return result;
}

callback_result_t game_safe_to_save_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	console_printf("game_safe_to_save: %s", game_safe_to_save() ? "true" : "false");

	return result;
}

callback_result_t game_safe_to_speak_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	console_printf("game_safe_to_speak: %s", game_safe_to_speak() ? "true" : "false");

	return result;
}

callback_result_t game_all_quiet_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	console_printf("game_all_quiet: %s", game_all_quiet() ? "true" : "false");

	return result;
}

callback_result_t game_saving_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	console_printf("game_saving: %s", game_saving() ? "true" : "false");

	return result;
}

callback_result_t game_reverted_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	console_printf("game_state_reverted: %s", game_state_reverted() ? "true" : "false");

	return result;
}

callback_result_t net_session_create_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	//network_test_create_session();

	const char* ui_game_mode_name = tokens[1]->get_string();
	const char* advertisement_mode_name = tokens[2]->get_string();

	network_test_set_ui_game_mode(ui_game_mode_name);
	network_test_set_advertisement_mode(advertisement_mode_name);

	c_static_string<512> invite_string;
	{
		transport_address insecure{};
		transport_secure_address_get_insecure(&insecure);

		static char ip_port[256]{};
		transport_address_to_string(&insecure, NULL, ip_port, 256, true, false);
		invite_string.print("add_session %s", ip_port);
	}

	s_file_reference invite_file{};
	if (file_reference_create_from_path(&invite_file, "invite.txt", false))
	{
		if (file_exists(&invite_file))
			file_delete(&invite_file);

		file_create(&invite_file);

		uns32 error = 0;
		if (file_open(&invite_file, FLAG(_file_open_flag_desired_access_write), &error))
			file_printf(&invite_file, "%s", invite_string.get_string());

		file_close(&invite_file);
	}

	return result;
}

// *this is fine*
template<int32 k_maximum_count>
bool split_host_string_into_parts(c_static_string<k_maximum_count>* str, c_static_string<k_maximum_count>(&parts)[2])
{
	parts[0] = str->get_string();

	int32 index = str->index_of(":");
	if (index == -1)
	{
		return false;
	}

	parts[1] = parts[0].get_offset(index + 1);
	parts[0].set_length(index);

	return true;
}

callback_result_t net_session_add_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const token_t& str = tokens[1];
	c_static_string<256> parts[2];
	if (!split_host_string_into_parts(str, parts))
	{
		parts[1].set("11774");
	}

	const char* host = parts[0].get_string();
	const char* port = parts[1].get_string();

	static transport_address address{};
	csmemset(&address, 0, sizeof(address));
	transport_address_from_host(host, address);
	address.port = static_cast<uns16>(atol(port));

	network_broadcast_search_update_callback = [](transport_address* outgoing_address) -> void
	{
		*outgoing_address = address;
		//network_broadcast_search_update_callback = NULL;
	};
	load_game_browser(k_any_controller, 0, _browse_system_link);

	console_close();

	return result;
}

callback_result_t net_test_ping_directed_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* ip_port = tokens[1]->get_string();
	if (tokens[1]->index_of(":") == -1)
	{
		result = "Invalid usage. ";
		result.append_print_line("%s %s", command.name, command.parameter_types);
		result.append(command.extra_info);
		return result;
	}

	static transport_address address{};
	csmemset(&address, 0, sizeof(address));
	transport_address_from_string(ip_port, address);

	network_test_ping_directed(&address);

	return result;
}

callback_result_t net_test_text_chat_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* text = tokens[1]->get_string();
	network_test_text_chat(text);

	return result;
}

callback_result_t net_test_text_chat_directed_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* ip_port = tokens[1]->get_string();
	const char* text = tokens[2]->get_string();
	if (tokens[1]->index_of(":") == -1)
	{
		result = "Invalid usage. ";
		result.append_print_line("%s %s", command.name, command.parameter_types);
		result.append(command.extra_info);
		return result;
	}

	static transport_address address{};
	csmemset(&address, 0, sizeof(address));
	transport_address_from_string(ip_port, address);

	network_test_text_chat_directed(&address, text);

	return result;
}

callback_result_t net_test_player_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 profile_color_index = (int32)atol(tokens[1]->get_string());
	network_test_set_player_color(profile_color_index);

	return result;
}

callback_result_t net_test_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* game_engine_name = tokens[1]->get_string();
	network_test_set_game_variant(game_engine_name);

	return result;
}

callback_result_t net_test_session_mode_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* session_mode_name = tokens[1]->get_string();
	network_test_set_session_mode(session_mode_name);

	return result;
}

callback_result_t net_test_ui_game_mode_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* ui_game_mode_name = tokens[1]->get_string();
	network_test_set_ui_game_mode(ui_game_mode_name);

	return result;
}

callback_result_t net_test_advertisement_mode_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* advertisement_mode_name = tokens[1]->get_string();
	network_test_set_advertisement_mode(advertisement_mode_name);

	return result;
}

callback_result_t net_test_game_variant_parameter_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* parameter_name = tokens[1]->get_string();
	int32 value = (int32)atol(tokens[2]->get_string());
	int32 old_value = -1;
	network_test_set_game_variant_parameter(parameter_name, value, &old_value);
	result.print("game_variant_parameter:%s: '%d' -> '%d'", parameter_name, old_value, value);

	return result;
}

callback_result_t cheat_all_chars_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	cheat_all_chars();

	return result;
}

callback_result_t drop_permutation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* tag_name = tokens[1]->get_string();
	const char* permutation = tokens[2]->get_string();
	cheat_drop_tag_name_with_permutation_hs(tag_name, permutation);

	return result;
}

callback_result_t player_drop_weapon_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 user_index = player_mapping_first_active_output_user();
	int32 player_index = player_mapping_get_player_by_output_user(user_index);
	if (player_index != NONE)
	{
		player_try_to_drop_weapon(player_index, true);
	}

	return result;
}

callback_result_t player_add_weapon_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* weapon_name = tokens[1]->get_string();
	int32 method = (int32)atol(tokens[2]->get_string());

	int32 weapon_definition_index = tag_loaded(WEAPON_TAG, weapon_name);
	if (!VALID_INDEX(method, 8))
	{
		method = 1;
	}

	int32 user_index = player_mapping_first_active_output_user();
	int32 unit_index = player_mapping_get_unit_by_output_user(user_index);
	if (unit_index != NONE && weapon_definition_index != NONE)
	{
		LOCAL_TAG_RESOURCE_SCOPE_LOCK;
		game_engine_add_starting_weapon_to_player(unit_index, weapon_definition_index, method);
	}

	return result;
}

callback_result_t levels_add_fake_map_solo_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* scenario_path = tokens[1]->get_string();
	levels_add_fake_map_from_scripting(scenario_path);

	return result;
}

callback_result_t levels_add_map_solo_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	e_map_id map_id = (e_map_id)atol(tokens[1]->get_string());
	const char* scenario_path = tokens[2]->get_string();
	levels_add_map_from_scripting(map_id, scenario_path);

	return result;
}

callback_result_t levels_add_fake_map_multi_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* scenario_path = tokens[1]->get_string();
	levels_add_fake_multiplayer_map_from_scripting(scenario_path);

	return result;
}

callback_result_t levels_add_map_multi_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	e_map_id map_id = (e_map_id)atol(tokens[1]->get_string());
	const char* scenario_path = tokens[2]->get_string();
	levels_add_multiplayer_map_from_scripting(map_id, scenario_path);

	return result;
}

callback_result_t controller_set_background_emblem_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 color = (int16)atol(tokens[2]->get_string());
	debug_set_background_emblem_color(controller_index, color);

	return result;
}

callback_result_t controller_set_emblem_info_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 foreground_emblem_index = (int16)atol(tokens[2]->get_string());
	int16 background_emblem_index = (int16)atol(tokens[3]->get_string());
	debug_set_emblem_info(controller_index, foreground_emblem_index, background_emblem_index);

	return result;
}

callback_result_t controller_set_primary_change_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 color = (int16)atol(tokens[2]->get_string());
	debug_set_primary_change_color(controller_index, color);

	return result;
}

callback_result_t controller_set_primary_emblem_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 color = (int16)atol(tokens[2]->get_string());
	debug_set_primary_emblem_color(controller_index, color);

	return result;
}

callback_result_t controller_set_secondary_change_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 color = (int16)atol(tokens[2]->get_string());
	debug_set_secondary_change_color(controller_index, color);

	return result;
}

callback_result_t controller_set_secondary_emblem_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 color = (int16)atol(tokens[2]->get_string());
	debug_set_secondary_emblem_color(controller_index, color);

	return result;
}

callback_result_t controller_set_tertiary_change_color_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 color = (int16)atol(tokens[2]->get_string());
	debug_set_tertiary_change_color(controller_index, color);

	return result;
}

