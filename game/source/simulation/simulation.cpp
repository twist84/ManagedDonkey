#include "simulation/simulation.hpp"
#include "main/main.hpp"

REFERENCE_DECLARE(0x019A9FA0, s_simulation_globals, simulation_globals);

c_wait_for_render_thread::c_wait_for_render_thread(char const* file, long line) :
	m_flags(_internal_halt_render_thread_and_lock_resources(file, line))
{
}

c_wait_for_render_thread::~c_wait_for_render_thread()
{
	unlock_resources_and_resume_render_thread(m_flags);
}

bool __cdecl simulation_starting_up()
{
	return INVOKE(0x004420E0, simulation_starting_up);
}

