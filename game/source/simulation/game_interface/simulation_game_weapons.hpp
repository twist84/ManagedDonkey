#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/aim_target.hpp"
#include "replication/replication_encoding.hpp"
#include "units/units.hpp"

struct s_simulation_weapon_effect_event_data
{
	// 0: bomb explode
	// 0: bomb defuse
	int32 weapon_effect_type;
	real_point3d position;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_effect_event_data) == 0x10);

struct s_simulation_weapon_empty_click_event_data
{
	int32 unit_inventory_index;
	int32 weapon_definition_index;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_empty_click_event_data) == 0x8);

struct s_simulation_weapon_fire_event_data
{
	int32 unit_inventory_index;
	int32 weapon_definition_index;

	int32 barrel_index;
	bool barrel_should_recover;

	bool aiming_vector_valid;
	real_vector3d aiming_vector;

	int32 predicted_impact_target_count;
	struct
	{
		byte __data0[0xC];
	} predicted_impact_targets[16];
	COMPILE_ASSERT(sizeof(*predicted_impact_targets) == 0xC);

	int32 predicted_projectile_fire_data_count;
	struct
	{
		byte __data0[0x20];
	} predicted_projectile_fire_data[16];
	COMPILE_ASSERT(sizeof(*predicted_projectile_fire_data) == 0x20);

	s_player_predicted_aim_assist predicted_aim_assist;
	s_aim_assist_targeting_result aim_assist_targeting;

	c_aim_target_simulation_entity simulation_entity;

	real32 projectile_error;
	uns16 unit_zoom_level;
	uns32 random_seed;
	bool generated_from_host_live_fire;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_fire_event_data) == 0x354);

struct s_simulation_unit_equipment_use_event_data
{
	int32 player_index;
	int32 consumable_index;
};
COMPILE_ASSERT(sizeof(s_simulation_unit_equipment_use_event_data) == 0x8);

struct s_simulation_weapon_reload_event_data
{
	bool interrupt;
	bool reload_primary;
	bool reload_secondary;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_reload_event_data) == 0x3);

struct s_simulation_weapon_pickup_event_data
{
	s_unit_weapon_set weapon_set;
	int32 unit_add_weapon_mode;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_pickup_event_data) == 0x8);

struct s_simulation_weapon_put_away_event_data
{
	uns16 unit_weapon_set_index;
	uns16 unit_inventory_index;
	int32 weapon_definition_index;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_put_away_event_data) == 0x8);

struct s_simulation_weapon_drop_event_data
{
	uns16 unit_weapon_set_index;
	uns16 unit_inventory_index;
	int32 weapon_definition_index;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_drop_event_data) == 0x8);

