#include "interface/chud/chud_draw.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x0526A085, bool, chud_debug_crosshair);
REFERENCE_DECLARE(0x0526A087, bool, chud_debug_grid);

void __cdecl chud_debug_render()
{
	INVOKE(0x00AC8070, chud_debug_render);
}

