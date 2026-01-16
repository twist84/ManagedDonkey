#pragma once

#include "animations/animation_id.hpp"
#include "math/real_math.hpp"

struct s_havok_vehicle_physics_instance;
struct render_model_definition;
class c_animation_channel;
class c_vehicle_type_vtol
{
public:
	static void __cdecl adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration);

private:
	static void __cdecl apply_thrust(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance, const real_vector3d* vector, const real_point3d* origin, int32 marker_name, real32 thrust, real_vector3d* force, real_vector3d* torque);
	real32 calculate_lift_turn_acceleration(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance);
	void compute_engine_scales(real32* left, real32* right) const;

public:
	bool compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active);

private:
	void compute_throttle(int32 vehicle_index, real_vector3d* throttle);

public:
	void create_effects(int32 vehicle_index);
	void deplete_function_variables(int32 vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(int32 vehicle_index);

private:
	void interpolate_lift_angles(int32 vehicle_index, real_euler_angles2d* angles, real_vector2d* velocity, const real_euler_angles2d* desired_angles, bool render_angles);

public:
	static bool __cdecl is_stopped(int32 vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(int32 vehicle_index);
	static bool __cdecl physics_disabled(int32 vehicle_index);
	void process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(int32 vehicle_index);
	static bool __cdecl should_override_deactivation(int32 vehicle_index);
	void update_control(int32 vehicle_index);

private:
	void update_lift_vector(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance, const real_vector3d* throttle, real32 desired_acceleration_k, int32 marker_name, real_vector3d* lift_vector, real_point3d* lift_origin, real_euler_angles2d* angles, real_euler_angles2d* render_angles, real_vector2d* velocity, real_vector2d* render_velocity, real32* effect_scale);

public:
	void update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance);

private:
	void update_rotor_damping(int32 vehicle_index);

public:
	static bool __cdecl vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector);

	enum e_lift_vector
	{
		_down_vector = 0,
		_forward_vector,
		_left_vector,
		_turn_vector,

		k_lift_vector_count
	};

	real32 m_steering;
	real32 m_turn;
	real32 m_target_elevation;
	uns8 m_lift_effect_scale_left_history[4];
	uns8 m_lift_effect_scale_right_history[4];
	real_euler_angles2d m_left_lift_angles;
	real_euler_angles2d m_left_render_lift_angles;
	real_euler_angles2d m_right_lift_angles;
	real_euler_angles2d m_right_render_lift_angles;
	real_vector2d m_left_lift_velocity;
	real_vector2d m_left_render_lift_velocity;
	real_vector2d m_right_lift_velocity;
	real_vector2d m_right_render_lift_velocity;
	c_animation_id m_lift_left_id;
	c_animation_id m_lift_right_id;
	real32 m_gear_position;
};
COMPILE_ASSERT(sizeof(c_vehicle_type_vtol) == 0x60);

extern real32 __cdecl calculate_acceleration_magnitude_vtol(bool govern_velocity, bool acceleration_as_force, real32 mass, real32 distance, real32 acceleration, real32 velocity_towards_target, real32 maximum_velocity, real32 dead_radius, real32 hookes_law_e, bool disable_damping, real32* scale);
extern real32 __cdecl calculate_desired_acceleration(real32 velocity, real32 minimum_acceleration, real32 maximum_acceleration, real32 distance);
extern real32 __cdecl calculate_desired_acceleration(real32 velocity, real32 gravity, real32 minimum_acceleration, real32 maximum_acceleration, real32 distance);

