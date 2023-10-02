#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct command_script_datum : s_datum_header
{
	byte __data[0x186];
};
static_assert(sizeof(command_script_datum) == 0x188);

struct s_scenario;
struct cs_script_data;
struct cs_point_set;

extern cs_script_data* cs_scenario_get_script_data(s_scenario* scenario);
extern cs_point_set* cs_get_point_set(long index);

