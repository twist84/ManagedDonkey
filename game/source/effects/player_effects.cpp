#include "effects/player_effects.hpp"

//.text:00683A30 ; 
//.text:00683B30 ; 
//.text:00684600 ; 

void __cdecl player_effect_dispose()
{
	INVOKE(0x00684650, player_effect_dispose);
}

void __cdecl player_effect_dispose_from_old_map()
{
	INVOKE(0x00684660, player_effect_dispose_from_old_map);
}

void __cdecl player_effect_frame_update(real game_seconds_elapsed)
{
	INVOKE(0x00684670, player_effect_frame_update, game_seconds_elapsed);
}

void __cdecl player_effect_initialize()
{
	INVOKE(0x00684A00, player_effect_initialize);
}

void __cdecl player_effect_initialize_for_new_map()
{
	INVOKE(0x00684A60, player_effect_initialize_for_new_map);
}

//.text:00684AC0 ; 

void __cdecl player_effect_render(long user_index)
{
	INVOKE(0x00684B20, player_effect_render, user_index);
}

//.text:00684D40 ; void __cdecl player_effect_screen_flash(long, s_damage_response_screen_flash_definition*, real)
//.text:00684D90 ; void __cdecl player_effect_screen_flash_stop(long)
//.text:00684DF0 ; void __cdecl player_effect_start(long, long, long, s_damage_owner const*, vector3d const*, real, real, bool)
//.text:00684E30 ; void __cdecl player_effect_start_internal(long, long, long, s_damage_owner const*, vector3d const*, real, real, bool)
//.text:00684F30 ; void __cdecl player_effect_start_response(long, long, long, s_damage_owner const*, vector3d const*, real, real, bool)

void __cdecl player_effect_update()
{
	INVOKE(0x006850C0, player_effect_update);
}

//.text:00685260 ; 
//.text:00685790 ; 
//.text:00685860 ; 

