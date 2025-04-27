#pragma once

#include "cseries/cseries.hpp"
#include "objects/widgets/antenna.hpp"
#include "objects/widgets/cloth.hpp"
#include "objects/widgets/leaf_system.hpp"
#include "objects/widgets/muffins.hpp"

struct widget_datum :
	s_datum_header
{
	short type;
	long type_datum_index;
	long next_widget_index;
};
static_assert(sizeof(widget_datum) == 0xC);

extern void __cdecl widgets_render_unattached(long entry_point);
extern void __cdecl widgets_update(real32 game_seconds_elapsed);

