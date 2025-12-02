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

	// some ticks
	uns8 __unknownD;

	uns8 __unknownE;
	uns8 __unknownF;
	int32 __unknown10;

	s_damage_owner creator_damage_owner;

	// creation effect definition index?
	int32 __unknown20;

	int32 __unknown24;
	int32 __unknown28;

	// child effect definition indices?
	int32 __unknown2C[16];
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
extern void __cdecl equipment_definition_handle_pickup(int32 player_index, int32 equipment_definition_index);

