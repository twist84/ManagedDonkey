#include "game/game_allegiance.hpp"

//.text:0066FAD0 ; 
//.text:0066FAF0 ; 
//.text:0066FB00 ; void __cdecl __tls_set_g_game_allegiance_globals_allocator(void*)
//.text:0066FB20 ; t_restricted_allocation_manager<3, 0, 0, &void __cdecl __tls_set_g_game_allegiance_globals_allocator(void*)>::allocate
//.text:0066FB60 ; t_restricted_allocation_manager<3, 0, 0, &void __cdecl __tls_set_g_game_allegiance_globals_allocator(void*)>::deallocate
//.text:0066FB90 ; t_restricted_allocation_manager<3, 0, 0, &void __cdecl __tls_set_g_game_allegiance_globals_allocator(void*)>::free_memory

bool __cdecl game_allegiance_betrayal_exists()
{
    return INVOKE(0x0066FBC0, game_allegiance_betrayal_exists);
}

//.text:0066FCA0 ; void __cdecl game_allegiance_broken(game_allegiance*, bool, bool, bool)
//.text:0066FE00 ; void __cdecl game_allegiance_create(e_game_team, bool, e_game_team, bool, int16, int16)

void __cdecl game_allegiance_dispose()
{
    INVOKE(0x0066FFC0, game_allegiance_dispose);
}

void __cdecl game_allegiance_dispose_from_old_map()
{
    INVOKE(0x0066FFD0, game_allegiance_dispose_from_old_map);
}

//.text:0066FFE0 ; int16 __cdecl game_allegiance_get_incidents(e_game_team, e_game_team, int16*)
//.text:00670070 ; s_game_allegiance_globals const* __cdecl game_allegiance_globals_get()
//.text:00670090 ; void __cdecl game_allegiance_globals_update(s_game_allegiance_globals const*)
//.text:006700C0 ; bool __cdecl game_allegiance_incident(e_game_team, e_game_team, int16)

void __cdecl game_allegiance_initialize()
{
    INVOKE(0x00670220, game_allegiance_initialize);
}

void __cdecl game_allegiance_initialize_for_new_map()
{
    INVOKE(0x00670290, game_allegiance_initialize_for_new_map);
}

//.text:006702F0 ; void __cdecl game_allegiance_notify_change(e_game_team, e_game_team)
//.text:00670370 ; void __cdecl game_allegiance_propogate_broken(e_game_team, e_game_team, bool, bool)
//.text:00670410 ; void __cdecl game_allegiance_provoke(e_game_team, e_game_team)
//.text:006704B0 ; bool __cdecl game_allegiance_remove(e_game_team, e_game_team)

void __cdecl game_allegiance_update()
{
    INVOKE(0x00670690, game_allegiance_update);
}

//.text:00670800 ; 
//.text:00670930 ; e_game_team __cdecl game_get_player_team(int32)
//.text:006709B0 ; e_game_team __cdecl game_get_unit_team(int32)
//.text:00670A50 ; bool __cdecl game_team_ally_status_changed(e_game_team, e_game_team)
//.text:00670AE0 ; bool __cdecl game_team_is_ally(e_game_team, e_game_team)
//.text:00670B80 ; bool __cdecl game_team_is_enemy(e_game_team, e_game_team)
//.text:00670C10 ; bool __cdecl game_team_is_friendly(e_game_team, e_game_team)
//.text:00670C40 ; bool __cdecl game_team_is_traitor(c_game_team const&, c_game_team const&)
//.text:00670CD0 ; t_restricted_allocation_manager<3, 0, 0, &void __cdecl __tls_set_g_game_allegiance_globals_allocator(void*)>::reserve_memory

