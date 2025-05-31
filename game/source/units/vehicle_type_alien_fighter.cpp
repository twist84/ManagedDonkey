#include "units/vehicle_type_alien_fighter.hpp"

#include "motor/vehicle_motor_program.hpp"

void c_vehicle_type_alien_fighter::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE_CLASS_MEMBER(0x00BCCC90, c_vehicle_type_alien_fighter, adjust_gravity, vehicle_index, gravity_acceleration);
}

bool c_vehicle_type_alien_fighter::can_select_new_trick(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCCD00, c_vehicle_type_alien_fighter, can_select_new_trick, vehicle_index);
}

bool c_vehicle_type_alien_fighter::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BCCD60, c_vehicle_type_alien_fighter, compute_function_value, vehicle_index, function, magnitude, force_active);
}

e_vehicle_trick_type __cdecl c_vehicle_type_alien_fighter::compute_new_trick_type(int32 vehicle_index)
{
	return INVOKE(0x00BCCF20, c_vehicle_type_alien_fighter::compute_new_trick_type, vehicle_index);
}

bool c_vehicle_type_alien_fighter::compute_reference_and_trick_rotation(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance, real_matrix3x3* reference_rotation, real_matrix3x3* trick_rotation)
{
	return INVOKE_CLASS_MEMBER(0x00BCD060, c_vehicle_type_alien_fighter, compute_reference_and_trick_rotation, vehicle_index, instance, reference_rotation, trick_rotation);
}

real32 c_vehicle_type_alien_fighter::compute_trick_t(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD420, c_vehicle_type_alien_fighter, compute_trick_t, vehicle_index);
}

void __cdecl c_vehicle_type_alien_fighter::create_effects(int32 vehicle_index)
{
	INVOKE(0x00BCD4F0, c_vehicle_type_alien_fighter::create_effects, vehicle_index);
}

void c_vehicle_type_alien_fighter::deplete_function_variables(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCD500, c_vehicle_type_alien_fighter, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BCD540, c_vehicle_type_alien_fighter::effected_by_vehicle_ceiling, vehicle_index);
}

bool c_vehicle_type_alien_fighter::is_running_trick(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD550, c_vehicle_type_alien_fighter, is_running_trick, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::is_stopped(int32 vehicle_index)
{
	return INVOKE(0x00BCD5F0, c_vehicle_type_alien_fighter::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BCD600, c_vehicle_type_alien_fighter::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_alien_fighter::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BCD610, c_vehicle_type_alien_fighter::physics_disabled, vehicle_index);
}

void c_vehicle_type_alien_fighter::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BCD650, c_vehicle_type_alien_fighter, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_alien_fighter::reset(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCD730, c_vehicle_type_alien_fighter, reset, vehicle_index);
}

bool c_vehicle_type_alien_fighter::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD750, c_vehicle_type_alien_fighter, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_alien_fighter::start_trick(int32 vehicle_index, e_vehicle_trick_type vehicle_trick_type)
{
	INVOKE_CLASS_MEMBER(0x00BCD7C0, c_vehicle_type_alien_fighter, start_trick, vehicle_index, vehicle_trick_type);
}

real32 c_vehicle_type_alien_fighter::trick_duration(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCD7E0, c_vehicle_type_alien_fighter, trick_duration, vehicle_index);
}

void c_vehicle_type_alien_fighter::update_control(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BCD860, c_vehicle_type_alien_fighter, update_control, vehicle_index);
}

void c_vehicle_type_alien_fighter::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BCD9E0, c_vehicle_type_alien_fighter, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_alien_fighter::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	return INVOKE(0x00BCEBE0, c_vehicle_type_alien_fighter::vector_is_upsides_down, vehicle_index, vector);
}

