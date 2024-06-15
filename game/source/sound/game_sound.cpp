#include "sound/game_sound.hpp"

void __cdecl game_sound_process_update_messages()
{
	INVOKE(0x005D9630, game_sound_process_update_messages);
}

void __cdecl game_sound_update(real game_seconds_elapsed)
{
	INVOKE(0x005D9BA0, game_sound_update, game_seconds_elapsed);
}

