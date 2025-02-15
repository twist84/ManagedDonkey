#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"

struct _equipment_datum
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
static_assert(sizeof(_equipment_datum) == 0x6C);

struct equipment_datum
{
	long definition_index;
	_object_datum object;
	_item_datum item;
	_equipment_datum equipment;
};
static_assert(sizeof(equipment_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_item_datum) + sizeof(_equipment_datum));

