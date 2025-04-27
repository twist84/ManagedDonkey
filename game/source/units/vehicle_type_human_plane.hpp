#pragma once

#include "cseries/cseries.hpp"

struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_human_plane
{
public:
	static void __cdecl adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration);
	bool compute_function_value(long vehicle_index, long function, real32* magnitude, bool* force_active);
	void create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	void process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);
	void update_control(long vehicle_index);
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, real_vector3d const* vector);

protected:
	real32 m_speed;
	real32 m_slide;
	real32 m_rise;
	real32 m_steering;
	real32 m_turn_animation_position;
	c_animation_id m_turn_animation_id;
};
static_assert(sizeof(c_vehicle_type_human_plane) == 0x18);

