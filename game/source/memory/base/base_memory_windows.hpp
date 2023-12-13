#pragma once

struct s_base_memory_information
{
	long __unknown0;
	long __unknown4;
	long __unknown8;
	long __unknownC;
};
static_assert(sizeof(s_base_memory_information) == 0x10);

extern void __cdecl base_memory_information_get(s_base_memory_information* out_information);

