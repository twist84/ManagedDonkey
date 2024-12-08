#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"

#define MAXIMUM_OBSTACLE_AVOIDANCE_STEPS 64

struct step
{
	real_point3d point;
	c_sector_ref sector_ref;
	real_vector2d direction;
	real distance;
	word obstacle_index;
	word disc_index;
	bool obstacle_direction_index;
	bool turning_point;
	short obstructed_goal_step_indices[2];
	real total_distance;
	short previous_step_index;
	short child_count;
	short children_failed_count;
	short blocking_obstacle_index;
	bool failed;
};
static_assert(sizeof(struct step) == 0x38);

struct obstacle_path
{
	real radius;
	bool ignore_broken_surfaces;
	struct obstacles const* obstacles;
	real_point3d goal;
	c_sector_ref goal_sector_ref;
	short goal_obstacle_index;
	short goal_step_index;
	short best_goal_blocked_step_index;
	real best_goal_blocked_distance;
	short projection_axis;
	bool projection_sign;
	bool goal_found_exactly;
	bool ignore_optional;
	short step_count;
	step steps[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	short heap_count;
	short heap[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	short blocking_obstacle_index;
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
	bool ignore_optional);

extern void render_debug_path(obstacle_path const* path);
extern void render_debug_obstacle_path();

