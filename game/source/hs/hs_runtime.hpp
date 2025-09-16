#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "scenario/scenario_trigger_volumes.hpp"

enum : int32
{
	//MAXIMUM_NUMBER_OF_DETERMINISTIC_HS_THREADS = 0x140,
	//MAXIMUM_NUMBER_OF_NON_DETERMINISTIC_HS_THREADS = 0x4,

	//MAXIMUM_NUMBER_OF_HS_THREADS = 0x144,
	//MAXIMUM_NUMBER_OF_HS_GLOBALS = 0xC00,

	HS_THREAD_STACK_SIZE = 0x500
};

enum : int32
{
	HS_SLEEP_FINISHED = -1,
	HS_SLEEP_INDEFINITE = -2,
	HS_SLEEP_COMMAND_SCRIPT_ATOM = -3
};

enum
{
	_hs_thread_in_function_call_bit = 0,
	_hs_thread_latent_sleep_bit,
	_hs_thread_display_expression_bit,
	_hs_thread_hide_display_bit,
	_hs_thread_verbose_bit,
	_hs_thread_woken_bit,
	_hs_thread_terminate_bit,
	_hs_thread_abort_bit,

	NUMBER_OF_HS_THREAD_FLAGS
};

enum
{
	_hs_thread_ai_abort_on_damage_bit = 0,
	_hs_thread_ai_abort_on_alert_bit,
	_hs_thread_ai_abort_on_combat_status_bit,
	_hs_thread_ai_abort_on_vehicle_exit_bit,
	_hs_thread_ai_cleanup_bit,

	k_hs_thread_ai_flags_count,
};

enum
{
	_hs_destination_none = 0,
	_hs_destination_stack,
	_hs_destination_runtime_global,
	_hs_destination_thread_result,

	k_hs_destination_count,
};

enum e_hs_thread_type
{
	_hs_thread_type_script = 0,
	_hs_thread_type_global_initialize,
	_hs_thread_type_runtime_evaluate,
	_hs_thread_type_runtime_internal_evaluate,
	_hs_thread_type_command_script,

	NUMBER_OF_HS_THREAD_TYPES,
};

enum e_hs_thread_tracking_type
{
	_hs_thread_tracking_deterministic = 0,
	_hs_thread_tracking_non_deterministic,

	k_hs_thread_tracking_type_count,
};

struct hs_stack_pointer
{
	int16 stack_offset;
};
static_assert(sizeof(hs_stack_pointer) == 0x2);

struct hs_destination_pointer
{
	int16 destination_type;

	union
	{
		hs_stack_pointer stack_pointer;
		int16 runtime_global_index;
	};
};
static_assert(sizeof(hs_destination_pointer) == 0x4);

struct hs_stack_frame
{
	int32 expression_index;
	int32 script_index;
	hs_destination_pointer child_result;
	hs_stack_pointer parameters;
	hs_stack_pointer parent;
	int16 size;
};
static_assert(sizeof(hs_stack_frame) == 0x14);

struct hs_thread :
	s_datum_header
{
	int16 script_index;
	int16 cleanup_script_index;
	int32 sleep_until;
	int32 latent_sleep_until;
	hs_stack_pointer stack;
	int32 result;
	int32 tracking_index;
	uns8 type;
	uns8 flags;
	uns8 ai_flags;
	int8 ai_data;
	int32 ai_index;
	byte stack_data[HS_THREAD_STACK_SIZE];
};
static_assert(sizeof(hs_thread) == 0x524);

struct s_hs_runtime_globals
{
	bool initialized;
	bool syntax_data_needs_gc;
	bool object_lists_need_gc;
	bool globals_initializing;
	int32 executing_thread_index;
};
static_assert(sizeof(s_hs_runtime_globals) == 0x8);

struct hs_global_runtime :
	s_datum_header
{
	int16 distributed_global_runtime_index;
	int32 value;
};
static_assert(sizeof(hs_global_runtime) == 0x8);

struct hs_distributed_global_data :
	s_datum_header
{
	byte __data[0x2A];
};
static_assert(sizeof(hs_distributed_global_data) == 0x2C);

struct hs_thread_tracking_data :
	s_datum_header
{
	e_hs_thread_tracking_type type;
	int32 index;
};
static_assert(sizeof(hs_thread_tracking_data) == 0xC);

struct s_hs_thread_iterator
{
	int32 raw_thread_index;
	bool iterate_deterministic;
	bool iterate_non_deterministic;
	bool non_deterministic;
};
static_assert(sizeof(s_hs_thread_iterator) == 0x8);

struct hs_debug_data_definition
{
	c_static_flags<MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO> activated_trigger_volumes;
};
static_assert(sizeof(hs_debug_data_definition) == sizeof(c_static_flags<MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO>));

extern bool& debug_scripting;
extern bool& debug_globals;
extern bool& debug_globals_all;
extern bool& hs_verbose;
extern bool debug_global_variables[512];

extern bool g_run_game_scripts;
extern bool breakpoints_enabled;

extern bool debug_trigger_volumes;
extern hs_debug_data_definition hs_debug_data;

extern int32* __cdecl hs_arguments_evaluate(int32 thread_index, int16 parameter_count, const int16* formal_parameters, bool a4);
extern void __cdecl hs_breakpoint(const char* s);
extern bool __cdecl hs_can_cast(int16 actual_type, int16 desired_type);
extern int32 __cdecl hs_cast(int32 thread_index, int16 actual_type, int16 desired_type, int32 value);
extern int32* __cdecl hs_destination(hs_thread* thread, hs_destination_pointer destination_pointer);
extern bool __cdecl hs_evaluate(int32 thread_index, int32 expression_index, hs_destination_pointer destination_pointer, int32* out_cast);
extern int32 __cdecl hs_find_thread_by_name(const char* script_name);
extern int32 __cdecl hs_global_evaluate(int16 global_designator);
extern int32* __cdecl hs_macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize);
extern bool __cdecl hs_object_type_can_cast(int16 actual_type, int16 desired_type);
extern void __cdecl hs_return(int32 thread_index, int32 value);
extern void __cdecl hs_runtime_dispose();
extern void __cdecl hs_runtime_dispose_from_old_map();
extern bool __cdecl hs_runtime_evaluate(int32 expression_index, bool display_expression_result, bool deterministic);
extern const char* __cdecl hs_runtime_get_executing_thread_name();
extern int32 __cdecl hs_runtime_index_from_global_designator(int32 designator);
extern void __cdecl hs_runtime_initialize();
extern void __cdecl hs_runtime_initialize_for_new_map();
extern void __cdecl hs_runtime_initialize_threads();
extern bool __cdecl hs_runtime_initialized();
extern int32 __cdecl hs_runtime_internal_evaluate(int32 expression_index);
extern bool __cdecl hs_runtime_nondeterministic_threads_running();
extern void __cdecl hs_runtime_push_script(int16 script_index);
extern int32 __cdecl hs_runtime_script_begin(int16 script_index, e_hs_script_type script_type, e_hs_thread_type thread_type);
extern void __cdecl hs_runtime_update();
extern void __cdecl hs_script_evaluate(int16 script_index, int32 thread_index, bool initialize);
extern hs_stack_frame* __cdecl hs_stack(hs_thread* thread, hs_stack_pointer stack_pointer);
extern const hs_stack_frame* __cdecl hs_stack(const hs_thread* thread, hs_stack_pointer stack_pointer);
extern void* __cdecl hs_stack_allocate(int32 thread_index, int32 size, int32 alignment_bits, hs_stack_pointer* out_reference);
extern int32* __cdecl hs_stack_destination(hs_thread* thread, hs_stack_pointer stack_pointer);
extern int32* __cdecl hs_stack_parameters(hs_thread* thread, hs_stack_frame* stack_frame, int32 parameter_count);
extern void __cdecl hs_stack_pop(int32 thread_index);
extern bool __cdecl hs_stack_push(int32 thread_index);
extern hs_syntax_node* __cdecl hs_syntax_get(int32 index);
extern bool __cdecl hs_syntax_node_exists(int32 index);
extern int32 hs_thread_allocate(bool deterministic);
extern void __cdecl hs_thread_delete(int32 thread_index, bool validate);
extern const char* __cdecl hs_thread_format(int32 thread_index);
extern bool __cdecl hs_thread_is_deterministic(int32 thread_index);
extern void __cdecl hs_thread_iterator_new(s_hs_thread_iterator* iterator, bool deterministic, bool non_deterministic);
extern int32 __cdecl hs_thread_iterator_next(s_hs_thread_iterator* iterator);
extern void __cdecl hs_thread_main(int32 thread_index);
extern int32 __cdecl hs_thread_new(e_hs_thread_type type, int32 script_index, bool deterministic);
extern hs_stack_frame* __cdecl hs_thread_stack(hs_thread* thread);
extern const hs_stack_frame* __cdecl hs_thread_stack(const hs_thread* thread);
extern bool __cdecl hs_wake_by_name(const char* script_name);
extern void __cdecl inspect_internal(int16 type, int32 value, char* buffer, int16 buffer_size);

extern hs_thread* hs_thread_get(int32 thread_index);
extern void hs_find_dormant_script(const char* dormant_script_name, int32* script_index_out);

extern void render_debug_scripting();
extern void render_debug_scripting_globals();
extern void render_debug_trigger_volumes();

