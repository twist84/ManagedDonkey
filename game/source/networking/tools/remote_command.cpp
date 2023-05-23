#include "networking/tools/remote_command.hpp"

#include "camera/observer.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "editor/editor_stubs.hpp"
#include "game/cheats.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_scripting.hpp"
#include "game/game_time.hpp"
#include "game/multiplayer_game_hopper.hpp"
#include "game/player_mapping.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/console.hpp"
#include "main/main_game_launch.hpp"
#include "memory/data_packets.hpp"
#include "memory/data_packet_groups.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/network_globals.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "networking/online/online.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"
#include "xbox/xnet.hpp"


s_remote_command_globals remote_command_globals;

void patch_remote_command()
{
	patch_pointer({ .address = 0x01655B90 }, remote_command_initialize);
	patch_pointer({ .address = 0x01655B94 }, remote_command_dispose);
}

void __cdecl remote_command_transport_shutdown(void*)
{
	remote_command_dispose();
}

void __cdecl remote_command_initialize()
{
	remote_command_globals.camera_send_time = network_time_get();
	remote_command_globals.reception_header_size = -1;
	remote_command_globals.connected = false;
	if (!remote_command_globals.listen_endpoint)
	{
		remote_command_globals.listen_endpoint = transport_endpoint_create(_transport_type_tcp);
		if (remote_command_globals.listen_endpoint)
		{
			transport_address listen_address{};
			transport_register_transition_functions(nullptr, remote_command_transport_shutdown, nullptr, nullptr);
			transport_get_listen_address(&listen_address, 1030);
			if (!transport_endpoint_bind(remote_command_globals.listen_endpoint, &listen_address) || !transport_endpoint_listen(remote_command_globals.listen_endpoint))
			{
				c_console::write_line("remote command client couldn't listen for incoming commands");
				transport_endpoint_delete(remote_command_globals.listen_endpoint);
				remote_command_globals.listen_endpoint = nullptr;
			}
		}
		else
		{
			c_console::write_line("remote command client couldn't create_transport_endpoint() for incoming commands");
		}
	}
}

void __cdecl remote_command_dispose()
{
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
			remote_command_globals.send_endpoint = NULL;

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

void command_handler(char* buffer, long buffer_length);

HOOK_DECLARE_CALL(0x00505CD5, remote_command_process);
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
				c_console::write_line("### remote connection attempt causing us to drop existing connection to a host");
				remote_command_disconnect();
			}

			// Assign the new connection endpoint as the receive endpoint
			ASSERT(remote_command_globals.receive_endpoint == nullptr);
			remote_command_globals.receive_endpoint = endpoint;

			// If the new endpoint is writeable, set it as the send endpoint too
			if (transport_endpoint_writeable(endpoint))
			{
				remote_command_globals.send_endpoint = remote_command_globals.receive_endpoint;
			}

			c_console::write_line("received a connection from a remote host!");
		}
	}

	// Check if we've lost connection to the remote host
	if (remote_command_globals.send_endpoint && (!transport_endpoint_connected(remote_command_globals.send_endpoint) || !transport_endpoint_writeable(remote_command_globals.send_endpoint)))
	{
		c_console::write_line("### lost connection to remote xbox");
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
			short buffer_length = transport_endpoint_read(remote_command_globals.receive_endpoint, buffer, sizeof(buffer));

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
				c_console::write_line("remote command client couldn't process received command");

				// Disconnect if there's an error
				c_console::write_line("### lost connection to remote host");
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
		for (long user_index = 0; user_index < 4; ++user_index)
		{
			if (players_user_is_active(user_index))
			{
				s_observer_result const* camera = observer_try_and_get_camera(user_index);
				if (camera)
				{
					remote_camera_update(user_index, camera);
				}
			}
		}
	}
}

bool __cdecl remote_command_process_received_chunk(char const* buffer, long buffer_length)
{
	ASSERT(buffer);
	ASSERT(buffer_length > 0);

	if (strcmp(buffer, "disconnect") == 0)
	{
		return false;
	}

	// #TODO: while loop...

	return true;
}

bool __cdecl remote_command_send_encoded(long encoded_command_size, void const* encoded_command_buffer, long payload_size, void const* payload)
{
	// Ensure that the input is valid
	ASSERT((encoded_command_size > 0) && (encoded_command_size <= MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE));
	ASSERT(encoded_command_buffer);
	ASSERT(((payload_size == 0) && (payload == nullptr)) || ((payload_size > 0) && (payload_size <= MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE) && (payload != nullptr)));

	// Check if the remote command is connected
	if (!remote_command_connected())
		return false;

	// Create a buffer for the encoded packet and construct the packet header
	static char encode_packet[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE + MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE]{};
	long header_length = sprintf_s(encode_packet, 32, "%c%s%04d:%04d%c", '>', REMOTE_COMMAND_HEADER_TAG, encoded_command_size + payload_size, payload_size, '/');

	// Copy the encoded command buffer to the encoded packet buffer
	csmemcpy(encode_packet + header_length, encoded_command_buffer, encoded_command_size);

	// If there is a payload, copy it to the encoded packet buffer
	long encode_packet_size = encoded_command_size + header_length;
	if (payload_size > 0)
	{
		csmemcpy(encode_packet + encode_packet_size, payload, payload_size);
		encode_packet_size += payload_size;
	}

	// Write the encoded packet to the send endpoint and check for errors
	ASSERT(remote_command_globals.send_endpoint != nullptr);
	short bytes_written = transport_endpoint_write(remote_command_globals.send_endpoint, encode_packet, static_cast<short>(encode_packet_size));
	if (bytes_written <= 0)
	{
		// If there was an error, disconnect the remote command
		c_console::write_line("### lost connection to remote xbox");
		remote_command_disconnect();
	}

	// Return true if the packet was successfully written, false otherwise
	return bytes_written > 0;
}

DATA_PACKET_DEFINITION(remote_command_packet_map_reset,        0x0 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_map_synch,        0x4 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_camera,          0x70 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_add_object,       0xC /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_modify_object,  0x114 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_delete_object,   0x10 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_sound_command,    0x4 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_hs_expression,  0x3FF /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_tag_placement,  0x104 /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);
DATA_PACKET_DEFINITION(remote_command_packet_flag_placement, 0x12C /* Halo 3 PC 64-bit size */, 1, /* #TODO: add fields */);

data_packet remote_command_packets[NUMBER_OF_REMOTE_COMMANDS]
{
	DATA_PACKET(0, remote_command_packet_map_reset),
	DATA_PACKET(0, remote_command_packet_map_synch),
	DATA_PACKET(0, remote_command_packet_camera),
	DATA_PACKET(0, remote_command_packet_add_object),
	DATA_PACKET(0, remote_command_packet_modify_object),
	DATA_PACKET(0, remote_command_packet_delete_object),
	DATA_PACKET(0, remote_command_packet_sound_command),
	DATA_PACKET(0, remote_command_packet_hs_expression),
	DATA_PACKET(0, remote_command_packet_tag_placement),
	DATA_PACKET(0, remote_command_packet_flag_placement)
};

DATA_PACKET_GROUP_DEFINITION(remote_command_packets_group, NUMBER_OF_REMOTE_COMMANDS, 1, MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE, MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE, remote_command_packets);

bool __cdecl remote_command_send(long command_type, void const* a2, long payload_size, void const* payload)
{
	ASSERT((command_type >= 0) && (command_type < NUMBER_OF_REMOTE_COMMANDS));

	if (remote_command_connected())
	{
		short encoded_command_size = MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE;
		char encoded_command_buffer[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE]{};

		if (data_packet_group_encode_packet(&remote_command_packets_group, const_cast<void*>(a2), encoded_command_buffer, &encoded_command_size, static_cast<short>(command_type), REMOTE_COMMAND_PACKET_VERSION))
		{
			return remote_command_send_encoded(encoded_command_size, encoded_command_buffer, payload_size, payload);
		}

		c_console::write_line("remote command couldn't encode packet type %d (%s)", command_type, remote_command_packets_group.packets[command_type].definition->name);
	}

	return false;
}

bool __cdecl remote_camera_update(long user_index, s_observer_result const* camera)
{
	// Check if the game is being run in the editor or if the user index is not the first active user.
	if (!game_in_editor() || user_index != players_first_active_user())
		return false;

	// If less than 4 seconds have passed since the last update, store the updated camera information and return false.
	if (network_time_since(remote_command_globals.camera_send_time) < 4000)
	{
		remote_command_globals.camera = *camera;
		return false;
	}

	// Send the updated camera information to the remote endpoint.
	bool result = remote_command_send(_remote_command_camera, camera, 0, nullptr);

	// Store the current time and updated camera information.
	remote_command_globals.camera_send_time = network_time_get();
	remote_command_globals.camera = *camera;

	return result;
}

//-----------------------------------------------------------------------------

void command_tokenize(char const* input, tokens_t& tokens, long* token_count)
{
	bool in_quotes = false;
	long num_chars = strlen(input);
	char current_token[k_token_length] = { '\0' };

	for (int i = 0; i < num_chars; i++)
	{
		char c = input[i];

		if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\0')
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
				memset(current_token, '\0', k_token_length);
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

void command_execute(long token_count, tokens_t& tokens, long command_count, s_command const* commands)
{
	if (token_count == 0)
		return;

	callback_result_t output;

	for (long i = 0; i < command_count; i++)
	{
		if (tokens[0]->equals(commands[i].name))
		{
			output = commands[i].callback(&commands[i], token_count, tokens);
			output.append_line();
			transport_endpoint_write(remote_command_globals.send_endpoint, output.get_string(), static_cast<short>(output.length()));
			return;
		}
	}

	output.print_line("Unknown command: '%s'", tokens[0]->get_string());
	output.append_line("For a list of command use 'help'");
	output.append_line();
	//output = help_callback(nullptr, 1, {});
	transport_endpoint_write(remote_command_globals.send_endpoint, output.get_string(), static_cast<short>(output.length()));
}

void command_handler(char* buffer, long buffer_length)
{
	tokens_t tokens{};
	long token_count = 0;
	command_tokenize(buffer, tokens, &token_count);
	command_execute(token_count, tokens, NUMBEROF(k_registered_commands), k_registered_commands);
}

//-----------------------------------------------------------------------------

callback_result_t help_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	ASSERT(token_count >= 1);

	static callback_result_t result;
	if (result.is_empty())
	{
		for (long i = 0; i < NUMBEROF(k_registered_commands); i++)
		{
			s_command const& command = k_registered_commands[i];

			result.append_print("%s ", command.name);
			result.append_line(command.parameter_types && *command.parameter_types != '\0' ? command.parameter_types : "");
			result.append_line(command.extra_info && *command.extra_info != '\0' ? command.extra_info : "");
			result.append_line();
		}
	}

	return result;
}

callback_result_t breakpoint_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* message = tokens.m_storage[1]->get_string();
	c_console::write_line(message);

	if (!IsDebuggerPresent())
		return __FUNCTION__ ": failed, no debugger present";

	__asm { int 3 };

	return result;
}

callback_result_t close_game_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	exit(-1);
}

callback_result_t script_start_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* name = tokens.m_storage[1]->get_string();
	user_interface_start_hs_script_by_name(name);

	return result;
}

callback_result_t game_splitscreen_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	long multiplayer_splitscreen_count = atol(tokens.m_storage[1]->get_string());
	main_game_launch_set_multiplayer_splitscreen_count(multiplayer_splitscreen_count);

	return result;
}

callback_result_t game_coop_players_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	long coop_player_count = atol(tokens.m_storage[1]->get_string());
	main_game_launch_set_coop_player_count(coop_player_count);

	return result;
}

callback_result_t game_start_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* map_name = tokens.m_storage[1]->get_string();
	main_game_launch(map_name);

	return result;
}

callback_result_t net_session_create_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	//network_test_create_session();

	char const* ui_game_mode_name = tokens.m_storage[1]->get_string();
	char const* advertisement_mode_name = tokens.m_storage[2]->get_string();

	network_test_set_ui_game_mode(ui_game_mode_name);
	network_test_set_advertisement_mode(advertisement_mode_name);

	return result;
}

callback_result_t net_test_map_name_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* scenario_path = tokens.m_storage[1]->get_string();
	network_test_set_map_name(scenario_path);

	return result;
}

callback_result_t net_test_variant_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* game_engine_name = tokens.m_storage[1]->get_string();
	network_test_set_game_variant(game_engine_name);

	return result;
}

callback_result_t net_test_session_mode_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* session_mode_name = tokens.m_storage[1]->get_string();
	network_test_set_session_mode(session_mode_name);

	return result;
}

callback_result_t net_test_ui_game_mode_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* ui_game_mode_name = tokens.m_storage[1]->get_string();
	network_test_set_ui_game_mode(ui_game_mode_name);

	return result;
}

callback_result_t net_test_advertisement_mode_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* advertisement_mode_name = tokens.m_storage[1]->get_string();
	network_test_set_advertisement_mode(advertisement_mode_name);

	return result;
}

callback_result_t net_test_game_variant_parameter_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* parameter_name = tokens.m_storage[1]->get_string();
	long value = atol(tokens.m_storage[2]->get_string());
	long old_value = -1;
	network_test_set_game_variant_parameter(parameter_name, value, &old_value);

	result.print("game_variant_parameter:%s: '%d' -> '%d'", parameter_name, old_value, value);

	return result;
}

callback_result_t net_build_game_variant_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* filename = tokens.m_storage[1]->get_string();
	network_build_game_variant(filename);

	return result;
}

callback_result_t net_verify_game_variant_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* filename = tokens.m_storage[1]->get_string();
	network_verify_packed_game_variant_file(filename);

	return result;
}

callback_result_t net_load_and_use_game_variant_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* filename = tokens.m_storage[1]->get_string();
	network_load_and_use_packed_game_variant_file(filename);

	return result;
}

callback_result_t net_build_map_variant_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* filename = tokens.m_storage[1]->get_string();
	network_build_map_variant(filename);

	return result;
}

callback_result_t net_verify_map_variant_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* filename = tokens.m_storage[1]->get_string();
	network_verify_packed_map_variant_file(filename);

	return result;
}

callback_result_t game_export_variant_settings_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* filename = tokens.m_storage[1]->get_string();
	game_engine_dump_variant_settings(filename);

	return result;
}

callback_result_t alert_carry_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	long user_index = atol(tokens.m_storage[1]->get_string());
	if (!VALID_INDEX(user_index, 4))
	{
		result = "Invalid parameter. ";
		result.append_print_line("%s %s", command.name, command.parameter_types);
		result.append(command.extra_info);
		return result;
	}

	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);
	player_control_globals->controls[user_index].alert_carry = !player_control_globals->controls[user_index].alert_carry;

	return result;
}

callback_result_t online_set_is_connected_to_live_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	bool is_connected_to_live = !!atol(tokens.m_storage[1]->get_string());

	online_set_is_connected_to_live(is_connected_to_live);

	return result;
}

callback_result_t online_user_set_name_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* name = tokens.m_storage[1]->get_string();
	c_static_wchar_string<16> name_wide;
	name_wide.print(L"%hs", name);
	online_user_set_name(name_wide.get_string());

	return result;
}

callback_result_t mp_game_won_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	short team = static_cast<short>(atol(tokens.m_storage[1]->get_string()));
	game_engine_game_won(team);

	return result;
}

callback_result_t cheat_all_powerups_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	cheat_all_powerups();

	return result;
}

callback_result_t cheat_all_vehicles_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	cheat_all_vehicles();

	return result;
}

callback_result_t cheat_all_weapons_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	cheat_all_weapons();

	return result;
}

callback_result_t connect_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	char const* ip_port = tokens.m_storage[1]->get_string();
	char const* secure_identifier = tokens.m_storage[2]->get_string();
	char const* secure_address = tokens.m_storage[3]->get_string();

	transport_address address{};
	s_transport_session_description description{};

	transport_address_from_string(ip_port, address);
	transport_secure_identifier_from_string(secure_identifier, description.id);
	transport_secure_address_from_string(secure_address, description.address);

	g_broadcast_port;

	XNetAddEntry(&address, &description.address, &description.id);
	sub_69D600();
	user_interface_join_remote_session(false, _network_session_class_system_link, &description.id, &description.address, &description.key);

	console_close();
	game_time_set_paused(false, _game_time_pause_reason_debug);

	return result;
}

