#pragma once

struct transport_endpoint;
struct s_remote_command_globals
{
    transport_endpoint* send_endpoint;
    transport_endpoint* receive_endpoint;
    transport_endpoint* listen_endpoint;

    long reception_packet_total_size;
    long unknown0;
    long reception_packet_received;

};

extern s_remote_command_globals remote_command_globals;

void __cdecl remote_command_disconnect();
bool __cdecl remote_command_process_received_chunk(char const* buffer, long buffer_length);
bool __cdecl remote_camera_update(long user_index, struct s_observer_result const* camera);
void __cdecl remote_command_process();

