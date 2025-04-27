#pragma once

extern bool& render_debug_toggle_default_static_lighting;
extern bool& render_debug_toggle_default_dynamic_lighting;
extern bool& render_debug_toggle_default_sfx;
extern int32 render_debug_toggle_default_lightmaps_texaccum;
extern bool render_debug_infinite_framerate;
extern bool g_last_render_debug_infinite_framerate;

extern bool render_infinite_framerate_setup(bool on, bool last);
extern void render_update_debug_commands();

