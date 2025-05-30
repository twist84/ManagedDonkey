#include "units/vehicle_type_human_jeep.hpp"

#include "motor/vehicle_motor_program.hpp"

void c_vehicle_type_human_jeep::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	INVOKE_CLASS_MEMBER(0x00BC8770, c_vehicle_type_human_jeep, adjust_gravity, vehicle_index, gravity_acceleration);
}

//.text:00BC87D0 ; 

bool c_vehicle_type_human_jeep::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	return INVOKE_CLASS_MEMBER(0x00BC8840, c_vehicle_type_human_jeep, compute_function_value, vehicle_index, function, magnitude, force_active);
}

void __cdecl c_vehicle_type_human_jeep::create_effects(int32 vehicle_index)
{
	INVOKE(0x00BC8BC0, c_vehicle_type_human_jeep::create_effects, vehicle_index);
}

void c_vehicle_type_human_jeep::deplete_function_variables(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BC8BD0, c_vehicle_type_human_jeep, deplete_function_variables, vehicle_index);
}

bool __cdecl c_vehicle_type_human_jeep::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	return INVOKE(0x00BC8BF0, c_vehicle_type_human_jeep::effected_by_vehicle_ceiling, vehicle_index);
}

//.text:00BC8C00 ; 

s_vehicle_engine* const c_vehicle_type_human_jeep::get_engine(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00BC8C10, c_vehicle_type_human_jeep, get_engine, vehicle_index);

	return &m_engine;
}

bool __cdecl c_vehicle_type_human_jeep::is_e_braking(int32 vehicle_index)
{
	return INVOKE(0x00BC8C20, c_vehicle_type_human_jeep::is_e_braking, vehicle_index);
}

bool c_vehicle_type_human_jeep::is_stopped(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC8C60, c_vehicle_type_human_jeep, is_stopped, vehicle_index);
}

bool __cdecl c_vehicle_type_human_jeep::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	return INVOKE(0x00BC8C90, c_vehicle_type_human_jeep::kills_riders_at_terminal_velocity, vehicle_index);
}

bool __cdecl c_vehicle_type_human_jeep::physics_disabled(int32 vehicle_index)
{
	return INVOKE(0x00BC8CA0, c_vehicle_type_human_jeep::physics_disabled, vehicle_index);
}

void c_vehicle_type_human_jeep::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	INVOKE_CLASS_MEMBER(0x00BC8CB0, c_vehicle_type_human_jeep, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);
}

void c_vehicle_type_human_jeep::reset(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BC8E90, c_vehicle_type_human_jeep, reset, vehicle_index);
}

bool c_vehicle_type_human_jeep::should_override_deactivation(int32 vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC8EE0, c_vehicle_type_human_jeep, should_override_deactivation, vehicle_index);
}

//.text:00BC8F20 ; 

void c_vehicle_type_human_jeep::update_control(int32 vehicle_index)
{
	INVOKE_CLASS_MEMBER(0x00BC8F40, c_vehicle_type_human_jeep, update_control, vehicle_index);
}

void c_vehicle_type_human_jeep::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	INVOKE_CLASS_MEMBER(0x00BC9050, c_vehicle_type_human_jeep, update_physics, vehicle_index, instance);
}

bool __cdecl c_vehicle_type_human_jeep::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	return INVOKE(0x00BC9670, c_vehicle_type_human_jeep::vector_is_upsides_down, vehicle_index, vector);
}

