#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"

struct hs_global_external
{
	int16 type; // e_hs_type
	void* value;
};
static_assert(sizeof(hs_global_external) == 0x8);

struct hs_global_external_debug
{
	const char* name;
	int16 type; // e_hs_type
	void* value;
};
static_assert(sizeof(hs_global_external_debug) == 0xC);

int16 const k_hs_external_global_count = 1315;
extern hs_global_external* (&hs_external_globals)[k_hs_external_global_count];

extern const char* const hs_external_globals_names[k_hs_external_global_count];

extern hs_global_external_debug* const hs_external_globals_debug[];
extern int16 const k_hs_external_global_debug_count;

