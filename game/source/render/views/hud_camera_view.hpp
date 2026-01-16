#pragma once

#include "cseries/cseries.hpp"

struct s_hud_camera_globals
{
	class c_player_window_data
	{
	public:
		byte __data[0x120];
	};
	COMPILE_ASSERT(sizeof(c_player_window_data) == 0x120);

	c_player_window_data player_windows[4];
};
COMPILE_ASSERT(sizeof(s_hud_camera_globals) == 0x480);

extern void __cdecl render_hud_camera_dispose();
extern void __cdecl render_hud_camera_dispose_from_old_map();
extern void __cdecl render_hud_camera_initialize();
extern void __cdecl render_hud_camera_initialize_for_new_map();

