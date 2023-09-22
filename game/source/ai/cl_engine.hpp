#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct command_script_datum : s_datum_header
{
	byte __data[0x186];
};
static_assert(sizeof(command_script_datum) == 0x188);

