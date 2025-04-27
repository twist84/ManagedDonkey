#include "networking/network_game_definitions.hpp"

#include "cseries/cseries.hpp"
#include "game/players.hpp"

//bool __cdecl player_configuration_client_decode(c_bitstream* packet, s_player_configuration_from_client* player_from_client, e_player_configuration_encoding_type encoding_type)
bool __cdecl player_configuration_client_decode(c_bitstream* packet, s_player_configuration_from_client* player_from_client, int32 encoding_type)
{
	return INVOKE(0x004432E0, player_configuration_client_decode, packet, player_from_client, encoding_type);
}

//void __cdecl player_configuration_client_encode(c_bitstream* packet, s_player_configuration_from_client const* player_from_client, e_player_configuration_encoding_type encoding_type)
void __cdecl player_configuration_client_encode(c_bitstream* packet, s_player_configuration_from_client const* player_from_client, int32 encoding_type)
{
	INVOKE(0x004433C0, player_configuration_client_encode, packet, player_from_client, encoding_type);
}

bool __cdecl squad_status_decode(c_bitstream* packet, s_network_squad_status_data const* status_data)
{
	return INVOKE(0x00443AD0, squad_status_decode, packet, status_data);
}

void __cdecl squad_status_encode(c_bitstream* packet, s_network_squad_status_data const* status_data)
{
	INVOKE(0x00443F00, squad_status_encode, packet, status_data);
}