#pragma once

#include "cseries/cseries.hpp"

struct s_rasterizer_game_states
{
	bool motion_blur;
	bool atmosphere_fog;
	bool patchy_fog;
	bool weather;
	bool cinematic_motion_blur;

	byte __unknown5[0x27];

	bool autoexposure;

	byte __unknown44[0x1DB];
};
static_assert(sizeof(s_rasterizer_game_states) == 0x208);

extern bool render_debug_force_4x3_aspect_ratio;

struct _D3DCAPS9;
struct _D3DDISPLAYMODE;
struct _D3DPRESENT_PARAMETERS_;

extern _D3DCAPS9& g_global_device_caps;
extern _D3DDISPLAYMODE& g_global_display_mode;
extern _D3DPRESENT_PARAMETERS_& g_presentation_parameters;
extern _D3DPRESENT_PARAMETERS_& g_new_presentation_parameters;

extern long __cdecl get_current_thread_index();
extern _D3DCAPS9* __cdecl get_global_device_caps();
extern _D3DDISPLAYMODE* __cdecl get_global_display_mode();
extern _D3DPRESENT_PARAMETERS_* __cdecl get_presentation_parameters();
extern _D3DPRESENT_PARAMETERS_* __cdecl get_new_presentation_parameters();

