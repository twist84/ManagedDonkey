#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_mantis_definition;
struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_mantis
{
public:
	void adjust_gravity(long vehicle_index, vector3d* gravity_acceleration);

private:
	void calculate_throttle(long vehicle_index, vector3d* throttle);

public:
	static bool __cdecl compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);
	static void __cdecl create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);

private:
	s_vehicle_mantis_definition const* get_type_definition(long vehicle_index);

public:
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	static void __cdecl process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);
	void update_control(long vehicle_index);

private:
	void update_leg_turn_and_displacment(long vehicle_index, real* final_turn, vector3d* final_leg_displacement, vector3d* displacement_delta, bool* same_displacement);
	void update_movement_leg_turn_and_displacment(long vehicle_index, real* final_turn, vector3d* final_leg_displacement);

public:
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, vector3d const* vector);

protected:
	real m_current_leg_turn;
	vector3d m_last_up_vector;
	vector2d m_foot_motion;
	real m_turn_control;
	real m_steering;
	real m_speed;
	real m_slide;
	char m_planted_leg_group;
	char m_legs_planted_ticks;
	long m_jump_time;
};
static_assert(sizeof(c_vehicle_type_mantis) == 0x30);

