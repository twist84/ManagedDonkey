#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"

struct weapon_data
{
	word_flags flags;

	byte __data2[0x14E];
};
static_assert(sizeof(weapon_data) == 0x150);

struct weapon_datum
{
	object_data object;
	item_data item;
	weapon_data weapon;
};
static_assert(sizeof(weapon_datum) == sizeof(object_data) + sizeof(item_data) + sizeof(weapon_data));

extern real __cdecl weapon_get_age(long weapon_index);
extern real __cdecl weapon_get_field_of_view_change_time(long weapon_index);
extern bool __cdecl weapon_has_infinite_ammo(long weapon_index);

