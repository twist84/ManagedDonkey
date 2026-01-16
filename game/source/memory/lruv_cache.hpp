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

class c_lruv_block_long
{
public:
	int32 peek() const;
	void set(int32 value);

	operator int32() const;

protected:
	volatile int32 m_value;
};
COMPILE_ASSERT(sizeof(c_lruv_block_long) == 0x4);

struct s_lruv_cache_block :
	s_datum_header
{
	c_flags<e_lruv_cache_block_bit, uns8, k_lruv_cache_block_bits> flags;
	int32 page_count;
	int32 first_page_index;
	int32 next_block_index;
	int32 previous_block_index;
	c_lruv_block_long last_used_frame_index;
};
COMPILE_ASSERT(sizeof(s_lruv_cache_block) == 0x18);

struct s_lruv_cache_hole
{
	int32 previous_block_index;
	int32 frame_index;
	int32 first_page_index;
	int32 page_count;
};
COMPILE_ASSERT(sizeof(s_lruv_cache_hole) == 0x10);

enum e_lruv_cache_bit
{
	_lruv_cache_disable_lock_bit = 0,

	k_lruv_cache_bits
};

using delete_block_proc_t = void __cdecl(void*, int32, bool);
using locked_block_proc_t = bool __cdecl(void*, int32);
using usage_block_proc_t = uns8 __cdecl(void*, int32);
using move_block_proc_t = void __cdecl(void*, int32, uns32, uns32);

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

	c_enum<e_hole_algorithm, int32, _hole_algorithm_age, k_number_of_hole_algorithms> hole_algorithm;
	int32 maximum_page_count;
	int32 page_size_bits;
	int32 fragmentation_threshold;
	int32 frame_index; // index into `ages`
	int32 first_block_index;
	int32 last_block_index;
	c_static_array<int32, 8> ages;
	c_smart_data_array<s_lruv_cache_block> blocks;
	tag signature; // LRUV_CACHE_SIGNATURE
	c_allocation_base* allocation;
	int32 critical_section_index;
	c_flags<e_lruv_cache_bit, uns32, k_lruv_cache_bits> flags;
};
COMPILE_ASSERT(sizeof(s_lruv_cache) == 0x84);

extern s_lruv_cache* __cdecl lruv_allocate(const char* name, int32 page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index);
extern int32 __cdecl lruv_allocation_size(int32 maximum_count);
extern void __cdecl lruv_block_delete(s_lruv_cache* cache, int32 block_index);
extern void __cdecl lruv_block_delete_internal(s_lruv_cache* cache, int32 block_index, bool a3);
extern uns32 __cdecl lruv_block_get_address(s_lruv_cache* cache, int32 block_index);
extern int32 __cdecl lruv_block_get_age(s_lruv_cache* cache, int32 block_index);
extern uns32 __cdecl lruv_block_get_page_index(s_lruv_cache* cache, int32 block_index);
extern uns32 __cdecl lruv_block_get_size(s_lruv_cache* cache, int32 block_index);
extern void __cdecl lruv_block_initialize(s_lruv_cache* cache, const s_lruv_cache_hole* hole, int32 page_count, int32 block_index);
extern int32 __cdecl lruv_block_new(s_lruv_cache* cache, int32 size_in_bytes, int32 minimum_age);
extern int32 __cdecl lruv_block_new_at_index(s_lruv_cache* cache, int32 block_index, int32 size_in_bytes, int32 minimum_age);
extern int32 __cdecl lruv_block_new_at_index_and_page(s_lruv_cache* cache, int32 block_index, int32 page_index, int32 size_in_bytes);
extern int32 __cdecl lruv_block_new_in_hole(s_lruv_cache* cache, int32 force_datum_index, int32 oldest_unlocked_block_index, s_lruv_cache_hole* hole, int32 page_count);
extern void __cdecl lruv_block_set_age(s_lruv_cache* cache, int32 block_index, int32 age);
extern void __cdecl lruv_block_set_always_locked(s_lruv_cache* cache, int32 block_index, bool always_locked);
extern void __cdecl lruv_block_touch(s_lruv_cache* cache, int32 block_index);
extern bool __cdecl lruv_block_touched(s_lruv_cache* cache, int32 block_index);
extern bool __cdecl lruv_cache_block_is_locked(s_lruv_cache* cache, int32 a2, int32 a3, s_lruv_cache_block* block);
extern uns32 __cdecl lruv_cache_bytes_to_pages(const s_lruv_cache* cache, uns32 size_in_bytes);
extern bool __cdecl lruv_cache_find_hole(s_lruv_cache* cache, int32 a2, int32 a3, s_lruv_cache_hole* hole, int32* a5, bool* a6);
extern void __cdecl lruv_cache_get_page_usage(s_lruv_cache* cache, uns8* page_usage);
extern void __cdecl lruv_cache_purge_hole(s_lruv_cache* cache, const s_lruv_cache_hole* hole, int32 desired_page_count);
extern bool __cdecl lruv_cache_should_use_hole(s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b);
extern uns32 __cdecl lruv_compact(s_lruv_cache* cache);
extern int32 __cdecl lruv_compute_fragmentation_threshold(const s_lruv_cache* cache);
extern void __cdecl lruv_connect(s_lruv_cache* cache, s_data_array* blocks, int32 maximum_page_count);
extern void __cdecl lruv_delete(s_lruv_cache* cache);
extern void __cdecl lruv_flush(s_lruv_cache* cache);
extern uns32 __cdecl lruv_get_address_from_page_index(s_lruv_cache* cache, uns32 page_index);
extern int32 __cdecl lruv_get_age(s_lruv_cache* cache);
extern int32 __cdecl lruv_get_largest_slot_in_pages(s_lruv_cache* cache);
extern int32 __cdecl lruv_get_locked_pages(s_lruv_cache* cache, int32 a2);
extern int32 __cdecl lruv_get_page_count(s_lruv_cache* cache);
extern uns32 __cdecl lruv_get_page_size(s_lruv_cache* cache);
extern uns32 __cdecl lruv_get_used_page_end(s_lruv_cache* cache);
extern uns32 __cdecl lruv_get_used_size(s_lruv_cache* cache);
extern bool __cdecl lruv_has_locked_proc(const s_lruv_cache* cache);
extern void __cdecl lruv_idle(s_lruv_cache* cache);
extern void __cdecl lruv_initialize(s_lruv_cache* cache, const char* name, int32 maximum_page_count, int32 page_size_bits, s_data_array* blocks, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index);
extern void __cdecl lruv_iterator_begin(const s_lruv_cache* cache, s_data_iterator* iterator);
extern void* __cdecl lruv_iterator_next(const s_lruv_cache* cache, s_data_iterator* iterator);
extern s_lruv_cache* __cdecl lruv_new(const char* name, int32 maximum_page_count, int32 page_size_bits, int32 maximum_block_count, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index);
extern void __cdecl lruv_reset_failed_pages(s_lruv_cache* cache);
extern void __cdecl lruv_resize(s_lruv_cache* cache, int32 new_page_count);
extern void __cdecl lruv_resize_non_destructive(s_lruv_cache* cache, int32 new_page_count);
extern void __cdecl lruv_set_function_pointers_new(s_lruv_cache* cache, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, move_block_proc_t* move_block_proc);
extern void __cdecl lruv_set_hole_algorithm(s_lruv_cache* cache, e_hole_algorithm hole_algorithm);
extern void __cdecl lruv_setup(s_lruv_cache* cache, const char* name, int32 page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index);
extern bool __cdecl lruv_verify_slave_data_array(const s_lruv_cache* cache, const s_data_array* data);
extern void __cdecl lruv_wrap_frame_index(s_lruv_cache* cache);
extern bool __cdecl should_use_this_hole_age(const s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b);
extern bool __cdecl should_use_this_hole_blend(const s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b);
extern bool __cdecl should_use_this_hole_fragmentation(const s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b);

