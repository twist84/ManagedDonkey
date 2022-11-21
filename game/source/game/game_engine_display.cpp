#include "game/game_engine_display.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x006E5040, game_engine_render_ui);

void __cdecl game_engine_get_place_string(long place, c_static_wchar_string<256>* place_string)
{
	INVOKE(0x006E48C0, game_engine_get_place_string, place, place_string);
}

void __cdecl game_engine_get_score_string(long score, class c_static_wchar_string<256>* score_string)
{
	INVOKE(0x006E4960, game_engine_get_place_string, score, score_string);
}

void __cdecl game_engine_get_team_name(long team, c_static_wchar_string<256>* team_name)
{
	INVOKE(0x006E49A0, game_engine_get_team_name, team, team_name);
}

void __cdecl game_engine_render_ui(long user_index)
{
	HOOK_INVOKE(, game_engine_render_ui, user_index);
}

