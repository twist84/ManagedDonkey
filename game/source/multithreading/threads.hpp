#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

enum e_registered_threads
{
	k_thread_unknown = 0,
	k_thread_main,
	k_thread_render,
	k_thread_async_io,
	k_thread_net_debug,
	k_thread_audio,
	k_thread_profiler,
	k_thread_event_logs,
	k_thread_net_block,
	k_thread_update,

	k_registered_thread_count
};

enum e_thread_priority
{
	_thread_priority_default = 0,
	_thread_priority_idle,
	_thread_priority_lowest,
	_thread_priority_below_normal,
	_thread_priority_normal,
	_thread_priority_above_normal,
	_thread_priority_highest,
	_thread_priority_time_critical,

	k_thread_priority_count
};

enum e_thread_processor
{

};

struct s_thread_assert_arguments
{
	const char* statement;
	const char* file;
	long line;
	bool is_exception;
};
static_assert(sizeof(s_thread_assert_arguments) == 0x10);

struct s_registered_thread_definition
{
	char const* name;
	e_thread_processor thread_processor;
	dword stack_size;
	e_thread_priority priority;
	dword(__cdecl* start_routine)(void* thread_parameter);
	void* thread_parameter;
};
static_assert(sizeof(s_registered_thread_definition) == 0x18);

struct s_thread_system_globals
{
	void* thread_handle[k_registered_thread_count];
	dword thread_id[k_registered_thread_count];
	void* thread_user_data[k_registered_thread_count];

	//c_interlocked_long thread_should_exit[k_registered_thread_count];
	//c_interlocked_long thread_has_crashed[k_registered_thread_count];
	//c_interlocked_long thread_should_assert[k_registered_thread_count];
	//c_interlocked_long thread_should_crash[k_registered_thread_count];

	c_interlocked_long thread_should_exit[k_registered_thread_count];

	// g_thread_system_initialized
	// why is this here?
	c_interlocked_long initialized;

	c_interlocked_long thread_has_crashed[k_registered_thread_count];
};
static_assert(sizeof(s_thread_system_globals) == 0xCC);

extern s_registered_thread_definition(&k_registered_thread_definitions)[k_registered_thread_count];
extern s_thread_system_globals& g_thread_globals;

extern void post_thread_assert_arguments(s_thread_assert_arguments* arguments);
extern char const* __cdecl get_registered_thread_name(long thread_index);
extern char const* __cdecl get_thread_name_from_thread_id(long thread_id);
extern bool __cdecl is_async_thread();
extern bool __cdecl is_main_thread();
extern void __cdecl sleep(dword milliseconds);
extern void __cdecl register_thread(void* handle, long thread_index, dword thread_id, void* user_data);
extern void __cdecl register_thread_running(long thread_index);
extern bool __cdecl switch_to_thread();
extern dword __stdcall thread_execution_wrapper(void* thread_parameter);
extern void __cdecl thread_set_priority(long thread_index, e_thread_priority priority);
extern void __cdecl signal_thread_to_crash(e_registered_threads thread_index);
extern void __cdecl signal_thread_to_assert(e_registered_threads thread_index);

