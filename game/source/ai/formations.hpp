#pragma once

#include "cseries/cseries.hpp"

struct formation_datum :
	s_datum_header
{
	byte __data[0x292];
};
COMPILE_ASSERT(sizeof(formation_datum) == 0x294);

extern void __cdecl formations_dispose();
extern void __cdecl formations_dispose_from_old_map();
extern void __cdecl formations_initialize();
extern void __cdecl formations_initialize_for_new_map();
extern void __cdecl formations_update();

