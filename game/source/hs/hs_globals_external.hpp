#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"

struct hs_global_external
{
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;
	short __unknown2;
	void* value;
};
static_assert(sizeof(hs_global_external) == 0x8);

short const k_hs_external_global_count = 1315;
extern hs_global_external* (&hs_external_globals)[k_hs_external_global_count];

