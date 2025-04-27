#include "scenario/scenario_interpolators.hpp"

//.text:00687590 ; 
//.text:006875B0 ; 
//.text:006875C0 ; void __cdecl __tls_set_g_scenario_interpolator_globals_allocator(void*)
//.text:006875E0 ; 
//.text:00687620 ; 
//.text:00687650 ; 
//.text:00687680 ; real32 __cdecl hs_interpolator_evaluate_at(int32, real32, bool)
//.text:006876E0 ; real32 __cdecl hs_interpolator_evaluate_at_time(int32, real32, bool)
//.text:006877C0 ; real32 __cdecl hs_interpolator_evaluate_at_time_delta(int32, real32, bool)
//.text:006878A0 ; real32 __cdecl hs_interpolator_evaluate_at_time_fraction(int32, real32, bool)
//.text:00687950 ; void __cdecl hs_interpolator_flip_last_interpolator()
//.text:006879D0 ; 
//.text:00687A70 ; 
//.text:00687B50 ; real32 __cdecl hs_interpolator_get_current_value(int32, bool)
//.text:00687BE0 ; 
//.text:00687C80 ; real32 __cdecl hs_interpolator_get_final_value(int32, bool)
//.text:00687CE0 ; 
//.text:00687D80 ; real32 __cdecl hs_interpolator_get_start_value(int32, bool)
//.text:00687DE0 ; 
//.text:00687E80 ; 
//.text:00687F20 ; int32 __cdecl hs_interpolator_restart(int32)
//.text:00687F90 ; 
//.text:00688040 ; int32 __cdecl hs_interpolator_set_current_value(int32, real32)
//.text:00688080 ; int32 __cdecl hs_interpolator_start(int32, real32, real32)
//.text:006880F0 ; int32 __cdecl hs_interpolator_start_smooth(int32, real32, real32)
//.text:006881F0 ; int32 __cdecl hs_interpolator_stop(int32)
//.text:00688220 ; void __cdecl hs_interpolator_stop_all()
//.text:00688270 ; 
//.text:006882B0 ; 
//.text:00688350 ; real32 __cdecl scenario_interpolator_evaluate_at(int32, real32)
//.text:00688450 ; real32 __cdecl scenario_interpolator_get_current_value(int32)
//.text:006884A0 ; 

void __cdecl scenario_interpolators_dispose()
{
	INVOKE(0x00688600, scenario_interpolators_dispose);
}

void __cdecl scenario_interpolators_dispose_from_old_map()
{
	INVOKE(0x00688620, scenario_interpolators_dispose_from_old_map);
}

void __cdecl scenario_interpolators_frame_update(real32 game_seconds_elapsed)
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

