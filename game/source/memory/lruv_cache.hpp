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

