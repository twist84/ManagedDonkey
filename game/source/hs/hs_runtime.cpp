#include "hs/hs_runtime.hpp"

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "interface/interface.hpp"
#include "main/main.hpp"
#include "memory/thread_local.hpp"
#include "physics/collisions.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario.hpp"
#include "text/draw_string.hpp"

REFERENCE_DECLARE(0x023FF440, bool, debug_scripting);
REFERENCE_DECLARE(0x023FF441, bool, debug_globals);
REFERENCE_DECLARE(0x023FF442, bool, debug_globals_all);
REFERENCE_DECLARE(0x023FF443, bool, hs_verbose);

// this is potentially at address `0x023FF444`,
// there's a 512 byte + 4 byte gap there between `hs_verbose` and `g_typecasting_procedures`
bool debug_global_variables[512]{};

bool breakpoints_enabled = true;

bool debug_trigger_volumes = false;
hs_debug_data_definition hs_debug_data{};

bool __cdecl hs_can_cast(short actual_type, short desired_type)
{
	return INVOKE(0x005942F0, hs_can_cast, actual_type, desired_type);

	//ASSERT(actual_type == _hs_passthrough || hs_type_valid(actual_type));
	//ASSERT(hs_type_valid(desired_type));
	//
	//if (actual_type == _hs_passthrough || actual_type == desired_type)
	//	return true;
	//
	//if (HS_TYPE_IS_OBJECT(desired_type))
	//{
	//	if (HS_TYPE_IS_OBJECT(actual_type))
	//		return hs_object_type_can_cast(actual_type - _hs_type_object, desired_type - _hs_type_object);
	//
	//	if (HS_TYPE_IS_OBJECT_NAME(desired_type))
	//		return hs_object_type_can_cast(actual_type - _hs_type_object_name, desired_type - _hs_type_object);
	//
	//	return actual_type == _hs_type_ai;
	//}
	//
	//if (HS_TYPE_IS_OBJECT_NAME(desired_type))
	//	return hs_object_type_can_cast(actual_type - _hs_type_object_name, desired_type - _hs_type_object_name);
	//
	//if (HS_TYPE_IS_OBJECT_NAME(actual_type))
	//	return g_typecasting_procedures[desired_type][actual_type] != NULL;
	//
	//return false;
}

//bool hs_evaluate(long, long, hs_destination_pointer, long*)
bool __cdecl hs_evaluate(long thread_index, long expression_index, long destination_pointer, long* out_cast)
{
	return INVOKE(0x00594510, hs_evaluate, thread_index, expression_index, destination_pointer, out_cast);
}

bool __cdecl hs_object_type_can_cast(short actual_type, short desired_type)
{
	return INVOKE(0x005973A0, hs_object_type_can_cast, actual_type, desired_type);

	//ASSERT(actual_type >= 0 && actual_type < NUMBER_OF_HS_OBJECT_TYPES);
	//ASSERT(desired_type >= 0 && desired_type < NUMBER_OF_HS_OBJECT_TYPES);
	//
	//short actual_type_mask = hs_object_type_masks[actual_type];
	//short desired_type_mask = hs_object_type_masks[desired_type];
	//
	//return actual_type_mask == (actual_type_mask & desired_type_mask);
}

bool __cdecl hs_runtime_nondeterministic_threads_running()
{
	return INVOKE(0x00597DD0, hs_runtime_nondeterministic_threads_running);
}

long __cdecl hs_runtime_script_begin(short script_index, e_hs_script_type script_type, e_hs_thread_type thread_type)
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

void __cdecl hs_runtime_update()
{
	INVOKE(0x005980C0, hs_runtime_update);
}

long __cdecl hs_thread_new(e_hs_thread_type thread_type, long script_index, bool deterministic)
{
	return INVOKE(0x00598E70, hs_thread_new, thread_type, script_index, deterministic);
}

void __cdecl inspect_internal(short type, long value, char* buffer, short buffer_size)
{
	INVOKE(0x00599280, inspect_internal, type, value, buffer, buffer_size);
}

void __cdecl render_debug_scripting()
{
	TLS_DATA_GET_VALUE_REFERENCE(hs_thread_deterministic_data);
	TLS_DATA_GET_VALUE_REFERENCE(hs_runtime_globals);

	//char buffer[10240]{};
	//
	//main_set_single_thread_request_flag(4, debug_scripting || debug_globals);
	//if (debug_scripting && *hs_thread_deterministic_data)
	//{
	//	short tab_stops[] { 250, 300, 350 };
	//	c_rasterizer_draw_string draw_string{};
	//	c_font_cache_mt_safe font_cache{};
	//	interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
	//	draw_string.set_tab_stops(tab_stops, NUMBEROF(tab_stops));
	//
	//	csnzprintf(buffer, sizeof(buffer), "script name\tLine \tsleep time\tfunction");
	//	hs_thread_iterator_new(thread_iterator, true, true);
	//	while (true)
	//	{
	//		long thread_index = hs_thread_iterator_next(thread_iterator);
	//		if (thread_index == NONE)
	//			break;
	//
	//		thread_render_debug_scripting(thread_index, buffer, sizeof(buffer));
	//	}
	//
	//	draw_string.draw(&font_cache, buffer);
	//}
	//
	//render_debug_scripting_globals();
}

void thread_render_debug_scripting(long thread_index, char* buffer, long buffer_size)
{
	//hs_thread* thread = hs_thread_get(thread_index);
	//
	//ASSERT(buffer);
	//ASSERT(buffer_size > 0);
}

void __cdecl render_debug_scripting_globals()
{
	//TLS_DATA_GET_VALUE_REFERENCE(hs_global_data);
	//
	//char buffer[10240]{};
	//
	//if (debug_globals && *hs_global_data)
	//{
	//	c_rasterizer_draw_string draw_string{};
	//	c_font_cache_mt_safe font_cache{};
	//	short tab_stops[]{ 300 };
	//	s_scenario* scenario = global_scenario_get();
	//	interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
	//	draw_string.set_tab_stops(tab_stops, NUMBEROF(tab_stops));
	//
	//	csnzprintf(buffer, sizeof(buffer), "|n|n|nglobal name|tvalue");
	//	for (short global_index = 0; global_index < scenario->globals.count(); global_index++)
	//	{
	//		hs_global_internal& global = scenario->globals[global_index];
	//		csnzappendf(buffer, sizeof(buffer), "|n%s|t", global.name.get_string());
	//		long runtime_index = hs_runtime_index_from_global_designator(global_index & 0x7FFF);
	//		hs_global_runtime* runtime_global = datum_get_absolute(*hs_global_data, runtime_index);
	//		char global_value[1024]{};
	//		inspect_internal(global.type.get(), runtime_global->storage, global_value, sizeof(global_value));
	//		csnzappendf(buffer, sizeof(buffer), "%s", global_value);
	//	}
	//	buffer[1024] = 0;
	//
	//	draw_string.draw(&font_cache, buffer);
	//}
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

				if (hs_debug_data.activated_trigger_volumes.test(trigger_volume_index))
				{
					render_debug_polygon_edges(points, NUMBEROF(points), global_real_argb_blue);
				}
				else
				{
					real_argb_color polygon_color = *global_real_argb_blue;
					polygon_color.alpha = 0.15f;
					render_debug_polygon_edges(points, NUMBEROF(points), global_real_argb_red);
					render_debug_polygon(points, NUMBEROF(points), &polygon_color);
				}
			}

			real_point3d name_point{};
			point_from_line3d(&matrix.center, &extents_transformed, 0.5f, &name_point);

			render_camera const* rasterizer_camera = c_player_view::get_current()->get_rasterizer_camera();
			
			vector3d name_vector{};
			vector_from_points3d(&rasterizer_camera->position, &name_point, &name_vector);
			scale_vector3d(&name_vector, 0.95f, &name_vector);
			
			collision_result collision;
			if (!collision_test_vector(_collision_test_for_line_of_sight_obstruction_flags, &rasterizer_camera->position, &name_vector, NONE, NONE, &collision))
			{
				real_argb_color const* color = hs_debug_data.activated_trigger_volumes.test(trigger_volume_index) ? global_real_argb_yellow : global_real_argb_white;
				render_debug_string_at_point(&name_point, trigger_volume.name.get_string(), color);
			}
		}

		trigger_volume_index++;
	}

}
