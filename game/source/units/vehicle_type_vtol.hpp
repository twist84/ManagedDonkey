#pragma once

#include "cseries/cseries.hpp"

struct s_havok_vehicle_physics_instance;
struct render_model_definition;
struct c_animation_channel;
struct c_vehicle_type_vtol
{
public:
	static void __cdecl adjust_gravity(long vehicle_index, vector3d* gravity_acceleration);

private:
	static void __cdecl apply_thrust(long vehicle_index, s_havok_vehicle_physics_instance const* instance, vector3d const* vector, real_point3d const* origin, long marker_name, real thrust, vector3d* force, vector3d* torque);
	real calculate_lift_turn_acceleration(long vehicle_index, s_havok_vehicle_physics_instance const* instance);
	void compute_engine_scales(real* left, real* right) const;

public:
	bool compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);

private:
	void compute_throttle(long vehicle_index, vector3d* throttle);

public:
	void create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);

private:
	void interpolate_lift_angles(long vehicle_index, euler_angles2d* angles, vector2d* velocity, euler_angles2d const* desired_angles, bool render_angles);

public:
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	void process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	static bool __cdecl should_override_deactivation(long vehicle_index);
	void update_control(long vehicle_index);

private:
	void update_lift_vector(long vehicle_index, s_havok_vehicle_physics_instance const* instance, vector3d const* throttle, real desired_acceleration_k, long marker_name, vector3d* lift_vector, real_point3d* lift_origin, euler_angles2d* angles, euler_angles2d* render_angles, vector2d* velocity, vector2d* render_velocity, real* effect_scale);

public:
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);

private:
	void update_rotor_damping(long vehicle_index);

public:
	static bool __cdecl vector_is_upsides_down(long vehicle_index, vector3d const* vector);

	enum e_lift_vector
	{
		_down_vector = 0,
		_forward_vector,
		_left_vector,
		_turn_vector,

		k_lift_vector_count
	};

	real m_steering;
	real m_turn;
	real m_target_elevation;
	byte m_lift_effect_scale_left_history[4];
	byte m_lift_effect_scale_right_history[4];
	euler_angles2d m_left_lift_angles;
	euler_angles2d m_left_render_lift_angles;
	euler_angles2d m_right_lift_angles;
	euler_angles2d m_right_render_lift_angles;
	vector2d m_left_lift_velocity;
	vector2d m_left_render_lift_velocity;
	vector2d m_right_lift_velocity;
	vector2d m_right_render_lift_velocity;
	c_animation_id m_lift_left_id;
	c_animation_id m_lift_right_id;
	real m_gear_position;
};
static_assert(sizeof(c_vehicle_type_vtol) == 0x60);

extern real __cdecl calculate_acceleration_magnitude_vtol(bool govern_velocity, bool acceleration_as_force, real mass, real distance, real acceleration, real velocity_towards_target, real maximum_velocity, real dead_radius, real hookes_law_e, bool disable_damping, real* scale);
extern real __cdecl calculate_desired_acceleration(real velocity, real minimum_acceleration, real maximum_acceleration, real distance);
extern real __cdecl calculate_desired_acceleration(real velocity, real gravity, real minimum_acceleration, real maximum_acceleration, real distance);

