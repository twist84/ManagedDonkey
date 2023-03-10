#include "networking/tools/remote_command.hpp"

#include "donkey_networking/donkey_command_server.hpp"

#include "camera/observer.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game.hpp"
#include "game/game_time.hpp"
#include "game/player_mapping.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/main_game_launch.hpp"
#include "memory/data_packets.hpp"
#include "memory/data_packet_groups.hpp"
#include "memory/module.hpp"
#include "networking/network_time.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

#include <assert.h>

s_remote_command_globals remote_command_globals;

void __cdecl remote_command_transport_shutdown(void*)
{
	remote_command_dispose();
}

void __cdecl remote_command_initialize()
{
	command_server.start(11770);

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
	command_server.stop();
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
			assert(remote_command_globals.receive_endpoint == nullptr);
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

			char buffer[512] = {};
			short buffer_length = transport_endpoint_read(remote_command_globals.receive_endpoint, buffer, sizeof(buffer));

			// If there's no data to read or an error occurred, stop processing
			if (buffer_length <= 0)
			{
				continue_processing = false;
				break;
			}

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

#define REMOTE_COMMAND_SEND_STRING(_string) transport_endpoint_write(remote_command_globals.send_endpoint, _string, NUMBEROF(_string))

#define DECLARE_FUNCTION_AS(_old, _new) decltype(_old)* _new = _old

#define REMOTE_COMMAND_NO_PARAMS(_name, _format) \
else if (strcmp(buffer, #_name) == 0)            \
{                                                \
    c_console::write_line(_format"");            \
    _name();                                     \
    REMOTE_COMMAND_SEND_STRING("test");          \
}

#define REMOTE_COMMAND_LONG(_name, _format)                               \
else if (STARTSWITH(buffer, buffer_length, #_name" "))                    \
{                                                                         \
    long long_value = atol(buffer + csstrnlen(#_name" ", buffer_length)); \
    c_console::write_line(_format" '%d'", long_value);                    \
    _name(long_value);                                                    \
    REMOTE_COMMAND_SEND_STRING("test");                                   \
}

#define REMOTE_COMMAND_STRING(_name, _format)                                  \
else if (STARTSWITH(buffer, buffer_length, #_name" "))                         \
{                                                                              \
    char const* string_value = buffer + csstrnlen(#_name" ", buffer_length);   \
    c_console::write_line(_format" '%s'", string_value);                       \
    _name(string_value);                                                       \
    REMOTE_COMMAND_SEND_STRING("test");                                        \
}

DECLARE_FUNCTION_AS(sub_69D600, enter_pregame);
DECLARE_FUNCTION_AS(user_interface_start_hs_script_by_name, start_hs_script_by_name);
DECLARE_FUNCTION_AS(main_game_launch_set_multiplayer_splitscreen_count, game_splitscreen);
DECLARE_FUNCTION_AS(main_game_launch_set_coop_player_count, game_coop_players);
DECLARE_FUNCTION_AS(main_game_launch, game_start);

bool __cdecl remote_command_process_received_chunk(char const* buffer, long buffer_length)
{
	assert(buffer);
	assert(buffer_length > 0);

	if (strcmp(buffer, "disconnect") == 0)
	{
		return false;
	}
	REMOTE_COMMAND_NO_PARAMS(enter_pregame, "entering pregame lobby")
	REMOTE_COMMAND_STRING(start_hs_script_by_name, "starting script")
	REMOTE_COMMAND_LONG(game_splitscreen, "debug map launching: sets the number of multiplayer splitscreen players for the next map.")
	REMOTE_COMMAND_LONG(game_coop_players, "debug map launching: sets the number of coop players for the next map.")
	REMOTE_COMMAND_STRING(game_start, "debug map launching: starts a game on the specified map.")
	else
	{

	}

	// #TODO: while loop...

	return true;
}

bool __cdecl remote_command_send_encoded(long encoded_command_size, void const* encoded_command_buffer, long payload_size, void const* payload)
{
	// Ensure that the input is valid
	assert((encoded_command_size > 0) && (encoded_command_size <= MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE));
	assert(encoded_command_buffer);
	assert(((payload_size == 0) && (payload == nullptr)) || ((payload_size > 0) && (payload_size <= MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE) && (payload != nullptr)));

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
	assert(remote_command_globals.send_endpoint != nullptr);
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
	assert((command_type >= 0) && (command_type < NUMBER_OF_REMOTE_COMMANDS));

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
	if (/*!game_in_editor() ||*/ user_index != players_first_active_user())
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

