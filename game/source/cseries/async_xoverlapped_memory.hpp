#pragma once

#include "cseries/cseries.hpp"

#include <new> // required for placement new
#define OVERLAPPED_MALLOC(CLASS, ...) new (overlapped_malloc(sizeof(CLASS))) CLASS(__VA_ARGS__)

extern void __cdecl overlapped_free(void const* ptr);
extern void* __cdecl overlapped_malloc(uns32 size);
extern void* __cdecl overlapped_malloc_tracked(uns32 size, const char* file, int32 line);
extern void __cdecl overlapped_memory_dispose();
extern void __cdecl overlapped_memory_initialize();

