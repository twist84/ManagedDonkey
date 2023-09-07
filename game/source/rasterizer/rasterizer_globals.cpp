#include "rasterizer/rasterizer_globals.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x050DDA00, s_rasterizer_globals, rasterizer_globals);

REFERENCE_DECLARE(0x019106C0, long, render_globals_width);
REFERENCE_DECLARE(0x019106C8, long, render_globals_height);

bool rasterizer_get_is_widescreen()
{
	return (render_globals_width / render_globals_height) > real(720 / 480);
}

