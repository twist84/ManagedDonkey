#include "effects/particle_system.hpp"

void __cdecl c_particle_system::submit_all(e_output_user_index output_user_index, e_effect_pass effect_pass)
{
	INVOKE(0x0074FB50, c_particle_system::submit_all, output_user_index, effect_pass);
}

