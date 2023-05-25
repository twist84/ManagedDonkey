#pragma once

#include "cseries/cseries.hpp"
#include "game/game_results.hpp"

struct s_network_message_distributed_game_results
{
	dword establishment_identifier;
	long update_number;
	s_game_results_incremental_update update;
};
static_assert(sizeof(s_network_message_distributed_game_results) == 0x1B7A8);

