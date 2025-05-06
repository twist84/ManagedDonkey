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
HOOK_DECLARE(0x005972F0, hs_macro_function_evaluate);

t_value_type<bool> const g_cinematic_debug_mode = { .value = true };
DATA_PATCH_DECLARE(0x024B0A3E, g_cinematic_debug_mode, g_cinematic_debug_mode.bytes);

// this is potentially at address `0x023FF444`,
// there's a 512 byte + 4 byte gap there between `hs_verbose` and `g_typecasting_procedures`
bool debug_global_variables[512]{};

bool breakpoints_enabled = true;

bool debug_trigger_volumes = false;
hs_debug_data_definition hs_debug_data{};

//.text:00593A00 ; void __cdecl hs_inspect_boolean(int16 type, int32 value, char* buffer, int32 buffer_size)
//.text:00593A30 ; void __cdecl hs_inspect_real(int16 type, int32 value, char* buffer, int32 buffer_size)
//.text:00593A60 ; void __cdecl hs_inspect_short_integer(int16 type, int32 value, char* buffer, int32 buffer_size)
//.text:00593A80 ; void __cdecl hs_inspect_long_integer(int16 type, int32 value, char* buffer, int32 buffer_size)
//.text:00593AA0 ; void __cdecl hs_inspect_string(int16 type, int32 value, char* buffer, int32 buffer_size)
//.text:00593AC0 ; void __cdecl hs_inspect_enum(int16 type, int32 value, char* buffer, int32 buffer_size)
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
//.text:00593C20 ; void __cdecl __tls_set_g_hs_distributed_global_data_allocator(void*)
//.text:00593C50 ; void __cdecl __tls_set_g_hs_global_data_allocator(void*)
//.text:00593C80 ; void __cdecl __tls_set_g_hs_runtime_globals_allocator(void*)
//.text:00593CA0 ; void __cdecl __tls_set_g_hs_thread_deterministic_data_allocator(void*)
//.text:00593CD0 ; void __cdecl __tls_set_g_hs_thread_non_deterministic_data_allocator(void*)
//.text:00593D00 ; void __cdecl __tls_set_g_hs_thread_tracking_data_allocator(void*)
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

int32* __cdecl hs_arguments_evaluate(int32 thread_index, int16 parameter_count, int16 const* formal_parameters, bool a4)
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

bool __cdecl hs_can_cast(int16 actual_type, int16 desired_type)
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

//.text:005943A0 ; int32 __cdecl hs_cast(int32 thread_index, int16 actual_type, int16 desired_type, int32 value)
//.text:00594450 ; int32 __cdecl hs_data_to_void(int32 _data)
//.text:00594460 ; int32* __cdecl hs_destination(hs_thread*, hs_destination_pointer destination_pointer)
//.text:005944F0 ; int32 __cdecl hs_enum_to_real(int32 _enum)

//bool hs_evaluate(int32 thread_index, int32 expression_index, hs_destination_pointer destination_pointer, int32* out_cast)
bool __cdecl hs_evaluate(int32 thread_index, int32 expression_index, int32 destination_pointer, int32* out_cast)
{
	return INVOKE(0x00594510, hs_evaluate, thread_index, expression_index, destination_pointer, out_cast);
}

//.text:00594680 ; void __cdecl hs_evaluate_arithmetic(int16 function_index, int32 thread_index, bool a3)
//.text:00594960 ; void __cdecl hs_evaluate_begin(int16 function_index, int32 thread_index, bool a3)
//.text:00594AB0 ; void __cdecl hs_evaluate_begin_random(int16 function_index, int32 thread_index, bool a3)
//.text:00594D20 ; void __cdecl hs_evaluate_debug_string(int16 function_index, int32 thread_index, bool a3)
//.text:00594FB0 ; void __cdecl hs_evaluate_equality(int16 function_index, int32 thread_index, bool a3)
//.text:005950B0 ; void __cdecl hs_evaluate_if(int16 function_index, int32 thread_index, bool a3)
//.text:005952A0 ; void __cdecl hs_evaluate_inequality(int16 function_index, int32 thread_index, bool a3)
//.text:00595450 ; void __cdecl hs_evaluate_inspect(int16 function_index, int32 thread_index, bool a3)
//.text:00595550 ; void __cdecl hs_evaluate_logical(int16 function_index, int32 thread_index, bool a3)
//.text:005956F0 ; void __cdecl hs_evaluate_object_cast_up(int16 function_index, int32 thread_index, bool a3)
//.text:005957F0 ; void __cdecl hs_evaluate_set(int16 function_index, int32 thread_index, bool a3)
//.text:00595A00 ; void __cdecl hs_evaluate_sleep(int16 function_index, int32 thread_index, bool a3)
//.text:00595C10 ; void __cdecl hs_evaluate_sleep_forever(int16 function_index, int32 thread_index, bool a3)
//.text:00595CC0 ; void __cdecl hs_evaluate_sleep_until(int16 function_index, int32 thread_index, bool a3)
//.text:00595FF0 ; void __cdecl hs_evaluate_wake(int16 function_index, int32 thread_index, bool a3)

int32 __cdecl hs_find_thread_by_name(char const* script_name)
{
	return INVOKE(0x00596070, hs_find_thread_by_name, script_name);
}

//.text:00596130 ; int32 __cdecl hs_find_thread_by_script(int16 script_index)
//.text:005961D0 ; int32 __cdecl hs_global_evaluate(int16 global_index)
//.text:00596230 ; void __cdecl hs_global_reconcile_read(int16 global_index)
//.text:00596C10 ; void __cdecl hs_global_reconcile_write(int16 global_index)
//.text:00596F50 ; void __cdecl hs_handle_deleted_object(int32 object_index)
//.text:00597280 ; int32 __cdecl hs_long_to_boolean(int32 _long)
//.text:005972A0 ; int32 __cdecl hs_long_to_real(int32 _long)
//.text:005972C0 ; int32 __cdecl hs_boolean_to_long(int32 _boolean)
//.text:005972E0 ; 

int32* __cdecl hs_macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize)
{
	//return INVOKE(0x005972F0, hs_macro_function_evaluate, function_index, thread_index, initialize);

	hs_function_definition const* function = hs_function_get(function_index);
	char const* function_name = hs_function_table_names[function_index];
	
	int32* parameters = hs_arguments_evaluate(thread_index, function->formal_parameter_count, function->formal_parameters, initialize);
	if (parameters)
	{
		hs_thread* thread = hs_thread_get(thread_index);
		if (hs_verbose || TEST_BIT(thread->flags, _hs_thread_verbose_bit))
		{
			char buffer[10240];
			csnzprintf(buffer, sizeof(buffer), "%s: %s ", hs_thread_format(thread_index), function_name);
			if (parameters)
			{
				for (int32 parameter_index = 0; parameter_index < function->formal_parameter_count; parameter_index++)
				{
					char valuebuffer[100]{};
					inspect_internal(function->formal_parameters[parameter_index], parameters[parameter_index], valuebuffer, sizeof(valuebuffer));
					csnzappendf(buffer, sizeof(buffer), "%s ", valuebuffer);
				}
			}
	
			int32 expression_index = hs_thread_stack(thread)->expression_index;
			if (expression_index != NONE)
				csnzappendf(buffer, sizeof(buffer), "   (line #%i)", hs_syntax_get(expression_index)->line_number);
	
			event(_event_warning, "hs: %s", buffer);
		}
	}
	return parameters;
}

//.text:00597320 ; int32 __cdecl hs_object_index_from_name_index(int32, int16)
//.text:00597330 ; int32 __cdecl hs_object_name_to_object_list(int32 name_index)
//.text:00597370 ; int32 __cdecl hs_object_to_object_list(int32 object_index)

bool __cdecl hs_object_type_can_cast(int16 actual_type, int16 desired_type)
{
	return INVOKE(0x005973A0, hs_object_type_can_cast, actual_type, desired_type);

	//ASSERT(actual_type >= 0 && actual_type < NUMBER_OF_HS_OBJECT_TYPES);
	//ASSERT(desired_type >= 0 && desired_type < NUMBER_OF_HS_OBJECT_TYPES);
	//
	//int16 actual_type_mask = hs_object_type_masks[actual_type];
	//int16 desired_type_mask = hs_object_type_masks[desired_type];
	//
	//return actual_type_mask == (actual_type_mask & desired_type_mask);
}

//.text:005973D0 ; int32 __cdecl hs_real_to_long(int32 _real)
//.text:005973E0 ; int32 __cdecl hs_real_to_short(int32 _real)
//.text:00597400 ; void __cdecl hs_reset_scripts()
//.text:005974D0 ; void __cdecl hs_restore_from_saved_game(int32)
//.text:005974E0 ; void __cdecl hs_return(int32, int32)
//.text:005975C0 ; bool __cdecl hs_running_game_scripts()
//.text:005975D0 ; int32 __cdecl hs_runtime_command_script_begin(int16)
//.text:00597640 ; void __cdecl hs_runtime_delete_internal_global_datums()
//.text:005976C0 ; void __cdecl hs_runtime_dirty()
//.text:00597730 ; void __cdecl hs_runtime_dispose()
//.text:00597750 ; void __cdecl hs_runtime_dispose_from_old_map()

bool __cdecl hs_runtime_evaluate(int32 expression_index, bool display_expression_result, bool deterministic)
{
	return INVOKE(0x005977A0, hs_runtime_evaluate, expression_index, display_expression_result, deterministic);
}

char const* __cdecl hs_runtime_get_executing_thread_name()
{
	return INVOKE(0x00597870, hs_runtime_get_executing_thread_name);
}

int32 __cdecl hs_runtime_index_from_global_designator(int32 designator)
{
	return INVOKE(0x005978A0, hs_runtime_index_from_global_designator, designator);
}

//.text:005978D0 ; void __cdecl hs_runtime_initialize()
//.text:00597A80 ; void __cdecl hs_runtime_initialize_for_new_map()
//.text:00597C70 ; void __cdecl hs_runtime_initialize_threads()
//.text:00597CF0 ; bool __cdecl hs_runtime_initialized()
//.text:00597D10 ; int32 __cdecl hs_runtime_internal_evaluate(int32)

bool __cdecl hs_runtime_nondeterministic_threads_running()
{
	return INVOKE(0x00597DD0, hs_runtime_nondeterministic_threads_running);
}

//.text:00597DE0 ; void __cdecl hs_runtime_push_script(int16)
//.text:00597E60 ; void __cdecl hs_runtime_require_gc()
//.text:00597E80 ; void __cdecl hs_runtime_require_object_list_gc()
//.text:00597EA0 ; void __cdecl hs_runtime_reset()
//.text:00597F00 ; void __cdecl hs_runtime_reset_time(int32)
//.text:00597FC0 ; bool __cdecl hs_runtime_safe_to_gc()

int32 __cdecl hs_runtime_script_begin(int16 script_index, e_hs_script_type script_type, e_hs_thread_type thread_type)
{
	//return INVOKE(0x00598050, hs_runtime_script_begin, script_index, script_type, thread_type);

	struct scenario* scenario = global_scenario_get();
	if (!scenario)
		return NONE;

	if (script_index < 0 || script_index >= scenario->scripts.count)
		return NONE;

	hs_script& script = scenario->scripts[script_index];
	if (script.script_type != script_type)
		return NONE;

	int32 thread_index = hs_thread_new(thread_type, script_index, true);
	if (thread_index != NONE)
		hs_evaluate(thread_index, script.root_expression_index, 3, NULL);

	return thread_index;
}

void __cdecl hs_runtime_update()
{
	INVOKE(0x005980C0, hs_runtime_update);
}

//.text:005981D0 ; void __cdecl hs_script_evaluate(int16 function_index, int32 thread_index, bool initialize)
//.text:00598570 ; bool __cdecl hs_script_finished(char const* script_name)
//.text:005985C0 ; bool __cdecl hs_script_started(char const* script_name)
//.text:00598610 ; void __cdecl hs_scripting_debug_thread(char const* thread_name, bool enable)
//.text:00598620 ; int32 __cdecl hs_scripting_get_executing_thread_index()
//.text:00598640 ; void __cdecl hs_scripting_kill_all_threads()
//.text:005986F0 ; void __cdecl hs_scripting_kill_running_thread(int32 thread_index)
//.text:00598740 ; int32 __cdecl hs_short_to_boolean(int32 s)
//.text:00598760 ; int32 __cdecl hs_short_to_long(int32 s)
//.text:00598770 ; int32 __cdecl hs_short_to_real(int32 s)
//.text:00598790 ; hs_stack_frame* __cdecl hs_stack(hs_thread* thread, hs_stack_pointer stack_pointer)
//.text:005987B0 ; hs_stack_frame const* __cdecl hs_stack(hs_thread const* thread, hs_stack_pointer stack_pointer)
//.text:005987D0 ; void* __cdecl hs_stack_allocate(int32 thread_index, int32 size, int32 alignment_bits, hs_stack_pointer* out_reference)
//.text:005988C0 ; int32* __cdecl hs_stack_destination(hs_thread* thread, hs_stack_pointer stack_pointer)
//.text:005988E0 ; int32* __cdecl hs_stack_parameters(hs_thread* thread, hs_stack_frame* stack_frame, int32 parameter_count)
//.text:00598900 ; void __cdecl hs_stack_pop(int32 thread_index)
//.text:00598940 ; bool __cdecl hs_stack_push(int32 thread_index)
//.text:005989E0 ; int32 __cdecl hs_string_to_boolean(int32 n)
//.text:00598A10 ; hs_syntax_node* __cdecl hs_syntax_get(int32 index)
//.text:00598A30 ; int32 __cdecl hs_syntax_nth(int32 expression_index, int16 n)
//.text:00598A60 ; void __cdecl hs_thread_delete(int32 thread_index, bool validate)

char const* __cdecl hs_thread_format(int32 thread_index)
{
	return INVOKE(0x00598A90, hs_thread_format, thread_index);
}

bool __cdecl hs_thread_is_deterministic(int32 thread_index)
{
	return INVOKE(0x00598B10, hs_thread_is_deterministic, thread_index);
}

void __cdecl hs_thread_iterator_new(s_hs_thread_iterator* iterator, bool deterministic, bool non_deterministic)
{
	INVOKE(0x00598B20, hs_thread_iterator_new, iterator, deterministic, non_deterministic);
}

int32 __cdecl hs_thread_iterator_next(s_hs_thread_iterator* iterator)
{
	return INVOKE(0x00598B70, hs_thread_iterator_next, iterator);
}

void __cdecl hs_thread_main(int32 thread_index)
{
	INVOKE(0x00598BC0, hs_thread_main, thread_index);
}

int32 __cdecl hs_thread_new(e_hs_thread_type thread_type, int32 script_index, bool deterministic)
{
	return INVOKE(0x00598E70, hs_thread_new, thread_type, script_index, deterministic);
}

hs_stack_frame* __cdecl hs_thread_stack(hs_thread* thread)
{
	//return INVOKE(0x00598F30, hs_thread_stack, thread);
	return DECLFUNC(0x00598F30, hs_stack_frame*, __cdecl, hs_thread*)(thread);
}

hs_stack_frame const* __cdecl hs_thread_stack(hs_thread const* thread)
{
	//return INVOKE(0x00598F50, hs_thread_stack, thread);
	return DECLFUNC(0x00598F50, hs_stack_frame const*, __cdecl, hs_thread const*)(thread);
}

//.text:00598F70 ; void __cdecl hs_thread_try_to_delete(int32, bool)
//.text:00598FC0 ; void __cdecl hs_typecasting_table_initialize()
//.text:00599170 ; void __cdecl hs_wake(int32, int32)

bool __cdecl hs_wake_by_name(char const* script_name)
{
	return INVOKE(0x00599250, hs_wake_by_name, script_name);
}

void __cdecl inspect_internal(int16 type, int32 value, char* buffer, int16 buffer_size)
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
//.text:00599470 ; void __cdecl thread_update_sleep_time_for_reset(int32, int32)

hs_thread* hs_thread_get(int32 thread_index)
{
	hs_thread* result = DATUM_TRY_AND_GET(hs_thread_deterministic_data, hs_thread, thread_index);
	return result;
}

void hs_find_dormant_script(char const* dormant_script_name, int32* script_index_out)
{
	ASSERT(dormant_script_name);
	ASSERT(script_index_out);

	*script_index_out = NONE;

	int32 thread_index = hs_find_thread_by_name(dormant_script_name);
	if (thread_index == NONE)
		return;

	hs_thread* thread = hs_thread_get(thread_index);

	if (global_scenario_get()->scripts[thread->script_index].script_type == _hs_script_type_dormant)
		*script_index_out = thread->script_index;
}

char const* expression_get_function_name(int32 thread_index, int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	hs_thread* thread = hs_thread_get(thread_index);

	while (true)
	{
		if (expression->flags.test(_hs_syntax_node_script_bit))
			break;

		if (expression->script_index || expression_index != hs_thread_stack(thread)->expression_index)
			return hs_function_table_names[expression->function_index];

		if (hs_thread_stack(thread)->size <= 0)
			return "(invalid expression reference)";

		expression_index = expression->next_node_index;
		if (expression_index == NONE)
			return "(end of script)";

		expression = hs_syntax_get(expression_index);
	}

	if (VALID_INDEX(expression->script_index, global_scenario->scripts.count))
		return global_scenario->scripts[expression->script_index].name;

	return "unknown script";
}

void thread_render_debug_scripting(int32 thread_index, char* buffer, int32 buffer_size)
{
	hs_thread* thread = hs_thread_get(thread_index);
	
	ASSERT(buffer);
	ASSERT(buffer_size > 0);

	if ((((thread->sleep_until + 0x80000000) & 0x80000000) != 0 || thread->sleep_until == HS_SLEEP_COMMAND_SCRIPT_ATOM) && !TEST_BIT(thread->flags, 3))
	{
		csnzappendf(buffer, buffer_size, "\r\n%s\t", hs_thread_format(thread_index));

		if (hs_thread_stack(thread)->expression_index == NONE)
			csnzappendf(buffer, buffer_size, "-\t");
		else
			csnzappendf(buffer, buffer_size, "%i\t", hs_syntax_get(hs_thread_stack(thread)->expression_index)->line_number);

		if (thread->sleep_until == HS_SLEEP_COMMAND_SCRIPT_ATOM)
			csnzappendf(buffer, buffer_size, "ATOM\t");
		else
			csnzappendf(buffer, buffer_size, "%d\t", thread->sleep_until ? thread->sleep_until - game_time_get() : 0);

		if (thread->stack.stack_offset && thread->sleep_until != HS_SLEEP_INDEFINITE && hs_thread_stack(thread)->expression_index != NONE)
		{
			char const* function_name = expression_get_function_name(thread_index, hs_thread_stack(thread)->expression_index);
			if (function_name)
				csstrnzcat(buffer, function_name, buffer_size);
		}
	}
}

void render_debug_scripting()
{
	//debug_scripting = true;
	//main_set_single_thread_request_flag(_single_thread_for_hs_debug, debug_scripting || debug_globals);

	if (debug_scripting)
	{
		char buffer[10240]{};
	
		int16 tab_stops[] { 250, 300, 350 };
		c_rasterizer_draw_string draw_string{};
		c_font_cache_mt_safe font_cache{};
		interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
		draw_string.set_tab_stops(tab_stops, NUMBEROF(tab_stops));

		csnzprintf(buffer, sizeof(buffer), "script name\tLine \tsleep time\tfunction");

		s_hs_thread_iterator iterator{};
		hs_thread_iterator_new(&iterator, true, true);
		for (int32 thread_index = hs_thread_iterator_next(&iterator); thread_index != NONE; thread_index = hs_thread_iterator_next(&iterator))
			thread_render_debug_scripting(thread_index, buffer, sizeof(buffer));
	
		draw_string.draw(&font_cache, buffer);
	}
	
	render_debug_scripting_globals();
}

void render_debug_scripting_globals()
{
	if (!debug_globals)
		return;

	if (hs_global_data)
	{
		char buffer[10240]{};

		c_rasterizer_draw_string draw_string{};
		c_font_cache_mt_safe font_cache{};
		int16 tab_stops[]{ 300 };
		struct scenario* scenario = global_scenario_get();
		interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
		draw_string.set_tab_stops(tab_stops, NUMBEROF(tab_stops));

		csnzprintf(buffer, sizeof(buffer), "|n|n|nglobal name|tvalue");
		for (int32 global_index = 0; global_index < scenario->globals.count; global_index++)
		{
			hs_global_internal& global = scenario->globals[global_index];
			csnzappendf(buffer, sizeof(buffer), "|n%s|t", global.name);

			int32 runtime_index = hs_runtime_index_from_global_designator(global_index);
			hs_global_runtime* runtime_global = DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime, runtime_index);

			char valuebuffer[1024]{};
			inspect_internal(global.type, runtime_global->value, valuebuffer, sizeof(valuebuffer));
			csnzappendf(buffer, sizeof(buffer), "%s", valuebuffer);
		}
		buffer[1024] = 0;

		draw_string.draw(&font_cache, buffer);
	}
}

void render_debug_trigger_volumes()
{
	if (!debug_trigger_volumes)
		return;

	int32 trigger_volume_index = 0;
	for (scenario_trigger_volume& trigger_volume : global_scenario_get()->trigger_volumes)
	{
		real_matrix4x3 matrix{};
		if (trigger_volume_get_matrix(&trigger_volume, &matrix))
		{
			real_vector3d extents = *(real_vector3d*)&trigger_volume.extents;
			real_vector3d world_extent{};
			matrix4x3_transform_vector(&matrix, &extents, &world_extent);

			for (int32 i = 0; i < 6; i++)
			{
				real_point3d sides[4]{};
				real_vector3d v23{};
				real_vector3d v24{};

				int16 v21 = int16(i / 2);
				if (i % 2)
				{
					point_from_line3d(&matrix.position, &world_extent, 1.0f, sides);
					v23.n[(v21 + 1) % 3] = -extents.n[(v21 + 1) % 3];
					v24.n[(v21 + 2) % 3] = -extents.n[(v21 + 2) % 3];
				}
				else
				{
					sides[0] = matrix.position;
					v23.n[(v21 + 1) % 3] = extents.n[(v21 + 1) % 3];
					v24.n[(v21 + 2) % 3] = extents.n[(v21 + 2) % 3];
				}

				matrix4x3_transform_vector(&matrix, &v23, &v23);
				matrix4x3_transform_vector(&matrix, &v24, &v24);
				point_from_line3d(sides, &v23, 1.0f, &sides[1]);
				point_from_line3d(&sides[1], &v24, 1.0f, &sides[2]);
				point_from_line3d(&sides[2], &v23, -1.0f, &sides[3]);

				if (hs_debug_data.activated_trigger_volumes.test(trigger_volume_index))
				{
					render_debug_polygon_edges(sides, NUMBEROF(sides), global_real_argb_blue);
				}
				else
				{
					real_argb_color polygon_color = *global_real_argb_blue;
					polygon_color.alpha = 0.15f;
					render_debug_polygon_edges(sides, NUMBEROF(sides), global_real_argb_red);
					render_debug_polygon(sides, NUMBEROF(sides), &polygon_color);
				}
			}

			real_point3d name_point{};
			point_from_line3d(&matrix.position, &world_extent, 0.5f, &name_point);

			render_camera const* rasterizer_camera = c_player_view::get_current()->get_rasterizer_camera();
			
			real_vector3d name_vector{};
			vector_from_points3d(&rasterizer_camera->position, &name_point, &name_vector);
			scale_vector3d(&name_vector, 0.95f, &name_vector);
			
			collision_result result;
			if (!collision_test_vector(_collision_test_for_line_of_sight_obstruction_flags, &rasterizer_camera->position, &name_vector, NONE, NONE, &result))
			{
				real_argb_color const* color = hs_debug_data.activated_trigger_volumes.test(trigger_volume_index) ? global_real_argb_yellow : global_real_argb_white;
				render_debug_string_at_point(&name_point, trigger_volume.name.get_string(), color);
			}
		}

		trigger_volume_index++;
	}
}
