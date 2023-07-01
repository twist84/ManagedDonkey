#pragma once

#include "cseries/cseries.hpp"

struct s_thread_assert_arguments
{
    const char* statement;
    const char* file;
    long line;
    bool is_exception;
};
static_assert(sizeof(s_thread_assert_arguments) == 0x10);

extern void post_thread_assert_arguments(s_thread_assert_arguments* arguments);
extern char const* __cdecl get_thread_name_from_thread_id(dword thread_id);
extern bool __cdecl is_async_thread();
extern bool __cdecl is_main_thread();
extern bool __cdecl switch_to_thread();

