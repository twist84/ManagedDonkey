#include "memory/memory_pool.hpp"

uint32 memory_pool_handle_from_address(s_memory_pool const* pool, void const* pointer)
{
	ASSERT(pool);
	ASSERT(pointer);

	return pointer_distance(pool, pointer);
}

int32 __cdecl memory_pool_block_handle_from_payload_handle(int32 payload_handle)
{
	return INVOKE(0x00969DB0, memory_pool_block_handle_from_payload_handle, payload_handle);
}

void* __cdecl memory_pool_get_address(s_memory_pool const* memory_pool, int32 payload_handle)
{
	ASSERT(memory_pool);

	return payload_handle ? (void*)offset_pointer(memory_pool, payload_handle) : NULL;
}

s_memory_pool_block* __cdecl memory_pool_get_block(s_memory_pool const* pool, int32 payload_handle)
{
	ASSERT(pool);

	return (s_memory_pool_block*)memory_pool_get_address(pool, payload_handle);
}

s_memory_pool_block* __cdecl memory_pool_block_get(s_memory_pool const* pool, int32 payload_handle)
{
	ASSERT(payload_handle);

	//memory_pool_verify(pool);
	s_memory_pool_block* block = memory_pool_get_block(pool, memory_pool_block_handle_from_payload_handle(payload_handle));
	//ASSERT(memory_pool_block_valid(pool, block));

	//s_memory_pool_block* other_block = NULL;
	//for (other_block = memory_pool_get_block(pool, pool->first_block_handle);
	//	other_block && block != other_block;
	//	other_block = memory_pool_get_block(pool, other_block->next_block_handle))
	//{
	//	;
	//}
	//
	//ASSERT(other_block);

	return block;
}

void __cdecl memory_pool_block_free_handle(s_memory_pool* pool, uint32 payload_handle)
{
	//INVOKE(0x00969C80, memory_pool_block_free_handle, pool, payload_handle);

	s_memory_pool_block* block = memory_pool_block_get(pool, payload_handle);
	//memory_pool_verify(pool);
	//ASSERT(memory_pool_block_valid(pool, block));

	pool->free_size += block->size;
	//ASSERT(pool->free_size <= pool->size);

	if (block->previous_block_handle)
		memory_pool_get_block(pool, block->previous_block_handle)->next_block_handle = block->next_block_handle;
	else
		pool->first_block_handle = block->next_block_handle;

	if (block->next_block_handle)
		memory_pool_get_block(pool, block->next_block_handle)->previous_block_handle = block->previous_block_handle;
	else
		pool->last_block_handle = block->previous_block_handle;

	//csmemset(memory_pool_block_get_base(pool, block), 0xDD, block->size);
	//memory_pool_verify(pool);
}

void memory_pool_block_free(s_memory_pool* pool, void const** payload_data)
{
	//INVOKE(0x00969CD0, memory_pool_block_free, pool, payload_data);

	ASSERT(payload_data);

	memory_pool_block_free_handle(pool, memory_pool_handle_from_address(pool, *payload_data));
}

int32 __cdecl memory_pool_get_block_size(s_memory_pool* memory_pool, void const** payload_handle)
{
	return INVOKE(0x0096A4E0, memory_pool_get_block_size, memory_pool, payload_handle);
}

