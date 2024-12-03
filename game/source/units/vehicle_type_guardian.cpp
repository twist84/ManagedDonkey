#include "units/vehicle_type_guardian.hpp"

#include "motor/vehicle_motor_program.hpp"

void __cdecl c_vehicle_type_guardian::adjust_gravity(long vehicle_index, vector3d* gravity_acceleration)
{
	INVOKE(0x00BD0050, c_vehicle_type_guardian::adjust_gravity, vehicle_index, gravity_acceleration);
}

bool __cdecl  c_vehicle_type_guardian::compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active)
{
	return INVOKE(0x00BD0060, c_vehicle_type_guardian::compute_function_value, vehicle_index, function, magnitude, force_active);
}

void __cdecl c_vehicle_type_guardian::create_effects(long vehicle_index)
{
	INVOKE(0x00BD0070, c_vehicle_type_guardian::create_effects, vehicle_index);
}

void c_vehicle_type_guardian::deplete_function_variables(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD0080, c_vehicle_type_guardian, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_guardian::effected_by_vehicle_ceiling(long vehicle_index)
{
	return INVOKE(0x00BD0090, c_vehicle_type_guardian::effected_by_vehicle_ceiling, vehicle_index);
}

s_vehicle_guardian_definition const* c_vehicle_type_guardian::get_type_definition(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BD00A0, c_vehicle_type_guardian, get_type_definition, vehicle_index);
}

void c_vehicle_type_guardian::handle_deleted_object(long vehicle_index, long deleted_object_index)
{
	INVOKE_CLASS_MEMBER(0x00BD00E0, c_vehicle_type_guardian, handle_deleted_object, vehicle_index, deleted_object_index);
}

bool __cdecl c_vehicle_type_guardian::is_stopped(long vehicle_index)
{
	return INVOKE(0x00BD00F0, c_vehicle_type_guardian::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_guardian::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return INVOKE(0x00BD0100, c_vehicle_type_guardian::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_guardian::physics_disabled(long vehicle_index)
{
	return INVOKE(0x00BD0110, c_vehicle_type_guardian::physics_disabled, vehicle_index);
}

void __cdecl c_vehicle_type_guardian::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE(0x00BD0150, c_vehicle_type_guardian::process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_guardian::reset(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD0160, c_vehicle_type_guardian, reset, vehicle_index);
}

bool c_vehicle_type_guardian::should_override_deactivation(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BD0180, c_vehicle_type_guardian, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_guardian::update_control(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BD01E0, c_vehicle_type_guardian, update_control, vehicle_index);
}

void c_vehicle_type_guardian::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BD0280, c_vehicle_type_guardian, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_guardian::vector_is_upsides_down(long vehicle_index, vector3d const* vector)
{
	return INVOKE(0x00BD0C20, c_vehicle_type_guardian::vector_is_upsides_down, vehicle_index, vector);
}

