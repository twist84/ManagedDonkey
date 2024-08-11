#pragma once

#include "cseries/cseries.hpp"

// you can thank @yokimklein on Github for these

enum e_simulation_object_update_flag
{
	_simulation_object_update_dead = 0,
	_simulation_object_update_position,
	_simulation_object_update_forward_and_up,
	_simulation_object_update_scale,
	_simulation_object_update_translational_velocity,
	_simulation_object_update_angular_velocity,
	_simulation_object_update_body_vitality, // vitality & stun ticks
	_simulation_object_update_shield_vitality,
	_simulation_object_update_region_state,
	_simulation_object_update_constraint_state,
	_simulation_object_update_owner_team_index,
	_simulation_object_update_parent_state,
	_simulation_object_update_multiplayer_properties,
	_simulation_object_update_map_variant_index,

	k_simulation_object_update_flag_count
};

enum e_simulation_unit_update_flag
{
	_simulation_unit_update_control = k_simulation_object_update_flag_count,
	_simulation_unit_update_parent_vehicle,
	_simulation_unit_update_desired_aiming_vector,
	_simulation_unit_update_desired_weapon_set,
	_simulation_unit_update_weapon1_type,
	_simulation_unit_update_weapon2_type,
	_simulation_unit_update_weapon3_type,
	_simulation_unit_update_weapon4_type,
	_simulation_unit_update_weapon1_state,
	_simulation_unit_update_weapon2_state,
	_simulation_unit_update_weapon3_state,
	_simulation_unit_update_weapon4_state,
	_simulation_unit_update_grenade_counts,
	_simulation_unit_update_active_camo,
	_simulation_unit_update_equipment,
	_simulation_unit_update_equipment_charges,
	_simulation_unit_update_consumable_energy,
	_simulation_unit_update_control_context,
	_simulation_unit_update_assassination_data,

	k_simulation_unit_update_flag_count
};

enum e_simulation_item_update_flag
{
	_simulation_item_update_set_at_rest = k_simulation_object_update_flag_count,
	_simulation_item_update_equipment_begin_animation_state,
	_simulation_item_update_equipment_creation_time,
	_simulation_item_update_equipment_owner,

	k_simulation_item_update_flag_count
};

enum e_simulation_projectile_update_flag
{
	_simulation_projectile_update_set_at_rest = k_simulation_object_update_flag_count,

	k_simulation_projectile_update_flag_count
};

enum e_simulation_generic_update_flag
{
	_simulation_generic_update_rigid_body = k_simulation_object_update_flag_count,

	k_simulation_generic_update_flag_count
};

enum e_simulation_weapon_update_flag
{
	_simulation_weapon_update_weapon_flags = k_simulation_item_update_flag_count,
	_simulation_weapon_update_multiplayer_weapon_registration,
	_simulation_weapon_update_ammo,

	k_simulation_weapon_update_flag_count
};

enum e_simulation_device_update_flag
{
	_simulation_device_update_position = k_simulation_object_update_flag_count,
	_simulation_device_update_position_group,
	_simulation_device_update_power,
	_simulation_device_update_power_group,

	k_simulation_device_update_flag_count
};

enum e_simulation_vehicle_update_flag
{
	_simulation_vehicle_update_auto_turret = k_simulation_object_update_flag_count,
	_simulation_vehicle_update_auto_turret_tracking,
	_simulation_vehicle_update_seat_power,
	_simulation_vehicle_update_active_camo,

	k_simulation_vehicle_update_flag_count
};

struct c_simulation_object_update_flags
{
public:
	c_simulation_object_update_flags();
	c_simulation_object_update_flags(long object_index, e_simulation_object_update_flag flag);
	c_simulation_object_update_flags(long weapon_index, e_simulation_weapon_update_flag flag);
	c_simulation_object_update_flags(long generic_index, e_simulation_generic_update_flag flag);
	c_simulation_object_update_flags(long item_index, e_simulation_item_update_flag flag);
	c_simulation_object_update_flags(long projectile_index, e_simulation_projectile_update_flag flag);
	c_simulation_object_update_flags(long vehicle_index, e_simulation_vehicle_update_flag flag);
	c_simulation_object_update_flags(long device_index, e_simulation_device_update_flag flag);
	c_simulation_object_update_flags(long unit_index, e_simulation_unit_update_flag flag);
	
	void set_flag(long device_index, e_simulation_device_update_flag flag);
	void set_flag(long generic_index, e_simulation_generic_update_flag flag);
	void set_flag(long item_index, e_simulation_item_update_flag flag);
	void set_flag(long object_index, e_simulation_object_update_flag flag);
	void set_flag(long projectile_index, e_simulation_projectile_update_flag flag);
	void set_flag(long unit_index, e_simulation_unit_update_flag flag);
	void set_flag(long vehicle_index, e_simulation_vehicle_update_flag flag);
	void set_flag(long weapon_index, e_simulation_weapon_update_flag flag);

protected:
	c_flags<long, qword, 64> m_flags;
};

struct object_placement_data;

extern void __cdecl simulation_action_game_engine_globals_update(c_flags<long, qword, 64>& flags);
extern void __cdecl simulation_action_game_engine_player_update(long player_index, c_flags<long, qword, 64>& flags);
extern void __cdecl simulation_action_object_create(long object_index);
extern void __cdecl simulation_action_object_update_internal(long object_index, c_simulation_object_update_flags& flags);
extern bool __cdecl simulation_query_object_is_predicted(long object_index);
extern bool __cdecl simulation_query_object_placement(object_placement_data const* data);

template<typename t_flag_enum>
void __cdecl simulation_action_object_update(long object_index, t_flag_enum flag);

enum e_simulation_entity_type;

extern bool simulation_entity_type_is_object(e_simulation_entity_type entity_type);

