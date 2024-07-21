#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "motor/sync_action.hpp"
#include "objects/damage_reporting.hpp"

struct s_simulation_unit_board_vehicle_event_data
{
	long boarding_seat_index;
};
static_assert(sizeof(s_simulation_unit_board_vehicle_event_data) == 0x4);

struct s_simulation_unit_pickup_event_data
{
	short unit_pickup_type;
	long unit_pickup_definition_index;
	long count;
};
static_assert(sizeof(s_simulation_unit_pickup_event_data) == 0xC);

struct s_simulation_unit_exit_vehicle_event_data
{
	long parent_seat_index;
	bool detach_on_exit;
};
static_assert(sizeof(s_simulation_unit_exit_vehicle_event_data) == 0x8);

struct s_simulation_unit_melee_damage_event_data
{
	real_point3d position;
	vector3d impact_normal;
	long damage_definition_index;
	long clang_damage_definition_index;
	short breakable_surface_set_index;
	short breakable_surface_index;
	long instanced_geometry_instance_index;
	long surface_index;
	long global_material_index;
	long object_material_index;
	real scale;
	s_damage_reporting_info damage_reporting_info;
};
static_assert(sizeof(s_simulation_unit_melee_damage_event_data) == 0x3C);

struct s_simulation_unit_assassinate_event_data
{
	s_animation_sync_action sync_action;
	s_simulation_unit_melee_damage_event_data melee_damage;
};
static_assert(sizeof(s_simulation_unit_assassinate_event_data) == 0x64);

struct s_simulation_unit_throw_initiate_event_data
{
	// 0: grenade type
	// 1: item
	// 2: projectile
	short throw_type;

	union
	{
		long grenade_type;
		long bomb_run_throw_definition_index;
		long unit_throw_definition_index;
	};
};
static_assert(sizeof(s_simulation_unit_throw_initiate_event_data) == 0x8);

struct s_simulation_unit_melee_initiate_event_data
{
	short melee_type;
	short prediction_type;
};
static_assert(sizeof(s_simulation_unit_melee_initiate_event_data) == 0x4);

struct s_simulation_unit_throw_release_event_data
{
	// 0: grenade type
	// 1: item
	// 2: projectile
	short throw_type;

	union
	{
		long grenade_type;
		long bomb_run_throw_definition_index;
		long unit_throw_definition_index;
	};

	real_point3d position;
	vector3d aiming_vector;
	c_aim_target_simulation_entity simulation_entity;
	vector3d lead_vector;
};
static_assert(sizeof(s_simulation_unit_throw_release_event_data) == 0x40);

struct s_simulation_unit_melee_clang_event_data
{
	long clang_effect_index;
	real_point3d position;
	vector3d forward;
};
static_assert(sizeof(s_simulation_unit_melee_clang_event_data) == 0x1C);

struct s_simulation_unit_enter_vehicle_event_data
{
	long parent_seat_index;
};
static_assert(sizeof(s_simulation_unit_enter_vehicle_event_data) == 0x4);

