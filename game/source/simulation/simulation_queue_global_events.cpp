#include "simulation/simulation_queue_global_events.hpp"

#include "cseries/cseries.hpp"

//.text:004723E0 ; void __cdecl simulation_queue_game_global_bookkeeping_event_apply(const s_simulation_queue_element*)
//.text:00472450 ; void __cdecl simulation_queue_game_global_bookkeeping_event_insert(e_simulation_queue_game_global_bookkeeping_event_type)
//.text:00472590 ; void __cdecl simulation_queue_game_global_event_apply(const s_simulation_queue_element*)

void __cdecl simulation_queue_game_global_event_insert(e_simulation_queue_game_global_event_type game_global_event_type)
{
	INVOKE(0x00472720, simulation_queue_game_global_event_insert, game_global_event_type);
}

//.text:00472870 ; bool __cdecl simulation_queue_game_global_event_requires_cutoff(const s_simulation_queue_element*)
//.text:004728F0 ; void __cdecl simulation_queue_gamestates_delete_apply(const s_simulation_queue_element*)
//.text:00472990 ; void __cdecl simulation_queue_gamestates_delete_insert(const s_simulation_queue_gamestate_clear_data*)
//.text:00472AE0 ; void __cdecl simulation_queue_player_event_apply(const s_simulation_queue_element*)
//.text:00472BC0 ; void __cdecl simulation_queue_player_event_insert(e_simulation_queue_player_event_type, int32, const s_simulation_queue_player_event_data*)
//.text:00472DA0 ; void __cdecl simulation_queue_player_event_apply_set_activation(int32, bool)
//.text:00472E20 ; void __cdecl simulation_queue_player_event_set_respawn_target(int32, int32)
//.text:00472E90 ; void __cdecl simulation_queue_player_unknown12_apply(const s_simulation_queue_element*) // $TODO find a better name for this
//.text:00472F20 ; void __cdecl simulation_queue_player_unknown12_insert(const void*) // $TODO find a better name for this
//.text:00473020 ; void __cdecl simulation_queue_player_update_apply(const s_simulation_queue_element*)
//.text:00473120 ; void __cdecl simulation_queue_player_update_insert(const simulation_player_update*)
//.text:00473230 ; void __cdecl simulation_queue_sandbox_event_apply(const s_simulation_queue_element*)
//.text:004735D0 ; void __cdecl simulation_queue_sandbox_event_insert(const s_simulation_queue_sandbox_event_data*)

