#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle :
	s_datum_header
{
	byte __data[0x7A];
};
static_assert(sizeof(c_particle) == 0x7C);

