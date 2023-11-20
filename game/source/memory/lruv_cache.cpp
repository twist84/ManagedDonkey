#include "memory/lruv_cache.hpp"

long c_lruv_block_long::peek() const
{
	return m_value;
}

void c_lruv_block_long::set(long value)
{
	m_value = value;
}

c_lruv_block_long::operator long() const
{
	return peek();
}

s_lruv_cache* __cdecl lruv_allocate(char const* name, long page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index)
{
	return INVOKE(0x00966890, lruv_allocate, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//s_lruv_cache* cache = static_cast<s_lruv_cache*>(allocation->allocate(sizeof(s_lruv_cache), name));
	//lruv_setup(cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//return cache;
}

long __cdecl lruv_allocation_size(long maximum_count)
{
	return INVOKE(0x009668D0, lruv_allocation_size, maximum_count);

	//return data_allocation_size(maximum_count, sizeof(s_lruv_cache_block), 0) + sizeof(s_lruv_cache);
}

void __cdecl lruv_block_delete(s_lruv_cache* cache, long block_index)
{
	INVOKE(0x009668F0, lruv_block_delete, cache, block_index);

	//lruv_block_delete_internal(cache, block_index, false);
}

void __cdecl lruv_block_delete_internal(s_lruv_cache* cache, long block_index, bool a3)
{
	INVOKE(0x00966910, lruv_block_delete_internal, cache, block_index, a3);
}

//.text:009669F0

dword __cdecl lruv_block_get_address(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A00, lruv_block_get_address, cache, block_index);

	//return cache->blocks[block_index].first_page_index << cache->page_size_bits;
	//return lruv_get_address_from_page_index(cache, cache->blocks[block_index].first_page_index);
}

long __cdecl lruv_block_get_age(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A20, lruv_block_get_age, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->maximum_page_size_bits - cache->blocks[block_index].last_used_frame_index;
}

dword __cdecl lruv_block_get_page_index(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A40, lruv_block_get_page_index, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].first_page_index;
}

dword __cdecl lruv_block_get_size(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A60, lruv_block_get_size, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].page_count << cache->page_size_bits;
}

void __cdecl lruv_block_initialize(s_lruv_cache* cache, s_lruv_cache_hole const* hole, long page_count, long block_index)
{
	INVOKE(0x00966A80, lruv_block_initialize, cache, hole, page_count, block_index);
}

long __cdecl lruv_block_new(s_lruv_cache* cache, long size_in_bytes, long minimum_age)
{
	return INVOKE(0x00966B60, lruv_block_new, cache, size_in_bytes, minimum_age);

	//return lruv_block_new_at_index(cache, NONE, size_in_bytes, minimum_age);
}

long __cdecl lruv_block_new_at_index(s_lruv_cache* cache, long block_index, long size_in_bytes, long minimum_age)
{
	return INVOKE(0x00966B80, lruv_block_new_at_index, cache, block_index, size_in_bytes, minimum_age);
}

long __cdecl lruv_block_new_at_index_and_page(s_lruv_cache* cache, long block_index, long page_index, long size_in_bytes)
{
	return INVOKE(0x00966C90, lruv_block_new_at_index_and_page, cache, block_index, page_index, size_in_bytes);
}

long __cdecl lruv_block_new_in_hole(s_lruv_cache* cache, long block_index, long hole_index, s_lruv_cache_hole* hole, long page_count)
{
	return INVOKE(0x00966DF0, lruv_block_new_in_hole, cache, block_index, hole_index, hole, page_count);
}

void __cdecl lruv_block_set_age(s_lruv_cache* cache, long block_index, long age)
{
	INVOKE(0x00966E80, lruv_block_set_age, cache, block_index, age);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].last_used_frame_index.set(cache->maximum_page_size_bits - age);
}

void __cdecl lruv_block_set_always_locked(s_lruv_cache* cache, long block_index, bool always_locked)
{
	INVOKE(0x00966EB0, lruv_block_set_always_locked, cache, block_index, always_locked);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].flags.set(_lruv_cache_block_always_locked_bit, always_locked);
}

void __cdecl lruv_block_touch(s_lruv_cache* cache, long block_index)
{
	INVOKE(0x00966EE0, lruv_block_touch, cache, block_index);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].last_used_frame_index.set(cache->maximum_page_size_bits);
}

bool __cdecl lruv_block_touched(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966F00, lruv_block_touched, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].last_used_frame_index == cache->maximum_page_size_bits;
}

bool __cdecl lruv_cache_block_is_locked(s_lruv_cache* cache, long a2, long a3, struct s_lruv_cache_block* block)
{
	return INVOKE(0x00966F30, lruv_cache_block_is_locked, cache, a2, a3, block);
}

//.text:00966F70 ; lruv_cache_bytes_to_pages
//.text:00966F90 ; lruv_cache_find_hole
//.text:00967250 ; lruv_cache_get_page_usage
//.text:00967310 ; lruv_cache_purge_hole
//.text:00967390 ; lruv_cache_should_use_hole
//.text:00967410 ; lruv_compact
//.text:00967470 ; lruv_compute_fragmentation_threshold
//.text:009674A0 ; lruv_connect
//.text:00967510 ; lruv_delete
//.text:00967540 ; lruv_flush
//.text:009675A0 ; lruv_get_address_from_page_index
//.text:009675B0 ; lruv_get_age
//.text:009675C0 ; lruv_get_largest_slot_in_pages
//.text:00967690 ; lruv_get_locked_pages
//.text:00967710 ; lruv_get_page_count
//.text:00967720 ; lruv_get_page_size
//.text:00967740 ; lruv_get_used_page_end
//.text:00967770 ; lruv_get_used_size
//.text:009677A0 ; lruv_has_locked_proc
//.text:009677B0 ; lruv_idle
//.text:00967860 ; lruv_initialize
//.text:009678A0 ; lruv_iterator_begin
//.text:009678C0 ; lruv_iterator_next
//.text:009678D0 ; lruv_new
//.text:00967950 ; lruv_reset_failed_pages
//.text:00967990 ; lruv_resize
//.text:00967A60 ; lruv_resize_non_destructive
//.text:00967AA0 ; lruv_set_function_pointers_new
//.text:00967AC0 ; lruv_set_hole_algorithm
//.text:00967AD0 ; lruv_setup
//.text:00967B60 ; lruv_verify_slave_data_array
//.text:00967C10 ; lruv_wrap_frame_index
//.text:00967D60 ; should_use_this_hole_age
//.text:00967D90 ; should_use_this_hole_blend
//.text:00967DD0 ; should_use_this_hole_fragmentation

