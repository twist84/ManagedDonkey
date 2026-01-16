#pragma once

#include "animations/animation_id.hpp"
#include "math/real_math.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct render_model_definition;
class c_animation_channel;
struct s_havok_vehicle_physics_instance;
class c_vehicle_type_human_jeep
{
public:
	void adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration);
	bool compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active);
	static void __cdecl create_effects(int32 vehicle_index);
	void deplete_function_variables(int32 vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(int32 vehicle_index);
	s_vehicle_engine* const get_engine(int32 vehicle_index);
	static bool __cdecl is_e_braking(int32 vehicle_index);
	bool is_stopped(int32 vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(int32 vehicle_index);
	static bool __cdecl physics_disabled(int32 vehicle_index);
	void process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(int32 vehicle_index);
	bool should_override_deactivation(int32 vehicle_index);
	void update_control(int32 vehicle_index);
	void update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector);

protected:
	s_vehicle_engine m_engine;
	c_animation_id m_ground_speed_animation_id;
	c_animation_id m_ground_rear_animation_id;
	c_animation_id m_turn_animation_id;
	real_vector3d m_grounded_up_vector;
	real32 m_wheel;
	real32 m_rear_wheel;
	real32 m_turn;
	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;
	uns8 m_control_e_brake;
};
COMPILE_ASSERT(sizeof(c_vehicle_type_human_jeep) == 0x3C);

