#include "simulation/simulation.hpp"

REFERENCE_DECLARE(0x019A9FA0, s_simulation_globals, simulation_globals);

bool __cdecl simulation_starting_up()
{
	return INVOKE(0x004420E0, simulation_starting_up);
}

