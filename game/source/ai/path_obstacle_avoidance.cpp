#include "ai/path_obstacle_avoidance.hpp"

#include "ai/path_obstacles.hpp"
#include "main/console.hpp"
#include "render/render_debug.hpp"

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

//.text:014DF710 ; bool __cdecl actor_test_ignorable_obstacles(long, struct obstacles const*, short, bool, real_point3d const*, c_sector_ref, vector3d const*, real, real, long*, short*)
//.text:014DF830 ; real __cdecl heap_cost(obstacle_path*, short)
//.text:014DF860 ; void __cdecl heap_down(obstacle_path*, short)
//.text:014DF970 ; bool __cdecl heap_insert(obstacle_path*, short)
//.text:014DF9C0 ; short __cdecl heap_left_index(short)
//.text:014DF9D0 ; short __cdecl heap_parent_index(short)
//.text:014DF9E0 ; short __cdecl heap_remove(obstacle_path*)
//.text:014DFA30 ; short __cdecl heap_right_index(short)
//.text:014DFA40 ; void __cdecl heap_up(obstacle_path*, short)
//.text:014DFAE0 ; vector3d* __cdecl inverse_project_vector3d(vector3d const*, short, bool, vector3d*)
//.text:014DFB30 ; short __cdecl obstacle_from_disc(struct obstacles const*, short)
//.text:014DFB60 ; short __cdecl path_add_step(obstacle_path*, real_point3d const*, c_sector_ref, short, bool, bool, short, real, short)
//.text:014DFEB0 ; short __cdecl path_add_steps(obstacle_path*, short, short)
//.text:014E04D0 ; short __cdecl path_add_turning_point_step(obstacle_path*, short, real_point3d const*, short, word, bool, bool)
//.text:014E0800 ; bool __cdecl path_avoid_obstacles(long, struct path_state*, short, struct path_step const*, bool, short*, struct path_step*, bool*, long*, short*, bool*)
//.text:014E17D0 ; bool __cdecl path_find(struct path_state const*, obstacle_path*, bool, struct obstacles const*, real, real_point3d const*, c_sector_ref, real_point3d const*, c_sector_ref, short, bool, bool, bool)

struct step* __cdecl path_get_step(obstacle_path* path, short step_index)
{
	return INVOKE(0x014E1970, path_get_step, path, step_index);
}

short __cdecl path_get_step_index(obstacle_path* path, short heap_index)
{
	return INVOKE(0x014E1990, path_get_step_index, path, heap_index);
}

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

//.text:014E2070 ; void __cdecl step_failed(obstacle_path*, short, short)

void render_debug_obstacle_path()
{
	if (debug_obstacle_path)
	{
		if (!debugging_system_has_focus())
		{
			if (input_key_frames_down(_key_code_o, _input_type_game))
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

				obstacles_new(&debug_obstacle_path_obstacles);
			}

			if (input_key_frames_down(_key_code_i, _input_type_game))
				path_iterate(&debug_obstacle_path_path, debug_obstacle_final_step);
		}

		render_debug_obstacles(&debug_obstacle_path_obstacles, debug_obstacle_path_radius);
		render_debug_path(&debug_obstacle_path_path);
	}
}

void render_debug_path(obstacle_path const* path)
{
	if (path->step_count <= 0)
		return;
	
	c_static_flags<64> step_flags{};
	step_flags.clear_range(path->step_count);
	
	short step_index0 = path->__unknown1E;
	while (step_index0 != 0xFFFF)
	{
		struct step* step0 = path_get_step((obstacle_path*)path, step_index0);
		step_flags.set(step_index0, true);
		step_index0 = step0->__unknown2C;
	}
	
	for (short step_index = 0; step_index < path->step_count; step_index++)
	{
		struct step* step1 = path_get_step((obstacle_path*)path, step_index);
		if (step1->__unknown2C == 0xFFFF)
			continue;
	
		struct step* step2 = path_get_step((obstacle_path*)path, step1->__unknown2C);
	
		real v15 = 0.0f;
		real v10 = 0.0f;
	
		//for (short disc_index = 0; disc_index < path->obstacles->disc_count; disc_index++)
		//{
		//	if (path->obstacles->discs[disc_index].obstacle_index == step1->obstacle_index)
		//	{
		//		v15 = path->obstacles->discs[disc_index].__unknown14;
		//		break;
		//	}
		//}
		//for (short disc_index = 0; disc_index < path->obstacles->disc_count; disc_index++)
		//{
		//	if (path->obstacles->discs[disc_index].obstacle_index == step2->obstacle_index)
		//	{
		//		v10 = path->obstacles->discs[disc_index].__unknown14;
		//		break;
		//	}
		//}
	
		real_point3d point0{};
		real_point3d point1{};
	
		point0.x = step2->__unknown0.x;
		point0.y = step2->__unknown0.y;
		point0.z = v10;
	
		point1.x = step1->__unknown0.x;
		point1.y = step1->__unknown0.y;
		point1.z = v15;
	
		real_argb_color const* color = global_real_argb_red;
		if (step_flags.test(step_index))
		{
			color = global_real_argb_white;
		}
		else if (step1->__unknown20)
		{
			color = global_real_argb_green;
		}
	
		render_debug_line(true, &point0, &point1, color);
	
		c_string_builder string("%.4f", step1->heap_cost);
		render_debug_string_at_point(&point1, string.get_string(), color);
	}
}

