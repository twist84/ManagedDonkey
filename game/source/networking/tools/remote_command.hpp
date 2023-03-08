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

extern s_remote_command_globals remote_command_globals;

extern void __cdecl remote_command_initialize();
extern void __cdecl remote_command_dispose();
extern bool __cdecl remote_command_connected();
extern void __cdecl remote_command_disconnect();
extern void __cdecl remote_command_process();
extern bool __cdecl remote_command_process_received_chunk(char const* buffer, long buffer_length);
extern bool __cdecl remote_command_send_encoded(long encoded_command_size, void const* encoded_command_buffer, long payload_size, void const* payload);
extern bool __cdecl remote_command_send(long command_type, void const* a2, long payload_size, void const* payload);
extern bool __cdecl remote_camera_update(long user_index, s_observer_result const* camera);

