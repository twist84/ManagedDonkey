#pragma once

#include "game/materials.hpp"
#include "items/items.hpp"
#include "objects/damage_owner.hpp"

struct s_scenario_equipment;

struct _equipment_datum
{
	uns16 flags;
	byte __pad2[0x2];

	int32 game_time_at_creation;
	int32 last_use_time;
	uns8 charges_used;
	uns8 proximity_triggered_counter;

	uns8 __unknownE;
	uns8 __unknownF;
	int32 __unknown10;

	s_damage_owner creator_damage_owner;
	int32 looping_effect_index;

	int32 __unknown24;
	int32 __unknown28;
	int32 __unknown2C[16]; // child effect definition indices?
};
static_assert(sizeof(_equipment_datum) == 0x6C);

struct equipment_datum
{
	int32 definition_index;
	_object_datum object;
	_item_datum item;
	_equipment_datum equipment;
};
static_assert(sizeof(equipment_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_item_datum) + sizeof(_equipment_datum));

extern bool __cdecl equipment_activate(int32 equipment_index, int32 owner_unit_index, bool network_predicted);
extern real32 __cdecl equipment_active_fraction(int32 equipment_index);
extern string_id __cdecl equipment_animation_get_desired_idle(int32 equipment_index);
extern bool __cdecl equipment_animation_is_interruptable(int32 equipment_index, int32 animation_state, int32 desired_state);
extern bool __cdecl equipment_begin_animation_state(int32 equipment_index, string_id state_name, int32 goal_flags, uns32 playback_flags);
extern void __cdecl equipment_calculate_noise_maker_blip(int32 equipment_index, uns32* seed, int32 blip_index, real_point2d* local_position);
extern int32 __cdecl equipment_calculate_noise_maker_blip_count(int32 equipment_index);
extern void __cdecl equipment_calculate_spawn_location(int32 owner_unit_index, real32 spawn_radius_offset, real32 spawn_z_offset, real32 spawn_area_radius, int16 type, real_point3d* result_position, real_vector3d* result_vector);
extern bool __cdecl equipment_can_be_thrown(int32 equipment_index);
extern bool __cdecl equipment_compute_function_value(int32 object_index, int32 function, int32 function_owner_definition_index, real32* value, bool* active, bool* deterministic);
extern void __cdecl equipment_definition_handle_pickup(int32 player_index, int32 equipment_definition_index);
extern int32 __cdecl equipment_get_age(int32 equipment_index);
extern real32 __cdecl equipment_get_invincible_fraction(int32 equipment_index);
extern void __cdecl equipment_handle_pickup(int32 player_index, int32 equipment_index);
extern bool __cdecl equipment_is_active(int32 equipment_index);
extern bool __cdecl equipment_is_active_noise_maker(int32 equipment_index);
extern bool __cdecl equipment_new(int32 equipment_index, object_placement_data* data, bool* out_of_memory);
extern bool __cdecl equipment_override_damage_material_type(int32 equipment_index, c_global_material_type* material_type);
extern void __cdecl equipment_place(int32 equipment_index, const s_scenario_equipment* scenario_equipment);
extern int32 __cdecl equipment_remaining_charges(int32 equipment_index);
extern void __cdecl equipment_update(int32 equipment_index, int32 owner_unit_index);
extern bool __cdecl equipment_update(int32 equipment_index);
extern bool __cdecl equipment_update_animation(int32 equipment_index);
extern bool __cdecl equipment_update_phantoms(int32 equipment_index);

