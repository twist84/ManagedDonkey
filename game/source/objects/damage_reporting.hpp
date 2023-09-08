#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct s_damage_reporting_info
{
	c_enum<e_damage_reporting_type, word, _damage_reporting_type_unknown, k_damage_reporting_type_count> type;
	byte modifier;
	byte_flags recyling_flags;
};
static_assert(sizeof(s_damage_reporting_info) == 0x4);

