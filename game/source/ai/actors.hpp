#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct actor_datum :
	s_datum_header
{
	byte __data[0xA96];
};
static_assert(sizeof(actor_datum) == 0xA98);

struct ai_reference_frame
{
	byte __data[0x4B0];
};
static_assert(sizeof(ai_reference_frame) == 0x4B0);

struct actor_iterator
{
	actor_datum* datum;
	s_data_iterator iterator;
	bool __unknown10;
	long actor_index;
};
static_assert(sizeof(actor_iterator) == 0x18);

extern void __cdecl actor_iterator_new(actor_iterator* iterator, bool a2);
extern actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator);

