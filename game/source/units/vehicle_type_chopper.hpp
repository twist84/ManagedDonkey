#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_chopper
{
public:
	static void __cdecl adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration);
	bool compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);
	static void __cdecl create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);
	static real_vector3d const* __cdecl get_debug_magic_force_vector(long vehicle_index, real_point3d* origin);
	s_vehicle_engine* const get_engine(long vehicle_index);
	bool is_e_braking(long vehicle_index);
	bool is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	void process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);

private:
	void update_boost(long vehicle_index, s_havok_vehicle_physics_instance* physics);

public:
	void update_control(long vehicle_index);

private:
	void update_damping(long vehicle_index, s_havok_vehicle_physics_instance* physics);

public:
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, real_vector3d const* vector);

protected:
	s_vehicle_engine m_engine;
	c_animation_id m_ground_speed_animation_id;
	c_animation_id m_turn_animation_id;
	real_vector3d m_last_relative_angular_velocity;
	real m_wheel;
	real m_wheel_speed;
	real m_turn;
	real m_bank;
	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;
	bool m_control_e_brake;
};
static_assert(sizeof(c_vehicle_type_chopper) == 0x3C);

