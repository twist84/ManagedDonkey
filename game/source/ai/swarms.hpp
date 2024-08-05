#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct swarm_datum :
	s_datum_header
{
	byte __data[0x32];
};
static_assert(sizeof(swarm_datum) == 0x34);

struct s_swarm_spawner
{
	struct s_unknown_struct_sizeof_78
	{
		byte __data[0x78];
	};

	s_unknown_struct_sizeof_78 __unknown0[5];
};
static_assert(sizeof(s_swarm_spawner) == 0x258);

struct s_spawner_globals
{
	short __unknown0;
};
static_assert(sizeof(s_spawner_globals) == 0x2);

extern void __cdecl swarm_delete(long swarm_index);

