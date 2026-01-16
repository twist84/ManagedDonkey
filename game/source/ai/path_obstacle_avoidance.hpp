#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"

#define MAXIMUM_OBSTACLE_AVOIDANCE_STEPS 64

struct step
{
	real_point3d point;
	c_sector_ref sector_ref;
	real_vector2d direction;
	real32 distance;
	uns16 obstacle_index;
	uns16 disc_index;
	bool obstacle_direction_index;
	bool turning_point;
	int16 obstructed_goal_step_indices[2];
	real32 total_distance;
	int16 previous_step_index;
	int16 child_count;
	int16 children_failed_count;
	int16 blocking_obstacle_index;
	bool failed;
};
COMPILE_ASSERT(sizeof(struct step) == 0x38);

struct obstacle_path
{
	real32 radius;
	bool ignore_broken_surfaces;
	const struct obstacles* obstacles;
	real_point3d goal;
	c_sector_ref goal_sector_ref;
	int16 goal_obstacle_index;
	int16 goal_step_index;
	int16 best_goal_blocked_step_index;
	real32 best_goal_blocked_distance;
	int16 projection_axis;
	bool projection_sign;
	bool goal_found_exactly;
	bool ignore_optional;
	int16 step_count;
	step steps[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	int16 heap_count;
	int16 heap[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	int16 blocking_obstacle_index;
};
COMPILE_ASSERT(sizeof(struct obstacle_path) == 0xEB4);

extern bool debug_obstacle_path;
extern bool debug_obstacle_path_on_failure;
extern real_point3d debug_obstacle_path_start_point;
extern c_sector_ref debug_obstacle_path_start_sector_ref;
extern real_point3d debug_obstacle_path_goal_point;
extern c_sector_ref debug_obstacle_path_goal_sector_ref;
extern int16 debug_obstacle_path_projection_axis;
extern real32 debug_obstacle_path_radius;
extern struct obstacles debug_obstacle_path_obstacles;
extern bool debug_obstacle_path_projection_sign;
extern bool debug_obstacle_path_finishing;
extern bool debug_ignore_broken_surfaces;
extern bool debug_obstacle_final_step;
extern obstacle_path debug_obstacle_path_path;

extern struct step* __cdecl path_get_step(obstacle_path* path, int16 step_index);
extern int16 __cdecl path_get_step_index(obstacle_path* path, int16 heap_index);
extern bool __cdecl path_iterate(obstacle_path* path, bool final_step);
extern bool __cdecl path_new(
	obstacle_path* path,
	const struct obstacles* obstacles,
	bool ignore_broken_surfaces,
	real32 radius,
	const real_point3d* start,
	c_sector_ref start_sector_ref,
	const real_point3d* goal,
	c_sector_ref goal_sector_ref,
	int16 projection_axis,
	bool projection_sign,
	bool final_step,
	bool ignore_optional);

extern void render_debug_path(const obstacle_path* path);
extern void render_debug_obstacle_path();

