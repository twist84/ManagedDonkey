#include "hs/hs_runtime.hpp"

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_library_external.hpp"
#include "interface/interface.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
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

HOOK_DECLARE(0x005942E0, hs_breakpoint);
//HOOK_DECLARE(0x005972F0, hs_macro_function_evaluate);

// this is potentially at address `0x023FF444`,
// there's a 512 byte + 4 byte gap there between `hs_verbose` and `g_typecasting_procedures`
bool debug_global_variables[512]{};

bool breakpoints_enabled = true;

bool debug_trigger_volumes = false;
hs_debug_data_definition hs_debug_data{};

//.text:00593A00 ; void __cdecl hs_inspect_boolean(short type, long value, char* buffer, long buffer_size)
//.text:00593A30 ; void __cdecl hs_inspect_real(short type, long value, char* buffer, long buffer_size)
//.text:00593A60 ; void __cdecl hs_inspect_short_integer(short type, long value, char* buffer, long buffer_size)
//.text:00593A80 ; void __cdecl hs_inspect_long_integer(short type, long value, char* buffer, long buffer_size)
//.text:00593AA0 ; void __cdecl hs_inspect_string(short type, long value, char* buffer, long buffer_size)
//.text:00593AC0 ; void __cdecl hs_inspect_enum(short type, long value, char* buffer, long buffer_size)
//.text:00593AF0 ; 
//.text:00593B10 ; 
//.text:00593B30 ; 
//.text:00593B50 ; 
//.text:00593B70 ; 
//.text:00593B90 ; 
//.text:00593BB0 ; 
//.text:00593BC0 ; 
//.text:00593BD0 ; 
//.text:00593BE0 ; 
//.text:00593BF0 ; 
//.text:00593C00 ; 
//.text:00593C10 ; 
//.text:00593C20 ; 
//.text:00593C50 ; 
//.text:00593C80 ; 
//.text:00593CA0 ; 
//.text:00593CD0 ; 
//.text:00593D00 ; 
//.text:00593D30 ; 
//.text:00593D70 ; 
//.text:00593DB0 ; 
//.text:00593DF0 ; 
//.text:00593E30 ; 
//.text:00593E70 ; 
//.text:00593EB0 ; 
//.text:00593ED0 ; 
//.text:00593F00 ; 
//.text:00593F30 ; 
//.text:00593F60 ; 
//.text:00593F90 ; 
//.text:00593FC0 ; 
//.text:00593FF0 ; 
//.text:00594020 ; 
//.text:00594050 ; 
//.text:00594080 ; 
//.text:005940B0 ; 
//.text:005940E0 ; 
//.text:00594110 ; 
//.text:00594120 ; 
//.text:00594130 ; 

long* __cdecl hs_arguments_evaluate(long thread_index, short parameter_count, short const* formal_parameters, bool a4)
{
	return INVOKE(0x00594140, hs_arguments_evaluate, thread_index, parameter_count, formal_parameters, a4);
}

void __cdecl hs_breakpoint(char const* s)
{
	//INVOKE(0x005942E0, hs_breakpoint, s);

	if (breakpoints_enabled)
	{
		debug_game_speed = 0.0f;
		char buffer[1024]{};
		csstrnzcpy(buffer, "BREAKPOINT: ", sizeof(buffer));
		csstrnzcat(buffer, s, sizeof(buffer));
		hs_print(buffer);
	}
}

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

//.text:005943A0 ; long __cdecl hs_cast(long thread_index, short actual_type, short desired_type, long value)
//.text:00594450 ; long __cdecl hs_data_to_void(long _data)
//.text:00594460 ; long* __cdecl hs_destination(hs_thread*, hs_destination_pointer destination_pointer)
//.text:005944F0 ; long __cdecl hs_enum_to_real(long _enum)

//bool hs_evaluate(long thread_index, long expression_index, hs_destination_pointer destination_pointer, long* out_cast)
bool __cdecl hs_evaluate(long thread_index, long expression_index, long destination_pointer, long* out_cast)
{
	return INVOKE(0x00594510, hs_evaluate, thread_index, expression_index, destination_pointer, out_cast);
}

//.text:00594680 ; void __cdecl hs_evaluate_arithmetic(short function_index, long thread_index, bool a3)
//.text:00594960 ; void __cdecl hs_evaluate_begin(short function_index, long thread_index, bool a3)
//.text:00594AB0 ; void __cdecl hs_evaluate_begin_random(short function_index, long thread_index, bool a3)
//.text:00594D20 ; void __cdecl hs_evaluate_debug_string(short function_index, long thread_index, bool a3)
//.text:00594FB0 ; void __cdecl hs_evaluate_equality(short function_index, long thread_index, bool a3)
//.text:005950B0 ; void __cdecl hs_evaluate_if(short function_index, long thread_index, bool a3)
//.text:005952A0 ; void __cdecl hs_evaluate_inequality(short function_index, long thread_index, bool a3)
//.text:00595450 ; void __cdecl hs_evaluate_inspect(short function_index, long thread_index, bool a3)
//.text:00595550 ; void __cdecl hs_evaluate_logical(short function_index, long thread_index, bool a3)
//.text:005956F0 ; void __cdecl hs_evaluate_object_cast_up(short function_index, long thread_index, bool a3)
//.text:005957F0 ; void __cdecl hs_evaluate_set(short function_index, long thread_index, bool a3)
//.text:00595A00 ; void __cdecl hs_evaluate_sleep(short function_index, long thread_index, bool a3)
//.text:00595C10 ; void __cdecl hs_evaluate_sleep_forever(short function_index, long thread_index, bool a3)
//.text:00595CC0 ; void __cdecl hs_evaluate_sleep_until(short function_index, long thread_index, bool a3)
//.text:00595FF0 ; void __cdecl hs_evaluate_wake(short function_index, long thread_index, bool a3)
//.text:00596070 ; long __cdecl hs_find_thread_by_name(long name)
//.text:00596130 ; long __cdecl hs_find_thread_by_script(short script_index)
//.text:005961D0 ; long __cdecl hs_global_evaluate(short global_index)
//.text:00596230 ; void __cdecl hs_global_reconcile_read(short global_index)
//.text:00596C10 ; void __cdecl hs_global_reconcile_write(short global_index)
//.text:00596F50 ; void __cdecl hs_handle_deleted_object(long object_index)
//.text:00597280 ; long __cdecl hs_long_to_boolean(long _long)
//.text:005972A0 ; long __cdecl hs_long_to_real(long _long)
//.text:005972C0 ; long __cdecl hs_boolean_to_long(long _boolean)
//.text:005972E0 ; 

long* __cdecl hs_macro_function_evaluate(short function_index, long thread_index, bool a3)
{
	return INVOKE(0x005972F0, hs_macro_function_evaluate, function_index, thread_index, a3);

	//hs_function_definition const* function = hs_function_get(function_index);
	//char const* function_name = hs_function_table_names[function_index];
	//
	//long* parameters = hs_arguments_evaluate(thread_index, function->formal_parameter_count, function->formal_parameters, a3);
	//if (parameters)
	//{
	//	TLS_DATA_GET_VALUE_REFERENCE(hs_thread_deterministic_data);
	//	hs_thread* thread = (hs_thread*)datum_get(*hs_thread_deterministic_data, thread_index);
	//	if (/*hs_verbose || */TEST_BIT(thread->stack_frame.__flagsD, 4))
	//	{
	//		static c_static_string<10240> string;
	//		string.print("%s: %s ", hs_thread_format(thread_index), function_name);
	//		if (parameters)
	//		{
	//			for (long parameter_index = 0; parameter_index < function->formal_parameter_count; parameter_index++)
	//			{
	//				static char parameter[100]{};
	//				inspect_internal(function->formal_parameters[parameter_index], parameters[parameter_index], parameter, sizeof(parameter));
	//				string.append_print("%s ", parameter);
	//			}
	//		}
	//
	//		long expression_index = *((dword*)thread->stack_data + 1);
	//		if (expression_index != NONE)
	//			string.append_print("   (line #%i)", hs_syntax_get(expression_index)->line_number);
	//
	//		generate_event(_event_level_warning, "hs: %s", string.get_string());
	//	}
	//}
	//return parameters;
}

//.text:00597320 ; long __cdecl hs_object_index_from_name_index(long, short)
//.text:00597330 ; long __cdecl hs_object_name_to_object_list(long name_index)
//.text:00597370 ; long __cdecl hs_object_to_object_list(long object_index)

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

//.text:005973D0 ; long __cdecl hs_real_to_long(long _real)
//.text:005973E0 ; long __cdecl hs_real_to_short(long _real)
//.text:00597400 ; void __cdecl hs_reset_scripts()
//.text:005974D0 ; void __cdecl hs_restore_from_saved_game(long)
//.text:005974E0 ; void __cdecl hs_return(long, long)
//.text:005975C0 ; 
//.text:005975D0 ; long __cdecl hs_runtime_command_script_begin(short)
//.text:00597640 ; void __cdecl hs_runtime_delete_internal_global_datums()
//.text:005976C0 ; void __cdecl hs_runtime_dirty()
//.text:00597730 ; void __cdecl hs_runtime_dispose()
//.text:00597750 ; void __cdecl hs_runtime_dispose_from_old_map()
//.text:005977A0 ; bool __cdecl hs_runtime_evaluate(long, long*, bool)
//.text:00597870 ; char const* __cdecl hs_runtime_get_executing_thread_name()
//.text:005978A0 ; long __cdecl hs_runtime_index_from_global_designator(long)
//.text:005978D0 ; void __cdecl hs_runtime_initialize()
//.text:00597A80 ; void __cdecl hs_runtime_initialize_for_new_map()
//.text:00597C70 ; void __cdecl hs_runtime_initialize_threads()
//.text:00597CF0 ; bool __cdecl hs_runtime_initialized()
//.text:00597D10 ; long __cdecl hs_runtime_internal_evaluate(long)

bool __cdecl hs_runtime_nondeterministic_threads_running()
{
	return INVOKE(0x00597DD0, hs_runtime_nondeterministic_threads_running);
}

//.text:00597DE0 ; void __cdecl hs_runtime_push_script(short)
//.text:00597E60 ; void __cdecl hs_runtime_require_gc()
//.text:00597E80 ; void __cdecl hs_runtime_require_object_list_gc()
//.text:00597EA0 ; void __cdecl hs_runtime_reset()
//.text:00597F00 ; void __cdecl hs_runtime_reset_time(long)
//.text:00597FC0 ; 

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

//.text:005981D0 ; void __cdecl hs_script_evaluate(short function_index, long thread_index, bool a3)
//.text:00598570 ; bool __cdecl hs_script_finished(char const*)
//.text:005985C0 ; bool __cdecl hs_script_started(char const*)
//.text:00598610 ; 
//.text:00598620 ; long __cdecl hs_scripting_get_executing_thread_index()
//.text:00598640 ; void __cdecl hs_scripting_kill_all_threads()
//.text:005986F0 ; void __cdecl hs_scripting_kill_running_thread(long)
//.text:00598740 ; long __cdecl hs_short_to_boolean(long _short)
//.text:00598760 ; long __cdecl hs_short_to_long(long _short)
//.text:00598770 ; long __cdecl hs_short_to_real(long _short)
//.text:00598790 ; 
//.text:005987B0 ; 
//.text:005987D0 ; void* __cdecl hs_stack_allocate(long, long, long, hs_stack_pointer*)
//.text:005988C0 ; 
//.text:005988E0 ; 
//.text:00598900 ; 
//.text:00598940 ; bool __cdecl hs_stack_push(long)
//.text:005989E0 ; long __cdecl hs_string_to_boolean(long _string)
//.text:00598A10 ; hs_syntax_node* __cdecl hs_syntax_get(long)
//.text:00598A30 ; long __cdecl hs_syntax_nth(long, short)
//.text:00598A60 ; void __cdecl hs_thread_delete(long, bool)

char const* __cdecl hs_thread_format(long thread_index)
{
	return INVOKE(0x00598A90, hs_thread_format, thread_index);
}

//.text:00598B10 ; 
//.text:00598B20 ; void __cdecl hs_thread_iterator_new(s_hs_thread_iterator*, bool, bool)
//.text:00598B70 ; long __cdecl hs_thread_iterator_next(s_hs_thread_iterator*)
//.text:00598BC0 ; void __cdecl hs_thread_main(long)

long __cdecl hs_thread_new(e_hs_thread_type thread_type, long script_index, bool deterministic)
{
	return INVOKE(0x00598E70, hs_thread_new, thread_type, script_index, deterministic);
}

//.text:00598F30 ; hs_stack_frame* __cdecl hs_thread_stack(hs_thread*)
//.text:00598F50 ; 
//.text:00598F70 ; 
//.text:00598FC0 ; void __cdecl hs_typecasting_table_initialize()
//.text:00599170 ; void __cdecl hs_wake(long, long)
//.text:00599250 ; bool __cdecl hs_wake_by_name(char const*)

void __cdecl inspect_internal(short type, long value, char* buffer, short buffer_size)
{
	INVOKE(0x00599280, inspect_internal, type, value, buffer, buffer_size);
}

//.text:005992C0 ; 
//.text:005992E0 ; 
//.text:00599320 ; 
//.text:00599360 ; 
//.text:005993A0 ; 
//.text:005993E0 ; 
//.text:00599420 ; 
//.text:00599460 ; 
//.text:00599470 ; void __cdecl thread_update_sleep_time_for_reset(long, long)

void render_debug_scripting()
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

void render_debug_scripting_globals()
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

void render_debug_trigger_volumes()
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
					point_from_line3d(&matrix.position, &extents_transformed, 1.0f, points);
					v23.n[(v21 + 1) % 3] = -extents.n[(v21 + 1) % 3];
					v24.n[(v21 + 2) % 3] = -extents.n[(v21 + 2) % 3];
				}
				else
				{
					points[0] = matrix.position;
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
			point_from_line3d(&matrix.position, &extents_transformed, 0.5f, &name_point);

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
