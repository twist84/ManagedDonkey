#pragma once

#include "cseries/cseries.hpp"

struct c_allocation_base
{
public:
	virtual void* allocate(dword size, char const* name);
	virtual void deallocate(void* buffer);
};

extern void __cdecl user_interface_free(void const* pointer);
extern void* __cdecl user_interface_malloc_tracked(dword size, char const* file, long line);