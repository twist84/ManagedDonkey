#include "render/render_game_state.hpp"

#include "memory/thread_local.hpp"

void __cdecl __tls_set_g_render_game_globals_allocator(void* new_address)
{
	//INVOKE(0x00A3E640, __tls_set_g_render_game_globals_allocator, new_address);

	TLS_DATA_GET_VALUE_REFERENCE(render_game_globals);
	render_game_globals = (s_render_game_state*)new_address;
}

void __cdecl s_render_game_state::dispose()
{
	//INVOKE(0x00A3E6D0, s_render_game_state::dispose);
}

void __cdecl s_render_game_state::dispose_from_old_map()
{
	//INVOKE(0x00A3E6E0, s_render_game_state::dispose_from_old_map);
}

s_render_game_state::s_player_window* __cdecl get_render_player_window_game_state(long player_window_index)
{
	//return INVOKE(0x00A3E720, get_render_player_window_game_state, player_window_index);

	TLS_DATA_GET_VALUE_REFERENCE(render_game_globals);
	ASSERT(render_game_globals);
	ASSERT(VALID_INDEX(player_window_index, MAXIMUM_PLAYER_WINDOWS));
	return &render_game_globals->player_window[player_window_index];
}

void __cdecl s_render_game_state::initialize()
{
	//INVOKE(0x00A3E750, s_render_game_state::initialize);

	TLS_DATA_GET_VALUE_REFERENCE(render_game_globals);
	render_game_globals = (s_render_game_state*)g_render_game_globals_allocator.allocate(sizeof(s_render_game_state), "render game globals");
}

void __cdecl s_render_game_state::initialize_for_new_map()
{
	//INVOKE(0x00A3E7B0, s_render_game_state::initialize_for_new_map);

	TLS_DATA_GET_VALUE_REFERENCE(render_game_globals);
	for (long player_window_index = 0; player_window_index < MAXIMUM_PLAYER_WINDOWS; player_window_index++)
	{
		s_render_game_state::s_player_window* player_window = get_render_player_window_game_state(player_window_index);
		player_window->m_camera_fx_settings.set_defaults(true);
		player_window->m_camera_fx_values.set(&player_window->m_camera_fx_settings);
	}
}

