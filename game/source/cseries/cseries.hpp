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

#define xstr(a) str(a)
#define str(a) #a

#define _STRCONCAT(x, y) x ## y
#define STRCONCAT(x, y) _STRCONCAT(x, y)

#define DECLFUNC(ADDR, R, CC, ...) reinterpret_cast<R(CC*)(__VA_ARGS__)>(ADDR)
#define INVOKE(ADDR, TYPE, ...) reinterpret_cast<decltype(&TYPE)>(ADDR)(__VA_ARGS__)
#define INVOKE_CLASS_MEMBER(ADDRESS, CLASS, NAME, ...) (this->*static_to_member_t<decltype(&CLASS##::##NAME)>{ .address = ADDRESS }.function)(__VA_ARGS__)

#define DECLARE_LAMBDA(NAME, RETURN_TYPE, FUNCTION) struct s_##NAME { RETURN_TYPE operator () FUNCTION } NAME; // https://stackoverflow.com/a/31920570
#define DECLARE_LAMBDA2(NAME, RETURN_TYPE, ...) RETURN_TYPE(*NAME)(__VA_ARGS__) = [](__VA_ARGS__) -> RETURN_TYPE

#define OFFSETOF(s,m) __builtin_offsetof(s,m)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
#define IN_RANGE(value, begin, end) ((value) > (begin) && (value) < (end))
#define IN_RANGE_INCLUSIVE(value, begin, end) ((value) >= (begin) && (value) <= (end))
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count))
#define VALID_COUNT(index, count) ((index) >= 0 && (index) <= (count))

// referenced
#define BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT) (((BIT_COUNT) + (LONG_BITS - 1)) >> 5)
#define BIT_VECTOR_SIZE_IN_BYTES(BIT_COUNT) (4 * BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT))
#define BIT_VECTOR_TEST_FLAG(BIT_VECTOR, BIT) ((BIT_VECTOR[BIT >> 5] & (1 << (BIT & (LONG_BITS - 1)))) != 0)

// not referenced
#define BIT_VECTOR_OR_FLAG(BIT_VECTOR, BIT) (BIT_VECTOR[BIT >> 5] |= (1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_AND_FLAG(BIT_VECTOR, BIT) (BIT_VECTOR[BIT >> 5] &= ~(1 << (BIT & (LONG_BITS - 1))))

#define REFERENCE_DECLARE(address, type, name) type& name = *reinterpret_cast<type*>(address)
#define REFERENCE_DECLARE_ARRAY(address, type, name, count) type(&name)[count] = *reinterpret_cast<type(*)[count]>(address)
#define REFERENCE_DECLARE_2D_ARRAY(address, type, name, count0, count1) type(&name)[count0][count1] = *reinterpret_cast<type(*)[count0][count1]>(address)
#define REFERENCE_DECLARE_3D_ARRAY(address, type, name, count0, count1, count3) type(&name)[count0][count1][count3] = *reinterpret_cast<type(*)[count0][count1][count3]>(address)
#define REFERENCE_DECLARE_STATIC_ARRAY(address, type, count, name) c_static_array<type, count> &name = *reinterpret_cast<c_static_array<type, count>*>(address)

#define FLOOR(a, b) ((a) <= (b) ? (b) : (a))
#define MIN(x, low) ((x) < (low) ? (x) : (low))
#define MAX(x, high) ((x) > (high) ? (x) : (high))
#define CLAMP(x, low, high) ((x) < (low) ? (low) : (x) > (high) ? (high) : (x))
#define CLAMP_LOWER(x, low, high) ((x) >= (high) - (low) ? (x) - (high) : (low))
#define CLAMP_UPPER(x, low, high) ((x) <= (high) - (low) ? (x) + (low) : (high))

#define try_bool(X) if (!X) return false

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

// 4-character tag group identifier
typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

enum : tag
{
	_tag_none = 0xFFFFFFFF
};

enum e_none_sentinel
{
	NONE = -1
};

#define k_tag_string_length 32
#define k_tag_long_string_length 256

// 32-character ascii string
//typedef char string[k_tag_string_length];
//static_assert(sizeof(string) == 0x20);

// 256-character ascii string
//typedef char long_string[k_tag_long_string_length];
//static_assert(sizeof(long_string) == 0x100);

typedef long string_id;
static_assert(sizeof(string_id) == 0x4);

typedef char int8;
static_assert(sizeof(int8) == 0x1);

typedef unsigned char uint8;
static_assert(sizeof(uint8) == 0x1);

typedef short int16;
static_assert(sizeof(int16) == 0x2);

typedef unsigned short uint16;
static_assert(sizeof(uint16) == 0x2);

typedef long int32;
static_assert(sizeof(int32) == 0x4);

typedef unsigned long uint32;
static_assert(sizeof(uint32) == 0x4);

typedef long long int64;
static_assert(sizeof(int64) == 0x8);

typedef unsigned long long uint64;
static_assert(sizeof(uint64) == 0x8);

typedef float real32;
static_assert(sizeof(real32) == 0x4);

typedef double real64;
static_assert(sizeof(real64) == 0x8);

typedef char utf8;

struct s_datum_header
{
	short identifier;
};
static_assert(sizeof(s_datum_header) == 0x2);

class c_tag_index
{
public:
	c_tag_index(long index) :
		m_index(index)
	{
	}

	c_tag_index() :
		m_index()
	{
	}

	operator long() const
	{
		return m_index;
	}

public:
	long m_index;
};

#define SIZEOF_BITS(value) 8 * sizeof(value)

const long CHAR_BYTES = sizeof(char);
const long SHORT_BYTES = sizeof(short);
const long LONG_BYTES = sizeof(long);
const long CHAR_BITS = SIZEOF_BITS(char);
const long SHORT_BITS = SIZEOF_BITS(short);
const long LONG_BITS = SIZEOF_BITS(long);

#define FLAG(bit) (1 << (bit))
#define MASK(bit) ((1 << (bit)) - 1)
#define TEST_BIT(flags, bit) (((flags) & (1 << (bit))) != 0)
#define TEST_RANGE(flags, start_bit, end_bit) (((flags) & (((1 << ((end_bit) - (start_bit) + 1)) - 1) << (start_bit))) != 0)
#define TEST_FLAG(flags, bit) (flags.test((bit)))
#define TEST_MASK(flags, mask) (((flags) & (mask)) != 0)
#define ALIGN(value, bit) (((value) & ~((1 << (bit)) - 1)) + (1 << (bit)))
#define ALIGN_UP(value, bit) ((((value) & ((1 << (bit)) - 1)) == 0) ? (value) : ((value) | ((1 << (bit)) - 1)) + 1)
#define SET_BIT(flags, bit, enable) { if ((enable)) { (flags) |= FLAG((bit)); } else { (flags) &= ~FLAG((bit)); } }
#define SET_MASK(flags, mask, enable) { if ((enable)) { (flags) |= (mask); } else { (flags) &= ~(mask); } }
#define VALID_BITS(flags, max_bits) ((flags) & ~((1 << (max_bits)) - 1))

#define INVALID_ASYNC_TASK_ID -1

//#define CHAR_MAX char(0x7F)
#define UNSIGNED_CHAR_MAX uint8(0xFF)

//#define SHORT_MAX short(0x7FFF)
#define UNSIGNED_SHORT_MAX uint16(0xFFFF)

//#define LONG_MAX long(0x7FFFFFFF)
#define UNSIGNED_LONG_MAX uint32(0xFFFFFFFF)

template<typename t_type>
union t_value_type
{
	uint8 bytes[sizeof(t_type)];
	t_type value;
};

extern void* offset_pointer(void* pointer, long offset);
extern void const* offset_pointer(void const* pointer, long offset);
extern unsigned int align_address(unsigned int address, long alignment_bits);
extern void* align_pointer(void* pointer, long alignment_bits);
extern long pointer_distance(void const* pointer_a, void const* pointer_b);
extern long pointer_difference(void const* pointer_a, void const* pointer_b);

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

struct c_allocation_base
{
public:
	virtual void* allocate(uint32 allocation, char const* name);
	virtual void deallocate(void* buffer);
};

struct c_system_allocation :
	public c_allocation_base
{
public:
};

struct c_normal_allocation :
	public c_allocation_base
{
public:
};

struct c_no_allocation :
	public c_allocation_base
{
public:
};

long const k_vtable_pointer_size = sizeof(void*);

struct c_robust_void_pointer
{
	void* m_value[3];
};

template<typename t_type>
struct c_robust_pointer
{
	c_robust_void_pointer m_pointer;
};

constexpr long bit_count(long val)
{
	long result = 0;
	while (val != 0)
	{
		val >>= 1;
		result++;
	}
	return result;
}

constexpr bool pointer_is_aligned(void* pointer, long alignment_bits)
{
	return ((unsigned long)pointer & ((1 << alignment_bits) - 1)) == 0;
}

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

#define ASSERT2(STATEMENT) ASSERT_EXCEPTION2(STATEMENT, true)
#define ASSERT_EXCEPTION2(STATEMENT, IS_EXCEPTION, ...) \
do { \
	if (!handle_assert_as_exception(STATEMENT, __FILE__, __LINE__, IS_EXCEPTION)) \
	{ \
	    display_assert(STATEMENT, __FILE__, __LINE__, IS_EXCEPTION); \
	    if (!is_debugger_present() && g_catch_exceptions) \
	        system_abort(); \
	    else \
	        system_exit(); \
	} \
} while (false)

#else

#define ASSERT(STATEMENT, ...) do { } while (false)
#define ASSERT_EXCEPTION(STATEMENT, ...) do { } while (false)

#define ASSERT2(STATEMENT, ...) do { } while (false)
#define ASSERT_EXCEPTION2(STATEMENT, ...) do { } while (false)

#define ASSERT3(STATEMENT, ...) do { } while (false)
#define ASSERT_EXCEPTION3(STATEMENT, ...) do { } while (false)

#endif // _DEBUG

extern bool& g_catch_exceptions;

extern void display_assert(char const* statement, char const* file, long line, bool fatal);
extern bool handle_assert_as_exception(char const* statement, char const* file, long line, bool fatal);

extern int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size);
extern void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size);
extern void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size);

extern int strncmp_debug(char const* s1, char const* s2, size_t size);

extern long csstricmp(char const* s1, char const* s2);
extern long csstrcmp(char const* s1, char const* s2);
extern long csstrnicmp(char const* s1, char const* s2, uint32 max_count);
extern char* __cdecl csstristr(char const* s1, char const* s2);
extern char* csstrnzcpy(char* s1, char const* s2, uint32 size);
extern char* csstrnzcat(char* s1, char const* s2, uint32 size);
extern uint32 csstrnlen(char const* s, uint32 size);
extern char* csstrnupr(char* s, uint32 size);
extern char* csstrnlwr(char* s, uint32 size);
extern char const* csstrstr(char const* look_for, char const* look_inside);
extern char* __cdecl csstrtok(char* s, char const* delimiters, long delimiter_mode, char** data);
extern long cvsnzprintf(char* buffer, uint32 size, char const* format, va_list list);
extern char* csnzprintf(char* buffer, uint32 size, char const* format, ...);
extern char* csnzappendf(char* buffer, uint32 size, char const* format, ...);
extern bool string_is_not_empty(char const* s);
extern void string_terminate_at_first_delimiter(char* s, char const* delimiter);

extern long ascii_tolower(long C);
extern bool ascii_isupper(char C);
extern void ascii_strnlwr(char* string, long count);
extern int __fastcall ascii_stristr(char const* look_inside, char const* look_for);
extern long __cdecl ascii_strnicmp(char const* s1, char const* s2, unsigned int size);
extern long __cdecl ascii_stricmp(char const* s1, char const* s2);

template<typename t_type, long k_count>
typename std::enable_if<!std::is_floating_point<t_type>::value, bool>::type
array_is_zeroed(t_type(&data)[k_count])
{
	for (long i = 0; i < k_count; i++)
	{
		if (data[i] != 0)
			return false;
	}

	return true;
}

template<typename t_type, long k_count>
void zero_array(t_type(&data)[k_count])
{
	csmemset(data, 0, sizeof(t_type) * k_count);
}

template<typename t_type>
struct c_wrapped_array
{
public:
	c_wrapped_array()
	{
		set_elements(nullptr, 0);
	}

	template<long k_element_count>
	c_wrapped_array(t_type(&elements)[k_element_count])
	{
		set_elements(elements, k_element_count);
	}

	void set_elements(t_type* elements, long element_count)
	{
		m_count = element_count;
		m_elements = elements;
	}

	long count() const
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

	bool valid_index(long index)
	{
		return VALID_INDEX(index, count());
	}

	t_type& operator[](long index)
	{
		ASSERT(valid_index(index));

		return m_elements[index];
	}

//protected:
	long m_count;
	t_type* m_elements;
};

template<typename t_type>
struct c_basic_buffer
{
public:
	//c_basic_buffer() :
	//	m_buffer(nullptr),
	//	m_size(0)
	//{
	//}
	//
	//c_basic_buffer(void* start, uint32 size) :
	//	m_buffer(start),
	//	m_size(size)
	//{
	//}
	//
	//c_basic_buffer(void* start, uint32 size) :
	//	m_buffer(start),
	//	m_size(size)
	//{
	//}
	//
	//c_basic_buffer(void* start, void const* end) :
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

	void set_buffer(t_type* start, uint32 size)
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

	uint32 size()
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
	uint32 m_size;
};

template<typename t_type, long k_count>
struct c_static_array
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

	t_type const* begin() const
	{
		return m_storage;
	}

	t_type const* end() const
	{
		return &m_storage[k_count];
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
	}

	long get_count() const
	{
		return k_count;
	}

	t_type* get_elements()
	{
		return m_storage;
	}

	t_type const& element(long index) const
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	t_type& element(long index)
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	bool valid(long index) const
	{
		return VALID_INDEX(index, k_count);
	}

	void set_all(t_type const& value)
	{
		for (long i = 0; i < k_count; i++)
			m_storage[i] = value;
	}

	long get_element_size() const
	{
		return sizeof(t_type);
	}

	long get_total_element_size() const
	{
		return sizeof(t_type) * k_count;
	}

	t_type const& operator[](long index) const
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	t_type& operator[](long index)
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

protected:
	t_type m_storage[k_count];
};

template<typename t_type, long k_count>
struct c_static_sized_dynamic_array
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

	t_type const* begin() const
	{
		return m_storage.begin();
	}

	t_type const* end() const
	{
		return begin() + m_allocated_count;
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
		m_allocated_count = 0;
	}

	long count() const
	{
		return m_allocated_count;
	}

	bool full() const
	{
		return m_allocated_count == m_storage.get_count();
	}

	bool valid_index(long index) const
	{
		return VALID_INDEX(index, m_allocated_count);
	}

	long new_element_index()
	{
		long new_index = m_allocated_count;
		ASSERT(m_storage.valid(new_index));

		m_allocated_count++;

		return new_index;
	}

	t_type& operator[](long index)
	{
		ASSERT(valid_index(index));

		return m_storage[index];
	}

protected:
	c_static_array<t_type, k_count> m_storage;
	long m_allocated_count;
};

template<typename t_type, long k_count>
struct c_static_stack
{
public:
	c_static_stack() :
		m_count(0),
		m_elements()
	{
	}

	t_type* get(long index)
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

	long push()
	{
		ASSERT(!full());

		return m_count++;
	}

	void pop()
	{
		ASSERT(!empty());

		m_count--;
	}

	void push_back(t_type const& cache)
	{
		*get(push()) = cache;
	}

	void clear()
	{
		m_count = 0;
	}

	long count() const
	{
		return m_count;
	}

	void resize(long count)
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

	long top() const
	{
		ASSERT(!empty());

		return m_count - 1;
	}

	bool valid(long index) const
	{
		ASSERT(valid());

		return VALID_INDEX(index, m_count);
	}

	bool valid() const
	{
		return VALID_COUNT(m_count, k_count);
	}

	t_type& operator[](long index)
	{
		ASSERT(valid(index));

		return m_elements[index];
	}

protected:
	long m_count;
	t_type m_elements[k_count];
};

template<typename t_type, long k_type_size = sizeof(t_type), long k_alignment_mask = __alignof(t_type) - 1>
struct c_typed_opaque_data
{
	t_type* get()
	{
		ASSERT(((uint32)m_opaque_storage & k_alignment_mask) == 0);
		return reinterpret_cast<t_type*>(((uint32)m_opaque_storage + k_alignment_mask) & ~k_alignment_mask);
	}

	uint8 m_opaque_storage[k_type_size];
	//t_type* m_live_object;
};

template<typename t_type, long k_maximum_mapped, long k_a_hash_scalar, long k_b_hash_scalar, long k_c_hash_scalar>
struct c_simple_hash_table
{
	struct s_data
	{
		t_type entry;
		long key_a;
		long key_b;
		long key_c;
	};
	static_assert(sizeof(s_data) == sizeof(t_type) + (sizeof(long) * 3));

	long m_total_count;
	c_static_array<s_data, k_maximum_mapped> data;
	c_static_array<short, k_maximum_mapped> m_hash_entry_count;
	c_static_array<short, k_maximum_mapped> m_original_hash_entry;
};
static_assert(sizeof(c_simple_hash_table<long, 8192, 3307, 3, 337>) == 0x28004);

template<typename t_type>
struct c_reference_count
{
	t_type m_reference_count;
};
static_assert(sizeof(c_reference_count<long>) == 0x4);

struct c_wrapped_flags
{
	uint32* m_bit_storage;
	long m_bit_count;
};
static_assert(sizeof(c_wrapped_flags) == 0x8);

template<typename t_type, typename t_storage_type, long k_count>
struct c_flags_no_init
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
		return (m_flags & MASK(k_maximum_count)) == 0;
	}

	bool is_empty() const
	{
#pragma warning(push)
#pragma warning(disable : 4293)
		return (m_flags & (MASK(SIZEOF_BITS(t_storage_type)) >> (SIZEOF_BITS(t_storage_type) - k_maximum_count))) == 0;
#pragma warning(pop)
	}

	t_type count() const
	{
		return k_maximum_count;
	}
	
	bool operator==(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa) const
	{
		return m_flags == rsa.m_flags;
	}

	bool operator!=(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa) const
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

	c_flags_no_init<t_type, t_storage_type, k_count>& operator|=(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa)
	{
		m_flags |= rsa.m_flags;
		ASSERT(valid());
		return this;
	}

	c_flags_no_init<t_type, t_storage_type, k_count>& operator&=(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa)
	{
		m_flags &= rsa.m_flags;
		ASSERT(valid());
		return this;
	}

	c_flags_no_init<t_type, t_storage_type, k_count> operator~() const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(~m_flags & MASK(k_count));
		return flags;
	}

	//c_flags_no_init<t_type, t_storage_type, k_count> operator&(t_storage_type const rsa) const
	
	c_flags_no_init<t_type, t_storage_type, k_count> operator&(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa) const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(m_flags & rsa.m_flags);
		return flags;
	}

	//c_flags_no_init<t_type, t_storage_type, k_count> operator|(t_storage_type const rsa) const

	c_flags_no_init<t_type, t_storage_type, k_count> operator|(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa)
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
		return static_cast<T>(m_flags);
	}

protected:
	t_storage_type m_flags;
};

template<typename t_type, typename t_storage_type, long k_count>
struct c_flags :
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

template<long k_maximum_count>
struct c_static_flags_no_init
{
	static long const MAX_COUNT = k_maximum_count;

	void and_(c_static_flags_no_init<k_maximum_count> const* vector_a)
	{
		ASSERT(vector_a);

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] &= vector_a[i];
	}

	void and_not_range(c_static_flags_no_init<k_maximum_count> const* vector_a, c_static_flags_no_init<k_maximum_count> const* vector_b, long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		uint32 const* vector_a_bits = vector_a->get_bits_direct();
		uint32 const* vector_b_bits = vector_b->get_bits_direct();

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = vector_a_bits[i] & ~vector_b_bits[i];
	}

	void and_range(c_static_flags_no_init<k_maximum_count> const* vector_a, c_static_flags_no_init<k_maximum_count> const* vector_b, long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		uint32 const* vector_a_bits = vector_a->get_bits_direct();
		uint32 const* vector_b_bits = vector_b->get_bits_direct();

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = vector_a_bits[i] & vector_b_bits[i];
	}

	void clear()
	{
		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void clear_range(long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void copy(c_static_flags_no_init<k_maximum_count> const* vector_a)
	{
		csmemcpy(m_flags, vector_a, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	long count_bits_set() const
	{
		return bit_vector_count_bits(m_flags, k_maximum_count);
	}

	void fill(long count, uint8 fill_value)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, fill_value, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	uint32 const* get_bits_direct() const
	{
		return m_flags;
	}

	uint32* get_writeable_bits_direct()
	{
		return m_flags;
	}

	long highest_bit_set_in_range(long count) const
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		return bit_vector_highest_bit_set(m_flags, count);
	}

	void invert_bits()
	{
		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = ~m_flags[i];

		// no clue
		//m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1] = (uint8)m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1];
	}

	bool is_clear() const
	{
		uint8 result = 1;
		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			result &= m_flags[i] == 0;

		return result;
	}

	void keep_range(long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (long i = BIT_VECTOR_SIZE_IN_LONGS(count); i < BIT_VECTOR_SIZE_IN_LONGS(count); ++i)
			m_flags[i] = 0;

		m_flags[BIT_VECTOR_SIZE_IN_LONGS(count) - 1] &= ((count & (LONG_BITS - 1)) != 0) ? 0xFFFFFFFF >> (LONG_BITS - (count & (LONG_BITS - 1))) : 0xFFFFFFFF;
	}

	void or_bits(uint32 const* bits, long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] |= bits[i];
	}

	void set(long index, bool enable)
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

	void set_bits_direct_destructive(long count, uint32 const* bits)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemcpy(m_flags, bits, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void set_range(long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	bool test(long index) const
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		return BIT_VECTOR_TEST_FLAG(m_flags, index);
	}

	uint32 m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)];
};

template<long k_maximum_count>
struct c_static_flags :
	public c_static_flags_no_init<k_maximum_count>
{
};

template<typename t_type, typename t_storage_type, t_type k_minimum_value, t_type k_maximum_value_plus_one>
struct c_enum
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

// $TODO: find an actual home
struct s_cluster_reference
{
	char bsp_index;
	uint8 cluster_index;
};
static_assert(sizeof(s_cluster_reference) == 0x2);

// $TODO: find an actual home
struct s_location
{
	s_cluster_reference cluster_reference;
	//uint16 leaf_index;
};
static_assert(sizeof(s_location) == sizeof(s_cluster_reference));
//static_assert(sizeof(s_location) == 0x4);

// $TODO: find an actual home
struct c_animation_id
{
	short m_subgraph;
	short m_index;
};
static_assert(sizeof(c_animation_id) == sizeof(short) * 2);

template<long k_maximum_count>
struct c_static_string
{
	using t_type = char[k_maximum_count];

public:
	static long const element_count = k_maximum_count;

	__forceinline t_type& get()
	{
		return m_string;
	}

	c_static_string() :
		m_string{}
	{
		clear();
	}

	c_static_string(char const* s) :
		m_string{}
	{
		clear();
		set(s);
	}

	void null_terminate_buffer()
	{
		m_string[k_maximum_count - 1] = 0;
	}

	void set_character(long index, char character)
	{
		ASSERT(VALID_INDEX(index, k_maximum_count - 1));

		//long initial_length = length();
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
			m_string[index + 1] = 0;

		m_string[index] = character;
	}

	void set_length(long length)
	{
		if (VALID_COUNT(length, k_maximum_count - 1))
		{
			m_string[length] = 0;
		}
	}

	void set(char const* s)
	{
		csstrnzcpy(m_string, s, k_maximum_count);
	}

	void append(char const* s)
	{
		csstrnzcat(m_string, s, k_maximum_count);
	}

	void append_line(char const* s = nullptr)
	{
		if (s != nullptr)
			csstrnzcat(m_string, s, k_maximum_count);
		csstrnzcat(m_string, "\r\n", k_maximum_count);
	}

	char const* print(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);

		va_end(list);

		return m_string;
	}

	char const* print_line(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	char const* print_va(char const* format, va_list list)
	{
		cvsnzprintf(m_string, k_maximum_count, format, list);

		return m_string;
	}

	char const* append_print(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_print_va(format, list);

		va_end(list);
		return result;
	}

	char const* append_print_line(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_print_va(format, list);
		append_line();

		va_end(list);
		return result;
	}

	char const* append_print_va(char const* format, va_list list)
	{
		uint32 current_length = length();

		ASSERT(format);
		ASSERT(current_length >= 0 && current_length < k_maximum_count);

		cvsnzprintf(m_string + current_length, k_maximum_count - current_length, format, list);

		return m_string;
	}

	void clear()
	{
		csmemset(m_string, 0, sizeof(m_string));
	}

	bool is_empty() const
	{
		return !m_string[0];
	}

	char const* get_string() const
	{
		return m_string;
	}

	char const* get_offset(long offset) const
	{
		if (VALID_INDEX(offset, length()))
			return &m_string[offset];

		return "";
	}

	char* get_buffer()
	{
		return m_string;
	}

	long length() const
	{
		return csstrnlen(m_string, k_maximum_count);
	}

	bool is_equal(char const* _string) const
	{
		ASSERT(_string);

		return csstrnlen(_string, k_maximum_count) == length() && csmemcmp(get_string(), _string, length()) == 0;
	}

	bool starts_with(char const* _string) const
	{
		ASSERT(_string);

		return csmemcmp(_string, get_string(), csstrnlen(_string, k_maximum_count)) == 0;
	}

	bool ends_with(char const* _string) const
	{
		ASSERT(_string);

		long _length = length();
		long suffix_length = csstrnlen(_string, k_maximum_count);

		if (suffix_length > _length)
			return false;

		char const* suffix = get_string() + (_length - suffix_length);

		bool result = csmemcmp(suffix, _string, suffix_length) == 0;
		return result;
	}

	long next_index_of(char const* _string, long index) const
	{
		ASSERT(_string);

		long result = NONE;

		if (index < length())
		{
			char const* s = csstrstr(m_string + index, _string);
			if (s)
				result = s - get_string();
		}

		return result;
	}

	long index_of(char const* _string) const
	{
		ASSERT(_string);

		return next_index_of(_string, 0);
	}

	void set_bounded(char const* _string, long _length)
	{
		if (_length + 1 < k_maximum_count)
			_length++;
		else
			_length = k_maximum_count;

		csstrnzcpy(m_string, _string, _length);
	}

	bool substring(long index, long _length, c_static_string<k_maximum_count>& s) const
	{
		if (index < 0 || _length <= 0 || index + _length > length())
			return false;

		s.set_bounded(get_offset(index), _length);

		return true;
	}

	char* copy_to(char* s, unsigned int size)const
	{
		if (size > k_maximum_count)
			size = k_maximum_count;

		return csstrnzcpy(s, m_string, size);
	}

protected:
	char m_string[k_maximum_count];
};

struct c_string_builder :
	public c_static_string<1024>
{
public:
	c_string_builder();
	c_string_builder(char const* format, ...);
	~c_string_builder();
};

extern char* tag_to_string(tag _tag, char* buffer);

struct c_string_id
{
public:
	c_string_id() : m_id(NONE) {}
	c_string_id(long value) : m_id(value) {}
	c_string_id(c_string_id const& other) : m_id(other.m_id) {}

	char const* get_string();
	char const* get_string() const;
	long get_value() const { return m_id; }

	bool operator==(c_string_id const& other) const { return m_id == other.m_id; }
	void operator=(c_string_id const& other) { m_id = other.m_id; }

protected:
	string_id m_id;
};
static_assert(sizeof(c_string_id) == sizeof(string_id));

struct c_old_string_id :
	public c_string_id
{
public:
	bool is_string(char const* string) const;
};
static_assert(sizeof(c_old_string_id) == sizeof(c_string_id));

struct c_word_quantized_position
{
	uint16 m_position[3];
};
static_assert(sizeof(c_word_quantized_position) == 0x6);

struct c_dword_unit_vector
{
	uint32 m_vector;
};
static_assert(sizeof(c_dword_unit_vector) == 0x4);

struct c_short_quaternion
{
	short m_quaternion[4];
};
static_assert(sizeof(c_short_quaternion) == 0x8);

extern int64 make_int64(long low, long high);

// IDA
template<class T>
T rotate_left(T value, int count)
{
	const uint32 nbits = sizeof(T) * 8;

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

#define __ROL1__(value, count) rotate_left(static_cast<uint8>(value), count)
#define __ROL2__(value, count) rotate_left(static_cast<uint16>(value), count)
#define __ROL4__(value, count) rotate_left(static_cast<uint32>(value), count)
#define __ROL8__(value, count) rotate_left(static_cast<uint64>(value), count)
#define __ROR1__(value, count) rotate_left(static_cast<uint8>(value), -count)
#define __ROR2__(value, count) rotate_left(static_cast<uint16>(value), -count)
#define __ROR4__(value, count) rotate_left(static_cast<uint32>(value), -count)
#define __ROR8__(value, count) rotate_left(static_cast<uint64>(value), -count)

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

extern real_argb_color const* const global_real_argb_white;
extern real_argb_color const* const global_real_argb_grey;
extern real_argb_color const* const global_real_argb_black;
extern real_argb_color const* const global_real_argb_red;
extern real_argb_color const* const global_real_argb_green;
extern real_argb_color const* const global_real_argb_blue;
extern real_argb_color const* const global_real_argb_yellow;
extern real_argb_color const* const global_real_argb_cyan;
extern real_argb_color const* const global_real_argb_magenta;
extern real_argb_color const* const global_real_argb_pink;
extern real_argb_color const* const global_real_argb_lightblue;
extern real_argb_color const* const global_real_argb_orange;
extern real_argb_color const* const global_real_argb_purple;
extern real_argb_color const* const global_real_argb_aqua;
extern real_argb_color const* const global_real_argb_darkgreen;
extern real_argb_color const* const global_real_argb_salmon;
extern real_argb_color const* const global_real_argb_violet;
extern real_rgb_color const* const global_real_rgb_white;
extern real_rgb_color const* const global_real_rgb_grey;
extern real_rgb_color const* const global_real_rgb_black;
extern real_rgb_color const* const global_real_rgb_red;
extern real_rgb_color const* const global_real_rgb_green;
extern real_rgb_color const* const global_real_rgb_blue;
extern real_rgb_color const* const global_real_rgb_yellow;
extern real_rgb_color const* const global_real_rgb_cyan;
extern real_rgb_color const* const global_real_rgb_magenta;
extern real_rgb_color const* const global_real_rgb_pink;
extern real_rgb_color const* const global_real_rgb_lightblue;
extern real_rgb_color const* const global_real_rgb_orange;
extern real_rgb_color const* const global_real_rgb_purple;
extern real_rgb_color const* const global_real_rgb_aqua;
extern real_rgb_color const* const global_real_rgb_darkgreen;
extern real_rgb_color const* const global_real_rgb_salmon;
extern real_rgb_color const* const global_real_rgb_violet;

extern c_system_allocation*& g_system_allocation;
extern c_normal_allocation*& g_normal_allocation;
extern c_no_allocation*& g_no_allocation;

