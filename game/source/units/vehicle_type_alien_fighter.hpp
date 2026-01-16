#pragma once

#include "animations/animation_id.hpp"
#include "math/real_math.hpp"

struct s_havok_vehicle_physics_instance;
struct render_model_definition;
class c_animation_channel;
class c_vehicle_type_alien_fighter
{
public:
	void adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration);

private:
	bool can_select_new_trick(int32 vehicle_index);

public:
	bool compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active);

private:
	static e_vehicle_trick_type __cdecl compute_new_trick_type(int32 vehicle_index);

	bool compute_reference_and_trick_rotation(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance, real_matrix3x3* reference_rotation, real_matrix3x3* trick_rotation);
	real32 compute_trick_t(int32 vehicle_index);

public:
	static void __cdecl create_effects(int32 vehicle_index);
	void deplete_function_variables(int32 vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(int32 vehicle_index);
	bool is_running_trick(int32 vehicle_index);
	static bool __cdecl is_stopped(int32 vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(int32 vehicle_index);
	static bool __cdecl physics_disabled(int32 vehicle_index);
	void process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(int32 vehicle_index);
	bool should_override_deactivation(int32 vehicle_index);
	void start_trick(int32 vehicle_index, e_vehicle_trick_type vehicle_trick_type);

private:
	real32 trick_duration(int32 vehicle_index);
public:
	void update_control(int32 vehicle_index);
	void update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector);

protected:
	c_animation_id m_turn_animation_id;
	real32 m_thrust;
	real32 m_speed;
	real32 m_turn;
	real32 m_slide;
	real32 m_speed_anti_gravity_fraction;
	int8 m_trick_type;
	uns8 m_trick_tick;
};
COMPILE_ASSERT(sizeof(c_vehicle_type_alien_fighter) == 0x1C);

