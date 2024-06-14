#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"

struct equipment_data
{
	word_flags flags;
	byte __pad2[0x2];

	long __time4;
	long __time8;

	char current_use_count;

	// some ticks
	byte __unknownD;

	byte __unknownE;
	byte __unknownF;
	long __unknown10;

	s_damage_owner creator_damage_owner;

	// creation effect definition index?
	long __unknown20;

	long __unknown24;
	long __unknown28;

	// child effect definition indices?
	long __unknown2C[16];
};
static_assert(sizeof(equipment_data) == 0x6C);

struct equipment_datum
{
	object_data object;
	item_data item;
	equipment_data equipment;
};
static_assert(sizeof(equipment_datum) == sizeof(object_data) + sizeof(item_data) + sizeof(equipment_data));

