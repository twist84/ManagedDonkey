#include "memory/byte_swapping.hpp"

#include "cseries/cseries.hpp"

//e_byte_order __cdecl byte_swap_get_runtime_byte_order()
long __cdecl byte_swap_get_runtime_byte_order()
{
	return INVOKE(0x006015A0, byte_swap_get_runtime_byte_order);
}

