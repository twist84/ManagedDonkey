#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_oddball_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_oddball_variant : c_game_engine_base_variant
{
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_oddball_variant_team_scoring_settings, short, k_oddball_variant_team_scoring_settings> m_team_scoring

	c_flags<e_oddball_variant_flags, dword_flags, k_oddball_variant_flags> m_variant_flags;
	c_enum<long, short, 200, 1001> m_score_to_win;
	c_enum<long, short, 180, 1000> m_carrying_points;     // points per second when carrying the ball
	c_enum<long, short, 1, 20> m_unknown_points;          // halo online specific
	c_enum<long, char, 0, 20> m_kill_points;
	c_enum<long, char, 0, 20> m_ball_kill_points;         // number of points for a melee kill with the ball
	c_enum<long, char, 0, 20> m_carrier_kill_points;      // number of points for killing the ball carrier
	c_enum<long, char, 1, 2> m_ball_count;
	c_enum<long, short, 5, 120> m_ball_spawn_delay;       // time until first ball spawn after round start, and also delay in respawning ball if it goes out of play
	c_enum<long, short, 30, 120> m_ball_inactive_respawn; // time until ball respawns if it is inactive

	c_player_traits m_carrier_traits;

	byte m_pad1[2];

	byte unused[0x60];

	void byteswap();
};
static_assert(sizeof(c_game_engine_oddball_variant) == 0x260);

struct c_oddball_engine : c_game_engine
{
};

struct s_oddball_globals
{
	c_static_array<long, 5> __unknown0;
	c_static_array<real_point3d, 5> __unknown14;
	c_static_array<short, 5> __unknown50;
	c_static_array<long, 5> __unknown5C;
	c_static_array<char, 5> __unknown70;
	c_static_array<char, 5> __unknown75;
	c_static_array<long, 5> __unknown7C;
};
static_assert(sizeof(s_oddball_globals) == 0x90);

