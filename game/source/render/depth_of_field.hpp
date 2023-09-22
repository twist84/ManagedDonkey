#pragma once

#include "cseries/cseries.hpp"

struct s_depth_of_field
{
	bool enable;

	byte __unknown1[0x3]; // pad?

	real __unknown4;
	real __unknown8;
	real __unknownC;

	real intensity;

	real __unknown14;
	real __unknown18;
	real __unknown1C;
	real __unknown20;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
	real __unknown30;
	real __unknown34;
	real __unknown38;
	real __unknown3C;
};
static_assert(sizeof(s_depth_of_field) == 0x40);

