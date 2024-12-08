#include "units/vehicle_type_alien_fighter.hpp"

#include "motor/vehicle_motor_program.hpp"

void c_vehicle_type_alien_fighter::adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE_CLASS_MEMBER(0x00BCCC90, c_vehicle_type_alien_fighter, adjust_gravity, vehicle_index, gravity_acceleration);
}

bool c_vehicle_type_alien_fighter::can_select_new_trick(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCCD00, c_vehicle_type_alien_fighter, can_select_new_trick, vehicle_index);
}

bool c_vehicle_type_alien_fighter::compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BCCD60, c_vehicle_type_alien_fighter, compute_function_value, vehicle_index, function, magnitude, force_active);
}

e_vehicle_trick_type __cdecl c_vehicle_type_alien_fighter::compute_new_trick_type(long vehicle_index)
{
	return INVOKE(0x00BCCF20, c_vehicle_type_alien_fighter::compute_new_trick_type, vehicle_index);
}

bool c_vehicle_type_alien_fighter::compute_reference_and_trick_rotation(long vehicle_index, s_havok_vehicle_physics_instance const* instance, real_matrix3x3* reference_rotation, real_matrix3x3* trick_rotation)
{
	return INVOKE_CLASS_MEMBER(0x00BCD060, c_vehicle_type_alien_fighter, compute_reference_and_trick_rotation, vehicle_index, instance, reference_rotation, trick_rotation);
}

real c_vehicle_type_alien_fighter::compute_trick_t(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD420, c_vehicle_type_alien_fighter, compute_trick_t, vehicle_index);
}

void __cdecl c_vehicle_type_alien_fighter::create_effects(long vehicle_index)
{
	INVOKE(0x00BCD4F0, c_vehicle_type_alien_fighter::create_effects, vehicle_index);
}

void c_vehicle_type_alien_fighter::deplete_function_variables(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCD500, c_vehicle_type_alien_fighter, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::effected_by_vehicle_ceiling(long vehicle_index)
{
	return INVOKE(0x00BCD540, c_vehicle_type_alien_fighter::effected_by_vehicle_ceiling, vehicle_index);
}

bool c_vehicle_type_alien_fighter::is_running_trick(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD550, c_vehicle_type_alien_fighter, is_running_trick, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::is_stopped(long vehicle_index)
{
	return INVOKE(0x00BCD5F0, c_vehicle_type_alien_fighter::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return INVOKE(0x00BCD600, c_vehicle_type_alien_fighter::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::physics_disabled(long vehicle_index)
{
	return INVOKE(0x00BCD610, c_vehicle_type_alien_fighter::physics_disabled, vehicle_index);
}

void c_vehicle_type_alien_fighter::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BCD650, c_vehicle_type_alien_fighter, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_alien_fighter::reset(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCD730, c_vehicle_type_alien_fighter, reset, vehicle_index);
}

bool c_vehicle_type_alien_fighter::should_override_deactivation(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD750, c_vehicle_type_alien_fighter, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_alien_fighter::start_trick(long vehicle_index, e_vehicle_trick_type vehicle_trick_type)
{
	INVOKE_CLASS_MEMBER(0x00BCD7C0, c_vehicle_type_alien_fighter, start_trick, vehicle_index, vehicle_trick_type);
}

real c_vehicle_type_alien_fighter::trick_duration(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD7E0, c_vehicle_type_alien_fighter, trick_duration, vehicle_index);
}

void c_vehicle_type_alien_fighter::update_control(long vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCD860, c_vehicle_type_alien_fighter, update_control, vehicle_index);
}

void c_vehicle_type_alien_fighter::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BCD9E0, c_vehicle_type_alien_fighter, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_alien_fighter::vector_is_upsides_down(long vehicle_index, real_vector3d const* vector)
{
	return INVOKE(0x00BCEBE0, c_vehicle_type_alien_fighter::vector_is_upsides_down, vehicle_index, vector);
}

