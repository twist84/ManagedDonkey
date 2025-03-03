#include "render/render_debug_commands.hpp"

#include "cseries/cseries.hpp"
#include "main/main_time.hpp"

REFERENCE_DECLARE(0x0191C920, bool, render_debug_toggle_default_static_lighting);
REFERENCE_DECLARE(0x0191C921, bool, render_debug_toggle_default_dynamic_lighting);
REFERENCE_DECLARE(0x0191C922, bool, render_debug_toggle_default_sfx);

//bool render_debug_toggle_default_static_lighting = true;
//bool render_debug_toggle_default_dynamic_lighting = true;
//bool render_debug_toggle_default_sfx = true;
long render_debug_toggle_default_lightmaps_texaccum = 0;
bool render_debug_infinite_framerate = false;
bool g_last_render_debug_infinite_framerate = false;

bool render_infinite_framerate_setup(bool on, bool last)
{
	if (on != last)
	{
		debug_disable_frame_rate_throttle = on;
		display_framerate = true;
	}
	return on;
}

void render_update_debug_commands()
{
	g_last_render_debug_infinite_framerate = render_infinite_framerate_setup(render_debug_infinite_framerate, g_last_render_debug_infinite_framerate);
}

