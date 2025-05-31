#include "units/vehicle_type_mantis.hpp"

#include "motor/vehicle_motor_program.hpp"

void c_vehicle_type_mantis::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	return INVOKE_CLASS_MEMBER(0x00BCED30, c_vehicle_type_mantis, adjust_gravity, vehicle_index, gravity_acceleration);
}

void c_vehicle_type_mantis::calculate_throttle(int32 vehicle_index, real_vector3d* throttle)
{
	return INVOKE_CLASS_MEMBER(0x00BCEE00, c_vehicle_type_mantis, calculate_throttle, vehicle_index, throttle);
}

bool __cdecl c_vehicle_type_mantis::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE(0x00BCEEB0, c_vehicle_type_mantis::compute_function_value, vehicle_index, function, magnitude, force_active);
}

void __cdecl c_vehicle_type_mantis::create_effects(int32 vehicle_index)
{
	return INVOKE(0x00BCEEC0, c_vehicle_type_mantis::create_effects, vehicle_index);
}

void c_vehicle_type_mantis::deplete_function_variables(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCEED0, c_vehicle_type_mantis, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_mantis::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BCEEF0, c_vehicle_type_mantis::effected_by_vehicle_ceiling, vehicle_index);
}

const s_vehicle_mantis_definition* c_vehicle_type_mantis::get_type_definition(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCEF00, c_vehicle_type_mantis, get_type_definition, vehicle_index);
}

bool __cdecl c_vehicle_type_mantis::is_stopped(int32 vehicle_index)
{
	return INVOKE(0x00BCEF40, c_vehicle_type_mantis::is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_mantis::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BCEF50, c_vehicle_type_mantis::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_mantis::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BCEF60, c_vehicle_type_mantis::physics_disabled, vehicle_index);
}

void __cdecl c_vehicle_type_mantis::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	return INVOKE(0x00BCEFA0, c_vehicle_type_mantis::process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_mantis::reset(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCEFB0, c_vehicle_type_mantis, reset, vehicle_index);
}

bool c_vehicle_type_mantis::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCEFE0, c_vehicle_type_mantis, should_override_deactivation, vehicle_index);
}

void c_vehicle_type_mantis::update_control(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BCF050, c_vehicle_type_mantis, update_control, vehicle_index);
}

void c_vehicle_type_mantis::update_leg_turn_and_displacment(int32 vehicle_index, real32* final_turn, real_vector3d* final_leg_displacement, real_vector3d* displacement_delta, bool* same_displacement)
{
	return INVOKE_CLASS_MEMBER(0x00BCF250, c_vehicle_type_mantis, update_leg_turn_and_displacment, vehicle_index, final_turn, final_leg_displacement, displacement_delta, same_displacement);
}

void c_vehicle_type_mantis::update_movement_leg_turn_and_displacment(int32 vehicle_index, real32* final_turn, real_vector3d* final_leg_displacement)
{
	return INVOKE_CLASS_MEMBER(0x00BCF630, c_vehicle_type_mantis, update_movement_leg_turn_and_displacment, vehicle_index, final_turn, final_leg_displacement);
}

void c_vehicle_type_mantis::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	return INVOKE_CLASS_MEMBER(0x00BCFA10, c_vehicle_type_mantis, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_mantis::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	return INVOKE(0x00BD0030, c_vehicle_type_mantis::vector_is_upsides_down, vehicle_index, vector);
}

