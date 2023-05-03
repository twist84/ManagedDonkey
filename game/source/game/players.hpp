#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/game_engine_player_traits.hpp"
#include "memory/data.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

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

	// online_xuid_is_guest_account
	// 0000 0000 1100 1001
	word_flags flags;
};
static_assert(sizeof(s_player_identifier) == 0x8);

struct s_emblem_info
{
	struct
	{
		struct
		{
			word __unknown0;
			word __unknown2;
			word __unknown4;
			word __unknown6;
			word __unknown8;
			word __unknownA;
			word __unknownC;
			byte __unknownE;
			byte __unknownF;
		} __unknown0[50];

		long __unknown0_count;
	} __unknown0[2];

	// checksums calculated with `fast_checksum` 
	// `fast_checksum` is a wrapper around the `hashlittle` as part of `lookup3`
	// lookup3.c, by Bob Jenkins, May 2006, Public Domain.
	// https://burtleburtle.net/bob/c/lookup3.c
	dword __unknown0_checksums[2];
};

struct s_player_appearance
{
	byte_flags flags;
	byte player_model_choice;
	s_emblem_info emblem_info;
	c_static_wchar_string<5> service_tag;
};
static_assert(sizeof(s_player_appearance) == 0x660);

struct s_s3d_player_armor_configuration_loadout
{
	c_static_array<rgb_color, k_color_type_count> colors;

	// array of 10 based on the decode function for this struct
	c_static_array<byte, k_armor_type_count> armors;
	byte : 8;

	// using this byte for our implementation
	bool armor_is_set;
};
static_assert(sizeof(s_s3d_player_armor_configuration_loadout) == 0x20);

struct s_s3d_player_configuration_armor
{
	byte_flags flags;
	byte : 8;
	byte : 8;
	byte : 8;

	dword loadout_index;
	c_static_array<s_s3d_player_armor_configuration_loadout, 5> loadouts;
	c_static_array<c_static_array<dword, k_gameplay_modifier_count>, 5> loadout_modifiers;
};
static_assert(sizeof(s_s3d_player_configuration_armor) == 0x7EC);

struct s_s3d_player_weapon_configuration_loadout
{
	byte primary_weapon_index;
	byte secondary_weapon_index;
	c_enum<e_grenade_type, char, _grenade_type_frag, k_grenade_type_count> grenade_index;
	c_static_array<char, 4> consumables;

	// padding?
	byte __unknown7;
};
static_assert(sizeof(s_s3d_player_weapon_configuration_loadout) == 0x8);

struct s_s3d_player_configuration_weapon
{
	bool __unknown0;
	byte : 8;
	byte : 8;
	byte : 8;

	dword loadout_index;
	c_static_array<s_s3d_player_weapon_configuration_loadout, 5> loadouts;
	c_static_array<c_static_array<dword, k_gameplay_modifier_count>, 5> loadout_modifiers;
};
static_assert(sizeof(s_s3d_player_configuration_weapon) == 0x774);

struct s_player_configuration_from_client
{
	c_static_wchar_string<16> desired_name;
	byte user_selected_team_index;
	byte vote_selection_index;
	byte active_armor_loadout;
	byte active_weapon_loadout;
	bool player_is_griefer;
	dword_flags cheat_flags;
	dword_flags ban_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_identifier player_identifier;
	c_static_wchar_string<16> name;
	long team_index; // renamed to `multiplayer_team` some time after ODST
	long user_selected_team_index;
	s_player_appearance appearance;
	s_s3d_player_configuration_armor armor;
	s_s3d_player_configuration_weapon weapon;
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

struct s_tracking_object
{
	byte __data[0xC];
};
static_assert(sizeof(s_tracking_object) == 0xC);

struct s_damage_reporting_info
{
	word type;
	byte modifier;
	byte_flags recyling_flags;
};
static_assert(sizeof(s_damage_reporting_info) == 0x4);

struct s_player_shot_info
{
	char __data0[0x2];
	s_damage_reporting_info damage_reporting_info;
	char __data6[0x6];
};
static_assert(sizeof(s_player_shot_info) == 0xC);

#pragma pack(push, 4)
struct player_datum : s_datum_header
{
	short team;
	dword_flags flags;
	s_player_identifier player_identifier;
	long left_game_time;
	s_machine_identifier machine_identifier;
	short machine_index;
	short machine_input_user_index;
	long machine_controller_index;
	s_cluster_reference cluster_reference;
	datum_index unit_index;
	datum_index dead_unit_index;
	datum_index failed_teleport_unit_index;
	dword __unknown3C;
	word __unknown40;
	word __unknown42;
	word __unknown44;
	word __unknown46;
	byte outside_of_world_timer;
	byte next_spawn_control_context;
	long active_armor_loadout;
	long active_weapon_loadout;
	byte melee_recovery_ticks;
	byte melee_soft_recovery_ticks;
	byte grenade_suppression_timer;
	bool sprinting;
	bool crouching;
	bool shooting_left;
	bool shooting_right; // #TODO: confirmed this by checking it in ED??
	byte __unknown5B;
	word magnification_level; // zoom
	bool __unknown5E;
	bool __unknown5F;
	word __unknown60;
	word __unknown62;
	vector3d position;
	s_player_configuration configuration;
	s_player_configuration desired_configuration;
	long single_player_respawn_timer;
	bool early_respawn_requested;
	bool respawn_in_progress;
	bool respawn_forced; // instant respawn when true
	byte __unknown2CB7;
	long respawn_time;
	long respawn_timer_countdown;
	long penalty_time;
	long __unknown2CC4;
	long dead_timer;
	long __unknown2CCC;
	long __unknown2CD0;
	long __unknown2CD4;
	long grenade_recharge_timer;
	long aim_assist_object_index;
	long aim_assist_timestamp;
	short momentum_timer;
	short momentum_decay_timer;
	short momentum_falloff_timer;
	byte __data2CEA[2];
	short vehicle_ban_timer;
	c_aim_target_object cached_target;
	long cached_target_untargeted_ticks;
	c_static_array<s_tracking_object, 8> tracking_objects;
	short recently_spawned_timer;
	bool recently_spawned_timer_is_initial_spawn;
	byte respawn_failure_reason;
	byte __data2D64[0x3C];
	short __unknown2DA0;
	short lives_per_round;
	byte __data2DA4[0x8];
	c_player_traits player_traits;
	c_static_array<dword, k_number_of_multiplayer_powerup_flavors> powerup_pickup_time;
	byte __data2DD4[0x74];
	long weak_assassination_unit_index;
	bool is_assassination_victim;
	real_point3d assassination_authoritative_position;
	vector3d assassination_authoritative_forward;
	byte __data2E68[0x3C];
	c_static_array<s_player_shot_info, 8> shot_info;
	short spawn_count;
	byte __pad2F06[2];
};
static_assert(0x0002 == OFFSETOF(player_datum, team));
static_assert(0x0004 == OFFSETOF(player_datum, flags));
static_assert(0x0008 == OFFSETOF(player_datum, player_identifier));
static_assert(0x0010 == OFFSETOF(player_datum, left_game_time));
static_assert(0x0014 == OFFSETOF(player_datum, machine_identifier));
static_assert(0x0024 == OFFSETOF(player_datum, machine_index));
static_assert(0x0026 == OFFSETOF(player_datum, machine_input_user_index));
static_assert(0x0028 == OFFSETOF(player_datum, machine_controller_index));
static_assert(0x002C == OFFSETOF(player_datum, cluster_reference));
static_assert(0x0030 == OFFSETOF(player_datum, unit_index));
static_assert(0x0034 == OFFSETOF(player_datum, dead_unit_index));
static_assert(0x0038 == OFFSETOF(player_datum, failed_teleport_unit_index));
static_assert(0x003C == OFFSETOF(player_datum, __unknown3C));
static_assert(0x0040 == OFFSETOF(player_datum, __unknown40));
static_assert(0x0042 == OFFSETOF(player_datum, __unknown42));
static_assert(0x0044 == OFFSETOF(player_datum, __unknown44));
static_assert(0x0046 == OFFSETOF(player_datum, __unknown46));
static_assert(0x0048 == OFFSETOF(player_datum, outside_of_world_timer));
static_assert(0x0049 == OFFSETOF(player_datum, next_spawn_control_context));
static_assert(0x004C == OFFSETOF(player_datum, active_armor_loadout));
static_assert(0x0050 == OFFSETOF(player_datum, active_weapon_loadout));
static_assert(0x0054 == OFFSETOF(player_datum, melee_recovery_ticks));
static_assert(0x0055 == OFFSETOF(player_datum, melee_soft_recovery_ticks));
static_assert(0x0056 == OFFSETOF(player_datum, grenade_suppression_timer));
static_assert(0x0057 == OFFSETOF(player_datum, sprinting));
static_assert(0x0058 == OFFSETOF(player_datum, crouching));
static_assert(0x0059 == OFFSETOF(player_datum, shooting_left));
static_assert(0x005A == OFFSETOF(player_datum, shooting_right));
static_assert(0x005B == OFFSETOF(player_datum, __unknown5B));
static_assert(0x005C == OFFSETOF(player_datum, magnification_level));
static_assert(0x005E == OFFSETOF(player_datum, __unknown5E));
static_assert(0x005F == OFFSETOF(player_datum, __unknown5F));
static_assert(0x0060 == OFFSETOF(player_datum, __unknown60));
static_assert(0x0062 == OFFSETOF(player_datum, __unknown62));
static_assert(0x0064 == OFFSETOF(player_datum, position));
static_assert(0x0070 == OFFSETOF(player_datum, configuration));
static_assert(0x1690 == OFFSETOF(player_datum, desired_configuration));
static_assert(0x2CB0 == OFFSETOF(player_datum, single_player_respawn_timer));
static_assert(0x2CB4 == OFFSETOF(player_datum, early_respawn_requested));
static_assert(0x2CB5 == OFFSETOF(player_datum, respawn_in_progress));
static_assert(0x2CB6 == OFFSETOF(player_datum, respawn_forced));
static_assert(0x2CB7 == OFFSETOF(player_datum, __unknown2CB7));
static_assert(0x2CB8 == OFFSETOF(player_datum, respawn_time));
static_assert(0x2CBC == OFFSETOF(player_datum, respawn_timer_countdown));
static_assert(0x2CC0 == OFFSETOF(player_datum, penalty_time));
static_assert(0x2CC4 == OFFSETOF(player_datum, __unknown2CC4));
static_assert(0x2CC8 == OFFSETOF(player_datum, dead_timer));
static_assert(0x2CCC == OFFSETOF(player_datum, __unknown2CCC));
static_assert(0x2CD0 == OFFSETOF(player_datum, __unknown2CD0));
static_assert(0x2CD4 == OFFSETOF(player_datum, __unknown2CD4));
static_assert(0x2CD8 == OFFSETOF(player_datum, grenade_recharge_timer));
static_assert(0x2CDC == OFFSETOF(player_datum, aim_assist_object_index));
static_assert(0x2CE0 == OFFSETOF(player_datum, aim_assist_timestamp));
static_assert(0x2CE4 == OFFSETOF(player_datum, momentum_timer));
static_assert(0x2CE6 == OFFSETOF(player_datum, momentum_decay_timer));
static_assert(0x2CE8 == OFFSETOF(player_datum, momentum_falloff_timer));
static_assert(0x2CEA == OFFSETOF(player_datum, __data2CEA));
static_assert(0x2CEC == OFFSETOF(player_datum, vehicle_ban_timer));
static_assert(0x2CF0 == OFFSETOF(player_datum, cached_target));
static_assert(0x2CFC == OFFSETOF(player_datum, cached_target_untargeted_ticks));
static_assert(0x2D00 == OFFSETOF(player_datum, tracking_objects));
static_assert(0x2D60 == OFFSETOF(player_datum, recently_spawned_timer));
static_assert(0x2D62 == OFFSETOF(player_datum, recently_spawned_timer_is_initial_spawn));
static_assert(0x2D63 == OFFSETOF(player_datum, respawn_failure_reason));
static_assert(0x2D64 == OFFSETOF(player_datum, __data2D64));
static_assert(0x2DA0 == OFFSETOF(player_datum, __unknown2DA0));
static_assert(0x2DA2 == OFFSETOF(player_datum, lives_per_round));
static_assert(0x2DA4 == OFFSETOF(player_datum, __data2DA4));
static_assert(0x2DAC == OFFSETOF(player_datum, player_traits));
static_assert(0x2DC8 == OFFSETOF(player_datum, powerup_pickup_time));
static_assert(0x2DD4 == OFFSETOF(player_datum, __data2DD4));
static_assert(0x2E48 == OFFSETOF(player_datum, weak_assassination_unit_index));
static_assert(0x2E4C == OFFSETOF(player_datum, is_assassination_victim));
static_assert(0x2E50 == OFFSETOF(player_datum, assassination_authoritative_position));
static_assert(0x2E5C == OFFSETOF(player_datum, assassination_authoritative_forward));
static_assert(0x2E68 == OFFSETOF(player_datum, __data2E68));
static_assert(0x2EA4 == OFFSETOF(player_datum, shot_info));
static_assert(0x2F04 == OFFSETOF(player_datum, spawn_count));
static_assert(0x2F06 == OFFSETOF(player_datum, __pad2F06));
static_assert(sizeof(player_datum) == 0x2F08);
#pragma pack(pop)

#pragma pack(push, 1)
struct s_players_global_data
{
	long players_in_game_count;

	bool input_disabled;
	bool mostly_inhibit;

	bool __unknown6;
	bool __unknown7;

	byte __data8[4];

	dword machine_valid_mask;
	c_static_array<s_machine_identifier, 17> machine_identifiers;

	bool local_machine_exists;
	s_machine_identifier local_machine_identifier;
	byte __pad131[0x3];

	long local_machine_index;

	// set to `false` when `zone_set_switch_flags` is memset
	bool __unknown138;

	byte __data139;

	c_enum<e_player_respawn_failure, short, _player_respawn_failure_none, k_player_respawn_failure_count> respawn_failure;

	// player_positions_initialize_for_new_structure_bsp
	// players_update_after_game
	bool __unknown13C;

	byte __data13D[0x3];

	real_point3d switching_player_position;
	vector3d switching_player_forward;

	// player_positions_initialize_for_new_structure_bsp
	// players_update_after_game
	long begin_zone_set_switch_trigger_volume_index;
	long commit_zone_set_switch_trigger_volume_index;

	// players_update_after_game
	// if (player_index != -1 && ++__unknown160 > 12)
	//    __unknown160 = 0
	short __unknown160;

	short __unknown162;

	// players_update_after_game
	long player_index;

	// players_update_after_game
	long zoneset_index;

	// memset in `players_initialize_for_new_map`
	// zone_set_trigger_volume_index
	c_static_flags<1024> zone_set_switch_flags;

	// `terminal_was_completed`
	// - returns whether or not the given terminal was read to completion
	word terminal_completed_flags;

	// `terminal_was_accessed`
	// - returns whether or not the given terminal was accessed
	word terminal_accessed_flags;

	// `terminal_is_being_read`
	// - returns whether or not a terminal is currently being read
	bool terminal_being_read;

	byte __data1F1[0x3];

	byte __data1F4[0x40];
};
static_assert(sizeof(s_players_global_data) == 0x234);
#pragma pack(pop)

struct c_player_in_game_iterator :
	public c_data_iterator<player_datum>
{
	bool next()
	{
		for (m_datum = (player_datum*)data_iterator_next(&m_iterator);
			m_datum && TEST_BIT(m_datum->flags, 1);
			m_datum = (player_datum*)data_iterator_next(&m_iterator))
		{
		}

		return m_datum != nullptr;
	}
};

struct c_player_with_unit_iterator :
	public c_data_iterator<player_datum>
{
	bool next()
	{
		for (m_datum = (player_datum*)data_iterator_next(&m_iterator);
			m_datum && m_datum->unit_index == -1;
			m_datum = (player_datum*)data_iterator_next(&m_iterator))
		{
		}

		return m_datum != nullptr;
	}
};

extern void __cdecl player_set_unit_index(long player_index, long unit_index);
extern bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position);
extern long __cdecl players_first_active_user();
extern s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player);
extern s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player);
extern long multiplayer_universal_data_get_absolute_equipment_block_index(char const* name);
extern long multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(char const* region_or_biped_name, char const* selection_name);
extern short multiplayer_universal_data_get_absolute_weapons_selection_block_index(char const* selection_name);

