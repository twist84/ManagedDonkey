#pragma once

#include "cseries/cseries.hpp"

struct s_machine_identifier
{
	long parts[4];
};
static_assert(sizeof(s_machine_identifier) == 0x10);

struct s_player_identifier
{
	long parts[2];
};
static_assert(sizeof(s_player_identifier) == 0x8);

struct s_player_configuration_from_client
{
	wchar_t player_name[16];

	byte __data[0x10];
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_identifier machine_identifier;
	wchar_t player_name[16];

	byte __data[0x15C8];
};
static_assert(sizeof(s_player_configuration_from_host) == 0x15F0);

struct s_player_configuration
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0x1620);

#pragma pack(push, 1)
struct game_machine_options
{
	dword valid_machine_mask;
	s_machine_identifier machines[17];

	bool local_machine_exists;
	s_machine_identifier local_machine;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(game_machine_options) == 0x128);
#pragma pack(pop)

struct game_player_options
{
	byte __data[0x1640];
};
static_assert(sizeof(game_player_options) == 0x1640);

extern long __cdecl players_first_active_user();
extern bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position);