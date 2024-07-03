#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct _item_datum
{
	byte_flags flags;
	byte inventory_state;
	short detonation_ticks;
	long ignore_object_index;
	long last_relevant_time;
	long inventory_unit_index;
	long last_inventory_unit_index;
};
static_assert(sizeof(_item_datum) == 0x14);

struct item_datum
{
	_object_datum object;
	_item_datum item;
};
static_assert(sizeof(item_datum) == sizeof(_object_datum) + sizeof(_item_datum));

extern bool __cdecl dangerous_items_near_player(long* out_item_index);

