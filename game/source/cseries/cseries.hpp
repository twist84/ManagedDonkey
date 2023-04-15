#pragma once

#include "cseries/cseries_windows.hpp"
#include "cseries/cseries_windows_debug_pc.hpp"
#include "cseries/integer_math.hpp"
#include "cseries/real_math.hpp"

#include <stdarg.h>
#include <type_traits>

#define STARTSWITH(s1, s1_len, s2) (csmemcmp((s1), (s2), csstrnlen((s2), (s1_len))) == 0)

#define _STRCONCAT(x, y) x ## y
#define STRCONCAT(x, y) _STRCONCAT(x, y)

#define DECLFUNC(ADDR, R, CC, ...) reinterpret_cast<R(CC*)(__VA_ARGS__)>(ADDR)
#define INVOKE(ADDR, TYPE, ...) reinterpret_cast<decltype(TYPE)*>(ADDR)(__VA_ARGS__)

#define OFFSETOF(s,m) __builtin_offsetof(s,m)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
#define IN_RANGE_INCLUSIVE(value, begin, end) (((value) >= (begin)) && ((value) <= (end)))
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count))

#define REFERENCE_DECLARE(address, type, name) type& name = *reinterpret_cast<type*>(address)
#define REFERENCE_DECLARE_ARRAY(address, type, name, count) type(&name)[count] = *reinterpret_cast<type(*)[count]>(address)
#define REFERENCE_DECLARE_STATIC_ARRAY(address, type, count, name) c_static_array<type, count> &name = *reinterpret_cast<c_static_array<type, count>*>(address)

#define try_bool(X) if (!X) return false

// 4-character tag group identifier
typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

enum : tag
{
	_tag_none = 0xFFFFFFFF
};

// 32-character ascii string
typedef char string[32];
static_assert(sizeof(string) == 0x20);

// 256-character ascii string
typedef char long_string[256];
static_assert(sizeof(long_string) == 0x100);

// a 32-bit string identifier
typedef long string_id;
static_assert(sizeof(string_id) == 0x4);

// 8-bit unsigned integer ranging from 0 to 255
typedef unsigned char byte;
static_assert(sizeof(byte) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
typedef unsigned short word;
static_assert(sizeof(word) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
typedef unsigned long dword;
static_assert(sizeof(dword) == 0x4);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
typedef unsigned long long qword;
static_assert(sizeof(qword) == 0x8);

// 8-bit enumerator value
typedef char char_enum;
static_assert(sizeof(char_enum) == 0x1);

// 16-bit enumerator value
typedef short short_enum;
static_assert(sizeof(short_enum) == 0x2);

// 32-bit enumerator value
typedef long long_enum;
static_assert(sizeof(long_enum) == 0x4);

// 8-bit flags container
typedef byte byte_flags;
static_assert(sizeof(byte_flags) == 0x1);

// 16-bit flags container
typedef word word_flags;
static_assert(sizeof(word_flags) == 0x2);

// 32-bit flags container
typedef dword dword_flags;
static_assert(sizeof(dword_flags) == 0x4);

// 64-bit flags container
typedef qword qword_flags;
static_assert(sizeof(qword_flags) == 0x8);

// 32-bit floating-point number ranging from 1.175494351e-38F to 3.402823466e+38F
typedef float real;
static_assert(sizeof(real) == 0x4);

typedef char utf8;

#define SIZEOF_BITS(value) 8 * sizeof(value)

const long CHAR_BITS = SIZEOF_BITS(char);
const long SHORT_BITS = SIZEOF_BITS(short);
const long LONG_BITS = SIZEOF_BITS(long);

#define FLAG(bit) (1 << (bit))
#define MASK(bit) ((1 << (bit)) - 1)
#define TEST_BIT(flags, bit) (((flags) & (1 << (bit))) != 0)
#define ALIGN(value, bit) (((value) & ~((1 << (bit)) - 1)) + (1 << (bit)))

#define NONE -1

long const k_vtable_pointer_size = sizeof(void*);

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

#ifdef _DEBUG
#define ASSERT_EXCEPTION(STATEMENT, IS_EXCEPTION, ...) \
if (!(STATEMENT) || !handle_assert_as_exception(#STATEMENT, __FILE__, __LINE__, IS_EXCEPTION)) \
{                                                                                              \
    display_assert(#STATEMENT, __FILE__, __LINE__, true);                                      \
    if (!is_debugger_present() && g_catch_exceptions)                                          \
        system_abort();                                                                        \
    else                                                                                       \
        system_exit();                                                                         \
}
#define ASSERT(STATEMENT, ...)  if (!(STATEMENT)) throw #STATEMENT // ASSERT_EXCEPTION((STATEMENT), true, __VA_ARGS__)
#else
#define ASSERT_EXCEPTION(...)
#define ASSERT(...)
#endif // _DEBUG

extern bool g_catch_exceptions;
extern void display_assert(char const* statement, char const* file, long line, bool is_assert);
extern bool handle_assert_as_exception(char const* statement, char const* file, long line, bool is_exception);

extern int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size);
extern void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size);
extern void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size);

extern long csstricmp(char const* s1, char const* s2);
//extern long csstrnicmp(char const* s1, char const* s2, dword size);
//extern char* csstristr(char const* s1, char const* s2);
extern char* csstrnzcpy(char* s1, char const* s2, dword size);
extern char* csstrnzcat(char* s1, char const* s2, dword size);
extern dword csstrnlen(char const* s, dword size);
extern char* csstrnupr(char* s, dword size);
extern char* csstrnlwr(char* s, dword size);
extern char const* csstrstr(char const* s1, char const* s2);
//extern char* csstrtok(char*, char const*, bool, struct csstrtok_data* data);
extern long cvsnzprintf(char* buffer, dword size, char const* format, va_list list);
extern char* csnzprintf(char* buffer, dword size, char const* format, ...);
extern char* csnzappendf(char* buffer, dword size, char const* format, ...);
extern bool string_is_not_empty(char const* s);

template<typename t_type, long count>
typename std::enable_if<!std::is_floating_point<t_type>::value, bool>::type
array_is_zeroed(t_type(&data)[count])
{
	for (long i = 0; i < count; i++)
	{
		if (data[i] != 0)
			return false;
	}

	return true;
}

template<typename t_type, long count>
void zero_array(t_type(&data)[count])
{
	csmemset(data, 0, sizeof(t_type) * count);
}

template<typename t_type, size_t k_count>
struct c_static_array
{
	t_type m_storage[k_count];

	t_type& operator[](long index)
	{
		ASSERT(VALID_INDEX(index, k_count));

		return m_storage[index];
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
	}
};

template<typename t_type, typename t_storage_type, size_t k_count>
struct c_flags
{
public:
	void set_raw_bits(t_storage_type raw_bits)
	{
		m_storage = raw_bits;
	}

	void set(t_type bit, bool enable)
	{
		if (bit < k_count)
		{
			if (enable)
				m_storage |= (1 << bit);
			else
				m_storage &= ~(1 << bit);
		}
	}

	bool valid_bit(t_type bit)
	{
		return VALID_INDEX(0, k_count);
	}

	bool valid_bit(t_type bit) const
	{
		return VALID_INDEX(0, k_count);
	}

	bool test(t_type bit)
	{
		ASSERT(valid_bit(bit));

		return TEST_BIT(m_storage, static_cast<t_storage_type>(bit));
	}

	bool test(t_type bit) const
	{
		ASSERT(valid_bit(bit));

		return TEST_BIT(m_storage, static_cast<t_storage_type>(bit));
	}

	bool operator==(t_type value)
	{
		return !!(m_storage & (1 << value));
	}

	template <class T>
	void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

template<size_t k_maximum_count, size_t k_storage_count = ((k_maximum_count - 1) >> 5) + 1>
struct c_static_flags
{
	bool test(long index)
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		return TEST_BIT(m_storage[index >> 5], (index & 31));
	}

	bool test(long index) const
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		return TEST_BIT(m_storage[index >> 5], (index & 31));
	}

	dword m_storage[k_storage_count];
};

template<typename t_type, typename t_storage_type, t_type k_minimum_value, t_type k_maximum_value_plus_one>
struct c_enum
{
public:
	template<typename T>
	bool operator==(T value) const
	{
		return m_storage == static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator!=(T value)
	{
		return m_storage != static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<(T value)
	{
		return m_storage < static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>(T value)
	{
		return m_storage > static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>=(T value)
	{
		return m_storage >= static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<=(T value)
	{
		return m_storage <= static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator++()
	{
		m_storage++;
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_storage);
	}

	t_type get()
	{
		return static_cast<t_type>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

// #TODO: find an actual home
struct s_cluster_reference
{
	char bsp_index;
	char cluster_index;
};
static_assert(sizeof(s_cluster_reference) == 0x2);

// #TODO: find an actual home
struct s_location
{
	s_cluster_reference cluster_reference;
	word leaf_index;
};
static_assert(sizeof(s_location) == 0x4);

template<long k_maximum_count>
struct c_static_string
{
public:
	static long const element_count = k_maximum_count;

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

		cvsnzprintf(m_string, k_maximum_count, format, list);

		va_end(list);

		return m_string;
	}

	char const* print_line(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		cvsnzprintf(m_string, k_maximum_count, format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	char const* vprint(char const* format, va_list list)
	{
		cvsnzprintf(m_string, k_maximum_count, format, list);

		return m_string;
	}

	char const* append_print(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_vprint(format, list);

		va_end(list);
		return result;
	}

	char const* append_print_line(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_vprint(format, list);
		append_line();

		va_end(list);
		return result;
	}

	char const* append_vprint(char const* format, va_list list)
	{
		dword current_length = length();

		ASSERT(format);
		ASSERT(current_length >= 0 && current_length < k_maximum_count);

		cvsnzprintf(m_string + current_length, k_maximum_count - current_length, format, list);

		return m_string;
	}

	void clear()
	{
		csmemset(m_string, 0, sizeof(m_string));
	}

	bool empty()
	{
		return m_string[0] == '\0';
	}

	char const* get_string() const
	{
		return m_string;
	}

	long length() const
	{
		return csstrnlen(m_string, k_maximum_count);
	}

	bool equals(char const* _string) const
	{
		ASSERT(_string);

		return csstrnlen(_string, k_maximum_count) == length() && csmemcmp(get_string(), _string, length()) == 0;
	}

	bool starts_with(char const* _string) const
	{
		ASSERT(_string);

		return csmemcmp(_string, get_string(), length()) == 0;
	}

	long next_index_of(char const* _string, long index) const
	{
		ASSERT(_string);

		long result = -1;

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

		s.set_bounded(get_string() + index, _length);

		return true;
	}

protected:
	char m_string[k_maximum_count];
};

extern char* tag_to_string(tag _tag, char* buffer);

struct c_string_id
{
public:
	char const* get_string();
	char const* get_string() const;
	long get_value() { return m_value; }

protected:
	string_id m_value;
};

extern __int64 make_int64(__int64 a, __int64 b);

// IDA
template<class T>
T rotate_left(T value, int count)
{
	const dword nbits = sizeof(T) * 8;

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

#define __ROL1__(value, count) rotate_left(static_cast<byte>(value), count)
#define __ROL2__(value, count) rotate_left(static_cast<word>(value), count)
#define __ROL4__(value, count) rotate_left(static_cast<dword>(value), count)
#define __ROL8__(value, count) rotate_left(static_cast<qword>(value), count)
#define __ROR1__(value, count) rotate_left(static_cast<byte>(value), -count)
#define __ROR2__(value, count) rotate_left(static_cast<word>(value), -count)
#define __ROR4__(value, count) rotate_left(static_cast<dword>(value), -count)
#define __ROR8__(value, count) rotate_left(static_cast<qword>(value), -count)

extern unsigned int address_from_pointer(void const* pointer);
extern void* pointer_from_address(unsigned int address);
extern unsigned int align_address(unsigned int address, long alignment_bits);
extern void* align_pointer(void* pointer, long alignment_bits);

extern real_argb_color const* const& global_real_argb_white;
extern real_argb_color const* const& global_real_argb_grey;
extern real_argb_color const* const& global_real_argb_black;
extern real_argb_color const* const& global_real_argb_red;
extern real_argb_color const* const& global_real_argb_green;
extern real_argb_color const* const& global_real_argb_blue;
extern real_argb_color const* const& global_real_argb_yellow;
extern real_argb_color const* const& global_real_argb_cyan;
extern real_argb_color const* const& global_real_argb_magenta;
extern real_argb_color const* const& global_real_argb_pink;
extern real_argb_color const* const& global_real_argb_lightblue;
extern real_argb_color const* const& global_real_argb_orange;
extern real_argb_color const* const& global_real_argb_purple;
extern real_argb_color const* const& global_real_argb_aqua;
extern real_argb_color const* const& global_real_argb_darkgreen;
extern real_argb_color const* const& global_real_argb_salmon;
extern real_argb_color const* const& global_real_argb_violet;
extern real_rgb_color const* const& global_real_rgb_white;
extern real_rgb_color const* const& global_real_rgb_grey;
extern real_rgb_color const* const& global_real_rgb_black;
extern real_rgb_color const* const& global_real_rgb_red;
extern real_rgb_color const* const& global_real_rgb_green;
extern real_rgb_color const* const& global_real_rgb_blue;
extern real_rgb_color const* const& global_real_rgb_yellow;
extern real_rgb_color const* const& global_real_rgb_cyan;
extern real_rgb_color const* const& global_real_rgb_magenta;
extern real_rgb_color const* const& global_real_rgb_pink;
extern real_rgb_color const* const& global_real_rgb_lightblue;
extern real_rgb_color const* const& global_real_rgb_orange;
extern real_rgb_color const* const& global_real_rgb_purple;
extern real_rgb_color const* const& global_real_rgb_aqua;
extern real_rgb_color const* const& global_real_rgb_darkgreen;
extern real_rgb_color const* const& global_real_rgb_salmon;
extern real_rgb_color const* const& global_real_rgb_violet;

