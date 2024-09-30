#pragma once

#include "cseries/cseries.hpp"

struct c_allocation_base;
struct s_memory_pool
{
	tag signature;
	char name[32];
	c_allocation_base* allocator;
	long size;
	long free_size;
	dword offset_to_data;
	dword first_block_handle;
	dword last_block_handle;
	bool reference_tracking;
	bool allocation_from_anywhere_in_pool;
	bool verification_enabled;
	bool __unknown3F_bool;
	long reference_tracking_index;

	// block?
	byte __data[0x10];
};
static_assert(sizeof(s_memory_pool) == 0x44 + 0x10);

extern dword memory_pool_handle_from_address(s_memory_pool const* memory_pool, void const* pointer);
extern void __cdecl memory_pool_block_free_handle(s_memory_pool* pool, dword payload_handle);
extern void memory_pool_block_free(s_memory_pool* pool, void const** payload_data);
extern long __cdecl memory_pool_get_block_size(s_memory_pool* memory_pool, void const** payload_handle);

