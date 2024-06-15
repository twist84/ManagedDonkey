#include "effects/player_effects.hpp"

void __cdecl player_effect_frame_update(real game_seconds_elapsed)
{
	INVOKE(0x00684670, player_effect_frame_update, game_seconds_elapsed);
}

void __cdecl player_effect_update()
{
	INVOKE(0x006850C0, player_effect_update);
}

