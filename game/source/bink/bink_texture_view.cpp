#include "bink/bink_texture_view.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x00836E40, bink_texture_view_update);

void __cdecl bink_texture_view_update(int32 player_index)
{
}

