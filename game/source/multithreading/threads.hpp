#pragma once

#include "cseries/cseries.hpp"

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

struct s_thread_assert_arguments
{
	const char* statement;
	const char* file;
	long line;
	bool is_exception;
};
static_assert(sizeof(s_thread_assert_arguments) == 0x10);

extern void post_thread_assert_arguments(s_thread_assert_arguments* arguments);
extern char const* __cdecl get_registered_thread_name(long thread_index);
extern char const* __cdecl get_thread_name_from_thread_id(long thread_id);
extern bool __cdecl is_async_thread();
extern bool __cdecl is_main_thread();
extern void __cdecl sleep(dword milliseconds);
extern bool __cdecl switch_to_thread();
extern void __cdecl signal_thread_to_crash(e_registered_threads thread_index);
extern void __cdecl signal_thread_to_assert(e_registered_threads thread_index);

