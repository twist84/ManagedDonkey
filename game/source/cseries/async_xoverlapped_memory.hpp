#pragma once

#include "cseries/cseries.hpp"

extern void* __cdecl overlapped_malloc_tracked(dword size, char const* file, long line);
extern void __cdecl overlapped_memory_dispose();
extern void __cdecl overlapped_memory_initialize();

