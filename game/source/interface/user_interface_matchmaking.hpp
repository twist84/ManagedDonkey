#pragma once

struct s_life_cycle_matchmaking_progress;

extern int32& g_network_matchmaking_fake_progress_stage;

extern void __cdecl user_interface_matchmaking_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out);
extern e_network_file_load_status __cdecl user_interface_matchmaking_hopper_catalog_load_status();

