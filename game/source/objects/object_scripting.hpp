#pragma once

#include "cseries/cseries.hpp"

struct s_object_scripting_state
{
	byte __data[0x304];
};
static_assert(sizeof(s_object_scripting_state) == 0x304);

