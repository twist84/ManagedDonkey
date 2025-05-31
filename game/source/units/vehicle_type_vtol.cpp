#include "units/vehicle_type_vtol.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_vtol::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	return INVOKE(0x00BD0C30, c_vehicle_type_vtol::adjust_gravity, vehicle_index, gravity_acceleration);
}

void __cdecl c_vehicle_type_vtol::apply_thrust(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance, const real_vector3d* vector, const real_point3d* origin, int32 marker_name, real32 thrust, real_vector3d* force, real_vector3d* torque)
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

real32 c_vehicle_type_vtol::calculate_lift_turn_acceleration(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance)
{
	return INVOKE_CLASS_MEMBER(0x00BD15F0, c_vehicle_type_vtol, calculate_lift_turn_acceleration, vehicle_index, instance);
}

void c_vehicle_type_vtol::compute_engine_scales(real32* left, real32* right) const
{
	INVOKE_CLASS_MEMBER(0x00BD1B10, c_vehicle_type_vtol, compute_engine_scales, left, right);
}

bool c_vehicle_type_vtol::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BD1BB0, c_vehicle_type_vtol, compute_function_value, vehicle_index, function, magnitude, force_active);
}

void c_vehicle_type_vtol::compute_throttle(int32 vehicle_index, real_vector3d* throttle)
{
	INVOKE_CLASS_MEMBER(0x00BD1CF0, c_vehicle_type_vtol, compute_throttle, vehicle_index, throttle);
}

void c_vehicle_type_vtol::create_effects(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD1EA0, c_vehicle_type_vtol, create_effects, vehicle_index);
}

void c_vehicle_type_vtol::deplete_function_variables(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD1F60, c_vehicle_type_vtol, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BD1F70, c_vehicle_type_vtol::effected_by_vehicle_ceiling, vehicle_index);
}

void c_vehicle_type_vtol::interpolate_lift_angles(int32 vehicle_index, real_euler_angles2d* angles, real_vector2d* velocity, const real_euler_angles2d* desired_angles, bool render_angles)
{
	INVOKE_CLASS_MEMBER(0x00BD1F80, c_vehicle_type_vtol, interpolate_lift_angles, vehicle_index, angles, velocity, desired_angles, render_angles);
}

bool __cdecl c_vehicle_type_vtol::is_stopped(int32 vehicle_index)
{
	return INVOKE(0x00BD2200, c_vehicle_type_vtol::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BD2210, c_vehicle_type_vtol::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BD2220, c_vehicle_type_vtol::physics_disabled, vehicle_index);
}

void c_vehicle_type_vtol::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BD2260, c_vehicle_type_vtol, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_vtol::reset(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD23C0, c_vehicle_type_vtol, reset, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE(0x00BD23E0, c_vehicle_type_vtol::should_override_deactivation, vehicle_index);
}

void c_vehicle_type_vtol::update_control(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD2430, c_vehicle_type_vtol, update_control, vehicle_index);
}

void c_vehicle_type_vtol::update_lift_vector(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance, const real_vector3d* throttle, real32 desired_acceleration_k, int32 marker_name, real_vector3d* lift_vector, real_point3d* lift_origin, real_euler_angles2d* angles, real_euler_angles2d* render_angles, real_vector2d* velocity, real_vector2d* render_velocity, real32* effect_scale)
{
	INVOKE_CLASS_MEMBER(0x00BD24E0, c_vehicle_type_vtol, update_lift_vector, vehicle_index, instance, throttle, desired_acceleration_k, marker_name, lift_vector, lift_origin, angles, render_angles, velocity, render_velocity, effect_scale);
}

void c_vehicle_type_vtol::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BD2C70, c_vehicle_type_vtol, update_physics, vehicle_index, instance);
}

void c_vehicle_type_vtol::update_rotor_damping(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD35A0, c_vehicle_type_vtol, update_rotor_damping, vehicle_index);
}

bool __cdecl c_vehicle_type_vtol::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	return INVOKE(0x00BD3790, c_vehicle_type_vtol::vector_is_upsides_down, vehicle_index, vector);
}

