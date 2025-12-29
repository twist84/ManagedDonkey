#pragma once

#include "cseries/cseries_console.hpp"
#include "cseries/cseries_memory.hpp"
#include "cseries/cseries_system_memory.hpp"
#include "cseries/cseries_windows.hpp"
#include "cseries/cseries_windows_debug_pc.hpp"
#include "cseries/stack_walk_windows.hpp"
#include "math/integer_math.hpp"
#include "math/matrix_math.hpp"
#include "math/real_math.hpp"
#include "memory/member_to_static.hpp"

#include <stdarg.h>
#include <type_traits>

#define STARTSWITH(s1, s1_len, s2) (csmemcmp((s1), (s2), csstrnlen((s2), (s1_len))) == 0)

#define DECLFUNC(ADDR, R, CC, ...) reinterpret_cast<R(CC*)(__VA_ARGS__)>(ADDR)
#define INVOKE(ADDR, TYPE, ...) reinterpret_cast<decltype(&TYPE)>(ADDR)(__VA_ARGS__)
#define INVOKE_CLASS_MEMBER(ADDRESS, CLASS, NAME, ...) (this->*static_to_member_t<decltype(&CLASS##::##NAME)>{ .address = ADDRESS }.function)(__VA_ARGS__)

#define DECLARE_LAMBDA(NAME, RETURN_TYPE, FUNCTION) struct s_##NAME { RETURN_TYPE operator () FUNCTION } NAME; // https://stackoverflow.com/a/31920570
#define DECLARE_LAMBDA2(NAME, RETURN_TYPE, ...) RETURN_TYPE(*NAME)(__VA_ARGS__) = [](__VA_ARGS__) -> RETURN_TYPE

#define REFERENCE_DECLARE(address, type, name) type& name = *reinterpret_cast<type*>(address)
#define REFERENCE_DECLARE_ARRAY(address, type, name, count) type(&name)[count] = *reinterpret_cast<type(*)[count]>(address)
#define REFERENCE_DECLARE_2D_ARRAY(address, type, name, count0, count1) type(&name)[count0][count1] = *reinterpret_cast<type(*)[count0][count1]>(address)
#define REFERENCE_DECLARE_3D_ARRAY(address, type, name, count0, count1, count3) type(&name)[count0][count1][count3] = *reinterpret_cast<type(*)[count0][count1][count3]>(address)
#define REFERENCE_DECLARE_STATIC_ARRAY(address, type, count, name) c_static_array<type, count> &name = *reinterpret_cast<c_static_array<type, count>*>(address)

#if defined(_DEBUG)
#define DEBUG_ONLY(...) __VA_ARGS__
#define RELEASE_ONLY(...)
#else
#define DEBUG_ONLY(...)
#define RELEASE_ONLY(...) __VA_ARGS__
#endif

#ifdef DEBUG_ONLY
#undef DEBUG_ONLY
#define DEBUG_ONLY(...)
#endif

#define k_tag_string_length 32
#define k_tag_long_string_length 256

// 32-character ascii string
//typedef char string[k_tag_string_length];
//static_assert(sizeof(string) == 0x20);

// 256-character ascii string
//typedef char long_string[k_tag_long_string_length];
//static_assert(sizeof(long_string) == 0x100);

struct s_datum_header
{
	int16 identifier;
};
static_assert(sizeof(s_datum_header) == 0x2);

class c_tag_index
{
public:
	c_tag_index(int32 index) :
		m_index(index)
	{
	}

	c_tag_index() :
		m_index()
	{
	}

	operator int32() const
	{
		return m_index;
	}

public:
	int32 m_index;
};

template<typename t_type>
union t_value_type
{
	byte bytes[sizeof(t_type)];
	t_type value;
};

extern void* offset_pointer(void* pointer, int32 offset);
extern const void* offset_pointer(const void* pointer, int32 offset);
extern unsigned int align_address(unsigned int address, int32 alignment_bits);
extern void* align_pointer(void* pointer, int32 alignment_bits);
extern int32 pointer_distance(const void* pointer_a, const void* pointer_b);
extern int32 pointer_difference(const void* pointer_a, const void* pointer_b);

template<typename t_element_type>
void _swap(t_element_type* a, t_element_type* b)
{
	t_element_type temp = *a;
	*a = *b;
	*b = temp;
}

template<typename t_element_type>
void _reverse(t_element_type* begin, t_element_type* end)
{
	while (begin < end)
	{
		_swap(begin, end);
		begin++;
		end--;
	}
}

class c_allocation_base
{
public:
	virtual void* allocate(uns32 allocation, const char* name);
	virtual void deallocate(void* buffer);
};

class c_system_allocation :
	public c_allocation_base
{
public:
};

class c_normal_allocation :
	public c_allocation_base
{
public:
};

class c_no_allocation :
	public c_allocation_base
{
public:
};

int32 const k_vtable_pointer_size = sizeof(void*);

template<typename t_type>
static uns32 count_bits_dont_call_me(t_type v);

static uns32 count_bits(uns32 v)
{
	return INVOKE(0x004566E0, count_bits_dont_call_me<uns32>, v);
}

static uns32 count_bits(int32 v)
{
	return INVOKE(0x0052DD80, count_bits_dont_call_me<int32>, v);
}

static uns32 count_bits(void* v)
{
	return INVOKE(0x014E9ED0, count_bits_dont_call_me<void*>, v);
}

constexpr int32 bit_count(int32 val)
{
	int32 result = 0;
	while (val != 0)
	{
		val >>= 1;
		result++;
	}
	return result;
}

constexpr bool pointer_is_aligned(void* pointer, int32 alignment_bits)
{
	return ((uns32)pointer & ((1 << alignment_bits) - 1)) == 0;
}

#define COMPILE_ASSERT(EXPR) static_assert(EXPR, #EXPR)

#if defined(_DEBUG)

#define ASSERT(STATEMENT, ...) do { if (!(STATEMENT)) ASSERT_EXCEPTION(STATEMENT, true, __VA_ARGS__); } while (false)
#define ASSERT_EXCEPTION(STATEMENT, IS_EXCEPTION, ...) \
do { \
	if (!(STATEMENT) && !handle_assert_as_exception(#STATEMENT, __FILE__, __LINE__, IS_EXCEPTION)) \
	{ \
	    display_assert(#STATEMENT, __FILE__, __LINE__, IS_EXCEPTION); \
	    if (!is_debugger_present() && g_catch_exceptions) \
	        system_abort(); \
	    else \
	        system_exit(); \
	} \
} while (false)

#define VASSERT(STATEMENT, MESSAGE, ...) VASSERT_EXCEPTION(STATEMENT, MESSAGE, true)
#define VASSERT_EXCEPTION(STATEMENT, MESSAGE, IS_EXCEPTION, ...) \
do { \
	if (!(STATEMENT) && !handle_assert_as_exception(MESSAGE, __FILE__, __LINE__, IS_EXCEPTION)) \
	{ \
	    display_assert(MESSAGE, __FILE__, __LINE__, IS_EXCEPTION); \
	    if (!is_debugger_present() && g_catch_exceptions) \
	        system_abort(); \
	    else \
	        system_exit(); \
	} \
} while (false)

#define UNREACHABLE(...) VASSERT(0, "unreachable")
#define HALT(...) VASSERT(0, "halt()")

#else

#define ASSERT(STATEMENT, ...) do { } while (false)
#define ASSERT_EXCEPTION(STATEMENT, ...) do { } while (false)

#define VASSERT(STATEMENT, MESSAGE, ...) do { } while (false)
#define VASSERT_EXCEPTION(STATEMENT, MESSAGE, ...) do { } while (false)

#define UNREACHABLE(...) do { } while (false)
#define HALT(...) do { } while (false)

#endif // _DEBUG

template<typename t_type>
class c_wrapped_array;

extern bool& g_catch_exceptions;

extern void display_assert(const char* statement, const char* file, int32 line, bool fatal);
extern bool handle_assert_as_exception(const char* statement, const char* file, int32 line, bool fatal);

extern int(__cdecl* csmemcmp)(const void* _Buf1, const void* _Buf2, size_t _Size);
extern void* (__cdecl* csmemcpy)(void* _Dst, const void* _Src, size_t _Size);
extern void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size);

extern size_t strlen_debug(const char* str);
extern int strncmp_debug(const char* s1, const char* s2, size_t size);

extern int32 csstricmp(const char* s1, const char* s2);
extern int32 csstrcmp(const char* s1, const char* s2);
extern int32 csstrnicmp(const char* s1, const char* s2, uns32 max_count);
extern char* __cdecl csstristr(const char* s1, const char* s2);
extern char* csstrnzcpy(char* s1, const char* s2, uns32 size);
extern char* csstrnzcat(char* s1, const char* s2, uns32 size);
extern uns32 csstrnlen(const char* s, uns32 size);
extern char* csstrnupr(char* s, uns32 size);
extern char* csstrnlwr(char* s, uns32 size);
extern const char* csstrstr(const char* look_for, const char* look_inside);
extern char* __cdecl csstrtok(char* s, const char* delimiters, int32 delimiter_mode, char** data);
extern int32 cvsnzprintf(char* buffer, uns32 size, const char* format, va_list list);
extern char* csnzprintf(char* buffer, uns32 size, const char* format, ...);
extern char* csnzappendf(char* buffer, uns32 size, const char* format, ...);
extern const char* __cdecl find_string_end(const char* string, const char* delimiters);
extern const char* __cdecl find_string_end_not_in_delimiter(const char* string, const char* delimiters);
extern void string_copy_bounded(c_wrapped_array<char> out_dest_string, c_wrapped_array<char const> const in_source_string);
extern bool string_is_not_empty(const char* s);
extern int16 __cdecl string_list_find(const char* string, int16 list_count, const char* const* string_list);
extern void string_terminate_at_first_delimiter(char* s, const char* delimiter);

extern int32 ascii_tolower(int32 C);
extern bool ascii_isupper(char C);
extern void ascii_strnlwr(char* string, int32 count);
extern int __fastcall ascii_stristr(const char* look_inside, const char* look_for);
extern int32 __cdecl ascii_strnicmp(const char* s1, const char* s2, unsigned int size);
extern int32 __cdecl ascii_stricmp(const char* s1, const char* s2);

template<typename t_type, int32 k_count>
typename std::enable_if<!std::is_floating_point<t_type>::value, bool>::type
array_is_zeroed(t_type(&data)[k_count])
{
	for (int32 i = 0; i < k_count; i++)
	{
		if (data[i] != 0)
			return false;
	}

	return true;
}

template<typename t_type, int32 k_count>
void zero_array(t_type(&data)[k_count])
{
	csmemset(data, 0, sizeof(t_type) * k_count);
}

template<typename t_type>
class c_wrapped_array
{
public:
	c_wrapped_array()
	{
		set_elements(nullptr, 0);
	}

	c_wrapped_array(t_type* elements, int32 k_element_count)
	{
		set_elements(elements, k_element_count);
	}

	c_wrapped_array(t_type* begin, t_type* end)
	{
		set_elements(begin, (int32)end - (int32)begin);
	}

	template<int32 k_element_count>
	c_wrapped_array(t_type(&elements)[k_element_count])
	{
		set_elements(elements, k_element_count);
	}

	void set_elements(t_type* elements, int32 element_count)
	{
		m_count = element_count;
		m_elements = elements;
	}

	int32 count() const
	{
		return m_count;
	}

	t_type* begin()
	{
		return m_elements;
	}

	t_type* end()
	{
		return m_elements + m_count;
	}

	bool valid_index(int32 index)
	{
		return VALID_INDEX(index, count());
	}

	t_type& operator[](int32 index)
	{
		ASSERT(valid_index(index));

		return m_elements[index];
	}

//protected:
	int32 m_count;
	t_type* m_elements;
};

template<typename t_type>
c_wrapped_array<t_type> make_wrapped_array(t_type* begin, t_type* end)
{
	return c_wrapped_array<t_type>(begin, end);
}

template<typename t_type>
class c_basic_buffer
{
public:
	//c_basic_buffer() :
	//	m_buffer(nullptr),
	//	m_size(0)
	//{
	//}
	//
	//c_basic_buffer(void* start, uns32 size) :
	//	m_buffer(start),
	//	m_size(size)
	//{
	//}
	//
	//c_basic_buffer(void* start, uns32 size) :
	//	m_buffer(start),
	//	m_size(size)
	//{
	//}
	//
	//c_basic_buffer(void* start, const void* end) :
	//	m_buffer(start),
	//	m_size(pointer_distance(start, end))
	//{
	//	ASSERT(start <= end);
	//}

	void clear()
	{
		m_buffer = nullptr;
		m_size = 0;
	}

	void set_buffer(t_type* start, uns32 size)
	{
		ASSERT(start || size == 0);

		m_buffer = start;
		m_size = size;
	}

	void set_buffer(t_type* start, t_type* end)
	{
		ASSERT(start <= end);

		m_buffer = start;
		m_size = pointer_distance(start, end);
	}

	uns32 size()
	{
		return m_size;
	}

	t_type* begin() const
	{
		return m_buffer;
	}

	t_type* end() const
	{
		return (t_type*)offset_pointer(m_buffer, m_size);
	}

//protected:
	t_type* m_buffer;
	uns32 m_size;
};

template<typename t_type, int32 k_count>
class c_static_array
{
public:
	c_static_array()
	{
		clear();
	}

	c_static_array<t_type, k_count>& reverse()
	{
		_reverse(begin(), end() - 1);

		return *this;
	}

	t_type* begin()
	{
		return m_storage;
	}

	t_type* end()
	{
		return &m_storage[k_count];
	}

	const t_type* begin() const
	{
		return m_storage;
	}

	const t_type* end() const
	{
		return &m_storage[k_count];
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
	}

	int32 get_count() const
	{
		return k_count;
	}

	t_type* get_elements()
	{
		return m_storage;
	}

	const t_type& element(int32 index) const
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	t_type& element(int32 index)
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	bool valid(int32 index) const
	{
		return VALID_INDEX(index, k_count);
	}

	void set_all(const t_type& value)
	{
		for (int32 i = 0; i < k_count; i++)
			m_storage[i] = value;
	}

	int32 get_element_size() const
	{
		return sizeof(t_type);
	}

	int32 get_total_element_size() const
	{
		return sizeof(t_type) * k_count;
	}

	const t_type& operator[](int32 index) const
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	t_type& operator[](int32 index)
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

protected:
	t_type m_storage[k_count];
};

template<typename t_type, int32 k_count>
class c_static_sized_dynamic_array
{
public:
	c_static_sized_dynamic_array() :
		m_storage(),
		m_allocated_count(0)
	{
	}

	c_static_sized_dynamic_array<t_type, k_count>& reverse()
	{
		_reverse(begin(), end() - 1);

		return *this;
	}

	t_type* begin()
	{
		return m_storage.begin();
	}

	t_type* end()
	{
		return begin() + m_allocated_count;
	}

	const t_type* begin() const
	{
		return m_storage.begin();
	}

	const t_type* end() const
	{
		return begin() + m_allocated_count;
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
		m_allocated_count = 0;
	}

	int32 count() const
	{
		return m_allocated_count;
	}

	bool full() const
	{
		return m_allocated_count == m_storage.get_count();
	}

	bool valid_index(int32 index) const
	{
		return VALID_INDEX(index, m_allocated_count);
	}

	int32 new_element_index()
	{
		int32 new_index = m_allocated_count;
		ASSERT(m_storage.valid(new_index));

		m_allocated_count++;

		return new_index;
	}

	void resize(int32 new_size)
	{
		ASSERT(new_size >= 0 && new_size <= m_storage.get_count());
		m_allocated_count = new_size;
	}

	t_type& operator[](int32 index)
	{
		ASSERT(valid_index(index));

		return m_storage[index];
	}

protected:
	c_static_array<t_type, k_count> m_storage;
	int32 m_allocated_count;
};

template<typename t_type, int32 k_count>
class c_static_stack
{
public:
	c_static_stack() :
		m_count(0),
		m_elements()
	{
	}

	t_type* get(int32 index)
	{
		ASSERT(valid(index));

		return &m_elements[index];
	}

	t_type* get_top()
	{
		return get(top());
	}

	t_type* get_elements()
	{
		return m_elements;
	}

	int32 push()
	{
		ASSERT(!full());

		return m_count++;
	}

	void pop()
	{
		ASSERT(!empty());

		m_count--;
	}

	void push_back(const t_type& cache)
	{
		*get(push()) = cache;
	}

	void clear()
	{
		m_count = 0;
	}

	int32 count() const
	{
		return m_count;
	}

	void resize(int32 count)
	{
		m_count = count;

		ASSERT(valid());
	}

	bool empty() const
	{
		ASSERT(valid());

		return m_count == 0;
	}

	bool full() const
	{
		ASSERT(valid());

		return m_count == k_count;
	}

	int32 top() const
	{
		ASSERT(!empty());

		return m_count - 1;
	}

	bool valid(int32 index) const
	{
		ASSERT(valid());

		return VALID_INDEX(index, m_count);
	}

	bool valid() const
	{
		return VALID_COUNT(m_count, k_count);
	}

	t_type& operator[](int32 index)
	{
		ASSERT(valid(index));

		return m_elements[index];
	}

protected:
	int32 m_count;
	t_type m_elements[k_count];
};

template<typename t_type, int32 k_type_size = sizeof(t_type), int32 k_alignment_mask = __alignof(t_type) - 1>
class c_typed_opaque_data
{
public:
	t_type* get()
	{
		ASSERT(((uns32)m_opaque_storage & k_alignment_mask) == 0);
		return reinterpret_cast<t_type*>(((uns32)m_opaque_storage + k_alignment_mask) & ~k_alignment_mask);
	}

	uns8 m_opaque_storage[k_type_size];
	//t_type* m_live_object;
};

template<typename t_type, int32 k_maximum_mapped, int32 k_a_hash_scalar, int32 k_b_hash_scalar, int32 k_c_hash_scalar>
class c_simple_hash_table
{
public:
	struct s_data
	{
		t_type entry;
		int32 key_a;
		int32 key_b;
		int32 key_c;
	};
	static_assert(sizeof(s_data) == sizeof(t_type) + (sizeof(int32) * 3));

	int32 m_total_count;
	c_static_array<s_data, k_maximum_mapped> data;
	c_static_array<int16, k_maximum_mapped> m_hash_entry_count;
	c_static_array<int16, k_maximum_mapped> m_original_hash_entry;
};
static_assert(sizeof(c_simple_hash_table<int32, 8192, 3307, 3, 337>) == 0x28004);

template<typename t_type>
class c_reference_count
{
public:
	t_type m_reference_count;
};
static_assert(sizeof(c_reference_count<int32>) == 0x4);

class c_wrapped_flags
{
public:
	uns32* m_bit_storage;
	int32 m_bit_count;
};
static_assert(sizeof(c_wrapped_flags) == 0x8);

template<typename t_type, typename t_storage_type, t_storage_type k_count>
class c_flags_no_init
{
public:
	static t_type const k_maximum_count = (t_type)k_count;
	//static_assert(k_maximum_count <= SIZEOF_BITS(t_storage_type));

	void clear()
	{
		m_flags = 0;
	}

	//void set_all()

	bool test(t_type bit) const
	{
		ASSERT(valid_bit(bit));

		return TEST_BIT(m_flags, bit);
	}

	bool test_range(t_type start_bit, t_type end_bit) const
	{
		ASSERT(valid_bit(start_bit) && valid_bit(end_bit) && (start_bit <= end_bit));
		return TEST_RANGE(m_flags, start_bit, end_bit);
	}

	void set(t_type bit, bool enable)
	{
		ASSERT(valid_bit(bit));

		if (bit < k_maximum_count)
		{
			SET_BIT(m_flags, bit, enable);
		}
	}

	void toggle(t_type bit)
	{
		ASSERT(valid_bit(bit));
		m_flags ^= FLAG(bit);
	}

	bool valid() const
	{
		static_assert((uns16)~MASK(11) == 0xF800);
		return !TEST_MASK(m_flags, ~MASK(k_count));
	}

	bool is_empty() const
	{
		static_assert(MASK(11) == 0x7FF);
		return !TEST_MASK(m_flags, MASK(k_count));
	}

	t_type count() const
	{
		return k_maximum_count;
	}
	
	bool operator==(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa) const
	{
		return m_flags == rsa.m_flags;
	}

	bool operator!=(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa) const
	{
		return m_flags != rsa.m_flags;
	}

	t_storage_type get_unsafe() const
	{
		return m_flags;
	}

	void set_unsafe(t_storage_type new_flags)
	{
		m_flags = new_flags;
	}

	c_flags_no_init<t_type, t_storage_type, k_count>& operator|=(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa)
	{
		m_flags |= rsa.m_flags;
		ASSERT(valid());
		return *this;
	}

	c_flags_no_init<t_type, t_storage_type, k_count>& operator&=(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa)
	{
		m_flags &= rsa.m_flags;
		ASSERT(valid());
		return *this;
	}

	c_flags_no_init<t_type, t_storage_type, k_count> operator~() const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(~m_flags & MASK(k_count));
		return flags;
	}

	//c_flags_no_init<t_type, t_storage_type, k_count> operator&(t_storage_type const rsa) const

	c_flags_no_init<t_type, t_storage_type, k_count> operator&(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa) const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(m_flags & rsa.m_flags);
		return flags;
	}

	//c_flags_no_init<t_type, t_storage_type, k_count> operator|(t_storage_type const rsa) const

	c_flags_no_init<t_type, t_storage_type, k_count> operator|(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa)
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(m_flags | rsa.m_flags);
		return flags;
	}

	static bool valid_bit(t_type bit)
	{
		return VALID_INDEX(bit, k_maximum_count);
	}

	template <class T>
	operator T () const
	{
		return (T)m_flags;
	}

protected:
	t_storage_type m_flags;
};

template<typename t_type, typename t_storage_type, t_storage_type k_count>
class c_flags :
	public c_flags_no_init<t_type, t_storage_type, k_count>
{
public:
	c_flags(t_storage_type flags)
	{
		this->m_flags = flags;
	}

	c_flags()
	{
		this->m_flags = 0;
	}
};

template<int32 k_maximum_count>
class c_static_flags_no_init
{
public:
	static int32 const MAX_COUNT = k_maximum_count;

	void and_(const c_static_flags_no_init<k_maximum_count>* vector_a)
	{
		ASSERT(vector_a);

		for (int32 i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] &= vector_a[i];
	}

	void and_not_range(const c_static_flags_no_init<k_maximum_count>* vector_a, const c_static_flags_no_init<k_maximum_count>* vector_b, int32 count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		const uns32* vector_a_bits = vector_a->get_bits_direct();
		const uns32* vector_b_bits = vector_b->get_bits_direct();

		for (int32 i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = vector_a_bits[i] & ~vector_b_bits[i];
	}

	void and_range(const c_static_flags_no_init<k_maximum_count>* vector_a, const c_static_flags_no_init<k_maximum_count>* vector_b, int32 count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		const uns32* vector_a_bits = vector_a->get_bits_direct();
		const uns32* vector_b_bits = vector_b->get_bits_direct();

		for (int32 i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = vector_a_bits[i] & vector_b_bits[i];
	}

	void clear()
	{
		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void clear_range(int32 count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void copy(const c_static_flags_no_init<k_maximum_count>* vector_a)
	{
		csmemcpy(m_flags, vector_a, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	int32 count_bits_set() const
	{
		return bit_vector_count_bits(m_flags, k_maximum_count);
	}

	void fill(int32 count, uns8 fill_value)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, fill_value, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	const uns32* get_bits_direct() const
	{
		return m_flags;
	}

	uns32* get_writeable_bits_direct()
	{
		return m_flags;
	}

	int32 highest_bit_set_in_range(int32 count) const
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		return bit_vector_highest_bit_set(m_flags, count);
	}

	void invert_bits()
	{
		for (int32 i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = ~m_flags[i];

		// no clue
		//m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1] = (uns8)m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1];
	}

	bool is_clear() const
	{
		uns8 result = 1;
		for (int32 i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			result &= m_flags[i] == 0;

		return result;
	}

	void keep_range(int32 count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (int32 i = BIT_VECTOR_SIZE_IN_LONGS(count); i < BIT_VECTOR_SIZE_IN_LONGS(count); ++i)
			m_flags[i] = 0;

		m_flags[BIT_VECTOR_SIZE_IN_LONGS(count) - 1] &= ((count & (LONG_BITS - 1)) != 0) ? 0xFFFFFFFF >> (LONG_BITS - (count & (LONG_BITS - 1))) : 0xFFFFFFFF;
	}

	void or_bits(const uns32* bits, int32 count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (int32 i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] |= bits[i];
	}

	void set(int32 index, bool enable)
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		if (enable)
			BIT_VECTOR_OR_FLAG(m_flags, index);
		else
			BIT_VECTOR_AND_FLAG(m_flags, index);
	}

	void set_all()
	{
		csmemset(m_flags, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void set_bits_direct_destructive(int32 count, const uns32* bits)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemcpy(m_flags, bits, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void set_range(int32 count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	bool test(int32 index) const
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		return BIT_VECTOR_TEST_FLAG(m_flags, index);
	}

private:
	uns32 m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)];
};

template<int32 k_maximum_count>
class c_static_flags :
	public c_static_flags_no_init<k_maximum_count>
{
};

template<typename t_type, typename t_storage_type, t_type k_minimum_value, t_type k_maximum_value_plus_one>
class c_enum
{
public:
	c_enum() :
		m_enum_value(k_minimum_value)
	{
	}

	c_enum(t_type value) :
		m_enum_value(static_cast<t_storage_type>(value))
	{
	}

	c_enum(t_storage_type value) :
		m_enum_value(value)
	{
	}

	t_type get()
	{
		return static_cast<t_type>(m_enum_value);
	}

	t_type const get() const
	{
		return static_cast<t_type>(m_enum_value);
	}

	void set_raw_value(t_storage_type raw_value)
	{
		m_enum_value = raw_value;
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_enum_value);
	}

	template<typename T>
	bool operator==(T value) const
	{
		return m_enum_value == static_cast<t_storage_type>(value);
	}

	template<typename T> const
	bool operator!=(T value)
	{
		return m_enum_value != static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<(T value) const
	{
		return m_enum_value < static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>(T value) const
	{
		return m_enum_value > static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>=(T value) const
	{
		return m_enum_value >= static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<=(T value) const
	{
		return m_enum_value <= static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator+=(T value)
	{
		m_enum_value += static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator-=(T value)
	{
		m_enum_value -= static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator= (T value)
	{
		m_enum_value = static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator++()
	{
		m_enum_value++;
	}

protected:
	t_storage_type m_enum_value;
};

template<int32 k_maximum_count>
class c_static_string
{
	using t_type = char[k_maximum_count];

public:
	static int32 const element_count = k_maximum_count;

	__forceinline t_type& get()
	{
		return m_string;
	}

	c_static_string() :
		m_string{}
	{
		clear();
	}

	c_static_string(const char* s) :
		m_string{}
	{
		clear();
		set(s);
	}

	void set_foreground_color(const real_rgb_color* color)
	{
		append_print("\x1b[38;2;%d;%d;%dm",
			(int)(255.0f * color->red),
			(int)(255.0f * color->green),
			(int)(255.0f * color->blue));
	}

	void set_background_color(const real_rgb_color* color)
	{
		append_print("\x1b[48;2;%d;%d;%dm",
			(int)(255.0f * color->red),
			(int)(255.0f * color->green),
			(int)(255.0f * color->blue));
	}

	void set_reset_color()
	{
		append_print("\x1b[0m");
	}

	void append(const char* s)
	{
		csstrnzcat(m_string, s, k_maximum_count);
	}

	void append_line(const char* s = nullptr)
	{
		if (s != nullptr)
		{
			csstrnzcat(m_string, s, k_maximum_count);
		}
		csstrnzcat(m_string, "\r\n", k_maximum_count);
	}

	const char* append_print(const char* format, ...)
	{
		va_list list;
		va_start(list, format);

		const char* result = append_print_va(format, list);

		va_end(list);
		return result;
	}

	const char* append_print_line(const char* format, ...)
	{
		va_list list;
		va_start(list, format);

		const char* result = append_print_va(format, list);
		append_line();

		va_end(list);
		return result;
	}

	const char* append_print_va(const char* format, va_list argument_list)
	{
		uns32 current_length = length();

		ASSERT(format);
		ASSERT(current_length >= 0 && current_length < k_maximum_count);

		cvsnzprintf(m_string + current_length, k_maximum_count - current_length, format, argument_list);

		return m_string;
	}

	void clear()
	{
		csmemset(m_string, 0, sizeof(m_string));
	}

	bool contains(const char* string)
	{
		return csstrstr(m_string, string) != 0;
	}

	char* copy_to(char* string, uns32 string_length) const
	{
		if (string_length > k_maximum_count)
		{
			string_length = k_maximum_count;
		}

		return csstrnzcpy(string, m_string, string_length);
	}

	bool ends_with(const char* string) const
	{
		ASSERT(string);

		int32 _length = length();
		int32 suffix_length = csstrnlen(string, k_maximum_count);

		if (suffix_length > _length)
		{
			return false;
		}

		const char* suffix = get_string() + (_length - suffix_length);

		bool result = csmemcmp(suffix, string, suffix_length) == 0;
		return result;
	}

	char* get_buffer()
	{
		return m_string;
	}

	const char* get_offset(int32 offset) const
	{
		if (VALID_INDEX(offset, length()))
		{
			return &m_string[offset];
		}

		return "";
	}

	const char* get_string() const
	{
		return m_string;
	}

	int32 index_of(const char* string) const
	{
		ASSERT(string);

		return next_index_of(string, 0);
	}

	bool is_empty() const
	{
		return !m_string[0];
	}

	bool is_equal(const char* string) const
	{
		ASSERT(string);

		return csstrnlen(string, k_maximum_count) == length() && csmemcmp(get_string(), string, length()) == 0;
	}

	int32 length() const
	{
		return csstrnlen(m_string, k_maximum_count);
	}

	int32 next_index_of(const char* string, int32 start_at) const
	{
		ASSERT(string);

		int32 result = NONE;

		if (start_at < length())
		{
			const char* s = csstrstr(m_string + start_at, string);
			if (s)
			{
				result = s - get_string();
			}
		}

		return result;
	}

	void null_terminate_buffer()
	{
		m_string[k_maximum_count - 1] = 0;
	}

	const char* print(const char* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);

		va_end(list);

		return m_string;
	}

	const char* print_line(const char* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	const char* print_va(const char* format, va_list argument_list)
	{
		cvsnzprintf(m_string, k_maximum_count, format, argument_list);

		return m_string;
	}

	void set(const char* s)
	{
		csstrnzcpy(m_string, s, k_maximum_count);
	}

	void set_bounded(const char* src, int32 length)
	{
		if (length + 1 < k_maximum_count)
		{
			length++;
		}
		else
		{
			length = k_maximum_count;
		}

		csstrnzcpy(m_string, src, length);
	}

	void set_character(int32 index, char character)
	{
		ASSERT(VALID_INDEX(index, k_maximum_count - 1));

		//int32 initial_length = length();
		//ASSERT(VALID_COUNT(index, initial_length));
		//
		//if (index >= initial_length)
		//{
		//	if (index == initial_length && index < k_maximum_count - 1)
		//	{
		//		m_string[index] = character;
		//		m_string[index + 1] = 0;
		//	}
		//}
		//else
		//{
		//	m_string[index] = character;
		//}

		if (!m_string[index])
		{
			m_string[index + 1] = 0;
		}

		m_string[index] = character;
	}

	void set_length(int32 desired_length)
	{
		if (VALID_COUNT(desired_length, k_maximum_count - 1))
		{
			m_string[desired_length] = 0;
		}
	}

	//void set_wchar(const wchar_t* src)
	//{
	//	wchar_string_to_ascii_string(src, m_string, k_maximum_count, nullptr);
	//}

	bool starts_with(const char* string) const
	{
		ASSERT(string);

		return csmemcmp(string, get_string(), csstrnlen(string, k_maximum_count)) == 0;
	}

	bool substring(int32 start_index, int32 count, c_static_string<k_maximum_count>& sub) const
	{
		if (start_index < 0 || count <= 0 || start_index + count > length())
			return false;

		sub.set_bounded(get_offset(start_index), count);

		return true;
	}

protected:
	char m_string[k_maximum_count];
};

class c_string_builder :
	public c_static_string<1024>
{
public:
	c_string_builder();
	c_string_builder(const char* format, ...);
	~c_string_builder();
};

class c_debug_output_path
{
public:
	c_debug_output_path() :
		m_root(),
		m_path()
	{
		m_root.clear();
		m_path.clear();
	}

	const char* get_path(const char* file_path)
	{
		m_path.print("%s%s", c_debug_output_path::get_root(), file_path);

		return m_path.get_string();
	}

	const char* get_root()
	{
		build_root_if_necessary();

		return m_root.get_string();
	}

private:
	void build_root_if_necessary()
	{
		if (m_root.is_empty())
		{
			m_root.set("");
		}
	}

	c_static_string<260> m_root;
	c_static_string<260> m_path;
};
static_assert(sizeof(c_debug_output_path) == 260 * 2);

extern char* tag_to_string(tag _tag, char* buffer);

class c_string_id
{
public:
	c_string_id() : m_id(NONE) {}
	c_string_id(int32 value) : m_id(value) {}
	c_string_id(const c_string_id& other) : m_id(other.m_id) {}

	const char* get_string();
	const char* get_string() const;
	int32 get_value() const { return m_id; }

	bool operator==(const c_string_id& other) const { return m_id == other.m_id; }
	void operator=(const c_string_id& other) { m_id = other.m_id; }

protected:
	string_id m_id;
};
static_assert(sizeof(c_string_id) == sizeof(string_id));

class c_old_string_id :
	public c_string_id
{
public:
	bool is_string(const char* string) const;
};
static_assert(sizeof(c_old_string_id) == sizeof(c_string_id));

class c_word_quantized_position
{
public:
	uns16 m_position[3];
};
static_assert(sizeof(c_word_quantized_position) == 0x6);

class c_dword_unit_vector
{
public:
	uns32 m_vector;
};
static_assert(sizeof(c_dword_unit_vector) == 0x4);

class c_short_quaternion
{
public:
	int16 m_quaternion[4];
};
static_assert(sizeof(c_short_quaternion) == 0x8);

extern int64 make_int64(int32 low, int32 high);

// IDA
template<class T>
T rotate_left(T value, int count)
{
	const uns32 nbits = sizeof(T) * 8;

	if (count > 0)
	{
		count %= nbits;
		T high = value >> (nbits - count);
		if (T(-1) < 0) // signed value
			high &= ~((T(-1) << count));
		value <<= count;
		value |= high;
	}
	else
	{
		count = -count % nbits;
		T low = value << (nbits - count);
		value >>= count;
		value |= low;
	}
	return value;
}

#define __ROL1__(value, count) rotate_left(static_cast<uns8>(value), count)
#define __ROL2__(value, count) rotate_left(static_cast<uns16>(value), count)
#define __ROL4__(value, count) rotate_left(static_cast<uns32>(value), count)
#define __ROL8__(value, count) rotate_left(static_cast<uns64>(value), count)
#define __ROR1__(value, count) rotate_left(static_cast<uns8>(value), -count)
#define __ROR2__(value, count) rotate_left(static_cast<uns16>(value), -count)
#define __ROR4__(value, count) rotate_left(static_cast<uns32>(value), -count)
#define __ROR8__(value, count) rotate_left(static_cast<uns64>(value), -count)

// IDA
template <typename T, typename F>
inline typename std::enable_if<sizeof(T) <= sizeof(F), T>::type __coerce(F f)
{
	T t;
	csmemcpy(&t, &f, sizeof(T));
	return t;
}
#define COERCE_FLOAT(v) __coerce<float>(v)
#define COERCE_DWORD(v) __coerce<DWORD>(v)

#define SLOBYTE(x) (*((signed char *)&(x)))

extern void __cdecl cseries_dispose();
extern void __cdecl cseries_initialize();

extern uns64 get_current_time_in_seconds(uns64 seconds);

extern const real_argb_color* const global_real_argb_white;
extern const real_argb_color* const global_real_argb_grey;
extern const real_argb_color* const global_real_argb_black;
extern const real_argb_color* const global_real_argb_red;
extern const real_argb_color* const global_real_argb_green;
extern const real_argb_color* const global_real_argb_blue;
extern const real_argb_color* const global_real_argb_yellow;
extern const real_argb_color* const global_real_argb_cyan;
extern const real_argb_color* const global_real_argb_magenta;
extern const real_argb_color* const global_real_argb_pink;
extern const real_argb_color* const global_real_argb_lightblue;
extern const real_argb_color* const global_real_argb_orange;
extern const real_argb_color* const global_real_argb_purple;
extern const real_argb_color* const global_real_argb_aqua;
extern const real_argb_color* const global_real_argb_darkgreen;
extern const real_argb_color* const global_real_argb_salmon;
extern const real_argb_color* const global_real_argb_violet;
extern const real_rgb_color* const global_real_rgb_white;
extern const real_rgb_color* const global_real_rgb_grey;
extern const real_rgb_color* const global_real_rgb_black;
extern const real_rgb_color* const global_real_rgb_red;
extern const real_rgb_color* const global_real_rgb_green;
extern const real_rgb_color* const global_real_rgb_blue;
extern const real_rgb_color* const global_real_rgb_yellow;
extern const real_rgb_color* const global_real_rgb_cyan;
extern const real_rgb_color* const global_real_rgb_magenta;
extern const real_rgb_color* const global_real_rgb_pink;
extern const real_rgb_color* const global_real_rgb_lightblue;
extern const real_rgb_color* const global_real_rgb_orange;
extern const real_rgb_color* const global_real_rgb_purple;
extern const real_rgb_color* const global_real_rgb_aqua;
extern const real_rgb_color* const global_real_rgb_darkgreen;
extern const real_rgb_color* const global_real_rgb_salmon;
extern const real_rgb_color* const global_real_rgb_violet;

extern c_system_allocation*& g_system_allocation;
extern c_normal_allocation*& g_normal_allocation;
extern c_no_allocation*& g_no_allocation;

