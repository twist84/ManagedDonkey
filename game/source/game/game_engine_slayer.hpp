#pragma once

#include "game/game_engine_default.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_slayer_traits.hpp"

struct c_game_engine_slayer_variant : c_game_engine_base_variant
{
	// `c_game_engine_base_variant::m_team_scoring_method` override
	// c_enum<e_slayer_team_scoring_settings, short, k_slayer_team_scoring_settings> m_team_scoring

	c_enum<long, short, 25, 251> m_score_to_win;
	c_enum<long, short, 20, 250> m_score_unknown; // halo online specific
	c_enum<long, short, 1, 20> m_kill_points;
	c_enum<long, char, 0, 20> m_assist_points;
	c_enum<long, char, 0, 20> m_death_points;
	c_enum<long, char, -1, 20> m_suicide_points;
	c_enum<long, char, -1, 20> m_betrayal_points;
	c_enum<long, char, 0, 20> m_leader_killed_points;
	c_enum<long, char, 0, 20> m_elimination_points;
	c_enum<long, char, 0, 20> m_assassination_points;
	c_enum<long, char, 0, 20> m_headshot_points;
	c_enum<long, char, 0, 20> m_melee_points;
	c_enum<long, char, 0, 20> m_sticky_points;
	c_enum<long, char, 0, 20> m_splatter_points;
	c_enum<long, char, 0, 20> m_killing_spree_points;
	c_player_traits m_leader_traits;

	byte m_pad1[2];

	byte unused[0x60];

	void byteswap();
};
static_assert(sizeof(c_game_engine_slayer_variant) == 0x260);

struct c_slayer_engine : c_game_engine
{
};