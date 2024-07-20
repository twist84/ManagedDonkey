#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "memory/data.hpp"
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
	// wtf is this?
	// 0: NULL
	// 1: &thread->stack_data[stack_pointer.stack_offset]
	// 2: &hs_global_data[index].long_value
	// 3: &thread->stack.return_value
	short type;

	union
	{
		short index;
		hs_stack_pointer stack_pointer;
	};
};
static_assert(sizeof(hs_destination_pointer) == 0x4);

struct hs_stack_frame
{
	hs_stack_pointer stack_pointer;
	long return_value;
	long tracking_index;

	c_enum<e_hs_thread_type, char, _hs_thread_type_script, k_number_of_hs_thread_types> type;

	byte_flags __flagsD;
	byte_flags __flagsE;
	byte __unknownF;
	dword __unknown10;
};
static_assert(sizeof(hs_stack_frame) == 0x14);

struct hs_thread :
	s_datum_header
{
	short script_index;
	short previous_script_index;
	long sleep_until;
	long sleep_time;
	hs_stack_frame stack_frame;
	byte stack_data[0x500];
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
	word __unknown2;

	union
	{
		bool bool_value;
		real real_value;
		short short_value;
		long long_value;
		char const* string_value;
		short enum_value;
		byte storage[4];
	};
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
	word __unknown2;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(hs_thread_tracking_data) == 0xC);

struct s_hs_thread_iterator
{
	long thread_index;
	bool __unknown4;
	bool __unknown5;
	bool __unknown6;
	long __unknown8;
};
static_assert(sizeof(s_hs_thread_iterator) == 0xC);

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
extern long* __cdecl hs_macro_function_evaluate(short function_index, long thread_index, bool a3);
extern bool __cdecl hs_object_type_can_cast(short actual_type, short desired_type);
extern bool __cdecl hs_runtime_nondeterministic_threads_running();
extern long __cdecl hs_runtime_script_begin(short script_index, e_hs_script_type script_type, e_hs_thread_type thread_type);
extern void __cdecl hs_runtime_update();
extern char const* __cdecl hs_thread_format(long thread_index);
extern long __cdecl hs_thread_new(e_hs_thread_type thread_type, long script_index, bool deterministic);
extern void __cdecl inspect_internal(short type, long value, char* buffer, short buffer_size);

extern void render_debug_scripting();
extern void render_debug_scripting_globals();
extern void render_debug_trigger_volumes();

