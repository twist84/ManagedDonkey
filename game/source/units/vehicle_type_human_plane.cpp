#include "units/vehicle_type_human_plane.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_human_plane::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE(0x00BC9690, c_vehicle_type_human_plane::adjust_gravity, vehicle_index, gravity_acceleration);
}

bool c_vehicle_type_human_plane::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BC96A0, c_vehicle_type_human_plane, compute_function_value, vehicle_index, function, magnitude, force_active);
}

void c_vehicle_type_human_plane::create_effects(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BC99B0, c_vehicle_type_human_plane, create_effects, vehicle_index);
}

void c_vehicle_type_human_plane::deplete_function_variables(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BC9A60, c_vehicle_type_human_plane, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_human_plane::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BC9A90, c_vehicle_type_human_plane::effected_by_vehicle_ceiling, vehicle_index);
}

bool __cdecl c_vehicle_type_human_plane::is_stopped(int32 vehicle_index)
{
	return INVOKE(0x00BC9AA0, c_vehicle_type_human_plane::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_human_plane::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BC9AB0, c_vehicle_type_human_plane::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_human_plane::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BC9AC0, c_vehicle_type_human_plane::physics_disabled, vehicle_index);
}

void c_vehicle_type_human_plane::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BC9B00, c_vehicle_type_human_plane, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_human_plane::reset(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC9BE0, c_vehicle_type_human_plane, reset, vehicle_index);
}

bool c_vehicle_type_human_plane::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC9C00, c_vehicle_type_human_plane, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_human_plane::update_control(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BC9C70, c_vehicle_type_human_plane, update_control, vehicle_index);
}

void c_vehicle_type_human_plane::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BC9DC0, c_vehicle_type_human_plane, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_human_plane::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	return INVOKE(0x00BCAA70, c_vehicle_type_human_plane::vector_is_upsides_down, vehicle_index, vector);
}

