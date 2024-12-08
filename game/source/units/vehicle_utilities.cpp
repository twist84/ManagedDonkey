#include "units/vehicle_utilities.hpp"

//.text:00BC0B50 ; void __cdecl vehicle_calculate_turning_torque(s_havok_vehicle_physics_instance const*, real_vector3d const*, real_vector3d*, bool, real, real, real)
//.text:00BC0E40 ; real_vector3d* __cdecl vehicle_compute_acceleration(real_vector3d const*, real_vector3d const*, real, real, real_vector3d*, real)
//.text:00BC0FE0 ; void __cdecl vehicle_compute_relative_throttle(long, real_vector3d*)
//.text:00BC11A0 ; real __cdecl vehicle_compute_throttle1d(long)
//.text:00BC1250 ; void __cdecl vehicle_create_ground_effects(long, real, real)
//.text:00BC1460 ; void __cdecl vehicle_degrade_variable(real*)
//.text:00BC14C0 ; real __cdecl vehicle_function_safe_divide(real, real)
//.text:00BC1500 ; 
//.text:00BC1580 ; void __cdecl vehicle_scale_gravity_for_friction_point_vehicle(long, real_vector3d const*, real_vector3d*, real)

void __cdecl vehicle_try_to_join_local_physics(long vehicle_index, s_havok_vehicle_physics_instance const* instance)
{
	INVOKE(0x00BC1690, vehicle_try_to_join_local_physics, vehicle_index, instance);
}

//.text:00BC1750 ; void __cdecl vehicle_update_antigravity_hover(s_havok_vehicle_physics_instance const*, real*)
//.text:00BC1840 ; bool __cdecl vehicle_update_boost(long, bool)
//.text:00BC1920 ; real __cdecl vehicle_update_control_classic(long, real_vector3d const*, real_vector3d const*, real, real, s_vehicle_steering_control_definition const*, s_vehicle_turning_control_definition const*, physics_variable_speed const*, physics_variable_speed const*, bool, bool, real*, real*, real*)

