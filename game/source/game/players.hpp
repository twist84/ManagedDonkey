#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/game_engine_player_traits.hpp"
#include "memory/data.hpp"
#include "objects/damage_reporting.hpp"
#include "objects/emblems.hpp"
#include "structures/structures.hpp"
#include "text/unicode.hpp"

struct s_game_cluster_bit_vectors;
struct transport_address;

enum e_bungienet_user
{
	_bungienet_user_seventh_column = 0,
	_bungienet_user_bungie_pro,
	_bungienet_user_bungie,
	_bungienet_user_default,

	k_bungienet_user_count,
};

enum e_player_appearance_flags
{
	_female_voice_bit = 0,
	k_number_of_player_appearance_flags
};
typedef c_flags_no_init<e_player_appearance_flags, uns8, k_number_of_player_appearance_flags> c_player_appearance_flags;
typedef c_enum<e_player_model_choice, char, _player_model_choice_first, k_player_model_choice_count> c_player_model_choice;
typedef c_enum<e_player_color_index, char, _player_color_none, k_player_color_index_count> c_player_color_index;

struct s_machine_identifier
{
	int32 parts[4];
};
static_assert(sizeof(s_machine_identifier) == 0x10);

struct s_player_identifier
{
	s_player_identifier();
	s_player_identifier(uns64 data);
	s_player_identifier(uns32 _ipv4_address, uns16 _port, uns16 _flags);
	s_player_identifier(const transport_address* address);

	byte identifier[8];

	bool operator==(const s_player_identifier& other)
	{
		return csmemcmp(this, &other, sizeof(*this)) == 0;
	}

	//// make_int64(ip, port)
	//uns32 ipv4_address;
	//uns16 port;
	//
	//// online_xuid_is_guest_account
	//// 0000 0000 1100 1001
	//uns16 flags;
};
static_assert(sizeof(s_player_identifier) == 0x8);

struct s_player_appearance
{
	c_player_appearance_flags flags;
	c_player_color_index change_color_index[3];
	c_player_model_choice player_model_choice;
	s_emblem_info emblem_info;
	uns8 model_permutations[10];
	byte emblem_pad[0x63C];
	c_static_wchar_string<5> service_tag;
	byte __pad65E[0x2];
};
static_assert(sizeof(s_player_appearance) == 0x660);

struct s_s3d_player_armor_configuration_loadout
{
	c_static_array<rgb_color, k_color_type_count> colors;

	// array of 10 based on the decode function for this struct
	c_static_array<uns8, k_armor_type_count> armors;
	uns8 : 8;
	uns8 : 8;
};
static_assert(sizeof(s_s3d_player_armor_configuration_loadout) == 0x20);

struct s_s3d_player_configuration_armor
{
	uns8 flags;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;

	uns32 loadout_index;
	c_static_array<s_s3d_player_armor_configuration_loadout, 5> loadouts;
	c_static_array<c_static_array<uns32, k_gameplay_modifier_count>, 5> loadout_modifiers;
};
static_assert(sizeof(s_s3d_player_configuration_armor) == 0x7EC);

struct s_s3d_player_weapon_configuration_loadout
{
	uns8 primary_weapon_index;
	uns8 secondary_weapon_index;
	c_enum<e_grenade_type, int8, _grenade_type_human_fragmentation, k_grenade_type_count> grenade_index;
	c_static_array<int8, 4> consumables;

	// using this uns8 for our implementation
	c_flags<e_bungienet_user, uns8, k_bungienet_user_count> bungienet_user;
};
static_assert(sizeof(s_s3d_player_weapon_configuration_loadout) == 0x8);

struct s_s3d_player_configuration_weapon
{
	bool __unknown0;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;

	uns32 loadout_index;
	c_static_array<s_s3d_player_weapon_configuration_loadout, 5> loadouts;
	c_static_array<c_static_array<uns32, k_gameplay_modifier_count>, 5> loadout_modifiers;
};
static_assert(sizeof(s_s3d_player_configuration_weapon) == 0x774);

struct s_player_configuration_from_client
{
	s_player_configuration_from_client();

	c_static_wchar_string<16> desired_name;
	uns8 user_selected_team_index;
	uns8 vote_selection_index;
	uns8 armor_loadout_index;
	uns8 weapon_loadout_index;
	bool player_is_griefer;
	uns32 cheat_flags;
	uns32 ban_flags;
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_configuration_from_host();

	s_player_identifier player_identifier;
	c_static_wchar_string<16> name;
	e_game_team team_index;
	e_game_team assigned_team_index;
	s_player_appearance appearance;
	s_s3d_player_configuration_armor armor;
	s_s3d_player_configuration_weapon weapon;
};
static_assert(sizeof(s_player_configuration_from_host) == 0x15F0);

struct s_player_configuration
{
	s_player_configuration();

	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0x1620);

#pragma pack(push, 1)
struct game_machine_options
{
	uns32 valid_machine_mask;
	s_machine_identifier machines[k_maximum_machines];

	bool local_machine_exists;
	s_machine_identifier local_machine;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
};
static_assert(sizeof(game_machine_options) == 0x128);
#pragma pack(pop)

struct game_player_options
{
	bool valid;
	bool player_left_game;
	int16 user_index;
	int32 controller_index;
	s_machine_identifier machine_identifier;
	s_player_identifier player_identifier;
	s_player_configuration configuration_data;
};
static_assert(sizeof(game_player_options) == 0x1640);

struct s_tracking_object
{
	byte __data[0x4];
	int32 object_index;
	int32 __time8;
};
static_assert(sizeof(s_tracking_object) == 0xC);

struct s_king_player_info
{
	int16 ticks_on_hill;
	int16 ticks_outside_hill;
	int16 ticks_on_hill_contested;
	int16 pad;
};
static_assert(sizeof(s_king_player_info) == 0x8);

struct s_juggernaut_player_info
{
	int32 juggernaut_index;
};
static_assert(sizeof(s_juggernaut_player_info) == 0x4);

struct multiplayer_player_info
{
	int32 player_display_index;
	int16 teleporter_index;
	int8 in_vehicle_timer;
	int8 without_unit_timer;
	int8 with_unit_timer;
	bool was_crouched;
	bool is_crouched;
	byte pad;
	int16 player_display_count;
	int16 remaining_lives;
	int32 last_betrayer_player_index;
	int32 time_of_death;
	c_player_traits player_traits;
	int32 powerup_pickup_time[k_multiplayer_powerup_flavor_count];
	int32 dead_camera_target_player_index;

	struct
	{
		bool player_locked_for_manipulation;
		bool pad[0x3];
		real_vector2d roll_distance_amount;
		real_vector2d yaw_pitch_angles;
	} map_editor_data;

	union
	{
		s_king_player_info king_player_info;
		s_juggernaut_player_info juggernaut_player_info;
	};
};
static_assert(sizeof(multiplayer_player_info) == 0x60);

struct s_simulation_unit_melee_damage_event_data;
struct _simulation_unit_melee_damage_event_data
{
	real_point3d impact_point;
	real_vector3d impact_normal;
	int32 damage_definition_index;
	int32 clang_damage_definition_index;
	int16 breakable_surface_set_index;
	int16 breakable_surface_index;
	int32 instanced_geometry_instance_index;
	int32 surface_index;
	int32 global_material_index;
	int32 object_material_index;
	real32 scale;
	s_damage_reporting_info damage_reporting_info;
};

struct s_player_shot_info
{
	int16 shot_id;
	s_damage_reporting_info damage_reporting_info;
	uns16 pad;
	int32 game_time;
};
static_assert(sizeof(s_player_shot_info) == 0xC);

// there's a high likelihood of Halo Online using the Halo Reach player flags enum
enum e_player_flags
{
	_player_active_in_game_bit = 0,
	_player_left_game_bit,
	_player_blocking_teleporter_bit,
	_player_initial_spawn_bit,
	_player_action_suppress_primary_bit,
	_player_action_suppress_secondary_bit,
	_player_action_suppress_tertiary_bit,
	_player_action_dual_wield_grenade_latch_bit,
	_player_standing_in_teleporter_bit,
	_player_standing_in_teleporter_teleported_bit,
	_player_vehicle_entrance_ban_bit,
	_player_play_coop_spawn_effect_bit,
	_player_sitting_out_bit,
	_player_joined_in_progress_bit,

	// game_engine_update_coop_spawning
	// halo reach x360: bit 14
	// halo 4 x360:     bit 16
	// look_training_hack?
	_player_unknown_bit14,

	// players_coop_update_respawn
	// survival_mode_respawn_dead_players
	// halo reach x360: bit 15
	// halo 4 x360:     bit 17
	_player_unknown_bit15,
		
	// player_prepare_action
	// unit_action_melee_player_update
	// player_submit_actions
	// halo reach x360: bit 16
	// halo 4 x360:     bit 18
	_player_unknown_bit16,
	
	k_player_flag_count
};

#pragma pack(push, 4)
struct player_datum :
	s_datum_header
{
	int16 __unknown2;
	uns32 flags;
	s_player_identifier player_identifier;
	int32 left_game_time;
	s_machine_identifier machine_identifier;
	int16 machine_index;
	int16 machine_user_index;
	int32 machine_controller_index;

	s_cluster_reference cluster_reference;
	int32 unit_index;
	int32 dead_unit_index;
	int32 failed_teleport_unit_index;
	uns32 latched_control_flags;

	struct // ---------- cooldown_reset ----------
	{
		// these are used in `players_update_after_game`, `player_spawn`, `sub_53C860`, struct?
		// if `player->cooldown_reset_unknown40 == 0` do something with equipment
		// gameplay_modifier: `cooldown_reset` related
		uns16 cooldown_reset_unknown40;
		uns16 cooldown_reset_unknown42;
		uns16 cooldown_reset_unknown44;
	};

	uns16 latched_action_flags;
	uns8 outside_of_world_timer;
	uns8 next_spawn_control_context;

	int32 armor_loadout_index;
	int32 weapon_loadout_index;

	// used in `players_update_after_game`
	uns8 melee_recovery_ticks;
	uns8 melee_soft_recovery_ticks;
	uns8 grenade_suppression_timer;

	bool sprinting;
	bool crouching;
	bool shooting_left;
	bool shooting_right; // $TODO confirmed this by checking it in ED??
	uns8 __unknown5B;
	uns16 magnification_level; // zoom

	struct // momentum
	{
		// these are used in `player_submit_actions`, struct?

		bool __unknown5E; // something todo with `shooting`
		bool __unknown5F; // something todo with `magnification_level`
		uns16 __unknown60; // set from tags, game_seconds_to_ticks_round(game_globals:player_information:__unknown98), something todo with `shooting`
		uns16 __unknown62; // set from tags, game_seconds_to_ticks_round(game_globals:player_information:__unknown94), something todo with `magnification_level`
	};

	real_vector3d position;

	s_player_configuration configuration;
	s_player_configuration desired_configuration;

	struct // timers
	{
		int32 single_player_respawn_timer;
		bool early_respawn_requested;
		bool respawn_in_progress;
		bool respawn_forced; // if true respawn instantaneously
		uns8 __unknown2CB7;
		int32 respawn_timer;
		int32 respawn_timer_countdown;
		int32 respawn_penalty;
		int32 telefrag_timer;
		int32 dead_timer;
		int32 __unknown2CCC;
		int32 __unknown2CD0;
		int32 __unknown2CD4;
		int32 grenade_recharge_timer;
	};

	int32 aim_assist_object_index;
	int32 aim_assist_timestamp;

	struct // momentum
	{
		// set in `player_submit_actions`, struct?
		int16 momentum_timer;

		int16 momemtum_unknown2CE6;

		// set in `player_submit_actions`
		int16 momentum_decay_timer; // set from tags, `game_globals:player_control:cooldown_time`

		// set in `player_submit_actions`, `sub_53C570`
		int16 momentum_falloff_timer; // set from tags, `momentum_falloff_timer - (game_globals:player_control:stamina_deplete_restore_time * s_equipment:adrenaline:sprint_restore)`

		bool momemtum_suppressed;
	};

	// used in `players_update_after_game`
	int16 vehicle_ban_timer;

	c_aim_target_object cached_target;
	int32 cached_target_untargeted_ticks;
	c_static_array<s_tracking_object, 8> tracking_objects;

	int16 recently_spawned_timer;
	bool recently_spawned_timer_is_initial_spawn;
	uns8 respawn_failure_reason;

	struct // tank_mode
	{
		// used in `sub_537D10`, struct?
		int32 tank_mode_time2D64; // = game_time_get();
		real32 tank_mode_duration; // set from tags, `equipment:tank_mode:duration`
		real32 tank_mode_unknown2D6C; // set from tags, `equipment:tank_mode:__unknown10 / 100.0f`
		real32 tank_mode_damage_absorption_scale; // set from tags, `equipment:tank_mode:damage_absorption_scale / 100.0f`

		// used in `sub_540730`
		real32 tank_mode_unknown2D74;
		real32 tank_mode_unknown2D78;
	};

	struct // reactive_armor
	{
		// used in `sub_537C90`, struct?
		int32 reactive_armor_time2D7C; // = game_time_get();
		real32 reactive_armor_duration; // set from tags, `equipment:reactive_armor:duration`
		real32 reactive_armor_damage_reflection_scale; // set from tags, `equipment:reactive_armor:damage_reflection_scale / 100.0f`
		real32 reactive_armor_unknown2D88; // set from tags, `equipment:reactive_armor:__unknown8 / 100.0f`
	};

	// used in `players_update_after_game`
	int32 stamina_restore_near_death_timer; // gameplay_modifier: `stamina_restore_near_death`

	// used in `sub_536FD0`
	bool grenade_scavenger_modifier_used; // gameplay_modifier: `grenade_scavenger`

	byte __data2D91[0x3];

	// `players_reset`
	multiplayer_player_info multiplayer;

	// -------- nemesis_mechanics ---------

	// used in `game_engine_player_killed`, struct?
	union // gameplay_modifier: `enable_nemesis_mechanics`
	{
		// if `++counts == multiplayer_globals:runtime:multiplayer_constants:__unknown1DC` award medal

		c_static_array<int16, 16 /* player_index */> nemesis_mechanics_nemesis_counts; // killing_player, nemesis medal
		c_static_array<int16, 16 /* player_index */> nemesis_mechanics_avenger_counts; // dead_player, avenger medal
	};

	// -------- nemesis_mechanics ---------

	// ------- revenge_shield_boost -------

	// used in `game_engine_add_starting_equipment`, struct?
	// gameplay_modifier: `revenge_shield_boost` values
	bool revenge_shield_boost_unknown2E14;
	int16 revenge_shield_boost_unknown2E16; // `object->__unknown100 *= (revenge_shield_boost_modifier_value * player->revenge_shield_boost_unknown2E16) + 1.0f`

	// used in `c_simulation_hit_marker_event_definition::apply_game_event`
	int32 revenge_shield_boost_unknown2E18; // = game_time_get();
	int32 revenge_shield_boost_player_index; // == `player_index`
	s_damage_reporting_info revenge_shield_boost_damage;
	bool revenge_shield_boost_unknown2E24;

	// pad?
	byte __data2E25[0x3];

	byte __data2E28[0x2];

	// used in `player_died`, struct?
	// `respawn_weapon_definition_indices`?
	// `respawn_equipment_definition_index`?
	// `respawn_grenades_additional`?
	// `desired_respawn_transform_valid`?
	// *(uns64*)__data2E2A = 0i64;
	// *(uns64*)&__data2E2A[0x8] = 0i64;
	// *(uns32*)&__data2E2A[0x10] = 0;
	// *(uns32*)&__data2E2A[0x16] = -1;
	// *(uns32*)&__data2E2A[0x1A] = 0;
	byte __data2E2A[0x1E];

	struct // assassination info
	{
		int32 weak_assassination_unit_index;
		bool is_assassination_victim;
		real_point3d assassination_authoritative_position;
		real_vector3d assassination_authoritative_forward;
		c_typed_opaque_data<struct s_simulation_unit_melee_damage_event_data, sizeof(_simulation_unit_melee_damage_event_data), __alignof(_simulation_unit_melee_damage_event_data) - 1> melee_damage_event_data;
	};

	c_static_array<s_player_shot_info, 8> shot_info;
	int16 spawn_count;
	byte __pad2F06[0x2];
};
static_assert(sizeof(player_datum) == 0x2F08);
static_assert(0x0002 == OFFSETOF(player_datum, __unknown2));
static_assert(0x0004 == OFFSETOF(player_datum, flags));
static_assert(0x0008 == OFFSETOF(player_datum, player_identifier));
static_assert(0x0010 == OFFSETOF(player_datum, left_game_time));
static_assert(0x0014 == OFFSETOF(player_datum, machine_identifier));
static_assert(0x0024 == OFFSETOF(player_datum, machine_index));
static_assert(0x0026 == OFFSETOF(player_datum, machine_user_index));
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
static_assert(0x2D94 == OFFSETOF(player_datum, multiplayer.player_display_index));
static_assert(0x2D98 == OFFSETOF(player_datum, multiplayer.teleporter_index));
static_assert(0x2D9A == OFFSETOF(player_datum, multiplayer.in_vehicle_timer));
static_assert(0x2D9B == OFFSETOF(player_datum, multiplayer.without_unit_timer));
static_assert(0x2D9C == OFFSETOF(player_datum, multiplayer.with_unit_timer));
static_assert(0x2D9D == OFFSETOF(player_datum, multiplayer.was_crouched));
static_assert(0x2D9E == OFFSETOF(player_datum, multiplayer.is_crouched));
static_assert(0x2D9F == OFFSETOF(player_datum, multiplayer.pad));
static_assert(0x2DA0 == OFFSETOF(player_datum, multiplayer.player_display_count));
static_assert(0x2DA2 == OFFSETOF(player_datum, multiplayer.remaining_lives));
static_assert(0x2DA4 == OFFSETOF(player_datum, multiplayer.last_betrayer_player_index));
static_assert(0x2DAC == OFFSETOF(player_datum, multiplayer.player_traits));
static_assert(0x2DC8 == OFFSETOF(player_datum, multiplayer.powerup_pickup_time));
static_assert(0x2DD4 == OFFSETOF(player_datum, multiplayer.dead_camera_target_player_index));
static_assert(0x2DD8 == OFFSETOF(player_datum, multiplayer.map_editor_data.player_locked_for_manipulation));
static_assert(0x2DD9 == OFFSETOF(player_datum, multiplayer.map_editor_data.pad));
static_assert(0x2DDC == OFFSETOF(player_datum, multiplayer.map_editor_data.roll_distance_amount));
static_assert(0x2DE4 == OFFSETOF(player_datum, multiplayer.map_editor_data.yaw_pitch_angles));
static_assert(0x2DEC == OFFSETOF(player_datum, multiplayer.king_player_info));
static_assert(0x2DEC == OFFSETOF(player_datum, multiplayer.juggernaut_player_info));
static_assert(0x2DF4 == OFFSETOF(player_datum, nemesis_mechanics_nemesis_counts));
static_assert(0x2DF4 == OFFSETOF(player_datum, nemesis_mechanics_avenger_counts));
static_assert(0x2E14 == OFFSETOF(player_datum, revenge_shield_boost_unknown2E14));
static_assert(0x2E16 == OFFSETOF(player_datum, revenge_shield_boost_unknown2E16));
static_assert(0x2E18 == OFFSETOF(player_datum, revenge_shield_boost_unknown2E18));
static_assert(0x2E1C == OFFSETOF(player_datum, revenge_shield_boost_player_index));
static_assert(0x2E20 == OFFSETOF(player_datum, revenge_shield_boost_damage));
static_assert(0x2E24 == OFFSETOF(player_datum, revenge_shield_boost_unknown2E24));
static_assert(0x2E25 == OFFSETOF(player_datum, __data2E25));
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
#pragma pack(pop)

struct players_global_data
{
	int32 players_in_game_count;
	bool input_inhibited;
	bool input_mostly_inhibited;
	bool weapon_pickup_inhibited;
	bool sprint_inhibited;
	bool equipment_use_inhibited;
	uns32 machine_valid_mask;
	s_machine_identifier machine_identifiers[k_maximum_machines];
	bool local_machine_exists;
	__declspec(align(1)) s_machine_identifier local_machine_identifier;
	int32 local_machine_index;
	bool scripted_dont_allow_respawning;
	int16 respawn_failure_reason;
	bool force_player_positions_initialize;
	real_point3d switching_player_position;
	real_vector3d switching_player_forward;
	int32 begin_zone_set_switch_trigger_volume_index;
	int32 commit_zone_set_switch_trigger_volume_index;
	int16 zone_set_switch_check_recursive_ticks;
	int32 zone_set_switch_player_index;
	int32 zone_set_switch_previous_zone_set_index;
	uns32 disabled_zone_set_switch_trigger_volume_flags[32];
	uns16 terminal_completed_flags;
	uns16 terminal_accessed_flags;
	bool terminal_being_read;
	uns32 combined_pvs[8];
	uns32 combined_pvs_local[8];
};
static_assert(sizeof(players_global_data) == 0x234);
static_assert(0x000 == OFFSETOF(players_global_data, players_in_game_count));
static_assert(0x004 == OFFSETOF(players_global_data, input_inhibited));
static_assert(0x005 == OFFSETOF(players_global_data, input_mostly_inhibited));
static_assert(0x006 == OFFSETOF(players_global_data, weapon_pickup_inhibited));
static_assert(0x007 == OFFSETOF(players_global_data, sprint_inhibited));
static_assert(0x008 == OFFSETOF(players_global_data, equipment_use_inhibited));
static_assert(0x00C == OFFSETOF(players_global_data, machine_valid_mask));
static_assert(0x010 == OFFSETOF(players_global_data, machine_identifiers));
static_assert(0x120 == OFFSETOF(players_global_data, local_machine_exists));
//static_assert(0x121 == OFFSETOF(players_global_data, local_machine_identifier));
static_assert(0x134 == OFFSETOF(players_global_data, local_machine_index));
static_assert(0x138 == OFFSETOF(players_global_data, scripted_dont_allow_respawning));
static_assert(0x13A == OFFSETOF(players_global_data, respawn_failure_reason));
static_assert(0x13C == OFFSETOF(players_global_data, force_player_positions_initialize));
static_assert(0x140 == OFFSETOF(players_global_data, switching_player_position));
static_assert(0x14C == OFFSETOF(players_global_data, switching_player_forward));
static_assert(0x158 == OFFSETOF(players_global_data, begin_zone_set_switch_trigger_volume_index));
static_assert(0x15C == OFFSETOF(players_global_data, commit_zone_set_switch_trigger_volume_index));
static_assert(0x160 == OFFSETOF(players_global_data, zone_set_switch_check_recursive_ticks));
static_assert(0x164 == OFFSETOF(players_global_data, zone_set_switch_player_index));
static_assert(0x168 == OFFSETOF(players_global_data, zone_set_switch_previous_zone_set_index));
static_assert(0x16C == OFFSETOF(players_global_data, disabled_zone_set_switch_trigger_volume_flags));
static_assert(0x1EC == OFFSETOF(players_global_data, terminal_completed_flags));
static_assert(0x1EE == OFFSETOF(players_global_data, terminal_accessed_flags));
static_assert(0x1F0 == OFFSETOF(players_global_data, terminal_being_read));
static_assert(0x1F4 == OFFSETOF(players_global_data, combined_pvs));
static_assert(0x214 == OFFSETOF(players_global_data, combined_pvs_local));

class c_player_in_game_iterator
{
public:
	void begin();
	bool next();
	player_datum* get_datum();
	int32 get_index() const;
	int16 get_absolute_index() const;

protected:
	c_data_iterator<player_datum> m_iterator;
};

class c_player_with_unit_iterator
{
public:
	void begin();
	bool next();
	player_datum* get_datum();
	int32 get_index() const;
	int16 get_absolute_index() const;

protected:
	c_data_iterator<player_datum> m_iterator;
};

struct s_player_interaction;
struct s_player_action_context;

extern string_id g_player_desired_mode_override;
extern void player_override_desired_mode(int32 desired_mode_override);
extern void players_debug_render();

extern void __cdecl player_action_context_clear(s_player_action_context* action_context);
extern bool __cdecl player_consider_biped_interaction(int32 player_index, int32 biped_index, s_player_action_context* result);
extern bool __cdecl player_consider_device_interaction(int32 player_index, int32 device_index, s_player_action_context* result);
extern bool __cdecl player_consider_equipment_interaction(int32 player_index, int32 equipment_index, s_player_action_context* result);
extern bool __cdecl player_consider_unit_interaction(int32 player_index, int32 unit_index, s_player_action_context* result);
extern bool __cdecl player_consider_vehicle_interaction(int32 player_index, int32 vehicle_index, s_player_action_context* result);
extern bool __cdecl player_consider_weapon_interaction(int32 player_index, int32 weapon_index, s_player_action_context* result);
extern void __cdecl player_copy_object_appearance(int32 player_index, int32 object_index);
extern void __cdecl player_delete(int32 player_index);
extern bool __cdecl player_evaluate_interaction(int32 player_index, const s_player_interaction* interaction, s_player_interaction* current_interaction);
extern void __cdecl player_find_action_context(int32 player_index, s_player_action_context* out_action_context);
extern int32 __cdecl player_index_from_absolute_player_index(int32 player_index);
extern int32 __cdecl player_index_from_unit_index(int32 unit_index);
extern bool __cdecl player_interaction_exists(int32 player_index, uns32 object_mask, const s_player_interaction* interaction);
extern bool __cdecl player_is_local(int32 player_index);
extern bool __cdecl player_is_reading_terminal();
extern int32 __cdecl player_new(int32 player_array_index, const game_player_options* options, bool joined_in_progress);
extern void __cdecl player_positions_dispose();
extern void __cdecl player_positions_dispose_from_old_map();
extern void __cdecl player_positions_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl player_positions_initialize();
extern void __cdecl player_positions_initialize_for_new_map();
extern void __cdecl player_positions_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern real_rgb_color __cdecl player_profile_get_rgb_color(int32 color_index);
extern void __cdecl player_set_facing(int32 player_index, const real_vector3d* facing);
extern void __cdecl player_set_unit_index(int32 player_index, int32 unit_index);
extern bool __cdecl player_spawn(int32 player_index, const real_point3d* position, const real32* facing);
extern void __cdecl player_suppress_action(int32 player_index, int32 player_suppress_action_type);
extern bool __cdecl player_teleport(int32 player_index, int32 object_index, const real_point3d* position);
extern bool __cdecl player_try_to_drop_weapon(int32 player_index, bool primary_weapon);
extern int32 __cdecl player_unit_get_representation_index(int32 unit_index);
extern int32 __cdecl player_unit_get_control_index(int32 unit_index);
extern void __cdecl player_update_invisibility(int32 player_index);
extern bool __cdecl players_all_are_dead();
extern bool __cdecl players_any_are_dead();
extern bool __cdecl players_any_are_in_the_air(int32* out_unit_index);
extern bool __cdecl players_any_are_near_death(int32* out_unit_index);
extern void __cdecl players_compute_combined_pvs(s_game_cluster_bit_vectors* combined_pvs, bool local_only, t_cluster_activation_reason* activation_reason);
extern void __cdecl players_detach_from_map();
extern void __cdecl players_dispose();
extern void __cdecl players_dispose_from_old_map();
extern void __cdecl players_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl players_finish_creation();
extern int32 __cdecl players_get_active_and_in_game_count(bool include_joined_in_progress_players);
extern void __cdecl players_handle_deleted_player_internal(int32 player_index);
extern void __cdecl players_initialize();
extern void __cdecl players_initialize_for_new_map();
extern void __cdecl players_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl players_rebuild_user_mapping(bool force_rebuild);
extern void __cdecl players_set_local_machine(const s_machine_identifier* machine_identifier);
extern void __cdecl players_set_machines(uns32 new_machine_valid_mask, const s_machine_identifier* new_machine_identifiers);
extern void __cdecl players_verify();
extern void __cdecl players_update_after_game(const struct simulation_update* update);
extern void __cdecl players_update_before_game(const struct simulation_update* update);
extern s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player);
extern s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player);
extern int32 multiplayer_universal_data_get_absolute_equipment_block_index(const char* name);
extern int32 multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(const char* region_or_biped_name, const char* selection_name);
extern int16 multiplayer_universal_data_get_absolute_weapons_selection_block_index(const char* selection_name);

extern void apply_player_representation_fixup();

