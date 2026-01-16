#pragma once

#include "cseries/cseries.hpp"

struct shield_render_cache_message :
	s_datum_header
{
	byte __data[0x12];
};
COMPILE_ASSERT(sizeof(shield_render_cache_message) == 0x14);

