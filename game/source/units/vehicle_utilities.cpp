#include "units/vehicle_utilities.hpp"

//.text:00BC0B50 ; void __cdecl vehicle_calculate_turning_torque(const s_havok_vehicle_physics_instance*, const real_vector3d*, real_vector3d*, bool, real32, real32, real32)
//.text:00BC0E40 ; real_vector3d* __cdecl vehicle_compute_acceleration(const real_vector3d*, const real_vector3d*, real32, real32, real_vector3d*, real32)
//.text:00BC0FE0 ; void __cdecl vehicle_compute_relative_throttle(int32, real_vector3d*)
//.text:00BC11A0 ; real32 __cdecl vehicle_compute_throttle1d(int32)
//.text:00BC1250 ; void __cdecl vehicle_create_ground_effects(int32, real32, real32)
//.text:00BC1460 ; void __cdecl vehicle_degrade_variable(real32*)
//.text:00BC14C0 ; real32 __cdecl vehicle_function_safe_divide(real32, real32)
//.text:00BC1500 ; 
//.text:00BC1580 ; void __cdecl vehicle_scale_gravity_for_friction_point_vehicle(int32, const real_vector3d*, real_vector3d*, real32)

void __cdecl vehicle_try_to_join_local_physics(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance)
{
	INVOKE(0x00BC1690, vehicle_try_to_join_local_physics, vehicle_index, instance);
}

//.text:00BC1750 ; void __cdecl vehicle_update_antigravity_hover(const s_havok_vehicle_physics_instance*, real32*)
//.text:00BC1840 ; bool __cdecl vehicle_update_boost(int32, bool)
//.text:00BC1920 ; real32 __cdecl vehicle_update_control_classic(int32, const real_vector3d*, const real_vector3d*, real32, real32, const s_vehicle_steering_control_definition*, const s_vehicle_turning_control_definition*, const physics_variable_speed*, const physics_variable_speed*, bool, bool, real32*, real32*, real32*)

