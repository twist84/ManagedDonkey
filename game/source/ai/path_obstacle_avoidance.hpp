#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"

#define MAXIMUM_OBSTACLE_AVOIDANCE_STEPS 64

struct step
{
	real_point3d __unknown0;
	c_sector_ref __unknownC;
	vector2d __unknown10;
	real __unknown18;
	word obstacle_index;
	word __unknown1E;
	bool __unknown20;
	bool turning_point;
	word __unknown22[2];
	byte __data26[2];
	real heap_cost;
	word __unknown2C; // step_index
	short __unknown2E;
	short __unknown30;
	short __unknown32;
	bool __unknown34;
	byte __data35[3];
};
static_assert(sizeof(struct step) == 0x38);

struct obstacle_path
{
	real radius;

	bool ignore_broken_surfaces;
	byte __pad5[0x3];

	struct obstacles* obstacles;
	real_point3d goal_point;
	c_sector_ref goal_sector_ref;
	word obstacle_index;
	word __unknown1E; // step_index

	short __unknown20;
	byte __pad22[0x2];

	real __unknown24;
	short projection_axis;
	bool projection_sign;
	bool __unknown2B;

	bool __unknown2C;
	byte __pad2D[0x1];

	short step_count;
	step steps[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];

	short heap_count;
	short heap[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];

	short __unknownEB2;
};
static_assert(sizeof(struct obstacle_path) == 0xEB4);

extern bool debug_obstacle_path;
extern bool debug_obstacle_path_on_failure;
extern real_point3d debug_obstacle_path_start_point;
extern c_sector_ref debug_obstacle_path_start_sector_ref;
extern real_point3d debug_obstacle_path_goal_point;
extern c_sector_ref debug_obstacle_path_goal_sector_ref;
extern short debug_obstacle_path_projection_axis;
extern real debug_obstacle_path_radius;
extern struct obstacles debug_obstacle_path_obstacles;
extern bool debug_obstacle_path_projection_sign;
extern bool debug_obstacle_path_finishing;
extern bool debug_ignore_broken_surfaces;
extern bool debug_obstacle_final_step;
extern obstacle_path debug_obstacle_path_path;

extern struct step* __cdecl path_get_step(obstacle_path* path, short step_index);
extern short __cdecl path_get_step_index(obstacle_path* path, short heap_index);
extern bool __cdecl path_iterate(obstacle_path* path, bool final_step);
extern bool __cdecl path_new(
	obstacle_path* path,
	struct obstacles const* obstacles,
	bool ignore_broken_surfaces,
	real radius,
	real_point3d const* start,
	c_sector_ref start_sector_ref,
	real_point3d const* goal,
	c_sector_ref goal_sector_ref,
	short projection_axis,
	bool projection_sign,
	bool final_step,
	bool a12);

extern void render_debug_path(obstacle_path const* path);
extern void render_debug_obstacle_path();

