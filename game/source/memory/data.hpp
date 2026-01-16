#pragma once

#include "cseries/cseries.hpp"

// 32-bit data array index handle
typedef uns32 datum_index;
COMPILE_ASSERT(sizeof(datum_index) == 0x4);

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

enum class data_address_type : int32
{
	memory,
	definition,
	resource
};
COMPILE_ASSERT(sizeof(data_address_type) == 0x4);

union data_address
{
	uns32 value;
	struct
	{
		data_address_type type : 3;
		int32 offset : 29;
	};
};
COMPILE_ASSERT(sizeof(data_address) == 0x4);

struct s_data_array
{
	c_static_string<32> name;
	int32 maximum_count;
	int32 size;
	uns8 alignment_bits;
	bool valid;
	uns16 flags;
	tag signature;
	c_allocation_base* allocation;
	int32 first_possibly_free_absolute_index;
	int32 count;
	int32 actual_count;
	uns16 next_identifier;
	uns16 isolated_next_identifier;
	void* data;
	void* in_use_bit_vector;
	uns32 offset_to_data;
	uns32 offset_to_bit_vector;
};
COMPILE_ASSERT(sizeof(s_data_array) == 0x54);

struct s_data_iterator
{
	s_data_array* data;
	int32 index;
	int32 absolute_index;
};
COMPILE_ASSERT(sizeof(s_data_iterator) == 0xC);

template <typename t_datum_type>
class c_smart_data_array
{
	COMPILE_ASSERT(__is_base_of(s_datum_header, t_datum_type));

public:
	s_data_array*& get_restricted_data_array_address()
	{
		return m_data_array;
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
COMPILE_ASSERT(sizeof(c_smart_data_array<s_datum_header>) == sizeof(s_data_array*));

template <typename t_datum_type>
class c_wrapped_data_array
{
	COMPILE_ASSERT(__is_base_of(s_datum_header, t_datum_type));

public:
	void dispose()
	{
		data_dispose(*m_data_array);
	}

	const t_datum_type* get(int32 datum_index) const
	{
		return m_data_array[datum_index];
	}

	const s_data_array* get_data()
	{
		return m_data_array.m_data_array;
	}

	s_data_array*& get_data_array_reference()
	{
		return m_data_array.m_data_array;
	}

	s_datum_header* get_mutable(int32 datum_index)
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
COMPILE_ASSERT(sizeof(c_wrapped_data_array<s_datum_header>) == sizeof(s_data_array*));

extern int32 __cdecl data_allocation_size(int32 maximum_count, int32 size, int32 alignment_bits);
extern void __cdecl data_connect(s_data_array* data, int32 count, void* datums);
extern void __cdecl data_copy(const s_data_array* src, s_data_array* dst);
extern void __cdecl data_delete_all(s_data_array* data);
extern void __cdecl data_disconnect(s_data_array* data);
extern void __cdecl data_dispose(s_data_array* data);
extern void __cdecl data_initialize(s_data_array* data, const char* name, int32 maximum_count, int32 size, int32 alignment_bits, c_allocation_base* allocation);
extern void __cdecl data_initialize_disconnected(s_data_array* data, const char* name, int32 maximum_count, int32 size, int32 alignment_bits, c_allocation_base* allocation, uns32* in_use_bit_vector);
extern bool __cdecl data_is_full(const s_data_array* data);
extern void __cdecl data_iterator_begin(s_data_iterator* iterator, const s_data_array* data);
extern void* __cdecl data_iterator_next(s_data_iterator* iterator);
extern void* __cdecl data_iterator_next_with_byte_flags(s_data_iterator* iterator, int32 flag_offset, uns8 flag_mask, uns8 flag_value);
extern int32 __cdecl data_last_index(s_data_array* data);
extern void __cdecl data_make_invalid(s_data_array* data);
extern void __cdecl data_make_valid(s_data_array* data);
extern s_data_array* __cdecl data_new(const char* name, int32 maximum_count, int32 size, int32 alignment_bits, c_allocation_base* allocation);
extern s_data_array* __cdecl data_new_disconnected(const char* name, int32 maximum_count, int32 size, int32 alignment_bits, c_allocation_base* allocation);
extern int32 __cdecl data_next_absolute_index(const s_data_array* data, int32 index);
extern int32 __cdecl data_next_index(const s_data_array* data, int32 index);
extern int32 __cdecl data_previous_index(s_data_array* data, int32 index);
extern void __cdecl data_set_new_base_address(s_data_array** pointer_to_set, s_data_array* new_address);
extern void __cdecl data_set_protection(const s_data_array* data, int32 unproteced_element_count);
extern bool __cdecl data_should_verify_data_pattern(const s_data_array* data);
extern void __cdecl data_unprotect_all(const s_data_array* data);
extern void __cdecl data_update_protection(const s_data_array* data);
extern void __cdecl data_verify(const s_data_array* data);
extern int32 __cdecl datum_absolute_index_to_index(const s_data_array* data, int32 absolute_index);
extern bool __cdecl datum_available_at_index(const s_data_array* data, int32 index);
extern void __cdecl datum_delete(s_data_array* data, int32 index);
extern void __cdecl datum_initialize(s_data_array* data, s_datum_header* header);
extern void __cdecl datum_initialize_common(s_data_array* data, s_datum_header* header);
extern void __cdecl datum_initialize_isolated(s_data_array* data, s_datum_header* header);
extern int32 __cdecl datum_new(s_data_array* data);
extern int32 __cdecl datum_new_at_absolute_index(s_data_array* data, int32 absolute_index);
extern int32 __cdecl datum_new_at_index(s_data_array* data, int32 index);
extern int32 __cdecl datum_new_in_range(s_data_array* data, int32 minimum_index, int32 count_indices, bool initialize);
extern void* __cdecl datum_get(s_data_array* data, int32 index);
extern void* __cdecl datum_try_and_get(const s_data_array* data, int32 index);
extern void* __cdecl datum_get_absolute(s_data_array* data, int32 index);
extern void* __cdecl datum_try_and_get_absolute(const s_data_array* data, int32 index);
extern void* __cdecl datum_try_and_get_unsafe(const s_data_array* data, int32 index);

template<typename t_datum_type>
class c_data_iterator
{
	COMPILE_ASSERT((std::is_same<t_datum_type, void>::value || std::is_base_of<s_datum_header, t_datum_type>::value));

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

	void begin(const s_data_array* data)
	{
		data_iterator_begin(&iterator, data);
	}

	int16 get_absolute_index() const
	{
		return DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.absolute_index);
	}

	int32 get_index() const
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
COMPILE_ASSERT(sizeof(c_data_iterator<void>) == 0x10);

template<typename t_datum_type>
class c_data_iterator_with_byte_flags
{
	COMPILE_ASSERT((std::is_same<t_datum_type, void>::value || std::is_base_of<s_datum_header, t_datum_type>::value));

public:
	c_data_iterator_with_byte_flags() :
		m_datum(),
		m_flag_offset(),
		m_flag_mask(),
		m_flag_value(),
		iterator()
	{
	}

	void begin(s_data_array* data, int32 flag_offset, uns8 flag_mask, uns8 flag_value)
	{
		iterator.m_flag_offset = flag_offset;
		iterator.m_flag_mask = flag_mask;
		iterator.m_flag_value = flag_value;
		data_iterator_begin(&iterator, data);
	}

	void begin(const s_data_array* data, int32 flag_offset, uns8 flag_mask, uns8 flag_value)
	{
		iterator.m_flag_offset = flag_offset;
		iterator.m_flag_mask = flag_mask;
		iterator.m_flag_value = flag_value;
		data_iterator_begin(&iterator, data);
	}

	int16 get_absolute_index() const
	{
		return DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.absolute_index);
	}

	int32 get_index() const
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
	int32 m_flag_offset;
	uns8 m_flag_mask;
	uns8 m_flag_value;
	s_data_iterator iterator;
};
COMPILE_ASSERT(sizeof(c_data_iterator_with_byte_flags<void>) == 0x18);

