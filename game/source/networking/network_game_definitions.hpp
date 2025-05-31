#pragma once

struct c_bitstream;

struct s_player_configuration_from_client;
struct s_network_squad_status_data;

bool __cdecl player_configuration_client_decode(c_bitstream* packet, s_player_configuration_from_client* player_from_client, int32 encoding_type);
void __cdecl player_configuration_client_encode(c_bitstream* packet, const s_player_configuration_from_client* player_from_client, int32 encoding_type);
bool __cdecl squad_status_decode(c_bitstream* packet, const s_network_squad_status_data* status_data);
void __cdecl squad_status_encode(c_bitstream* packet, const s_network_squad_status_data* status_data);

