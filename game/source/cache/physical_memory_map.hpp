#pragma once

#include "cseries/cseries.hpp"

long const k_total_allocated_physical_memory = 0x2F900000;

enum
{
	k_physical_memory_compression_scratch_buffer_size_size = 0x4B000,
	k_physical_memory_runtime_state_size = 0x380000,
	k_physical_memory_tag_cache_size = 0x4B00000,
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

struct s_physical_memory_stage
{
	dword low_address;
	dword high_address;
	char* next_available_zero_allocation;
	dword allocation_count;
	dword fixed_address_allocation_count;
};
static_assert(sizeof(s_physical_memory_stage) == 0x14);

struct c_physical_memory_contiguous_region_listener
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
	long current_stage;
	dword minimum_address;
	dword maximum_address;
	char* no_mans_land;
	c_static_array<s_physical_memory_stage, k_memory_stage_count> memory_stages;
};
static_assert(sizeof(s_physical_memory_globals) == 0xB4);

extern s_physical_memory_globals& physical_memory_globals;
extern dword g_physical_memory_data_size_increase_mb;
extern dword g_physical_memory_cache_size_increase_mb;
extern char const* const k_physical_memory_stage_names[];

extern void* __cdecl _physical_memory_malloc_fixed(memory_stage stage, char const* name, long size, dword flags);
extern dword align_up(dword value, long alignment_bits);
extern void __cdecl physical_memory_adjust_resize_region(c_basic_buffer<void> resize_region_a, c_basic_buffer<void> resize_region_b);
extern bool __cdecl physical_memory_can_allocate_fixed_allocations();
extern void __cdecl physical_memory_create_resizeable_contiguous_region(c_physical_memory_contiguous_region_listener* resize_region_listener);
extern void __cdecl physical_memory_destroy_resizeable_contiguous_region(c_physical_memory_contiguous_region_listener const* resize_region_listener);
extern void __cdecl physical_memory_dispose();
extern void __cdecl physical_memory_free(void* memory);
extern dword __cdecl physical_memory_get_broken_memory_offset();
extern s_physical_memory_stage* __cdecl physical_memory_get_current_stage();
extern c_basic_buffer<void>* __cdecl physical_memory_get_free_memory_region(c_basic_buffer<void>* buffer);
extern long __cdecl physical_memory_get_remaining();
extern void __cdecl physical_memory_initialize();
extern void __cdecl physical_memory_mark_free_memory(c_basic_buffer<void> old_free_region, c_basic_buffer<void> new_free_region);
extern void __cdecl physical_memory_resize_region_dispose();
extern void __cdecl physical_memory_query_bounds(dword physical_memory_base_address, dword physical_memory_query_address, dword* out_physical_memory_start, dword* out_physical_memory_end);
extern void __cdecl physical_memory_resize_region_initialize();
extern c_basic_buffer<void>& __cdecl physical_memory_resize_region_lock();
extern void __cdecl physical_memory_resize_region_unlock(c_basic_buffer<void> resize_region);
extern dword __cdecl physical_memory_round_up_allocation_size(dword size);
extern void __cdecl physical_memory_stage_pop(memory_stage stage);
extern void __cdecl physical_memory_stage_push(memory_stage stage);
extern void __cdecl physical_memory_system_free(void* address);
extern void* __cdecl physical_memory_system_malloc(dword size, void* address);
extern bool __cdecl physical_memory_try_to_resize_contiguous_buffer_simple(c_physical_memory_contiguous_region_listener* region_listener, c_basic_buffer<void> in_region, dword minimum_new_size, c_basic_buffer<void>* out_new_region);
extern bool __cdecl physical_memory_try_to_resize_contiguous_region(c_physical_memory_contiguous_region_listener const* region_listener, void* in_region_buffer, dword in_region_size, dword minimum_new_size, dword requested_size, dword a6, void** out_new_base_address, dword* out_new_region_size);

