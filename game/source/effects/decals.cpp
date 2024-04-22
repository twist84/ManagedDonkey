#include "effects/decals.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x006947FE, sub_6948C0);

bool disable_sub_6948C0 = true;

void __cdecl sub_6948C0(long a1)
{
	if (disable_sub_6948C0)
		return;

	INVOKE(0x006948C0, sub_6948C0, a1);
}

