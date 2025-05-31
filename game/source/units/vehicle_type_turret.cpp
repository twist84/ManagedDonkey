#include "units/vehicle_type_turret.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_turret::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE(0x00BCEC00, c_vehicle_type_turret::adjust_gravity, vehicle_index, gravity_acceleration);
}

bool __cdecl c_vehicle_type_turret::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE(0x00BCEC10, c_vehicle_type_turret::compute_function_value, vehicle_index, function, magnitude, force_active);
}

void __cdecl c_vehicle_type_turret::create_effects(int32 vehicle_index)
{
	INVOKE(0x00BCEC20, c_vehicle_type_turret::create_effects, vehicle_index);
}

void __cdecl c_vehicle_type_turret::deplete_function_variables(int32 vehicle_index)
{
	INVOKE(0x00BCEC30, c_vehicle_type_turret::deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BCEC40, c_vehicle_type_turret::effected_by_vehicle_ceiling, vehicle_index);
}

void c_vehicle_type_turret::handle_deleted_object(int32 vehicle_index, int32 deleted_object_index)
{
	INVOKE_CLASS_MEMBER(0x00BCEC50, c_vehicle_type_turret, handle_deleted_object, vehicle_index, deleted_object_index);
}

bool __cdecl c_vehicle_type_turret::is_stopped(int32 vehicle_index)
{
	return INVOKE(0x00BCEC60, c_vehicle_type_turret::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BCEC70, c_vehicle_type_turret::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BCEC80, c_vehicle_type_turret::physics_disabled, vehicle_index);
}

void __cdecl c_vehicle_type_turret::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE(0x00BCECC0, c_vehicle_type_turret::process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_turret::reset(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCECD0, c_vehicle_type_turret, reset, vehicle_index);
}

bool __cdecl c_vehicle_type_turret::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE(0x00BCECE0, c_vehicle_type_turret::should_override_deactivation, vehicle_index);
}

void __cdecl c_vehicle_type_turret::update_control(int32 vehicle_index)
{
	INVOKE(0x00BCECF0, c_vehicle_type_turret::update_control, vehicle_index);
}

void __cdecl c_vehicle_type_turret::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* physics)
{
	INVOKE(0x00BCED00, c_vehicle_type_turret::update_physics, vehicle_index, physics);
}

bool __cdecl c_vehicle_type_turret::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	return INVOKE(0x00BCED10, c_vehicle_type_turret::vector_is_upsides_down, vehicle_index, vector);
}

