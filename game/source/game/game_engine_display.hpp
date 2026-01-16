#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_game_engine_render_globals
{
	uns8 fade_to_black_cache_latch_faliure_count[4];
};
COMPILE_ASSERT(sizeof(s_game_engine_render_globals) == 0x4);

extern bool g_fade_to_black_enabled;


extern void __cdecl game_engine_get_place_string(int32 place, c_static_wchar_string<256>* place_string);
extern void __cdecl game_engine_get_score_string(int32 score, class c_static_wchar_string<256>* score_string);
extern void __cdecl game_engine_get_team_name(int32 team, c_static_wchar_string<256>* team_name);
extern real32 __cdecl game_engine_get_user_fade_to_black_amount(int32 user_index);
extern real32 __cdecl game_engine_hud_get_fade(int32 user_index);
extern void __cdecl game_engine_render(int32 user_index);
extern void __cdecl game_engine_render_debug(int32 user_index);
extern void __cdecl game_engine_render_fade_to_black(int32 user_index);
extern void __cdecl game_engine_render_watermarks();
extern bool __cdecl game_engine_suppress_render_scene(int32 user_index);

