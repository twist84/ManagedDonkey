#pragma once

#include "cseries/cseries.hpp"
#include "objects/widgets/antenna.hpp"
#include "objects/widgets/cloth.hpp"
#include "objects/widgets/leaf_system.hpp"
#include "objects/widgets/muffins.hpp"

struct widget_datum :
	s_datum_header
{
	int16 type;
	int32 type_datum_index;
	int32 next_widget_index;
};
COMPILE_ASSERT(sizeof(widget_datum) == 0xC);

extern void __cdecl widgets_render_unattached(int32 entry_point);
extern void __cdecl widgets_update(real32 game_seconds_elapsed);

