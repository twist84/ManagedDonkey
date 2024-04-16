#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_game_engine_render_globals
{
	byte __unknown0[4];
};
static_assert(sizeof(s_game_engine_render_globals) == 0x4);

extern bool g_fade_to_black_enabled;

extern void __cdecl game_engine_get_place_string(long place, c_static_wchar_string<256>* place_string);
extern void __cdecl game_engine_get_score_string(long score, class c_static_wchar_string<256>* score_string);
extern void __cdecl game_engine_get_team_name(long team, c_static_wchar_string<256>* team_name);
extern real __cdecl game_engine_get_user_fade_to_black_amount(long user_index);
extern real __cdecl game_engine_hud_get_fade(long user_index);
extern void __cdecl game_engine_render_fade_to_black(long user_index);

