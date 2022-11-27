#include "bink/bink_playback.hpp"

#include "cseries/cseries.hpp"

bool __cdecl bink_playback_active()
{
	return INVOKE(0x00616910, bink_playback_active);
}

