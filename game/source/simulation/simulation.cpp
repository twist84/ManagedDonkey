#include "simulation/simulation.hpp"

s_simulation_globals& simulation_globals = *reinterpret_cast<s_simulation_globals*>(0x019A9FA0);

bool __cdecl simulation_starting_up()
{
	return INVOKE(0x004420E0, simulation_starting_up);
}