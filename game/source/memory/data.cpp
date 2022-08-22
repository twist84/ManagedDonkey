#include "memory/data.hpp"

long data_array_header::get_index(long index) const
{
	if ((index < 0) || (index >= first_unallocated))
		return -1;

	while (!((1 << (index & 0x1F)) & active_indices[index >> 5]))
	{
		if (++index >= first_unallocated)
			return -1;
	}

	return index;
}

long data_array_header::get_allocation_size() const
{
	long padding = flags ? ((1 << flags) - 1) : 0;

	return padding + datum_size * capacity + 4 * (((capacity + 31) >> 5) + 21);
}

datum_header* data_array_header::get_datum(const datum_index index) const
{
	if (index == -1 || DATUM_INDEX_INDEX(index) < (dword)first_unallocated)
		return nullptr;

	datum_header* datum = (datum_header*)&data[index * datum_size];

	if (!datum->salt || datum->salt != DATUM_INDEX_SALT(index))
		return nullptr;

	return datum;
}

data_iterator_header::data_iterator_header(const data_array_header* array) :
	array(array), current_datum_index((datum_index)-1), current_index(-1)
{
}

datum_header* data_iterator_header::next()
{
	datum_header* result;

	long index = array->get_index(current_index + 1);

	if (index == -1)
	{
		current_index = array->capacity;
		current_datum_index = (datum_index)-1;
		result = nullptr;
	}
	else
	{
		result = (datum_header*)&array->data[index * array->datum_size];
		current_index = index;
		current_datum_index = (datum_index)(index | (result->salt << 16));
	}

	return result;
}

bool data_iterator_header::operator==(const data_iterator_header& other) const
{
	return (array == other.array)
		&& (current_index == other.current_index)
		&& (current_datum_index == other.current_datum_index);
}

bool data_iterator_header::operator!=(const data_iterator_header& other) const
{
	return !(*this == other);
}