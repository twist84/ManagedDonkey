#pragma once

#include <intrin.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)

#define bswap_word(value) _byteswap_ushort(value)
#define bswap_dword(value) _byteswap_ulong(value)
#define bswap_qword(value) _byteswap_uint64(value)
#define bswap_word_inplace(value) value = bswap_word(value)
#define bswap_dword_inplace(value) value = bswap_dword(value)
#define bswap_qword_inplace(value) value = bswap_qword(value)

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
	char const* name;
	long size;
	long const* codes;
	unsigned long signature;
	bool verified;
};
static_assert(sizeof(byte_swap_definition) == 0x14);

extern void __cdecl _byte_swap_data(byte_swap_definition* definition, void* data, long const* codes, long* total_size_in_bytes, long* total_size_in_codes);
extern long __cdecl byte_swap_codes_size(char const* name, long const* codes);
extern void __cdecl byte_swap_data(byte_swap_definition* definition, void* data, long data_count);
extern void __cdecl byte_swap_data_explicit(char const* name, long size, long const* codes, long data_count, void* data);
extern byte_swap_definition* __cdecl byte_swap_definition_verify(byte_swap_definition* definition);
extern long __cdecl byte_swap_get_runtime_byte_order();
extern void __cdecl byte_swap_memory(void* memory, long count, long code);

