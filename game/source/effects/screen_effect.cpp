#include "effects/screen_effect.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x006836A0, screen_effect_update);

void __cdecl screen_effect_update(real update_interval)
{
	HOOK_INVOKE(, screen_effect_update, update_interval);
}

