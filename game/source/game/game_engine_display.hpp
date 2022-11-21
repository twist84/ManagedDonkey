#pragma once

#include "text/unicode.hpp"

extern void __cdecl game_engine_get_place_string(long place, c_static_wchar_string<256>* place_string);
extern void __cdecl game_engine_get_score_string(long score, class c_static_wchar_string<256>* score_string);
extern void __cdecl game_engine_get_team_name(long team, c_static_wchar_string<256>* team_name);
extern void __cdecl game_engine_render_ui(long user_index);

