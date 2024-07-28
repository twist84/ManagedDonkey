#include "items/weapons.hpp"

#include "cache/cache_files.hpp"
#include "game/cheats.hpp"
#include "items/weapon_definitions.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00B61550, weapon_can_be_dual_wielded);
HOOK_DECLARE(0x00B62AC0, weapon_get_age);
HOOK_DECLARE(0x00B63C30, weapon_has_infinite_ammo);

bool __cdecl weapon_can_be_dual_wielded(long weapon_index)
{
	//return INVOKE(0x00B61550, weapon_can_be_dual_wielded, weapon_index);

	weapon_datum* weapon = (weapon_datum*)object_get_and_verify_type(weapon_index, _object_mask_weapon);
	struct weapon_definition* weapon_definition = (struct weapon_definition*)tag_get(WEAPON_TAG, weapon->definition_index);
	return weapon_definition->weapon.flags.test(_weapon_can_be_dual_wielded_bit) || weapon_definition->weapon.flags.test(_weapon_can_only_be_dual_wielded_bit);
}

real __cdecl weapon_get_age(long weapon_index)
{
	real result = 0.0f;
	HOOK_INVOKE(, weapon_get_age, weapon_index);

	if (cheat.bottomless_clip)
		result = 0.0f;

	return result;
}

real __cdecl weapon_get_field_of_view_change_time(long weapon_index)
{
	return INVOKE(0x00B62B80, weapon_get_field_of_view_change_time, weapon_index);
}

bool __cdecl weapon_has_infinite_ammo(long weapon_index)
{
	bool result = false;
	HOOK_INVOKE(result =, weapon_has_infinite_ammo, weapon_index);

	if (cheat.infinite_ammo)
		result = true;

	return result;
}

