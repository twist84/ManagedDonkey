#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_mantis_definition;
struct render_model_definition;
class c_animation_channel;
struct s_havok_vehicle_physics_instance;
class c_vehicle_type_mantis
{
public:
	void adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration);

private:
	void calculate_throttle(int32 vehicle_index, real_vector3d* throttle);

public:
	static bool __cdecl compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active);
	static void __cdecl create_effects(int32 vehicle_index);
	void deplete_function_variables(int32 vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(int32 vehicle_index);

private:
	const s_vehicle_mantis_definition* get_type_definition(int32 vehicle_index);

public:
	static bool __cdecl is_stopped(int32 vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(int32 vehicle_index);
	static bool __cdecl physics_disabled(int32 vehicle_index);
	static void __cdecl process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(int32 vehicle_index);
	bool should_override_deactivation(int32 vehicle_index);
	void update_control(int32 vehicle_index);

private:
	void update_leg_turn_and_displacment(int32 vehicle_index, real32* final_turn, real_vector3d* final_leg_displacement, real_vector3d* displacement_delta, bool* same_displacement);
	void update_movement_leg_turn_and_displacment(int32 vehicle_index, real32* final_turn, real_vector3d* final_leg_displacement);

public:
	void update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector);

protected:
	real32 m_current_leg_turn;
	real_vector3d m_last_up_vector;
	real_vector2d m_foot_motion;
	real32 m_turn_control;
	real32 m_steering;
	real32 m_speed;
	real32 m_slide;
	int8 m_planted_leg_group;
	int8 m_legs_planted_ticks;
	int32 m_jump_time;
};
COMPILE_ASSERT(sizeof(c_vehicle_type_mantis) == 0x30);

