#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_player_traits.hpp"
#include "memory/data.hpp"
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
	} __unknown4_emblems;

	c_static_wchar_string<5> service_tag;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_player_appearance) == 0x660);

enum e_color_type
{
	_color_type_primary = 0,
	_color_type_secondary,
	_color_type_visor,
	_color_type_lights,
	_color_type_holo,

	k_color_type_count
};

enum e_armor_type
{
	_armor_type_helmet = 0,
	_armor_type_chest,
	_armor_type_shoulders,
	_armor_type_arms,
	_armor_type_legs,

	_armor_type_acc,
	_armor_type_pelvis,

	_armor_type_unknown7,
	_armor_type_unknown8,
	_armor_type_unknown9,

	k_armor_type_count
};

enum e_grenade_type
{
	_grenade_type_frag = 0,
	_grenade_type_plasma,
	_grenade_type_claymore,
	_grenade_type_firebomb,

	k_grenade_type_count
};

// `0069C3D0`, gameplay_modifier_get_name
// `0069C730`, gameplay_modifier_get_index
enum e_gameplay_modifier
{
	_gameplay_modifier_safety_booster = 0,
	_gameplay_modifier_explosives_damage,
	_gameplay_modifier_explosives_aoe_size_boost,
	_gameplay_modifier_grenade_scavenger,
	_gameplay_modifier_stamina_restore_near_death,
	_gameplay_modifier_cooldown_reset,
	_gameplay_modifier_reflex_boost,
	_gameplay_modifier_zoom_radar,
	_gameplay_modifier_cooldown_boost,
	_gameplay_modifier_enable_nemesis_mechanics,
	_gameplay_modifier_silent_sprint,
	_gameplay_modifier_vehicle_shield_regen_speed,
	_gameplay_modifier_vehicle_shield_regen_delay,
	_gameplay_modifier_scavenger_aura,
	_gameplay_modifier_armor_to_vehicle,
	_gameplay_modifier_armor_to_player_in_vehicle,
	_gameplay_modifier_armor_to_player_in_vehicle_transferred,
	_gameplay_modifier_sprint_speed,
	_gameplay_modifier_sprint_stamina,
	_gameplay_modifier_reload_speed,
	_gameplay_modifier_weapon_switch_speed,
	_gameplay_modifier_ammo_max,
	_gameplay_modifier_melee_damage,
	_gameplay_modifier_melee_resist,
	_gameplay_modifier_revenge_shield_boost,
	_gameplay_modifier_respawn_modifier,
	_gameplay_modifier_grenades_max,
	_gameplay_modifier_motion_sensor_range,
	_gameplay_modifier_shield_regen_rate,
	_gameplay_modifier_shield_regen_delay,
	_gameplay_modifier_teflon_armor,
	_gameplay_modifier_explosion_resist,
	_gameplay_modifier_vehicle_ram_damage,
	_gameplay_modifier_turret_damage,
	_gameplay_modifier_machine_gun_rof_acceleration_time,
	_gameplay_modifier_grenade_warning,
	_gameplay_modifier_consumable_duration,
	_gameplay_modifier_energy_increase_max,
	_gameplay_modifier_energy_increase_count_on_spawn,
	_gameplay_modifier_energy_regeneration_rate,
	_gameplay_modifier_assault_rifle_rounds_per_second,
	_gameplay_modifier_assault_rifle_damage,
	_gameplay_modifier_assault_rifle_ammo_capacity,
	_gameplay_modifier_assault_rifle_reload_speed,
	_gameplay_modifier_battle_rifle_rounds_per_second,
	_gameplay_modifier_battle_rifle_fire_recovery_time,
	_gameplay_modifier_battle_rifle_damage,
	_gameplay_modifier_battle_rifle_ammo_capacity,
	_gameplay_modifier_battle_rifle_reload_speed,
	_gameplay_modifier_shotgun_spread,
	_gameplay_modifier_shotgun_damage,
	_gameplay_modifier_shotgun_ammo_capacity,
	_gameplay_modifier_shotgun_reload_speed,
	_gameplay_modifier_sniper_rifle_zoom_protection,
	_gameplay_modifier_sniper_rifle_damage,
	_gameplay_modifier_sniper_rifle_ammo_capacity,
	_gameplay_modifier_sniper_rifle_reload_speed,
	_gameplay_modifier_dmr_zoom_protection,
	_gameplay_modifier_dmr_damage,
	_gameplay_modifier_dmr_ammo_capacity,
	_gameplay_modifier_dmr_reload_speed,
	_gameplay_modifier_magnum_rounds_per_second,
	_gameplay_modifier_magnum_penetration,
	_gameplay_modifier_magnum_accuracy,
	_gameplay_modifier_magnum_damage,
	_gameplay_modifier_magnum_ammo_capacity,
	_gameplay_modifier_magnum_reload_speed,
	_gameplay_modifier_plasma_pistol_charge_up_time,
	_gameplay_modifier_plasma_pistol_damage,
	_gameplay_modifier_plasma_pistol_ammo_capacity,
	_gameplay_modifier_plasma_pistol_heat_per_round,
	_gameplay_modifier_frag_grenade_aoe_size,
	_gameplay_modifier_frag_grenade_damage,
	_gameplay_modifier_frag_grenade_ammo_capacity,
	_gameplay_modifier_plasma_grenade_aoe_size,
	_gameplay_modifier_plasma_grenade_damage,
	_gameplay_modifier_plasma_grenade_ammo_capacity,
	_gameplay_modifier_mauler_damage,
	_gameplay_modifier_spiker_damage,
	_gameplay_modifier_brute_shot_damage,
	_gameplay_modifier_plasma_rifle_heat_per_round,
	_gameplay_modifier_detonate_on_player_cdt,
	_gameplay_modifier_detonate_on_vehicle_cdt,
	_gameplay_modifier_enemies_always_on_radar,
	_gameplay_modifier_plant_plasma_on_death,
	_gameplay_modifier_shield_regen_rate_near_killed_enemies,
	_gameplay_modifier_shield_recharge_on_melee_kill,
	_gameplay_modifier_frag_grenades_max,
	_gameplay_modifier_plasma_grenades_max,
	_gameplay_modifier_claymore_grenade_aoe_size,
	_gameplay_modifier_smg_damage,
	_gameplay_modifier_smg_reload_speed,
	_gameplay_modifier_vehicle_autoflip,

	k_gameplay_modifier_count
};

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
	c_enum<e_grenade_type, char, k_grenade_type_count> grenade_index;
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
	wchar_t player_name[16];

	byte __data[0x10];
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

struct s_player_configuration_from_host
{
	s_player_identifier machine_identifier;
	c_static_wchar_string<16> player_name;
	long team;
	long player_assigned_team;
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
extern long multiplayer_universal_data_get_absolute_equipment_block_index(char const* name);
extern long multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(char const* region_or_biped_name, char const* selection_name);
extern short multiplayer_universal_data_get_absolute_weapons_selection_block_index(char const* selection_name);

