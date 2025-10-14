#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "motor/mover.hpp"
#include "objects/objects.hpp"
#include "units/units.hpp"

struct s_giant_foot_data
{
	int32 foot_marker_name;
	real_point3d target_point;
	real_point3d source_point;
	real_point3d test_point;
	real_vector3d test_vector;
	real32 target_scale;
	int16 target_frame_index;
	uns8 foot_flags;
	uns8 state;
};
static_assert(sizeof(s_giant_foot_data) == 0x3C);

struct s_buckling_workspace
{
	point2d size;
	real_point3d points[1024];
	real_matrix4x3 bottom_center;
	real_point3d maxima;
	real_point3d minima;
};
static_assert(sizeof(s_buckling_workspace) == 0x3050);

struct _giant_datum
{
	s_giant_foot_data foot_data[4];
	real_point3d contact_points[3];
	s_buckling_workspace buckle_workspace;
	real_quaternion buckle_rotation;
	uns32 flags;
	uns8 static_physics_timer;
	real32 buckling_slider_start;
	real32 buckling_slider_velocity;
	real32 buckling_slider_acceleration;
	real32 buckling_depth;
	real32 buckling_timeout;
	real32 custom_animation_recovery_time;
	int32 foot_count;
	int32 last_known_graph_index;
	int32 weapon_timeout;
	int32 trigger_timeout;
	int32 buckle_rotation_node_index;
	int16 buckling_type;
	int16 buckling_state;
	real_matrix4x3 buckling_target;
	int32 last_pathfinding_attempt_time;
	int16 pathfinding_structure_index;
	int32 pathfinding_surface_index;
	c_sector_ref pathfinding_sector;
	int32 pathfinding_instanced_geometry_index;
	int32 pathfinding_object_index;
	uns32 pathfinding_bsp_reference;
	real_point3d pathfinding_point;
};
static_assert(sizeof(_giant_datum) == 0x3208);

struct giant_datum
{
	int32 definition_index;
	_object_datum object;
	_mover_datum mover;
	_unit_datum unit;
	_giant_datum giant;
};
static_assert(sizeof(giant_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_unit_datum) + sizeof(_giant_datum));

extern int32& g_giant_hunt_player;
extern real32& g_giant_hunting_min_radius;
extern real32& g_giant_hunting_max_radius;
extern real32& g_giant_hunting_throttle_scale;
extern int32& g_giant_weapon_wait_time;
extern int32& g_giant_weapon_trigger_time;
extern bool& g_giant_foot_ik;
extern bool& g_giant_ankle_ik;
extern bool& g_giant_elevation_control;
extern bool& g_giant_buckle_rotation;
extern real32& g_giant_custom_animation_recovery_time;

extern real32 __cdecl giant_buckling_magnitude_get(int32 giant_index);

