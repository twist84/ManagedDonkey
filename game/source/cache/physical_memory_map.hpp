#pragma once

#include "cseries/cseries.hpp"

int32 const k_total_allocated_physical_memory = 0x2F900000;

enum
{
	k_physical_memory_compression_scratch_buffer_size = 0x4B000,
	k_physical_memory_runtime_state_size = 0x380000,
	k_physical_memory_tag_cache_size = 0x4B00000,
	k_physical_memory_tag_cache_size_new = 0x6400000,
	k_physical_memory_cache_file_debug_globals_size = 0xF1B300,

	k_physical_memory_data_size = 0xAE00000,
	k_physical_memory_cache_size = 0x24B00000,
	k_physical_memory_total_size = k_physical_memory_data_size + k_physical_memory_cache_size,
};

static_assert(k_physical_memory_total_size == k_total_allocated_physical_memory);

enum
{
	k_physical_memory_page_size_bits = 16,
	k_physical_memory_page_mask = MASK(k_physical_memory_page_size_bits)
};

enum memory_stage
{
	_memory_stage_initial = 0,
	_memory_stage_game_initialize,
	_memory_stage_resize_initialize,
	_memory_stage_resize_available,
	_memory_stage_map_configuration,
	_memory_stage_level_initialize,
	_memory_stage_resize_locked,
	_memory_stage_in_level,

	k_memory_stage_count
};

class c_physical_memory_allocation :
	public c_allocation_base
{
	memory_stage m_memory_stage;
};
static_assert(sizeof(c_physical_memory_allocation) == 0x8);
static_assert(sizeof(c_physical_memory_allocation) == sizeof(c_allocation_base) + 0x4);

struct s_physical_memory_stage
{
	uns32 low_address;
	uns32 high_address;
	int8* next_available_zero_allocation;
	uns32 allocation_count;
	uns32 fixed_address_allocation_count;
};
static_assert(sizeof(s_physical_memory_stage) == 0x14);

class c_physical_memory_contiguous_region_listener
{
public:
	virtual void initialize_resize_buffer(c_basic_buffer<void>) = 0;
	virtual void resize_no_fail(c_basic_buffer<void>, c_basic_buffer<void>) = 0;
	virtual void dispose_resize_buffer(c_basic_buffer<void>) = 0;
};
static_assert(sizeof(c_physical_memory_contiguous_region_listener) == sizeof(void*));

struct s_physical_memory_globals
{
	c_physical_memory_contiguous_region_listener* resize_region_listener;
	int32 current_stage;
	uns32 minimum_address;
	uns32 maximum_address;
	int8* no_mans_land;
	c_static_array<s_physical_memory_stage, k_memory_stage_count> memory_stages;
};
static_assert(sizeof(s_physical_memory_globals) == 0xB4);

extern s_physical_memory_globals& physical_memory_globals;
extern uns32 g_physical_memory_data_size_increase_mb;
extern uns32 g_physical_memory_cache_size_increase_mb;
extern const char* const k_physical_memory_stage_names[];

extern void* __cdecl _physical_memory_malloc(memory_stage stage, const char* name, int32 size, uns32 flags);
extern uns32 align_up(uns32 value, int32 alignment_bits);
extern void __cdecl physical_memory_adjust_resize_region(c_basic_buffer<void> resize_region_a, c_basic_buffer<void> resize_region_b);
extern bool __cdecl physical_memory_can_allocate_fixed_allocations();
extern void __cdecl physical_memory_create_resizeable_contiguous_region(c_physical_memory_contiguous_region_listener* resize_region_listener);
extern void __cdecl physical_memory_destroy_resizeable_contiguous_region(const c_physical_memory_contiguous_region_listener* resize_region_listener);
extern void __cdecl physical_memory_dispose();
extern void __cdecl physical_memory_free(void* memory);
extern uns32 __cdecl physical_memory_get_broken_memory_offset();
extern s_physical_memory_stage* __cdecl physical_memory_get_current_stage();
extern c_basic_buffer<void>* __cdecl physical_memory_get_free_memory_region(c_basic_buffer<void>* buffer);
extern int32 __cdecl physical_memory_get_remaining();
extern void __cdecl physical_memory_initialize();
extern void __cdecl physical_memory_mark_free_memory(c_basic_buffer<void> old_free_region, c_basic_buffer<void> new_free_region);
extern void __cdecl physical_memory_resize_region_dispose();
extern void __cdecl physical_memory_query_bounds(uns32 physical_memory_base_address, uns32 physical_memory_query_address, uns32* out_physical_memory_start, uns32* out_physical_memory_end);
extern void __cdecl physical_memory_resize_region_initialize();
extern c_basic_buffer<void>& __cdecl physical_memory_resize_region_lock();
extern void __cdecl physical_memory_resize_region_unlock(c_basic_buffer<void> resize_region);
extern uns32 __cdecl physical_memory_round_up_allocation_size(uns32 size);
extern void __cdecl physical_memory_stage_pop(memory_stage stage);
extern void __cdecl physical_memory_stage_push(memory_stage stage);
extern void __cdecl physical_memory_system_free(void* address);
extern void* __cdecl physical_memory_system_malloc(uns32 size, void* address);
extern bool __cdecl physical_memory_try_to_resize_contiguous_buffer_simple(c_physical_memory_contiguous_region_listener* region_listener, c_basic_buffer<void> in_region, uns32 minimum_new_size, c_basic_buffer<void>* out_new_region);
extern bool __cdecl physical_memory_try_to_resize_contiguous_region(const c_physical_memory_contiguous_region_listener* region_listener, void* in_region_buffer, uns32 in_region_size, uns32 minimum_new_size, uns32 requested_size, uns32 a6, void** out_new_base_address, uns32* out_new_region_size);

