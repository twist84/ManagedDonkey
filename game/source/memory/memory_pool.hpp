#pragma once

#include "cseries/cseries.hpp"

typedef void __cdecl t_memory_pool_callback_list(int32, int32);

enum e_memory_pool_callback_list
{
	k_memory_pool_no_callback = 0,
	k_memory_pool_objects_update_header_callback = 1,
	k_total_memory_pool_callbacks = 2,
};

struct s_memory_pool_block_header
{
	uns32 header_signature;
	const char* file;
	int32 line;
	int32 timestamp;
};
COMPILE_ASSERT(sizeof(s_memory_pool_block_header) == 0x10);

struct s_memory_pool_block
{
	int32 size;
	int32 reference_value;
	int32 next_block_handle;
	int32 previous_block_handle;
};
COMPILE_ASSERT(sizeof(s_memory_pool_block) == 0x10);

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
	e_memory_pool_callback_list reference_tracking_callback_index;
};
COMPILE_ASSERT(sizeof(s_memory_pool) == 0x44);

extern int32 __cdecl memory_pool_allocation_size(int32 size);
extern bool __cdecl memory_pool_block_allocate(s_memory_pool* pool, void** ptr, int32 size, const char* file, int32 line);
extern void __cdecl memory_pool_block_free(s_memory_pool* pool, uns32 payload_handle);
extern void memory_pool_block_free(s_memory_pool* pool, const void** payload_data);
extern s_memory_pool_block* __cdecl memory_pool_block_get(s_memory_pool* pool, int32 payload_handle);
extern int32 __cdecl memory_pool_block_handle_from_payload_handle(int32 payload_handle);
extern bool __cdecl memory_pool_block_reallocate(s_memory_pool* pool, void** ptr, int32 size, const char* file, int32 line);
extern uns32 __cdecl memory_pool_block_sizeof(const s_memory_pool* pool);
extern void __cdecl memory_pool_check_validity(s_memory_pool* pool);
extern uns32 memory_pool_create_handle(const s_memory_pool* memory_pool, const void* data);
extern void __cdecl memory_pool_dispose(s_memory_pool* pool);
extern void* __cdecl memory_pool_get_address(const s_memory_pool* memory_pool, int32 handle);
extern s_memory_pool_block* __cdecl memory_pool_get_block(const s_memory_pool* pool, int32 handle);
extern int32 __cdecl memory_pool_get_block_size(s_memory_pool* pool, const void** payload_data);
extern s_memory_pool* __cdecl memory_pool_new(const char* name, int32 size, e_memory_pool_callback_list tracking_callback_index, c_allocation_base* allocation);
extern void __cdecl memory_pool_toggle_allocation_from_anywhere_in_pool(s_memory_pool* pool, bool can_allocate_from_anywhere);
extern void __cdecl memory_pool_toggle_reference_tracking(s_memory_pool* pool, bool track_references);

