#include "ai/sector.hpp"

#include "ai/sector_definitions.hpp"
#include "render/render_debug.hpp"

pathfinding_data const* __cdecl pathfinding_data_get(short structure_bsp_index)
{
	return INVOKE(0x01445440, pathfinding_data_get, structure_bsp_index);
}

