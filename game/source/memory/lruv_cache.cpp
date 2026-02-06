#include "memory/lruv_cache.hpp"

#include "cache/physical_memory_map.hpp"
#include "multithreading/synchronization.hpp"

int32 c_lruv_block_long::peek() const
{
	return m_value;
}

void c_lruv_block_long::set(int32 value)
{
	m_value = value;
}

c_lruv_block_long::operator int32() const
{
	return peek();
}

const s_lruv_cache_block* lruv_cache_block_get(const s_lruv_cache* cache, int32 block_index)
{
	return DATUM_GET(cache->blocks, const s_lruv_cache_block, block_index);
}

s_lruv_cache_block* lruv_cache_block_get_mutable(s_lruv_cache* cache, int32 block_index)
{
	return DATUM_GET(cache->blocks, s_lruv_cache_block, block_index);
}

void lruv_cache_verify(s_lruv_cache* cache, bool a2)
{
	//ASSERT(cache);
	//ASSERT(cache->signature == LRUV_CACHE_SIGNATURE);
	//
	//data_verify(cache->blocks);
	//
	//if (a2 && !cache->flags.test(_lruv_cache_disable_lock_bit))
	//{
	//	const s_lruv_cache_block* block;
	//	for (int32 block_index = cache->first_block_index; block_index != NONE; block_index = block->next_block_index)
	//	{
	//		block = lruv_cache_block_get(cache, block_index);
	//		if (block->previous_block_index == NONE)
	//		{
	//			ASSERT(cache->first_block_index == block_index);
	//		}
	//		else
	//		{
	//			const s_lruv_cache_block* previous_block = lruv_cache_block_get(cache, block->previous_block_index);
	//			ASSERT(previous_block->next_block_index == block_index);
	//			ASSERT(previous_block->first_page_index < block->first_page_index);
	//			ASSERT(previous_block->first_page_index + previous_block->page_count <= block->first_page_index);
	//		}
	//
	//		if (block->next_block_index == NONE)
	//		{
	//			ASSERT(cache->last_block_index == block_index);
	//		}
	//		else
	//		{
	//			const s_lruv_cache_block* next_block = lruv_cache_block_get(cache, block->next_block_index);
	//			ASSERT(next_block->previous_block_index == block_index);
	//			ASSERT(next_block->first_page_index > block->first_page_index);
	//			ASSERT(block->first_page_index + block->page_count <= next_block->first_page_index);
	//		}
	//	}
	//}
}

s_lruv_cache* __cdecl lruv_allocate(const char* name, int32 page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index)
{
	return INVOKE(0x00966890, lruv_allocate, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//s_lruv_cache* cache = static_cast<s_lruv_cache*>(allocation->allocate(sizeof(s_lruv_cache), name));
	//lruv_setup(cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//return cache;
}

int32 __cdecl lruv_allocation_size(int32 maximum_count)
{
	return INVOKE(0x009668D0, lruv_allocation_size, maximum_count);

	//return data_allocation_size(maximum_count, sizeof(s_lruv_cache_block), 0) + sizeof(s_lruv_cache);
}

void __cdecl lruv_block_delete(s_lruv_cache* cache, int32 block_index)
{
	INVOKE(0x009668F0, lruv_block_delete, cache, block_index);

	//lruv_block_delete_internal(cache, block_index, false);
}

void __cdecl lruv_block_detach_and_delete(s_lruv_cache* cache, int32 block_index)
{
	const s_lruv_cache_block* block = lruv_cache_block_get(cache, block_index);

	if (block->previous_block_index != NONE)
	{
		s_lruv_cache_block* previous_block = lruv_cache_block_get_mutable(cache, block->previous_block_index);
		previous_block->next_block_index = block->next_block_index;
	}
	else
	{
		ASSERT(cache->first_block_index == block_index);
		cache->first_block_index = block->next_block_index;
	}

	if (block->next_block_index != NONE)
	{
		s_lruv_cache_block* next_block = lruv_cache_block_get_mutable(cache, block->next_block_index);
		next_block->previous_block_index = block->previous_block_index;
	}
	else
	{
		ASSERT(cache->last_block_index == block_index);
		cache->last_block_index = block->previous_block_index;
	}

	datum_delete(cache->blocks, block_index);
	lruv_cache_verify(cache, true);
}

void __cdecl lruv_block_delete_internal(s_lruv_cache* cache, int32 block_index, bool a3)
{
	INVOKE(0x00966910, lruv_block_delete_internal, cache, block_index, a3);

	//c_critical_section_scope critical_section(cache->critical_section_index);
	//lruv_cache_verify(cache, true);
	//
	//if (cache->delete_block_proc)
	//	cache->delete_block_proc(cache->proc_context, block_index, a3);
	//
	//lruv_block_detach_and_delete(cache, block_index);
}

//.text:009669F0

uns32 __cdecl lruv_block_get_address(s_lruv_cache* cache, int32 block_index)
{
	return INVOKE(0x00966A00, lruv_block_get_address, cache, block_index);

	//return cache->blocks[block_index].first_page_index << cache->page_size_bits;
	//return lruv_get_address_from_page_index(cache, cache->blocks[block_index].first_page_index);
}

int32 __cdecl lruv_block_get_age(s_lruv_cache* cache, int32 block_index)
{
	return INVOKE(0x00966A20, lruv_block_get_age, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->frame_index - cache->blocks[block_index].last_used_frame_index;
}

uns32 __cdecl lruv_block_get_page_index(s_lruv_cache* cache, int32 block_index)
{
	return INVOKE(0x00966A40, lruv_block_get_page_index, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].first_page_index;
}

uns32 __cdecl lruv_block_get_size(s_lruv_cache* cache, int32 block_index)
{
	return INVOKE(0x00966A60, lruv_block_get_size, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].page_count << cache->page_size_bits;
}

void __cdecl lruv_block_initialize(s_lruv_cache* cache, const s_lruv_cache_hole* hole, int32 page_count, int32 block_index)
{
	INVOKE(0x00966A80, lruv_block_initialize, cache, hole, page_count, block_index);

	//if (block_index != NONE)
	//{
	//	c_critical_section_scope critical_section(cache->critical_section_index);
	//
	//	s_lruv_cache_block* block = lruv_cache_block_get_mutable(cache, block_index);
	//	if (hole->previous_block_index == NONE)
	//	{
	//		if (cache->first_block_index == NONE)
	//		{
	//			block->previous_block_index = NONE;
	//			cache->last_block_index = block_index;
	//		}
	//		else
	//		{
	//			s_lruv_cache_block* next_block = lruv_cache_block_get_mutable(cache, cache->first_block_index);
	//			ASSERT(next_block->previous_block_index == NONE);
	//			ASSERT(next_block->first_page_index >= hole->first_page_index);
	//			ASSERT(next_block->first_page_index >= hole->first_page_index + hole->page_count);
	//
	//			block->previous_block_index = NONE;
	//			next_block->previous_block_index = block_index;
	//		}
	//	}
	//	else
	//	{
	//		s_lruv_cache_block* previous_block = lruv_cache_block_get_mutable(cache, hole->previous_block_index);
	//		ASSERT(previous_block->first_page_index < hole->first_page_index);
	//		ASSERT(previous_block->first_page_index + previous_block->page_count <= hole->first_page_index);
	//
	//		if (previous_block->next_block_index == NONE)
	//		{
	//			block->previous_block_index = cache->last_block_index;
	//			cache->last_block_index = block_index;
	//		}
	//		else
	//		{
	//			s_lruv_cache_block* next_block = lruv_cache_block_get_mutable(cache, cache->first_block_index);
	//			ASSERT(next_block->first_page_index >= hole->first_page_index);
	//			ASSERT(next_block->first_page_index >= hole->first_page_index + hole->page_count);
	//
	//			block->previous_block_index = next_block->previous_block_index;
	//			next_block->previous_block_index = block_index;
	//		}
	//	}
	//
	//	if (hole->previous_block_index == NONE)
	//	{
	//		block->next_block_index = cache->first_block_index;
	//		cache->first_block_index = block_index;
	//	}
	//	else
	//	{
	//		s_lruv_cache_block* previous_block = lruv_cache_block_get_mutable(cache, hole->previous_block_index);
	//		block->next_block_index = previous_block->next_block_index;
	//		previous_block->next_block_index = block_index;
	//	}
	//
	//	block->first_page_index = hole->first_page_index;
	//	block->page_count = page_count;
	//	block->last_used_frame_index.set(cache->frame_index);
	//	lruv_cache_verify(cache, true);
	//}
}

int32 __cdecl lruv_block_new(s_lruv_cache* cache, int32 size_in_bytes, int32 minimum_age)
{
	return INVOKE(0x00966B60, lruv_block_new, cache, size_in_bytes, minimum_age);

	//return lruv_block_new_at_index(cache, NONE, size_in_bytes, minimum_age);
}

int32 __cdecl lruv_block_new_at_index(s_lruv_cache* cache, int32 block_index, int32 size_in_bytes, int32 minimum_age)
{
	return INVOKE(0x00966B80, lruv_block_new_at_index, cache, block_index, size_in_bytes, minimum_age);
}

int32 __cdecl lruv_block_new_at_index_and_page(s_lruv_cache* cache, int32 block_index, int32 page_index, int32 size_in_bytes)
{
	return INVOKE(0x00966C90, lruv_block_new_at_index_and_page, cache, block_index, page_index, size_in_bytes);
}

int32 __cdecl lruv_block_new_in_hole(s_lruv_cache* cache, int32 force_datum_index, int32 oldest_unlocked_block_index, s_lruv_cache_hole* hole, int32 page_count)
{
	return INVOKE(0x00966DF0, lruv_block_new_in_hole, cache, force_datum_index, oldest_unlocked_block_index, hole, page_count);

	//lruv_cache_purge_hole(cache, hole, page_count);
	//if (cache->blocks->actual_count == cache->blocks->maximum_count)
	//{
	//	ASSERT(force_datum_index == NONE);
	//
	//	if (oldest_unlocked_block_index != NONE)
	//	{
	//		if (hole->previous_block_index == oldest_unlocked_block_index)
	//		{
	//			hole->previous_block_index = cache->blocks[oldest_unlocked_block_index].previous_block_index;
	//		}
	//	}
	//}
	//
	//int32 block_index = NONE;
	//if (force_datum_index != NONE)
	//{
	//	block_index = datum_new_at_index(cache->blocks, force_datum_index);
	//}
	//else
	//{
	//	block_index = datum_new(cache->blocks);
	//}
	//lruv_block_initialize(cache, hole, page_count, block_index);
}

void __cdecl lruv_block_set_age(s_lruv_cache* cache, int32 block_index, int32 age)
{
	INVOKE(0x00966E80, lruv_block_set_age, cache, block_index, age);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].last_used_frame_index.set(cache->frame_index - age);
}

void __cdecl lruv_block_set_always_locked(s_lruv_cache* cache, int32 block_index, bool always_locked)
{
	INVOKE(0x00966EB0, lruv_block_set_always_locked, cache, block_index, always_locked);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].flags.set(_lruv_cache_block_always_locked_bit, always_locked);
}

void __cdecl lruv_block_touch(s_lruv_cache* cache, int32 block_index)
{
	INVOKE(0x00966EE0, lruv_block_touch, cache, block_index);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].last_used_frame_index.set(cache->frame_index);
}

bool __cdecl lruv_block_touched(s_lruv_cache* cache, int32 block_index)
{
	return INVOKE(0x00966F00, lruv_block_touched, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->frame_index == cache->blocks[block_index].last_used_frame_index;
}

bool __cdecl lruv_cache_block_is_locked(s_lruv_cache* cache, int32 a2, int32 a3, s_lruv_cache_block* block)
{
	return INVOKE(0x00966F30, lruv_cache_block_is_locked, cache, a2, a3, block);

	//if (block->flags.test(_lruv_cache_block_always_locked_bit))
	//	return true;
	//
	//if (cache->frame_index >= (block->last_used_frame_index + a2))
	//	return true;
	//
	//return cache->locked_block_proc && cache->locked_block_proc(cache->proc_context, a3);
}

uns32 __cdecl lruv_cache_bytes_to_pages(const s_lruv_cache* cache, uns32 size_in_bytes)
{
	return INVOKE(0x00966F70, lruv_cache_bytes_to_pages, cache, size_in_bytes);

	//uns32 page_count = size_in_bytes >> cache->page_size_bits;
	//if (TEST_MASK(size_in_bytes, MASK(cache->page_size_bits)))
	//	page_count++;
	//
	//ASSERT(page_count == (align_up(size_in_bytes, cache->page_size_bits) >> cache->page_size_bits));
	//return page_count;
}

bool __cdecl lruv_cache_find_hole(s_lruv_cache* cache, int32 a2, int32 a3, s_lruv_cache_hole* hole, int32* a5, bool* a6)
{
	return INVOKE(0x00966F90, lruv_cache_find_hole, cache, a2, a3, hole, a5, a6);
}

void __cdecl lruv_cache_get_page_usage(s_lruv_cache* cache, uns8* page_usage)
{
	INVOKE(0x00967250, lruv_cache_get_page_usage, cache, page_usage);
}

void __cdecl lruv_cache_purge_hole(s_lruv_cache* cache, const s_lruv_cache_hole* hole, int32 desired_page_count)
{
	INVOKE(0x00967310, lruv_cache_purge_hole, cache, hole, desired_page_count);

	//ASSERT(desired_page_count >= 0 && hole->first_page_index >= 0);
	//
	//int32 block_index = cache->first_block_index;
	//if (hole->previous_block_index != NONE)
	//	block_index = cache->blocks[hole->previous_block_index].next_block_index;
	//
	//int32 next_block_index = block_index;
	//while (next_block_index != NONE)
	//{
	//	int32 block_index_to_delete = next_block_index;
	//
	//	s_lruv_cache_block* next_block = lruv_cache_block_get_mutable(cache, next_block_index);
	//	next_block_index = next_block->next_block_index;
	//
	//	if (next_block->first_page_index >= (hole->first_page_index + desired_page_count))
	//		break;
	//
	//	if ((next_block->first_page_index + next_block->page_count) <= hole->first_page_index)
	//		break;
	//
	//	lruv_block_delete_internal(cache, block_index_to_delete, true);
	//}
}

bool __cdecl lruv_cache_should_use_hole(s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b)
{
	return INVOKE(0x00967390, lruv_cache_should_use_hole, cache, desired_page_count, hole_a, hole_b);

	//e_hole_algorithm hole_algorithm = cache->hole_algorithm;
	//switch (hole_algorithm)
	//{
	//case _hole_algorithm_age:
	//	should_use_this_hole_age(cache, desired_page_count, hole_a, hole_b);
	//	break;
	//case _hole_algorithm_fragmentation:
	//	should_use_this_hole_fragmentation(cache, desired_page_count, hole_a, hole_b);
	//	break;
	//case _hole_algorithm_blend:
	//	should_use_this_hole_blend(cache, desired_page_count, hole_a, hole_b);
	//	break;
	//default:
	//	UNREACHABLE();
	//	break;
	//}
}

uns32 __cdecl lruv_compact(s_lruv_cache* cache)
{
	return INVOKE(0x00967410, lruv_compact, cache);

	//lruv_cache_verify(cache, true);
	//
	//uns32 next_available_page_index = 0;
	//for (int32 block_index = cache->first_block_index; block_index != NONE; block_index = lruv_cache_block_get(cache, block_index)->next_block_index)
	//{
	//	s_lruv_cache_block* block = lruv_cache_block_get_mutable(cache, block_index);
	//	ASSERT(block->first_page_index >= next_available_page_index);
	//	if (block->first_page_index > next_available_page_index)
	//	{
	//		if (cache->move_block_proc)
	//			cache->move_block_proc(cache->proc_context, block_index, block->first_page_index, next_available_page_index);
	//
	//		block->first_page_index = next_available_page_index;
	//	}
	//
	//	ASSERT(block->first_page_index == next_available_page_index);
	//
	//	next_available_page_index += block->page_count;
	//}
	//
	//return next_available_page_index;
}

int32 __cdecl lruv_compute_fragmentation_threshold(const s_lruv_cache* cache)
{
	return INVOKE(0x00967470, lruv_compute_fragmentation_threshold, cache);

	//if (cache->maximum_page_count && cache->blocks)
	//{
	//	int32 maximum_blocks = cache->blocks->maximum_count;
	//	ASSERT(maximum_blocks > 0);
	//
	//	return 8 * (cache->maximum_page_count / cache->blocks->maximum_count);
	//}
	//
	//return NONE;
}

void __cdecl lruv_connect(s_lruv_cache* cache, s_data_array* blocks, int32 maximum_page_count)
{
	INVOKE(0x009674A0, lruv_connect, cache, blocks, maximum_page_count);

	//ASSERT(cache);
	//ASSERT(blocks);
	//ASSERT(cache->blocks == nullptr);
	//
	//cache->maximum_page_count = maximum_page_count;
	//cache->blocks = blocks;
	//cache->fragmentation_threshold = lruv_compute_fragmentation_threshold(cache);
	//lruv_reset_failed_pages(cache);
}

void __cdecl lruv_delete(s_lruv_cache* cache)
{
	INVOKE(0x00967510, lruv_delete, cache);

	//data_dispose(cache->blocks);
	//cache->allocation->deallocate(cache);
}

void __cdecl lruv_flush(s_lruv_cache* cache)
{
	INVOKE(0x00967540, lruv_flush, cache);

	//lruv_cache_verify(cache, true);
	//c_data_iterator<s_lruv_cache_block const> block_iterator;
	//block_iterator.begin(cache->blocks);
	//while (block_iterator.next())
	//{
	//	lruv_block_delete(cache, block_iterator.get_index());
	//}
}

uns32 __cdecl lruv_get_address_from_page_index(s_lruv_cache* cache, uns32 page_index)
{
	return INVOKE(0x009675A0, lruv_get_address_from_page_index, cache, page_index);

	//return page_index << cache->page_size_bits;
}

int32 __cdecl lruv_get_age(s_lruv_cache* cache)
{
	return INVOKE(0x009675B0, lruv_get_age, cache);

	//return cache->frame_index;
}

int32 __cdecl lruv_get_largest_slot_in_pages(s_lruv_cache* cache)
{
	return INVOKE(0x009675C0, lruv_get_largest_slot_in_pages, cache);

	//int32 largest_slot_in_pages = 0;
	//
	//c_data_iterator<s_lruv_cache_block const> block_iterator;
	//block_iterator.begin(cache->blocks);
	//if (block_iterator.next())
	//{
	//	while (block_iterator.next())
	//	{
	//		if (const s_lruv_cache_block* block = block_iterator.get_datum())
	//		{
	//			int32 slot_in_pages = block->first_page_index - (block->first_page_index + block->page_count);
	//			if (largest_slot_in_pages <= slot_in_pages)
	//				largest_slot_in_pages = slot_in_pages;
	//		}
	//
	//	}
	//}
	//
	//if (cache->first_block_index != NONE)
	//{
	//	const s_lruv_cache_block* first_block = lruv_cache_block_get(cache, cache->first_block_index);
	//
	//	if (largest_slot_in_pages <= first_block->first_page_index)
	//		largest_slot_in_pages = first_block->first_page_index;
	//
	//	if (cache->last_block_index != NONE)
	//	{
	//		const s_lruv_cache_block* last_block = lruv_cache_block_get(cache, cache->last_block_index);
	//
	//		int32 slot_in_pages = cache->maximum_page_count - last_block->first_page_index - last_block->page_count;
	//		if (largest_slot_in_pages <= slot_in_pages)
	//			largest_slot_in_pages = slot_in_pages;
	//	}
	//}
	//else
	//{
	//	largest_slot_in_pages = cache->maximum_page_count;
	//}
	//
	//return largest_slot_in_pages;
}

int32 __cdecl lruv_get_locked_pages(s_lruv_cache* cache, int32 a2)
{
	return INVOKE(0x00967690, lruv_get_locked_pages, cache, a2);

	//int32 locked_pages = 0;
	//
	//c_data_iterator<s_lruv_cache_block const> block_iterator;
	//block_iterator.begin(cache->blocks);
	//while (block_iterator.next())
	//{
	//	const s_lruv_cache_block* block = block_iterator.get_datum();
	//	if (!block->flags.test(_lruv_cache_block_always_locked_bit) && (a2 + block->last_used_frame_index) < cache->frame_index)
	//	{
	//		if (!cache->locked_block_proc || !cache->locked_block_proc(cache->proc_context, block_iterator.get_index()))
	//			continue;
	//	}
	//
	//	locked_pages += block->page_count;
	//}
	//
	//return locked_pages;
}

int32 __cdecl lruv_get_page_count(s_lruv_cache* cache)
{
	return INVOKE(0x00967710, lruv_get_page_count, cache);

	//return cache->maximum_page_count;
}

uns32 __cdecl lruv_get_page_size(s_lruv_cache* cache)
{
	return INVOKE(0x00967720, lruv_get_page_size, cache);

	//return 1 << cache->page_size_bits;
}

uns32 __cdecl lruv_get_used_page_end(s_lruv_cache* cache)
{
	return INVOKE(0x00967740, lruv_get_used_page_end, cache);

	//lruv_cache_verify(cache, true);
	//if (cache->first_block_index != NONE)
	//{
	//	ASSERT(cache->last_block_index != NONE);
	//
	//	const s_lruv_cache_block* block = lruv_cache_block_get(cache, cache->last_block_index);
	//	return block->first_page_index + block->page_count;
	//}
	//
	//return 0;
}

uns32 __cdecl lruv_get_used_size(s_lruv_cache* cache)
{
	return INVOKE(0x00967770, lruv_get_used_size, cache);

	//return lruv_get_used_page_end(cache) << cache->page_size_bits;
}

bool __cdecl lruv_has_locked_proc(const s_lruv_cache* cache)
{
	return INVOKE(0x009677A0, lruv_has_locked_proc, cache);

	//ASSERT(cache);
	//
	//return cache->locked_block_proc != nullptr;
}

void __cdecl lruv_idle(s_lruv_cache* cache)
{
	INVOKE(0x009677B0, lruv_idle, cache);

	//c_critical_section_scope critical_section(cache->critical_section_index);
	//lruv_cache_verify(cache, false);
	//
	//if (cache->frame_index == k_lruv_max_frame_index)
	//	lruv_wrap_frame_index(cache);
	//else
	//	cache->frame_index++;
	//
	//lruv_reset_failed_pages(cache);
}

void __cdecl lruv_initialize(s_lruv_cache* cache, const char* name, int32 maximum_page_count, int32 page_size_bits, s_data_array* blocks, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index)
{
	INVOKE(0x00967860, lruv_initialize, cache, name, maximum_page_count, page_size_bits, blocks, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//lruv_setup(cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//lruv_connect(cache, blocks, maximum_page_count);
}

void __cdecl lruv_iterator_begin(const s_lruv_cache* cache, s_data_iterator* iterator)
{
	INVOKE(0x009678A0, lruv_iterator_begin, cache, iterator);

	//data_iterator_begin(iterator, cache->blocks);
}

void* __cdecl lruv_iterator_next(const s_lruv_cache* cache, s_data_iterator* iterator)
{
	return INVOKE(0x009678C0, lruv_iterator_next, cache, iterator);

	//return data_iterator_next(iterator);
}

s_lruv_cache* __cdecl lruv_new(const char* name, int32 maximum_page_count, int32 page_size_bits, int32 maximum_block_count, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index)
{
	return INVOKE(0x009678D0, lruv_new, name, maximum_page_count, page_size_bits, maximum_block_count, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//s_lruv_cache* cache = static_cast<s_lruv_cache*>(allocation->allocate(lruv_allocation_size(maximum_block_count), name));
	//s_data_array* data = (s_data_array*)cache + 1;
	//s_data_array* data = static_cast<s_data_array*>(align_pointer(cache + 1, 0));
	//
	//ASSERT(name);
	//ASSERT(page_size_bits >= 0 && page_size_bits < SHORT_BITS);
	//ASSERT(maximum_block_count > 0);
	//
	//data_initialize(data, name, maximum_block_count, sizeof(s_lruv_cache_block), 0, g_no_allocation);
	//data_make_valid(data);
	//
	//lruv_initialize(cache, name, maximum_page_count, page_size_bits, data, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//
	//return cache;
}

void __cdecl lruv_reset_failed_pages(s_lruv_cache* cache)
{
	INVOKE(0x00967950, lruv_reset_failed_pages, cache);

	//ASSERT(cache);
	//for (int32 i = 0; i < cache->ages.get_count(); i++)
	//	cache->ages[i] = k_lruv_max_frame_index;
}

void __cdecl lruv_resize(s_lruv_cache* cache, int32 new_page_count)
{
	INVOKE(0x00967990, lruv_resize, cache, new_page_count);

	//ASSERT(new_page_count >= 0);
	//
	//c_critical_section_scope critical_section(cache->critical_section_index);
	//lruv_cache_verify(cache, true);
	//
	//c_data_iterator<s_lruv_cache_block> block_iterator;
	//block_iterator.begin(cache->blocks);
	//while (block_iterator.next())
	//{
	//	s_lruv_cache_block* block = block_iterator.get_datum();
	//	if (block->first_page_index + block->page_count > new_page_count)
	//	{
	//		lruv_block_delete(cache, block_iterator.get_index());
	//	}
	//}
	//
	//cache->maximum_page_count = new_page_count;
	//cache->fragmentation_threshold = lruv_compute_fragmentation_threshold(cache);
}

void __cdecl lruv_resize_non_destructive(s_lruv_cache* cache, int32 new_page_count)
{
	INVOKE(0x00967A60, lruv_resize_non_destructive, cache, new_page_count);

	//ASSERT(new_page_count >= 0);
	//ASSERT(new_page_count >= lruv_get_used_page_end(cache));
	//
	//lruv_cache_verify(cache, true);
	//
	//c_data_iterator<s_lruv_cache_block> block_iterator;
	//block_iterator.begin(cache->blocks);
	//while (block_iterator.next())
	//{
	//	s_lruv_cache_block* block = block_iterator.get_datum();
	//	ASSERT(block->first_page_index + block->page_count > new_page_count, "can't resize non-destructively if we have blocks allocated past the new page count");
	//}
	//
	//cache->maximum_page_count = new_page_count;
	//cache->fragmentation_threshold = lruv_compute_fragmentation_threshold(cache);
}

void __cdecl lruv_set_function_pointers_new(s_lruv_cache* cache, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, move_block_proc_t* move_block_proc)
{
	INVOKE(0x00967AA0, lruv_set_function_pointers_new, cache, delete_block_proc, locked_block_proc, move_block_proc);

	//ASSERT(cache);
	//
	//cache->delete_block_proc = delete_block_proc;
	//cache->locked_block_proc = locked_block_proc;
	//cache->move_block_proc = move_block_proc;
}

void __cdecl lruv_set_hole_algorithm(s_lruv_cache* cache, e_hole_algorithm hole_algorithm)
{
	INVOKE(0x00967AC0, lruv_set_hole_algorithm, cache, hole_algorithm);

	//ASSERT(cache);
	//
	//cache->hole_algorithm = hole_algorithm;
}

void __cdecl lruv_setup(s_lruv_cache* cache, const char* name, int32 page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, int32 critical_section_index)
{
	INVOKE(0x00967AD0, lruv_setup, cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//ASSERT(name);
	//ASSERT(page_size_bits >= 0 && page_size_bits < SHORT_BITS);
	//
	//csmemset(cache, 0, sizeof(s_lruv_cache));
	//cache->critical_section_index = critical_section_index;
	//cache->name.set(name);
	//cache->proc_context = proc_context;
	//cache->delete_block_proc = delete_block_proc;
	//cache->locked_block_proc = locked_block_proc;
	//cache->usage_block_proc = usage_block_proc;
	//cache->page_size_bits = page_size_bits;
	//cache->maximum_page_count = 0;
	//cache->fragmentation_threshold = NONE;
	//cache->blocks = nullptr;
	//cache->signature = LRUV_CACHE_SIGNATURE;
	//cache->first_block_index = NONE;
	//cache->last_block_index = NONE;
	//cache->frame_index = 1;
	//cache->hole_algorithm = _hole_algorithm_age;
	//cache->allocation = allocation;
}

bool __cdecl lruv_verify_slave_data_array(const s_lruv_cache* cache, const s_data_array* data)
{
	return INVOKE(0x00967B60, lruv_verify_slave_data_array, cache, data);

	//uns8 valid = 1;
	//
	//c_data_iterator<s_lruv_cache_block> block_iterator;
	//block_iterator.begin(cache->blocks);
	//while (block_iterator.next() && valid)
	//{
	//	valid &= datum_try_and_get(data, block_iterator.get_index()) != nullptr;
	//}
	//
	//c_data_iterator<void> iterator;
	//iterator.begin(data);
	//while (iterator.next() && valid)
	//{
	//	valid &= datum_try_and_get(cache->blocks, iterator.get_index()) != nullptr;
	//}
	//
	//return valid;
}

void __cdecl lruv_wrap_frame_index(s_lruv_cache* cache)
{
	INVOKE(0x00967C10, lruv_wrap_frame_index, cache);

	//ASSERT(cache->frame_index == k_lruv_max_frame_index);
	//cache->frame_index = k_post_wrap_frame_index + 1;
	//
	//c_data_iterator<s_lruv_cache_block> iterator;
	//iterator.begin(cache->blocks);
	//while (iterator.next())
	//{
	//	uns32 block_age = k_lruv_max_frame_index - iterator.get_datum()->last_used_frame_index;
	//	if (block_age <= k_post_wrap_frame_index)
	//	{
	//		iterator.get_datum()->last_used_frame_index.set(k_post_wrap_frame_index - block_age);
	//		ASSERT((k_post_wrap_frame_index - iterator.m_datum->last_used_frame_index) == block_age);
	//	}
	//	else
	//	{
	//		iterator.get_datum()->last_used_frame_index.set(0);
	//	}
	//}
}

bool __cdecl should_use_this_hole_age(const s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b)
{
	return INVOKE(0x00967D60, should_use_this_hole_age, cache, desired_page_count, hole_a, hole_b);

	//if (hole_a->frame_index < hole_b->frame_index)
	//	return true;
	//
	//if (hole_a->frame_index == hole_b->frame_index)
	//	return true;
	//
	//if (hole_a->page_count < hole_b->page_count)
	//	return true;
	//
	//return false;
}

bool __cdecl should_use_this_hole_blend(const s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b)
{
	return INVOKE(0x00967D90, should_use_this_hole_blend, cache, desired_page_count, hole_a, hole_b);

	//return (cache->frame_index != hole_a->frame_index ? cache->frame_index - hole_a->frame_index : 0) * hole_b->page_count > (cache->frame_index != hole_b->frame_index ? cache->frame_index - hole_b->frame_index : 0) * hole_a->page_count;

	//int32 diff_a = cache->frame_index != hole_a->frame_index ? cache->frame_index - hole_a->frame_index : 0;
	//int32 diff_b = cache->frame_index != hole_b->frame_index ? cache->frame_index - hole_b->frame_index : 0;
	//
	//int32 term_a = diff_a * hole_b->page_count;
	//int32 term_b = diff_b * hole_a->page_count;
	//
	//return term_a > term_b;
}

bool __cdecl should_use_this_hole_fragmentation(const s_lruv_cache* cache, int32 desired_page_count, const s_lruv_cache_hole* hole_a, const s_lruv_cache_hole* hole_b)
{
	return INVOKE(0x00967DD0, should_use_this_hole_fragmentation, cache, desired_page_count, hole_a, hole_b);

	//int32 diff_a = cache->frame_index - hole_a->frame_index;
	//int32 diff_b = cache->frame_index - hole_b->frame_index;
	//
	//int32 term_a = diff_a != 0 ? diff_a : 0;
	//int32 term_b = diff_b != 0 ? diff_b : 0;
	//
	//if (diff_a && diff_a >= 300)
	//	term_a = 300;
	//
	//if (diff_b && diff_b >= 300)
	//	term_b = 300;
	//
	//if (term_a + 150 < term_b)
	//	return false;
	//
	//if (term_b + 150 > term_a)
	//	return true;
	//
	//return hole_a->page_count < hole_b->page_count;
}

