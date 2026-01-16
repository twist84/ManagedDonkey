#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_definitions.hpp"
#include "units/vehicle_engine.hpp"

class c_vehicle_auto_turret;
struct s_vehicle_engine;
class c_vehicle_type_human_tank;
class c_vehicle_type_human_jeep;
class c_vehicle_type_human_plane;
class c_vehicle_type_alien_scout;
class c_vehicle_type_alien_fighter;
class c_vehicle_type_turret;
class c_vehicle_type_mantis;
class c_vehicle_type_vtol;
class c_vehicle_type_chopper;
class c_vehicle_type_guardian;
struct s_damage_owner;
struct render_model_definition;
class c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct __declspec(align(4)) c_vehicle_type_component
{
public:
	void adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration);
	bool auto_control(int32 vehicle_index);
	bool compute_function_value(int32 vehicle_index, int32 function_name, real32* function_value, bool* function_result);
	void create_effects(int32 vehicle_index);
	void deplete_function_variables(int32 vehicle_index);
	bool effected_by_vehicle_ceiling(int32 vehicle_index);
	c_vehicle_auto_turret* get_auto_turret(int32 vehicle_index);
	bool get_auto_turret_damage_owner(int32 vehicle_index, s_damage_owner* damage_owner);
	const s_vehicle_engine* get_engine(int32 vehicle_index);
	c_vehicle_type_human_tank* get_type_human_tank();
	c_vehicle_type_human_jeep* get_type_human_jeep();
	c_vehicle_type_human_plane* get_type_human_plane();
	c_vehicle_type_alien_fighter* get_type_alien_fighter();
	c_vehicle_type_alien_scout* get_type_alien_scout();
	c_vehicle_type_turret* get_type_turret();
	c_vehicle_type_mantis* get_type_mantis();
	c_vehicle_type_guardian* get_type_guardian();
	c_vehicle_type_vtol* get_type_vtol();
	c_vehicle_type_chopper* get_type_chopper();
	void handle_deleted_object(int32 vehicle_index, int32 damage_owner_object_index);
	void handled_changed_vehicle_type(int32 vehicle_index);
	bool init_auto_turret(int32 vehicle_index, int32 trigger_volume, real32 min_range, real32 alt_range, real32 alt_time, int32 object_index);
	bool is_e_braking(int32 vehicle_index);
	bool is_running_trick(int32 vehicle_index);
	bool is_stopped(int32 vehicle_index);
	bool kills_riders_at_terminal_velocity(int32 vehicle_index);
	bool physics_disabled(int32 vehicle_index);
	void process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(int32 vehicle_index);
	void set_auto_turret_damage_owner(int32 vehicle_index, int32 damage_owner_index);
	bool should_override_deactivation(int32 vehicle_index);
	void start_trick(int32 vehicle_index, e_vehicle_trick_type trick_type);
	void update_control(int32 vehicle_index);
	void update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance);
	bool vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector);

protected:
	union
	{
		//c_vehicle_type_human_tank m_type_human_tank;
		//c_vehicle_type_human_jeep m_type_human_jeep;
		//c_vehicle_type_human_plane m_type_human_plane;
		//c_vehicle_type_alien_scout m_type_alien_scout;
		//c_vehicle_type_alien_fighter m_type_alien_fighter;
		//c_vehicle_type_turret m_type_turret;
		//c_vehicle_type_mantis m_type_mantis;
		//c_vehicle_type_vtol m_type_vtol;
		//c_vehicle_type_chopper m_type_chopper;
		//c_vehicle_type_guardian m_type_guardian;
	};

	byte m_vehicle_type_buffer[0x60];
	c_enum<e_vehicle_type, int8, _vehicle_type_human_tank, k_vehicle_type_count> m_initialization_type;
};
COMPILE_ASSERT(sizeof(c_vehicle_type_component) == 0x64);

