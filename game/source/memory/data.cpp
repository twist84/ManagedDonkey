#include "memory/data.hpp"

#include "interface/user_interface_memory.hpp"

#include <assert.h>

long s_data_array::get_index(long index) const
{
	if ((index < 0) || (index >= first_unallocated))
		return -1;

	while (!((1 << (index & 31)) & in_use_bit_vector[index >> 5]))
	{
		if (++index >= first_unallocated)
			return -1;
	}

	return index;
}

long s_data_array::get_allocation_size() const
{
	long padding = flags ? ((1 << flags) - 1) : 0;

	return padding + size * maximum_count + 4 * (((maximum_count + 31) >> 5) + 21);
}

s_datum_header* s_data_array::get_datum(const datum_index index) const
{
	if (index == -1 || DATUM_INDEX_TO_ABSOLUTE_INDEX(index) < (dword)first_unallocated)
		return nullptr;

	s_datum_header* datum = (s_datum_header*)&data[index * size];

	if (!datum->identifier || datum->identifier != DATUM_INDEX_TO_IDENTIFIER(index))
		return nullptr;

	return datum;
}

long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits)
{
	return INVOKE(0x0055AAB0, data_allocation_size, maximum_count, size, alignment_bits);

	//long alignment = 0;
	//if (alignment_bits)
	//	alignment = (1 << alignment_bits) - 1;
	//
	//return maximum_count * size + sizeof(s_data_array) + alignment + 4 * ((maximum_count + 31) >> 5);
}

void __cdecl data_connect(s_data_array* data, long count, void* datums)
{
	INVOKE(0x0055AAE0, data_connect, data, count, datums);
}

void __cdecl data_delete_all(s_data_array* data)
{
	INVOKE(0x0055ABF0, data_delete_all, data);
}

void __cdecl data_disconnect(s_data_array* data)
{
	INVOKE(0x0055ACA0, data_disconnect, data);

	//assert(!TEST_BIT(data->flags, _data_array_disconnected_bit));
	//assert(TEST_BIT(data->flags, _data_array_can_disconnect_bit));
	//
	//data->flags.set(_data_array_disconnected_bit, true);
	//data->data = nullptr;
	//data->offset_to_data = 0;
	//data->valid = false;
}

void __cdecl data_dispose(s_data_array* data)
{
	INVOKE(0x0055ACC0, data_dispose, data);

	//c_allocation_base* allocation = data->allocator;
	//assert(allocation != NULL);
	//
	//csmemset(data, 0, sizeof(s_data_array));
	//
	//if (allocation)
	//	allocation->deallocate(data);
}

void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	INVOKE(0x0055ACF0, data_initialize, data, name, maximum_count, size, alignment_bits, allocation);
}

void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector)
{
	INVOKE(0x0055ADA0, data_initialize_disconnected, data, name, maximum_count, size, alignment_bits, allocation, in_use_bit_vector);
}

void data_iterator_begin(s_data_iterator* iterator, s_data_array* data)
{
	INVOKE(0x0055AE10, data_iterator_begin, iterator, data);
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
}

void __cdecl data_make_valid(s_data_array* data)
{
	INVOKE(0x0055AF80, data_make_valid, data);
}

s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0055AFA0, data_new, name, maximum_count, size, alignment_bits, allocation);
}

s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0055B010, data_new_disconnected, name, maximum_count, size, alignment_bits, allocation);
}

long __cdecl data_next_absolute_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B060, data_next_absolute_index, data, index);
}

// sub_55B0B0, called by `data_iterator_next_with_word_flags`

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

long __cdecl datum_absolute_index_to_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B280, datum_absolute_index_to_index, data, index);
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

long __cdecl datum_new_at_absolute_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B4D0, datum_new_at_absolute_index, data, index);
}

long __cdecl datum_new_at_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B550, datum_new_at_index, data, index);
}

long __cdecl datum_new_in_range(s_data_array* data, long begin_index, long end_index, bool initialize)
{
	return INVOKE(0x0055B5D0, datum_new_in_range, data, begin_index, end_index, initialize);
}

void* __cdecl datum_try_and_get(s_data_array const* data, long index)
{
	return INVOKE(0x0055B6D0, datum_try_and_get, data, index);
}

void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long index)
{
	return INVOKE(0x0055B710, datum_try_and_get_absolute, data, index);
}

void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index)
{
	return INVOKE(0x0055B740, datum_try_and_get_unsafe, data, index);
}

