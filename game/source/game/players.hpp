#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/game_engine_player_traits.hpp"
#include "memory/data.hpp"
#include "objects/damage_reporting.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct s_machine_identifier
{
	long parts[4];
};
static_assert(sizeof(s_machine_identifier) == 0x10);

struct transport_address;
struct s_player_identifier
{
	s_player_identifier();
	s_player_identifier(dword _ipv4_address, word _port, word_flags _flags);
	s_player_identifier(transport_address const* address);

	// make_int64(ip, port)
	dword ipv4_address;
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
static_assert(sizeof(s_emblem_info) == 0x650);

enum e_bungienet_user
{
	_bungienet_user_seventh_column = 0,
	_bungienet_user_bungie_pro,
	_bungienet_user_bungie,
	_bungienet_user_default,

	k_bungienet_user_count,
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
	byte : 8;
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
	c_enum<e_grenade_type, char, _grenade_type_human_fragmentation, k_grenade_type_count> grenade_index;
	c_static_array<char, 4> consumables;

	// using this byte for our implementation
	c_flags<e_bungienet_user, byte, k_bungienet_user_count> bungienet_user;
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
	byte armor_loadout_index;
	byte weapon_loadout_index;
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
	byte __data[0x4];
	long object_index;
	long __time8;
};
static_assert(sizeof(s_tracking_object) == 0xC);

struct multiplayer_player_info
{
	long target_player_index;
	short __unknown4;
	char __unknown6;
	char __unknown7;
	char __unknown8;
	char __unknown9;
	char __unknownA;
	char __unknownB;
	short __unknownC;
	short lives;

	// used in `game_engine_respond_to_betrayal`, `game_engine_player_is_dead_and_betrayed_by_griefer`
	long betrayal_player_index;

	// used in `game_engine_player_killed`, `game_engine_add_starting_equipment`
	dword last_killed_round_time; // = `game_engine_round_time_get`

	c_player_traits player_traits;

	c_static_array<dword, k_number_of_multiplayer_powerup_flavors> powerup_pickup_time;

	// used in `game_engine_spawn_influencers_add_ally_bias`
	long dead_camera_target_player_index;

	bool map_editor_rotating;
	real_point2d map_editor_throttle;
	euler_angles2d map_editor_rotation;

	byte __data58[0x8];

	// -------- nemesis_mechanics ---------

	// used in `game_engine_player_killed`, struct?
	union // gameplay_modifier: `enable_nemesis_mechanics`
	{
		// if `++counts == multiplayer_globals:runtime:multiplayer_constants:__unknown1DC` award medal

		c_static_array<short, 16 /* player_index */> nemesis_mechanics_nemesis_counts; // killing_player, nemesis medal
		c_static_array<short, 16 /* player_index */> nemesis_mechanics_avenger_counts; // dead_player, avenger medal
	};

	// -------- nemesis_mechanics ---------

	// ------- revenge_shield_boost -------

	// used in `game_engine_add_starting_equipment`, struct?
	// gameplay_modifier: `revenge_shield_boost` values
	bool revenge_shield_boost_unknown80;
	short revenge_shield_boost_unknown82; // `object->__unknown100 *= (revenge_shield_boost_modifier_value * player->revenge_shield_boost_unknown2E16) + 1.0f`
	long revenge_shield_boost_unknown84;
	long revenge_shield_boost_player_index; // == `player_index`

	dword __unknown8C;
	bool __unknown90;
	byte __data91[3];
};
static_assert(sizeof(multiplayer_player_info) == 0x94);

struct s_simulation_unit_melee_damage_event_data;
struct _simulation_unit_melee_damage_event_data
{
	byte __data[0x3C];
};

struct s_player_shot_info
{
	word __unknown0;
	s_damage_reporting_info damage_reporting_info;
	char __data6[0x6];
};
static_assert(sizeof(s_player_shot_info) == 0xC);

#pragma pack(push, 4)
struct player_datum : s_datum_header
{
	short __unknown2;
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
	dword_flags latched_control_flags;

	// ---------- cooldown_reset ----------

	// these are used in `players_update_after_game`, `player_spawn`, `sub_53C860`, struct?
	// if `player->cooldown_reset_unknown40 == 0` do something with equipment
	// gameplay_modifier: `cooldown_reset` related
	word cooldown_reset_unknown40;
	word cooldown_reset_unknown42;
	word cooldown_reset_unknown44;

	// ---------- cooldown_reset ----------

	word_flags latched_action_flags;
	byte outside_of_world_timer;
	byte next_spawn_control_context;

	long armor_loadout_index;
	long weapon_loadout_index;

	// used in `players_update_after_game`
	byte melee_recovery_ticks;
	byte melee_soft_recovery_ticks;
	byte grenade_suppression_timer;

	bool sprinting;
	bool crouching;
	bool shooting_left;
	bool shooting_right; // #TODO: confirmed this by checking it in ED??
	byte __unknown5B;
	word magnification_level; // zoom

	// ------------- momentum -------------

	// these are used in `player_submit_actions`, struct?

	bool __unknown5E; // something todo with `shooting`
	bool __unknown5F; // something todo with `magnification_level`
	word __unknown60; // set from tags, game_seconds_to_ticks_round(game_globals:player_information:__unknown98), something todo with `shooting`
	word __unknown62; // set from tags, game_seconds_to_ticks_round(game_globals:player_information:__unknown94), something todo with `magnification_level`

	// ------------- momentum -------------

	vector3d position;

	s_player_configuration configuration;
	s_player_configuration desired_configuration;

	// timers
	long single_player_respawn_timer;
	bool early_respawn_requested;
	bool respawn_in_progress;
	bool respawn_forced; // instant respawn when true
	byte __unknown2CB7;
	long respawn_timer;
	long respawn_timer_countdown;
	long respawn_penalty;
	long telefrag_timer;
	long dead_timer;
	long __unknown2CCC;
	long __unknown2CD0;
	long __unknown2CD4;
	long grenade_recharge_timer;

	long aim_assist_object_index;
	long aim_assist_timestamp;

	// ------------- momentum -------------

	// set in `player_submit_actions`, struct?
	short momentum_timer;

	short momemtum_unknown2CE6;

	// set in `player_submit_actions`
	short momentum_decay_timer; // set from tags, `game_globals:player_control:cooldown_time`

	// set in `player_submit_actions`, `sub_53C570`
	short momentum_falloff_timer; // set from tags, `momentum_falloff_timer - (game_globals:player_control:stamina_deplete_restore_time * s_equipment:adrenaline:sprint_restore)`

	bool momemtum_suppressed;

	// ------------- momentum -------------

	// used in `players_update_after_game`
	short vehicle_ban_timer;

	c_aim_target_object cached_target;
	long cached_target_untargeted_ticks;
	c_static_array<s_tracking_object, 8> tracking_objects;

	short recently_spawned_timer;
	bool recently_spawned_timer_is_initial_spawn;
	byte respawn_failure_reason;

	// ------------ tank_mode -------------

	// used in `sub_537D10`, struct?
	long tank_mode_time2D64; // = game_time_get();
	real tank_mode_duration; // set from tags, `equipment:tank_mode:duration`
	real tank_mode_unknown2D6C; // set from tags, `equipment:tank_mode:__unknown10 / 100.0f`
	real tank_mode_damage_absorption_scale; // set from tags, `equipment:tank_mode:damage_absorption_scale / 100.0f`

	// used in `sub_540730`
	real tank_mode_unknown2D74;
	real tank_mode_unknown2D78;

	// ------------ tank_mode -------------

	// ---------- reactive_armor ----------

	// used in `sub_537C90`, struct?
	long reactive_armor_time2D7C; // = game_time_get();
	real reactive_armor_duration; // set from tags, `equipment:reactive_armor:duration`
	real reactive_armor_damage_reflection_scale; // set from tags, `equipment:reactive_armor:damage_reflection_scale / 100.0f`
	real reactive_armor_unknown2D88; // set from tags, `equipment:reactive_armor:__unknown8 / 100.0f`

	// ---------- reactive_armor ----------

	// used in `players_update_after_game`
	long stamina_restore_near_death_timer; // gameplay_modifier: `stamina_restore_near_death`

	// used in `sub_536FD0`
	bool grenade_scavenger_modifier_used; // gameplay_modifier: `grenade_scavenger`

	byte __data2D91[0x3];

	// `players_reset`
	multiplayer_player_info multiplayer;

	byte __data2E28[0x2];

	// used in `player_died`, struct?
	// `respawn_weapon_definition_indices`?
	// `respawn_equipment_definition_index`?
	// `respawn_grenades_additional`?
	// `desired_respawn_transform_valid`?
	// *(qword*)__data2E2A = 0i64;
	// *(qword*)&__data2E2A[0x8] = 0i64;
	// *(dword*)&__data2E2A[0x10] = 0;
	// *(dword*)&__data2E2A[0x16] = -1;
	// *(dword*)&__data2E2A[0x1A] = 0;
	byte __data2E2A[0x1E];

	long weak_assassination_unit_index;
	bool is_assassination_victim;
	real_point3d assassination_authoritative_position;
	vector3d assassination_authoritative_forward;
	c_typed_opaque_data<struct s_simulation_unit_melee_damage_event_data, sizeof(_simulation_unit_melee_damage_event_data), __alignof(_simulation_unit_melee_damage_event_data) - 1> melee_damage_event_data;

	c_static_array<s_player_shot_info, 8> shot_info;
	short spawn_count;
	byte __pad2F06[2];
};

static_assert(0x0002 == OFFSETOF(player_datum, __unknown2));
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
static_assert(0x003C == OFFSETOF(player_datum, latched_control_flags));
static_assert(0x0040 == OFFSETOF(player_datum, cooldown_reset_unknown40));
static_assert(0x0042 == OFFSETOF(player_datum, cooldown_reset_unknown42));
static_assert(0x0044 == OFFSETOF(player_datum, cooldown_reset_unknown44));
static_assert(0x0046 == OFFSETOF(player_datum, latched_action_flags));
static_assert(0x0048 == OFFSETOF(player_datum, outside_of_world_timer));
static_assert(0x0049 == OFFSETOF(player_datum, next_spawn_control_context));
static_assert(0x004C == OFFSETOF(player_datum, armor_loadout_index));
static_assert(0x0050 == OFFSETOF(player_datum, weapon_loadout_index));
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
static_assert(0x2CB8 == OFFSETOF(player_datum, respawn_timer));
static_assert(0x2CBC == OFFSETOF(player_datum, respawn_timer_countdown));
static_assert(0x2CC0 == OFFSETOF(player_datum, respawn_penalty));
static_assert(0x2CC4 == OFFSETOF(player_datum, telefrag_timer));
static_assert(0x2CC8 == OFFSETOF(player_datum, dead_timer));
static_assert(0x2CCC == OFFSETOF(player_datum, __unknown2CCC));
static_assert(0x2CD0 == OFFSETOF(player_datum, __unknown2CD0));
static_assert(0x2CD4 == OFFSETOF(player_datum, __unknown2CD4));
static_assert(0x2CD8 == OFFSETOF(player_datum, grenade_recharge_timer));
static_assert(0x2CDC == OFFSETOF(player_datum, aim_assist_object_index));
static_assert(0x2CE0 == OFFSETOF(player_datum, aim_assist_timestamp));
static_assert(0x2CE4 == OFFSETOF(player_datum, momentum_timer));
static_assert(0x2CE6 == OFFSETOF(player_datum, momemtum_unknown2CE6));
static_assert(0x2CE8 == OFFSETOF(player_datum, momentum_decay_timer));
static_assert(0x2CEA == OFFSETOF(player_datum, momentum_falloff_timer));
static_assert(0x2CEC == OFFSETOF(player_datum, momemtum_suppressed));
static_assert(0x2CEE == OFFSETOF(player_datum, vehicle_ban_timer));
static_assert(0x2CF0 == OFFSETOF(player_datum, cached_target));
static_assert(0x2CFC == OFFSETOF(player_datum, cached_target_untargeted_ticks));
static_assert(0x2D00 == OFFSETOF(player_datum, tracking_objects));
static_assert(0x2D60 == OFFSETOF(player_datum, recently_spawned_timer));
static_assert(0x2D62 == OFFSETOF(player_datum, recently_spawned_timer_is_initial_spawn));
static_assert(0x2D63 == OFFSETOF(player_datum, respawn_failure_reason));
static_assert(0x2D64 == OFFSETOF(player_datum, tank_mode_time2D64));
static_assert(0x2D68 == OFFSETOF(player_datum, tank_mode_duration));
static_assert(0x2D6C == OFFSETOF(player_datum, tank_mode_unknown2D6C));
static_assert(0x2D70 == OFFSETOF(player_datum, tank_mode_damage_absorption_scale));
static_assert(0x2D74 == OFFSETOF(player_datum, tank_mode_unknown2D74));
static_assert(0x2D78 == OFFSETOF(player_datum, tank_mode_unknown2D78));
static_assert(0x2D7C == OFFSETOF(player_datum, reactive_armor_time2D7C));
static_assert(0x2D80 == OFFSETOF(player_datum, reactive_armor_duration));
static_assert(0x2D84 == OFFSETOF(player_datum, reactive_armor_damage_reflection_scale));
static_assert(0x2D88 == OFFSETOF(player_datum, reactive_armor_unknown2D88));
static_assert(0x2D8C == OFFSETOF(player_datum, stamina_restore_near_death_timer));
static_assert(0x2D90 == OFFSETOF(player_datum, grenade_scavenger_modifier_used));
static_assert(0x2D91 == OFFSETOF(player_datum, __data2D91));
static_assert(0x2D94 == OFFSETOF(player_datum, multiplayer.target_player_index));
static_assert(0x2D98 == OFFSETOF(player_datum, multiplayer.__unknown4));
static_assert(0x2D9A == OFFSETOF(player_datum, multiplayer.__unknown6));
static_assert(0x2D9B == OFFSETOF(player_datum, multiplayer.__unknown7));
static_assert(0x2D9C == OFFSETOF(player_datum, multiplayer.__unknown8));
static_assert(0x2D9D == OFFSETOF(player_datum, multiplayer.__unknown9));
static_assert(0x2D9E == OFFSETOF(player_datum, multiplayer.__unknownA));
static_assert(0x2D9F == OFFSETOF(player_datum, multiplayer.__unknownB));
static_assert(0x2DA0 == OFFSETOF(player_datum, multiplayer.__unknownC));
static_assert(0x2DA2 == OFFSETOF(player_datum, multiplayer.lives));
static_assert(0x2DA4 == OFFSETOF(player_datum, multiplayer.betrayal_player_index));
static_assert(0x2DAC == OFFSETOF(player_datum, multiplayer.player_traits));
static_assert(0x2DC8 == OFFSETOF(player_datum, multiplayer.powerup_pickup_time));
static_assert(0x2DD4 == OFFSETOF(player_datum, multiplayer.dead_camera_target_player_index));
static_assert(0x2DD8 == OFFSETOF(player_datum, multiplayer.map_editor_rotating));
static_assert(0x2DDC == OFFSETOF(player_datum, multiplayer.map_editor_throttle));
static_assert(0x2DE4 == OFFSETOF(player_datum, multiplayer.map_editor_rotation));
static_assert(0x2DEC == OFFSETOF(player_datum, multiplayer.__data58));
static_assert(0x2DF4 == OFFSETOF(player_datum, multiplayer.nemesis_mechanics_nemesis_counts));
static_assert(0x2DF4 == OFFSETOF(player_datum, multiplayer.nemesis_mechanics_avenger_counts));
static_assert(0x2E14 == OFFSETOF(player_datum, multiplayer.revenge_shield_boost_unknown80));
static_assert(0x2E16 == OFFSETOF(player_datum, multiplayer.revenge_shield_boost_unknown82));
static_assert(0x2E18 == OFFSETOF(player_datum, multiplayer.revenge_shield_boost_unknown84));
static_assert(0x2E1C == OFFSETOF(player_datum, multiplayer.revenge_shield_boost_player_index));
static_assert(0x2E20 == OFFSETOF(player_datum, multiplayer.__unknown8C));
static_assert(0x2E24 == OFFSETOF(player_datum, multiplayer.__unknown90));
static_assert(0x2E25 == OFFSETOF(player_datum, multiplayer.__data91));
static_assert(0x2E28 == OFFSETOF(player_datum, __data2E28));
static_assert(0x2E2A == OFFSETOF(player_datum, __data2E2A));
static_assert(0x2E48 == OFFSETOF(player_datum, weak_assassination_unit_index));
static_assert(0x2E4C == OFFSETOF(player_datum, is_assassination_victim));
static_assert(0x2E50 == OFFSETOF(player_datum, assassination_authoritative_position));
static_assert(0x2E5C == OFFSETOF(player_datum, assassination_authoritative_forward));
static_assert(0x2E68 == OFFSETOF(player_datum, melee_damage_event_data));
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
	bool mostly_inhibited;

	bool disable_weapon_pickup;
	bool __unknown7;
	bool disable_equipment_use;

	byte __data9[3];

	dword machine_valid_mask;
	c_static_array<s_machine_identifier, 17> machine_identifiers;

	bool local_machine_exists;
	s_machine_identifier local_machine_identifier;
	byte __pad131[0x3];

	long local_machine_index;
	bool scripted_dont_allow_respawning;

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
	c_player_in_game_iterator(c_data_iterator<player_datum> iterator) :
		c_data_iterator<player_datum>(iterator)
	{
	}

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
	c_player_with_unit_iterator(c_data_iterator<player_datum> iterator) :
		c_data_iterator<player_datum>(iterator)
	{
	}

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

extern string_id g_player_desired_mode_override;
extern void player_override_desired_mode(long desired_mode_override);
extern void players_debug_render();

extern void __cdecl player_set_unit_index(long player_index, long unit_index);
extern void __cdecl player_suppress_action(long player_index, long player_suppress_action_type);
extern bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position);
extern long __cdecl players_first_active_user();
extern long __cdecl player_index_from_unit_index(long unit_index);
extern s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player);
extern s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player);
extern long multiplayer_universal_data_get_absolute_equipment_block_index(char const* name);
extern long multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(char const* region_or_biped_name, char const* selection_name);
extern short multiplayer_universal_data_get_absolute_weapons_selection_block_index(char const* selection_name);

