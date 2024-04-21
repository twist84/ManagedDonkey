#include "items/projectiles.hpp"

bool debug_projectiles = false;

bool __cdecl dangerous_projectiles_near_player(long* out_projectile_index)
{
	return INVOKE(0x00B77330, dangerous_projectiles_near_player, out_projectile_index);
}

void render_debug_projectiles()
{
	if (debug_projectiles)
	{
		// #TODO: implement
	}
}

