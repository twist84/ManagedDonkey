#pragma once

#include "cseries/cseries.hpp"

struct s_machine_identifier
{
	long parts[4];
};
static_assert(sizeof(s_machine_identifier) == 0x10);

struct s_player_identifier
{
	// make_int64(ip, port)
	dword ip_addr;
	word port;
	short : 16;
};
static_assert(sizeof(s_player_identifier) == 0x8);

struct s_player_appearance
{
	byte_flags flags;
	byte player_model_choice;
	byte : 8;
	byte : 8;

	struct
	{
		byte __data[0x650];
	} __unknown4;

	wchar_t service_tag[5];
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_player_appearance) == 0x660);

struct s_s3d_player_armor_data
{
	byte_flags flags;
	byte : 8;
	byte : 8;
	byte : 8;
	dword loadout_index;

	struct
	{
		dword colors[5];
		byte armor[10];
		byte : 8;
		byte : 8;
	} __unknown8[5];

	dword __unknownA8[5][93];
};
static_assert(sizeof(s_s3d_player_armor_data) == 0x7EC);

struct s_s3d_player_weapon_data
{
	bool __unknown0;
	byte : 8;
	byte : 8;
	byte : 8;
	dword loadout_index;

	struct
	{
		byte __unknown0;
		byte __unknown1;
		byte __unknown2;
		byte __unknown3[4];
		byte __unknown7;
	} __unknown8[5];

	dword __unknown30[5][93];
};
static_assert(sizeof(s_s3d_player_weapon_data) == 0x774);

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
	long team;
	long player_assigned_team;
	s_player_appearance appearance;
	s_s3d_player_armor_data armor_data;
	s_s3d_player_weapon_data weapon_data;
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