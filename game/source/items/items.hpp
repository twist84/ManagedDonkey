#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct item_data
{
	byte_flags flags;
	byte inventory_state;
	short detonationTicks;
	long ignore_object_index;
	long last_relevant_time;
	long inventory_unit_index;
	long last_inventory_unit_index;
};
static_assert(sizeof(item_data) == 0x14);

struct item_datum
{
	object_data object;
	item_data item;
};
static_assert(sizeof(item_datum) == sizeof(object_data) + sizeof(item_data));

extern bool __cdecl dangerous_items_near_player(long* out_item_index);

