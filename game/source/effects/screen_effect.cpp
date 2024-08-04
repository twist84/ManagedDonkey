#include "effects/screen_effect.hpp"

#include "memory/module.hpp"
#include "scenario/scenario.hpp"

HOOK_DECLARE(0x006836A0, screen_effect_update);

long __cdecl screen_effect_new(long definition_index, long object_index, short node_index, real_point3d const* point, real_rectangle2d const* rectangle)
{
	return INVOKE(0x00683060, screen_effect_new, definition_index, object_index, node_index, point, rectangle);
}

void __cdecl screen_effect_update(real update_interval)
{
	HOOK_INVOKE(, screen_effect_update, update_interval);
}

void apply_global_screen_effect()
{
	if (struct scenario* scenario = global_scenario_get())
	{
		if (scenario->global_screen_effect.index != NONE)
			screen_effect_new(scenario->global_screen_effect.index, NONE, NONE, global_origin3d, global_zero_rectangle2d);
	}
}

