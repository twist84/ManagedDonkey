#pragma once

#include "cseries/cseries.hpp"

// 32-bit data array index handle
typedef unsigned long datum_index;
static_assert(sizeof(datum_index) == 0x4);

enum : datum_index
{
	_datum_index_none = 0xFFFFFFFF
};

#define DATUM_INDEX_INDEX(VALUE) ((VALUE) & 0xFFFF)
#define DATUM_INDEX_SALT(VALUE)  ((VALUE) >> 16)

struct datum_header
{
	word salt;
};
static_assert(sizeof(datum_header) == 0x2);

struct data_array_header
{
	string name;
	long capacity;
	long datum_size;
	byte alignment;
	bool is_valid;
	word flags;
	tag signature;
	void* allocator;
	long next_index;
	long first_unallocated;
	long actual_count;
	word next_salt;
	word next_salt_alt;
	char* data;
	dword* active_indices;
	long header_size;
	long total_size;

	long get_index(long index) const;
	long get_allocation_size() const;
	datum_header* get_datum(const datum_index index) const;
};
static_assert(sizeof(data_array_header) == 0x54);

template <typename datum>
struct data_iterator;

template <typename datum>
struct data_array : data_array_header
{
	static_assert(__is_base_of(datum_header, datum));

	datum& operator[](datum_index index) const { return *(datum*)get_datum(index); }

	data_iterator<datum> begin()
	{
		data_iterator<datum> result(this);
		result.next();
		return result;
	}

	data_iterator<datum> end()
	{
		data_iterator<datum> result(this);
		result.current_index = capacity;
		return result;
	}
};
static_assert(sizeof(data_array<datum_header>) == sizeof(data_array_header));

typedef data_array<datum_header> data_array_base;

struct data_iterator_header
{
	const data_array_header* array;
	datum_index current_datum_index;
	long current_index;

	data_iterator_header(const data_array_header* array);

	datum_header* next();

	bool operator==(const data_iterator_header& other) const;
	bool operator!=(const data_iterator_header& other) const;
};
static_assert(sizeof(data_iterator_header) == 0xC);

template <typename datum>
struct data_iterator : data_iterator_header
{
	static_assert(__is_base_of(datum_header, datum));

	data_iterator(data_array<datum>* array) : data_iterator_header(array) {}
	data_iterator() : data_iterator(nullptr) {}

	//datum* next() { return static_cast<datum*>(data_iterator_base::next()); }

	data_iterator<datum>& operator++() { next(); return *this; }
	data_iterator<datum> operator++(int) { auto result = *this; operator++(); return result; }

	datum* operator->() const { return (datum*)array->get_datum(current_datum_index); }

	datum& operator*() const { return *operator->(); }
};
static_assert(sizeof(data_iterator<datum_header>) == sizeof(data_iterator_header));

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