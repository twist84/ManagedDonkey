#pragma once

#include "cseries/cseries.hpp"

struct s_game_engine_request_boot_player_event_data
{
	long booted_player_index;
	long booting_player_index;
};
static_assert(sizeof(s_game_engine_request_boot_player_event_data) == 0x8);

