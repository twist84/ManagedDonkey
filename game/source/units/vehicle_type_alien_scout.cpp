#include "units/vehicle_type_alien_scout.hpp"

void c_vehicle_type_alien_scout::adjust_gravity(long vehicle_index, vector3d* gravity_acceleration)
{
	INVOKE_CLASS_MEMBER(0x00BCAA90, c_vehicle_type_alien_scout, adjust_gravity, vehicle_index, gravity_acceleration);
}

bool c_vehicle_type_alien_scout::compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BCAAA0, c_vehicle_type_alien_scout, compute_function_value, vehicle_index, function, magnitude, force_active);
}

// this belongs in vehicle_scout_utilities.cpp
//.text:00BCAC60 ; real __cdecl scout_compute_function_velocity(real, real, bool, s_engine_function_definition const*, vector3d const*, vector3d const*, vector3d const*, real, real, real)

void c_vehicle_type_alien_scout::compute_throttle(long vehicle_index, vector3d* throttle)
{
	INVOKE_CLASS_MEMBER(0x00BCAE30, c_vehicle_type_alien_scout, compute_throttle, vehicle_index, throttle);
}

void c_vehicle_type_alien_scout::create_effects(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCAEA0, c_vehicle_type_alien_scout, create_effects, vehicle_index);
}

void c_vehicle_type_alien_scout::deplete_function_variables(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCB160, c_vehicle_type_alien_scout, deplete_function_variables, vehicle_index);
}

bool c_vehicle_type_alien_scout::effected_by_vehicle_ceiling(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCB190, c_vehicle_type_alien_scout, effected_by_vehicle_ceiling, vehicle_index);
}

bool c_vehicle_type_alien_scout::is_stopped(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCB1A0, c_vehicle_type_alien_scout, is_stopped, vehicle_index);
}

bool c_vehicle_type_alien_scout::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCB1B0, c_vehicle_type_alien_scout, kills_riders_at_terminal_velocity, vehicle_index);
}

bool c_vehicle_type_alien_scout::physics_disabled(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCB1C0, c_vehicle_type_alien_scout, physics_disabled, vehicle_index);
}

void c_vehicle_type_alien_scout::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BCB200, c_vehicle_type_alien_scout, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_alien_scout::reset(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCB2E0, c_vehicle_type_alien_scout, reset, vehicle_index);
}

bool c_vehicle_type_alien_scout::should_override_deactivation(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCB310, c_vehicle_type_alien_scout, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_alien_scout::update_control(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCB370, c_vehicle_type_alien_scout, update_control, vehicle_index);
}

void c_vehicle_type_alien_scout::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BCB510, c_vehicle_type_alien_scout, update_physics, vehicle_index, instance);
}

bool c_vehicle_type_alien_scout::vector_is_upsides_down(long vehicle_index, vector3d const* vector)
{
	return INVOKE_CLASS_MEMBER(0x00BCCC70, c_vehicle_type_alien_scout, vector_is_upsides_down, vehicle_index, vector);
}


