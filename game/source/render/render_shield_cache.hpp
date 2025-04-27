#pragma once

#include "cseries/cseries.hpp"

struct shield_render_cache_message :
	s_datum_header
{
	uint8 __data[0x12];
};
static_assert(sizeof(shield_render_cache_message) == 0x14);

