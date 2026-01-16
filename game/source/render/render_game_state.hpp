#pragma once

#include "render/camera_fx_settings.hpp"

struct s_render_game_state
{
public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();

	struct s_player_window
	{
		c_camera_fx_settings m_camera_fx_settings;
		c_camera_fx_values m_camera_fx_values;
	};
	COMPILE_ASSERT(sizeof(s_player_window) == 0x360);

	s_player_window player_window[MAXIMUM_PLAYER_WINDOWS];
};
COMPILE_ASSERT(sizeof(s_render_game_state) == 0xD80);

extern void __cdecl __tls_set_g_render_game_globals_allocator(void* new_address);
extern s_render_game_state::s_player_window* __cdecl get_render_player_window_game_state(int32 player_window_index);

