#include "multithreading/threads.hpp"

#include "memory/thread_local.hpp"
#include "multithreading/synchronized_value.hpp"

#include <windows.h>

struct s_registered_thread
{
	char const* name;
	long thread_processor; // e_thread_processor
	dword stack_size;
	dword priority;
	long(__stdcall* start_routine)(dword);
	dword start_routine_data;
};
static_assert(sizeof(s_registered_thread) == 0x18);

struct s_thread_system_globals
{
	HANDLE thread_handle[k_registered_thread_count];
	long thread_id[k_registered_thread_count];
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

REFERENCE_DECLARE_ARRAY(0x01651258, s_registered_thread, g_registered_threads, k_registered_thread_count);
REFERENCE_DECLARE(0x0238EB80, s_thread_system_globals, g_thread_globals);

thread_local s_thread_assert_arguments g_thread_assert_arguments;
thread_local bool g_thread_assert_triggered = false;

void post_thread_assert_arguments(s_thread_assert_arguments* arguments)
{
	if (arguments)
		csmemcpy(&g_thread_assert_arguments, arguments, sizeof(s_thread_assert_arguments));

	g_thread_assert_triggered = true;
}

char const* __cdecl get_thread_name_from_thread_id(long thread_id)
{
	return INVOKE(0x0051C480, get_thread_name_from_thread_id, thread_id);
}

bool __cdecl is_async_thread()
{
	return INVOKE(0x0051C620, is_async_thread);

	//TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
	//return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_async_io;
}

bool __cdecl is_main_thread()
{
	return INVOKE(0x0051C650, is_main_thread);

	//TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
	//return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_main;
}

char const* __cdecl get_registered_thread_name(long thread_index)
{
	return INVOKE(0x0051C440, get_registered_thread_name, thread_index);

	//if (thread_index < k_thread_unknown || thread_index >= k_registered_thread_count)
	//	ASSERT_EXCEPTION("invalid thread index", true);
	//
	//g_registered_threads[thread_index].name;
}

bool __cdecl switch_to_thread()
{
	return INVOKE(0x0051C8A0, switch_to_thread);

	//return SwitchToThread() == TRUE;
}

void __cdecl signal_thread_to_crash(e_registered_threads thread_index)
{
	//if (VALID_INDEX(thread_index, k_registered_thread_count))
	//	g_thread_globals.thread_should_crash[thread_index].set(true);
}

void __cdecl signal_thread_to_assert(e_registered_threads thread_index)
{
	//if (VALID_INDEX(thread_index, k_registered_thread_count))
	//	g_thread_globals.g_thread_globals.thread_should_assert[thread_index].set(true);
}

