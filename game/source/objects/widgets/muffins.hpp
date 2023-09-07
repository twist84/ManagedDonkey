#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct muffin_datum : s_datum_header
{
	byte __data[0x188E];
};
static_assert(sizeof(muffin_datum) == 0x1890);

