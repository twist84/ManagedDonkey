#pragma once

#include "cseries/cseries.hpp"

struct s_object_scehdule_globals
{
	byte __data[0x27C];
};
static_assert(sizeof(s_object_scehdule_globals) == 0x27C);

extern void __cdecl object_scheduler_update();

