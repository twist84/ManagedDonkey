#include "ai/ai.hpp"

void __cdecl ai_handle_bump(long object_index, long bump_object_index, vector3d const* linear_velocity)
{
	INVOKE(0x014328E0, ai_handle_bump, object_index, bump_object_index, linear_velocity);
}

