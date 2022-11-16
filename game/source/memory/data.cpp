#include "memory/data.hpp"

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

s_data_iterator::s_data_iterator(const s_data_array* data) :
	data(data), index((datum_index)-1), current_index(-1)
{
}

s_datum_header* s_data_iterator::next()
{
	s_datum_header* result;

	long index = data->get_index(current_index + 1);

	if (index == -1)
	{
		current_index = data->maximum_count;
		index = (datum_index)-1;
		result = nullptr;
	}
	else
	{
		result = (s_datum_header*)&data->data[index * data->size];
		current_index = index;
		index = (datum_index)(index | (result->identifier << 16));
	}

	return result;
}

bool s_data_iterator::operator==(const s_data_iterator& other) const
{
	return (data == other.data)
		&& (current_index == other.current_index)
		&& (index == other.index);
}

bool s_data_iterator::operator!=(const s_data_iterator& other) const
{
	return !(*this == other);
}

void data_iterator_begin(s_data_iterator* iterator, s_data_array* data)
{
	INVOKE(0x0055AE10, data_iterator_begin, iterator, data);
}

void* data_iterator_next(s_data_iterator* iterator)
{
	return INVOKE(0x0055AE30, data_iterator_next, iterator);
}

