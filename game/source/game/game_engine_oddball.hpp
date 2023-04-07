#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_oddball_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_oddball_variant : c_game_engine_base_variant
{
public:
	c_game_engine_oddball_variant* constructor()
	{
		return DECLFUNC(0x00572BD0, c_game_engine_oddball_variant*, __thiscall, c_game_engine_oddball_variant*)(this);
	}

	void byteswap();

protected:
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_oddball_variant_team_scoring_settings, short, k_oddball_variant_team_scoring_settings> m_team_scoring

	c_flags<e_oddball_variant_flags, dword_flags, k_oddball_variant_flags> m_variant_flags;
	c_enum<long, short, -1, 1000> m_score_to_win;              // default: 200
	c_enum<long, short, 0, 1000> m_score_unknown;              // default: 180, halo online specific
	c_enum<long, short, -10, 10> m_carrying_points;            // default: 1, points per second when carrying the ball
	c_enum<long, char, -10, 10> m_kill_points;                 // default: 0
	c_enum<long, char, -10, 10> m_ball_kill_points;            // default: 0, number of points for a melee kill with the ball
	c_enum<long, char, -10, 10> m_carrier_kill_points;         // default: 0, number of points for killing the ball carrier
	c_enum<long, char, 1, 3> m_ball_count;                     // default: 1
	c_enum<long, short, 0, 120> m_ball_spawn_delay;            // default: 5, time until first ball spawn after round start, and also delay in respawning ball if it goes out of play
	c_enum<long, short, 0, 120> m_ball_inactive_respawn_delay; // default: 30 time until ball respawns if it is inactive

	c_player_traits m_carrier_traits;

	byte m_pad1[2];
};
static_assert(sizeof(c_game_engine_oddball_variant) == 0x200);

struct c_oddball_engine : c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
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

extern c_game_engine_oddball_variant*& oddball_variant;

