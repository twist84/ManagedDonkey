#pragma once

#include "game/game_engine_default.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_slayer_traits.hpp"

struct c_game_engine_slayer_variant : c_game_engine_base_variant
{
	// `c_game_engine_base_variant::m_team_scoring_method` override
	// c_enum<e_slayer_team_scoring_settings, short, k_slayer_team_scoring_settings> m_team_scoring

	c_enum<long, short, -1, 250> m_score_to_win;        // default: 25
	c_enum<long, short, 0, 250> m_score_unknown;        // default: 20, halo online specific
	c_enum<long, short, -10, 10> m_kill_points;         // default: 1
	c_enum<long, char, -10, 10> m_assist_points;        // default: 0
	c_enum<long, char, -10, 10> m_death_points;         // default: 0
	c_enum<long, char, -10, 10> m_suicide_points;       // default: -1
	c_enum<long, char, -10, 10> m_betrayal_points;      // default: -1
	c_enum<long, char, -10, 10> m_leader_killed_points; // default: 0
	c_enum<long, char, -10, 10> m_elimination_points;   // default: 0
	c_enum<long, char, -10, 10> m_assassination_points; // default: 0
	c_enum<long, char, -10, 10> m_headshot_points;      // default: 0
	c_enum<long, char, -10, 10> m_melee_points;         // default: 0
	c_enum<long, char, -10, 10> m_sticky_points;        // default: 0
	c_enum<long, char, -10, 10> m_splatter_points;      // default: 0
	c_enum<long, char, -10, 10> m_killing_spree_points; // default: 0
	c_player_traits m_leader_traits;

	byte m_pad1[2];

	void byteswap();
};
static_assert(sizeof(c_game_engine_slayer_variant) == 0x200);

struct c_slayer_engine : c_game_engine
{
};