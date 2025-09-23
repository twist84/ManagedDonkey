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

int32 token_try_parse_bool(const token_t& token)
{
	const char* value = token->get_string();
	switch (string_hash(value))
	{
	case "0"_hash:
	case "false"_hash:
		return 0;
	case "1"_hash:
	case "true"_hash:
		return 1;
	}

	return 2;
}

//-----------------------------------------------------------------------------

bool load_preference(const char* name, const char* value)
{
	// special case
	if (csstricmp(name, "screen_resolution") == 0)
	{
		char width[8]{};
		char height[8]{};
		sscanf_s(value, "%[^x]x%s", width, sizeof(width), height, sizeof(height));

		return global_preference_set(name, atol(width), atol(height));
	}
	else
	{
		if (csstricmp(value, "true") == 0)
		{
			return global_preference_set(name, true);
		}

		if (csstricmp(value, "false") == 0)
		{
			return global_preference_set(name, false);
		}

		if (csstrstr(value, "."))
		{
			return global_preference_set(name, real32(atof(value)));
		}

		return global_preference_set(name, atol(value));
	}

	return false;
}

callback_result_t script_start_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name = tokens[1]->get_string();
	user_interface_start_hs_script_by_name(name);

	return result;
}

callback_result_t game_save_and_quit_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	main_save_and_exit_campaign();

	return result;
}

callback_result_t game_save_unsafe_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	main_save_map();

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

callback_result_t game_save_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	game_save_safe();

	return result;
}

callback_result_t game_save_cancel_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	game_save_cancel();

	return result;
}

callback_result_t game_save_no_timeout_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	game_save_no_timeout();

	return result;
}

callback_result_t game_save_immediate_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	game_save_immediate();

	return result;
}

callback_result_t game_save_cinematic_skip_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	game_save_cinematic_skip();

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

callback_result_t net_test_ping_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	network_test_ping();

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

callback_result_t net_test_map_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* scenario_path = tokens[1]->get_string();
	network_test_set_map_name(scenario_path);

	return result;
}

callback_result_t net_test_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* game_engine_name = tokens[1]->get_string();
	network_test_set_game_variant(game_engine_name);

	return result;
}

callback_result_t net_test_reset_objects_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	network_test_reset_objects();

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

callback_result_t net_build_network_config_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	network_build_network_configuration();

	return result;
}

callback_result_t net_build_game_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_build_game_variant(filename);

	return result;
}

callback_result_t net_verify_game_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_verify_game_variant_file(filename);

	return result;
}

callback_result_t net_load_and_use_game_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_load_and_use_game_variant_file(filename);

	return result;
}

callback_result_t net_verify_packed_game_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_verify_packed_game_variant_file(filename);

	return result;
}

callback_result_t net_load_and_use_packed_game_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_load_and_use_packed_game_variant_file(filename);

	return result;
}

callback_result_t net_build_map_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_build_map_variant(filename);

	return result;
}

callback_result_t net_verify_map_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_verify_map_variant_file(filename);

	return result;
}

callback_result_t net_load_and_use_map_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_load_and_use_map_variant_file(filename);

	return result;
}

callback_result_t net_verify_packed_map_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_verify_packed_map_variant_file(filename);

	return result;
}

callback_result_t net_load_and_use_packed_map_variant_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	network_load_and_use_packed_map_variant_file(filename);

	return result;
}

callback_result_t game_export_variant_settings_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* filename = tokens[1]->get_string();
	game_engine_dump_variant_settings(filename);

	return result;
}

callback_result_t online_set_is_connected_to_live_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 value = token_try_parse_bool(tokens[1]);
	if (value <= 1)
	{
		online_set_is_connected_to_live((bool)value);
	}

	return result;
}

callback_result_t online_user_set_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 user_index = (int32)atol(tokens[1]->get_string());
	const char* name = tokens[2]->get_string();
	c_static_wchar_string<16> name_wide;
	name_wide.print(L"%hs", name);
	online_user_set_name(user_index, name_wide.get_string());

	return result;
}

callback_result_t mp_players_by_team_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 team = (int32)atol(tokens[1]->get_string());
	game_engine_players_by_team(team);

	return result;
}

callback_result_t deterministic_end_game_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	game_engine_finish_game();

	return result;
}

callback_result_t mp_active_player_count_by_team_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 team = (int32)atol(tokens[1]->get_string());
	game_engine_active_player_count_by_team(team);

	return result;
}

callback_result_t mp_respawn_override_timers_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 team = static_cast<int16>(atol(tokens[1]->get_string()));
	game_engine_respawn_override_timers(team);

	return result;
}

callback_result_t mp_ai_allegiance_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 campaign_team = static_cast<int16>(atol(tokens[1]->get_string()));
	int16 team = static_cast<int16>(atol(tokens[2]->get_string()));
	game_engine_ai_scripting_allegiance(campaign_team, team);

	return result;
}

callback_result_t mp_allegiance_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 team_a = static_cast<int16>(atol(tokens[1]->get_string()));
	int16 team_b = static_cast<int16>(atol(tokens[2]->get_string()));
	game_engine_mp_team_allegiance(team_a, team_b);

	return result;
}

callback_result_t mp_object_belongs_to_team_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 object_index = (int32)atol(tokens[1]->get_string());
	int16 team = static_cast<int16>(atol(tokens[2]->get_string()));
	game_engine_give_object_ownership_to_team(object_index, team);

	return result;
}

callback_result_t mp_weapon_belongs_to_team_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 weapon_index = (int32)atol(tokens[1]->get_string());
	int16 team = static_cast<int16>(atol(tokens[2]->get_string()));
	game_engine_give_weapon_ownership_to_team(weapon_index, team);

	return result;
}

callback_result_t mp_debug_goal_object_boundary_geometry_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 value = token_try_parse_bool(tokens[1]);
	if (value <= 1)
	{
		debug_multiplayer_object_boundary_geometry((bool)value);
	}

	return result;
}

callback_result_t load_preferences_from_file_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* preferences_filename = tokens[1]->get_string();

	FILE* preferences_file = NULL;
	if (fopen_s(&preferences_file, preferences_filename, "r") == 0 && preferences_file)
	{
		char buffer[200]{};
		while (fgets(buffer, NUMBEROF(buffer), preferences_file))
		{
			string_terminate_at_first_delimiter(buffer, "\r\n");
			char name[128]{};
			char value[32]{};

			sscanf_s(buffer, "%[^:]: %s", name, sizeof(name), value, sizeof(value));
			if (*name && *value)
				load_preference(name, value);
		}

		fclose(preferences_file);
	}
	else
	{
		result = "Invalid filename. Does the file exist? ";
		result.append_print_line("%s %s", command.name, command.parameter_types);
		result.append(command.extra_info);
		return result;
	}

	return result;
}

callback_result_t load_customization_from_file_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	bool cache_file_has_halo3_armors = false;
	c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& armor_regions = get_armor_regions(_player_model_choice_spartan, &cache_file_has_halo3_armors);

	const char* customization_filename = tokens[1]->get_string();

	FILE* customization_file = NULL;
	if (fopen_s(&customization_file, customization_filename, "r") == 0 && customization_file)
	{
		s_s3d_player_armor_configuration_loadout& armor_loadout = get_armor_loadout();
		s_s3d_player_weapon_configuration_loadout& weapon_loadout = get_weapon_loadout();

		char buffer[200]{};
		while (fgets(buffer, NUMBEROF(buffer), customization_file))
		{
			string_terminate_at_first_delimiter(buffer, "\r\n");

			// consumables
			{
				int32 consumable_index = NONE;
				char consumable_name[32]{};
				if (sscanf_s(buffer, "consumables[%d]: %s", &consumable_index, consumable_name, sizeof(consumable_name)) && (consumable_index != NONE && *consumable_name))
				{
					weapon_loadout.consumables[consumable_index] = static_cast<int8>(multiplayer_universal_data_get_absolute_equipment_block_index(consumable_name));
					continue;
				}
			}

			// colors
			{
				char color_type_name[128]{};
				uns32 rgb_value = NONE;
				if (sscanf_s(buffer, "colors[%[^]]]: #%08X", color_type_name, sizeof(color_type_name), &rgb_value) && (*color_type_name && rgb_value != NONE))
				{
					int32 index = NONE;
					if (csstricmp(color_type_name, "primary") == 0)
					{
						index = _color_type_primary;
					}
					else if (csstricmp(color_type_name, "secondary") == 0)
					{
						index = _color_type_secondary;
					}
					else if (csstricmp(color_type_name, "visor") == 0)
					{
						index = _color_type_visor;
					}
					else if (csstricmp(color_type_name, "lights") == 0)
					{
						index = _color_type_lights;
					}
					else if (csstricmp(color_type_name, "holo") == 0 && !cache_file_has_halo3_armors)
					{
						index = _color_type_holo;
					}

					if (VALID_INDEX(index, k_color_type_count))
						armor_loadout.colors[index].value = rgb_value;

					continue;
				}
			}

			// armors
			{
				char armor_region_name[128]{};
				char armor_name[32]{};
				if (sscanf_s(buffer, "armors[%[^]]]: %s", armor_region_name, sizeof(armor_region_name), armor_name, sizeof(armor_name)) && (*armor_region_name && *armor_name))
				{
					int32 armor_region_count = cache_file_has_halo3_armors ? _armor_type_arms + 1 : armor_loadout.armors.get_count();
					for (int32 armor_region_index = 0; armor_region_index < armor_region_count; armor_region_index++)
					{
						const char* armor_region = NULL;
						switch (armor_region_index)
						{
						case _armor_type_helmet:
							armor_region = "helmet";
							break;
						case _armor_type_chest:
							armor_region = "chest";
							break;
						case _armor_type_shoulders:
							armor_region = cache_file_has_halo3_armors ? "rightshoulder" : "shoulders";
							break;
						case _armor_type_arms:
							armor_region = cache_file_has_halo3_armors ? "leftshoulder" : "arms";
							break;
						case _armor_type_legs:
							armor_region = "legs";
							break;
						case _armor_type_acc:
							armor_region = "acc";
							break;
						case _armor_type_pelvis:
							armor_region = "pelvis";
							break;
						}

						if (armor_region && csstricmp(armor_region, armor_region_name) == 0)
						{
							armor_loadout.armors[armor_region_index] = static_cast<uns8>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(armor_region, armor_name));
						}
					}
				}
			}
		}

		fclose(customization_file);
	}
	else
	{
		FILE* customization_info_file = NULL;
		if (fopen_s(&customization_info_file, "customization_info.txt", "w") == 0 && customization_info_file)
		{
			fprintf_s(customization_info_file, "This file serves as a reference to what a customization file contains\n\n");

			fprintf_s(customization_info_file, "Example usage:\n\n");

			if (cache_file_has_halo3_armors)
			{
				fprintf_s(customization_info_file, "consumables[<consumable_index(0 - 3)>]: <consumable_name>\n");
				fprintf_s(customization_info_file, "consumables[0]: empty\n");
				fprintf_s(customization_info_file, "consumables[1]: empty\n");
				fprintf_s(customization_info_file, "consumables[2]: empty\n");
				fprintf_s(customization_info_file, "consumables[3]: empty\n\n");
			}

			fprintf_s(customization_info_file, "colors[<color_type_name>]: #RGB\n");
			fprintf_s(customization_info_file, "colors[primary]: #FF0000\n");
			fprintf_s(customization_info_file, "colors[secondary]: #0000FF\n");
			fprintf_s(customization_info_file, "colors[visor]: #0000FF\n");
			fprintf_s(customization_info_file, "colors[lights]: #00F00F\n");

			if (!cache_file_has_halo3_armors)
				fprintf_s(customization_info_file, "colors[holo]: #0F0F00\n\n");

			if (cache_file_has_halo3_armors)
			{
				fprintf_s(customization_info_file, "armors[<armor_region_name>]: <armor_name>\n");
				fprintf_s(customization_info_file, "armors[helmet]: base\n");
				fprintf_s(customization_info_file, "armors[chest]: base\n");
				fprintf_s(customization_info_file, "armors[rightshoulder]: base\n");
				fprintf_s(customization_info_file, "armors[leftshoulder]: base\n");
			}
			else
			{
				fprintf_s(customization_info_file, "armors[<armor_region_name>]: <armor_name>\n");
				fprintf_s(customization_info_file, "armors[helmet]: base\n");
				fprintf_s(customization_info_file, "armors[chest]: base\n");
				fprintf_s(customization_info_file, "armors[shoulders]: base\n");
				fprintf_s(customization_info_file, "armors[arms]: base\n");
				fprintf_s(customization_info_file, "armors[legs]: base\n");
				fprintf_s(customization_info_file, "armors[acc]: flashlight\n");
				fprintf_s(customization_info_file, "armors[pelvis]: base\n\n");
			}

			if (!cache_file_has_halo3_armors)
			{
				fprintf_s(customization_info_file, "consumable:\n");
				fprintf_s(customization_info_file, "\tempty\n");
				fprintf_s(customization_info_file, "\tjammer\n");
				fprintf_s(customization_info_file, "\tpowerdrain\n");
				fprintf_s(customization_info_file, "\tinvisibility\n");
				fprintf_s(customization_info_file, "\tinvisibility_vehicle\n");
				fprintf_s(customization_info_file, "\tbubbleshield\n");
				fprintf_s(customization_info_file, "\tsuperflare\n");
				fprintf_s(customization_info_file, "\tregenerator\n");
				fprintf_s(customization_info_file, "\ttripmine\n");
				fprintf_s(customization_info_file, "\tauto_turret\n");
				fprintf_s(customization_info_file, "\tdeployable_cover\n");
				fprintf_s(customization_info_file, "\tforced_reload\n");
				fprintf_s(customization_info_file, "\tconcussive_blast\n");
				fprintf_s(customization_info_file, "\ttank_mode\n");
				fprintf_s(customization_info_file, "\tmag_pulse\n");
				fprintf_s(customization_info_file, "\thologram\n");
				fprintf_s(customization_info_file, "\treactive_armor\n");
				fprintf_s(customization_info_file, "\tbomb_run\n");
				fprintf_s(customization_info_file, "\tarmor_lock\n");
				fprintf_s(customization_info_file, "\tadrenaline\n");
				fprintf_s(customization_info_file, "\tlightning_strike\n");
				fprintf_s(customization_info_file, "\tscrambler\n");
				fprintf_s(customization_info_file, "\tweapon_jammer\n");
				fprintf_s(customization_info_file, "\tammo_pack\n");
				fprintf_s(customization_info_file, "\tconsumable_vision\n");
				fprintf_s(customization_info_file, "\tbubbleshield_tutorial\n");
				fprintf_s(customization_info_file, "\tconsumable_vision_tutorial\n\n");
			}

			int32 armor_region_count = cache_file_has_halo3_armors ? _armor_type_arms + 1 : k_armor_type_count;
			for (int32 armor_region_index = 0; armor_region_index < armor_region_count; armor_region_index++)
			{
				c_static_array<c_static_string<64>, 100>& armor_types = armor_regions[armor_region_index];

				const char* armor_region = NULL;
				switch (armor_region_index)
				{
				case _armor_type_helmet:
					armor_region = "helmet";
					break;
				case _armor_type_chest:
					armor_region = "chest";
					break;
				case _armor_type_shoulders:
					armor_region = cache_file_has_halo3_armors ? "rightshoulder" : "shoulders";
					break;
				case _armor_type_arms:
					armor_region = cache_file_has_halo3_armors ? "leftshoulder" : "arms";
					break;
				case _armor_type_legs:
					armor_region = "legs";
					break;
				case _armor_type_acc:
					armor_region = "acc";
					break;
				case _armor_type_pelvis:
					armor_region = "pelvis";
					break;
				}

				if (armor_region)
				{
					fprintf_s(customization_info_file, "%s:\n", armor_region);
					for (int32 armor_type_index = 0; armor_type_index < armor_types.get_count(); armor_type_index++)
					{
						const char* value = armor_types[armor_type_index].get_string();
						if (*value)
						{
							fprintf_s(customization_info_file, "\t%s\n", value);
						}
					}
					fprintf_s(customization_info_file, "\n");
				}
			}

			fclose(customization_info_file);
		}

		result = "Invalid filename. Does the file exist? ";
		result.append_print_line("%s %s", command.name, command.parameter_types);
		result.append(command.extra_info);
		return result;
	}

	return result;
}

callback_result_t cheat_all_chars_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	cheat_all_chars();

	return result;
}

callback_result_t debug_menu_rebuild_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	g_debug_menu_rebuild_request = true;

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

callback_result_t director_debug_camera_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 value = token_try_parse_bool(tokens[1]);
	if (value <= 1)
	{
		director_debug_camera((bool)value);
	}

	return result;
}

callback_result_t camera_control_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 value = token_try_parse_bool(tokens[1]);
	if (value <= 1)
	{
		director_script_camera((bool)value);
	}

	return result;
}

callback_result_t camera_set_mode_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 user_index = (int32)atol(tokens[1]->get_string());
	e_camera_mode camera_mode = static_cast<e_camera_mode>(atol(tokens[2]->get_string()));
	if (user_index != NONE && VALID_INDEX(camera_mode, 4))
	{
		director_set_camera_mode(user_index, camera_mode);
	}

	return result;
}

callback_result_t debug_camera_save_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	director_save_camera();

	return result;
}

callback_result_t debug_camera_load_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	director_load_camera();

	return result;
}

callback_result_t crash_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* type = tokens[1]->get_string();
	main_crash(type);

	return result;
}

callback_result_t status_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	main_status_print();

	return result;
}

callback_result_t player_force_mode_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* desired_mode_string = tokens[1]->get_string();
	int32 desired_mode = string_id_retrieve(desired_mode_string);

	player_override_desired_mode(desired_mode);

	return result;
}

//test_download_storage_file /storage/test.txt test/storage/test.txt
callback_result_t test_download_storage_file_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* url = tokens[1]->get_string();
	const char* filename = tokens[2]->get_string();
	test_download_storage_file(url, filename);

	return result;
}

callback_result_t lsp_info_get_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	union
	{
		int32 ip_address = 0;
		uns8 ina[4];
	};

	uns16 port = 0;
	online_lsp_get_info(&ip_address, &port);
	console_printf_color(global_real_argb_cyan, "%hd.%hd.%hd.%hd:%hd", ina[3], ina[2], ina[1], ina[0], port);

	return result;
}

callback_result_t lsp_info_set_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const token_t& str = tokens[1];
	c_static_string<256> parts[2];

	split_host_string_into_parts(str, parts);
	const char* host = parts[0].get_string();
	const char* port = parts[1].get_string();
	online_lsp_set_info(host, port);

	return result;
}

callback_result_t player_ragdoll_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 user_index = player_mapping_first_active_output_user();
	int32 unit_index = player_mapping_get_unit_by_output_user(user_index);
	if (unit_index != NONE)
	{
		biped_scripting_ragdoll(unit_index);
	}

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

callback_result_t ui_debug_text_font_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 value = token_try_parse_bool(tokens[1]);
	if (value <= 1)
	{
		user_interface_text_debug_display_font_index((bool)value);
	}

	return result;
}

callback_result_t xoverlapped_debug_render_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 value = token_try_parse_bool(tokens[1]);
	if (value <= 1)
	{
		overlapped_task_toggle_debug_rendering((bool)value);
	}

	return result;
}

callback_result_t overlapped_display_task_descriptions_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	overlapped_task_display_task_descriptions();

	return result;
}

callback_result_t overlapped_task_inject_error_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* context = tokens[1]->get_string();
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		overlapped_task_inject_error(context, (bool)value);
	}

	return result;
}

callback_result_t overlapped_task_pause_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* context = tokens[1]->get_string();
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		overlapped_task_pause(context, (bool)value);
	}

	return result;
}

callback_result_t sound_loop_spam_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	scripting_looping_sound_spam();

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

callback_result_t controller_set_button_preset_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 presest = (int16)atol(tokens[2]->get_string());
	debug_set_button_preset(controller_index, presest);

	return result;
}

callback_result_t controller_set_auto_center_look_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		debug_set_controller_auto_center_look(controller_index, (bool)value);
	}

	return result;
}

callback_result_t controller_set_crouch_lock_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		debug_set_controller_crouch_lock(controller_index, (bool)value);
	}

	return result;
}

callback_result_t controller_set_flight_stick_aircraft_controls_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		debug_set_controller_flight_stick_aircraft_controls(controller_index, (bool)value);
	}

	return result;
}

callback_result_t controller_set_look_inverted_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		debug_set_controller_look_inverted(controller_index, (bool)value);
	}

	return result;
}

callback_result_t controller_set_vibration_enabled_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 value = token_try_parse_bool(tokens[2]);
	if (value <= 1)
	{
		debug_set_controller_vibration_enabled(controller_index, (bool)value);
	}

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

callback_result_t controller_set_joystick_preset_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 preset = (int16)atol(tokens[2]->get_string());
	debug_set_joystick_preset(controller_index, preset);

	return result;
}

callback_result_t controller_set_look_sensitivity_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 sensitivity = (int16)atol(tokens[2]->get_string());
	debug_set_look_sensitivity(controller_index, sensitivity);

	return result;
}

callback_result_t controller_set_player_character_type_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 type = (int16)atol(tokens[2]->get_string());
	debug_set_player_character_type(controller_index, type);

	return result;
}

callback_result_t controller_set_popup_message_index_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 message_index = (int32)atol(tokens[2]->get_string());
	debug_set_popup_message_index(controller_index, message_index);

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

callback_result_t controller_set_single_player_level_completed_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int32 level_index = (int32)atol(tokens[2]->get_string());

	int32 coop = token_try_parse_bool(tokens[3]);
	if (coop <= 1)
	{
		int16 difficulty = (int16)atol(tokens[4]->get_string());
		int32 completed = token_try_parse_bool(tokens[5]);
		if (completed <= 1)
		{
			debug_set_single_player_level_completed(controller_index, level_index, (bool)coop, difficulty, (bool)completed);
		}
	}

	return result;
}

callback_result_t controller_set_single_player_level_unlocked_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 level_index = (int16)atol(tokens[2]->get_string());
	int32 completed = token_try_parse_bool(tokens[3]);
	if (completed <= 1)
	{
		debug_set_single_player_level_unlocked(controller_index, level_index, (bool)completed);
	}

	return result;
}

callback_result_t controller_set_subtitle_setting_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 setting = (int16)atol(tokens[2]->get_string());
	debug_set_subtitle_setting(controller_index, setting);

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

callback_result_t controller_set_voice_mask_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 mask = (int16)atol(tokens[2]->get_string());
	debug_set_voice_mask(controller_index, mask);

	return result;
}

callback_result_t controller_set_voice_output_setting_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	int16 setting = (int16)atol(tokens[2]->get_string());
	debug_set_voice_output_setting(controller_index, setting);
	
	return result;
}

callback_result_t gui_debug_bitmap_animation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_bitmap_animation_state(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_bitmap_bounds_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_bitmap_bounds(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_bitmap_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_bitmap_display_name(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_bitmap_rotation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_bitmap_rotation(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_group_animation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_group_animation_state(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_group_bounds_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_group_bounds(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_group_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_group_display_name(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_group_rotation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_group_rotation(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_animation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_animation_state(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_bounds_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_bounds(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_display_name(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_item_animation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_item_animation_state(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_item_bounds_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_item_bounds(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_item_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_item_display_name(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_item_rotation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_item_rotation(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_list_rotation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_list_rotation(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_screen_animation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_screen_animation_state(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_screen_bounds_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_screen_bounds(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_screen_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_screen_display_name(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_screen_rotation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	int32 include_children = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1 && include_children <= 1)
	{
		gui_debug_screen_rotation(name, (bool)activate, (bool)include_children);
	}

	return result;
}

callback_result_t gui_debug_text_animation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_text_animation_state(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_text_bounds_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_text_bounds(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_text_name_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_text_display_name(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_debug_text_rotation_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* name_string = tokens[1]->get_string();
	int32 name = string_id_retrieve(name_string);
	int32 activate = token_try_parse_bool(tokens[2]);
	if (name != _string_id_invalid && activate <= 1)
	{
		gui_debug_text_rotation(name, (bool)activate);
	}

	return result;
}

callback_result_t gui_print_active_screen_strings_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	gui_print_active_screen_strings_tag_name();

	return result;
}

callback_result_t gui_print_active_screens_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	gui_print_active_screens();

	return result;
}

callback_result_t saved_film_play_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int16 controller_index = (int16)atol(tokens[1]->get_string());
	const char* film_name = tokens[2]->get_string();
	saved_film_manager_play_hs(controller_index, film_name);

	return result;
}

callback_result_t saved_film_play_last_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	saved_film_manager_play_last_hs();

	return result;
}

callback_result_t saved_film_disable_version_checking_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 disable = token_try_parse_bool(tokens[1]);
	if (disable <= 1)
	{
		saved_film_manager_disable_version_checking((bool)disable);
	}

	return result;
}

callback_result_t saved_film_toggle_debug_saving_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 enable = token_try_parse_bool(tokens[1]);
	if (enable <= 1)
	{
		saved_film_manager_toggle_automatic_debug_saving((bool)enable);
	}

	return result;
}

callback_result_t saved_films_delete_on_level_load_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 delete_on_level_load = token_try_parse_bool(tokens[1]);
	if (delete_on_level_load <= 1)
	{
		saved_film_manager_delete_on_level_load((bool)delete_on_level_load);
	}

	return result;
}

callback_result_t saved_films_show_timestamp_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 show_timestamp = token_try_parse_bool(tokens[1]);
	if (show_timestamp <= 1)
	{
		saved_film_manager_show_timestamp((bool)show_timestamp);
	}

	return result;
}

callback_result_t saved_film_manager_should_record_film_default_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 b = token_try_parse_bool(tokens[1]);
	if (b <= 1)
	{
		saved_film_manager_should_record_film_default_set((bool)b);
	}

	return result;
}

callback_result_t saved_film_set_playback_game_speed_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	real32 game_speed = (real32)atof(tokens[1]->get_string());
	saved_film_manager_set_playback_game_speed(game_speed);

	return result;
}

callback_result_t saved_film_set_pending_playback_game_speed_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	real32 game_speed = (real32)atof(tokens[1]->get_string());
	saved_film_manager_set_pending_playback_game_speed(game_speed);

	return result;
}

callback_result_t saved_film_seek_to_film_tick_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	int32 film_tick = (int32)atol(tokens[1]->get_string());
	saved_film_manager_seek_to_film_tick_hs(film_tick);

	return result;
}

