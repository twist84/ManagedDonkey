#include "effects/particle_system.hpp"

void __cdecl c_particle_system::submit_all(long user_index, e_effect_pass effect_pass)
{
	INVOKE(0x0074FB50, c_particle_system::submit_all, user_index, effect_pass);
}

