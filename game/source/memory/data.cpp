#include "memory/data.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x0055B6D0, datum_try_and_get);

void data_verify(s_data_array const* data)
{
	//ASSERT(data);
	//
	//if (data->signature != k_data_signature
	//	|| data->maximum_count < 0
	//	|| data->first_unallocated < 0
	//	|| data->first_unallocated > data->maximum_count
	//	|| data->next_index < 0
	//	|| data->next_index > data->maximum_count
	//	|| data->actual_count < 0
	//	|| data->actual_count > data->first_unallocated
	//	|| !TEST_BIT(data->flags, _data_array_disconnected_bit) && !data->offset_to_data
	//	|| !data->offset_to_bit_vector)
	//{
	//	c_static_string<256> assert_string;
	//	assert_string.print("%s data array @%p is bad or not allocated", data->name, data);
	//	ASSERT3(assert_string.get_string());
	//}
}

long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits)
{
	return INVOKE(0x0055AAB0, data_allocation_size, maximum_count, size, alignment_bits);

	//long alignment = 0;
	//if (alignment_bits)
	//	alignment = MASK(alignment_bits);
	//
	//return maximum_count * size + sizeof(s_data_array) + alignment + BIT_VECTOR_SIZE_IN_BYTES(maximum_count);
}

void __cdecl data_connect(s_data_array* data, long count, void* datums)
{
	INVOKE(0x0055AAE0, data_connect, data, count, datums);

	//ASSERT(TEST_BIT(data->flags, _data_array_disconnected_bit));
	//ASSERT(datums == align_pointer(datums, data->alignment_bits));
	//ASSERT(data->offset_to_bit_vector);
	//ASSERT(data->in_use_bit_vector);
}

void __cdecl data_delete_all(s_data_array* data)
{
	INVOKE(0x0055ABF0, data_delete_all, data);

	//ASSERT(data->valid);
	//ASSERT(data->offset_to_bit_vector);
}

void __cdecl data_disconnect(s_data_array* data)
{
	INVOKE(0x0055ACA0, data_disconnect, data);

	//ASSERT(!TEST_BIT(data->flags, _data_array_disconnected_bit));
	//ASSERT(TEST_BIT(data->flags, _data_array_can_disconnect_bit));
	//
	//data->flags |= FLAG(_data_array_disconnected_bit);
	//data->data = nullptr;
	//data->offset_to_data = 0;
	//data->valid = false;
}

void __cdecl data_dispose(s_data_array* data)
{
	INVOKE(0x0055ACC0, data_dispose, data);

	//c_allocation_base* allocation = data->allocator;
	//ASSERT(allocation != NULL);
	//
	//csmemset(data, 0, sizeof(s_data_array));
	//
	//if (allocation)
	//	allocation->deallocate(data);
}

void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	INVOKE(0x0055ACF0, data_initialize, data, name, maximum_count, size, alignment_bits, allocation);

	//void* data_ptr = align_pointer(data + 1, alignment_bits);
	//data_initialize_disconnected(data, name, maximum_count, size, alignment_bits, allocation, static_cast<dword*>(offset_pointer(data_ptr, maximum_count * size)));
	//data->data = data_ptr;
	//data->offset_to_data = pointer_distance(data, data_ptr);
	//data->flags &= ~FLAG(_data_array_disconnected_bit);
	//data->flags &= ~FLAG(_data_array_can_disconnect_bit);
	//csmemset(offset_pointer(data, offsetof(s_data_array, in_use_bit_vector)), 0, BIT_VECTOR_SIZE_IN_BYTES(maximum_count));
	//data_update_protection(data);
}

void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector)
{
	INVOKE(0x0055ADA0, data_initialize_disconnected, data, name, maximum_count, size, alignment_bits, allocation, in_use_bit_vector);

	//ASSERT(data);
	//ASSERT(name);
	//ASSERT(maximum_count >= 0 && maximum_count <= UNSIGNED_SHORT_MAX);
	//ASSERT(size > 0);
	//ASSERT(alignment_bits >= 0);
	//ASSERT((unsigned int)size == align_address(size, alignment_bits));
	//ASSERT(allocation);
	//ASSERT(in_use_bit_vector);
	//
	//csmemset(data, 0, sizeof(s_data_array));
	//data->name.set(name);
	//data->maximum_count = maximum_count;
	//data->size = size;
	//data->alignment_bits = alignment_bits;
	//data->signature = k_data_signature;
	//data->data = NULL;
	//data->offset_to_data = 0;
	//data->valid = false;
	//data->in_use_bit_vector = in_use_bit_vector;
	//data->offset_to_bit_vector = pointer_distance(data, in_use_bit_vector);
	//data->allocation = allocation;
	//
	//data->flags |= FLAG(_data_array_disconnected_bit);
	//data->flags |= FLAG(_data_array_can_disconnect_bit);
	////data->flags |= FLAG(_data_array_should_verify_data_pattern_bit);
}

void data_iterator_begin(s_data_iterator* iterator, s_data_array const* data)
{
	INVOKE(0x0055AE10, data_iterator_begin, iterator, data);

	//data_verify(data);
	//ASSERT(data->valid);
	//
	//iterator->data = data;
	//iterator->absolute_index = NONE;
	//iterator->index = NONE;
}

void* data_iterator_next(s_data_iterator* iterator)
{
	return INVOKE(0x0055AE30, data_iterator_next, iterator);
}

void* __cdecl data_iterator_next_with_word_flags(s_data_iterator* iterator, long flag_offset, word flag_mask, word flag_value)
{
	return INVOKE(0x0055AE80, data_iterator_next_with_word_flags, iterator, flag_offset, flag_mask, flag_value);
}

// sub_55AEE0, called by unreferenced `c_content_catalogue` function

void __cdecl data_make_invalid(s_data_array* data)
{
	INVOKE(0x0055AF70, data_make_invalid, data);

	//data_verify(data);
	//data->valid = false;
}

void __cdecl data_make_valid(s_data_array* data)
{
	INVOKE(0x0055AF80, data_make_valid, data);

	//data->valid = true;
	//data_delete_all(data);
	//data_verify(data);
}

s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0055AFA0, data_new, name, maximum_count, size, alignment_bits, allocation);

	//s_data_array* data = static_cast<s_data_array*>(allocation->allocate(data_allocation_size(maximum_count, size, alignment_bits), name));
	//if (data)
	//{
	//	data_initialize(data, name, maximum_count, size, alignment_bits, allocation);
	//	data->flags |= FLAG(_data_array_unknown_bit2);
	//}
	//return data;
}

s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0055B010, data_new_disconnected, name, maximum_count, size, alignment_bits, allocation);

	//s_data_array* data = static_cast<s_data_array*>(allocation->allocate(BIT_VECTOR_SIZE_IN_BYTES(maximum_count) + sizeof(s_data_array), name));
	//if (data)
	//{
	//	data_initialize_disconnected(data, name, maximum_count, size, alignment_bits, allocation, static_cast<dword*>(offset_pointer(data, sizeof(s_data_array))));
	//}
	//return data;
}

long __cdecl data_next_absolute_index(s_data_array const* data, long absolute_index)
{
	return INVOKE(0x0055B060, data_next_absolute_index, data, absolute_index);
}

long __cdecl data_next_absolute_index_with_word_flags(s_data_array const* data, long index, long flag_offset, byte flag_mask, byte flag_value)
{
	return INVOKE(0x0055B0B0, data_next_absolute_index_with_word_flags, data, index, flag_offset, flag_mask, flag_value);
}

long __cdecl data_next_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B130, data_next_index, data, index);
}

long __cdecl data_previous_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B170, data_previous_index, data, index);
}

void __cdecl data_set_new_base_address(s_data_array** out_data, s_data_array* data)
{
	INVOKE(0x0055B1D0, data_set_new_base_address, out_data, data);
}

bool __cdecl data_should_verify_data_pattern(s_data_array const* data)
{
	return INVOKE(0x0055B230, data_should_verify_data_pattern, data);
}

long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index)
{
	return INVOKE(0x0055B280, datum_absolute_index_to_index, data, absolute_index);
}

bool __cdecl datum_available_at_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B2B0, datum_available_at_index, data, index);
}

void __cdecl datum_delete(s_data_array* data, long index)
{
	INVOKE(0x0055B2E0, datum_delete, data, index);
}

void __cdecl datum_initialize(s_data_array* data, s_datum_header* header)
{
	INVOKE(0x0055B370, datum_initialize, data, header);
}

long __cdecl datum_new(s_data_array* data)
{
	return INVOKE(0x0055B410, datum_new, data);
}

long __cdecl datum_new_at_absolute_index(s_data_array* data, long absolute_index)
{
	return INVOKE(0x0055B4D0, datum_new_at_absolute_index, data, absolute_index);
}

long __cdecl datum_new_at_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B550, datum_new_at_index, data, index);
}

long __cdecl datum_new_in_range(s_data_array* data, long minimum_index, long count_indices, bool initialize)
{
	return INVOKE(0x0055B5D0, datum_new_in_range, data, minimum_index, count_indices, initialize);
}

void* __cdecl datum_get(s_data_array* data, long index)
{
	word identifier = DATUM_INDEX_TO_IDENTIFIER(index);
	word absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);

	void** data_ptr = (void**)offset_pointer(data, offsetof(s_data_array, data));
	s_datum_header* header = (s_datum_header*)offset_pointer(*data_ptr, absolute_index * data->size);

	ASSERT(data);
	ASSERT(data->valid);

	if (index == NONE)
		ASSERT2(c_string_builder("tried to access %s index NONE",
			data->name.get_string()).get_string());

	if (!identifier)
		ASSERT2(c_string_builder("tried to access %s using datum_get() with an absolute index #%d",
			data->name.get_string(),
			index).get_string());

	if (absolute_index < 0 || absolute_index >= data->first_unallocated)
		ASSERT2(c_string_builder("%s index #%d (0x%x) is out of range (%d)",
			data->name.get_string(),
			absolute_index,
			index,
			data->first_unallocated).get_string());

	if (!header->identifier)
		ASSERT2(c_string_builder("%s index #%d (0x%x) is unused",
			data->name.get_string(),
			absolute_index,
			index).get_string());

	if (header->identifier != identifier)
		ASSERT2(c_string_builder("%s index #%d (0x%x) is changed, should be 0x%x",
			data->name.get_string(),
			absolute_index,
			index,
			BUILD_DATUM_INDEX(header->identifier, absolute_index)).get_string());

	ASSERT(data->alignment_bits == 0 || header == align_pointer(header, data->alignment_bits));
	return header;
}

void* __cdecl datum_try_and_get(s_data_array const* data, long index)
{
	//return INVOKE(0x0055B6D0, datum_try_and_get, data, index);

	if (!data)
		return NULL;

	void* result = NULL;
	
	ASSERT(data);
	ASSERT(data->valid);

	word identifier = DATUM_INDEX_TO_IDENTIFIER(index);
	word absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);

	if (index != NONE || absolute_index != 0xFFFF)
	{
		if (!identifier)
			ASSERT2(c_string_builder("tried to access %s using datum_try_and_get() with an absolute index #%d",
				data->name.get_string(),
				absolute_index).get_string());

		if (absolute_index < 0 || absolute_index >= data->maximum_count)
			ASSERT2(c_string_builder("tried to access %s using datum_try_and_get() with an index 0x%08X outside maximum range [0, %d)",
				data->name.get_string(),
				index,
				data->maximum_count).get_string());

		if (absolute_index < data->first_unallocated)
		{
			void** data_ptr = (void**)offset_pointer(data, offsetof(s_data_array, data));
			s_datum_header* header = (s_datum_header*)offset_pointer(*data_ptr, absolute_index * data->size);

			if (header->identifier)
			{
				if (header->identifier == identifier)
					result = header;
			}
		}
	}
	
	ASSERT(result == align_pointer(result, data->alignment_bits));
	return result;
}

void* __cdecl datum_get_absolute(s_data_array* data, long index)
{
	void** data_ptr = (void**)offset_pointer(data, offsetof(s_data_array, data));
	s_datum_header* header = (s_datum_header*)offset_pointer(*data_ptr, index * data->size);

	ASSERT(data->valid);

	if (index == NONE)
		ASSERT2(c_string_builder("tried to access %s index NONE",
			data->name.get_string()).get_string());

	if (TEST_MASK(index, 0xFFFF0000))
		ASSERT2(c_string_builder("tried to access %s using datum_get_absolute() with a non absolute index #%d",
			data->name.get_string(),
			index).get_string());

	if (index < 0 || index >= data->first_unallocated)
		ASSERT2(c_string_builder("%s absolute index #%d is out of range (%d)",
			data->name.get_string(),
			index,
			data->first_unallocated).get_string());

	if (!header->identifier)
		ASSERT2(c_string_builder("%s absolute index #%d is unused",
			data->name.get_string(),
			index).get_string());

	ASSERT(header == align_pointer(header, data->alignment_bits));

	return header;
}

void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long index)
{
	return INVOKE(0x0055B710, datum_try_and_get_absolute, data, index);

	//void* result = NULL;
	//
	//ASSERT(data);
	//ASSERT(data->valid);
	//
	//long identifier = DATUM_INDEX_TO_IDENTIFIER(index);
	//long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
	//
	//if (index != NONE)
	//{
	//	if (identifier)
	//	{
	//		c_static_string<1024> assert_string;
	//		assert_string.print("tried to access %s using datum_try_and_get_absolute() with a non absolute index #%d (0x%x)",
	//			data->name.get_string(),
	//			DATUM_INDEX_TO_ABSOLUTE_INDEX(index),
	//			index);
	//
	//		ASSERT2(assert_string.get_string());
	//	}
	//
	//	if (index < 0 || index >= data->maximum_count)
	//	{
	//		c_static_string<1024> assert_string;
	//		assert_string.print("tried to access %s using datum_try_and_get_absolute() with an absolute index 0x%04X outside maximum range [0, %d)", 
	//			data->name.get_string(),
	//			index,
	//			data->maximum_count);
	//		ASSERT2(assert_string.get_string());
	//	}
	//
	//	if (absolute_index < data->first_unallocated)
	//	{
	//		void** data_ptr = (void**)offset_pointer(data, offsetof(s_data_array, data));
	//		s_datum_header* header = (s_datum_header*)offset_pointer(data_ptr, absolute_index * data->size);
	//
	//		if (header->identifier)
	//			result = header;
	//	}
	//}
	//
	//ASSERT(result == align_pointer(result, data->alignment_bits));
	//return result;
}

void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index)
{
	return INVOKE(0x0055B740, datum_try_and_get_unsafe, data, index);

	//void* result = NULL;
	//
	//ASSERT(data);
	//ASSERT(data->valid);
	//
	//long identifier = DATUM_INDEX_TO_IDENTIFIER(index);
	//long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
	//
	//if (index != NONE && absolute_index < data->first_unallocated)
	//{
	//	void** data_ptr = (void**)offset_pointer(data, offsetof(s_data_array, data));
	//	s_datum_header* header = (s_datum_header*)offset_pointer(data_ptr, absolute_index * data->size);
	//
	//	if (header->identifier && header->identifier == identifier)
	//		result = header;
	//}
	//
	//ASSERT(result == align_pointer(result, data->alignment_bits));
	//return result;
}

bool __cdecl data_is_full(s_data_array const* data)
{
	ASSERT(data);
	ASSERT(data->valid);

	return data->maximum_count == data->first_unallocated;
}

