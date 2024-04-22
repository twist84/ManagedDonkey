#include "effects/effects.hpp"

bool debug_damage_effects = false;

void render_debug_damage_effects()
{
	if (debug_damage_effects)
	{

	}
}

bool __cdecl dangerous_effects_near_player()
{
    return INVOKE(0x005B4DA0, dangerous_effects_near_player);
}

