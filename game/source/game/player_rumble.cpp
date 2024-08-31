#include "game/player_rumble.hpp"

//.text:00615220 ; 
//.text:00615240 ; 
//.text:00615250 ; void __cdecl __tls_set_g_rumble_globals_allocator(void*)
//.text:00615270 ; 
//.text:006152B0 ; 
//.text:006152E0 ; 
//.text:00615310 ; 
//.text:00615350 ; 

void __cdecl rumble_clear_all_now()
{
	INVOKE(0x006155D0, rumble_clear_all_now);
}

void __cdecl rumble_dispose()
{
	INVOKE(0x006156E0, rumble_dispose);
}

void __cdecl rumble_dispose_from_old_map()
{
	INVOKE(0x006156F0, rumble_dispose_from_old_map);
}

void __cdecl rumble_initialize()
{
	INVOKE(0x00615700, rumble_initialize);
}

void __cdecl rumble_initialize_for_new_map()
{
	INVOKE(0x00615760, rumble_initialize_for_new_map);
}

//.text:00615860 ; 
//.text:00615940 ; void __cdecl rumble_player_clear_scripted_effect()
//.text:00615980 ; 
//.text:006159C0 ; void __cdecl rumble_player_impulse(long, long, long, real)
//.text:00615AA0 ; 
//.text:00615AD0 ; 
//.text:00615B10 ; void __cdecl rumble_player_set_scripted_values_for_player(long, real, real)

void __cdecl rumble_update(real world_seconds_elapsed)
{
	INVOKE(0x00615BA0, rumble_update, world_seconds_elapsed);
}

//.text:00615D00 ; 
//.text:00615D20 ; 

