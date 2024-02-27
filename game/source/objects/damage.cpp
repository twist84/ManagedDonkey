#include "objects/damage.hpp"

#include "game/cheats.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x00B4F3FC, compute_total_damage);
HOOK_DECLARE_CALL(0x00B53410, compute_total_damage);

bool debug_damage_radius = false;
bool debug_damage_this_event = false;
bool debug_damage_verbose = false;
bool debug_player_damage = false;
bool debug_damage = false;

void render_debug_object_damage()
{
	if (debug_damage_radius)
	{
		// #TODO: implement
	}

	if (debug_damage)
	{
		// #TODO: implement
	}
}

real __cdecl compute_total_damage(void* damage_data, void* damage_effect_definition, void const* damage_definition, long object_index, bool* a5)
{
	real result = INVOKE(0x00B4FB10, compute_total_damage, damage_data, damage_effect_definition, damage_definition, object_index, a5);

	if (cheat.chevy && TEST_MASK(FLAG(object_get_type(object_index)), _object_mask_vehicle))
		result = 0.0f;

	return result;
};

