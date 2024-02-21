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

