#pragma once

#include "render/camera_fx_settings.hpp"

struct s_render_game_state
{
	struct s_player_window
	{
		c_camera_fx_settings camera_fx_settings;
		c_camera_fx_values camera_fx_values;
	};
	static_assert(sizeof(s_player_window) == 0x360);

	s_player_window player_windows[4];
};
static_assert(sizeof(s_render_game_state) == 0xD80);

