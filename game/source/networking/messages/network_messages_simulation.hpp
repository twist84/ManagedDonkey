#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct s_network_message_view_establishment
{
	c_enum<e_simulation_view_establishment_mode, int32, _simulation_view_establishment_mode_none, k_simulation_view_establishment_mode_count> establishment_mode;
	int32 establishment_identifier;

	bool signature_exists;
	int32 signature_size;
	byte signature_data[0x3C];
};
COMPILE_ASSERT(sizeof(s_network_message_view_establishment) == 0x4C);

struct s_network_message_player_acknowledge
{
	uns32 player_valid_mask;
	uns32 player_in_game_mask;
	s_player_identifier player_identifiers[16];
};
COMPILE_ASSERT(sizeof(s_network_message_player_acknowledge) == 0x88);

class c_bitstream;

class c_network_message_view_establishment
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_player_acknowledge
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_type_collection;
extern void __cdecl network_message_types_register_simulation(c_network_message_type_collection* message_collection);

