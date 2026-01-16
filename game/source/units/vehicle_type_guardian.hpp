#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct s_vehicle_guardian_definition;
struct render_model_definition;
class c_animation_channel;
struct s_havok_vehicle_physics_instance;
class c_vehicle_type_guardian
{
public:
	static void __cdecl adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration);
	static bool __cdecl compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active);
	static void __cdecl create_effects(int32 vehicle_index);
	void deplete_function_variables(int32 vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(int32 vehicle_index);
	c_vehicle_auto_turret* get_auto_turret();

private:
	const s_vehicle_guardian_definition* get_type_definition(int32 vehicle_index);

public:
	void handle_deleted_object(int32 vehicle_index, int32 deleted_object_index);
	static bool __cdecl is_stopped(int32 vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(int32 vehicle_index);
	static bool __cdecl physics_disabled(int32 vehicle_index);
	static void __cdecl process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(int32 vehicle_index);
	bool should_override_deactivation(int32 vehicle_index);
	void update_control(int32 vehicle_index);
	void update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance);
	static bool __cdecl vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector);

protected:
	real32 m_steering;
	real32 m_hover;
	c_vehicle_auto_turret m_auto_turret;
};
COMPILE_ASSERT(sizeof(c_vehicle_type_guardian) == 0x38);

