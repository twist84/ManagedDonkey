#pragma once

#include "tag_files/tag_groups.hpp"

struct geometry_material
{
	s_tag_reference render_method;
	s_tag_block properties;
	int32 imported_material_index;
	uns8 breakable_surface_set_index;
	byte pad[1];
	uns16 runtime_flags;
};
COMPILE_ASSERT(sizeof(geometry_material) == 0x24);

