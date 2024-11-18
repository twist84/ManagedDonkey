#pragma once

#include "cseries/cseries.hpp"

// 32-bit data array index handle
typedef unsigned long datum_index;
static_assert(sizeof(datum_index) == 0x4);

enum : datum_index
{
	_datum_index_none = 0xFFFFFFFF
};

enum
{
	k_data_signature = 'd@t@'
};

#define DATUM_INDEX_TO_ABSOLUTE_INDEX(VALUE) ((VALUE) & 0xFFFF)
#define DATUM_INDEX_TO_IDENTIFIER(VALUE) ((VALUE) >> 16)
#define BUILD_DATUM_INDEX(IDENTIFIER, ABSOLUTE_INDEX) ((IDENTIFIER << 16) | ABSOLUTE_INDEX)
#define DATA_ARRAY_GET_DATA(DATA) ((DATA)->data)

struct s_datum_header
{
	word identifier;
};
static_assert(sizeof(s_datum_header) == 0x2);

enum e_data_array_flags
{
	_data_array_can_disconnect_bit = 0,
	_data_array_disconnected_bit,

	_data_array_unknown_bit2,
	_data_array_should_verify_data_pattern_bit,

	// are there more flags?
};

enum e_datum_salt
{
	_datum_salt_next_identifier = 0,
	_datum_salt_isolated_next_identifier,

	k_datum_salt_count
};

struct s_data_array;

struct s_data_iterator
{
	s_data_array const* data;
	long index;
	long absolute_index;
};
static_assert(sizeof(s_data_iterator) == 0xC);

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

struct s_data_array
{
	c_static_string<32> name;
	long maximum_count;
	long size;
	byte alignment_bits;
	bool valid;

	// e_data_array_flags
	word_flags flags;

	tag signature; // k_data_signature

	c_allocation_base* allocation;

	long next_index;
	long first_unallocated;
	long actual_count;

	// e_datum_salt
	// salt_type == 0
	word next_identifier;

	// salt_type == 1
	word isolated_next_identifier;

	void* data;
	dword* in_use_bit_vector;
	long offset_to_data;
	long offset_to_bit_vector;
};
static_assert(sizeof(s_data_array) == 0x54);

extern void data_verify(s_data_array const* data);
extern long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits);
extern void __cdecl data_connect(s_data_array* data, long count, void* datums);
extern void __cdecl data_delete_all(s_data_array* data);
extern void __cdecl data_disconnect(s_data_array* data);
extern void __cdecl data_dispose(s_data_array* data);
extern void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
extern void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector);
extern void __cdecl data_iterator_begin(s_data_iterator* iterator, s_data_array const* data);
extern void* __cdecl data_iterator_next(s_data_iterator* iterator);
extern void* __cdecl data_iterator_next_with_word_flags(s_data_iterator* iterator, long flag_offset, word flag_mask, word flag_value);
extern void __cdecl data_make_invalid(s_data_array* data);
extern void __cdecl data_make_valid(s_data_array* data);
extern s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
extern s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
extern long __cdecl data_next_absolute_index(s_data_array const* data, long index);
extern long __cdecl data_next_index(s_data_array const* data, long index);
extern long __cdecl data_previous_index(s_data_array* data, long index);
extern void __cdecl data_set_new_base_address(s_data_array** out_data, s_data_array* data);
extern bool __cdecl data_should_verify_data_pattern(s_data_array const* data);
extern long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index);
extern bool __cdecl datum_available_at_index(s_data_array const* data, long index);
extern void __cdecl datum_delete(s_data_array* data, long index);
extern void __cdecl datum_initialize(s_data_array* data, s_datum_header* header);
extern long __cdecl datum_new(s_data_array* data);
extern long __cdecl datum_new_at_absolute_index(s_data_array* data, long absolute_index);
extern long __cdecl datum_new_at_index(s_data_array* data, long index);
extern long __cdecl datum_new_in_range(s_data_array* data, long minimum_index, long count_indices, bool initialize);
extern void* __cdecl datum_get(s_data_array* data, long index);
extern void* __cdecl datum_try_and_get(s_data_array const* data, long index);
extern void* __cdecl datum_get_absolute(s_data_array* data, long index);
extern void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long index);
extern void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index);
extern bool __cdecl data_is_full(s_data_array const* data);

template <typename t_datum_type>
struct c_smart_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	//s_data_array*& get_restricted_data_array_address()
	//{
	//}

	t_datum_type& operator[](datum_index index) const
	{
		return *(t_datum_type*)datum_get(m_data_array, index);
	}

	void operator=(s_data_array* rhs)
	{
		ASSERT(!rhs || sizeof(t_datum_type) == rhs->size);

		m_data_array = rhs;
	}

	s_data_array* operator*() const
	{
		return m_data_array;
	}

	s_data_array* operator->() const
	{
		return m_data_array;
	}

	//operator struct s_data_array*() const
	//{
	//}

	struct s_typed_access
	{
		byte unused[offsetof(s_data_array, data)];
		t_datum_type* data;
	};

	union
	{
		s_data_array* m_data_array;
		s_typed_access* m_type_access;
	};
};
static_assert(sizeof(c_smart_data_array<s_datum_header>) == sizeof(s_data_array*));

typedef c_smart_data_array<s_datum_header> data_array_base;

template <typename t_datum_type>
struct c_wrapped_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	void initialize(s_data_array* new_data_array)
	{
		m_data_array = new_data_array;
		ASSERT(sizeof(t_datum_type) == m_data_array->size);
	}

	void dispose()
	{
		data_dispose(*m_data_array);
	}

	void reset()
	{
		m_data_array = NULL;
	}

	t_datum_type const* get(long datum_index) const
	{
		return m_data_array[datum_index];
	}

	s_datum_header* get_mutable(long datum_index)
	{
		return m_data_array[datum_index];
	}

	s_data_array const* get_data()
	{
		return m_data_array.m_data_array;
	}

	s_data_array*& get_data_array_reference()
	{
		return m_data_array.m_data_array;
	}

	operator s_data_array* ()
	{
		return m_data_array.m_data_array;
	}

	c_smart_data_array<t_datum_type> m_data_array;
};
static_assert(sizeof(c_wrapped_data_array<s_datum_header>) == sizeof(s_data_array*));

//template <typename t_datum_type>
//struct c_typed_datum_index

template <typename t_datum_type>
struct c_typed_data_array
{
	//void initialize(char const*, long, c_allocation_base*)
	//{
	//}
	//
	//void dispose()
	//{
	//}
	//
	//void delete_datum(c_typed_datum_index<c_typed_data_array<t_datum_type>> const&)
	//{
	//}
	//
	//t_datum_type* get(c_typed_datum_index<c_typed_data_array<t_datum_type>> const&)
	//{
	//}
	//
	//s_data_array* get_data() const
	//{
	//}

	s_data_array* operator*() const
	{
		return reinterpret_cast<s_data_array*>(this);
	}

	s_data_array* operator->() const
	{
		return reinterpret_cast<s_data_array*>(this);
	}

	c_static_string<32> name;
	long maximum_count;
	long size;
	byte alignment_bits;
	bool valid;

	// e_data_array_flags
	word_flags flags;

	tag signature; // k_data_signature

	c_allocation_base* allocation;

	long next_index;
	long first_unallocated;
	long actual_count;

	// e_datum_salt
	// salt_type == 0
	word next_identifier;

	// salt_type == 1
	word isolated_next_identifier;

	t_datum_type* data;
	dword* in_use_bit_vector;
	long offset_to_data;
	long offset_to_bit_vector;
};
static_assert(sizeof(c_typed_data_array<void>) == sizeof(s_data_array));

//template <typename t_datum_type>
//struct c_typed_datum_index
//{
//	c_typed_datum_index(long)
//	{
//	}
//
//	bool valid() const
//	{
//	}
//
//	void clear()
//	{
//	}
//
//	c_typed_datum_index<c_typed_data_array<t_datum_type>> new_datum()
//	{
//	}
//};

template<typename t_datum_type>
struct c_data_iterator
{
	static_assert(std::is_same<t_datum_type, void>::value || std::is_base_of<s_datum_header, t_datum_type>::value);

public:
	c_data_iterator() :
		m_datum(),
		m_iterator()
	{
	}

	void begin(s_data_array* data)
	{
		data_iterator_begin(&m_iterator, data);
	}

	void begin(s_data_array const* data)
	{
		data_iterator_begin(&m_iterator, data);
	}

	bool next()
	{
		m_datum = (t_datum_type*)data_iterator_next(&m_iterator);
		return m_datum != nullptr;
	}

	long get_index() const
	{
		return m_iterator.index;
	}

	short get_absolute_index() const
	{
		return static_cast<short>(m_iterator.absolute_index);
	}

	t_datum_type* get_datum() const
	{
		return m_datum;
	}

//protected:
	t_datum_type* m_datum;
	s_data_iterator m_iterator;
};

