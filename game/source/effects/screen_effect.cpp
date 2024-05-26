#include "effects/screen_effect.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x006836A0, screen_effect_update);

long __cdecl screen_effect_new(long definition_index, long object_index, short node_index, real_point3d const* point, real_rectangle2d const* rectangle)
{
	return INVOKE(0x00683060, screen_effect_new, definition_index, object_index, node_index, point, rectangle);
}

void __cdecl screen_effect_update(real update_interval)
{
	HOOK_INVOKE(, screen_effect_update, update_interval);
}

