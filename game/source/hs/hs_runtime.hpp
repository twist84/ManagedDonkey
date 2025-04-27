#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "scenario/scenario_trigger_volumes.hpp"

enum e_hs_thread_type
{
	_hs_thread_type_script = 0,
	_hs_thread_type_global_initialize,
	_hs_thread_type_runtime_evaluate,
	_hs_thread_type_command_script,
	_hs_thread_type_performance_script,

	//... are there anymore?

	k_number_of_hs_thread_types
};

struct hs_stack_pointer
{
	short stack_offset;
};
static_assert(sizeof(hs_stack_pointer) == 0x2);

struct hs_destination_pointer
{
	short destination_type;

	union
	{
		hs_stack_pointer stack_pointer;
		short runtime_global_index;
	};
};
static_assert(sizeof(hs_destination_pointer) == 0x4);

struct hs_stack_frame
{
	long expression_index;
	long script_index;
	hs_destination_pointer child_result;
	hs_stack_pointer parameters;
	hs_stack_pointer parent;
	short size;
};
static_assert(sizeof(hs_stack_frame) == 0x14);

enum : long
{
	//MAXIMUM_NUMBER_OF_DETERMINISTIC_HS_THREADS = 0x140,
	//MAXIMUM_NUMBER_OF_NON_DETERMINISTIC_HS_THREADS = 0x4,

	//MAXIMUM_NUMBER_OF_HS_THREADS = 0x144,
	//MAXIMUM_NUMBER_OF_HS_GLOBALS = 0xC00,

	HS_THREAD_STACK_SIZE = 0x500
};

enum : long
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

struct hs_thread :
	s_datum_header
{
	short script_index;
	short cleanup_script_index;
	long sleep_until;
	long latent_sleep_until;
	hs_stack_pointer stack;
	long result;
	long tracking_index;
	uint8 type;
	uint8 flags;
	uint8 ai_flags;
	char ai_data;
	long ai_index;
	uint8 stack_data[HS_THREAD_STACK_SIZE];
};
static_assert(sizeof(hs_thread) == 0x524);

struct s_hs_runtime_globals
{
	bool initialized;
	bool require_gc;
	bool require_object_list_gc;
	bool globals_initializing;
	long executing_thread_index;
};
static_assert(sizeof(s_hs_runtime_globals) == 0x8);

struct hs_global_runtime :
	s_datum_header
{
	short distributed_global_runtime_index;
	long value;
};
static_assert(sizeof(hs_global_runtime) == 0x8);

struct hs_distributed_global_data :
	s_datum_header
{
	uint8 __data[0x2A];
};
static_assert(sizeof(hs_distributed_global_data) == 0x2C);

struct hs_thread_tracking_data :
	s_datum_header
{
	uint16 __unknown2;
	uint32 __unknown4;
	uint32 __unknown8;
};
static_assert(sizeof(hs_thread_tracking_data) == 0xC);

struct s_hs_thread_iterator
{
	long raw_thread_index;
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

extern bool breakpoints_enabled;

extern bool debug_trigger_volumes;
extern hs_debug_data_definition hs_debug_data;

extern long* __cdecl hs_arguments_evaluate(long thread_index, short parameter_count, short const* formal_parameters, bool a4);
extern void __cdecl hs_breakpoint(char const* s);
extern bool __cdecl hs_can_cast(short actual_type, short desired_type);
extern bool __cdecl hs_evaluate(long thread_index, long expression_index, long destination_pointer, long* out_cast);
extern long __cdecl hs_find_thread_by_name(char const* script_name);
extern long* __cdecl hs_macro_function_evaluate(short function_index, long thread_index, bool initialize);
extern bool __cdecl hs_object_type_can_cast(short actual_type, short desired_type);
extern bool __cdecl hs_runtime_evaluate(long expression_index, bool display_expression_result, bool deterministic);
extern char const* __cdecl hs_runtime_get_executing_thread_name();
extern long __cdecl hs_runtime_index_from_global_designator(long designator);
extern bool __cdecl hs_runtime_nondeterministic_threads_running();
extern long __cdecl hs_runtime_script_begin(short script_index, e_hs_script_type script_type, e_hs_thread_type thread_type);
extern void __cdecl hs_runtime_update();
extern char const* __cdecl hs_thread_format(long thread_index);
extern bool __cdecl hs_thread_is_deterministic(long thread_index);
extern void __cdecl hs_thread_iterator_new(s_hs_thread_iterator* iterator, bool deterministic, bool non_deterministic);
extern long __cdecl hs_thread_iterator_next(s_hs_thread_iterator* iterator);
extern void __cdecl hs_thread_main(long thread_index);
extern long __cdecl hs_thread_new(e_hs_thread_type thread_type, long script_index, bool deterministic);
extern hs_stack_frame* __cdecl hs_thread_stack(hs_thread* thread);
extern hs_stack_frame const* __cdecl hs_thread_stack(hs_thread const* thread);
extern bool __cdecl hs_wake_by_name(char const* script_name);
extern void __cdecl inspect_internal(short type, long value, char* buffer, short buffer_size);

extern hs_thread* hs_thread_get(long thread_index);
extern void hs_find_dormant_script(char const* dormant_script_name, long* script_index_out);

extern void render_debug_scripting();
extern void render_debug_scripting_globals();
extern void render_debug_trigger_volumes();

