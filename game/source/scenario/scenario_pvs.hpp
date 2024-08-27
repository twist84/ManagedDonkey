#pragma once

#include "cseries/cseries.hpp"

struct s_scenario_pvs_row
{
	c_static_flags<1024> flags;
};
static_assert(sizeof(s_scenario_pvs_row) == 0x80);

