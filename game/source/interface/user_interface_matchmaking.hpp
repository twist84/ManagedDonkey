#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

extern long& g_network_matchmaking_fake_progress_stage;

struct s_life_cycle_matchmaking_progress;
extern void __cdecl user_interface_matchmaking_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out);

