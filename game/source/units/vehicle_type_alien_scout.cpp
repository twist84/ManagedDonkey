#include "units/vehicle_type_alien_scout.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_alien_scout::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE(0x00BCAA90, c_vehicle_type_alien_scout::adjust_gravity, vehicle_index, gravity_acceleration);
}

bool c_vehicle_type_alien_scout::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BCAAA0, c_vehicle_type_alien_scout, compute_function_value, vehicle_index, function, magnitude, force_active);
}

// this belongs in vehicle_scout_utilities.cpp
//.text:00BCAC60 ; real32 __cdecl scout_compute_function_velocity(real32, real32, bool, s_engine_function_definition const*, real_vector3d const*, real_vector3d const*, real_vector3d const*, real32, real32, real32)

void c_vehicle_type_alien_scout::compute_throttle(int32 vehicle_index, real_vector3d* throttle)
{
	INVOKE_CLASS_MEMBER(0x00BCAE30, c_vehicle_type_alien_scout, compute_throttle, vehicle_index, throttle);
}

void __cdecl c_vehicle_type_alien_scout::create_effects(int32 vehicle_index)
{
	return INVOKE(0x00BCAEA0, c_vehicle_type_alien_scout::create_effects, vehicle_index);
}

void c_vehicle_type_alien_scout::deplete_function_variables(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCB160, c_vehicle_type_alien_scout, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_scout::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BCB190, c_vehicle_type_alien_scout::effected_by_vehicle_ceiling, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_scout::is_stopped(int32 vehicle_index)
{
	return INVOKE(0x00BCB1A0, c_vehicle_type_alien_scout::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_scout::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BCB1B0, c_vehicle_type_alien_scout::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_scout::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BCB1C0, c_vehicle_type_alien_scout::physics_disabled, vehicle_index);
}

void c_vehicle_type_alien_scout::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, render_model_definition const*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BCB200, c_vehicle_type_alien_scout, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_alien_scout::reset(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCB2E0, c_vehicle_type_alien_scout, reset, vehicle_index);
}

bool c_vehicle_type_alien_scout::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCB310, c_vehicle_type_alien_scout, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_alien_scout::update_control(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCB370, c_vehicle_type_alien_scout, update_control, vehicle_index);
}

void c_vehicle_type_alien_scout::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BCB510, c_vehicle_type_alien_scout, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_alien_scout::vector_is_upsides_down(int32 vehicle_index, real_vector3d const* vector)
{
	return INVOKE(0x00BCCC70, c_vehicle_type_alien_scout::vector_is_upsides_down, vehicle_index, vector);
}


