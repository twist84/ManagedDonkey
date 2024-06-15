#pragma once

#include "cseries/cseries.hpp"
#include "objects/widgets/antenna.hpp"
#include "objects/widgets/cloth.hpp"
#include "objects/widgets/leaf_system.hpp"
#include "objects/widgets/muffins.hpp"
#include "memory/data.hpp"

struct widget_datum :
	s_datum_header
{
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(widget_datum) == 0xC);

extern void __cdecl widgets_update(real game_seconds_elapsed);

