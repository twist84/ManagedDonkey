#include "objects/damage_owner.hpp"

REFERENCE_DECLARE(0x01949360, s_damage_owner const* const, global_damage_owner_unknown);

void __cdecl damage_compute_damage_owner_from_object_index(long object_index, s_damage_owner* damage_owner)
{
	INVOKE(0x00B770E0, damage_compute_damage_owner_from_object_index, object_index, damage_owner);
}

//.text:00B771C0 ; void __cdecl damage_owner_set(s_damage_owner*, long, long, c_game_team)
//.text:00B771F0 ; 
//.text:00B77200 ; 
//.text:00B77210 ; 
//.text:00B77220 ; 

