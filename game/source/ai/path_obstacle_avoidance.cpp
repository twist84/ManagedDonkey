#include "ai/path_obstacle_avoidance.hpp"

#include "main/console.hpp"

bool debug_obstacle_path = false;
bool debug_obstacle_path_on_failure = false;
real_point3d debug_obstacle_path_start_point = {};
c_sector_ref debug_obstacle_path_start_sector_ref = {};
real_point3d debug_obstacle_path_goal_point = {};
c_sector_ref debug_obstacle_path_goal_sector_ref = {};
short debug_obstacle_path_projection_axis = 0;
real debug_obstacle_path_radius = 0.0f;
struct obstacles debug_obstacle_path_obstacles = {};
bool debug_obstacle_path_projection_sign = false;
bool debug_obstacle_path_finishing = false;
bool debug_ignore_broken_surfaces = false;
bool debug_obstacle_final_step = false;
obstacle_path debug_obstacle_path_path = {};

bool __cdecl path_iterate(obstacle_path* path, bool final_step)
{
	return INVOKE(0x014E19B0, path_iterate, path, final_step);
}

bool __cdecl path_new(
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
	bool a12)
{
	return INVOKE(0x014E1EB0, path_new,
		path,
		obstacles,
		ignore_broken_surfaces,
		radius,
		start,
		start_sector_ref,
		goal,
		goal_sector_ref,
		projection_axis,
		projection_sign,
		final_step,
		a12);
}

void render_debug_obstacles(struct obstacles const* obstacles, real radius)
{
	// #TODO: implement this
}

void render_debug_path(obstacle_path const* path)
{
	// #TODO: implement this
}

void render_debug_obstacle_path()
{
	if (debug_obstacle_path)
	{
		if (!debugging_system_has_focus())
		{
			if (input_key_frames_down(_key_code_r, _input_type_game))
			{
				debug_obstacle_path_on_failure = true;
				debug_obstacle_path = false;
			}

			if (input_key_frames_down(_key_code_g, _input_type_game))
			{
				debug_obstacle_path_on_failure = true;
				debug_obstacle_path = false;
			}

			if (input_key_frames_down(_key_code_n, _input_type_game))
			{
				path_new(
					&debug_obstacle_path_path,
					&debug_obstacle_path_obstacles,
					debug_ignore_broken_surfaces,
					debug_obstacle_path_radius,
					&debug_obstacle_path_start_point,
					debug_obstacle_path_start_sector_ref,
					&debug_obstacle_path_goal_point,
					debug_obstacle_path_goal_sector_ref,
					debug_obstacle_path_projection_axis,
					debug_obstacle_path_projection_sign,
					debug_obstacle_final_step,
					false);
			}

			if (input_key_frames_down(_key_code_i, _input_type_game))
				path_iterate(&debug_obstacle_path_path, debug_obstacle_final_step);
		}

		render_debug_obstacles(&debug_obstacle_path_obstacles, debug_obstacle_path_radius);
		render_debug_path(&debug_obstacle_path_path);
	}
}

