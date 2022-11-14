#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_player_traits.hpp"
#include "memory/data.hpp"

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

struct s_s3d_player_armor_configuration_loadout
{
	c_static_array<rgb_color, 5> colors;
	c_static_array<byte, 7> armors;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	// using this byte for our implementation
	bool armor_is_set;
};
static_assert(sizeof(s_s3d_player_armor_configuration_loadout) == 0x20);

struct s_s3d_player_armor_configuration
{
	byte_flags flags;
	byte : 8;
	byte : 8;
	byte : 8;

	dword loadout_index;
	c_static_array<s_s3d_player_armor_configuration_loadout, 5> loadouts;

	c_static_array<c_static_array<dword, 93>, 5> __unknownA8;
};
static_assert(sizeof(s_s3d_player_armor_configuration) == 0x7EC);

enum e_grenade_type
{
	_grenade_type_frag = 0,
	_grenade_type_plasma,
	_grenade_type_claymore,
	_grenade_type_firebomb,

	k_grenade_type_count
};

struct s_s3d_player_weapon_configuration_loadout
{
	byte primary_weapon_index;
	byte secondary_weapon_index;
	c_enum<e_grenade_type, char, k_grenade_type_count> grenade_index;
	c_static_array<char, 4> consumables;

	// padding?
	byte __unknown7;
};
static_assert(sizeof(s_s3d_player_weapon_configuration_loadout) == 0x8);

struct s_s3d_player_weapon_configuration
{
	bool __unknown0;
	byte : 8;
	byte : 8;
	byte : 8;

	dword loadout_index;
	c_static_array<s_s3d_player_weapon_configuration_loadout, 5> loadouts;

	c_static_array<c_static_array<dword, 93>, 5> __unknown30;
};
static_assert(sizeof(s_s3d_player_weapon_configuration) == 0x774);

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
	s_s3d_player_armor_configuration armor_configuration;
	s_s3d_player_weapon_configuration weapon_configuration;
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

struct s_network_session_status_data_player
{
	s_player_identifier identifier;
	s_player_configuration configuration;
};
static_assert(sizeof(s_network_session_status_data_player) == 0x1628);

struct game_player_options
{
	bool player_valid;
	bool player_left_game;
	short user_index;
	long controller_index;
	s_machine_identifier machine_identifier;
	s_network_session_status_data_player player_data;
};
static_assert(sizeof(game_player_options) == 0x1640);

struct player_datum : s_datum_header
{
	short __unknown2;
	dword_flags player_flags;
	s_player_identifier player_identifier;
	dword __unknown10;
	s_machine_identifier machine_identifier;
	short machine_index;
	short machine_user_index;
	long player_index;
	s_location location;
	datum_index unit_index;
	datum_index dead_unit_index;
	datum_index previous_unit_index;
	dword __unknown3C;
	word __unknown40;
	word __unknown42;
	word __unknown44;
	word __unknown46;
	dword __unknown48;
	long active_armor_loadout;
	long active_weapon_loadout;
	byte __unknown54;
	byte __unknown55;
	byte __unknown56;
	bool __unknown57;
	bool __unknown58;
	bool __unknown59;
	bool __unknown5A;
	byte __unknown5B;
	word __unknown5C;
	byte __unknown5E;
	byte __unknown5F;
	byte __unknown60;
	byte __unknown61;
	word __unknown62;
	real_vector3d position;
	s_player_configuration configuration;
	s_player_configuration desired_configuration;
	long __unknown2CB0;
	byte unknown2CB0[0x4];
	long respawn_time;
	long spawn_timer;
	long penalty_time;
	long __unknown2CC4;
	long __unknown2CC8;
	long __unknown2CCC;
	long __unknown2CD0;
	long __unknown2CD4;
	long __unknown2CD8;
	byte __data2CDC[0xC4];
	short unknown2DA0;
	short lives_per_round;
	byte __data2DA4[0x8];
	c_player_traits player_traits;

	// count is number of powerup flavors, red, blue and yellow
	dword powerup_pickup_time[3];

	byte __data2DD4[0x134];
};
static_assert(sizeof(player_datum) == 0x2F08);

struct players_globals
{
	byte __data[0x234];
};
static_assert(sizeof(players_globals) == 0x234);

extern long __cdecl players_first_active_user();
extern bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position);
extern s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player);
extern s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player);

