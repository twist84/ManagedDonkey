#include "hs/hs_runtime.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "editor/editor_stubs.hpp"
#include "hs/hs.hpp"
#include "hs/hs_compile.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "hs/hs_glue.hpp"
#include "hs/hs_library_external.hpp"
#include "hs/hs_library_internal_compile.hpp"
#include "hs/hs_looper.hpp"
#include "interface/interface.hpp"
#include "interface/user_interface.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "math/random_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/network_utilities.hpp"
#include "physics/collisions.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario.hpp"
#include "text/draw_string.hpp"

#include <math.h>

REFERENCE_DECLARE_ARRAY(0x018A1090, hs_type_inspector, hs_type_inspectors, NUMBER_OF_HS_NODE_TYPES);
REFERENCE_DECLARE(0x023FF440, bool, debug_scripting);
REFERENCE_DECLARE(0x023FF441, bool, debug_globals);
REFERENCE_DECLARE(0x023FF442, bool, debug_globals_all);
REFERENCE_DECLARE(0x023FF443, bool, hs_verbose);// = true;
REFERENCE_DECLARE_ARRAY(0x023FF444, bool, debug_global_variables, 512);
REFERENCE_DECLARE_2D_ARRAY(0x023FF648, typecasting_procedure, g_typecasting_procedures, NUMBER_OF_HS_NODE_TYPES, NUMBER_OF_HS_NODE_TYPES);

bool __cdecl hs_evaluate_runtime(int32 thread_index, int32 expression_index, hs_destination_pointer destination_pointer, int32* out_cast)
{
	return hs_evaluate(thread_index, expression_index, destination_pointer, out_cast);
}

hs_stack_frame* __cdecl hs_stack_0(hs_thread* thread, hs_stack_pointer stack_pointer)
{
	return hs_stack(thread, stack_pointer);
}

const hs_stack_frame* __cdecl hs_stack_1(const hs_thread* thread, hs_stack_pointer stack_pointer)
{
	return hs_stack(thread, stack_pointer);
}

HOOK_DECLARE(0x00593A00, hs_inspect_boolean);
HOOK_DECLARE(0x00593A30, hs_inspect_real);
HOOK_DECLARE(0x00593A60, hs_inspect_short_integer);
HOOK_DECLARE(0x00593A80, hs_inspect_long_integer);
HOOK_DECLARE(0x00593AA0, hs_inspect_string);
HOOK_DECLARE(0x00593AC0, hs_inspect_enum);
HOOK_DECLARE(0x00593C20, __tls_set_g_hs_distributed_global_data_allocator);
HOOK_DECLARE(0x00593C50, __tls_set_g_hs_global_data_allocator);
HOOK_DECLARE(0x00593C80, __tls_set_g_hs_runtime_globals_allocator);
HOOK_DECLARE(0x00593CA0, __tls_set_g_hs_thread_deterministic_data_allocator);
HOOK_DECLARE(0x00593CD0, __tls_set_g_hs_thread_non_deterministic_data_allocator);
HOOK_DECLARE(0x00593D00, __tls_set_g_hs_thread_tracking_data_allocator);
HOOK_DECLARE(0x00594140, hs_arguments_evaluate);
HOOK_DECLARE(0x005942E0, hs_breakpoint);
//HOOK_DECLARE(0x005942F0, hs_can_cast);
//HOOK_DECLARE(0x005943A0, hs_cast);
//HOOK_DECLARE(0x00594450, hs_data_to_void);
HOOK_DECLARE(0x00594460, hs_destination);
//HOOK_DECLARE(0x005944F0, hs_enum_to_real);
HOOK_DECLARE(0x00594510, hs_evaluate_runtime);
HOOK_DECLARE(0x00594680, hs_evaluate_arithmetic);
HOOK_DECLARE(0x00594960, hs_evaluate_begin);
HOOK_DECLARE(0x00594AB0, hs_evaluate_begin_random);
HOOK_DECLARE(0x00594D20, hs_evaluate_debug_string);
HOOK_DECLARE(0x00594FB0, hs_evaluate_equality);
HOOK_DECLARE(0x005950B0, hs_evaluate_if);
HOOK_DECLARE(0x005952A0, hs_evaluate_inequality);
HOOK_DECLARE(0x00595450, hs_evaluate_inspect);
HOOK_DECLARE(0x00595550, hs_evaluate_logical);
HOOK_DECLARE(0x005956F0, hs_evaluate_object_cast_up);
HOOK_DECLARE(0x005957F0, hs_evaluate_set);
HOOK_DECLARE(0x00595A00, hs_evaluate_sleep);
HOOK_DECLARE(0x00595C10, hs_evaluate_sleep_forever);
HOOK_DECLARE(0x00595CC0, hs_evaluate_sleep_until);
HOOK_DECLARE(0x00595FF0, hs_evaluate_wake);
HOOK_DECLARE(0x00596070, hs_find_thread_by_name);
HOOK_DECLARE(0x00596130, hs_find_thread_by_script);
HOOK_DECLARE(0x005961D0, hs_global_evaluate);
HOOK_DECLARE(0x00596230, hs_global_reconcile_read);
HOOK_DECLARE(0x00596C10, hs_global_reconcile_write);
HOOK_DECLARE(0x00596F50, hs_handle_deleted_object);
//HOOK_DECLARE(0x00597280, hs_long_to_boolean);
//HOOK_DECLARE(0x005972A0, hs_long_to_real);
//HOOK_DECLARE(0x005972C0, hs_long_to_short);
HOOK_DECLARE(0x005972F0, hs_macro_function_evaluate);
HOOK_DECLARE(0x00597320, hs_object_index_from_name_index);
HOOK_DECLARE(0x00597330, hs_object_name_to_object_list);
HOOK_DECLARE(0x00597370, hs_object_to_object_list);
HOOK_DECLARE(0x005973A0, hs_object_type_can_cast);
//HOOK_DECLARE(0x005973D0, hs_real_to_long);
//HOOK_DECLARE(0x005973E0, hs_real_to_short);
HOOK_DECLARE(0x00597400, hs_reset_scripts);
HOOK_DECLARE(0x005974D0, hs_restore_from_saved_game);
HOOK_DECLARE(0x005974E0, hs_return);
HOOK_DECLARE(0x005975C0, hs_running_game_scripts);
HOOK_DECLARE(0x005975D0, hs_runtime_command_script_begin);
HOOK_DECLARE(0x00597640, hs_runtime_delete_internal_global_datums);
HOOK_DECLARE(0x005976C0, hs_runtime_dirty);
HOOK_DECLARE(0x00597730, hs_runtime_dispose);
HOOK_DECLARE(0x00597750, hs_runtime_dispose_from_old_map);
HOOK_DECLARE(0x005977A0, hs_runtime_evaluate);
HOOK_DECLARE(0x00597870, hs_runtime_get_executing_thread_name);
HOOK_DECLARE(0x005978A0, hs_runtime_index_from_global_designator);
HOOK_DECLARE(0x005978D0, hs_runtime_initialize);
HOOK_DECLARE(0x00597A80, hs_runtime_initialize_for_new_map);
HOOK_DECLARE(0x00597C70, hs_runtime_initialize_threads);
HOOK_DECLARE(0x00597CF0, hs_runtime_initialized);
HOOK_DECLARE(0x00597D10, hs_runtime_internal_evaluate);
HOOK_DECLARE(0x00597DD0, hs_runtime_nondeterministic_threads_running);
HOOK_DECLARE(0x00597DE0, hs_runtime_push_script);
HOOK_DECLARE(0x00597E60, hs_runtime_require_gc);
HOOK_DECLARE(0x00597E80, hs_runtime_require_object_list_gc);
HOOK_DECLARE(0x00597EA0, hs_runtime_reset);
HOOK_DECLARE(0x00597F00, hs_runtime_reset_time);
HOOK_DECLARE(0x00597FC0, hs_runtime_safe_to_gc);
HOOK_DECLARE(0x00598050, hs_runtime_script_begin);
HOOK_DECLARE(0x005980C0, hs_runtime_update);
HOOK_DECLARE(0x005981D0, hs_script_evaluate);
HOOK_DECLARE(0x00598570, hs_script_finished);
HOOK_DECLARE(0x005985C0, hs_script_started);
HOOK_DECLARE(0x00598610, hs_scripting_debug_thread);
HOOK_DECLARE(0x00598620, hs_scripting_get_executing_thread_index);
HOOK_DECLARE(0x00598640, hs_scripting_kill_all_threads);
HOOK_DECLARE(0x005986F0, hs_scripting_kill_running_thread);
//HOOK_DECLARE(0x00598740, hs_short_to_boolean);
//HOOK_DECLARE(0x00598760, hs_short_to_long);
//HOOK_DECLARE(0x00598770, hs_short_to_real);
HOOK_DECLARE(0x00598790, hs_stack_0);
HOOK_DECLARE(0x005987B0, hs_stack_1);
HOOK_DECLARE(0x005987D0, hs_stack_allocate);
HOOK_DECLARE(0x005988C0, hs_stack_destination);
HOOK_DECLARE(0x005988E0, hs_stack_parameters);
HOOK_DECLARE(0x00598900, hs_stack_pop);
HOOK_DECLARE(0x00598940, hs_stack_push);
//HOOK_DECLARE(0x005989E0, hs_string_to_boolean);
HOOK_DECLARE(0x00598A10, hs_syntax_get);
HOOK_DECLARE(0x00598A30, hs_syntax_nth);
HOOK_DECLARE(0x00598A60, hs_thread_delete);
HOOK_DECLARE(0x00598A90, hs_thread_format);
HOOK_DECLARE(0x00598B10, hs_thread_is_deterministic);
HOOK_DECLARE(0x00598B20, hs_thread_iterator_new);
HOOK_DECLARE(0x00598B70, hs_thread_iterator_next);
HOOK_DECLARE(0x00598BC0, hs_thread_main);
HOOK_DECLARE(0x00598E70, hs_thread_new);
HOOK_DECLARE(0x00598F70, hs_thread_try_to_delete);
HOOK_DECLARE(0x00598FC0, hs_typecasting_table_initialize);
HOOK_DECLARE(0x00599170, hs_wake);
HOOK_DECLARE(0x00599250, hs_wake_by_name);
HOOK_DECLARE(0x00599280, inspect_internal);
HOOK_DECLARE(0x00599470, thread_update_sleep_time_for_reset);

bool g_run_game_scripts = true;
bool breakpoints_enabled = true;

bool debug_trigger_volumes = false;
hs_debug_data_definition hs_debug_data{};

inline static bool script_error(int32 thread_index, const char* message, const char* condition)
{
	event(_event_warning, "script %s needs to be recompiled. (%s: %s)",
		hs_thread_format(thread_index),
		message ? message : "no reason given.",
		condition);

	return false;
}

inline static bool script_error2(int32 thread_index, const char* message, const char* condition)
{
	event(_event_warning, "a problem occurred while executing the script %s: %s (%s)",
		hs_thread_format(thread_index),
		message ? message : "no reason given.",
		condition);

	return false;
}

#define SCRIPT_COMPILE_ERROR(THREAD_INDEX, CONDITION, MESSAGE) ((CONDITION) || script_error((THREAD_INDEX), (MESSAGE), #CONDITION))
#define SCRIPT_EXECUTION_ERROR(THREAD_INDEX, CONDITION, MESSAGE) ((CONDITION) || script_error2((THREAD_INDEX), (MESSAGE), #CONDITION))

const char* __cdecl expression_get_function_name(int32 thread_index, int32 expression_index)
{
	hs_thread* thread = hs_thread_get(thread_index);

	const char* result = "unknown script";

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	for (; !TEST_BIT(expression->flags, _hs_syntax_node_script_bit); expression = hs_syntax_get(expression_index))
	{
		if (expression->function_index || expression_index != hs_thread_stack(thread)->expression_index)
		{
			result = hs_function_get(expression->function_index)->name;
			break;
		}

		if (hs_thread_stack(thread)->size <= 0)
		{
			result = "(invalid expression reference)";
			break;
		}

		expression_index = expression->next_node_index;
		if (expression_index == NONE)
		{
			result = "(end of script)";
			break;
		}
	}

	if (VALID_INDEX(expression->script_index, global_scenario->hs_scripts.count))
	{
		hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario->hs_scripts, expression->script_index, hs_script);

		result = script->name;
	}

	return result;
}

void __cdecl hs_inspect_boolean(int16 type, int32 value, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00593A00, hs_inspect_boolean, type, value, buffer, buffer_size);

	ASSERT(type == _hs_type_boolean);
	bool value_ = *(bool*)&value;
	csstrnzcpy(buffer, value_ ? "true" : "false", buffer_size);
}

void __cdecl hs_inspect_real(int16 type, int32 value, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00593A30, hs_inspect_real, type, value, buffer, buffer_size);

	ASSERT(type == _hs_type_real);
	real32 value_ = *(real32*)&value;
	csnzprintf(buffer, buffer_size, "%f", value_);
}

void __cdecl hs_inspect_short_integer(int16 type, int32 value, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00593A60, hs_inspect_short_integer, type, value, buffer, buffer_size);

	ASSERT(type == _hs_type_short_integer);
	int16 value_ = *(int16*)&value;
	csnzprintf(buffer, buffer_size, "%d", value_);
}

void __cdecl hs_inspect_long_integer(int16 type, int32 value, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00593A80, hs_inspect_long_integer, type, value, buffer, buffer_size);

	ASSERT(type == _hs_type_long_integer);
	csnzprintf(buffer, buffer_size, "%ld", value);
}

void __cdecl hs_inspect_string(int16 type, int32 value, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00593AA0, hs_inspect_string, type, value, buffer, buffer_size);

	ASSERT(type == _hs_type_string);
	const char* value_ = *(const char**)&value;
	csstrnzcpy(buffer, value_, buffer_size);
}

void __cdecl hs_inspect_enum(int16 type, int32 value, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00593AC0, hs_inspect_enum, type, value, buffer, buffer_size);

	int16 enum_value = (int16)value;
	const hs_enum_definition* enum_definition = &hs_enum_table[type - FIRST_HS_ENUM_TYPE];

	ASSERT(HS_TYPE_IS_ENUM(type));
	ASSERT(VALID_INDEX(enum_value, enum_definition->count));
	csstrnzcpy(buffer, enum_definition->identifiers[enum_value], buffer_size);
}

//.text:00593AF0 ; tls
//.text:00593B10 ; tls
//.text:00593B30 ; tls
//.text:00593B50 ; tls
//.text:00593B70 ; tls
//.text:00593B90 ; tls
//.text:00593BB0 ; tls
//.text:00593BC0 ; tls
//.text:00593BD0 ; tls
//.text:00593BE0 ; tls
//.text:00593BF0 ; tls
//.text:00593C00 ; tls
//.text:00593C10 ; tls

void __cdecl __tls_set_g_hs_distributed_global_data_allocator(void* new_address)
{
	//INVOKE(0x00593C20, __tls_set_g_hs_distributed_global_data_allocator, new_address);

	data_set_new_base_address(&hs_distributed_global_data.get_restricted_data_array_address(), (s_data_array*)new_address);
}

void __cdecl __tls_set_g_hs_global_data_allocator(void* new_address)
{
	//INVOKE(0x00593C50, __tls_set_g_hs_global_data_allocator, new_address);

	data_set_new_base_address(&hs_global_data.get_restricted_data_array_address(), (s_data_array*)new_address);
}

void __cdecl __tls_set_g_hs_runtime_globals_allocator(void* new_address)
{
	//INVOKE(0x00593C80, __tls_set_g_hs_runtime_globals_allocator, new_address);

	hs_runtime_globals = (s_hs_runtime_globals*)new_address;
}

void __cdecl __tls_set_g_hs_thread_deterministic_data_allocator(void* new_address)
{
	//INVOKE(0x00593CA0, __tls_set_g_hs_thread_deterministic_data_allocator, new_address);

	data_set_new_base_address(&hs_thread_deterministic_data.get_restricted_data_array_address(), (s_data_array*)new_address);
}

void __cdecl __tls_set_g_hs_thread_non_deterministic_data_allocator(void* new_address)
{
	//INVOKE(0x00593CD0, __tls_set_g_hs_thread_non_deterministic_data_allocator, new_address);

	data_set_new_base_address(&hs_thread_non_deterministic_data.get_restricted_data_array_address(), (s_data_array*)new_address);
}

void __cdecl __tls_set_g_hs_thread_tracking_data_allocator(void* new_address)
{
	//INVOKE(0x00593D00, __tls_set_g_hs_thread_tracking_data_allocator, new_address);

	data_set_new_base_address(&hs_thread_tracking_data.get_restricted_data_array_address(), (s_data_array*)new_address);
}

//.text:00593D30 ; tls
//.text:00593D70 ; tls
//.text:00593DB0 ; tls
//.text:00593DF0 ; tls
//.text:00593E30 ; tls
//.text:00593E70 ; tls
//.text:00593EB0 ; public: void c_data_iterator<hs_global_runtime>::begin(s_data_array* data)
//.text:00593ED0 ; tls
//.text:00593F00 ; tls
//.text:00593F30 ; tls
//.text:00593F60 ; tls
//.text:00593F90 ; tls
//.text:00593FC0 ; tls
//.text:00593FF0 ; tls
//.text:00594020 ; tls
//.text:00594050 ; tls
//.text:00594080 ; tls
//.text:005940B0 ; tls
//.text:005940E0 ; tls
//.text:00594110 ; public: int16 c_data_iterator<hs_global_runtime>::get_absolute_index() const
//.text:00594120 ; public: hs_global_runtime* c_data_iterator<hs_global_runtime>::get_datum() const
//.text:00594130 ; public: int32 c_data_iterator<hs_global_runtime>::get_index() const

int32* __cdecl hs_arguments_evaluate(int32 thread_index, int16 formal_parameter_count, const int16* formal_parameters, bool initialize)
{
	//return INVOKE(0x00594140, hs_arguments_evaluate, thread_index, formal_parameter_count, formal_parameters, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);

	hs_stack_pointer evaluation_results_reference{};
	int32* evaluation_results = (int32*)hs_stack_allocate(thread_index, sizeof(int32) * formal_parameter_count, 2, &evaluation_results_reference);
	int16* argument_index = (int16*)hs_stack_allocate(thread_index, sizeof(int16), 1, NULL);
	int32* expression_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	if (evaluation_results && argument_index && expression_index)
	{
		if (initialize)
		{
			*argument_index = 0;
			*expression_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
		}

		if (*argument_index < formal_parameter_count)
		{
			SCRIPT_EXECUTION_ERROR(thread_index, *expression_index != NONE, "corrupted syntax tree.");

			if (SCRIPT_COMPILE_ERROR(thread_index, hs_syntax_get(*expression_index)->type == formal_parameters[*argument_index], "unexpected actual parameters."))
			{
				hs_destination_pointer destination{};
				destination.destination_type = _hs_destination_stack;
				destination.stack_pointer = evaluation_results_reference;
				destination.stack_pointer.stack_offset += sizeof(int32) * *argument_index;
				hs_evaluate(thread_index, *expression_index, destination, NULL);
				*expression_index = hs_syntax_get(*expression_index)->next_node_index;
				(*argument_index)++;
				evaluation_results = 0;
			}
		}
		else
		{
			SCRIPT_EXECUTION_ERROR(thread_index, *expression_index == NONE, "corrupted syntax tree.");
		}
	}

	return evaluation_results;
}

void __cdecl hs_breakpoint(const char* s)
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

int32 __cdecl hs_cast(int32 thread_index, int16 actual_type, int16 desired_type, int32 value)
{
	return INVOKE(0x005943A0, hs_cast, thread_index, actual_type, desired_type, value);
}

int32 __cdecl hs_data_to_void(int32 n)
{
	return INVOKE(0x00594450, hs_data_to_void, n);

	//return 0;
}

int32* __cdecl hs_destination(hs_thread* thread, hs_destination_pointer destination_pointer)
{
	//return INVOKE(0x00594460, hs_destination, thread, destination_pointer);

	int32* destination = NULL;
	switch (destination_pointer.destination_type)
	{
	case _hs_destination_none:
	{
		destination = NULL;
	}
	break;
	case _hs_destination_stack:
	{
		destination = hs_stack_destination(thread, destination_pointer.stack_pointer);
	}
	break;
	case _hs_destination_runtime_global:
	{
		destination = &DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime,
			hs_runtime_index_from_global_designator(destination_pointer.runtime_global_index))->value;
	}
	break;
	case _hs_destination_thread_result:
	{
		destination = &thread->result;
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
	return destination;
}

int32 __cdecl hs_enum_to_real(int32 e)
{
	return INVOKE(0x005944F0, hs_enum_to_real, e);

	//int32 result;
	//*(real32*)&result = (real32)((int16)e + 1);
	//return result;
}

bool __cdecl hs_evaluate(int32 thread_index, int32 expression_index, hs_destination_pointer destination_pointer, int32* local_destination)
{
	//return INVOKE(0x00594510, hs_evaluate_runtime, thread_index, expression_index, destination_pointer, local_destination);

	bool result = true;
	hs_thread* thread = hs_thread_get(thread_index);
	const hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 expression_result = NONE;

	SCRIPT_EXECUTION_ERROR(thread_index, valid_thread(thread_index), "corrupted stack.");

	if (!TEST_BIT(expression->flags, _hs_syntax_node_primitive_bit))
	{
		hs_thread_stack(thread)->child_result = destination_pointer;
		result = hs_stack_push(thread_index);
		thread->flags |= FLAG(_hs_thread_in_function_call_bit);
		hs_thread_stack(thread)->expression_index = expression_index;
	}
	else
	{
		if (!TEST_BIT(expression->flags, _hs_syntax_node_variable_bit))
		{
			expression_result = hs_cast(thread_index, expression->constant_type, expression->type, expression->long_value);
		}
		else if (!TEST_BIT(expression->flags, _hs_syntax_node_parameter_bit))
		{
			expression_result = hs_cast(
				thread_index,
				hs_global_get_type(expression->short_value),
				expression->type,
				hs_global_evaluate(expression->short_value));
		}
		else
		{
			bool found = false;
			hs_stack_frame* current_frame = hs_thread_stack(thread);
			while (current_frame)
			{
				if (current_frame->script_index != NONE)
				{
					hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, current_frame->script_index, hs_script);
					hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, expression->long_value, hs_script_parameter);
					int32* parameters = hs_stack_parameters(thread, current_frame, expression->long_value + 1);
					expression_result = hs_cast(thread_index, parameter->type, expression->type, parameters[expression->long_value]);

					found = true;
					break;
				}
				current_frame = hs_stack(thread, current_frame->parent);
			}
			ASSERT(found);
		}

		int32* destination = hs_destination(thread, destination_pointer);
		if (destination)
		{
			*destination = expression_result;
		}
	}

	if (local_destination)
	{
		*local_destination = expression_result;
	}

	return result;
}

void __cdecl hs_evaluate_arithmetic(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00594680, hs_evaluate_arithmetic, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	int16* parameter_index = (int16*)hs_stack_allocate(thread_index, sizeof(int16), 1, NULL);
	int32* parameters_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	hs_stack_pointer result_reference{};
	int32* parameter_result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &result_reference);
	real32* result = (real32*)hs_stack_allocate(thread_index, sizeof(real32), 2, NULL);
	if (parameter_index && parameters_index && parameter_result && result)
	{
		if (initialize)
		{
			*parameter_index = 0;
			*parameters_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
		}
		else
		{
			real32 parameter_result_real = *(real32*)parameter_result;
			if (*parameter_index)
			{
				switch (function_index)
				{
				case _hs_function_plus:
				{
					*result = *result + parameter_result_real;
				}
				break;
				case _hs_function_minus:
				{
					*result = *result - parameter_result_real;
				}
				break;
				case _hs_function_times:
				{
					*result = *result * parameter_result_real;
				}
				break;
				case _hs_function_divide:
				{
					ASSERT(!(fabs((parameter_result_real)-(0.0f)) < (k_real_epsilon)));
					if (fabs(parameter_result_real - 0.0f) < k_real_epsilon)
					{
						*result = 0.0f;
					}
					else
					{
						*result = *result / parameter_result_real;
					}
				}
				break;
				case _hs_function_modulo:
				{
					*result = fmodf(*result, parameter_result_real);
				}
				break;
				case _hs_function_min:
				{
					*result = MIN(parameter_result_real, *result);
				}
				break;
				case _hs_function_max:
				{
					*result = MAX(*result, parameter_result_real);
				}
				break;
				default:
				{
					HALT();
				}
				break;
				}
			}
			else
			{
				*result = parameter_result_real;
			}
			++*parameter_index;
		}

		if (*parameters_index != NONE)
		{
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = result_reference;
			hs_evaluate(thread_index, *parameters_index, destination, NULL);
			*parameters_index = hs_syntax_get(*parameters_index)->next_node_index;
		}
		else
		{
			int32 result_long = 0;
			*reinterpret_cast<real32*>(&result_long) = *result;
			hs_return(thread_index, result_long);
		}
	}
}

void __cdecl hs_evaluate_begin(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00594960, hs_evaluate_begin, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	int32* parameters_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	hs_stack_pointer result_reference{};
	int32* parameters_result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &result_reference);
	if (parameters_index && parameters_result)
	{
		ASSERT(function_index == _hs_function_begin);
		if (initialize)
		{
			*parameters_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
			*parameters_result = 0;
		}

		if (*parameters_index != NONE)
		{
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = result_reference;
			hs_evaluate(thread_index, *parameters_index, destination, NULL);
			*parameters_index = hs_syntax_get(*parameters_index)->next_node_index;
		}
		else
		{
			hs_return(thread_index, *parameters_result);
		}
	}
}

void __cdecl hs_evaluate_begin_random(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00594AB0, hs_evaluate_begin_random, function_index, thread_index, initialize);

	hs_thread const* thread = hs_thread_get(thread_index);
	int16* argument_count = (int16*)hs_stack_allocate(thread_index, sizeof(int16), 1, NULL);
	int32* argument_evaluation_flags = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	hs_stack_pointer result_reference{};
	int32* result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &result_reference);
	if (argument_count && argument_evaluation_flags && result)
	{
		ASSERT(function_index == _hs_function_begin_random);

		if (initialize)
		{
			int32 expression_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;

			*argument_count = 0;
			while (expression_index != NONE)
			{
				expression_index = hs_syntax_get(expression_index)->next_node_index;
				++*argument_count;
			}
			ASSERT(*argument_count < LONG_BITS);

			memset(argument_evaluation_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(*argument_count));
		}

		int16 random_argument_index = _random_range(
			/* seed */ get_random_seed_address(),
			/* purpose */ "hs-begin-random",
			/* source_file, source_line */ __FILE__, __LINE__,
			/* lower_bound, upper_bound */ 0, *argument_count);

		int16 argument_index = 0;
		while (argument_index < *argument_count)
		{
			int16 actual_argument_index = (argument_index + random_argument_index) % *argument_count;
			if (!BIT_VECTOR_TEST_FLAG(argument_evaluation_flags, actual_argument_index))
			{
				hs_destination_pointer destination;
				destination.destination_type = _hs_destination_stack;
				destination.stack_pointer = result_reference;
				hs_evaluate(thread_index, hs_syntax_nth(hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index, actual_argument_index), destination, NULL);
				BIT_VECTOR_OR_FLAG(argument_evaluation_flags, actual_argument_index);
				break;
			}
			argument_index++;
		}

		if (argument_index == *argument_count)
		{
			hs_return(thread_index, *result);
		}
	}
}

void __cdecl hs_evaluate_debug_string(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00594D20, hs_evaluate_debug_string, function_index, thread_index, initialize);

	hs_thread const* thread = hs_thread_get(thread_index);
	int32* parameters_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	hs_stack_pointer parameters_result_reference{};
	int32* parameters_result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &parameters_result_reference);
	int32* string_count = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	char** string_storage = (char**)hs_stack_allocate(thread_index, 128, 2, NULL);
	if (parameters_index && parameters_result && string_count && string_storage)
	{
		ASSERT(IN_RANGE_INCLUSIVE(function_index, _hs_function_debug_string__first, _hs_function_debug_string__last));
		if (initialize)
		{
			*parameters_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
			*parameters_result = 0;
			*string_count = 0;
			csmemset(string_storage, 0, 128);
		}
		if (*parameters_index != NONE && *string_count < 32)
		{
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = parameters_result_reference;
			hs_evaluate(thread_index, *parameters_index, destination, NULL);
			*parameters_index = hs_syntax_get(*parameters_index)->next_node_index;
			string_storage[(*string_count)++] = (char*)*parameters_result;
		}
		else
		{
			hs_return(thread_index, NONE);
		}
	}
}

void __cdecl hs_evaluate_equality(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00594FB0, hs_evaluate_equality, function_index, thread_index, initialize);

	ASSERT(function_index == _hs_function_equal || function_index == _hs_function_not_equal);

	int16 parameter_types[2];
	parameter_types[0] = parameter_types[1] = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(hs_thread_get(thread_index))->expression_index)->long_value)->next_node_index)->type;
	int32* parameter_results = hs_arguments_evaluate(thread_index, NUMBEROF(parameter_types), parameter_types, initialize);
	if (parameter_results)
	{
		int32 result_long = 0;
		bool result = false;

		switch (function_index)
		{
		case _hs_function_equal:
		{
			result = csmemcmp(parameter_results, parameter_results + 1, hs_type_sizes[parameter_types[0]]) == 0;
		}
		break;
		case _hs_function_not_equal:
		{
			result = csmemcmp(parameter_results, parameter_results + 1, hs_type_sizes[parameter_types[0]]) != 0;
		}
		break;
		}

		*reinterpret_cast<bool*>(&result_long) = result;
		hs_return(thread_index, result_long);
	}
}

void __cdecl hs_evaluate_if(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x005950B0, hs_evaluate_if, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	hs_stack_pointer condition_result_reference{};
	int32* condition_result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &condition_result_reference);
	int32* evaluate_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	hs_stack_pointer evaluate_result_reference{};
	int32* evaluate_result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &evaluate_result_reference);
	if (condition_result && evaluate_index && evaluate_result)
	{
		ASSERT(function_index == _hs_function_if);

		if (initialize)
		{
			hs_destination_pointer destination;
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = condition_result_reference;
			*condition_result = 0;
			*evaluate_index = NONE;
			hs_evaluate(thread_index, hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index, destination, NULL);
		}
		else if (*evaluate_index == NONE)
		{
			hs_destination_pointer destination;
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = evaluate_result_reference;

			if (*(bool*)condition_result)
			{
				*evaluate_index = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index)->next_node_index;
				hs_evaluate(thread_index, *evaluate_index, destination, NULL);
			}
			else
			{
				*evaluate_index = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index)->next_node_index)->next_node_index;
				if (*evaluate_index == NONE)
				{
					hs_return(thread_index, 0);
				}
				else
				{
					hs_evaluate(thread_index, *evaluate_index, destination, NULL);
				}
			}
		}
		else
		{
			hs_return(thread_index, *evaluate_result);
		}
	}
}

void __cdecl hs_evaluate_inequality(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x005952A0, hs_evaluate_inequality, function_index, thread_index, initialize);

	ASSERT(function_index >= _hs_function_gt && function_index <= _hs_function_lte);

	int16 parameter_types[2];
	parameter_types[0] = parameter_types[1] = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(hs_thread_get(thread_index))->expression_index)->long_value)->next_node_index)->type;
	int32* parameter_results = hs_arguments_evaluate(thread_index, NUMBEROF(parameter_types), parameter_types, initialize);
	if (parameter_results)
	{
		real32 arg0 = 0.0f;
		real32 arg1 = 0.0f;

		if (parameter_types[0] == _hs_type_real)
		{
			ASSERT(hs_type_sizes[parameter_types[0]] == sizeof(real32));

			arg0 = *(real32*)&parameter_results[0];
			arg1 = *(real32*)&parameter_results[1];
		}
		else if (parameter_types[0] == _hs_type_long_integer)
		{
			ASSERT(hs_type_sizes[parameter_types[0]] == sizeof(int32));

			arg0 = (real32)parameter_results[0];
			arg1 = (real32)parameter_results[1];
		}
		else
		{
			ASSERT(parameter_types[0] == _hs_type_short_integer || HS_TYPE_IS_ENUM(parameter_types[0]));
			ASSERT(hs_type_sizes[parameter_types[0]] == sizeof(int16));

			arg0 = (real32)*(int16*)&parameter_results[0];
			arg1 = (real32)*(int16*)&parameter_results[1];
		}

		bool result = false;
		int32 result_long = 0;

		switch (function_index)
		{
		case _hs_function_gt:
		{
			result = arg0 > arg1;
		}
		break;
		case _hs_function_lt:
		{
			result = arg1 > arg0;
		}
		break;
		case _hs_function_gte:
		{
			result = arg0 >= arg1;
		}
		break;
		case _hs_function_lte:
		{
			result = arg1 >= arg0;
		}
		break;
		default:
		{
			result_long = 0;
			HALT();
		}
		break;
		}

		*reinterpret_cast<bool*>(&result_long) = result;
		hs_return(thread_index, result_long);
	}
}

void __cdecl hs_evaluate_inspect(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00595450, hs_evaluate_inspect, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);

	hs_stack_pointer expression_reference{};
	int32* expression = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &expression_reference);
	if (expression)
	{
		ASSERT(function_index == _hs_function_inspect);
		if (initialize)
		{
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = expression_reference;
			hs_evaluate(thread_index, hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index, destination, NULL);
		}
		else
		{
			const hs_syntax_node* expression_node = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index);
			if (TEST_BIT(thread->flags, _hs_thread_display_expression_bit))
			{
				char printbuffer[1024]{};
				if (hs_type_inspectors[expression_node->type])
				{
					hs_type_inspectors[expression_node->type](expression_node->type, *expression, printbuffer, sizeof(printbuffer));
					console_printf(printbuffer);
				}
			}
			hs_return(thread_index, 0);
		}
	}
}

void __cdecl hs_evaluate_logical(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00595550, hs_evaluate_logical, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	int32* parameters_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	hs_stack_pointer result_reference{};
	int32* parameter_result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &result_reference);
	bool* result = (bool*)hs_stack_allocate(thread_index, sizeof(bool), 0, NULL);
	if (parameters_index && parameter_result && result)
	{
		ASSERT(function_index == _hs_function_and || function_index == _hs_function_or);

		bool and_ = function_index == _hs_function_and;
		if (initialize)
		{
			*parameters_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
			*result = and_;
		}
		else
		{
			bool parameter_result_boolean = *parameter_result;
			if (and_)
			{
				*result = *result && parameter_result_boolean;
			}
			else
			{
				*result = *result || parameter_result_boolean;
			}
		}

		if (*parameters_index != NONE && *result == and_)
		{
			hs_destination_pointer destination;
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = result_reference;
			hs_evaluate(thread_index, *parameters_index, destination, NULL);
			*parameters_index = hs_syntax_get(*parameters_index)->next_node_index;
		}
		else
		{
			int32 result_long = 0;
			*reinterpret_cast<bool*>(&result_long) = *result;
			hs_return(thread_index, result_long);
		}
	}
}

void __cdecl hs_evaluate_object_cast_up(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x005956F0, hs_evaluate_object_cast_up, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	hs_stack_pointer result_reference{};
	int32* result_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &result_reference);
	{
		ASSERT(function_index >= _hs_function_object_to_unit && function_index <= _hs_function_object_to_unit);

		if (initialize)
		{
			hs_destination_pointer destination;
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = result_reference;
			hs_evaluate(thread_index, hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index, destination, NULL);
		}
		else if (*result_index != NONE)
		{
			const object_datum* object = OBJECT_GET(const object_datum, *result_index);
			int16 desired_type = function_index - (_hs_function_object_to_unit - 1);
			if (TEST_BIT(hs_object_type_masks[desired_type], object->object.object_identifier.get_type()))
			{
				hs_return(thread_index, *result_index);
			}
			else
			{
				event(_event_warning, "attempt to convert object %s to type %s",
					tag_get_name(object->definition_index),
					hs_type_names[FIRST_HS_OBJECT_TYPE + desired_type]);

				hs_return(thread_index, NONE);
			}
		}
		else
		{
			hs_return(thread_index, NONE);
		}
	}
}

void __cdecl hs_evaluate_set(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x005957F0, hs_evaluate_set, function_index, thread_index, initialize);

	int32* evaluate_result = NULL; // what's supposed to use this?

	const hs_thread* thread = hs_thread_get(thread_index);
	int32 variable_reference_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
	const hs_syntax_node* variable_reference = hs_syntax_get(variable_reference_index);
	if (hs_stack_allocate(thread_index, sizeof(int32), 2, NULL))
	{
		int16 type = hs_global_get_type(variable_reference->short_value);
		if (initialize)
		{
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_runtime_global;
			destination.stack_pointer.stack_offset = variable_reference->short_value;
			if (type == _hs_type_object_list)
			{
				object_list_add_reference(hs_global_evaluate(variable_reference->short_value));
			}
			hs_evaluate(thread_index, variable_reference->next_node_index, destination, NULL);
		}
		else
		{
			hs_global_reconcile_write(variable_reference->short_value);
			if (type == _hs_type_object_list)
			{
				object_list_add_reference(hs_global_evaluate(variable_reference->short_value));
			}

			bool verbose_thread = TEST_BIT(thread->flags, _hs_thread_verbose_bit);
			if (hs_verbose || verbose_thread)
			{
				char valuebuffer[1024]{};
				inspect_internal(
					type,
					DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime, hs_runtime_index_from_global_designator(variable_reference->short_value))->value,
					valuebuffer, sizeof(valuebuffer));
				event(_event_warning, "hs: %s <-- %s (thread %s  line #%i)",
					hs_global_get_name(variable_reference->short_value),
					valuebuffer,
					hs_thread_format(thread_index),
					variable_reference->line_number);
				if (verbose_thread)
				{
					event(_event_warning, "hs: %s <-- %s (thread %s  line #%i)",
						hs_global_get_name(variable_reference->short_value),
						valuebuffer,
						hs_thread_format(thread_index),
						variable_reference->line_number);
				}
			}

			hs_return(thread_index, hs_global_evaluate(variable_reference->short_value));
		}
	}
}

void __cdecl hs_evaluate_sleep(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00595A00, hs_evaluate_sleep, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	hs_stack_pointer time_reference{};
	int32* time = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &time_reference);
	hs_stack_pointer script_reference{};
	int32* script_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &script_reference);
	int16* argument_index = (int16*)hs_stack_allocate(thread_index, sizeof(int16), 1, NULL);
	if (time && script_index && argument_index)
	{
		ASSERT(function_index == _hs_function_sleep);

		int32 sleep_thread_index = thread_index;

		if (initialize)
		{
			hs_destination_pointer destination;
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = time_reference;
			hs_evaluate(thread_index, hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index, destination, NULL);
			*argument_index = 0;
		}
		else if (*argument_index == 0)
		{
			int32 script_node_index = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index)->next_node_index;
			++*argument_index;

			if (script_node_index != NONE)
			{
				hs_destination_pointer destination;
				destination.destination_type = _hs_destination_stack;
				destination.stack_pointer = script_reference;
				hs_evaluate(thread_index, script_node_index, destination, NULL);
			}
			else
			{
				*script_index = NONE;
			}
		}
		else if (*argument_index)
		{
			int16 short_script_index = *(int16*)script_index;
			if (short_script_index == NONE)
			{
				short_script_index = hs_thread_get(thread_index)->script_index;
			}
			else
			{
				sleep_thread_index = hs_find_thread_by_script(short_script_index);
			}

			if (sleep_thread_index != NONE)
			{
				int32 sleep_game_ticks = 0;
				int32 sleep_hs_ticks = *(int16*)time;
				if (sleep_hs_ticks < 0)
				{
					event(_event_warning, "script %s tried to sleep %d, maybe use sleep_forever?",
						short_script_index == NONE ? "<unknown>" : TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, short_script_index, hs_script)->name,
						sleep_hs_ticks);
				}
				else
				{
					real32 sleep_seconds = (real32)hs_ticks_to_seconds((int16)sleep_hs_ticks);
					sleep_game_ticks = game_seconds_to_ticks_round(sleep_seconds);
				}

				if (sleep_game_ticks > 0)
				{
					hs_thread* sleep_thread = hs_thread_get(sleep_thread_index);
					int32 sleep_until = game_time_get() + sleep_game_ticks;
					if (sleep_thread->sleep_until != NONE)
					{
						if (sleep_thread_index != thread_index && !TEST_BIT(sleep_thread->flags, _hs_thread_latent_sleep_bit))
						{
							sleep_thread->flags |= FLAG(_hs_thread_latent_sleep_bit);
							sleep_thread->latent_sleep_until = sleep_thread->sleep_until;
						}
						hs_thread_get(sleep_thread_index)->sleep_until = sleep_until;
					}
				}
			}
			hs_return(thread_index, 0);
		}
	}
}

void __cdecl hs_evaluate_sleep_forever(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00595C10, hs_evaluate_sleep_forever, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	int32 script_node_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
	int32 sleep_thread_index = thread_index;

	ASSERT(function_index == _hs_function_sleep_forever);

	if (script_node_index != NONE)
	{
		hs_destination_pointer destination{};
		destination.destination_type = _hs_destination_none;
		int32 script_index = 0;
		int16 short_script_index = 0;
		hs_evaluate(thread_index, script_node_index, destination, &script_index);
		short_script_index = (int16)script_index;
		sleep_thread_index = hs_find_thread_by_script(short_script_index);
	}

	if (sleep_thread_index != NONE)
	{
		hs_thread* sleep_thread = hs_thread_get(sleep_thread_index);
		sleep_thread->sleep_until = HS_SLEEP_INDEFINITE;
	}

	hs_return(thread_index, 0);
}

void __cdecl hs_evaluate_sleep_until(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00595CC0, hs_evaluate_sleep_until, function_index, thread_index, initialize);

	hs_thread* thread = hs_thread_get(thread_index);
	hs_stack_pointer condition_reference{};
	int32* condition = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &condition_reference);
	hs_stack_pointer period_reference{};
	int32* period = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &period_reference);
	hs_stack_pointer expiration_reference{};
	int32* expiration = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &expiration_reference);
	int32* start_time = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
	int16* optional_argument_index = (int16*)hs_stack_allocate(thread_index, sizeof(int16), 1, NULL);
	if (condition && period && expiration && start_time && optional_argument_index)
	{
		int32 optional_argument_expression_index = hs_syntax_get(hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index)->next_node_index;

		ASSERT(function_index == _hs_function_sleep_until);

		if (initialize)
		{
			*(bool*)condition = false;
			*start_time = game_time_get();
			*optional_argument_index = 0;
			*(int16*)period = 30;
			*expiration = NONE;

			if (optional_argument_expression_index != NONE)
			{
				hs_destination_pointer destination;
				destination.destination_type = _hs_destination_stack;
				destination.stack_pointer = period_reference;
				hs_evaluate(thread_index, optional_argument_expression_index, destination, NULL);
			}
		}
		else if (*optional_argument_index == 0)
		{
			*optional_argument_index = 1;
			if (optional_argument_expression_index != NONE && hs_syntax_get(optional_argument_expression_index)->next_node_index != NONE)
			{
				hs_destination_pointer destination;
				destination.destination_type = _hs_destination_stack;
				destination.stack_pointer = expiration_reference;
				hs_evaluate(thread_index, hs_syntax_get(optional_argument_expression_index)->next_node_index, destination, NULL);
			}
		}
		else
		{
			int32 expiration_game_ticks = NONE;
			if (*expiration != NONE)
			{
				int32 expiration_hs_ticks = *expiration;
				{
					real32 expirationSeconds = (real32)hs_ticks_to_seconds((int16)expiration_hs_ticks);
					expiration_game_ticks = game_seconds_to_ticks_round(expirationSeconds);
				}
			}

			if (*(bool*)condition)
			{
				int32 result_long = 1;
				hs_return(thread_index, result_long);
			}
			else if (expiration_game_ticks != NONE && game_time_get() >= *start_time + expiration_game_ticks)
			{
				int32 result_long = 0;
				hs_return(thread_index, result_long);
			}
			else
			{
				{
					hs_destination_pointer destination;
					destination.destination_type = _hs_destination_stack;
					destination.stack_pointer = condition_reference;
					hs_evaluate(thread_index, hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index, destination, NULL);
				}
				{
					int32 period_hs_ticks = *period;
					int32 period_game_ticks = 0;
					{
						real32 periodSecs = (real32)hs_ticks_to_seconds((int16)period_hs_ticks);
						period_game_ticks = game_seconds_to_ticks_round(periodSecs);
					}
					if (period_game_ticks < 1)
					{
						period_game_ticks = 1;
					}

					thread->sleep_until = game_time_get() + period_game_ticks;
					if (expiration_game_ticks != NONE)
					{
						thread->sleep_until = MAX(thread->sleep_until, *start_time + expiration_game_ticks);
					}
				}
			}
		}
	}
}

void __cdecl hs_evaluate_wake(int16 function_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x00595FF0, hs_evaluate_wake, function_index, thread_index, initialize);

	const hs_thread* thread = hs_thread_get(thread_index);
	int32 script_name_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
	hs_syntax_node* script_name_node = hs_syntax_get(script_name_index);

	ASSERT(function_index == _hs_function_wake);
	ASSERT(TEST_BIT(script_name_node->flags, _hs_syntax_node_primitive_bit));
	ASSERT(script_name_node->type == _hs_type_script);

	int32 wake_thread_index = hs_find_thread_by_script(script_name_node->short_value);
	if (wake_thread_index != NONE)
	{
		hs_wake(wake_thread_index, thread_index);
	}
	hs_return(thread_index, 0);
}

void __cdecl hs_find_dormant_script(const char* dormant_script_name, int32* script_index_out)
{
	ASSERT(dormant_script_name);
	ASSERT(script_index_out);

	*script_index_out = NONE;

	int32 thread_index = hs_find_thread_by_name(dormant_script_name);
	if (thread_index != NONE)
	{
		hs_thread* thread = hs_thread_get(thread_index);
		hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, thread->script_index, hs_script);
		if (script && script->script_type == _hs_script_dormant)
		{
			*script_index_out = thread->script_index;
		}
	}
}

int32 __cdecl hs_find_thread_by_name(const char* name)
{
	//return INVOKE(0x00596070, hs_find_thread_by_name, name);

	s_hs_thread_iterator iterator{};
	hs_thread_iterator_new(&iterator, true, true);
	for (int32 thread_index = hs_thread_iterator_next(&iterator);
		thread_index != NONE;
		thread_index = hs_thread_iterator_next(&iterator))
	{
		const hs_thread* thread = hs_thread_get(thread_index);
		if (thread->script_index != NONE)
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, thread->script_index, hs_script);
			if (ascii_stricmp(script->name, name) == 0)
			{
				return thread_index;
			}
		}
	}
	return NONE;
}

int32 __cdecl hs_find_thread_by_script(int16 script_index)
{
	//return INVOKE(0x00596130, hs_find_thread_by_script, script_index);

	s_hs_thread_iterator iterator{};
	hs_thread_iterator_new(&iterator, true, true);
	for (int32 thread_index = hs_thread_iterator_next(&iterator);
		thread_index != NONE;
		thread_index = hs_thread_iterator_next(&iterator))
	{
		const hs_thread* thread = hs_thread_get(thread_index);
		if (thread->script_index == script_index)
		{
			return thread_index;
		}
	}
	return NONE;
}

int32 __cdecl hs_global_evaluate(int16 global_designator)
{
	//return INVOKE(0x005961D0, hs_global_evaluate, global_designator);

	hs_global_reconcile_read(global_designator);
	return DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime,
		hs_runtime_index_from_global_designator(global_designator))->value;
}

void __cdecl hs_global_reconcile_read(int16 global_designator)
{
	//INVOKE(0x00596230, hs_global_reconcile_read, global_designator);

	if (TEST_BIT(global_designator, 15))
	{
		hs_global_external* external_global = hs_global_external_get(global_designator & MASK(15));
		hs_global_runtime* runtime_global = DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime,
			hs_runtime_index_from_global_designator(global_designator));
		switch (hs_global_get_type(global_designator))
		{
		#define HANDLE_READ_CASE(TYPE_NAME) \
		case TYPE_NAME: \
		{ \
			if (external_global->pointer) \
			{ \
				*(TYPE_NAME##_*)&runtime_global->value = *(TYPE_NAME##_*)external_global->pointer; \
			} \
			else \
			{ \
				*(TYPE_NAME##_*)&runtime_global->value = TYPE_NAME##_default; \
			} \
		} \
		break
		//_hs_unparsed
		//_hs_special_form
		//_hs_function_name
		//_hs_passthrough
		//_hs_type_void
		HANDLE_READ_CASE(_hs_type_boolean);
		HANDLE_READ_CASE(_hs_type_real);
		HANDLE_READ_CASE(_hs_type_short_integer);
		HANDLE_READ_CASE(_hs_type_long_integer);
		HANDLE_READ_CASE(_hs_type_string);
		HANDLE_READ_CASE(_hs_type_script);
		HANDLE_READ_CASE(_hs_type_string_id);
		HANDLE_READ_CASE(_hs_type_unit_seat_mapping);
		HANDLE_READ_CASE(_hs_type_trigger_volume);
		HANDLE_READ_CASE(_hs_type_cutscene_flag);
		HANDLE_READ_CASE(_hs_type_cutscene_camera_point);
		HANDLE_READ_CASE(_hs_type_cutscene_title);
		HANDLE_READ_CASE(_hs_type_cutscene_recording);
		HANDLE_READ_CASE(_hs_type_device_group);
		HANDLE_READ_CASE(_hs_type_ai);
		HANDLE_READ_CASE(_hs_type_ai_command_list);
		HANDLE_READ_CASE(_hs_type_ai_command_script);
		HANDLE_READ_CASE(_hs_type_ai_behavior);
		HANDLE_READ_CASE(_hs_type_ai_orders);
		HANDLE_READ_CASE(_hs_type_ai_line);
		HANDLE_READ_CASE(_hs_type_starting_profile);
		HANDLE_READ_CASE(_hs_type_conversation);
		HANDLE_READ_CASE(_hs_type_zone_set);
		//_hs_type_designer_zone
		HANDLE_READ_CASE(_hs_type_point_ref);
		HANDLE_READ_CASE(_hs_type_style);
		HANDLE_READ_CASE(_hs_type_object_list);
		HANDLE_READ_CASE(_hs_type_folder);
		HANDLE_READ_CASE(_hs_type_sound);
		HANDLE_READ_CASE(_hs_type_effect);
		HANDLE_READ_CASE(_hs_type_damage);
		HANDLE_READ_CASE(_hs_type_looping_sound);
		HANDLE_READ_CASE(_hs_type_animation_graph);
		HANDLE_READ_CASE(_hs_type_damage_effect);
		HANDLE_READ_CASE(_hs_type_object_definition);
		HANDLE_READ_CASE(_hs_type_bitmap);
		HANDLE_READ_CASE(_hs_type_shader);
		HANDLE_READ_CASE(_hs_type_render_model_definition);
		HANDLE_READ_CASE(_hs_type_structure_bsp_definition);
		HANDLE_READ_CASE(_hs_type_structure_lightmap_definition);
		HANDLE_READ_CASE(_hs_type_cinematic_definition);
		HANDLE_READ_CASE(_hs_type_cinematic_scene_definition);
		HANDLE_READ_CASE(_hs_type_bink_definition);
		HANDLE_READ_CASE(_hs_type_any_tag);
		//_hs_type_any_tag_not_resolving
		HANDLE_READ_CASE(_hs_type_enum_game_difficulty);
		HANDLE_READ_CASE(_hs_type_enum_team);
		HANDLE_READ_CASE(_hs_type_enum_mp_team);
		HANDLE_READ_CASE(_hs_type_enum_controller);
		HANDLE_READ_CASE(_hs_type_enum_button_preset);
		HANDLE_READ_CASE(_hs_type_enum_joystick_preset);
		HANDLE_READ_CASE(_hs_type_enum_player_character_type);
		HANDLE_READ_CASE(_hs_type_enum_voice_output_setting);
		//_hs_type_enum_voice_mask
		HANDLE_READ_CASE(_hs_type_enum_subtitle_setting);
		HANDLE_READ_CASE(_hs_type_enum_actor_type);
		HANDLE_READ_CASE(_hs_type_enum_model_state);
		HANDLE_READ_CASE(_hs_type_enum_event);
		HANDLE_READ_CASE(_hs_type_enum_character_physics_override);
		HANDLE_READ_CASE(_hs_type_enum_primary_skull);
		HANDLE_READ_CASE(_hs_type_enum_secondary_skull);
		HANDLE_READ_CASE(_hs_type_object);
		HANDLE_READ_CASE(_hs_type_unit);
		HANDLE_READ_CASE(_hs_type_vehicle);
		HANDLE_READ_CASE(_hs_type_weapon);
		HANDLE_READ_CASE(_hs_type_device);
		HANDLE_READ_CASE(_hs_type_scenery);
		HANDLE_READ_CASE(_hs_type_effect_scenery);
		HANDLE_READ_CASE(_hs_type_object_name);
		//_hs_type_unit_name,
		//_hs_type_vehicle_name,
		//_hs_type_weapon_name,
		//_hs_type_device_name,
		//_hs_type_scenery_name,
		//_hs_type_effect_scenery_name,
		HANDLE_READ_CASE(_hs_type_cinematic_lightprobe);
		HANDLE_READ_CASE(_hs_type_budget_reference_animation_graph);
		HANDLE_READ_CASE(_hs_type_budget_reference_looping_sound);
		HANDLE_READ_CASE(_hs_type_budget_reference_sound);
		default:
		{
			HALT();
		}
		break;
		#undef HANDLE_READ_CASE
		}
	}
}

void __cdecl hs_global_reconcile_write(int16 global_designator)
{
	//INVOKE(0x00596C10, hs_global_reconcile_write, global_designator);

	int16 global_type = hs_global_get_type(global_designator);
	hs_global_runtime* runtime_global = DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime,
		hs_runtime_index_from_global_designator(global_designator));

	if (TEST_BIT(global_designator, 15))
	{
		hs_global_external* external_global = hs_global_external_get(global_designator & MASK(15));
		switch (global_type)
		{
		#define HANDLE_WRITE_CASE(TYPE_NAME) \
		case TYPE_NAME: \
		{ \
			if (external_global->pointer) \
			{ \
				*(TYPE_NAME##_*)external_global->pointer = *(TYPE_NAME##_*)&runtime_global->value; \
			} \
		} \
		break
		#define HANDLE_WRITE_CASE_WITH_DEFAULT(TYPE_NAME) \
		case TYPE_NAME: \
		{ \
			if (external_global->pointer) \
			{ \
				*(TYPE_NAME##_*)&runtime_global->value = *(TYPE_NAME##_*)external_global->pointer; \
			} \
			else \
			{ \
				*(TYPE_NAME##_*)&runtime_global->value = TYPE_NAME##_default; \
			} \
		} \
		break
		//_hs_unparsed
		//_hs_special_form
		//_hs_function_name
		//_hs_passthrough
		//_hs_type_void
		HANDLE_WRITE_CASE(_hs_type_boolean);
		HANDLE_WRITE_CASE(_hs_type_real);
		HANDLE_WRITE_CASE(_hs_type_short_integer);
		HANDLE_WRITE_CASE(_hs_type_long_integer);
		HANDLE_WRITE_CASE(_hs_type_string);
		HANDLE_WRITE_CASE(_hs_type_script);
		HANDLE_WRITE_CASE(_hs_type_string_id);
		HANDLE_WRITE_CASE(_hs_type_unit_seat_mapping);
		HANDLE_WRITE_CASE(_hs_type_trigger_volume);
		HANDLE_WRITE_CASE(_hs_type_cutscene_flag);
		HANDLE_WRITE_CASE(_hs_type_cutscene_camera_point);
		HANDLE_WRITE_CASE(_hs_type_cutscene_title);
		HANDLE_WRITE_CASE(_hs_type_cutscene_recording);
		HANDLE_WRITE_CASE(_hs_type_device_group);
		HANDLE_WRITE_CASE(_hs_type_ai);
		HANDLE_WRITE_CASE(_hs_type_ai_command_list);
		HANDLE_WRITE_CASE(_hs_type_ai_command_script);
		HANDLE_WRITE_CASE(_hs_type_ai_behavior);
		HANDLE_WRITE_CASE(_hs_type_ai_orders);
		HANDLE_WRITE_CASE(_hs_type_ai_line);
		HANDLE_WRITE_CASE(_hs_type_starting_profile);
		HANDLE_WRITE_CASE(_hs_type_conversation);
		HANDLE_WRITE_CASE(_hs_type_zone_set);
		//_hs_type_designer_zone
		HANDLE_WRITE_CASE(_hs_type_point_ref);
		HANDLE_WRITE_CASE(_hs_type_style);
		HANDLE_WRITE_CASE(_hs_type_object_list);
		HANDLE_WRITE_CASE(_hs_type_folder);
		HANDLE_WRITE_CASE(_hs_type_sound);
		HANDLE_WRITE_CASE(_hs_type_effect);
		HANDLE_WRITE_CASE(_hs_type_damage);
		HANDLE_WRITE_CASE(_hs_type_looping_sound);
		HANDLE_WRITE_CASE(_hs_type_animation_graph);
		HANDLE_WRITE_CASE(_hs_type_damage_effect);
		HANDLE_WRITE_CASE(_hs_type_object_definition);
		HANDLE_WRITE_CASE(_hs_type_bitmap);
		HANDLE_WRITE_CASE(_hs_type_shader);
		HANDLE_WRITE_CASE(_hs_type_render_model_definition);
		HANDLE_WRITE_CASE(_hs_type_structure_bsp_definition);
		HANDLE_WRITE_CASE(_hs_type_structure_lightmap_definition);
		HANDLE_WRITE_CASE(_hs_type_cinematic_definition);
		HANDLE_WRITE_CASE(_hs_type_cinematic_scene_definition);
		HANDLE_WRITE_CASE(_hs_type_bink_definition);
		HANDLE_WRITE_CASE(_hs_type_any_tag);
		//_hs_type_any_tag_not_resolving
		HANDLE_WRITE_CASE(_hs_type_enum_game_difficulty);
		HANDLE_WRITE_CASE(_hs_type_enum_team);
		HANDLE_WRITE_CASE(_hs_type_enum_mp_team);
		HANDLE_WRITE_CASE(_hs_type_enum_controller);
		HANDLE_WRITE_CASE_WITH_DEFAULT(_hs_type_enum_button_preset);
		HANDLE_WRITE_CASE_WITH_DEFAULT(_hs_type_enum_joystick_preset);
		HANDLE_WRITE_CASE_WITH_DEFAULT(_hs_type_enum_player_character_type);
		HANDLE_WRITE_CASE_WITH_DEFAULT(_hs_type_enum_voice_output_setting);
		//_hs_type_enum_voice_mask
		HANDLE_WRITE_CASE_WITH_DEFAULT(_hs_type_enum_subtitle_setting);
		HANDLE_WRITE_CASE(_hs_type_enum_actor_type);
		HANDLE_WRITE_CASE(_hs_type_enum_model_state);
		HANDLE_WRITE_CASE(_hs_type_enum_event);
		HANDLE_WRITE_CASE(_hs_type_enum_character_physics_override);
		HANDLE_WRITE_CASE(_hs_type_enum_primary_skull);
		HANDLE_WRITE_CASE(_hs_type_enum_secondary_skull);
		HANDLE_WRITE_CASE(_hs_type_object);
		HANDLE_WRITE_CASE(_hs_type_unit);
		HANDLE_WRITE_CASE(_hs_type_vehicle);
		HANDLE_WRITE_CASE(_hs_type_weapon);
		HANDLE_WRITE_CASE(_hs_type_device);
		HANDLE_WRITE_CASE(_hs_type_scenery);
		HANDLE_WRITE_CASE(_hs_type_effect_scenery);
		HANDLE_WRITE_CASE(_hs_type_object_name);
		//_hs_type_unit_name,
		//_hs_type_vehicle_name,
		//_hs_type_weapon_name,
		//_hs_type_device_name,
		//_hs_type_scenery_name,
		//_hs_type_effect_scenery_name,
		HANDLE_WRITE_CASE(_hs_type_cinematic_lightprobe);
		HANDLE_WRITE_CASE(_hs_type_budget_reference_animation_graph);
		HANDLE_WRITE_CASE(_hs_type_budget_reference_looping_sound);
		HANDLE_WRITE_CASE(_hs_type_budget_reference_sound);
		default:
		{
			HALT();
		}
		break;
		#undef HANDLE_WRITE_CASE2
		#undef HANDLE_WRITE_CASE
		}
	}

	if (!HS_TYPE_IS_OBJECT(global_type))
	{
		if (global_type == _hs_type_object_list)
		{
			int32 object_list_reference_index = NONE;
			for (int32 object_index = object_list_get_first(runtime_global->value, &object_list_reference_index);
				object_index != NONE;
				object_index = object_list_get_next(runtime_global->value, &object_list_reference_index))
			{
				object_datum* object = OBJECT_GET(object_datum, runtime_global->value);
				object->object.flags.set(_object_ever_referenced_by_hs_bit, true);
			}
		}
	}
	else if (runtime_global->value != NONE)
	{
		object_datum* object = OBJECT_GET(object_datum, runtime_global->value);
		object->object.flags.set(_object_ever_referenced_by_hs_bit, true);
	}
}

// $REVIEW if you crash and `hs_handle_deleted_object` is within the callstack let me know
void __cdecl hs_handle_deleted_object(int32 object_index)
{
	//INVOKE(0x00596F50, hs_handle_deleted_object, object_index);

	const object_datum* object = OBJECT_GET(const object_datum, object_index);
	if (object->object.flags.test(_object_ever_referenced_by_hs_bit))
	{
		bool need_object_list_gc = false;
		{
			c_data_iterator<hs_global_runtime> hs_global_iterator;
			hs_global_iterator.begin(hs_global_data);
			while (hs_global_iterator.next())
			{
				int16 global_index = hs_global_iterator.get_absolute_index();

				// $REVIEW is this correct
				int16 global_runtime_index = global_index >= k_hs_external_global_count ? global_index - k_hs_external_global_count : global_index;
				bool global_is_external = global_index < k_hs_external_global_count;
				int16 global_designator = global_runtime_index & MASK(15) | (global_is_external ? FLAG(15) : 0);

				int16 global_type = hs_global_get_type(global_designator);
				if (HS_TYPE_IS_OBJECT(global_type))
				{
					int32 global_object_index = hs_global_evaluate(global_designator);
					if (global_object_index == object_index)
					{
						hs_global_iterator.get_datum()->value = NONE;
						hs_global_reconcile_write(global_designator);
					}
				}
				else if (global_type == _hs_type_object_list)
				{
					int32 object_list_index = hs_global_evaluate(global_designator);
					if (object_list_index != NONE
						&& object_list_remove(object_list_index, object_index)
						&& !object_list_count(object_list_index))
					{
						object_list_remove_reference(object_list_index);
						hs_global_iterator.get_datum()->value = NONE;
						hs_global_reconcile_write(global_designator);
						need_object_list_gc = true;
					}
				}
			}
		}

		{
			const struct scenario* scenario = global_scenario_get();

			s_hs_thread_iterator iterator;
			hs_thread_iterator_new(&iterator, true, true);
			for (int32 thread_index = hs_thread_iterator_next(&iterator);
				thread_index != NONE;
				thread_index = hs_thread_iterator_next(&iterator))
			{
				hs_thread* thread = hs_thread_get(thread_index);
				hs_stack_pointer current_frame_pointer = thread->stack;
				while (current_frame_pointer.stack_offset != NONE)
				{
					hs_stack_frame* current_frame = hs_stack(thread, current_frame_pointer);
					if (VALID_INDEX(current_frame->script_index, scenario->hs_scripts.count))
					{
						hs_script* script = TAG_BLOCK_GET_ELEMENT(&scenario->hs_scripts, current_frame->script_index, hs_script);
						if (script->parameters.count > 0)
						{
							int32* stack_parameters = hs_stack_parameters(thread, current_frame, script->parameters.count);
							for (int16 parameter_index = 0; parameter_index < script->parameters.count; parameter_index++)
							{
								hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, parameter_index, hs_script_parameter);
								if (HS_TYPE_IS_OBJECT(parameter->type))
								{
									if (stack_parameters[parameter_index] == object_index)
									{
										stack_parameters[parameter_index] = NONE;
									}
								}
								else if (parameter->type == _hs_type_object_list)
								{
									int32 object_list_index = stack_parameters[parameter_index];
									if (object_list_index != NONE
										&& object_list_remove(object_list_index, object_index)
										&& !object_list_count(object_list_index))
									{
										object_list_remove_reference(object_list_index);
										stack_parameters[parameter_index] = NONE;
										need_object_list_gc = true;
									}
								}
							}
						}
					}
					current_frame_pointer = current_frame->parent;
				}
			}
		}

		// This was probably what used `need_object_list_gc`
		// but doesn't in all the builds I've seen
		object_list_gc();
	}
}

int32 __cdecl hs_long_to_boolean(int32 n)
{
	return INVOKE(0x00597280, hs_long_to_boolean, n);

	//int32 result;
	//*(bool*)&result = (bool)n;
	//return result;
}

int32 __cdecl hs_long_to_real(int32 l)
{
	return INVOKE(0x005972A0, hs_long_to_real, l);

	//int32 result;
	//*(real32*)&result = (real32)l;
	//return result;
}

int32 __cdecl hs_long_to_short(int32 l)
{
	return INVOKE(0x005972C0, hs_long_to_short, l);

	//int32 result;
	//*(int16*)&result = (int16)l;
	//return result;
}

//.text:005972E0 ; 

int32* __cdecl hs_macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize)
{
	//return INVOKE(0x005972F0, hs_macro_function_evaluate, function_index, thread_index, initialize);

	const hs_function_definition* function_definition = hs_function_get(function_index);
	
	int32* results = hs_arguments_evaluate(thread_index, function_definition->formal_parameter_count, function_definition->formal_parameters, initialize);

	if (results)
	{
		const hs_thread* thread = hs_thread_get(thread_index);
		if (TEST_BIT(thread->flags, _hs_thread_verbose_bit))
		{
			char buffer[10240]{};
			csnzprintf(buffer, sizeof(buffer), "%s: %s ",
				hs_thread_format(thread_index),
				hs_function_get(function_index)->name);

			for (int32 param_index = 0; param_index < function_definition->formal_parameter_count; param_index++)
			{
				char valuebuffer[100]{};
				inspect_internal(function_definition->formal_parameters[param_index], results[param_index], valuebuffer, sizeof(valuebuffer));
				csnzappendf(buffer, sizeof(buffer), "%s ", valuebuffer);
			}
	
			if (hs_thread_stack(thread)->expression_index != NONE)
			{
				hs_syntax_node* expression = hs_syntax_get(hs_thread_stack(thread)->expression_index);
				csnzappendf(buffer, sizeof(buffer), "   (line #%i)", expression->line_number);
			}
	
			event(_event_warning, "hs: %s", buffer);
		}
	}

	return results;
}

int32 __cdecl hs_object_index_from_name_index(int32 thread_index, int16 name_index)
{
	//return INVOKE(0x00597320, hs_object_index_from_name_index, thread_index, name_index);

	int32 result = object_index_from_name_index(name_index);
	if ((thread_index != NONE && name_index != NONE) && result == NONE)
	{
		event(_event_warning, "hs: thread %s attempted to access unplaced object %s",
			hs_thread_format(thread_index),
			TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->object_names, name_index, scenario_object_name)->name);
	}
	return result;
}

int32 __cdecl hs_object_name_to_object_list(int32 object_name_index)
{
	//return INVOKE(0x00597330, hs_object_name_to_object_list, object_name_index);

	int32 object_index = hs_object_index_from_name_index(hs_runtime_globals->executing_thread_index, (int16)object_name_index);
	int32 object_list_index = NONE; // should this just be `hs_object_to_object_list`
	if (object_index != NONE)
	{
		object_list_index = object_list_new();
		if (object_list_index != NONE)
		{
			object_list_add(object_list_index, object_index);
		}
	}
	return object_list_index;
}

int32 __cdecl hs_object_to_object_list(int32 object_index)
{
	//return INVOKE(0x00597370, hs_object_to_object_list, object_index);

	int32 object_list_index = NONE;
	if (object_index != NONE)
	{
		object_list_index = object_list_new();
		if (object_list_index != NONE)
		{
			object_list_add(object_list_index, object_index);
		}
	}
	return object_list_index;
}

bool __cdecl hs_object_type_can_cast(int16 actual_type, int16 desired_type)
{
	//return INVOKE(0x005973A0, hs_object_type_can_cast, actual_type, desired_type);

	ASSERT(actual_type >= 0 && actual_type < NUMBER_OF_HS_OBJECT_TYPES);
	ASSERT(desired_type >= 0 && desired_type < NUMBER_OF_HS_OBJECT_TYPES);
	
	int16 actual_type_mask = hs_object_type_masks[actual_type];
	int16 desired_type_mask = hs_object_type_masks[desired_type];
	
	return actual_type_mask == (actual_type_mask & desired_type_mask);
}

int32 __cdecl hs_real_to_long(int32 r)
{
	return INVOKE(0x005973D0, hs_real_to_long, r);

	//int32 result;
	//result = (int32)(*(real32*)&r);
	//return result;
}

int32 __cdecl hs_real_to_short(int32 r)
{
	return INVOKE(0x005973E0, hs_real_to_short, r);

	//int32 result;
	//result = (int32)(int16)(*(real32*)&r);
	//return result;
}

void __cdecl hs_rebuild_and_compile(char* error_buffer, int32 buffer_length, bool verbose)
{
	hs_runtime_dirty();
	g_error_output_buffer = error_buffer;
	g_error_buffer_length = buffer_length;
	hs_dispose_from_old_map();
	hs_initialize_for_new_map(true, verbose);
	g_error_output_buffer = 0;
	g_error_buffer_length = 0;
}

void __cdecl hs_reset_scripts()
{
	//INVOKE(0x00597400, hs_reset_scripts);

	if (game_is_authoritative())
	{
		s_hs_thread_iterator iterator{};
		for (int32 thread_index = hs_thread_iterator_next(&iterator);
			thread_index != NONE;
			thread_index = hs_thread_iterator_next(&iterator))
		{
			const hs_thread* thread = hs_thread_get(thread_index);
			if (thread->type != _hs_thread_type_runtime_evaluate)
			{
				hs_thread_delete(thread_index, false);
			}
		}

		hs_runtime_initialize_threads();
		ai_handle_script_verification(true);
		ui_handle_script_verification();
	}
}

void __cdecl hs_restore_from_saved_game(int32 game_state_restore_flags)
{
	//INVOKE(0x005974D0, hs_restore_from_saved_game, game_state_restore_flags);

	hs_looper_restore_from_saved_game();
}

void __cdecl hs_return(int32 thread_index, int32 value)
{
	//INVOKE(0x005974E0, hs_return, thread_index, value);

	hs_thread* thread = hs_thread_get(thread_index);
	hs_stack_frame* current_frame = hs_thread_stack(thread);
	const hs_syntax_node* expression = hs_syntax_get(current_frame->expression_index);

	SCRIPT_EXECUTION_ERROR(thread_index, valid_thread(thread_index), "corrupted stack.");

	int16 return_type = _hs_unparsed;
	if (!TEST_BIT(expression->flags, _hs_syntax_node_script_bit))
	{
		return_type = hs_function_get(expression->function_index)->return_type;
	}
	else if (expression->script_index != NONE)
	{
		return_type = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, expression->script_index, hs_script)->return_type;
	}
	else
	{
		return_type = _hs_type_void;
	}

	hs_stack_frame* parent_frame = hs_stack(thread, current_frame->parent);
	int32* destination = hs_destination(thread, parent_frame->child_result);
	if (destination)
	{
		*destination = hs_cast(thread_index, return_type, expression->type, value);
	}

	hs_stack_pop(thread_index);
}

bool __cdecl hs_running_game_scripts()
{
	//return INVOKE(0x005975C0, hs_running_game_scripts);

	return g_run_game_scripts;
}

int32 __cdecl hs_runtime_command_script_begin(int16 script_index)
{
	//return INVOKE(0x005975D0, hs_runtime_command_script_begin, script_index);

	return hs_runtime_script_begin(script_index, _hs_script_command_script, _hs_thread_type_command_script);
}

void __cdecl hs_runtime_delete_internal_global_datums()
{
	//INVOKE(0x00597640, hs_runtime_delete_internal_global_datums);

	c_data_iterator<hs_global_runtime> hs_global_runtime_iterator;
	hs_global_runtime_iterator.begin(hs_global_data);
	while (hs_global_runtime_iterator.next())
	{
		if ((int16)hs_global_runtime_iterator.get_index() >= k_hs_external_global_count)
		{
			datum_delete(hs_global_data, hs_global_runtime_iterator.get_index());
		}
	}
}

void __cdecl hs_runtime_dirty()
{
	//INVOKE(0x005976C0, hs_runtime_dirty);

	ai_reset();
	hs_runtime_dispose_from_old_map();
	hs_compile_initialize(true);
	hs_compile_dispose();
	hs_runtime_initialize_for_new_map();
	ai_handle_script_verification(true);
	ui_handle_script_verification();
}

void __cdecl hs_runtime_dispose()
{
	//INVOKE(0x00597730, hs_runtime_dispose);

	data_make_invalid(hs_global_data);
}

void __cdecl hs_runtime_dispose_from_old_map()
{
	//INVOKE(0x00597750, hs_runtime_dispose_from_old_map);

	data_make_invalid(hs_thread_tracking_data);
	data_make_invalid(hs_thread_deterministic_data);
	data_make_invalid(hs_thread_non_deterministic_data);

	hs_runtime_delete_internal_global_datums();

	if (hs_distributed_global_data->actual_count)
	{
		event(_event_warning, "networking:hs_runtime: hs distributed globals not cleaned up properly");
	}
	data_delete_all(hs_distributed_global_data);

	hs_runtime_globals->initialized = false;
}

bool __cdecl hs_runtime_evaluate(int32 expression_index, bool display_expression_result, bool deterministic)
{
	//return INVOKE(0x005977A0, hs_runtime_evaluate, expression_index, display_expression_result, deterministic);

	int32 result = NONE;

	bool temporary_initialization = false;
	if (!hs_runtime_globals->initialized)
	{
		hs_runtime_initialize_for_new_map();
		if (hs_runtime_globals->initialized)
		{
			temporary_initialization = true;
		}
		else
		{
			event(_event_warning, "design:hs: unable to initialize scripting system to execute that command.");
		}
	}

	if (hs_runtime_globals->initialized)
	{
		if (expression_index != NONE)
		{
			int32 thread_index = hs_thread_new(_hs_thread_type_runtime_evaluate, NONE, deterministic);
			if (thread_index != NONE)
			{
				hs_thread* thread = hs_thread_get(thread_index);
				SET_BIT(thread->flags, _hs_thread_display_expression_bit, display_expression_result);

				hs_destination_pointer destination{};
				destination.destination_type = _hs_destination_thread_result;
				hs_evaluate(thread_index, expression_index, destination, NULL);
				if (TEST_BIT(thread->flags, _hs_thread_in_function_call_bit))
				{
					hs_thread_main(thread_index);
				}
				else
				{
					result = thread->result;
				}
			}
			else
			{
				event(_event_critical, "design:hs: could not allocate thread to execute a command!");
			}
		}
		else
		{
			event(_event_warning, "design:hs: unable to initialize scripting system to execute that command.");
		}

		if (temporary_initialization)
		{
			hs_runtime_dispose_from_old_map();
		}
	}
	return result;
}

const char* __cdecl hs_runtime_get_executing_thread_name()
{
	//return INVOKE(0x00597870, hs_runtime_get_executing_thread_name);

	const char* thread_name = NULL;
	if (hs_runtime_globals->executing_thread_index != NONE)
	{
		thread_name = hs_thread_format(hs_runtime_globals->executing_thread_index);
		if (!thread_name)
		{
			thread_name = "[unknown]";
		}
	}
	return thread_name;
}

int32 __cdecl hs_runtime_index_from_global_designator(int32 designator)
{
	//return INVOKE(0x005978A0, hs_runtime_index_from_global_designator, designator);

	int32 index = designator & MASK(15);
	if (!TEST_BIT(designator, 15))
	{
		index += k_hs_external_global_count;
	}
	return index;
}

void __cdecl hs_runtime_initialize()
{
	//INVOKE(0x005978D0, hs_runtime_initialize);

	hs_thread_deterministic_data = data_new("det hs thread", MAXIMUM_NUMBER_OF_DETERMINISTIC_HS_THREADS, sizeof(hs_thread), 0, &g_hs_thread_deterministic_data_allocator);
	hs_thread_tracking_data = data_new("tracking hs thread", MAXIMUM_NUMBER_OF_HS_THREADS, sizeof(s_hs_thread_tracking_data), 0, &g_hs_thread_tracking_data_allocator);
	hs_thread_non_deterministic_data = data_new("non-det hs thread", MAXIMUM_NUMBER_OF_NON_DETERMINISTIC_HS_THREADS, sizeof(hs_thread), 0, &g_hs_thread_non_deterministic_data_allocator);
	hs_global_data = data_new("hs globals", MAXIMUM_NUMBER_OF_HS_GLOBALS, sizeof(hs_global_runtime), 0, &g_hs_global_data_allocator);
	hs_distributed_global_data = data_new("hs dist. globals", 512, sizeof(s_hs_distributed_global_data), 0, &g_hs_distributed_global_data_allocator);
	
	hs_runtime_globals = (s_hs_runtime_globals*)g_hs_runtime_globals_allocator.allocate(sizeof(s_hs_runtime_globals), "hs runtime globals");
	
	if (hs_thread_tracking_data
		&& hs_thread_deterministic_data
		&& hs_thread_non_deterministic_data
		&& hs_global_data
		&& hs_distributed_global_data)
	{
		VASSERT(2 * k_hs_external_global_count <= MAXIMUM_NUMBER_OF_HS_GLOBALS, "raise MAXIMUM_NUMBER_OF_HS_GLOBALS.");

		data_make_valid(hs_global_data);
		data_make_valid(hs_distributed_global_data);
		for (int32 external_global_index = 0; external_global_index < k_hs_external_global_count; external_global_index++)
		{
			int32 index = datum_new_at_absolute_index(hs_global_data, external_global_index);
			if (index == NONE)
			{
				VASSERT(0, "couldn't add external global.");
			}
			else
			{
				hs_global_runtime* global_runtime = DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime,
					//hs_runtime_index_from_global_designator(external_global_index)
					external_global_index & 0x7FFF
				);
				global_runtime->distributed_global_runtime_index = NONE;
			}
		}
	}
	else
	{
		VASSERT(0, "couldn't allocate scripting globals.");
	}

	hs_typecasting_table_initialize();
	g_run_game_scripts = !game_in_editor();
}

void __cdecl hs_runtime_initialize_for_new_map()
{
	//INVOKE(0x00597A80, hs_runtime_initialize_for_new_map);

	hs_looper_reinitialize();

	data_make_valid(hs_thread_tracking_data);
	data_make_valid(hs_thread_deterministic_data);
	data_make_valid(hs_thread_non_deterministic_data);

	hs_runtime_globals->initialized = true;
	hs_runtime_globals->object_lists_need_gc = false;
	hs_runtime_globals->syntax_data_needs_gc = false;
	hs_runtime_globals->globals_initialization = false;
	hs_runtime_globals->executing_thread_index = NONE;

	if (global_scenario_index_get() != NONE)
	{
		int32 internal_thread_index = hs_thread_new(_hs_thread_type_global_initialize, NONE, true);
		hs_thread* internal_thread = hs_thread_get(internal_thread_index);
		hs_runtime_globals->globals_initialization = true;

		const struct scenario* scenario = global_scenario_get();
		for (int32 global_index = 0; global_index < scenario->hs_globals.count; global_index++)
		{
			const hs_global_internal* internal_global = TAG_BLOCK_GET_ELEMENT(&scenario->hs_globals, global_index, const hs_global_internal);
			int32 global_datum_index = hs_runtime_index_from_global_designator(global_index);
			datum_new_at_absolute_index(hs_global_data, global_datum_index);
			hs_global_runtime* runtime_global = DATUM_GET_ABSOLUTE(hs_global_data, hs_global_runtime, global_datum_index);

			internal_thread->script_index = NONE;
			hs_thread_stack(internal_thread)->size = 0;

			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_runtime_global;
			destination.runtime_global_index = (int16)global_index;
			hs_evaluate(internal_thread_index, internal_global->initialization_expression_index, destination, NULL);

			if (TEST_BIT(internal_thread->flags, _hs_thread_in_function_call_bit))
			{
				hs_thread_main(internal_thread_index);
				if (internal_global->type == _hs_type_object_list)
				{
					object_list_add_reference(hs_global_evaluate((int16)global_index));
				}

				SCRIPT_EXECUTION_ERROR(internal_thread_index, internal_thread->sleep_until == 0, "a global initialization attempted to sleep.");
			}

			hs_global_reconcile_write((int16)(global_index & MASK(15)));
		}

		hs_runtime_globals->globals_initialization = false;
		hs_thread_delete(internal_thread_index, true);

		if (hs_verbose)
		{
			event(_event_warning, "hs: --------------------------HS RESET--------------------------");
		}

		hs_runtime_initialize_threads();
	}
}

void __cdecl hs_runtime_initialize_threads()
{
	//INVOKE(0x00597C70, hs_runtime_initialize_threads);

	if (global_scenario_index_get() != NONE)
	{
		const struct scenario* scenario = global_scenario_get();
		for (int32 script_index = 0; script_index < scenario->hs_scripts.count; script_index++)
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&scenario->hs_scripts, script_index, hs_script);

			bool create_thread_for_script = game_is_predicted() ?
				script->script_type == _hs_script_startup :
				script->script_type == _hs_script_startup || script->script_type == _hs_script_dormant || script->script_type == _hs_script_continuous;

			if (create_thread_for_script)
			{
				int32 new_thread_index = hs_thread_new(_hs_thread_type_script, script_index, true);
				VASSERT(new_thread_index != NONE, "ran out of script threads.");

				if (hs_verbose)
				{
					event(_event_warning, "hs: CREATED %s",
						hs_thread_format(new_thread_index));
				}
			}
		}
	}
}

bool __cdecl hs_runtime_initialized()
{
	//return INVOKE(0x00597CF0, hs_runtime_initialized);

	return hs_runtime_globals->initialized;
}

int32 __cdecl hs_runtime_internal_evaluate(int32 expression_index)
{
	//return INVOKE(0x00597D10, hs_runtime_internal_evaluate, expression_index);

	int32 result = NONE;
	if (!hs_runtime_globals->initialized && expression_index != NONE)
	{
		int32 thread_index = hs_thread_new(_hs_thread_type_runtime_internal_evaluate, NONE, true);
		if (thread_index != NONE)
		{
			hs_thread* thread = hs_thread_get(thread_index);
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_thread_result;
			hs_evaluate(thread_index, expression_index, destination, NULL);
			if (TEST_BIT(thread->flags, _hs_thread_in_function_call_bit))
			{
				hs_thread_main(thread_index);
			}
			result = thread->result;
			hs_thread_delete(thread_index, true);
		}
		else
		{
			event(_event_critical, "design:hs: hs_runtime_internal_evaluate: there are not enough threads to execute that command!!!");
		}
	}
	return result;
}

bool __cdecl hs_runtime_nondeterministic_threads_running()
{
	//return INVOKE(0x00597DD0, hs_runtime_nondeterministic_threads_running);

	bool result = false;

#ifdef USE_HS_THREAD_TRACKING
	s_hs_thread_iterator iterator{};
	hs_thread_iterator_new(&iterator, false, true);
	result = hs_thread_iterator_next(&iterator) != NONE;
#endif

	return result;
}

void __cdecl hs_runtime_push_script(int16 script_index)
{
	//INVOKE(0x00597DE0, hs_runtime_push_script, script_index);

	int32 thread_index = hs_runtime_globals->executing_thread_index;
	if (thread_index != NONE)
	{
		hs_thread* thread = hs_thread_get(thread_index);
		hs_stack_frame* frame = hs_thread_stack(thread);
		int32 expression_index = frame->expression_index;
		hs_script_evaluate(script_index, thread_index, frame->child_result.destination_type != _hs_destination_stack);
		hs_stack_push(thread_index);
		hs_thread_stack(thread)->expression_index = expression_index;
	}
}

void __cdecl hs_runtime_require_gc()
{
	//INVOKE(0x00597E60, hs_runtime_require_gc);

	hs_runtime_globals->syntax_data_needs_gc = true;
}

void __cdecl hs_runtime_require_object_list_gc()
{
	//INVOKE(0x00597E80, hs_runtime_require_object_list_gc);

	hs_runtime_globals->object_lists_need_gc = true;
}

void __cdecl hs_runtime_reset()
{
	//INVOKE(0x00597EA0, hs_runtime_reset);

	hs_runtime_dispose_from_old_map();
	hs_runtime_initialize_for_new_map();
}

void __cdecl hs_runtime_reset_time(int32 previous_time)
{
	//INVOKE(0x00597F00, hs_runtime_reset_time, previous_time);

	if (hs_runtime_globals->initialized)
	{
		int32 time_offset = game_time_get() - previous_time;
	
		s_hs_thread_iterator iterator{};
		hs_thread_iterator_new(&iterator, true, true);
	
		for (int32 thread_index = hs_thread_iterator_next(&iterator);
			thread_index != NONE;
			thread_index = hs_thread_iterator_next(&iterator))
		{
			thread_update_sleep_time_for_reset(thread_index, time_offset);
		}
	}
}

bool __cdecl hs_runtime_safe_to_gc()
{
	//return INVOKE(0x00597FC0, hs_runtime_safe_to_gc);

	bool safe_to_gc = true;
	if (hs_runtime_globals->initialized)
	{
		s_hs_thread_iterator iterator{};
		hs_thread_iterator_new(&iterator, true, true);

		for (int32 thread_index = hs_thread_iterator_next(&iterator);
			thread_index != NONE;
			thread_index = hs_thread_iterator_next(&iterator))
		{
			const hs_thread* thread = hs_thread_get(thread_index);
			if (thread->type == _hs_thread_type_runtime_evaluate)
			{
				safe_to_gc = false;
				break;
			}
		}
	}
	return safe_to_gc;
}

int32 __cdecl hs_runtime_script_begin(int16 script_index, e_hs_script_type script_type, e_hs_thread_type thread_type)
{
	//return INVOKE(0x00598050, hs_runtime_script_begin, script_index, script_type, thread_type);

	hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_index, hs_script);

	int32 thread_index = NONE;

	if (script->script_type == script_type)
	{
		thread_index = hs_thread_new(thread_type, script_index, true);
		if (thread_index != NONE)
		{
			hs_destination_pointer destination{};
			if (hs_verbose)
			{
				event(_event_warning, "hs: SCRIPT STARTED %s",
					hs_thread_format(thread_index));
			}
			destination.destination_type = _hs_destination_thread_result;
			hs_evaluate(thread_index, script->root_expression_index, destination, NULL);
		}
	}
	else
	{
		event(_event_warning, "Error: script is not of type #%ld",
			script_type);
	}

	return thread_index;
}

void __cdecl hs_runtime_update()
{
	//INVOKE(0x005980C0, hs_runtime_update);

	if (hs_runtime_globals->initialized)
	{
		int32 time = game_time_get();
		bool internal_threads = false;

		s_hs_thread_iterator iterator{};
		hs_thread_iterator_new(&iterator, true, true);
		for (int32 thread_index = hs_thread_iterator_next(&iterator);
			thread_index != NONE;
			thread_index = hs_thread_iterator_next(&iterator))
		{
			hs_thread* thread = hs_thread_get(thread_index);
			bool allow = true;

			if (thread->sleep_until == HS_SLEEP_COMMAND_SCRIPT_ATOM && !cs_blocked(thread_index))
			{
				thread->sleep_until = 0;
			}

			if (thread->type == _hs_thread_type_runtime_evaluate)
			{
				internal_threads = true;
			}
			else if (thread->type == _hs_thread_type_command_script)
			{
				cs_setup_global_script_context(thread_index);
			}
			else
			{
				allow = g_run_game_scripts;
			}

			if (allow && IN_RANGE_INCLUSIVE(thread->sleep_until, 0, time))
			{
				hs_thread_main(thread_index);
			}

			cs_global_script_context_clear();
		}

		object_list_gc();

		if (hs_runtime_globals->syntax_data_needs_gc && !internal_threads && !(game_time_get() % 16))
		{
			hs_node_gc();
			hs_runtime_globals->syntax_data_needs_gc = false;
		}
	}
}

void __cdecl hs_script_evaluate(int16 script_index, int32 thread_index, bool initialize)
{
	//INVOKE(0x005981D0, hs_script_evaluate, script_index, thread_index, initialize);

	if (script_index == NONE)
	{
		hs_return(thread_index, 0);
	}
	else
	{
		hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_index, hs_script);
		hs_thread* thread = hs_thread_get(thread_index);

		int16* argument_index = (int16*)hs_stack_allocate(thread_index, sizeof(int16), 1, NULL);
		int32* expression_index = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, NULL);
		hs_stack_pointer result_reference{};
		int32* result = (int32*)hs_stack_allocate(thread_index, sizeof(int32), 2, &result_reference);
		if (argument_index && expression_index && result)
		{
			hs_stack_pointer parameter_reference = { .stack_offset = NONE };
			int32* parameter_results = NULL;
			if (script->parameters.count > 0
				&& (parameter_results = (int32*)hs_stack_allocate(thread_index, sizeof(int32) * script->parameters.count, 2, &parameter_reference)) == NULL)
			{
				event(_event_error, "hs: stack overflow allocating #%d parameters (thread 0x%08X '%s' stack size/max %d/%d)",
					script->parameters.count,
					thread_index,
					hs_thread_format(thread_index),
					hs_thread_stack(thread)->size + thread->stack.stack_offset + sizeof(hs_stack_frame));
			}
			else
			{
				if (initialize)
				{
					*argument_index = 0;
					*expression_index = hs_syntax_get(hs_syntax_get(hs_thread_stack(thread)->expression_index)->long_value)->next_node_index;
				}

				if (*argument_index < script->parameters.count)
				{
					SCRIPT_EXECUTION_ERROR(thread_index, *expression_index != NONE, "corrupted stack.");

					hs_destination_pointer destination;
					destination.destination_type = _hs_destination_stack;
					destination.stack_pointer = parameter_reference;
					destination.stack_pointer.stack_offset += sizeof(int32) * *argument_index;

					hs_evaluate(thread_index, *expression_index, destination, NULL);
					*expression_index = hs_syntax_get(*expression_index)->next_node_index;
				}
				else if (*argument_index == script->parameters.count)
				{
					int32 call_expression_index = script->root_expression_index;
					hs_destination_pointer destination;
					destination.destination_type = _hs_destination_stack;
					destination.stack_pointer = result_reference;
					if (hs_evaluate(thread_index, call_expression_index, destination, NULL))
					{
						if (TEST_BIT(thread->flags, _hs_thread_verbose_bit))
						{
							char buffer[10240]{};

							csnzprintf(buffer, NUMBEROF(buffer), "%s: %s ",
								hs_thread_format(thread_index),
								script->name);

							if (parameter_results)
							{
								for (int16 param_index = 0; param_index < script->parameters.count; param_index++)
								{
									hs_script_parameter* param = TAG_BLOCK_GET_ELEMENT(&script->parameters, param_index, hs_script_parameter);
									char valuebuffer[100]{};
									inspect_internal(param->type, parameter_results[param_index], valuebuffer, NUMBEROF(valuebuffer));
									csnzappendf(buffer, NUMBEROF(buffer), "%s ", valuebuffer);
								}
							}

							if (hs_thread_stack(thread)->expression_index != NONE)
							{
								const hs_syntax_node* expression = hs_syntax_get(hs_thread_stack(thread)->expression_index);
								csnzappendf(buffer, NUMBEROF(buffer), "   (line #%i)", expression->line_number);
							}

							event(_event_warning, "hs: %s", buffer);
						}

						for (int16 parameter_index = 0; parameter_index < script->parameters.count; parameter_index++)
						{
							hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, parameter_index, hs_script_parameter);
							if (HS_TYPE_IS_OBJECT(parameter->type))
							{
								int32 object_index = parameter_results[parameter_index];
								if (object_index != NONE)
								{
									object_datum* object = OBJECT_GET(object_datum, object_index);
									object->object.flags.set(_object_ever_referenced_by_hs_bit, true);
								}
							}
							else if (parameter->type == _hs_type_object_list)
							{
								int32 object_list_index = parameter_results[parameter_index];
								if (object_list_index != NONE)
								{
									object_list_add_reference(object_list_index);
								}
							}
						}

						hs_thread_stack(thread)->parameters = parameter_reference;
						hs_thread_stack(thread)->script_index = script_index;
					}
				}
				else
				{
					for (int16 parameter_index = 0; parameter_index < script->parameters.count; parameter_index++)
					{
						hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, parameter_index, hs_script_parameter);
						if (parameter->type == _hs_type_object_list)
						{
							int32 object_list_index = parameter_results[parameter_index];
							if (object_list_index != NONE)
							{
								object_list_remove_reference(object_list_index);
							}
						}
					}
					hs_return(thread_index, *result);
				}

				++*argument_index;
			}
		}
	}
}

bool __cdecl hs_script_finished(const char* script_name)
{
	//return INVOKE(0x00598570, hs_script_finished, script_name);

	bool result = false;
	int32 thread_index = hs_find_thread_by_name(script_name);
	if (thread_index != NONE)
	{
		const hs_thread* thread = hs_thread_get(thread_index);
		result = thread->sleep_until == HS_SLEEP_FINISHED;
	}
	else
	{
		event(_event_warning, "design:hs: Could not find thread %s", script_name);
	}
	return result;
}

bool __cdecl hs_script_started(const char* script_name)
{
	//return INVOKE(0x005985C0, hs_script_started, script_name);

	bool result = false;
	int32 thread_index = hs_find_thread_by_name(script_name);
	if (thread_index != NONE)
	{
		const hs_thread* thread = hs_thread_get(thread_index);
		result = thread->sleep_until != HS_SLEEP_INDEFINITE || TEST_BIT(thread->flags, _hs_thread_woken_bit);
	}
	else
	{
		event(_event_warning, "design:hs: Could not find thread %s", script_name);
	}
	return result;
}

void __cdecl hs_scripting_debug_thread(const char* thread_name, bool enable)
{
	//INVOKE(0x00598610, hs_scripting_debug_thread, thread_name, enable);

	int32 thread_index = hs_find_thread_by_name(thread_name);
	if (thread_index != NONE)
	{
		hs_thread* thread = hs_thread_get(thread_index);
		SET_BIT(thread->flags, _hs_thread_verbose_bit, enable);
	}
	else
	{
		event(_event_warning, "HS ERROR: Unable to find thread %s", thread_name);
	}
}

int32 __cdecl hs_scripting_get_executing_thread_index()
{
	//return INVOKE(0x00598620, hs_scripting_get_executing_thread_index);

	return hs_runtime_globals->executing_thread_index;
}

void __cdecl hs_scripting_kill_all_threads()
{
	//INVOKE(0x00598640, hs_scripting_kill_all_threads);

	s_hs_thread_iterator iterator{};
	hs_thread_iterator_new(&iterator, true, true);
	for (int32 thread_index = hs_thread_iterator_next(&iterator);
		thread_index != NONE;
		thread_index = hs_thread_iterator_next(&iterator))
	{
		if (thread_index != hs_runtime_globals->executing_thread_index)
		{
			hs_scripting_kill_running_thread(thread_index);
		}
	}
}

void __cdecl hs_scripting_kill_running_thread(int32 thread_index)
{
	//INVOKE(0x005986F0, hs_scripting_kill_running_thread, thread_index);

	hs_thread* thread = hs_thread_get(thread_index);
	bool terminate = thread->type < NUMBER_OF_HS_THREAD_TYPES && thread->sleep_until != HS_SLEEP_FINISHED && thread->sleep_until != HS_SLEEP_INDEFINITE;
	if (terminate)
	{
		thread->flags |= FLAG(_hs_thread_terminate_bit);
		thread->sleep_until = 0;
	}
}

void __cdecl hs_scripting_show_thread(const char* substring, bool show)
{
	s_hs_thread_iterator iterator{};
	hs_thread_iterator_new(&iterator, true, true);
	for (int32 thread_index = hs_thread_iterator_next(&iterator);
		thread_index != NONE;
		thread_index = hs_thread_iterator_next(&iterator))
	{
		hs_thread* thread = hs_thread_get(thread_index);
		const char* thread_name = hs_thread_format(thread_index);
		if (!strlen_debug(thread_name) || strstr(thread_name, substring))
		{
			SET_BIT(thread->flags, _hs_thread_hide_display_bit, !show);
		}
	}
}

int32 __cdecl hs_short_to_boolean(int32 s)
{
	return INVOKE(0x00598740, hs_short_to_boolean, s);

	//int32 result = s == 0;
	//return result;
}

int32 __cdecl hs_short_to_long(int32 s)
{
	return INVOKE(0x00598760, hs_short_to_long, s);

	//int32 result;
	//*(int16*)&result = (int16)s;
	//return result;
}

int32 __cdecl hs_short_to_real(int32 s)
{
	return INVOKE(0x00598770, hs_short_to_real, s);

	//int32 result;
	//*(real32*)&result = (real32)(int16)s;
	//return result;
}

hs_stack_frame* __cdecl hs_stack(hs_thread* thread, hs_stack_pointer stack_pointer)
{
	//return INVOKE(0x00598790, hs_stack, thread, stack_pointer);
	//return DECLFUNC(0x00598790, hs_stack_frame*, __cdecl, hs_thread*, hs_stack_pointer)(thread, stack_pointer);

	ASSERT(stack_pointer.stack_offset >= 0 && stack_pointer.stack_offset + sizeof(struct hs_stack_frame) <= sizeof(thread->stack_data));

	hs_stack_frame* result = (hs_stack_frame*)&thread->stack_data[stack_pointer.stack_offset];
	return result;
}

const hs_stack_frame* __cdecl hs_stack(const hs_thread* thread, hs_stack_pointer stack_pointer)
{
	//return INVOKE(0x005987B0, hs_stack, thread, stack_pointer);
	//return DECLFUNC(0x005987B0, const hs_stack_frame*, __cdecl, const hs_thread*, hs_stack_pointer)(thread, stack_pointer);

	ASSERT(stack_pointer.stack_offset >= 0 && stack_pointer.stack_offset + sizeof(struct hs_stack_frame) <= sizeof(thread->stack_data));

	const hs_stack_frame* result = (const hs_stack_frame*)&thread->stack_data[stack_pointer.stack_offset];
	return result;
}

void* __cdecl hs_stack_allocate(int32 thread_index, int32 size, int32 alignment_bits, hs_stack_pointer* out_reference)
{
	//return INVOKE(0x005987D0, hs_stack_allocate, thread_index, size, alignment_bits, out_reference);

	hs_thread* thread = hs_thread_get(thread_index);
	hs_stack_frame* frame = hs_thread_stack(thread);
	int32 unaligned_offset = thread->stack.stack_offset + sizeof(hs_stack_frame) + frame->size;
	void* unaligned_result = align_pointer(&thread->stack_data[unaligned_offset], alignment_bits);
	int32 alignment_pad = pointer_distance(&thread->stack_data[unaligned_offset], unaligned_result);
	void* result = NULL;

	ASSERT(alignment_bits <= 4);
	SCRIPT_EXECUTION_ERROR(thread_index, valid_thread(thread_index), "corrupted stack.");
	SCRIPT_EXECUTION_ERROR(thread_index, size, "attempt to allocate zero space from the stack.");
	ASSERT(alignment_pad >= 0);

	int32 allocation_offset = alignment_pad + unaligned_offset;
	if (allocation_offset + size > sizeof(thread->stack_data))
	{
		*(int16*)allocation_offset = -1;
		thread->flags |= FLAG(_hs_thread_terminate_bit);
		thread->stack.stack_offset = 0;

		event(_event_critical, "thread %s STACK OVERFLOW. Terminating thread.",
			hs_thread_format(thread_index));
	}
	else
	{
		ASSERT(allocation_offset >= 0);
		result = &thread->stack_data[allocation_offset];
		frame->size += (int16)(alignment_pad + size);

		ASSERT(thread->stack.stack_offset + (int32)sizeof(struct hs_stack_frame) + frame->size == allocation_offset + size);

		if (alignment_pad > 0)
		{
			csmemset(&thread->stack_data[unaligned_offset], 0, alignment_pad);
		}

		if (!(allocation_offset % sizeof(int32)) && size < sizeof(int32))
		{
			csmemset(&((byte*)result)[size], 0, sizeof(int32) - size);
		}
	}

	if (out_reference)
	{
		out_reference->stack_offset = (int16)allocation_offset;
	}

	return result;
}

int32* __cdecl hs_stack_destination(hs_thread* thread, hs_stack_pointer stack_pointer)
{
	//return INVOKE(0x005988C0, hs_stack_destination, thread, stack_pointer);

	int32* results = (int32*)&thread->stack_data[stack_pointer.stack_offset];
	return results;
}

int32* __cdecl hs_stack_parameters(hs_thread* thread, hs_stack_frame* stack_frame, int32 parameter_count)
{
	//return INVOKE(0x005988E0, hs_stack_parameters, thread, stack_frame, parameter_count);

	ASSERT(stack_frame->parameters.stack_offset >= 0 && stack_frame->parameters.stack_offset + parameter_count * sizeof(int32) <= sizeof(thread->stack_data));
	int32* results = (int32*)&thread->stack_data[stack_frame->parameters.stack_offset];
	return results;
}

void __cdecl hs_stack_pop(int32 thread_index)
{
	//INVOKE(0x00598900, hs_stack_pop, thread_index);

	hs_thread* thread = hs_thread_get(thread_index);
	ASSERT(hs_stack(thread, hs_thread_stack(thread)->parent) != NULL);
	thread->stack = hs_thread_stack(thread)->parent;
}

bool __cdecl hs_stack_push(int32 thread_index)
{
	//return INVOKE(0x00598940, hs_stack_push, thread_index);

	hs_thread* thread = hs_thread_get(thread_index);

	hs_stack_pointer new_frame_pointer = thread->stack;
	new_frame_pointer.stack_offset += sizeof(hs_stack_frame) + hs_thread_stack(thread)->size;

	bool result = false;
	if (new_frame_pointer.stack_offset + sizeof(hs_stack_frame) > sizeof(thread->stack_data))
	{
		thread->flags |= FLAG(_hs_thread_terminate_bit);
		thread->stack.stack_offset = 0;

		event(_event_critical, "thread %s STACK OVERFLOW. Terminating thread.",
			hs_thread_format(thread_index));
	}
	else
	{
		hs_stack_frame* new_frame = hs_stack(thread, new_frame_pointer);
		csmemset(new_frame, 0, sizeof(hs_stack_frame));

		new_frame->parent = thread->stack;
		thread->stack = new_frame_pointer;

		new_frame->parameters.stack_offset = NONE;
		new_frame->script_index = NONE;
		new_frame->expression_index = NONE;

		result = true;
	}
	return result;
}

int32 __cdecl hs_string_to_boolean(int32 n)
{
	return INVOKE(0x005989E0, hs_string_to_boolean, n);

	//return hs_long_to_boolean(strlen_debug((const char*)n));
}

hs_syntax_node* __cdecl hs_syntax_get(int32 index)
{
	//return INVOKE(0x00598A10, hs_syntax_get, index);

	return (struct hs_syntax_node*)datum_get(g_hs_syntax_data, index);
}

bool __cdecl hs_syntax_node_exists(int32 index)
{
	return datum_try_and_get(g_hs_syntax_data, index) != NULL;
}

int32 __cdecl hs_syntax_nth(int32 expression_index, int16 n)
{
	//INVOKE(0x00598A30, hs_syntax_nth, expression_index, n);

	while (n-- > 0)
	{
		expression_index = hs_syntax_get(expression_index)->next_node_index;
	}
	return expression_index;
}

int32 hs_thread_allocate(bool deterministic)
{
#ifndef USE_HS_THREAD_TRACKING
	int32 thread_index = datum_new(hs_thread_deterministic_data);
#else
	int32 thread_index = datum_new(hs_thread_tracking_data);

	if (thread_index != NONE)
	{
		s_hs_thread_tracking_data* tracking_data = DATUM_GET(hs_thread_tracking_data, s_hs_thread_tracking_data, thread_index);

		if (deterministic)
		{
			tracking_data->type = _hs_thread_tracking_deterministic;
			tracking_data->index = datum_new(hs_thread_deterministic_data);
		}
		else
		{
			tracking_data->type = _hs_thread_tracking_non_deterministic;
			tracking_data->index = datum_new(hs_thread_non_deterministic_data);
		}

		if (tracking_data->index == NONE)
		{
			event(_event_error, "hs: failed to allocate %s datum",
				deterministic ? "deterministic" : "non-deterministic");

			datum_delete(hs_thread_tracking_data, thread_index);
			thread_index = NONE;
		}
	}
#endif
	return thread_index;
}

void __cdecl hs_thread_delete(int32 thread_index, bool validate)
{
	//INVOKE(0x00598A60, hs_thread_delete, thread_index, validate);

	if (validate)
	{
		ASSERT(hs_thread_get(thread_index)->type != _hs_thread_type_script);
	}
	
	cs_handle_thread_delete(thread_index);
	hs_looper_handle_thread_delete(thread_index);
	//cinematic_handle_thread_delete(thread_index);

#ifndef USE_HS_THREAD_TRACKING
	datum_delete(hs_thread_deterministic_data, thread_index);
#else
	s_hs_thread_tracking_data* tracking_data = DATUM_GET(hs_thread_tracking_data, s_hs_thread_tracking_data, thread_index);
	switch (tracking_data->type)
	{
	case _hs_thread_tracking_deterministic:
	{
		datum_delete(hs_thread_deterministic_data, tracking_data->index);
	}
	break;
	case _hs_thread_tracking_non_deterministic:
	{
		datum_delete(hs_thread_non_deterministic_data, tracking_data->index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
	datum_delete(hs_thread_tracking_data, thread_index);
#endif
}

const char* __cdecl hs_thread_format(int32 thread_index)
{
	//return INVOKE(0x00598A90, hs_thread_format, thread_index);

	const hs_thread* thread = hs_thread_get(thread_index);

	const char* result = NULL;
	switch (thread->type)
	{
	case _hs_thread_type_script:
	case _hs_thread_type_runtime_internal_evaluate:
	case _hs_thread_type_command_script:
	{
		int32 script_index = thread->script_index;
		if (script_index == NONE)
		{
			result = "[unknown thread type]";
		}
		else
		{
			result = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_index, hs_script)->name;
		}
	}
	break;
	case _hs_thread_type_global_initialize:
	{
		result = "[global initialize]";
	}
	break;
	case _hs_thread_type_runtime_evaluate:
	{
		result = "[console command]";
	}
	break;
	default:
	{
		result = "[unknown thread type]";
	}
	break;
	}
	return result;
}

hs_thread* hs_thread_get(int32 thread_index)
{
	hs_thread* thread = NULL;

#ifndef USE_HS_THREAD_TRACKING
	thread = DATUM_TRY_AND_GET(hs_thread_deterministic_data, hs_thread, thread_index);
#else
	s_hs_thread_tracking_data* tracking_data = DATUM_GET(hs_thread_tracking_data, s_hs_thread_tracking_data, thread_index);
	switch (tracking_data->type)
	{
	case _hs_thread_tracking_deterministic:
	{
		thread = DATUM_TRY_AND_GET(hs_thread_deterministic_data, hs_thread, tracking_data->index);
	}
	break;
	case _hs_thread_tracking_non_deterministic:
	{
		thread = DATUM_TRY_AND_GET(hs_thread_non_deterministic_data, hs_thread, tracking_data->index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
#endif

	return thread;
}

int32 hs_thread_get_tracking_index_by_non_deterministic_thread_index(int32 non_deterministic_thread_index)
{
	hs_thread* thread = DATUM_GET(hs_thread_non_deterministic_data, hs_thread, non_deterministic_thread_index);
	ASSERT(thread->tracking_index != NONE);
	return thread->tracking_index;
}

int32 hs_thread_get_tracking_index_by_deterministic_thread_index(int32 non_deterministic_thread_index)
{
	hs_thread* thread = DATUM_GET(hs_thread_deterministic_data, hs_thread, non_deterministic_thread_index);
	ASSERT(thread->tracking_index != NONE);
	return thread->tracking_index;
}

bool __cdecl hs_thread_is_deterministic(int32 thread_index)
{
	//return INVOKE(0x00598B10, hs_thread_is_deterministic, thread_index);

#ifndef USE_HS_THREAD_TRACKING
	bool deterministic = true;
#else
	s_hs_thread_tracking_data* tracking_data = DATUM_GET(hs_thread_tracking_data, s_hs_thread_tracking_data, thread_index);
	bool deterministic = tracking_data->type == _hs_thread_tracking_deterministic;
#endif
	return deterministic;
}

void __cdecl hs_thread_iterator_new(s_hs_thread_iterator* iterator, bool deterministic, bool non_deterministic)
{
	//INVOKE(0x00598B20, hs_thread_iterator_new, iterator, deterministic, non_deterministic);

#ifndef USE_HS_THREAD_TRACKING
	if (hs_runtime_globals->initialized)
	{
		iterator->iterate_deterministic = deterministic;
		iterator->iterate_non_deterministic = non_deterministic;
		iterator->raw_thread_index = NONE;
		iterator->non_deterministic = false;
		if (iterator->iterate_deterministic)
		{
			iterator->raw_thread_index = data_next_index(hs_thread_deterministic_data, NONE);
		}
	}
#else
	if (hs_runtime_globals->initialized)
	{
		iterator->iterate_deterministic = deterministic;
		iterator->iterate_non_deterministic = non_deterministic;
		iterator->raw_thread_index = NONE;
		iterator->non_deterministic = !iterator->iterate_deterministic && iterator->iterate_non_deterministic;

		if (iterator->non_deterministic)
		{
			iterator->raw_thread_index = data_next_index(hs_thread_non_deterministic_data, NONE);
		}
		else if (iterator->iterate_deterministic)
		{
			iterator->raw_thread_index = data_next_index(hs_thread_deterministic_data, NONE);
			if (iterator->raw_thread_index == NONE && iterator->iterate_non_deterministic)
			{
				iterator->raw_thread_index = data_next_index(hs_thread_non_deterministic_data, NONE);
				iterator->non_deterministic = true;
			}
		}
	}
#endif
}

int32 __cdecl hs_thread_iterator_next(s_hs_thread_iterator* iterator)
{
	//return INVOKE(0x00598B70, hs_thread_iterator_next, iterator);

	int32 result = NONE;
#ifndef USE_HS_THREAD_TRACKING
	if (hs_runtime_globals->initialized && iterator->raw_thread_index != NONE)
	{
		result = iterator->raw_thread_index;
		iterator->raw_thread_index = data_next_index(hs_thread_deterministic_data, iterator->raw_thread_index);
	}
#else
	if (hs_runtime_globals->initialized && iterator->raw_thread_index != NONE)
	{
		if (iterator->non_deterministic)
		{
			result = hs_thread_get_tracking_index_by_non_deterministic_thread_index(iterator->raw_thread_index);
			iterator->raw_thread_index = data_next_index(hs_thread_non_deterministic_data, iterator->raw_thread_index);
		}
		else
		{
			result = hs_thread_get_tracking_index_by_deterministic_thread_index(iterator->raw_thread_index);
			iterator->raw_thread_index = data_next_index(hs_thread_deterministic_data, iterator->raw_thread_index);

			if (iterator->raw_thread_index == NONE && iterator->iterate_non_deterministic)
			{
				iterator->raw_thread_index = data_next_index(hs_thread_non_deterministic_data, NONE);
				iterator->non_deterministic = true;
			}
		}
	}
#endif
	return result;
}

void __cdecl hs_thread_main(int32 thread_index)
{
	//INVOKE(0x00598BC0, hs_thread_main, thread_index);

	hs_thread* thread = hs_thread_get(thread_index);
	hs_script* script = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->hs_scripts, thread->script_index, hs_script);

	SCRIPT_EXECUTION_ERROR(thread_index, valid_thread(thread_index), "corrupted stack.");
	thread->sleep_until = 0;

	if (!thread->stack.stack_offset)
	{
		ASSERT(script);

		hs_thread_stack(thread)->size = 0;

		hs_stack_pointer start_reference{};
		if (hs_stack_allocate(thread_index, sizeof(hs_destination_pointer), 2, &start_reference))
		{
			hs_destination_pointer destination{};
			destination.destination_type = _hs_destination_stack;
			destination.stack_pointer = start_reference;
			hs_evaluate(thread_index, script->root_expression_index, destination, NULL);
		}
	}

	while (thread->stack.stack_offset
		&& thread->sleep_until >= 0
		&& (!game_in_progress() || thread->sleep_until <= game_time_get())
		&& !TEST_BIT(thread->flags, _hs_thread_terminate_bit)
		&& hs_runtime_globals->initialized)
	{
		if (thread->type == _hs_thread_type_runtime_evaluate)
		{
			if (!hs_syntax_node_exists(hs_thread_stack(thread)->expression_index))
			{
				event(_event_warning, "terminating console script unexpectedly");
				thread->flags |= FLAG(_hs_thread_terminate_bit);
				break;
			}
		}

		hs_syntax_node* expression = hs_syntax_get(hs_thread_stack(thread)->expression_index);

		bool call = TEST_BIT(thread->flags, _hs_thread_in_function_call_bit);
		hs_thread_stack(thread)->size = 0;
		thread->flags &= ~FLAG(_hs_thread_in_function_call_bit);

		if (TEST_BIT(expression->flags, _hs_syntax_node_script_bit))
		{
			hs_script_evaluate(expression->script_index, thread_index, call);
		}
		else
		{
			const hs_function_definition* function = hs_function_get(expression->function_index);
			ASSERT(function->evaluate);
			function->evaluate(expression->function_index, thread_index, call);
		}
	}

	if (hs_runtime_globals->object_lists_need_gc && !hs_runtime_globals->globals_initialization)
	{
		object_list_gc();
		hs_runtime_globals->object_lists_need_gc = false;
	}

	if (!thread->stack.stack_offset || TEST_BIT(thread->flags, _hs_thread_terminate_bit))
	{
		bool finished = true;

		if (TEST_BIT(thread->ai_flags, _hs_thread_ai_cleanup_bit))
		{
			int16 script_index = thread->script_index;
			thread->script_index = thread->cleanup_script_index;
			thread->cleanup_script_index = script_index;
		}
		else if (thread->cleanup_script_index != NONE)
		{
			hs_script* cleanup_script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, thread->cleanup_script_index, hs_script);
			ASSERT(cleanup_script);

			if (cleanup_script->script_type != _hs_script_static || cleanup_script->parameters.count > 0)
			{
				event(_event_error, "design:hs: invalid cleanup script (must be a static script that takes no parameters): %s",
					cleanup_script->name);
			}
			else
			{
				int16 script_index = thread->script_index;
				thread->script_index = thread->cleanup_script_index;
				thread->cleanup_script_index = script_index;
				thread->stack.stack_offset = 0;
				thread->flags &= MASK(_hs_thread_terminate_bit) | ~MASK(_hs_thread_abort_bit);
				thread->ai_flags |= FLAG(_hs_thread_ai_cleanup_bit);

				finished = false;
			}
		}

		if (finished)
		{
			switch (thread->type)
			{
			case _hs_thread_type_script:
			{
				ASSERT(script);

				if (script->script_type == _hs_script_startup || script->script_type == _hs_script_dormant)
				{
					thread->sleep_until = HS_SLEEP_FINISHED;

					if (hs_verbose || TEST_BIT(thread->flags, _hs_thread_verbose_bit))
					{
						event(_event_warning, "hs: FINISHED %s", hs_thread_format(thread_index));
						if (TEST_BIT(thread->flags, _hs_thread_verbose_bit))
						{
							event(_event_warning, "hs: FINISHED %s", hs_thread_format(thread_index));
						}
					}
				}
			}
			break;
			case _hs_thread_type_runtime_evaluate:
			{
				hs_thread_delete(thread_index, true);
			}
			break;
			case _hs_thread_type_command_script:
			{
				if (hs_verbose || TEST_BIT(thread->flags, _hs_thread_verbose_bit))
				{
					event(_event_warning, "hs: COMMAND-SCRIPT FINISHED %s", hs_thread_format(thread_index));
					if (TEST_BIT(thread->flags, _hs_thread_verbose_bit))
					{
						event(_event_warning, "hs: COMMAND-SCRIPT FINISHED %s", hs_thread_format(thread_index));
					}
				}

				hs_thread_delete(thread_index, true);
			}
			break;
			}
		}
	}

	hs_runtime_globals->executing_thread_index = NONE;
}

int32 __cdecl hs_thread_new(e_hs_thread_type type, int32 script_index, bool deterministic)
{
	//return INVOKE(0x00598E70, hs_thread_new, type, script_index, deterministic);

	int32 thread_index = hs_thread_allocate(deterministic);

	ASSERT(type >= 0 && type < NUMBER_OF_HS_THREAD_TYPES);
	ASSERT(type != _hs_thread_type_script || script_index != NONE);

	if (deterministic && thread_index == NONE)
	{
		event(_event_critical, "design:hs: hs_thread_new: Unable to create a new deterministic thread. This is the apocalypse.");
	}
	else if (thread_index != NONE)
	{
		hs_thread* thread = hs_thread_get(thread_index);
		thread->stack.stack_offset = 0;

		hs_stack_frame* stack = hs_thread_stack(thread);
		csmemset(stack, 0, sizeof(hs_stack_frame));
		stack->parent.stack_offset = NONE;
		stack->parameters.stack_offset = NONE;
		stack->script_index = NONE;
		stack->expression_index = NONE;

		thread->type = type;
		thread->script_index = (int16)script_index;
		thread->cleanup_script_index = NONE;
		thread->flags = 0;
		thread->ai_flags = 0;
		thread->tracking_index = thread_index;
		thread->ai_index = 0;
		thread->ai_data = 0;
		thread->sleep_until = 0;
		if (script_index != NONE && TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_index, hs_script)->script_type == _hs_script_dormant)
		{
			thread->sleep_until = HS_SLEEP_INDEFINITE;
		}
	}
	return thread_index;
}

hs_stack_frame* __cdecl hs_thread_stack(hs_thread* thread)
{
	//return INVOKE(0x00598F30, hs_thread_stack, thread);
	//return DECLFUNC(0x00598F30, hs_stack_frame*, __cdecl, hs_thread*)(thread);

	return hs_stack(thread, thread->stack);
}

const hs_stack_frame* __cdecl hs_thread_stack(const hs_thread* thread)
{
	//return INVOKE(0x00598F50, hs_thread_stack, thread);
	//return DECLFUNC(0x00598F50, const hs_stack_frame*, __cdecl, const hs_thread*)(thread);

	return hs_stack(thread, thread->stack);
}

void __cdecl hs_thread_try_to_delete(int32 thread_index, bool validate)
{
	//INVOKE(0x00598F70, hs_thread_try_to_delete, thread_index, validate);

	if (datum_try_and_get(hs_thread_tracking_data, thread_index))
	{
		hs_thread_delete(thread_index, validate);
	}
}

void __cdecl hs_typecasting_table_initialize()
{
	//INVOKE(0x00598FC0, hs_typecasting_table_initialize);

	int32 actual_type = NONE;

	for (actual_type = (FIRST_HS_TYPE + 1); actual_type < NUMBER_OF_HS_NODE_TYPES; actual_type++)
	{
		g_typecasting_procedures[_hs_type_void][actual_type] = hs_data_to_void;
	}

	g_typecasting_procedures[_hs_type_boolean][_hs_type_real] = hs_long_to_boolean;
	g_typecasting_procedures[_hs_type_boolean][_hs_type_short_integer] = hs_short_to_boolean;
	g_typecasting_procedures[_hs_type_boolean][_hs_type_long_integer] = hs_long_to_boolean;
	g_typecasting_procedures[_hs_type_boolean][_hs_type_string] = hs_string_to_boolean;

	g_typecasting_procedures[_hs_type_real][_hs_type_short_integer] = hs_short_to_real;
	g_typecasting_procedures[_hs_type_real][_hs_type_long_integer] = hs_long_to_real;

	for (actual_type = FIRST_HS_ENUM_TYPE; actual_type <= LAST_HS_ENUM_TYPE; actual_type++)
	{
		g_typecasting_procedures[_hs_type_real][actual_type] = hs_enum_to_real;
	}

	g_typecasting_procedures[_hs_type_short_integer][_hs_type_real] = hs_real_to_short;
	g_typecasting_procedures[_hs_type_short_integer][_hs_type_long_integer] = hs_long_to_short;

	g_typecasting_procedures[_hs_type_long_integer][_hs_type_real] = hs_real_to_long;
	g_typecasting_procedures[_hs_type_long_integer][_hs_type_short_integer] = hs_short_to_long;

	for (actual_type = FIRST_HS_OBJECT_TYPE; actual_type <= LAST_HS_OBJECT_TYPE; actual_type++)
	{
		g_typecasting_procedures[_hs_type_object_list][actual_type] = hs_object_to_object_list;
	}

	for (actual_type = FIRST_HS_OBJECT_NAME_TYPE; actual_type <= LAST_HS_OBJECT_NAME_TYPE; actual_type++)
	{
		g_typecasting_procedures[_hs_type_object_list][actual_type] = hs_object_name_to_object_list;
	}

	g_typecasting_procedures[_hs_type_object_list][_hs_type_ai] = object_list_from_ai_reference;
}

void __cdecl hs_wake(int32 thread_index, int32 waking_thread_index)
{
	//INVOKE(0x00599170, hs_wake, thread_index, waking_thread_index);

	hs_thread* thread = hs_thread_get(thread_index);
	if (thread->sleep_until != HS_SLEEP_FINISHED && thread->sleep_until != HS_SLEEP_COMMAND_SCRIPT_ATOM)
	{
		thread->sleep_until = 0;
		if (TEST_BIT(thread->flags, _hs_thread_latent_sleep_bit))
		{
			thread->sleep_until = thread->latent_sleep_until;
			thread->flags &= ~FLAG(_hs_thread_latent_sleep_bit);
		}
		else
		{
			hs_stack_frame* stack = hs_thread_stack(thread);
			if (stack->expression_index == NONE
				|| hs_syntax_get(stack->expression_index)->function_index != _hs_function_sleep_until)
			{
				if (stack->parent.stack_offset != NONE)
				{
					hs_stack_frame* parent_stack = hs_stack(thread, stack->parent);
					if (parent_stack->expression_index != NONE && hs_syntax_get(parent_stack->expression_index)->function_index == _hs_function_sleep_until)
					{
						hs_stack_pop(thread_index);
						hs_stack_pop(thread_index);
						thread->flags &= ~FLAG(_hs_thread_in_function_call_bit);
					}
				}
			}
			else
			{
				hs_stack_pop(thread_index);
			}
		}
		thread->flags |= FLAG(_hs_thread_woken_bit);

		if (hs_verbose || TEST_BIT(thread->flags, _hs_thread_verbose_bit))
		{
			int16 line_number = NONE;
			if (waking_thread_index == NONE)
			{
				event(_event_warning, "hs: WAKE %s (line #%i)",
					hs_thread_format(thread_index),
					line_number);

				if (TEST_BIT(thread->flags, _hs_thread_verbose_bit))
				{
					event(_event_warning, "hs: WAKE %s (line #%i)",
						hs_thread_format(thread_index),
						line_number);
				}
			}
			else
			{
				hs_thread* waking_thread = hs_thread_get(waking_thread_index);
				if (hs_thread_stack(waking_thread)->expression_index != NONE)
				{
					hs_syntax_node* expression = hs_syntax_get(hs_thread_stack(waking_thread)->expression_index);
					line_number = expression->line_number;
				}

				event(_event_warning, "hs: WAKE %s (by %s   line #%i)",
					hs_thread_format(thread_index),
					hs_thread_format(waking_thread_index),
					line_number);

				if (TEST_BIT(thread->flags, _hs_thread_verbose_bit))
				{
					event(_event_warning, "hs: WAKE %s (by %s   line #%i)",
						hs_thread_format(thread_index),
						hs_thread_format(waking_thread_index),
						line_number);
				}
			}
		}
	}
}

bool __cdecl hs_wake_by_name(const char* script_name)
{
	//return INVOKE(0x00599250, hs_wake_by_name, script_name);

	bool success = false;
	int32 thread_index = hs_find_thread_by_name(script_name);
	if (thread_index != NONE)
	{
		hs_wake(thread_index, NONE);
		success = true;
	}
	return success;
}

void __cdecl inspect_internal(int16 type, int32 value, char* buffer, int16 buffer_size)
{
	//INVOKE(0x00599280, inspect_internal, type, value, buffer, buffer_size);

	if (hs_type_inspectors[type])
	{
		hs_type_inspectors[type](type, value, buffer, buffer_size);
	}
	else
	{
		csstrnzcpy(buffer, "[value unavailable]", buffer_size);
	}
}

//.text:005992C0 ; private: virtual bool c_data_iterator<hs_global_runtime>::next()
//.text:005992E0 ; tls
//.text:00599320 ; tls
//.text:00599360 ; tls
//.text:005993A0 ; tls
//.text:005993E0 ; tls
//.text:00599420 ; tls
//.text:00599460 ; 

void __cdecl render_debug_scripting()
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
		for (int32 thread_index = hs_thread_iterator_next(&iterator);
			thread_index != NONE;
			thread_index = hs_thread_iterator_next(&iterator))
		{
			thread_render_debug_scripting(thread_index, buffer, sizeof(buffer));
		}
	
		draw_string.draw(&font_cache, buffer);
	}
	
	render_debug_scripting_globals();
}

void __cdecl render_debug_scripting_globals()
{
	if (debug_globals && hs_global_data)
	{
		char buffer[10240]{};

		c_rasterizer_draw_string draw_string{};
		c_font_cache_mt_safe font_cache{};
		int16 tab_stops[]{ 300 };
		struct scenario* scenario = global_scenario_get();
		interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
		draw_string.set_tab_stops(tab_stops, NUMBEROF(tab_stops));

		csnzprintf(buffer, sizeof(buffer), "|n|n|nglobal name|tvalue");
		for (int32 global_index = 0; global_index < scenario->hs_globals.count; global_index++)
		{
			hs_global_internal& global = scenario->hs_globals[global_index];
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

void __cdecl render_debug_trigger_volumes()
{
	if (debug_trigger_volumes)
	{
		const struct scenario* scenario = global_scenario_get();
		
		for (int32 trigger_index = 0; trigger_index < scenario->trigger_volumes.count; trigger_index++)
		{
			const scenario_trigger_volume* volume = TAG_BLOCK_GET_ELEMENT(&scenario->trigger_volumes, trigger_index, const scenario_trigger_volume);

			real_matrix4x3 matrix{};
			if (trigger_volume_get_matrix(volume, &matrix))
			{
				real_vector3d local_extent = volume->extents;
				real_vector3d world_extent{};
				matrix4x3_transform_vector(&matrix, &local_extent, &world_extent);

				for (int32 face = 0; face < 6; face++)
				{
					int16 side = int16(face / 2);
					real_point3d corner[4]{};
					real_vector3d sides[2]{};

					if (face % 2)
					{
						point_from_line3d(&matrix.position, &world_extent, 1.0f, &corner[0]);
						sides[0].n[(side + 1) % 3] = -local_extent.n[(side + 1) % 3];
						sides[1].n[(side + 2) % 3] = -local_extent.n[(side + 2) % 3];
					}
					else
					{
						corner[0] = matrix.position;
						sides[0].n[(side + 1) % 3] = local_extent.n[(side + 1) % 3];
						sides[1].n[(side + 2) % 3] = local_extent.n[(side + 2) % 3];
					}

					matrix4x3_transform_vector(&matrix, &sides[0], &sides[0]);
					matrix4x3_transform_vector(&matrix, &sides[1], &sides[1]);
					point_from_line3d(&corner[0], &sides[0],  1.0f, &corner[1]);
					point_from_line3d(&corner[1], &sides[1],  1.0f, &corner[2]);
					point_from_line3d(&corner[2], &sides[0], -1.0f, &corner[3]);

					if (hs_debug_data.activated_trigger_volumes.test(trigger_index))
					{
						render_debug_polygon_edges(corner, NUMBEROF(corner), global_real_argb_blue);
					}
					else
					{
						real_argb_color color = *global_real_argb_blue;
						color.alpha = 0.15f;
						render_debug_polygon_edges(corner, NUMBEROF(corner), global_real_argb_red);
						render_debug_polygon(corner, NUMBEROF(corner), &color);
					}
				}

				real_point3d center{};
				point_from_line3d(&matrix.position, &world_extent, 0.5f, &center);

				real_vector3d ray{};
				const render_camera* camera = c_player_view::get_current()->get_rasterizer_camera();
				vector_from_points3d(&camera->position, &center, &ray);
				scale_vector3d(&ray, 0.95f, &ray);

				collision_result result;
				if (!collision_test_vector(_collision_test_for_line_of_sight_obstruction_flags, &camera->position, &ray, NONE, NONE, &result))
				{
					const char* volume_name = volume->name.get_string();
					render_debug_string_at_point(&center,
						volume_name,
						hs_debug_data.activated_trigger_volumes.test(trigger_index) ? global_real_argb_yellow : global_real_argb_white);
				}
			}
		}
	}
}

void __cdecl thread_render_debug_scripting(int32 thread_index, char* buffer, int32 buffer_size)
{
	hs_thread* thread = hs_thread_get(thread_index);

	ASSERT(buffer);
	ASSERT(buffer_size > 0);

	if ((TEST_BIT(thread->sleep_until + FLAG(31), 31) || thread->sleep_until == HS_SLEEP_COMMAND_SCRIPT_ATOM) && !TEST_BIT(thread->flags, _hs_thread_hide_display_bit))
	{
		csnzappendf(buffer, buffer_size, "\r\n%s\t", hs_thread_format(thread_index));

		if (hs_thread_stack(thread)->expression_index == NONE)
		{
			csnzappendf(buffer, buffer_size, "-\t");
		}
		else
		{
			csnzappendf(buffer, buffer_size, "%i\t", hs_syntax_get(hs_thread_stack(thread)->expression_index)->line_number);
		}

		if (thread->sleep_until == HS_SLEEP_COMMAND_SCRIPT_ATOM)
		{
			csnzappendf(buffer, buffer_size, "ATOM\t");
		}
		else
		{
			csnzappendf(buffer, buffer_size, "%d\t", thread->sleep_until ? (thread->sleep_until - game_time_get()) : 0);
		}

		if (thread->stack.stack_offset && thread->sleep_until != HS_SLEEP_INDEFINITE && hs_thread_stack(thread)->expression_index != NONE)
		{
			const char* function_name = expression_get_function_name(thread_index, hs_thread_stack(thread)->expression_index);
			if (function_name)
			{
				csstrnzcat(buffer, function_name, buffer_size);
			}
		}
	}
}

void __cdecl thread_update_sleep_time_for_reset(int32 thread_index, int32 time_offset)
{
	//INVOKE(0x00599470, thread_update_sleep_time_for_reset, thread_index, time_offset);

	hs_thread* thread = hs_thread_get(thread_index);
	if (thread->sleep_until >= 0)
	{
		thread->sleep_until = MIN(0, thread->sleep_until + time_offset);
	}

	if (TEST_BIT(thread->flags, _hs_thread_latent_sleep_bit) && thread->latent_sleep_until >= 0)
	{
		thread->sleep_until = MIN(0, thread->latent_sleep_until + time_offset);
	}
}

bool __cdecl valid_thread(int32 thread_index)
{
	bool valid = false;

	s_data_array* data_array = hs_thread_is_deterministic(thread_index) ? hs_thread_deterministic_data : hs_thread_non_deterministic_data;
	void* data_array_data = data_array->data;

	hs_thread* thread = hs_thread_get(thread_index);
	if ((void*)thread >= data_array_data && (void*)thread < offset_pointer(data_array_data, data_array->size * data_array->count))
	{
		if (thread->stack.stack_offset >= 0 && thread->stack.stack_offset + (int16)sizeof(hs_stack_frame) <= HS_THREAD_STACK_SIZE)
		{
			if (thread->stack.stack_offset + hs_thread_stack(thread)->size + (int16)sizeof(hs_stack_frame) <= HS_THREAD_STACK_SIZE)
			{
				valid = true;
			}
		}
	}
	return valid;
}

