#include "simulation/game_interface/simulation_game_engine_sandbox.hpp"

c_simulation_sandbox_engine_globals_definition& c_simulation_sandbox_engine_globals_definition::instance()
{
	return INVOKE(0x00470860, instance);
}

