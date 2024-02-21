#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

#define LRUV_CACHE_SIGNATURE 'weee'

enum
{
	k_post_wrap_frame_index = 256,
	k_lruv_max_frame_index = 0x7FFFFFFF
};

enum e_lruv_cache_block_bit
{
	_lruv_cache_block_bit0 = 0,
	_lruv_cache_block_always_locked_bit,

	k_lruv_cache_block_bits
};

struct c_lruv_block_long
{
public:
	long peek() const;
	void set(long value);

	operator long() const;

protected:
	volatile long m_value;
};
static_assert(sizeof(c_lruv_block_long) == 0x4);

struct s_lruv_cache_block :
	s_datum_header
{
	c_flags<e_lruv_cache_block_bit, byte, k_lruv_cache_block_bits> flags;
	long page_count;
	long first_page_index;
	long next_block_index;
	long previous_block_index;
	c_lruv_block_long last_used_frame_index;
};
static_assert(sizeof(s_lruv_cache_block) == 0x18);

struct s_lruv_cache_hole
{
	long previous_block_index;
	long frame_index;
	long first_page_index;
	long page_count;
};
static_assert(sizeof(s_lruv_cache_hole) == 0x10);

enum e_lruv_cache_bit
{
	_lruv_cache_disable_lock_bit = 0,

	k_lruv_cache_bits
};

using delete_block_proc_t = void __cdecl(void*, long, bool);
using locked_block_proc_t = bool __cdecl(void*, long);
using usage_block_proc_t = byte __cdecl(void*, long);
using move_block_proc_t = void __cdecl(void*, long, dword, dword);

enum e_hole_algorithm
{
	_hole_algorithm_age = 0,
	_hole_algorithm_fragmentation,
	_hole_algorithm_blend,

	k_number_of_hole_algorithms
};

struct s_lruv_cache
{
	c_static_string<32> name;

	void* proc_context;
	delete_block_proc_t* delete_block_proc;
	locked_block_proc_t* locked_block_proc;
	usage_block_proc_t* usage_block_proc;
	move_block_proc_t* move_block_proc;

	c_enum<e_hole_algorithm, long, _hole_algorithm_age, k_number_of_hole_algorithms> hole_algorithm;
	long maximum_page_count;
	long page_size_bits;
	long fragmentation_threshold;
	long frame_index; // index into `ages`
	long first_block_index;
	long last_block_index;
	c_static_array<long, 8> ages;
	c_smart_data_array<s_lruv_cache_block> blocks;
	tag signature; // LRUV_CACHE_SIGNATURE
	c_allocation_base* allocation;
	long critical_section_index;
	c_flags<e_lruv_cache_bit, dword, k_lruv_cache_bits> flags;
};
static_assert(sizeof(s_lruv_cache) == 0x84);

extern s_lruv_cache* __cdecl lruv_allocate(char const* name, long page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index);
extern long __cdecl lruv_allocation_size(long maximum_count);
extern void __cdecl lruv_block_delete(s_lruv_cache* cache, long block_index);
extern void __cdecl lruv_block_delete_internal(s_lruv_cache* cache, long block_index, bool a3);
extern dword __cdecl lruv_block_get_address(s_lruv_cache* cache, long block_index);
extern long __cdecl lruv_block_get_age(s_lruv_cache* cache, long block_index);
extern dword __cdecl lruv_block_get_page_index(s_lruv_cache* cache, long block_index);
extern dword __cdecl lruv_block_get_size(s_lruv_cache* cache, long block_index);
extern void __cdecl lruv_block_initialize(s_lruv_cache* cache, s_lruv_cache_hole const* hole, long page_count, long block_index);
extern long __cdecl lruv_block_new(s_lruv_cache* cache, long size_in_bytes, long minimum_age);
extern long __cdecl lruv_block_new_at_index(s_lruv_cache* cache, long block_index, long size_in_bytes, long minimum_age);
extern long __cdecl lruv_block_new_at_index_and_page(s_lruv_cache* cache, long block_index, long page_index, long size_in_bytes);
extern long __cdecl lruv_block_new_in_hole(s_lruv_cache* cache, long force_datum_index, long oldest_unlocked_block_index, s_lruv_cache_hole* hole, long page_count);
extern void __cdecl lruv_block_set_age(s_lruv_cache* cache, long block_index, long age);
extern void __cdecl lruv_block_set_always_locked(s_lruv_cache* cache, long block_index, bool always_locked);
extern void __cdecl lruv_block_touch(s_lruv_cache* cache, long block_index);
extern bool __cdecl lruv_block_touched(s_lruv_cache* cache, long block_index);
extern bool __cdecl lruv_cache_block_is_locked(s_lruv_cache* cache, long a2, long a3, s_lruv_cache_block* block);
extern dword __cdecl lruv_cache_bytes_to_pages(s_lruv_cache const* cache, dword size_in_bytes);
extern bool __cdecl lruv_cache_find_hole(s_lruv_cache* cache, long a2, long a3, s_lruv_cache_hole* hole, long* a5, bool* a6);
extern void __cdecl lruv_cache_get_page_usage(s_lruv_cache* cache, byte* page_usage);
extern void __cdecl lruv_cache_purge_hole(s_lruv_cache* cache, s_lruv_cache_hole const* hole, long desired_page_count);
extern bool __cdecl lruv_cache_should_use_hole(s_lruv_cache* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b);
extern dword __cdecl lruv_compact(s_lruv_cache* cache);
extern long __cdecl lruv_compute_fragmentation_threshold(s_lruv_cache const* cache);
extern void __cdecl lruv_connect(s_lruv_cache* cache, s_data_array* blocks, long maximum_page_count);
extern void __cdecl lruv_delete(s_lruv_cache* cache);
extern void __cdecl lruv_flush(s_lruv_cache* cache);
extern dword __cdecl lruv_get_address_from_page_index(s_lruv_cache* cache, dword page_index);
extern long __cdecl lruv_get_age(s_lruv_cache* cache);
extern long __cdecl lruv_get_largest_slot_in_pages(s_lruv_cache* cache);
extern long __cdecl lruv_get_locked_pages(s_lruv_cache* cache, long a2);
extern long __cdecl lruv_get_page_count(s_lruv_cache* cache);
extern dword __cdecl lruv_get_page_size(s_lruv_cache* cache);
extern dword __cdecl lruv_get_used_page_end(s_lruv_cache* cache);
extern dword __cdecl lruv_get_used_size(s_lruv_cache* cache);
extern bool __cdecl lruv_has_locked_proc(s_lruv_cache const* cache);
extern void __cdecl lruv_idle(s_lruv_cache* cache);
extern void __cdecl lruv_initialize(s_lruv_cache* cache, char const* name, long maximum_page_count, long page_size_bits, s_data_array* blocks, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index);
extern void __cdecl lruv_iterator_begin(s_lruv_cache const* cache, s_data_iterator* iterator);
extern void* __cdecl lruv_iterator_next(s_lruv_cache const* cache, s_data_iterator* iterator);
extern s_lruv_cache* __cdecl lruv_new(char const* name, long maximum_page_count, long page_size_bits, long maximum_block_count, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index);
extern void __cdecl lruv_reset_failed_pages(s_lruv_cache* cache);
extern void __cdecl lruv_resize(s_lruv_cache* cache, long new_page_count);
extern void __cdecl lruv_resize_non_destructive(s_lruv_cache* cache, long new_page_count);
extern void __cdecl lruv_set_function_pointers_new(s_lruv_cache* cache, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, move_block_proc_t* move_block_proc);
extern void __cdecl lruv_set_hole_algorithm(s_lruv_cache* cache, e_hole_algorithm hole_algorithm);
extern void __cdecl lruv_setup(s_lruv_cache* cache, char const* name, long page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index);
extern bool __cdecl lruv_verify_slave_data_array(s_lruv_cache const* cache, s_data_array const* data);
extern void __cdecl lruv_wrap_frame_index(s_lruv_cache* cache);
extern bool __cdecl should_use_this_hole_age(s_lruv_cache const* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b);
extern bool __cdecl should_use_this_hole_blend(s_lruv_cache const* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b);
extern bool __cdecl should_use_this_hole_fragmentation(s_lruv_cache const* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b);

