#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_system : s_datum_header
{
	short particle_tag_index;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;
	long __unknown24;
	long __unknown28;
	long __unknown2C;
	long __unknown30;
	long __unknown34;
	long __unknown38;
	long __unknown3C;
	long __unknown40;
	long __unknown44;
	long __unknown48;
	long __unknown4C;
	long __unknown50;
	long __unknown54;
};
static_assert(sizeof(c_particle_system) == 0x58);

