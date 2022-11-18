#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_candy_monitor.hpp"
#include "game/game_statborg.hpp"
#include "saved_games/scenario_map_variant.hpp"

struct s_game_engine_globals
{
	dword_flags flags;
	dword __unknown4;
	dword __unknown8;
	word __unknownC;
	word game_simulation;
	dword __unknown16[7];
	dword gamestate_index;
	dword statborg_gamestate_index;
	c_static_array<long, 16> player_simulation_object_glue_indices;
	byte __data74[0x4];
	c_map_variant map_variant;
	short state;
	short current_round;
	dword round_ticks_elapsed;
	byte game_engine_round_condition;
	byte __unknownE111[0x3];
	byte __unknownE114[0x7C];
	byte __unknownE190[0x728];
	byte forge_legal_notice;
	byte __unknownE8B9[0x10D7];
	word round_time_limit;
	word __unknownF992;
	dword game_variant_round_time_limit_ticks_per_second;
	byte __unknownF996[0x518];
	c_game_statborg statborg;
	byte __data102D4[0x2C8];
	c_multiplayer_candy_monitor_manager candy_monitor_manager;
	dword __unknown13D9C;
	dword desired_state;
	dword __unknown13DA4[3];
	dword game_variant_game_type;
	dword object_count;
	dword __unknown13DB8;
	byte __unknown13DBC[0x1A9C];
};
static_assert(sizeof(s_game_engine_globals) == 0x15858);

struct s_local_game_engine_globals
{
	dword __time0;
	byte __data[0xC0];
};
static_assert(sizeof(s_local_game_engine_globals) == 0xC4);

extern long __cdecl game_engine_get_player_place(long absolute_player_index);
extern long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score);
extern long __cdecl game_engine_get_team_place(long team);
extern long __cdecl game_engine_get_team_score_for_display(long team, bool final_score);
extern bool __cdecl game_engine_in_round();
extern void __cdecl game_engine_interface_update(float world_seconds_elapsed);
extern long __cdecl game_engine_get_pre_round_ticks();

