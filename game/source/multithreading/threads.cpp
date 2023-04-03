#include "multithreading/threads.hpp"

#include "cseries/cseries.hpp"

thread_local s_thread_assert_arguments g_thread_assert_arguments;
thread_local bool g_thread_assert_triggered = false;

void post_thread_assert_arguments(s_thread_assert_arguments* arguments)
{
    if (arguments)
        csmemcpy(&g_thread_assert_arguments, arguments, sizeof(s_thread_assert_arguments));

    g_thread_assert_triggered = true;
}

bool __cdecl is_async_thread()
{
	return INVOKE(0x0051C620, is_async_thread);
}

bool __cdecl is_main_thread()
{
	return INVOKE(0x0051C650, is_main_thread);
}

