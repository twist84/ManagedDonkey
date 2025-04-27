#pragma once

#include "cseries/cseries.hpp"

struct c_simulation_object_update_flags :
	public c_flags<long, uint64, 64>
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

	e_simulation_entity_type get_simulation_entity_type(long object_index);
};

struct object_placement_data;

extern void __cdecl simulation_action_game_engine_globals_update(c_flags<long, uint64, 64>& flags);
extern void __cdecl simulation_action_game_engine_player_update(long player_index, c_flags<long, uint64, 64>& flags);
extern void __cdecl simulation_action_object_create(long object_index);
extern void __cdecl simulation_action_object_force_update(long object_index, c_simulation_object_update_flags& flags);
extern void __cdecl simulation_action_object_update_internal(long object_index, c_simulation_object_update_flags& flags);
extern bool __cdecl simulation_query_object_is_predicted(long object_index);
extern bool __cdecl simulation_query_object_placement(object_placement_data const* data);

template<typename t_flag_enum>
void __cdecl simulation_action_object_update(long object_index, t_flag_enum flag);

extern bool __cdecl simulation_object_is_attached_to_distributed_networking(long object_index);

