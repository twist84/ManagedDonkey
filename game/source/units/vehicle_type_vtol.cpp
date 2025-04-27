#include "units/vehicle_type_vtol.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_vtol::adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration)
{
	return INVOKE(0x00BD0C30, c_vehicle_type_vtol::adjust_gravity, vehicle_index, gravity_acceleration);
}

void __cdecl c_vehicle_type_vtol::apply_thrust(long vehicle_index, s_havok_vehicle_physics_instance const* instance, real_vector3d const* vector, real_point3d const* origin, long marker_name, real32 thrust, real_vector3d* force, real_vector3d* torque)
{
	INVOKE(0x00BD0C40, c_vehicle_type_vtol::apply_thrust, vehicle_index, instance, vector, origin, marker_name, thrust, force, torque);
}

real32 __cdecl calculate_acceleration_magnitude_vtol(bool govern_velocity, bool acceleration_as_force, real32 mass, real32 distance, real32 acceleration, real32 velocity_towards_target, real32 maximum_velocity, real32 dead_radius, real32 hookes_law_e, bool disable_damping, real32* scale)
{
	return INVOKE(0x00BD0D20, calculate_acceleration_magnitude_vtol, govern_velocity, acceleration_as_force, mass, distance, acceleration, velocity_towards_target, maximum_velocity, dead_radius, hookes_law_e, disable_damping, scale);
}

// dummy declaration
real32 __cdecl calculate_desired_acceleration_real4(real32 velocity, real32 minimum_acceleration, real32 maximum_acceleration, real32 distance);
real32 __cdecl calculate_desired_acceleration(real32 velocity, real32 minimum_acceleration, real32 maximum_acceleration, real32 distance)
{
	return INVOKE(0x00BD10C0, calculate_desired_acceleration_real4, velocity, minimum_acceleration, maximum_acceleration, distance);
}

// dummy declaration
real32 __cdecl calculate_desired_acceleration_real5(real32 velocity, real32 gravity, real32 minimum_acceleration, real32 maximum_acceleration, real32 distance);
real32 __cdecl calculate_desired_acceleration(real32 velocity, real32 gravity, real32 minimum_acceleration, real32 maximum_acceleration, real32 distance)
{
	return INVOKE(0x00BD1250, calculate_desired_acceleration_real5, velocity, gravity, minimum_acceleration, maximum_acceleration, distance);
}

real32 c_vehicle_type_vtol::calculate_lift_turn_acceleration(long vehicle_index, s_havok_vehicle_physics_instance const* instance)
{
	return INVOKE_CLASS_MEMBER(0x00BD15F0, c_vehicle_type_vtol, calculate_lift_turn_acceleration, vehicle_index, instance);
}

void c_vehicle_type_vtol::compute_engine_scales(real32* left, real32* right) const
{
	INVOKE_CLASS_MEMBER(0x00BD1B10, c_vehicle_type_vtol, compute_engine_scales, left, right);
}

bool c_vehicle_type_vtol::compute_function_value(long vehicle_index, long function, real32* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BD1BB0, c_vehicle_type_vtol, compute_function_value, vehicle_index, function, magnitude, force_active);
}

void c_vehicle_type_vtol::compute_throttle(long vehicle_index, real_vector3d* throttle)
{
	INVOKE_CLASS_MEMBER(0x00BD1CF0, c_vehicle_type_vtol, compute_throttle, vehicle_index, throttle);
}

void c_vehicle_type_vtol::create_effects(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD1EA0, c_vehicle_type_vtol, create_effects, vehicle_index);
}

void c_vehicle_type_vtol::deplete_function_variables(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD1F60, c_vehicle_type_vtol, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::effected_by_vehicle_ceiling(long vehicle_index)
{
	return INVOKE(0x00BD1F70, c_vehicle_type_vtol::effected_by_vehicle_ceiling, vehicle_index);
}

void c_vehicle_type_vtol::interpolate_lift_angles(long vehicle_index, real_euler_angles2d* angles, real_vector2d* velocity, real_euler_angles2d const* desired_angles, bool render_angles)
{
	INVOKE_CLASS_MEMBER(0x00BD1F80, c_vehicle_type_vtol, interpolate_lift_angles, vehicle_index, angles, velocity, desired_angles, render_angles);
}

bool __cdecl c_vehicle_type_vtol::is_stopped(long vehicle_index)
{
	return INVOKE(0x00BD2200, c_vehicle_type_vtol::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return INVOKE(0x00BD2210, c_vehicle_type_vtol::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::physics_disabled(long vehicle_index)
{
	return INVOKE(0x00BD2220, c_vehicle_type_vtol::physics_disabled, vehicle_index);
}

void c_vehicle_type_vtol::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BD2260, c_vehicle_type_vtol, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_vtol::reset(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD23C0, c_vehicle_type_vtol, reset, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::should_override_deactivation(long vehicle_index)
{
	return INVOKE(0x00BD23E0, c_vehicle_type_vtol::should_override_deactivation, vehicle_index);
}

void c_vehicle_type_vtol::update_control(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD2430, c_vehicle_type_vtol, update_control, vehicle_index);
}

void c_vehicle_type_vtol::update_lift_vector(long vehicle_index, s_havok_vehicle_physics_instance const* instance, real_vector3d const* throttle, real32 desired_acceleration_k, long marker_name, real_vector3d* lift_vector, real_point3d* lift_origin, real_euler_angles2d* angles, real_euler_angles2d* render_angles, real_vector2d* velocity, real_vector2d* render_velocity, real32* effect_scale)
{
	INVOKE_CLASS_MEMBER(0x00BD24E0, c_vehicle_type_vtol, update_lift_vector, vehicle_index, instance, throttle, desired_acceleration_k, marker_name, lift_vector, lift_origin, angles, render_angles, velocity, render_velocity, effect_scale);
}

void c_vehicle_type_vtol::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BD2C70, c_vehicle_type_vtol, update_physics, vehicle_index, instance);
}

void c_vehicle_type_vtol::update_rotor_damping(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD35A0, c_vehicle_type_vtol, update_rotor_damping, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::vector_is_upsides_down(long vehicle_index, real_vector3d const* vector)
{
	return INVOKE(0x00BD3790, c_vehicle_type_vtol::vector_is_upsides_down, vehicle_index, vector);
}

