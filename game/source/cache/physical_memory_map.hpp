#pragma once

#include "cseries/cseries.hpp"

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
	_memory_stage_in_level
};

struct s_physical_memory_stage
{
	dword low_address;
	dword high_address;
	dword next_available_zero_allocation;
	dword __unknownC;
	dword __unknown10;
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
	void* allocation_base_address;
	void* allocation_end_address;
	dword no_mans_land;
	c_static_array<s_physical_memory_stage, _memory_stage_in_level + 1> memory_stages;
};
static_assert(sizeof(s_physical_memory_globals) == 0xB4);

extern s_physical_memory_globals& physical_memory_globals;
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
extern void __cdecl physical_memory_mark_free_memory(c_basic_buffer<void> resize_region_a, class c_basic_buffer<void> resize_region_b);
extern void __cdecl physical_memory_resize_region_dispose();
extern void __cdecl physical_memory_query_bounds(dword a1, dword a2, dword* a3, dword* a4);
extern void __cdecl physical_memory_resize_region_initialize();
extern c_basic_buffer<void> __cdecl physical_memory_resize_region_lock();
extern void __cdecl physical_memory_resize_region_unlock(c_basic_buffer<void> resize_region);
extern dword __cdecl physical_memory_round_up_allocation_size(dword size);
extern void __cdecl physical_memory_stage_pop(memory_stage stage);
extern void __cdecl physical_memory_stage_push(memory_stage stage);
extern void __cdecl physical_memory_system_free(void* address);

