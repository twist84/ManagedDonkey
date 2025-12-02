#pragma once

#include "cseries/cseries.hpp"

enum e_weapon_addition_method;

struct object_placement_data;
struct s_game_engine_event_data;

class c_simulation_object_update_flags :
	public c_flags<int32, uns64, 64>
{
public:
	c_simulation_object_update_flags();
	c_simulation_object_update_flags(int32 object_index, e_simulation_object_update_flag flag);
	c_simulation_object_update_flags(int32 weapon_index, e_simulation_weapon_update_flag flag);
	c_simulation_object_update_flags(int32 generic_index, e_simulation_generic_update_flag flag);
	c_simulation_object_update_flags(int32 item_index, e_simulation_item_update_flag flag);
	c_simulation_object_update_flags(int32 projectile_index, e_simulation_projectile_update_flag flag);
	c_simulation_object_update_flags(int32 vehicle_index, e_simulation_vehicle_update_flag flag);
	c_simulation_object_update_flags(int32 device_index, e_simulation_device_update_flag flag);
	c_simulation_object_update_flags(int32 unit_index, e_simulation_unit_update_flag flag);
	
	void set_flag(int32 device_index, e_simulation_device_update_flag flag);
	void set_flag(int32 generic_index, e_simulation_generic_update_flag flag);
	void set_flag(int32 item_index, e_simulation_item_update_flag flag);
	void set_flag(int32 object_index, e_simulation_object_update_flag flag);
	void set_flag(int32 projectile_index, e_simulation_projectile_update_flag flag);
	void set_flag(int32 unit_index, e_simulation_unit_update_flag flag);
	void set_flag(int32 vehicle_index, e_simulation_vehicle_update_flag flag);
	void set_flag(int32 weapon_index, e_simulation_weapon_update_flag flag);

	e_simulation_entity_type get_simulation_entity_type(int32 object_index);
};


extern void __cdecl simulation_action_game_engine_globals_update(c_flags<int32, uns64, 64>& flags);
extern void __cdecl simulation_action_game_engine_player_update(int32 player_index, c_flags<int32, uns64, 64>& flags);
extern void __cdecl simulation_action_multiplayer_event(const s_game_engine_event_data* in_event_data);
extern void __cdecl simulation_action_object_create(int32 object_index);
extern void __cdecl simulation_action_object_force_update(int32 object_index, c_simulation_object_update_flags& flags);
extern void __cdecl simulation_action_object_update_internal(int32 object_index, c_simulation_object_update_flags& flags);
extern void __cdecl simulation_action_pickup_powerup(int32 unit_index, int32 equipment_index);
extern bool __cdecl simulation_query_object_is_predicted(int32 object_index);
extern bool __cdecl simulation_query_object_placement(const object_placement_data* data);
extern void __cdecl simulation_request_autopickup_ammo(int32 unit_index, int32 weapon_index, int32 item_index);
extern void __cdecl simulation_request_autopickup_equipment(int32 unit_index, int32 equipment_index);
extern void __cdecl simulation_request_autopickup_grenade(int32 unit_index, int32 item_index);
extern void __cdecl simulation_request_autopickup_powerup(int32 unit_index, int32 item_index);
extern void __cdecl simulation_request_object_refresh(int32 object_index);
extern void __cdecl simulation_request_pickup_weapon(int32 unit_index, int32 weapon_index, e_weapon_addition_method add_weapon_mode);

template<typename t_flag_enum>
void __cdecl simulation_action_object_update(int32 object_index, t_flag_enum flag);

extern bool __cdecl simulation_object_is_attached_to_distributed_networking(int32 object_index);

