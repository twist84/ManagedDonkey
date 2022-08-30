#pragma once

#include "cseries/integer_math.hpp"
#include "cseries/real_math.hpp"

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
typedef unsigned long string_id;
static_assert(sizeof(string_id) == 0x4);

enum : string_id
{
	_string_id_invalid = 0
};

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

// 32-bit floating-point number ranging from 1.175494351e-38F to 3.402823466e+38F
typedef float real;
static_assert(sizeof(real) == 0x4);

template<typename t_type, size_t k_count>
struct c_static_array
{
	t_type m_storage[k_count];

	t_type operator[](long index)
	{
		if (index < k_count)
			return m_storage[index];
		return t_type(0);
	}
};

template<typename t_type, typename t_storage_type, size_t k_count>
struct c_flags
{
public:
	inline void set_raw_bits(t_storage_type raw_bits)
	{
		m_storage = raw_bits;
	}

	inline void set(t_type bit, bool enable)
	{
		if (bit < k_count)
		{
			if (enable)
				m_storage |= (1 << bit);
			else
				m_storage &= ~(1 << bit);
		}
	}

	inline bool operator==(t_type value)
	{
		return !!(m_storage & (1 << value));
	}

	template <class T>
	inline void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template <class T>
	inline operator T () const
	{
		return static_cast<T>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

template<size_t k_bit_count>
struct c_static_flags
{
	dword m_storage[(k_bit_count / 8) / sizeof(dword)];
};

template<typename t_type, typename t_storage_type, size_t k_count>
struct c_enum
{
public:
	template<typename T>
	inline bool operator==(T value)
	{
		return m_storage == static_cast<t_storage_type>(value);
	}

	template<typename T>
	inline bool operator!=(T value)
	{
		return m_storage != static_cast<t_storage_type>(value);
	}

	template<typename T>
	inline bool operator<(T value)
	{
		return m_storage < static_cast<t_storage_type>(value);
	}

	template<typename T>
	inline bool operator>(T value)
	{
		return m_storage > static_cast<t_storage_type>(value);
	}

	template<typename T>
	inline bool operator>=(T value)
	{
		return m_storage >= static_cast<t_storage_type>(value);
	}

	template<typename T>
	inline void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template <class T>
	inline operator T () const
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