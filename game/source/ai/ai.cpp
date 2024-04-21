#include "ai/ai.hpp"

bool __cdecl ai_enemies_can_see_player(long* out_unit_index)
{
	return INVOKE(0x014316D0, ai_enemies_can_see_player, out_unit_index);
}

void __cdecl ai_handle_bump(long object_index, long bump_object_index, vector3d const* linear_velocity)
{
	INVOKE(0x014328E0, ai_handle_bump, object_index, bump_object_index, linear_velocity);
}

