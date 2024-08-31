#include "scenario/scenario_interpolators.hpp"

//.text:00687590 ; 
//.text:006875B0 ; 
//.text:006875C0 ; void __cdecl __tls_set_g_scenario_interpolator_globals_allocator(void*)
//.text:006875E0 ; 
//.text:00687620 ; 
//.text:00687650 ; 
//.text:00687680 ; real __cdecl hs_interpolator_evaluate_at(long, real, bool)
//.text:006876E0 ; real __cdecl hs_interpolator_evaluate_at_time(long, real, bool)
//.text:006877C0 ; real __cdecl hs_interpolator_evaluate_at_time_delta(long, real, bool)
//.text:006878A0 ; real __cdecl hs_interpolator_evaluate_at_time_fraction(long, real, bool)
//.text:00687950 ; void __cdecl hs_interpolator_flip_last_interpolator()
//.text:006879D0 ; 
//.text:00687A70 ; 
//.text:00687B50 ; real __cdecl hs_interpolator_get_current_value(long, bool)
//.text:00687BE0 ; 
//.text:00687C80 ; real __cdecl hs_interpolator_get_final_value(long, bool)
//.text:00687CE0 ; 
//.text:00687D80 ; real __cdecl hs_interpolator_get_start_value(long, bool)
//.text:00687DE0 ; 
//.text:00687E80 ; 
//.text:00687F20 ; long __cdecl hs_interpolator_restart(long)
//.text:00687F90 ; 
//.text:00688040 ; long __cdecl hs_interpolator_set_current_value(long, real)
//.text:00688080 ; long __cdecl hs_interpolator_start(long, real, real)
//.text:006880F0 ; long __cdecl hs_interpolator_start_smooth(long, real, real)
//.text:006881F0 ; long __cdecl hs_interpolator_stop(long)
//.text:00688220 ; void __cdecl hs_interpolator_stop_all()
//.text:00688270 ; 
//.text:006882B0 ; 
//.text:00688350 ; real __cdecl scenario_interpolator_evaluate_at(long, real)
//.text:00688450 ; real __cdecl scenario_interpolator_get_current_value(long)
//.text:006884A0 ; 

void __cdecl scenario_interpolators_dispose()
{
	INVOKE(0x00688600, scenario_interpolators_dispose);
}

void __cdecl scenario_interpolators_dispose_from_old_map()
{
	INVOKE(0x00688620, scenario_interpolators_dispose_from_old_map);
}

void __cdecl scenario_interpolators_frame_update(real game_seconds_elapsed)
{
	INVOKE(0x00688630, scenario_interpolators_frame_update, game_seconds_elapsed);
}

//.text:00688850 ; 

void __cdecl scenario_interpolators_initialize()
{
	INVOKE(0x00688880, scenario_interpolators_initialize);
}

void __cdecl scenario_interpolators_initialize_for_new_map()
{
	INVOKE(0x006888E0, scenario_interpolators_initialize_for_new_map);
}

