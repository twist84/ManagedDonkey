#pragma once

#include "cseries/cseries.hpp"

struct s_damage_reporting_info
{
	c_enum<e_damage_reporting_type, uns16, _damage_reporting_type_unknown, k_damage_reporting_type_count> type;
	uns8 modifier;
	uns8 recycling_flag;
};
COMPILE_ASSERT(sizeof(s_damage_reporting_info) == 0x4);

