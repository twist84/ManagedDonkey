#pragma once

#include <type_traits>

#define bswap_uns16(value) bswap_impl<uns16>(value)
#define bswap_uns32(value) bswap_impl<uns32>(value)
#define bswap_uns64(value) bswap_impl<uns64>(value)
#define bswap_uns16_inplace(value) value = bswap_uns16(value)
#define bswap_uns32_inplace(value) value = bswap_uns32(value)
#define bswap_uns64_inplace(value) value = bswap_uns64(value)

enum e_byte_order
{
	_byte_order_little_endian = 0,
	_byte_order_big_endian,
	_byte_order_middle_endian,

	k_number_of_byte_orders,

	_byte_order_unknown = -1,
};

enum e_byte_swap_code_constant
{
	_1byte = 1,
	_2byte = -2,
	_4byte = -4,
	_8byte = -8,
	_begin_bs_array = -100,
	_end_bs_array = -101,
	_extern_bs_definition = -102,
	BYTE_SWAP_DEFINITION_SIGNATURE = 'bysw'
};

struct byte_swap_definition
{
	const char* name;
	int32 size;
	const int32* codes;
	uns32 signature;
	bool verified;
};
COMPILE_ASSERT(sizeof(byte_swap_definition) == 0x14);

extern void __cdecl _byte_swap_data(byte_swap_definition* definition, void* data, const int32* codes, int32* total_size_in_bytes, int32* total_size_in_codes);
extern int32 __cdecl byte_swap_codes_size(const char* name, const int32* codes);
extern void __cdecl byte_swap_data(byte_swap_definition* definition, void* data, int32 data_count);
extern void __cdecl byte_swap_data_explicit(const char* name, int32 size, const int32* codes, int32 data_count, void* data);
extern byte_swap_definition* __cdecl byte_swap_definition_verify(byte_swap_definition* definition);
extern int32 __cdecl byte_swap_get_runtime_byte_order();
extern void __cdecl byte_swap_memory(void* memory, int32 count, int32 code);

template <typename t_integer>
constexpr t_integer bswap_impl(t_integer value) noexcept
	requires std::is_integral_v<t_integer>
{
	using t_unsigned = std::make_unsigned_t<t_integer>;
	t_unsigned unsigned_value = static_cast<t_unsigned>(value);

	if constexpr (sizeof(t_integer) == sizeof(__int8))
	{
		return value;
	}
	else if constexpr (sizeof(t_integer) == sizeof(__int16))
	{
		unsigned_value =
			(unsigned_value >> 8) |
			(unsigned_value << 8);
	}
	else if constexpr (sizeof(t_integer) == sizeof(__int32))
	{
		unsigned_value =
			((unsigned_value & 0x000000FFu) << 24) |
			((unsigned_value & 0x0000FF00u) << 8)  |
			((unsigned_value & 0x00FF0000u) >> 8)  |
			((unsigned_value & 0xFF000000u) >> 24);
	}
	else if constexpr (sizeof(t_integer) == sizeof(__int64))
	{
		unsigned_value =
			((unsigned_value & 0x00000000000000FFull) << 56) |
			((unsigned_value & 0x000000000000FF00ull) << 40) |
			((unsigned_value & 0x0000000000FF0000ull) << 24) |
			((unsigned_value & 0x00000000FF000000ull) << 8)  |
			((unsigned_value & 0x000000FF00000000ull) >> 8)  |
			((unsigned_value & 0x0000FF0000000000ull) >> 24) |
			((unsigned_value & 0x00FF000000000000ull) >> 40) |
			((unsigned_value & 0xFF00000000000000ull) >> 56);
	}
	else
	{
		COMPILE_ASSERT(sizeof(t_integer) <= sizeof(__int64), "unsupported integer size");
	}

	return static_cast<t_integer>(unsigned_value);
}

template<typename t_integer, int32 t_integer_count>
constexpr void bswap_array(t_integer(&data)[t_integer_count]) noexcept
	requires std::is_integral_v<t_integer>
{
	for (int32 integer_index = 0; integer_index < t_integer_count; integer_index++)
	{
		data[integer_index] = bswap_impl<t_integer>(data[integer_index]);
	}
}

