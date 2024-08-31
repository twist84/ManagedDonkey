#include "render/render_game_state.hpp"

void __cdecl s_render_game_state::dispose()
{
	INVOKE(0x00A3E6D0, dispose);
}

void __cdecl s_render_game_state::dispose_from_old_map()
{
	INVOKE(0x00A3E6E0, dispose_from_old_map);
}

void __cdecl s_render_game_state::initialize()
{
	INVOKE(0x00A3E750, initialize);
}

void __cdecl s_render_game_state::initialize_for_new_map()
{
	INVOKE(0x00A3E7B0, initialize_for_new_map);
}

