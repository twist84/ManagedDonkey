#include "units/vehicle_engine.hpp"

//.text:00BEC690 ; void __cdecl vehicle_engine_deplete_function_variables(s_vehicle_engine*)
//.text:00BEC6B0 ; int16 __cdecl vehicle_engine_get_desired_gear(const s_vehicle_engine*, const s_vehicle_engine_definition*, real32, bool, bool, bool)

real32 __cdecl vehicle_engine_get_rpm_function_scale(const s_vehicle_engine* engine)
{
	return INVOKE(0x00BEC8E0, vehicle_engine_get_rpm_function_scale, engine);
}

//.text:00BEC990 ; real32 __cdecl vehicle_engine_get_wheel_ground_speed(const s_vehicle_engine*, const s_vehicle_engine_definition*)
//.text:00BEC9E0 ; void __cdecl vehicle_engine_post_physics_update(s_vehicle_engine*, const s_vehicle_engine_definition*, s_havok_vehicle_physics_instance*)
//.text:00BECA20 ; void __cdecl vehicle_engine_reset(s_vehicle_engine*)
//.text:00BECA50 ; void __cdecl vehicle_engine_update(int32, s_vehicle_engine*, const s_vehicle_engine_definition*, real32, int16, bool)
//.text:00BECE10 ; real32 __cdecl vehicle_get_gear_ratio(const s_vehicle_engine*, const s_vehicle_engine_definition*, int16)
//.text:00BECE60 ; real32 __cdecl vehicle_get_torque_from_angular_velocity(const s_vehicle_engine*, const s_vehicle_engine_definition*, int16, real32)
//.text:00BECF20 ; real32 __cdecl vehicle_get_torque_from_torque_curve_definition(const s_vehicle_engine_definition*, real32, const s_toruqe_curve_definition*)

