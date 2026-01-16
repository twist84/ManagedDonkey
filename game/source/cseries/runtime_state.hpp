#pragma once

#include "cseries/cseries.hpp"

enum
{
	k_runtime_state_size = 0x380000
};

class c_runtime_state_allocation :
	public c_allocation_base
{
};
COMPILE_ASSERT(sizeof(c_runtime_state_allocation) == sizeof(c_allocation_base));

struct s_runtime_state_globals
{
	bool locked;
	void* base_address;
	int32 allocated_size;
	uns32 size_checksum;
};
COMPILE_ASSERT(sizeof(s_runtime_state_globals) == 0x10);

extern c_allocation_base*& g_runtime_state_allocation;
extern c_runtime_state_allocation& g_actual_runtime_state_allocation;
extern s_runtime_state_globals& g_runtime_state_globals;

struct s_data_array;

extern s_data_array* __cdecl runtime_state_data_new(const char* name, int32 maximum_count, int32 size);
extern void __cdecl runtime_state_dispose();
extern void __cdecl runtime_state_dispose_from_old_map();
extern const void* __cdecl runtime_state_get_buffer_address(int32* buffer_size);
extern void __cdecl runtime_state_initialize();
extern void __cdecl runtime_state_initialize_for_new_map();
extern void* __cdecl runtime_state_malloc_aligned(const char* name, const char* type, int32 size, int32 alignment_bits);
extern void* __cdecl runtime_state_malloc(const char* name, const char* type, int32 size);
extern void __cdecl runtime_state_shell_dispose();
extern void __cdecl runtime_state_shell_initialize();
extern void* __cdecl runtime_state_allocate_buffer(int32 size);
extern void __cdecl runtime_state_free_buffer(void* buffer);

