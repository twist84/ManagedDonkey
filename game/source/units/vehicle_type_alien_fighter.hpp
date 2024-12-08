#pragma once

#include "cseries/cseries.hpp"

enum e_vehicle_trick_type
{
	_vehicle_trick_type_none = 0,
	_vehicle_trick_type_left_barrel_roll,
	_vehicle_trick_type_right_barrel_roll,
	_vehicle_trick_type_back_loop,

	k_vehicle_trick_type_count
};

struct s_havok_vehicle_physics_instance;
struct render_model_definition;
struct c_animation_channel;
struct c_vehicle_type_alien_fighter
{
public:
	void adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration);

private:
	bool can_select_new_trick(long vehicle_index);

public:
	bool compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);

private:
	static e_vehicle_trick_type __cdecl compute_new_trick_type(long vehicle_index);

	bool compute_reference_and_trick_rotation(long vehicle_index, s_havok_vehicle_physics_instance const* instance, real_matrix3x3* reference_rotation, real_matrix3x3* trick_rotation);
	real compute_trick_t(long vehicle_index);

public:
	static void __cdecl create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);
	bool is_running_trick(long vehicle_index);
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	void process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);
	void start_trick(long vehicle_index, e_vehicle_trick_type vehicle_trick_type);

private:
	real trick_duration(long vehicle_index);
public:
	void update_control(long vehicle_index);
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, real_vector3d const* vector);

protected:
	c_animation_id m_turn_animation_id;
	real m_thrust;
	real m_speed;
	real m_turn;
	real m_slide;
	real m_speed_anti_gravity_fraction;
	char m_trick_type;
	byte m_trick_tick;
};
static_assert(sizeof(c_vehicle_type_alien_fighter) == 0x1C);

