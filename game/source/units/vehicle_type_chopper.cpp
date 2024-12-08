#include "units/vehicle_type_chopper.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_chopper::adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE(0x00BD37B0, c_vehicle_type_chopper::adjust_gravity, vehicle_index, gravity_acceleration);
}

real __cdecl chopper_calculate_acceleration_magnitude(real distance, real acceleration, real velocity_towards_target, real maximum_velocity, real dead_radius, real hookes_law_e, real* scale)
{
	return INVOKE(0x00BD37C0, chopper_calculate_acceleration_magnitude, distance, acceleration, velocity_towards_target, maximum_velocity, dead_radius, hookes_law_e, scale);
}

bool c_vehicle_type_chopper::compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BD39C0, c_vehicle_type_chopper, compute_function_value, vehicle_index, function, magnitude, force_active);
}

void __cdecl c_vehicle_type_chopper::create_effects(long vehicle_index)
{
	INVOKE(0x00BD3C80, c_vehicle_type_chopper::create_effects, vehicle_index);
}

void c_vehicle_type_chopper::deplete_function_variables(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD3C90, c_vehicle_type_chopper, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_chopper::effected_by_vehicle_ceiling(long vehicle_index)
{
	return INVOKE(0x00BD3CB0, c_vehicle_type_chopper::effected_by_vehicle_ceiling, vehicle_index);
}

real_vector3d const* __cdecl c_vehicle_type_chopper::get_debug_magic_force_vector(long vehicle_index, real_point3d* origin)
{
	return INVOKE(0x00BD3CC0, c_vehicle_type_chopper::get_debug_magic_force_vector, vehicle_index, origin);
}

s_vehicle_engine* const c_vehicle_type_chopper::get_engine(long vehicle_index)
{
	return &m_engine;
}

bool c_vehicle_type_chopper::is_e_braking(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BD3CE0, c_vehicle_type_chopper, is_e_braking, vehicle_index);
}

bool c_vehicle_type_chopper::is_stopped(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BD3D20, c_vehicle_type_chopper, is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_chopper::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return INVOKE(0x00BD3D50, c_vehicle_type_chopper::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_chopper::physics_disabled(long vehicle_index)
{
	return INVOKE(0x00BD3D60, c_vehicle_type_chopper::physics_disabled, vehicle_index);
}

void c_vehicle_type_chopper::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BD3D70, c_vehicle_type_chopper, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_chopper::reset(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD3EE0, c_vehicle_type_chopper, reset, vehicle_index);
}

bool c_vehicle_type_chopper::should_override_deactivation(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BD3F20, c_vehicle_type_chopper, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_chopper::update_boost(long vehicle_index, s_havok_vehicle_physics_instance* physics)
{
	INVOKE_CLASS_MEMBER(0x00BD3F60, c_vehicle_type_chopper, update_boost, vehicle_index, physics);
}

void c_vehicle_type_chopper::update_control(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD4040, c_vehicle_type_chopper, update_control, vehicle_index);
}

void c_vehicle_type_chopper::update_damping(long vehicle_index, s_havok_vehicle_physics_instance* physics)
{
	INVOKE_CLASS_MEMBER(0x00BD4100, c_vehicle_type_chopper, update_damping, vehicle_index, physics);
}

void c_vehicle_type_chopper::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BD4200, c_vehicle_type_chopper, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_chopper::vector_is_upsides_down(long vehicle_index, real_vector3d const* vector)
{
	return INVOKE(0x00BD52C0, c_vehicle_type_chopper::vector_is_upsides_down, vehicle_index, vector);
}

