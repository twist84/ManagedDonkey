#pragma once

#include "cseries/cseries.hpp"

struct s_memory_pool_block_header
{
	uns32 header_signature;
	const char* file;
	int32 line;
	int32 timestamp;
};
static_assert(sizeof(s_memory_pool_block_header) == 0x10);

struct s_memory_pool_block
{
	int32 size;
	int32 reference_value;
	int32 next_block_handle;
	int32 previous_block_handle;
};
static_assert(sizeof(s_memory_pool_block) == 0x10);

struct c_allocation_base;
struct s_memory_pool
{
	uns32 signature;
	c_static_string<32> name;
	c_allocation_base* allocation;
	int32 size;
	int32 free_size;
	int32 base_handle;
	int32 first_block_handle;
	int32 last_block_handle;
	bool doesnt_track_references;
	bool can_allocate_from_anywhere_in_the_pool;
	bool verification_disabled;
	bool debug;
	int32 reference_tracking_callback_index;
};
static_assert(sizeof(s_memory_pool) == 0x44);

extern uns32 memory_pool_handle_from_address(const s_memory_pool* memory_pool, const void* pointer);
extern void __cdecl memory_pool_block_free_handle(s_memory_pool* pool, uns32 payload_handle);
extern void memory_pool_block_free(s_memory_pool* pool, const void** payload_data);
extern int32 __cdecl memory_pool_get_block_size(s_memory_pool* memory_pool, const void** payload_handle);

