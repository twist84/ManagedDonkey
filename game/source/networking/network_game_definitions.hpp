#pragma once

struct c_bitstream;
struct s_network_session_status_data;
bool __cdecl squad_status_decode(c_bitstream* packet, s_network_session_status_data const* status_data);
void __cdecl squad_status_encode(c_bitstream* packet, s_network_session_status_data const* status_data);

