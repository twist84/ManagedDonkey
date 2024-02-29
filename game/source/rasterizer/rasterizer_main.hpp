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

extern long __cdecl get_current_thread_index();

