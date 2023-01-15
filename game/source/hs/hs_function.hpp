#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"

struct hs_function_definition
{
	c_enum<e_hs_type, short, k_hs_type_count> return_type;
	short parse;
	short(__cdecl* evaluate)(long, long, bool);
	const char* usage;

	short parameter_count;

#pragma warning(push)
#pragma warning(disable : 4200)
	c_enum<e_hs_type, short, k_hs_type_count> parameters[];
#pragma warning(pop)
};
static_assert(sizeof(hs_function_definition) == 0x10);

long const hs_function_table_count = 1675;
extern hs_function_definition* (&hs_function_table)[hs_function_table_count];

