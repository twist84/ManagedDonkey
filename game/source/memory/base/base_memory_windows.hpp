#pragma once

struct s_base_memory_information
{
	long free;
	long total;
	long pad[2];
};
static_assert(sizeof(s_base_memory_information) == 0x10);

extern void __cdecl base_memory_information_get(s_base_memory_information* out_information);

