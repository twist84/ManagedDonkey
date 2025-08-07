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

//.text:0056CBF0 ; public: c_game_results::c_game_results()
//.text:0056CC00 ; public: s_game_results::s_game_results()
//.text:0056CC60 ; 
//.text:0056CC70 ; public: int32 c_game_results::add_machine(const s_machine_identifier*)
//.text:0056CDC0 ; 
//.text:0056CE10 ; 
//.text:0056CE60 ; 
//.text:0056CEF0 ; public: void c_game_results::fixup_player_machine_references()
//.text:0056CFA0 ; 
//.text:0056CFF0 ; void __cdecl game_results_apply_incremental_update(s_game_results_incremental*, const s_game_results_incremental_update*)
//.text:0056D480 ; void __cdecl game_results_award_achievement(int32, e_achievement)
//.text:0056D4F0 ; 
//.text:0056D560 ; 
//.text:0056D5C0 ; bool __cdecl game_results_calculate_incremental_update(const s_game_results_incremental*, s_game_results_incremental_update*)
//.text:0056DC80 ; void __cdecl game_results_clear()
//.text:0056DCC0 ; int32 __cdecl game_results_count_damage_statistic(int32, e_game_results_damage_statistic, e_game_results_damage_breakdown_type)
//.text:0056DD30 ; void __cdecl game_results_damage_statistic_increment(int32, e_game_results_damage_statistic, e_damage_reporting_type, int32)
//.text:0056DDC0 ; void __cdecl game_results_decode_player_data(c_bitstream*, s_game_results_player_data_update*)

void __cdecl game_results_dispose()
{
	INVOKE(0x0056DE30, game_results_dispose);
}

void __cdecl game_results_dispose_from_old_map()
{
	INVOKE(0x0056DE60, game_results_dispose_from_old_map);
}

void __cdecl game_results_initialize_for_new_map()
{
	INVOKE(0x0056F340, game_results_initialize_for_new_map);
}

int32 __cdecl game_results_statistic_get_current(int32 player_absolute_index, int32 team_index, e_game_results_statistic statistic)
{
	return INVOKE(0x005702F0, game_results_statistic_get_current, player_absolute_index, team_index, statistic);
}

void __cdecl game_results_statistic_increment(int32 player_absolute_index, int32 team_index, e_game_results_statistic statistic, int32 a4)
{
	return INVOKE(0x005703A0, game_results_statistic_increment, player_absolute_index, team_index, statistic, a4);
}

void __cdecl game_results_statistic_set(int32 player_absolute_index, int32 team_index, e_game_results_statistic statistic, int32 a4)
{
	INVOKE(0x005704A0, game_results_statistic_set, player_absolute_index, team_index, statistic, a4);
}

bool __cdecl game_results_statistics_decode(c_bitstream* packet, s_integer_statistic_update* statistics, int32 statistics_count, const s_integer_statistic_definition* statistic_definitions, int32 statistics_definitions_count)
{
	//return INVOKE(0x00570560, game_results_statistics_decode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);

	bool result = false;
	HOOK_INVOKE(result =, game_results_statistics_decode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);
	return result;
}

void __cdecl game_results_statistics_encode(c_bitstream* packet, const s_integer_statistic_update* statistics, int32 statistics_count, const s_integer_statistic_definition* statistic_definitions, int32 statistics_definitions_count)
{
	//INVOKE(0x00570600, game_results_statistics_encode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);

	HOOK_INVOKE(, game_results_statistics_encode, packet, statistics, statistics_count, statistic_definitions, statistics_definitions_count);
}

void __cdecl game_results_update()
{
	INVOKE(0x00570730, game_results_update);
}

