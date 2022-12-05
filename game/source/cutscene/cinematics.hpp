#pragma once

#include "cseries/cseries.hpp"

extern bool __cdecl cinematic_in_progress();
extern void __cdecl cinematic_update();
extern void __cdecl cinematics_game_tick();
extern void __cdecl draw_quad(short_rectangle2d* rect, dword color);

