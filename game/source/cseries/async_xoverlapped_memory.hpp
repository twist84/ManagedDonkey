#pragma once

#include "cseries/cseries.hpp"

extern void* __cdecl overlapped_malloc_tracked(uint32 size, char const* file, int32 line);
extern void __cdecl overlapped_memory_dispose();
extern void __cdecl overlapped_memory_initialize();

