#include "cutscene/cinematics.hpp"

bool __cdecl cinematic_in_progress()
{
	return INVOKE(0x0067D270, cinematic_in_progress);
}

void __cdecl cinematic_update()
{
	return INVOKE(0x0067EDF0, cinematic_update);
}

void __cdecl cinematics_game_tick()
{
	return INVOKE(0x0067F080, cinematics_game_tick);
}

void __cdecl draw_quad(short_rectangle2d* rect, dword color)
{
	return INVOKE(0x0067F200, draw_quad, rect, color);
}

