#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

enum e_simulation_view_establishment_mode
{
	_simulation_view_establishment_mode_none = 0,
	_simulation_view_establishment_mode_unknown1,
	_simulation_view_establishment_mode_unknown2,
	_simulation_view_establishment_mode_unknown3,
	_simulation_view_establishment_mode_joining,
	_simulation_view_establishment_mode_unknown5,

	k_simulation_view_establishment_mode_count
};

struct s_network_message_view_establishment
{
	c_enum<e_simulation_view_establishment_mode, long, _simulation_view_establishment_mode_none, k_simulation_view_establishment_mode_count> establishment_mode;
	long establishment_identifier;

	bool signature_exists;
	long signature_size;
	byte signature_data[0x3C];
};
static_assert(sizeof(s_network_message_view_establishment) == 0x4C);

struct s_network_message_player_acknowledge
{
	dword player_valid_mask;
	dword player_in_game_mask;
	s_player_identifier player_identifiers[16];
};
static_assert(sizeof(s_network_message_player_acknowledge) == 0x88);

struct c_bitstream;

class c_network_message_view_establishment
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_player_acknowledge
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_simulation(c_network_message_type_collection* message_collection);

