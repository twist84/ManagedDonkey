#pragma once

struct s_thread_assert_arguments
{
    const char* statement;
    const char* file;
    long line;
    bool is_exception;
};
static_assert(sizeof(s_thread_assert_arguments) == 0x10);

extern void post_thread_assert_arguments(s_thread_assert_arguments* arguments);
extern bool __cdecl is_async_thread();
extern bool __cdecl is_main_thread();

