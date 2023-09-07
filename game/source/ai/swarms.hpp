#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct swarm_datum : s_datum_header
{
	byte __data[0x32];
};
static_assert(sizeof(swarm_datum) == 0x34);

struct s_swarm_spawner
{
	byte __data[0x258];
};
static_assert(sizeof(s_swarm_spawner) == 0x258);

struct s_spawner_globals
{
	short __unknown0;
};
static_assert(sizeof(s_spawner_globals) == 0x2);

