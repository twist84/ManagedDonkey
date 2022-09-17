#include "items/weapons.hpp"

#include "game/cheats.hpp"

real __cdecl weapon_get_age(long weapon_index)
{
	if (cheat.bottomless_clip)
		return 0.0f;

	return INVOKE(0x00B62AC0, weapon_get_age, weapon_index);
}

bool __cdecl weapon_has_infinite_ammo(long weapon_index)
{
	if (cheat.infinite_ammo)
		return true;

	return INVOKE(0x00B63C30, weapon_has_infinite_ammo, weapon_index);
}
