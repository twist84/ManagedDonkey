#include "game/game_results.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE_ARRAY(0x0189FB10, s_integer_statistic_definition, g_game_results_achievement_statistic_definitions, k_achievement_count);
REFERENCE_DECLARE_ARRAY(0x0189FE00, s_integer_statistic_definition, g_game_results_statistic_definitions, k_game_results_statistic_count);
REFERENCE_DECLARE_ARRAY(0x018A0130, s_integer_statistic_definition, g_game_results_damage_statistic_definitions, k_game_results_damage_statistic_count);
REFERENCE_DECLARE_ARRAY(0x018A0180, s_integer_statistic_definition, g_game_results_player_vs_player_statistic_definitions, k_game_results_player_vs_player_statistic_count);
REFERENCE_DECLARE_ARRAY(0x018A01A0, s_integer_statistic_definition, g_game_results_medal_statistic_definitions, k_game_results_medal_count);
REFERENCE_DECLARE(0x023DAE98, s_game_results_globals, g_game_results_globals);
REFERENCE_DECLARE(0x023DAEA8, c_game_results, g_current_game_results);

HOOK_DECLARE(0x00570560, game_results_statistics_decode);
HOOK_DECLARE(0x00570600, game_results_statistics_encode);

long __cdecl game_results_statistic_get_current(long player_absolute_index, long team_index, e_game_results_statistic statistic)
{
	return INVOKE(0x005702F0, game_results_statistic_get_current, player_absolute_index, team_index, statistic);
}

void __cdecl game_results_statistic_increment(long player_absolute_index, long team_index, e_game_results_statistic statistic, long a4)
{
	return INVOKE(0x005703A0, game_results_statistic_increment, player_absolute_index, team_index, statistic, a4);
}

void __cdecl game_results_statistic_set(long player_absolute_index, long team_index, e_game_results_statistic statistic, long a4)
{
	INVOKE(0x005704A0, game_results_statistic_set, player_absolute_index, team_index, statistic, a4);
}

bool __cdecl game_results_statistics_decode(c_bitstream* packet, s_integer_statistic_update* statistics, long statistics_count, s_integer_statistic_definition const* statistic_definitions, long statistics_definitions_count)
{
	//return INVOKE(0x00570560, game_results_statistics_decode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);

	bool result = false;
	HOOK_INVOKE(result =, game_results_statistics_decode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);
	return result;
}

void __cdecl game_results_statistics_encode(c_bitstream* packet, s_integer_statistic_update const* statistics, long statistics_count, s_integer_statistic_definition const* statistic_definitions, long statistics_definitions_count)
{
	//INVOKE(0x00570600, game_results_statistics_encode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);

	HOOK_INVOKE(, game_results_statistics_encode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);
}

void __cdecl game_results_update()
{
	INVOKE(0x00570730, game_results_update);
}

