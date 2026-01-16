#pragma once

#include <intrin.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)

#define bswap_uns16(value) _byteswap_ushort(value)
#define bswap_uns32(value) _byteswap_ulong(value)
#define bswap_uns64(value) _byteswap_uint64(value)
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

