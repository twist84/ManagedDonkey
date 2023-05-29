#include "networking/network_game_definitions.hpp"

#include "cseries/cseries.hpp"

bool __cdecl squad_status_decode(c_bitstream* packet, s_network_session_status_data const* status_data)
{
	return INVOKE(0x00443AD0, squad_status_decode, packet, status_data);
}

void __cdecl squad_status_encode(c_bitstream* packet, s_network_session_status_data const* status_data)
{
	INVOKE(0x00443F00, squad_status_encode, packet, status_data);
}
