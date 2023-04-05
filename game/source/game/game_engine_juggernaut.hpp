#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_juggernaut_traits.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_juggernaut_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_juggernaut_variant : c_game_engine_base_variant
{
	c_enum<long, short, 0, 500> m_score_to_win_round;         // default: 15
	c_enum<long, short, 0, 500> m_score_unknown;              // default: 13, halo online specific

	byte m_pad[2];

	c_enum<e_juggernaut_initial_juggernaut_settings, char, _juggernaut_initial_juggernaut_settings_random, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, char, _juggernaut_next_juggernaut_settings_on_killing_juggernaut, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_variant_flags, byte_flags, k_juggernaut_variant_flags> m_variant_flags;
	c_enum<e_juggernaut_zone_movement_settings, char, _juggernaut_zone_movement_settings_off, k_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, char, _juggernaut_zone_order_settings_random, k_juggernaut_zone_order_settings> m_zone_order;
	c_enum<long, char, -10, 10> m_kill_points;                // default: 0
	c_enum<long, char, -10, 10> m_juggeraut_kill_points;      // default: 1
	c_enum<long, char, -10, 10> m_kill_as_juggernaut_points;  // default: 1
	c_enum<long, char, -10, 10> m_destination_arrival_points; // default: 1
	c_enum<long, char, -10, 10> m_suicide_points;             // default: 1
	c_enum<long, char, -10, 10> m_betrayal_points;            // default: 1
	c_enum<long, char, 0, 10> m_juggernaut_delay;             // default: 0
	c_player_traits m_juggernaut_traits;

	byte m_pad1[2];

	void byteswap();
};
static_assert(sizeof(c_game_engine_juggernaut_variant) == 0x200);

struct c_juggernaut_engine : c_game_engine
{
};

struct s_juggernaut_globals
{
	dword __unknown0;
	word __unknown4;
	byte __unknown6;
	dword __unknown8;
	c_area_set<c_area, 12> area_set;
	c_static_array<long, 16> __unknown4CC;
};
static_assert(sizeof(s_juggernaut_globals) == 0x50C);

extern c_game_engine_juggernaut_variant*& juggernaut_variant;

