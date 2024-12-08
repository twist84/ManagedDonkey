#pragma once

#include "cseries/cseries.hpp"

struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_alien_scout
{
public:
	static void __cdecl adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration);
	bool compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);

private:
	void compute_throttle(long vehicle_index, real_vector3d* throttle);

public:
	static void __cdecl create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	void process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);
	void update_control(long vehicle_index);
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, real_vector3d const* vector);

protected:
	real m_engine_function_position;
	real m_engine_function_velocity;
	real m_boost_function_velocity;
	real m_steering;
	real m_hover;
	real m_gear_position;
	real m_turn_animation_position;
	c_animation_id m_turn_animation_id;
};
static_assert(sizeof(c_vehicle_type_alien_scout) == 0x20);

