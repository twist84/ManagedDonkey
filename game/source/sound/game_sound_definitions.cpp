#include "sound/game_sound_definitions.hpp"

#include "cseries/cseries.hpp"

void __cdecl sound_location_set_forward(s_sound_location* sound_location, const real_vector3d* forward)
{
	INVOKE(0x006608B0, sound_location_set_forward, sound_location, forward);
}

