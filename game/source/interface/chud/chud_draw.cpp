#include "interface/chud/chud_draw.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x0191CC64, bool, chud_enabled);
REFERENCE_DECLARE(0x052604C1, bool, chud_debug_messages);
REFERENCE_DECLARE(0x0526A085, bool, chud_debug_crosshair);
REFERENCE_DECLARE(0x0526A087, bool, chud_debug_grid);

void __cdecl chud_debug_render()
{
	INVOKE(0x00AC8070, chud_debug_render);
}

