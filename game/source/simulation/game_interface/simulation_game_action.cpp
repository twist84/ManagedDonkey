#include "simulation/game_interface/simulation_game_action.hpp"

void __cdecl simulation_action_game_engine_globals_update(c_flags<long, qword, 64>& flags)
{
	INVOKE(0x004B2570, simulation_action_game_engine_globals_update, flags);
}

void __cdecl simulation_action_object_create(long object_index)
{
	INVOKE(0x004B2CD0, simulation_action_object_create, object_index);
}

