#include "multithreading/threads.hpp"

#include "cache/restricted_memory_regions.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/synchronization.hpp"
#include "multithreading/synchronized_value.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/views/render_view.hpp"

#include <windows.h>

REFERENCE_DECLARE_ARRAY(0x01651258, s_thread_definition, k_registered_thread_definitions, k_registered_thread_count);
REFERENCE_DECLARE(0x0238EB80, s_thread_system_globals, g_thread_globals);

c_interlocked_long thread_should_assert[k_registered_thread_count]{};
c_interlocked_long thread_should_crash[k_registered_thread_count]{};

HOOK_DECLARE(0x0051C510, initialize_thread);
//HOOK_DECLARE(0x0051C960, thread_execution_wrapper);

void __cdecl SetThreadName(uns32 thread_id, const char* thread_name)
{
	INVOKE(0x0051C330, SetThreadName, thread_id, thread_name);
}

bool __cdecl current_thread_should_exit()
{
	return INVOKE(0x0051C360, current_thread_should_exit);

	//int32 thread_index = get_current_thread_index();
	//VASSERT(VALID_INDEX(thread_index, k_registered_thread_count), "invalid thread index");
	//return g_thread_globals.thread_should_exit[thread_index].peek() == TRUE;
}

bool __cdecl current_thread_should_assert()
{
	int32 thread_index = get_current_thread_index();
	if (VALID_INDEX(thread_index, k_registered_thread_count))
		return thread_should_assert[thread_index].peek() == TRUE;

	return false;
}

bool __cdecl current_thread_should_crash()
{
	int32 thread_index = get_current_thread_index();
	if (VALID_INDEX(thread_index, k_registered_thread_count))
		return thread_should_crash[thread_index].peek() == TRUE;

	return false;
}

void __cdecl current_thread_update_test_functions()
{
	//INVOKE(0x0051C390, current_thread_update_test_functions);

	ASSERT(!current_thread_should_assert());
	if (current_thread_should_crash())
		*(const char**)NULL = "Forced crash to test the minidump system, this is awesome";
}

void __cdecl destroy_thread_management()
{
	INVOKE(0x0051C3A0, destroy_thread_management);

	//ASSERT(g_thread_globals.initialized.peek() != FALSE, "thread management has not been initialized");
	//ASSERT(g_registered_thread_index == k_thread_main, "Must be called by the main thread");
	//for (int32 thread_index = k_thread_render; thread_index < k_registered_thread_count; thread_index++)
	//{
	//	if (g_thread_globals.thread_id[thread_index] != NONE)
	//		g_thread_globals.thread_should_exit[thread_index].set(TRUE);
	//}
	//sleep(500);
	//for (int32 thread_index = k_thread_render; thread_index < k_registered_thread_count; thread_index++)
	//{
	//	if (g_thread_globals.thread_handle[thread_index] != INVALID_HANDLE_VALUE)
	//	{
	//		//wait_for_thread_to_exit(thread_index, 1000);
	//		CloseHandle(g_thread_globals.thread_handle[thread_index]);
	//		g_thread_globals.thread_handle[thread_index] = INVALID_HANDLE_VALUE;
	//		g_thread_globals.thread_id[thread_index] = NONE;
	//	}
	//}
	//g_thread_globals.initialized.set(FALSE);
}

uns32 __cdecl get_main_thread_id()
{
	//return INVOKE(0x0051C430, get_main_thread_id);

	return g_thread_globals.thread_id[k_thread_main];
}

const char* __cdecl get_registered_thread_name(int32 thread_index)
{
	//return INVOKE(0x0051C440, get_registered_thread_name, thread_index);

	VASSERT(VALID_INDEX(thread_index, k_registered_thread_count), "invalid thread index");
	
	return k_registered_thread_definitions[thread_index].name;
}

e_thread_processor __cdecl get_registered_thread_processor(e_registered_threads thread_index)
{
	//return INVOKE(0x0051C460, get_registered_thread_processor, thread_index);

	return (e_thread_processor)k_registered_thread_definitions[thread_index].processor_index;
}

const char* __cdecl get_thread_name_from_thread_id(int32 thread_id)
{
	return INVOKE(0x0051C480, get_thread_name_from_thread_id, thread_id);

	//for (int32 thread_index = 0; thread_index < k_registered_thread_count; thread_index++)
	//{
	//	if (g_thread_globals.thread_id[thread_index] == thread_id)
	//		return get_registered_thread_name(thread_index);
	//}
	//return "unknown thread";
}

void __cdecl initialize_thread(e_registered_threads thread_index)
{
	//INVOKE(0x0051C510, initialize_thread, thread_index);

	//ASSERT(thread_index> k_thread_main && thread_index<k_registered_thread_count);
	s_thread_definition* definition = &k_registered_thread_definitions[thread_index];
	if (definition->start_routine)
	{
		//ASSERT(g_thread_globals.thread_handle[thread_index] != INVALID_HANDLE_VALUE, "Thread already created");
		uns32 thread_id;
		HANDLE thread_handle = CreateThread(NULL, definition->stack_size, thread_execution_wrapper, (void*)thread_index, 0, &thread_id);
		register_thread(thread_handle, thread_index, thread_id, NULL);
	}
}

void __cdecl initialize_thread_management()
{
	INVOKE(0x0051C580, initialize_thread_management);

	//SetThreadName(GetCurrentThreadId(), "MAIN_THREAD");
	//for (int32 thread_index = k_thread_unknown; thread_index < k_registered_thread_count; thread_index++)
	//{
	//	g_thread_globals.thread_handle[thread_index] = INVALID_HANDLE_VALUE;
	//	g_thread_globals.thread_has_crashed[thread_index].set(FALSE);
	//	g_thread_globals.thread_should_exit[thread_index].set(FALSE);
	//	thread_should_assert[thread_index].set(FALSE);
	//	thread_should_crash[thread_index].set(FALSE);
	//}
	//g_registered_thread_index = k_thread_main;
	//g_thread_globals.thread_handle[k_thread_unknown] = INVALID_HANDLE_VALUE;
	//g_thread_globals.thread_id[k_thread_unknown] = NONE;
	//g_thread_globals.thread_handle[k_thread_main] = GetCurrentThread(); // INVALID_HANDLE_VALUE;
	//g_thread_globals.thread_id[k_thread_main] = GetCurrentThreadId();
	//g_thread_globals.initialized.set(TRUE);
}

bool __cdecl is_async_thread()
{
	return INVOKE(0x0051C620, is_async_thread);

	//return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_async_io;
}

bool __cdecl is_main_thread()
{
	return INVOKE(0x0051C650, is_main_thread);

	//return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_main;
}

bool __cdecl is_render_thread()
{
	return INVOKE(0x0051C680, is_render_thread);

	//return !g_thread_globals.initialized.peek() || g_registered_thread_index == k_thread_render;
}

void __cdecl post_thread_assert_arguments(s_thread_assert_arguments* arguments)
{
	INVOKE(0x0051C6B0, post_thread_assert_arguments, arguments);

	//if (arguments)
	//	csmemcpy(&g_thread_assert_arguments, arguments, sizeof(s_thread_assert_arguments));
	//g_thread_assert_triggered = true;
}

void __cdecl register_thread(void* handle, int32 thread_index, uns32 thread_id, void* user_data)
{
	//INVOKE(0x0051C710, register_thread, handle, thread_index, thread_id, user_data);

	ASSERT(VALID_INDEX(thread_index, k_registered_thread_count));
	//ASSERT(registered_thread_handle_valid(thread_index));
	//ASSERT(thread_handle_valid(handle));
	//ASSERT(thread_id_valid(thread_id));
	g_thread_globals.thread_handle[thread_index] = handle;
	g_thread_globals.thread_id[thread_index] = thread_id;
	g_thread_globals.thread_user_data[thread_index] = user_data;
	SetThreadName(g_thread_globals.thread_id[thread_index], k_registered_thread_definitions[thread_index].name);
	thread_set_priority(thread_index, _thread_priority_default);
}

void __cdecl register_thread_running(int32 thread_index)
{
	INVOKE(0x0051C760, register_thread_running, thread_index);

	//ASSERT(VALID_INDEX(thread_index, k_registered_thread_count));
	//g_registered_thread_index = thread_index;
}

void __cdecl set_thread_exception_arguments(_EXCEPTION_POINTERS* exception_pointers)
{
	INVOKE(0x0051C780, set_thread_exception_arguments, exception_pointers);

	if (exception_pointers && exception_pointers->ExceptionRecord)
	{
		csmemcpy(exception_pointers->ExceptionRecord->ExceptionInformation, &g_thread_assert_arguments, sizeof(s_thread_assert_arguments));
		exception_pointers->ExceptionRecord->NumberParameters = (sizeof(s_thread_assert_arguments) / sizeof(ULONG_PTR));
	}
}

void __cdecl sleep(uns32 milliseconds)
{
	//INVOKE(0x0051C7F0, sleep, milliseconds);

	Sleep(milliseconds);
}

void __cdecl start_thread(e_registered_threads thread_index)
{
	INVOKE(0x0051C800, start_thread, thread_index);

	//g_thread_globals.thread_should_exit[thread_index].set(FALSE);
	//g_thread_globals.thread_has_crashed[thread_index].set(FALSE);
	//initialize_thread(thread_index);
}

void __cdecl stop_thread(e_registered_threads thread_index)
{
	INVOKE(0x0051C830, stop_thread, thread_index);

	//if (registered_thread_handle_valid(thread_index) && wait_for_thread_to_exit(thread_index, INFINITE))
	//{
	//	thread_destroy(g_thread_globals.thread_handle[thread_index]);
	//	unregister_thread(thread_index);
	//}
}

bool __cdecl switch_to_thread()
{
	return INVOKE(0x0051C8A0, switch_to_thread);

	//return SwitchToThread() == TRUE;
}

int __stdcall thread_execution_crash_handler(_EXCEPTION_POINTERS* exception_pointers, int32 thread_index)
{
	return INVOKE(0x0051C8C0, thread_execution_crash_handler, exception_pointers, thread_index);

	//g_thread_globals.thread_has_crashed[thread_index].set(TRUE);
	//thread_release_locks_and_d3d_device();
	//if (g_thread_assert_triggered)
	//{
	//	set_thread_exception_arguments(exception_pointers);
	//	if (exception_pointers->ExceptionRecord)
	//		exception_pointers->ExceptionRecord->ExceptionCode = 'asrt';
	//}
	//cache_exception_information(exception_pointers);
	//
	//if (is_debugger_present() && !g_catch_exceptions)
	//	__debugbreak();
	//else
	//	while (true)
	//		Sleep(1000);
	//
	//return 0;
}

uns32 __stdcall thread_execution_wrapper(void* parameter)
{
	return INVOKE(0x0051C960, thread_execution_wrapper, parameter);

	//__try
	//{
	//	int32 registered_thread_index = *static_cast<int32*>(parameter);
	//	ASSERT(registered_thread_index > k_thread_main && registered_thread_index < k_registered_thread_count);
	//	const s_thread_definition* definition = &k_registered_thread_definitions[registered_thread_index];
	//	ASSERT(definition->start_routine);
	//	register_thread_running(registered_thread_index);
	//	return definition->start_routine(definition->user_parameter);
	//}
	//__except (thread_execution_crash_handler(GetExceptionInformation(), registered_thread_index))
	//{
	//
	//}
}

bool __cdecl thread_has_crashed(e_registered_threads thread_index)
{
	//return INVOKE(0x0051CA10, thread_has_crashed, thread_index);

	return g_thread_globals.thread_has_crashed[thread_index].peek() == TRUE;
}

bool __cdecl thread_is_being_traced(e_registered_threads thread_index)
{
	return INVOKE(0x0051CA30, thread_is_being_traced, thread_index);

	//return trace_dump_is_trace_enabled_for_thread(thread_index);
}

void __cdecl thread_release_locks_and_d3d_device()
{
	INVOKE(0x0051CA40, thread_release_locks_and_d3d_device);

	//if (c_rasterizer::rasterizer_thread_owns_device())
	//{
	//	c_rasterizer::rasterizer_device_release_thread();
	//	c_view::abort_current_view_stack();
	//}
	//restricted_region_handle_crash_of_current_thread();
	//release_all_locks_owned_by_thread();
}

void __cdecl thread_set_priority(int32 thread_index, e_thread_priority priority)
{
	//INVOKE(0x0051CA60, thread_set_priority, thread_index, priority);

	ASSERT(thread_index > k_thread_unknown && thread_index < k_registered_thread_count);
	ASSERT(g_thread_globals.thread_handle[thread_index] != INVALID_HANDLE_VALUE);

	if (priority == _thread_priority_default)
		priority = k_registered_thread_definitions[thread_index].default_priority;

	switch (priority)
	{
	case _thread_priority_idle:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_IDLE);
		break;
	case _thread_priority_lowest:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_LOWEST);
		break;
	case _thread_priority_below_normal:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_BELOW_NORMAL);
		break;
	case _thread_priority_normal:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_NORMAL);
		break;
	case _thread_priority_above_normal:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_ABOVE_NORMAL);
		break;
	case _thread_priority_highest:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_HIGHEST);
		break;
	case _thread_priority_time_critical:
		SetThreadPriority(g_thread_globals.thread_handle[thread_index], THREAD_PRIORITY_TIME_CRITICAL);
		break;
	}
}

bool __cdecl thread_system_initialized()
{
	return INVOKE(0x0051CB30, thread_system_initialized);

	//return g_thread_globals.initialized.peek() != FALSE;
}

void __cdecl unregister_thread(int32 thread_index)
{
	INVOKE(0x0051CB40, unregister_thread, thread_index);

	//ASSERT(VALID_INDEX(thread_index, k_registered_thread_count));
	//g_thread_globals.thread_handle[thread_index] = INVALID_HANDLE_VALUE;
	//g_thread_globals.thread_id[thread_index] = NONE;
	//g_thread_globals.thread_user_data[thread_index] = NULL;
}

bool __cdecl wait_for_thread_to_exit(int32 thread_index, uns32 timeout_in_milliseconds)
{
	return INVOKE(0x0051CB70, wait_for_thread_to_exit, thread_index, timeout_in_milliseconds);

	//ASSERT(g_thread_globals.initialized.peek() != FALSE, "thread management has not been initialized");
	//ASSERT(VALID_INDEX(thread_index, k_registered_thread_count), "invalid thread index");
	//g_thread_globals.thread_should_exit[thread_index].set(TRUE);
	//if (timeout_in_milliseconds == INFINITE)
	//	return wait_for_single_object_internal(g_thread_globals.thread_handle[thread_index], INFINITE);
	//return wait_for_single_object_internal(g_thread_globals.thread_handle[thread_index], timeout_in_milliseconds);
}

void __cdecl signal_thread_to_crash(e_registered_threads thread_index)
{
	if (VALID_INDEX(thread_index, k_registered_thread_count))
		thread_should_crash[thread_index].set(TRUE);
}

void __cdecl signal_thread_to_assert(e_registered_threads thread_index)
{
	if (VALID_INDEX(thread_index, k_registered_thread_count))
		thread_should_assert[thread_index].set(TRUE);
}

//// all your threads are belong to donkey
//s_thread_definition k_registered_thread_definitions[k_registered_thread_count]
//{
//	{
//		.name             = "UNKNOWN_THREAD",
//		.thread_processor = (e_thread_processor)4,
//		.stack_size       = 0,
//		.priority         = _thread_priority_normal,
//		.start_routine    = NULL,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "MAIN_THREAD",
//		.thread_processor = (e_thread_processor)0,
//		.stack_size       = 0,
//		.priority         = _thread_priority_normal,
//		.start_routine    = NULL,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "RENDER",
//		.thread_processor = (e_thread_processor)2,
//		.stack_size       = 0x64000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = render_thread_loop,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "ASYNC_IO",
//		.thread_processor = (e_thread_processor)5,
//		.stack_size       = 0x10000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = async_main,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "NET_DEBUG",
//		.thread_processor = (e_thread_processor)5,
//		.stack_size       = 0x10000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = netdebug_thread_function,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "AUDIO",
//		.thread_processor = (e_thread_processor)4,
//		.stack_size       = 0x10000,
//		.priority         = _thread_priority_time_critical,
//		.start_routine    = audio_thread_loop,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "PROFILER",
//		.thread_processor = (e_thread_processor)5,
//		.stack_size       = 0x10000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = NULL,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "EVENT_LOGS",
//		.thread_processor = (e_thread_processor)4,
//		.stack_size       = 0x10000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = NULL,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "NET_BLOCK",
//		.thread_processor = (e_thread_processor)5,
//		.stack_size       = 0x10000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = NULL,
//		.thread_parameter = NULL
//	},
//	{
//		.name             = "UPDATE",
//		.thread_processor = (e_thread_processor)5,
//		.stack_size       = 0x64000,
//		.priority         = _thread_priority_normal,
//		.start_routine    = update_thread_loop,
//		.thread_parameter = NULL
//	},
//};
