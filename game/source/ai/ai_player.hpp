#pragma once

#include "cseries/cseries.hpp"

struct ai_player_state
{
	byte __data[0xB0];
};
static_assert(sizeof(ai_player_state) == 0xB0);

