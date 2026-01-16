#pragma once

#include "tag_files/tag_groups.hpp"

struct s_dead_player_info
{
	int32 player_index;
	int32 time_of_death;
	real_point3d location;
};
COMPILE_ASSERT(sizeof(s_dead_player_info) == 0x14);

struct s_netgame_goal_influencer
{
	int32 type;
	int32 object_index;
};
COMPILE_ASSERT(sizeof(s_netgame_goal_influencer) == 0x8);

struct s_netgame_goal_spawn_influence
{
	real32 full_weight_radius; // wu
	real32 fall_off_radius; // wu
	real32 upper_cylinder_height; // wu
	real32 lower_cylinder_height; // wu
	real32 weight;
};
COMPILE_ASSERT(sizeof(s_netgame_goal_spawn_influence) == 0x14);

struct s_player_spawn_influence
{
	real32 full_weight_radius; // wu
	real32 fall_off_radius; // wu
	real32 upper_height; // wu
	real32 lower_height; // wu
	real32 weight;
};
COMPILE_ASSERT(sizeof(s_player_spawn_influence) == 0x14);

struct s_weapon_spawn_influence
{
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> weapon;
	real32 full_weight_range;
	real32 fall_off_range;
	real32 fall_off_cone_radius;
	real32 weight;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_weapon_spawn_influence) == 0x20);

struct s_vehicle_spawn_influence
{
	c_typed_tag_reference<VEHICLE_TAG, INVALID_TAG> vehicle;
	real32 pill_radius;
	real32 lead_time;
	real32 minimum_velocity;
	real32 weight;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_vehicle_spawn_influence) == 0x20);

struct s_projectile_spawn_influence
{
	c_typed_tag_reference<PROJECTILE_TAG, INVALID_TAG> projectile;
	real32 lead_time;
	real32 collision_cylinder_radius;
	real32 weight;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_projectile_spawn_influence) == 0x1C);

struct s_equipment_spawn_influence
{
	c_typed_tag_reference<EQUIPMENT_TAG, INVALID_TAG> equipment;
	real32 weight;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_equipment_spawn_influence) == 0x14);

