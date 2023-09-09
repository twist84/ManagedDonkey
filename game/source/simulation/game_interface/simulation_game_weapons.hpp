#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/aim_target.hpp"
#include "replication/replication_encoding.hpp"
#include "units/units.hpp"

struct s_simulation_weapon_effect_event_data
{
	long weapon_effect_type;
	real_point3d position;
};
static_assert(sizeof(s_simulation_weapon_effect_event_data) == 0x10);

struct s_simulation_weapon_empty_click_event_data
{
	long unit_inventory_index;
	long weapon_definition_index;
};
static_assert(sizeof(s_simulation_weapon_empty_click_event_data) == 0x8);

struct s_simulation_weapon_fire_event_data
{
	long unit_inventory_index;
	long weapon_definition_index;

	long barrel_index;
	bool barrel_should_recover;

	bool aiming_vector_valid;
	vector3d aiming_vector;

	long predicted_impact_target_count;
	struct
	{
		byte __data0[0xC];
	} predicted_impact_targets[16];
	static_assert(sizeof(*predicted_impact_targets) == 0xC);

	long predicted_projectile_fire_data_count;
	struct
	{
		byte __data0[0x20];
	} predicted_projectile_fire_data[16];
	static_assert(sizeof(*predicted_projectile_fire_data) == 0x20);

	s_player_predicted_aim_assist predicted_aim_assist;
	s_aim_assist_targeting_result aim_assist_targeting;

	c_aim_target_simulation_entity simulation_entity;

	real projectile_error;
	word unit_zoom_level;
	dword random_seed;
	bool generated_from_host_live_fire;
};
static_assert(sizeof(s_simulation_weapon_fire_event_data) == 0x354);

struct s_simulation_unit_equipment_use_event_data
{
	long player_index;
	long consumable_index;
};
static_assert(sizeof(s_simulation_unit_equipment_use_event_data) == 0x8);

struct s_simulation_weapon_reload_event_data
{
	bool interrupt;
	bool reload_primary;
	bool reload_secondary;
};
static_assert(sizeof(s_simulation_weapon_reload_event_data) == 0x3);

struct s_simulation_weapon_pickup_event_data
{
	s_unit_weapon_set weapon_set;
	long add_weapon_type;
};
static_assert(sizeof(s_simulation_weapon_pickup_event_data) == 0x8);

struct s_simulation_weapon_put_away_event_data
{
	word unit_weapon_set_index;
	word unit_inventory_index;
	long weapon_definition_index;
};
static_assert(sizeof(s_simulation_weapon_put_away_event_data) == 0x8);

struct s_simulation_weapon_drop_event_data
{
	word unit_weapon_set_index;
	word unit_inventory_index;
	long weapon_definition_index;
};
static_assert(sizeof(s_simulation_weapon_drop_event_data) == 0x8);

