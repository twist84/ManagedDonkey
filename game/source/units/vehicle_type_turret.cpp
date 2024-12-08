#include "units/vehicle_type_turret.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_turret::adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE(0x00BCEC00, c_vehicle_type_turret::adjust_gravity, vehicle_index, gravity_acceleration);
}

bool __cdecl c_vehicle_type_turret::compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active)
{
	return INVOKE(0x00BCEC10, c_vehicle_type_turret::compute_function_value, vehicle_index, function, magnitude, force_active);
}

void __cdecl c_vehicle_type_turret::create_effects(long vehicle_index)
{
	INVOKE(0x00BCEC20, c_vehicle_type_turret::create_effects, vehicle_index);
}

void __cdecl c_vehicle_type_turret::deplete_function_variables(long vehicle_index)
{
	INVOKE(0x00BCEC30, c_vehicle_type_turret::deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::effected_by_vehicle_ceiling(long vehicle_index)
{
	return INVOKE(0x00BCEC40, c_vehicle_type_turret::effected_by_vehicle_ceiling, vehicle_index);
}

void c_vehicle_type_turret::handle_deleted_object(long vehicle_index, long deleted_object_index)
{
	INVOKE_CLASS_MEMBER(0x00BCEC50, c_vehicle_type_turret, handle_deleted_object, vehicle_index, deleted_object_index);
}

bool __cdecl c_vehicle_type_turret::is_stopped(long vehicle_index)
{
	return INVOKE(0x00BCEC60, c_vehicle_type_turret::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return INVOKE(0x00BCEC70, c_vehicle_type_turret::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::physics_disabled(long vehicle_index)
{
	return INVOKE(0x00BCEC80, c_vehicle_type_turret::physics_disabled, vehicle_index);
}

void __cdecl c_vehicle_type_turret::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE(0x00BCECC0, c_vehicle_type_turret::process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_turret::reset(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCECD0, c_vehicle_type_turret, reset, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::should_override_deactivation(long vehicle_index)
{
	return INVOKE(0x00BCECE0, c_vehicle_type_turret::should_override_deactivation, vehicle_index);
}

void __cdecl c_vehicle_type_turret::update_control(long vehicle_index)
{
	INVOKE(0x00BCECF0, c_vehicle_type_turret::update_control, vehicle_index);
}

void __cdecl c_vehicle_type_turret::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* physics)
{
	INVOKE(0x00BCED00, c_vehicle_type_turret::update_physics, vehicle_index, physics);
}

bool __cdecl c_vehicle_type_turret::vector_is_upsides_down(long vehicle_index, real_vector3d const* vector)
{
	return INVOKE(0x00BCED10, c_vehicle_type_turret::vector_is_upsides_down, vehicle_index, vector);
}

