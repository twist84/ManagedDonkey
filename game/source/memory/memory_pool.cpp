#include "memory/memory_pool.hpp"

//.text:00969AC0 ; s_memory_pool* __cdecl fixed_memory_pool_new(const char*, void*, int32, e_memory_pool_callback_list)
//.text:00969AF0 ; t_memory_pool_callback_list* (get_memory_pool_callback)(e_memory_pool_callback_list)
//.text:00969B00 ; void __cdecl memory_block_notify_reference(s_memory_pool*, s_memory_pool_block*)

int32 __cdecl memory_pool_allocation_size(int32 size)
{
	return INVOKE(0x00969B50, memory_pool_allocation_size, size);

	//return size + sizeof(s_memory_pool) + sizeof(s_memory_pool_block);
}

//.text:00969B60 ; unsigned int __cdecl memory_pool_overhead_size(const s_memory_pool*)

bool __cdecl memory_pool_block_allocate(s_memory_pool* pool, void** ptr, int32 size, const char* file, int32 line)
{
	return INVOKE(0x00969B80, memory_pool_block_allocate, pool, ptr, size, file, line);
}

//.text:00969BF0 ; uns32 __cdecl memory_pool_block_allocate_handle_tracked(s_memory_pool*, int32, unsigned int, const char*, int32)
//.text:00969C50 ; unsigned int __cdecl memory_pool_block_compute_actual_size(const s_memory_pool*, unsigned int)

void __cdecl memory_pool_block_free(s_memory_pool* pool, uns32 payload_handle)
{
	//INVOKE(0x00969C80, memory_pool_block_free, pool, payload_handle);
	DECLFUNC(0x00969C80, void, __cdecl, s_memory_pool*, uns32)(pool, payload_handle);

	//s_memory_pool_block* block = memory_pool_block_get(pool, payload_handle);
	//memory_pool_check_validity(pool);
	////ASSERT(memory_pool_block_valid(pool, block));
	//
	//pool->free_size += block->size;
	//ASSERT(pool->free_size <= pool->size);
	//
	//if (block->previous_block_handle)
	//{
	//	memory_pool_get_block(pool, block->previous_block_handle)->next_block_handle = block->next_block_handle;
	//}
	//else
	//{
	//	pool->first_block_handle = block->next_block_handle;
	//}
	//
	//if (block->next_block_handle)
	//{
	//	memory_pool_get_block(pool, block->next_block_handle)->previous_block_handle = block->previous_block_handle;
	//}
	//else
	//{
	//	pool->last_block_handle = block->previous_block_handle;
	//}
	//
	////csmemset(memory_pool_block_get_base(pool, block), 0xDD, block->size);
	////memory_pool_check_validity(pool);
}

void memory_pool_block_free(s_memory_pool* pool, const void** payload_data)
{
	//INVOKE(0x00969CD0, memory_pool_block_free, pool, payload_data);
	DECLFUNC(0x00969CD0, void, __cdecl, s_memory_pool*, const void**)(pool, payload_data);

	//ASSERT(payload_data);
	//
	//memory_pool_block_free(pool, memory_pool_create_handle(pool, *payload_data));
}

s_memory_pool_block* __cdecl memory_pool_block_get(s_memory_pool* pool, int32 payload_handle)
{
	return INVOKE(0x00969D30, memory_pool_block_get, pool, payload_handle);

	//ASSERT(payload_handle);
	//
	//memory_pool_check_validity(pool);
	//s_memory_pool_block* block = memory_pool_get_block(pool, memory_pool_block_handle_from_payload_handle(payload_handle));
	//return block;
}

void* __cdecl memory_pool_block_get_base(const s_memory_pool* pool, s_memory_pool_block* block)
{
	return INVOKE(0x00969D50, memory_pool_block_get_base, pool, block);

	//if (pool->debug)
	//{
	//	return block;
	//}
	//
	//return ((byte*)block - sizeof(s_memory_pool_block_header));
}

//.text:00969D70 ; void* __cdecl memory_pool_block_get_end_of_block(s_memory_pool*, s_memory_pool_block*)
//.text:00969D90 ; s_memory_pool_block_header* __cdecl memory_pool_block_get_header(s_memory_pool*, s_memory_pool_block*)

int32 __cdecl memory_pool_block_handle_from_payload_handle(int32 payload_handle)
{
	return INVOKE(0x00969DB0, memory_pool_block_handle_from_payload_handle, payload_handle);

	//return payload_handle - sizeof(s_memory_pool_block);
}

//.text:00969DC0 ; s_memory_pool_block* __cdecl memory_pool_block_initialize(s_memory_pool*, void*, int32, const char*, int32, s_memory_pool_block*, s_memory_pool_block*)
//.text:00969E50 ; int32 __cdecl memory_pool_block_allocate_handle(s_memory_pool*, int32, const char*, int32)
//.text:00969EC0 ; s_memory_pool_block* __cdecl memory_pool_block_new_from_end_of_pool(s_memory_pool*, int32, const char*, int32)
//.text:00969F90 ; s_memory_pool_block* __cdecl memory_pool_block_new_from_front_of_pool(s_memory_pool*, int32, const char*, int32)
//.text:0096A060 ; s_memory_pool_block* __cdecl memory_pool_block_new_from_in_between_blocks(s_memory_pool*, int32, const char*, int32)

bool __cdecl memory_pool_block_reallocate(s_memory_pool* pool, void** ptr, int32 size, const char* file, int32 line)
{
	return INVOKE(0x0096A0E0, memory_pool_block_reallocate, pool, ptr, size, file, line);
}

//.text:0096A130 ; int32 __cdecl memory_pool_block_reallocate_handle(s_memory_pool*, int32, int32, const char*, int32)

uns32 __cdecl memory_pool_block_sizeof(const s_memory_pool* pool)
{
	return INVOKE(0x0096A280, memory_pool_block_sizeof, pool);
}

//.text:0096A2A0 ; bool __cdecl memory_pool_block_valid(s_memory_pool*, s_memory_pool_block*)

void __cdecl memory_pool_check_validity(s_memory_pool* pool)
{
	//INVOKE(0x0096A2B0, memory_pool_check_validity, pool);
}

//.text:0096A2C0 ; void __cdecl memory_pool_clear(s_memory_pool*)
//.text:0096A2E0 ; void __cdecl memory_pool_compact(s_memory_pool*)
//.text:0096A3D0 ; int32 __cdecl memory_pool_compute_fixed_size(int32)

uns32 memory_pool_create_handle(const s_memory_pool* memory_pool, const void* data)
{
	return INVOKE(0x0096A3E0, memory_pool_create_handle, memory_pool, data);

	//ASSERT(memory_pool);
	//
	//if (!data)
	//{
	//	return 0;
	//}
	//
	//return pointer_distance(memory_pool, data);
}

void __cdecl memory_pool_dispose(s_memory_pool* pool)
{
	INVOKE(0x0096A400, memory_pool_dispose, pool);

	//c_allocation_base* allocation = pool->allocation;
	//csmemset(pool, 0, sizeof(s_memory_pool));
	//allocation->deallocate(pool);
}

//.text:0096A430 ; void __cdecl memory_pool_dump(const s_memory_pool*, const char*, c_formatted_output*)
//.text:0096A440 ; void __cdecl memory_pool_dump_to_debugger(const s_memory_pool*, const char*)
//.text:0096A450 ; void __cdecl memory_pool_dump_to_file(s_memory_pool*, const char*)

void* __cdecl memory_pool_get_address(const s_memory_pool* memory_pool, int32 handle)
{
	return INVOKE(0x0096A460, memory_pool_get_address, memory_pool, handle);

	//ASSERT(memory_pool);
	//
	//if (!handle)
	//{
	//	return nullptr;
	//}
	//
	//return offset_pointer((void*)memory_pool, handle);
}

//.text:0096A480 ; void* __cdecl memory_pool_get_base_address(const s_memory_pool*)

s_memory_pool_block* __cdecl memory_pool_get_block(const s_memory_pool* pool, int32 handle)
{
	return INVOKE(0x0096A4A0, memory_pool_get_block, pool, handle);

	//ASSERT(pool);
	//
	//return (s_memory_pool_block*)memory_pool_get_address(pool, handle);
}

int32 __cdecl memory_pool_get_block_size(s_memory_pool* pool, int32 payload_handle)
{
	//return INVOKE(0x0096A4C0, memory_pool_get_block_size, pool, payload_handle);
	return DECLFUNC(0x0096A4C0, int32, __cdecl, s_memory_pool*, int32)(pool, payload_handle);

	//s_memory_pool_block* block = memory_pool_block_get(pool, payload_handle);
	//uns32 block_base = memory_pool_block_sizeof(pool);
	//uns32 usable_size = (uns32)block->size - block_base;
	//ASSERT(usable_size >= 0);
	//return block->size;
}

int32 __cdecl memory_pool_get_block_size(s_memory_pool* pool, const void** payload_data)
{
	//return INVOKE(0x0096A4E0, memory_pool_get_block_size, pool, payload_data);
	return DECLFUNC(0x0096A4E0, int32, __cdecl, s_memory_pool*, const void**)(pool, payload_data);

	//ASSERT(payload_data);
	//
	//return memory_pool_get_block_size(pool, memory_pool_create_handle(pool, payload_data));
}

//.text:0096A510 ; unsigned int __cdecl memory_pool_get_contiguous_free_size(const s_memory_pool*)
//.text:0096A560 ; int32 __cdecl memory_pool_get_free_size(s_memory_pool*)
//.text:0096A570 ; int32 __cdecl memory_pool_get_number_of_blocks(s_memory_pool*)
//.text:0096A5A0 ; unsigned int __cdecl memory_pool_get_used_size(const s_memory_pool*)
//.text:0096A5E0 ; void __cdecl memory_pool_initialize(s_memory_pool*, const char*, int32, e_memory_pool_callback_list, c_allocation_base*)

s_memory_pool* __cdecl memory_pool_new(const char* name, int32 size, e_memory_pool_callback_list tracking_callback_index, c_allocation_base* allocation)
{
	return INVOKE(0x0096A650, memory_pool_new, name, size, tracking_callback_index, allocation);

	//s_memory_pool* memory_pool = (s_memory_pool*)allocation->allocate(memory_pool_allocation_size(size), name);
	//if (memory_pool)
	//{
	//	memory_pool_initialize(memory_pool, name, size, tracking_callback_index, allocation);
	//}
	//return memory_pool;
}

//.text:0096A690 ; int32 __cdecl memory_pool_payload_handle_block_handle(int32)

void __cdecl memory_pool_toggle_allocation_from_anywhere_in_pool(s_memory_pool* pool, bool can_allocate_from_anywhere)
{
	INVOKE(0x0096A6A0, memory_pool_toggle_allocation_from_anywhere_in_pool, pool, can_allocate_from_anywhere);
}

void __cdecl memory_pool_toggle_reference_tracking(s_memory_pool* pool, bool track_references)
{
	INVOKE(0x0096A6B0, memory_pool_toggle_reference_tracking, pool, track_references);
}

//.text:0096A6D0 ; void __cdecl memory_pool_toggle_verification(s_memory_pool*, bool)

