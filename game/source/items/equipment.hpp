#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"

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
extern bool __cdecl equipment_begin_animation_state(int32 equipment_index, string_id state_name, int32 goal_flags, uns32 playback_flags);
extern bool __cdecl equipment_can_be_thrown(int32 equipment_index);
extern void __cdecl equipment_definition_handle_pickup(int32 player_index, int32 equipment_definition_index);
extern int32 __cdecl equipment_remaining_charges(int32 equipment_index);
extern void __cdecl equipment_update(int32 equipment_index, int32 owner_unit_index);
extern bool __cdecl equipment_update(int32 equipment_index);

