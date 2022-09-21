#include "items/weapons.hpp"

#include "game/cheats.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00B62AC0, weapon_get_age);
HOOK_DECLARE(0x00B63C30, weapon_has_infinite_ammo);

real __cdecl weapon_get_age(long weapon_index)
{
	real result = 0.0f;
	HOOK_INVOKE(, weapon_get_age, weapon_index);

	if (cheat.bottomless_clip)
		result = 0.0f;

	return result;
}

bool __cdecl weapon_has_infinite_ammo(long weapon_index)
{
	bool result = false;
	HOOK_INVOKE(result =, weapon_has_infinite_ammo, weapon_index);

	if (cheat.infinite_ammo)
		result = true;

	return result;
}
