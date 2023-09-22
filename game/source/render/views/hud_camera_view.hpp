#pragma once

#include "cseries/cseries.hpp"

struct s_hud_camera_globals
{
	struct c_player_window_data
	{
		byte __data[0x120];
	};
	static_assert(sizeof(c_player_window_data) == 0x120);

	c_player_window_data player_windows[4];
};
static_assert(sizeof(s_hud_camera_globals) == 0x480);

