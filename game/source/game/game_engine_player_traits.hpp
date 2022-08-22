#pragma once
#pragma pack(push, 1)

#include "cseries/cseries.hpp"

enum e_damage_resistance_percentage_setting
{
	// UNCHANGED
	_damage_resistance_percentage_setting_unchanged = 0,

	// 0.1
	_damage_resistance_percentage_setting_10_percent,

	// 0.5
	_damage_resistance_percentage_setting_50_percent,

	// 0.9
	_damage_resistance_percentage_setting_90_percent,

	// 1.0
	_damage_resistance_percentage_setting_100_percent,

	// 1.1
	_damage_resistance_percentage_setting_110_percent,

	// 1.5
	_damage_resistance_percentage_setting_150_percent,

	// 2.0
	_damage_resistance_percentage_setting_200_percent,

	// 3.0
	_damage_resistance_percentage_setting_300_percent,

	// 5.0
	_damage_resistance_percentage_setting_500_percent,

	// 10.0
	_damage_resistance_percentage_setting_1000_percent,

	// 20.0
	_damage_resistance_percentage_setting_2000_percent,

	// 1000.0
	_damage_resistance_percentage_setting_invulnerable,


	k_damage_resistance_percentage_settings
};

enum e_shield_recharge_rate_percentage_setting
{
	// UNCHANGED
	_shield_recharge_rate_percentage_setting_unchanged = 0,

	// -0.25
	_shield_recharge_rate_percentage_setting_negative_25_percent,

	// -0.1
	_shield_recharge_rate_percentage_setting_negative_10_percent,

	// -0.05
	_shield_recharge_rate_percentage_setting_negative_5_percent,

	// 0.0
	_shield_recharge_rate_percentage_setting_0_percent,

	// 0.5
	_shield_recharge_rate_percentage_setting_50_percent,

	// 0.9
	_shield_recharge_rate_percentage_setting_90_percent,

	// 1.0
	_shield_recharge_rate_percentage_setting_100_percent,

	// 1.1
	_shield_recharge_rate_percentage_setting_110_percent,

	// 2.0
	_shield_recharge_rate_percentage_setting_200_percent,


	k_shield_recharge_rate_percentage_settings
};

enum e_vampirism_percentage_setting
{
	// UNCHANGED
	_vampirism_percentage_setting_unchanged = 0,

	// 0.0
	_vampirism_percentage_setting_0_percent,

	// 0.1
	_vampirism_percentage_setting_10_percent,

	// 0.25
	_vampirism_percentage_setting_25_percent,

	// 0.5
	_vampirism_percentage_setting_50_percent,

	// 1.0
	_vampirism_percentage_setting_100_percent,


	k_vampirism_percentage_settings
};

enum e_headshot_immunity_setting
{
	// UNCHANGED
	_headshot_immunity_setting_unchanged = 0,

	// ENABLED
	_headshot_immunity_setting_immune_to_headshots,

	// DISABLED
	_headshot_immunity_setting_not_immune_to_headshots,


	k_headshot_immunity_settings
};

enum e_shield_multiplier_setting
{
	// UNCHANGED
	_shield_multiplier_setting_unchanged = 0,

	// 0
	_shield_multiplier_setting_0x,

	// 1 (NORMAL)
	_shield_multiplier_setting_1x,

	// 2
	_shield_multiplier_setting_2x,

	// 3
	_shield_multiplier_setting_3x,

	// 4
	_shield_multiplier_setting_4x,


	k_shield_multiplier_settings
};

enum e_damage_modifier_percentage_setting
{
	// UNCHANGED
	_damage_modifier_percentage_setting_unchanged = 0,

	// 0.0
	_damage_modifier_percentage_setting_0_percent,

	// 0.25
	_damage_modifier_percentage_setting_25_percent,

	// 0.5
	_damage_modifier_percentage_setting_50_percent,

	// 0.75
	_damage_modifier_percentage_setting_75_percent,

	// 0.9
	_damage_modifier_percentage_setting_90_percent,

	// 1.0
	_damage_modifier_percentage_setting_100_percent,

	// 1.1
	_damage_modifier_percentage_setting_110_percent,

	// 1.25
	_damage_modifier_percentage_setting_125_percent,

	// 1.5
	_damage_modifier_percentage_setting_150_percent,

	// 2.0
	_damage_modifier_percentage_setting_200_percent,

	// 3.0
	_damage_modifier_percentage_setting_300_percent,

	// 1000.0 (FATALITY)
	_damage_modifier_percentage_setting_fatality,


	k_damage_modifier_percentage_settings
};

enum e_grenade_count_setting
{
	// UNCHANGED
	_grenade_count_setting_unchanged = 0,

	// MAP DEFAULT
	_grenade_count_setting_map_default,

	// NONE
	_grenade_count_setting_none,


	k_grenade_count_settings
};

enum e_recharging_grenades_setting
{
	// UNCHANGED
	_recharging_grenades_setting_unchanged = 0,

	// ENABLED
	_recharging_grenades_setting_enabled,

	// DISABLED
	_recharging_grenades_setting_disabled,


	k_recharging_grenades_settings
};

enum e_infinite_ammo_setting
{
	// UNCHANGED
	_infinite_ammo_setting_unchanged = 0,

	// DISABLED
	_infinite_ammo_setting_disabled,

	// ENABLED
	_infinite_ammo_setting_enabled,


	k_infinite_ammo_settings
};

enum e_weapon_pickup_setting
{
	// UNCHANGED
	_weapon_pickup_setting_unchanged = 0,

	// ENABLED
	_weapon_pickup_setting_allowed,

	// DISABLED
	_weapon_pickup_setting_disallowed,


	k_weapon_pickup_settings
};

enum e_player_speed_setting
{
	// UNCHANGED
	_player_speed_setting_unchanged = 0,

	// 0.25
	_player_speed_setting_25_percent,

	// 0.5
	_player_speed_setting_50_percent,

	// 0.75
	_player_speed_setting_75_percent,

	// 0.9
	_player_speed_setting_90_percent,

	// 1.0
	_player_speed_setting_100_percent,

	// 1.1
	_player_speed_setting_110_percent,

	// 1.25
	_player_speed_setting_125_percent,

	// 1.5
	_player_speed_setting_150_percent,

	// 2.0
	_player_speed_setting_200_percent,

	// 3.0
	_player_speed_setting_300_percent,


	k_player_speed_settings
};

enum e_player_gravity_setting
{
	// UNCHANGED
	_player_gravity_setting_unchanged = 0,

	// 0.5
	_player_gravity_setting_50_percent,

	// 0.75
	_player_gravity_setting_75_percent,

	// 1.0
	_player_gravity_setting_100_percent,

	// 1.5
	_player_gravity_setting_150_percent,

	// 2.0
	_player_gravity_setting_200_percent,


	k_player_gravity_settings
};

enum e_vehicle_usage_setting
{
	// UNCHANGED
	_vehicle_usage_setting_unchanged = 0,

	// NONE
	_vehicle_usage_setting_none,

	// PASSENGER ONLY
	_vehicle_usage_setting_passenger_only,

	// FULL
	_vehicle_usage_setting_full,


	k_vehicle_usage_settings
};

enum e_active_camo_setting
{
	// UNCHANGED
	_active_camo_setting_unchanged = 0,

	// OFF (0.0)
	_active_camo_setting_off,

	// POOR (0.33)
	_active_camo_setting_poor,

	// GOOD (0.66)
	_active_camo_setting_good,

	// INVISIBLE (1.0)
	_active_camo_setting_invisible,


	k_active_camo_settings
};

enum e_waypoint_setting
{
	// UNCHANGED
	_waypoint_setting_unchanged = 0,

	// OFF
	_waypoint_setting_off,

	// ALLIES
	_waypoint_setting_allies,

	// ALL
	_waypoint_setting_all,


	k_waypoint_settings
};

enum e_aura_setting
{
	// UNCHANGED
	_aura_setting_unchanged = 0,

	// OFF
	_aura_setting_off,

	// TEAM COLOR
	_aura_setting_team_color,

	// BLACK
	_aura_setting_black,

	// WHITE
	_aura_setting_white,


	k_aura_settings
};

enum e_forced_change_color_setting
{
	// UNCHANGED
	_forced_change_color_setting_unchanged = 0,

	// OFF
	_forced_change_color_setting_off,

	// RED
	_forced_change_color_setting_mp_team_red,

	// BLUE
	_forced_change_color_setting_mp_team_blue,

	// GREEN
	_forced_change_color_setting_mp_team_green,

	// YELLOW
	_forced_change_color_setting_mp_team_yellow,

	// PURPLE
	_forced_change_color_setting_mp_team_purple,

	// ORANGE
	_forced_change_color_setting_mp_team_orange,

	// BROWN
	_forced_change_color_setting_mp_team_brown,

	// GREY (PINK)
	_forced_change_color_setting_mp_team_grey,

	// XTRA 1 (PRIMARY COLOR)
	_forced_change_color_setting_extra1,

	// XTRA 2 (SECONDARY COLOR)
	_forced_change_color_setting_extra2,

	// XTRA 3 (TERTIARY COLOR)
	_forced_change_color_setting_extra3,

	// XTRA 4 (QUATERNARY COLOR)
	_forced_change_color_setting_extra4,


	k_forced_change_color_settings
};

enum e_motion_tracker_setting
{
	// UNCHANGED
	_motion_tracker_setting_unchanged = 0,

	// OFF
	_motion_tracker_setting_off,

	// FRIENDLY ONLY
	_motion_tracker_setting_moving_friendly_bipeds_moving_neutral_vehicles,

	// NORMAL
	_motion_tracker_setting_moving_bipeds_moving_vehicles,

	// ALWAYS
	_motion_tracker_setting_all_bipeds_moving_vehicles,


	k_motion_tracker_settings
};

enum e_motion_tracker_range_setting
{
	// UNCHANGED
	_motion_tracker_range_setting_unchanged = 0,

	// 10m
	_motion_tracker_range_setting_10_meters,

	// 15m
	_motion_tracker_range_setting_15_meters,

	// 25m
	_motion_tracker_range_setting_25_meters,

	// 50m
	_motion_tracker_range_setting_50_meters,

	// 75m
	_motion_tracker_range_setting_75_meters,

	// 100m
	_motion_tracker_range_setting_100_meters,

	// 150m
	_motion_tracker_range_setting_150_meters,


	k_motion_tracker_range_settings
};

struct c_player_trait_shield_vitality
{
	c_enum<e_damage_resistance_percentage_setting, char, k_damage_resistance_percentage_settings> m_damage_resistance;
	c_enum<e_shield_recharge_rate_percentage_setting, char, k_shield_recharge_rate_percentage_settings> m_shield_recharge_rate;
	c_enum<e_vampirism_percentage_setting, char, k_vampirism_percentage_settings> m_shield_vampirism;
	c_enum<e_headshot_immunity_setting, char, k_headshot_immunity_settings> m_headshot_immunity;
	c_enum<e_shield_multiplier_setting, char, k_shield_multiplier_settings> m_shield_multiplier;

	// elephant
	byte pad[3];
};
static_assert(sizeof(c_player_trait_shield_vitality) == 0x8);

struct c_player_trait_weapons
{
	c_enum<e_grenade_count_setting, word, k_grenade_count_settings> m_initial_grenade_count;
	char m_initial_primary_weapon;
	char m_initial_secondary_weapon;
	c_enum<e_damage_modifier_percentage_setting, char, k_damage_modifier_percentage_settings>  m_damage_modifier;
	c_enum<e_recharging_grenades_setting, char, k_recharging_grenades_settings>  m_recharging_grenades;
	c_enum<e_infinite_ammo_setting, char, k_infinite_ammo_settings>  m_infinite_ammo;
	c_enum<e_weapon_pickup_setting, char, k_weapon_pickup_settings> m_weapon_pickup;
};
static_assert(sizeof(c_player_trait_weapons) == 0x8);

struct c_player_trait_movement
{
	c_enum<e_player_speed_setting, char, k_player_speed_settings> m_speed_multiplier;
	c_enum<e_player_gravity_setting, char, k_player_gravity_settings> m_gravity_multiplier;
	c_enum<e_vehicle_usage_setting, char, k_vehicle_usage_settings> m_vehicle_usage;

	// shark
	byte pad;
};
static_assert(sizeof(c_player_trait_movement) == 0x4);

// Traits that affect the player's appearance
struct c_player_trait_appearance
{
	c_enum<e_active_camo_setting, char, k_active_camo_settings> m_active_camo_setting;
	c_enum<e_waypoint_setting, char, k_waypoint_settings> m_waypoint_setting;
	c_enum<e_aura_setting, char, k_aura_settings> m_aura_setting;
	c_enum<e_forced_change_color_setting, char, k_forced_change_color_settings> m_forced_change_color_setting;
};
static_assert(sizeof(c_player_trait_appearance) == 0x4);

// Traits that affect the HUD motion sensor behavior
struct c_player_trait_sensors
{
	c_enum<e_motion_tracker_setting, short, k_motion_tracker_settings> m_motion_tracker_settings;
	c_enum<e_motion_tracker_range_setting, short, k_motion_tracker_range_settings> m_motion_tracker_range;
};
static_assert(sizeof(c_player_trait_sensors) == 0x4);

struct c_player_traits
{
	c_player_trait_shield_vitality m_shield_traits;
	c_player_trait_weapons m_weapon_traits;
	c_player_trait_movement m_movement_traits;
	c_player_trait_appearance m_appearance_traits;
	c_player_trait_sensors m_sensor_traits;
};
static_assert(sizeof(c_player_traits) == 0x1C);

#pragma pack(pop)