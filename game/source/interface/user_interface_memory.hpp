#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl user_interface_free(void const* pointer);
extern void* __cdecl user_interface_malloc_tracked(dword size, char const* file, long line);
extern c_allocation_base* __cdecl user_interface_memory_allocation();

