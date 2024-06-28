#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_definitions.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_type_alien_fighter.hpp"
#include "units/vehicle_type_alien_scout.hpp"
#include "units/vehicle_type_chopper.hpp"
#include "units/vehicle_type_guardian.hpp"
#include "units/vehicle_type_human_jeep.hpp"
#include "units/vehicle_type_human_plane.hpp"
#include "units/vehicle_type_human_tank.hpp"
#include "units/vehicle_type_mantis.hpp"
#include "units/vehicle_type_turret.hpp"
#include "units/vehicle_type_vtol.hpp"

struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_component
{
public:
	void adjust_gravity(long vehicle_index, vector3d* a2);
	bool auto_control(long vehicle_index);
	bool compute_function_value(long vehicle_index, long function_name, real* function_value, bool* function_result);
	void create_effects(long vehicle_index);
	void deplete_function_variables(long vehicle_index);
	bool effected_by_vehicle_ceiling(long vehicle_index);
	c_vehicle_auto_turret* get_auto_turret(long vehicle_index);
	bool get_auto_turret_damage_owner(long vehicle_index, s_damage_owner* auto_turret_damage_owner);
	s_vehicle_engine* const get_engine(long vehicle_index);
	c_vehicle_type_human_tank* get_type_human_tank();
	c_vehicle_type_human_jeep* get_type_human_jeep();
	c_vehicle_type_human_plane* get_type_human_plane();
	c_vehicle_type_alien_scout* get_type_alien_scout();
	c_vehicle_type_alien_fighter* get_type_alien_fighter();
	c_vehicle_type_turret* get_type_turret();
	c_vehicle_type_mantis* get_type_mantis();
	c_vehicle_type_vtol* get_type_vtol();
	c_vehicle_type_chopper* get_type_chopper();
	c_vehicle_type_guardian* get_type_guardian();
	void handle_deleted_object(long vehicle_index, long damage_owner_object_index);
	void handled_changed_vehicle_type(long vehicle_index);
	bool init_auto_turret(long vehicle_index, long trigger_volume, real a3, real a4, real a5, long object_index);
	bool is_e_braking(long vehicle_index);
	bool is_running_trick(long vehicle_index);
	bool is_stopped(long vehicle_index);
	bool kills_riders_at_terminal_velocity(long vehicle_index);
	bool physics_disabled(long vehicle_index);
	void process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* callback_data, c_animation_channel* channel, bool a5);
	void reset(long vehicle_index);
	void set_auto_turret_damage_owner(long vehicle_index, long object_index);
	bool should_override_deactivation(long vehicle_index);
	void start_trick(long vehicle_index, long trick_type);
	void update_control(long vehicle_index);
	void update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance);
	bool vector_is_upsides_down(long vehicle_index, vector3d const* vector);

protected:
	union
	{
		c_vehicle_type_human_tank m_type_human_tank;
		c_vehicle_type_human_jeep m_type_human_jeep;
		c_vehicle_type_human_plane m_type_human_plane;
		c_vehicle_type_alien_scout m_type_alien_scout;
		c_vehicle_type_alien_fighter m_type_alien_fighter;
		c_vehicle_type_turret m_type_turret;
		c_vehicle_type_mantis m_type_mantis;
		c_vehicle_type_vtol m_type_vtol;
		c_vehicle_type_chopper m_type_chopper;
		c_vehicle_type_guardian m_type_guardian;

		byte m_type_storage[0x60];
	};

	c_enum<e_vehicle_type, char, _vehicle_type_human_tank, k_vehicle_type_count> m_initialization_type;

	byte __data61[0x43];
};
static_assert(sizeof(c_vehicle_type_component) == 0xA4);

