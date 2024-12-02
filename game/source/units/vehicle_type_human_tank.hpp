#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct s_havok_vehicle_physics_instance;
struct render_model_definition;
struct c_animation_channel;
struct c_vehicle_type_human_tank
{
public:
	void adjust_gravity(long vehicle_index, vector3d* gravity_acceleration);
	bool compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);
private:
	void compute_tread_speeds(long vehicle_index, real* left_tread_speed, real* right_tread_speed);
public:
	void create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	bool effected_by_vehicle_ceiling(long vehicle_index);
	s_vehicle_engine* const get_engine(long vehicle_index);
	bool is_stopped(long vehicle_index);
	bool kills_riders_at_terminal_velocity(long vehicle_index);
	bool physics_disabled(long vehicle_index);
	static void __cdecl process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);
	void update_control(long vehicle_index);
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	bool vector_is_upsides_down(long vehicle_index, vector3d const* vector);

protected:
	s_vehicle_engine m_engine;
	c_animation_id m_ground_speed_animation_id;
	real m_differential;
	real m_left_tread;
	real m_right_tread;
	c_vehicle_stop_counter m_stop_counter;
};
static_assert(sizeof(c_vehicle_type_human_tank) == 0x28);

