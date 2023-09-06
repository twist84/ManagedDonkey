#pragma once

#include "cseries/cseries.hpp"

struct c_hue_saturation_control
{
	dword graphics_override;
	real saturation;
	real color;
	real __unknownC;
	real __unknown10;
};
static_assert(sizeof(c_hue_saturation_control) == 0x14);

