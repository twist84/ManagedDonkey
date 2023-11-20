#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

#define LRUV_CACHE_SIGNATURE 'weee'

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

struct s_lruv_cache_block : s_datum_header
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
	long __unknown4; // maximum_page_size_bits?
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

// #TODO: map this
enum e_hole_algorithm;

struct s_lruv_cache
{
	c_static_string<32> name;

	void* proc_context;
	delete_block_proc_t* delete_block_proc;
	locked_block_proc_t* locked_block_proc;
	usage_block_proc_t* usage_block_proc;
	move_block_proc_t* move_block_proc;

	long hole_algorithm; // e_hole_algorithm
	long maximum_page_count;
	long page_size_bits;
	long fragmentation_threshold;
	long maximum_page_size_bits; // age?
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
extern long __cdecl lruv_block_new_in_hole(s_lruv_cache* cache, long block_index, long hole_index, s_lruv_cache_hole* hole, long page_count);
extern void __cdecl lruv_block_set_age(s_lruv_cache* cache, long block_index, long age);
extern void __cdecl lruv_block_set_always_locked(s_lruv_cache* cache, long block_index, bool always_locked);
extern void __cdecl lruv_block_touch(s_lruv_cache* cache, long block_index);
extern bool __cdecl lruv_block_touched(s_lruv_cache* cache, long block_index);
extern bool __cdecl lruv_cache_block_is_locked(s_lruv_cache* cache, long a2, long a3, struct s_lruv_cache_block* block);

