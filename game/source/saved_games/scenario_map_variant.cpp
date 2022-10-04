#include "saved_games/scenario_map_variant.hpp"

#include <string.h>

c_map_variant::c_map_variant()
{
	memset(this, 0, sizeof(*this));

	DECLFUNC(0x00580B80, void, __thiscall, c_map_variant*)(this);
}