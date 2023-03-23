#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_koth_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_king_variant : c_game_engine_base_variant
{
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_king_team_scoring_settings, short, k_king_team_scoring_settings> m_team_scoring

	c_flags<e_king_variant_flags, dword_flags, k_king_variant_flags> m_variant_flags;
	c_enum<long, short, 0, 1000> m_score_to_win;          // default: 100
	c_enum<long, short, 0, 1000> m_score_unknown;         // default: 90, halo online specific
	c_enum<e_king_moving_hill_settings, char, _king_moving_hill_settings_off, k_king_moving_hill_settings> m_moving_hill;
	c_enum<e_king_moving_hill_order_settings, char, _king_moving_hill_order_settings_random, k_king_moving_hill_order_settings> m_moving_hill_order;
	c_enum<long, char, -10, 10> m_uncontested_hill_bonus; // default: 0
	c_enum<long, char, -10, 10> m_kill_points;            // default: 0
	c_enum<long, char, -10, 10> m_inside_hill_points;     // default: 0
	c_enum<long, char, -10, 10> m_outside_hill_points;    // default: 0
	c_player_traits m_inside_hill_traits;

	byte m_pad1[6];

	byte unused[0x60];

	void byteswap();
};
static_assert(sizeof(c_game_engine_king_variant) == 0x260);

struct c_king_engine : c_game_engine
{
};

struct s_king_globals
{
	c_area_set<c_area, 10> area_set;
	byte __data3F8[0x88];
};
static_assert(sizeof(s_king_globals) == 0x480);

