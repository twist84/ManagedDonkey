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
#define DATUM_GET(DATA, TYPE, INDEX) ((TYPE*)datum_get(DATA, INDEX))
#define DATUM_TRY_AND_GET(DATA, TYPE, INDEX) ((TYPE*)datum_try_and_get(DATA, INDEX))
#define DATUM_GET_ABSOLUTE(DATA, TYPE, INDEX) ((TYPE*)datum_get_absolute(DATA, INDEX))
#define DATUM_TRY_AND_GET_ABSOLUTE(DATA, TYPE, INDEX) ((TYPE*)datum_try_and_get_absolute(DATA, INDEX))

enum
{
	_data_array_can_disconnect_bit = 0,
	_data_array_disconnected_bit,
	_data_array_protection_bit,
	_data_array_verify_data_pattern_bit,
};

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
	word_flags flags;
	tag signature;
	c_allocation_base* allocation;
	long first_possibly_free_absolute_index;
	long count;
	long actual_count;
	word next_identifier;
	word isolated_next_identifier;
	void* data;
	void* in_use_bit_vector;
	dword offset_to_data;
	dword offset_to_bit_vector;
};
static_assert(sizeof(s_data_array) == 0x54);

struct s_data_iterator
{
	s_data_array* data;
	long index;
	long absolute_index;
};
static_assert(sizeof(s_data_iterator) == 0xC);

template <typename t_datum_type>
struct c_smart_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	s_data_array*& get_restricted_data_array_address()
	{
		return this;
	}

	operator s_data_array* () const
	{
		return m_data_array;
	}

	s_data_array* operator->() const
	{
		return m_data_array;
	}

	void operator=(s_data_array* rhs)
	{
		ASSERT(!rhs || sizeof(t_datum_type) == rhs->size);

		m_data_array = rhs;
	}

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

template <typename t_datum_type>
struct c_wrapped_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	void dispose()
	{
		data_dispose(*m_data_array);
	}

	t_datum_type const* get(long datum_index) const
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

	s_datum_header* get_mutable(long datum_index)
	{
		return m_data_array[datum_index];
	}

	void initialize(s_data_array* new_data_array)
	{
		m_data_array = new_data_array;
		ASSERT(sizeof(t_datum_type) == m_data_array->size);
	}

	operator s_data_array* ()
	{
		return m_data_array.m_data_array;
	}

	void reset()
	{
		*m_data_array = NULL;
	}

	c_smart_data_array<t_datum_type> m_data_array;
};
static_assert(sizeof(c_wrapped_data_array<s_datum_header>) == sizeof(s_data_array*));

extern long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits);
extern void __cdecl data_connect(s_data_array* data, long count, void* datums);
extern void __cdecl data_copy(s_data_array const* src, s_data_array* dst);
extern void __cdecl data_delete_all(s_data_array* data);
extern void __cdecl data_disconnect(s_data_array* data);
extern void __cdecl data_dispose(s_data_array* data);
extern void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
extern void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector);
extern bool __cdecl data_is_full(s_data_array const* data);
extern void __cdecl data_iterator_begin(s_data_iterator* iterator, s_data_array const* data);
extern void* __cdecl data_iterator_next(s_data_iterator* iterator);
extern void* __cdecl data_iterator_next_with_byte_flags(s_data_iterator* iterator, long flag_offset, byte flag_mask, byte flag_value);
extern long __cdecl data_last_index(s_data_array* data);
extern void __cdecl data_make_invalid(s_data_array* data);
extern void __cdecl data_make_valid(s_data_array* data);
extern s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
extern s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
extern long __cdecl data_next_absolute_index(s_data_array const* data, long index);
extern long __cdecl data_next_index(s_data_array const* data, long index);
extern long __cdecl data_previous_index(s_data_array* data, long index);
extern void __cdecl data_set_new_base_address(s_data_array** pointer_to_set, s_data_array* new_address);
extern void __cdecl data_set_protection(s_data_array const* data, long unproteced_element_count);
extern bool __cdecl data_should_verify_data_pattern(s_data_array const* data);
extern void __cdecl data_unprotect_all(s_data_array const* data);
extern void __cdecl data_update_protection(s_data_array const* data);
extern void __cdecl data_verify(s_data_array const* data);
extern long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index);
extern bool __cdecl datum_available_at_index(s_data_array const* data, long index);
extern void __cdecl datum_delete(s_data_array* data, long index);
extern void __cdecl datum_initialize(s_data_array* data, s_datum_header* header);
extern void __cdecl datum_initialize_common(s_data_array* data, s_datum_header* header);
extern void __cdecl datum_initialize_isolated(s_data_array* data, s_datum_header* header);
extern long __cdecl datum_new(s_data_array* data);
extern long __cdecl datum_new_at_absolute_index(s_data_array* data, long absolute_index);
extern long __cdecl datum_new_at_index(s_data_array* data, long index);
extern long __cdecl datum_new_in_range(s_data_array* data, long minimum_index, long count_indices, bool initialize);
extern void* __cdecl datum_get(s_data_array* data, long index);
extern void* __cdecl datum_try_and_get(s_data_array const* data, long index);
extern void* __cdecl datum_get_absolute(s_data_array* data, long index);
extern void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long index);
extern void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index);

template<typename t_datum_type>
struct c_data_iterator
{
	static_assert(std::is_same<t_datum_type, void>::value || std::is_base_of<s_datum_header, t_datum_type>::value);

public:
	c_data_iterator() :
		m_datum(),
		iterator()
	{
	}

	void begin(s_data_array* data)
	{
		data_iterator_begin(&iterator, data);
	}

	void begin(s_data_array const* data)
	{
		data_iterator_begin(&iterator, data);
	}

	short get_absolute_index() const
	{
		return DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.absolute_index);
	}

	long get_index() const
	{
		return iterator.index;
	}

	t_datum_type* get_datum() const
	{
		return m_datum;
	}

	bool next()
	{
		m_datum = (t_datum_type*)data_iterator_next(&iterator);
		return m_datum != nullptr;
	}

//protected:
	t_datum_type* m_datum;
	s_data_iterator iterator;
};
static_assert(sizeof(c_data_iterator<void>) == 0x10);

template<typename t_datum_type>
struct c_data_iterator_with_byte_flags
{
	static_assert(std::is_same<t_datum_type, void>::value || std::is_base_of<s_datum_header, t_datum_type>::value);

public:
	c_data_iterator_with_byte_flags() :
		m_datum(),
		m_flag_offset(),
		m_flag_mask(),
		m_flag_value(),
		iterator()
	{
	}

	void begin(s_data_array* data, long flag_offset, byte flag_mask, byte flag_value)
	{
		iterator.m_flag_offset = flag_offset;
		iterator.m_flag_mask = flag_mask;
		iterator.m_flag_value = flag_value;
		data_iterator_begin(&iterator, data);
	}

	void begin(s_data_array const* data, long flag_offset, byte flag_mask, byte flag_value)
	{
		iterator.m_flag_offset = flag_offset;
		iterator.m_flag_mask = flag_mask;
		iterator.m_flag_value = flag_value;
		data_iterator_begin(&iterator, data);
	}

	short get_absolute_index() const
	{
		return DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.absolute_index);
	}

	long get_index() const
	{
		return iterator.index;
	}

	t_datum_type* get_datum() const
	{
		return m_datum;
	}

	bool next()
	{
		m_datum = (t_datum_type*)data_iterator_next(&iterator);
		return m_datum != nullptr;
	}

//protected:
	t_datum_type* m_datum;
	long m_flag_offset;
	byte m_flag_mask;
	byte m_flag_value;
	s_data_iterator iterator;
};
static_assert(sizeof(c_data_iterator_with_byte_flags<void>) == 0x18);

