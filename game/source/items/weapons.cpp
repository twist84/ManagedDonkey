#include "items/weapons.hpp"

real __cdecl weapon_get_age(long weapon_index)
{
	return INVOKE(0x00B62AC0, weapon_get_age, weapon_index);
}

bool __cdecl weapon_has_infinite_ammo(long weapon_index)
{
	return INVOKE(0x00B63C30, weapon_has_infinite_ammo, weapon_index);
}
