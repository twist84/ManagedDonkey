#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct s_vehicle_guardian_definition;
struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_guardian
{
public:
	static void __cdecl adjust_gravity(long vehicle_index, vector3d* gravity_acceleration);
	static bool __cdecl compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);
	static void __cdecl create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);
	c_vehicle_auto_turret* get_auto_turret();

private:
	s_vehicle_guardian_definition const* get_type_definition(long vehicle_index);

public:
	void handle_deleted_object(long vehicle_index, long deleted_object_index);
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	static void __cdecl process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	bool should_override_deactivation(long vehicle_index);
	void update_control(long vehicle_index);
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, vector3d const* vector);

protected:
	real m_steering;
	real m_hover;
	c_vehicle_auto_turret m_auto_turret;
};
static_assert(sizeof(c_vehicle_type_guardian) == 0x38);

