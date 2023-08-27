#include "hs/hs_runtime.hpp"

#include "cseries/cseries.hpp"
#include "render/render_debug.hpp"
#include "render/render_lights.hpp"
#include "scenario/scenario.hpp"

bool debug_trigger_volumes = false;

//bool hs_evaluate(long, long, hs_destination_pointer, long*)
bool __cdecl hs_evaluate(long thread_index, long expression_index, long destination_pointer, long* out_cast)
{
	return INVOKE(0x00594510, hs_evaluate, thread_index, expression_index, destination_pointer, out_cast);
}

//long hs_runtime_script_begin(short, e_hs_script_type, e_hs_thread_type)
long __cdecl hs_runtime_script_begin(short script_index, short script_type, char thread_type)
{
	//return INVOKE(0x00598050, hs_runtime_script_begin, script_index, script_type, thread_type);

	s_scenario* scenario = global_scenario_get();
	if (!scenario)
		return NONE;

	if (script_index < 0 || script_index >= scenario->scripts.count())
		return NONE;

	hs_script& script = scenario->scripts[script_index];
	if (script.script_type != script_type)
		return NONE;

	long thread_index = hs_thread_new(thread_type, script_index, true);
	if (thread_index != NONE)
		hs_evaluate(thread_index, script.root_expression_index, 3, nullptr);

	return thread_index;
}

//long hs_thread_new(e_hs_thread_type, long, bool)
long __cdecl hs_thread_new(char thread_index, long script_index, bool deterministic)
{
	return INVOKE(0x00598E70, hs_thread_new, thread_index, script_index, deterministic);
}

void __cdecl render_debug_trigger_volumes()
{
	if (!debug_trigger_volumes)
		return;

	long trigger_volume_index = 0;
	for (scenario_trigger_volume& trigger_volume : global_scenario_get()->trigger_volumes)
	{
		real_matrix4x3 matrix{};
		if (trigger_volume_get_matrix(&trigger_volume, &matrix))
		{
			vector3d extents = *(vector3d*)&trigger_volume.extents;
			vector3d extents_transformed{};
			matrix4x3_transform_vector(&matrix, &extents, &extents_transformed);

			for (long i = 0; i < 6; i++)
			{
				real_point3d points[4]{};
				vector3d v23{};
				vector3d v24{};

				short v21 = short(i / 2);
				if (i % 2)
				{
					point_from_line3d(&matrix.center, &extents_transformed, 1.0f, points);
					v23.n[(v21 + 1) % 3] = -extents.n[(v21 + 1) % 3];
					v24.n[(v21 + 2) % 3] = -extents.n[(v21 + 2) % 3];
				}
				else
				{
					points[0] = matrix.center;
					v23.n[(v21 + 1) % 3] = extents.n[(v21 + 1) % 3];
					v24.n[(v21 + 2) % 3] = extents.n[(v21 + 2) % 3];
				}

				matrix4x3_transform_vector(&matrix, &v23, &v23);
				matrix4x3_transform_vector(&matrix, &v24, &v24);
				point_from_line3d(points, &v23, 1.0f, &points[1]);
				point_from_line3d(&points[1], &v24, 1.0f, &points[2]);
				point_from_line3d(&points[2], &v23, -1.0f, &points[3]);

				// #TODO: find all the places `hs_debug_data` should be used and add it back
				//if ((hs_debug_data[trigger_volume_index >> 5] & (1 << (trigger_volume_index & 0x1F))) != 0) // this is a static flags
				//{
				//	render_debug_polygon_edges(points, NUMBEROF(points), global_real_argb_blue);
				//}
				//else
				{
					real_argb_color polygon_color = *global_real_argb_blue;
					polygon_color.alpha = 0.15f;
					render_debug_polygon_edges(points, NUMBEROF(points), global_real_argb_red);
					render_debug_polygon(points, NUMBEROF(points), &polygon_color);
				}
			}

			real_point3d name_point{};
			point_from_line3d(&matrix.center, &extents_transformed, 0.5f, &name_point);

			//render_camera const* rasterizer_camera = c_player_view::get_global_player_view()->get_rasterizer_camera();
			//
			//vector3d name_vector{};
			//vector_from_points3d(&rasterizer_camera->position, &name_point, &name_vector);
			//scale_vector3d(&name_vector, 0.95f, &name_vector);
			//
			//s_collision_test_flags flags = { 0x2D051855 };
			//collision_result collision;
			//if (!collision_test_vector(flags, &rasterizer_camera->position, &name_vector, NONE, NONE, &collision))
			{
				// #TODO: find all the places `hs_debug_data` should be used and add it back
				//if ((hs_debug_data[volume_index >> 5] & (1 << (volume_index & 0x1F))) != 0)
				//{
				//	render_debug_string_at_point(&name_point, trigger_volume.name.get_string(), global_real_argb_yellow);
				//}
				//else
				{
					render_debug_string_at_point(&name_point, trigger_volume.name.get_string(), global_real_argb_white);
				}
			}
		}

		trigger_volume_index++;
	}

}
