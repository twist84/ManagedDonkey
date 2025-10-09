#include "effects/screen_effect.hpp"

#include "memory/module.hpp"
#include "scenario/scenario.hpp"

HOOK_DECLARE(0x006836A0, screen_effect_update);

void __cdecl screen_effect_dispose()
{
	INVOKE(0x00682EF0, screen_effect_dispose);
}

void __cdecl screen_effect_dispose_from_old_map()
{
	INVOKE(0x00682F20, screen_effect_dispose_from_old_map);
}

void __cdecl screen_effect_initialize()
{
	INVOKE(0x00683010, screen_effect_initialize);
}

void __cdecl screen_effect_initialize_for_new_map()
{
	INVOKE(0x00683040, screen_effect_initialize_for_new_map);
}

int32 __cdecl screen_effect_new(int32 definition_index, int32 object_index, int16 node_index, const real_point3d* point, const real_rectangle2d* rectangle)
{
	return INVOKE(0x00683060, screen_effect_new, definition_index, object_index, node_index, point, rectangle);
}

void __cdecl screen_effect_sample(const real_point3d* point, const real_vector3d* vector, s_screen_effect_settings* settings, s_screen_effect_shader_sample_result* result, int32 user_index)
{
	INVOKE(0x00683190, screen_effect_sample, point, vector, settings, result, user_index);
}

void __cdecl screen_effect_update(real32 update_interval)
{
	HOOK_INVOKE(, screen_effect_update, update_interval);
}

void apply_global_screen_effect()
{
	struct scenario* scenario = global_scenario_get();
	if (scenario && scenario->global_screen_effect.index != NONE)
	{
		screen_effect_new(scenario->global_screen_effect.index, NONE, NONE, global_origin3d, global_zero_rectangle2d);
	}
}

