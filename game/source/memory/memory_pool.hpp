#pragma once

#include "cseries/cseries.hpp"

struct s_memory_pool_block_header
{
	dword header_signature;
	char const* file;
	long line;
	long timestamp;
};
static_assert(sizeof(s_memory_pool_block_header) == 0x10);

struct s_memory_pool_block
{
	long size;
	long reference_value;
	long next_block_handle;
	long previous_block_handle;
};
static_assert(sizeof(s_memory_pool_block) == 0x10);

struct c_allocation_base;
struct s_memory_pool
{
	dword signature;
	c_static_string<32> name;
	c_allocation_base* allocation;
	long size;
	long free_size;
	long base_handle;
	long first_block_handle;
	long last_block_handle;
	bool doesnt_track_references;
	bool can_allocate_from_anywhere_in_the_pool;
	bool verification_disabled;
	bool debug;
	long reference_tracking_callback_index;
};
static_assert(sizeof(s_memory_pool) == 0x44);

extern dword memory_pool_handle_from_address(s_memory_pool const* memory_pool, void const* pointer);
extern void __cdecl memory_pool_block_free_handle(s_memory_pool* pool, dword payload_handle);
extern void memory_pool_block_free(s_memory_pool* pool, void const** payload_data);
extern long __cdecl memory_pool_get_block_size(s_memory_pool* memory_pool, void const** payload_handle);

