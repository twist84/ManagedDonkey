#pragma once

#include "cseries/cseries.hpp"

// 32-bit data array index handle
typedef unsigned long datum_index;
static_assert(sizeof(datum_index) == 0x4);

enum : datum_index
{
	_datum_index_none = 0xFFFFFFFF
};

#define DATUM_INDEX_TO_ABSOLUTE_INDEX(VALUE) ((VALUE) & 0xFFFF)
#define DATUM_INDEX_TO_IDENTIFIER(VALUE)  ((VALUE) >> 16)

struct s_datum_header
{
	word identifier;
};
static_assert(sizeof(s_datum_header) == 0x2);

struct s_data_array
{
	string name;
	long maximum_count;
	long size;
	byte alignment_bits;
	bool valid;

	// bit 0, _data_array_can_disconnect_bit
	// bit 1, _data_array_disconnected_bit
	word flags;

	tag signature;

	// c_allocation_interface
	void* allocator;

	long next_index;
	long first_unallocated;
	long actual_count;

	// e_datum_salt
	// salt_type == 0
	word next_identifier;

	// salt_type == 1
	word isolated_next_identifier;

	char* data;
	dword* in_use_bit_vector;
	long offset_to_data;
	long offset_to_bit_vector;

	long get_index(long index) const;
	long get_allocation_size() const;
	s_datum_header* get_datum(const datum_index index) const;
};
static_assert(sizeof(s_data_array) == 0x54);

template <typename t_datum_type>
struct c_data_iterator;

// #TODO: decide if this should be `c_smart_data_array` or `c_wrapped_data_array`
template <typename t_datum_type>
struct c_smart_data_array : s_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	t_datum_type& operator[](datum_index index) const { return *(t_datum_type*)get_datum(index); }

	c_data_iterator<t_datum_type> begin()
	{
		c_data_iterator<t_datum_type> result(this);
		result.next();
		return result;
	}

	c_data_iterator<t_datum_type> end()
	{
		c_data_iterator<t_datum_type> result(this);
		result.current_index = maximum_count;
		return result;
	}
};
static_assert(sizeof(c_smart_data_array<s_datum_header>) == sizeof(s_data_array));

typedef c_smart_data_array<s_datum_header> data_array_base;

struct s_data_iterator
{
	const s_data_array* data;
	datum_index index;
	long current_index;

	s_data_iterator(const s_data_array* data);

	s_datum_header* next();

	bool operator==(const s_data_iterator& other) const;
	bool operator!=(const s_data_iterator& other) const;
};
static_assert(sizeof(s_data_iterator) == 0xC);

template <typename t_datum_type>
struct c_data_iterator : s_data_iterator
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	c_data_iterator(c_smart_data_array<t_datum_type>* data) : s_data_iterator(data) {}
	c_data_iterator() : c_data_iterator(nullptr) {}

	//t_datum_type* next() { return static_cast<t_datum_type*>(data_iterator_base::next()); }

	c_data_iterator<t_datum_type>& operator++() { next(); return *this; }
	c_data_iterator<t_datum_type> operator++(int) { auto result = *this; operator++(); return result; }

	t_datum_type* operator->() const { return (t_datum_type*)data->get_datum(index); }

	t_datum_type& operator*() const { return *operator->(); }
};
static_assert(sizeof(c_data_iterator<s_datum_header>) == sizeof(s_data_iterator));

enum class data_address_type : long
{
	memory,
	definition,
	resource
};
static_assert(sizeof(data_address_type) == 0x4);

union data_address
{
	dword value;
	struct
	{
		data_address_type type : 3;
		long offset : 29;
	};
};
static_assert(sizeof(data_address) == 0x4);