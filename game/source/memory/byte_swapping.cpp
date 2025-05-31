#include "memory/byte_swapping.hpp"

#include "cseries/cseries.hpp"

void __cdecl _byte_swap_data(byte_swap_definition* definition, void* data, const int32* codes, int32* total_size_in_bytes, int32* total_size_in_codes)
{
	INVOKE(0x00600FD0, _byte_swap_data, definition, data, codes, total_size_in_bytes, total_size_in_codes);
}

int32 __cdecl byte_swap_codes_size(const char* name, const int32* codes)
{
	return INVOKE(0x00601260, byte_swap_codes_size, name, codes);
}

void __cdecl byte_swap_data(byte_swap_definition* definition, void* data, int32 data_count)
{
	INVOKE(0x006012A0, byte_swap_data, definition, data, data_count);
}

void __cdecl byte_swap_data_explicit(const char* name, int32 size, const int32* codes, int32 data_count, void* data)
{
	INVOKE(0x00601550, byte_swap_data_explicit, name, size, codes, data_count, data);
}

byte_swap_definition* __cdecl byte_swap_definition_verify(byte_swap_definition* definition)
{
	return INVOKE(0x00601590, byte_swap_definition_verify, definition);
}

//e_byte_order __cdecl byte_swap_get_runtime_byte_order()
int32 __cdecl byte_swap_get_runtime_byte_order()
{
	return INVOKE(0x006015A0, byte_swap_get_runtime_byte_order);
}

void __cdecl byte_swap_memory(void* memory, int32 count, int32 code)
{
	INVOKE(0x006015B0, byte_swap_memory, memory, count, code);
}

