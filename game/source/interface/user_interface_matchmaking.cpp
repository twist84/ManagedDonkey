#include "interface/user_interface_matchmaking.hpp"

#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"

struct s_life_cycle_matchmaking_progress
{
	long progress_type;
	c_static_wchar_string<32> hopper_name;
	long hopper_squad_player_count;
	long hopper_group_player_count;
	bool hopper_is_ranked;
	bool hopper_is_team_game;
	long search_preference;
	s_replicated_life_cycle_matchmaking_progress_search_criteria search_criteria;

	union
	{
		s_replicated_life_cycle_matchmaking_progress_searching_for_match searching_for_match;
		s_replicated_life_cycle_matchmaking_progress_assembling_match assembling_match;
		s_replicated_life_cycle_matchmaking_progress_configuring_match configuring_match;
		s_replicated_life_cycle_progress_post_match post_match;
	};

	byte __data[0x64];
};
static_assert(sizeof(s_life_cycle_matchmaking_progress) == 0xF8);

REFERENCE_DECLARE(0x0191D298, long, g_network_matchmaking_fake_progress_stage);
REFERENCE_DECLARE(0x052604C8, s_life_cycle_matchmaking_progress, g_network_matchmaking_fake_progress);

HOOK_DECLARE(0x00A98920, user_interface_matchmaking_get_matchmaking_progress);

void __cdecl user_interface_matchmaking_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
    network_life_cycle_get_matchmaking_progress(progress_out);

    if (g_network_matchmaking_fake_progress_stage == -1)
        csmemcpy(&g_network_matchmaking_fake_progress, progress_out, sizeof(g_network_matchmaking_fake_progress));
    else
        csmemcpy(progress_out, &g_network_matchmaking_fake_progress, sizeof(s_life_cycle_matchmaking_progress));
}

