#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_juggernaut_traits.hpp"

struct c_game_engine_juggernaut_variant : c_game_engine_base_variant
{
	// default: 15
	// maximum: 500
	short m_score_to_win_round;

	// default: 13
	// maximum: 500
	short m_score_unknown;

	// always set to '0'
	short m_unknown;

	c_enum<e_juggernaut_initial_juggernaut_settings, char, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, char, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_variant_flags, byte_flags, k_juggernaut_variant_flags> m_variant_flags;
	c_enum<e_juggernaut_zone_movement_settings, char, k_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, char, k_juggernaut_zone_order_settings> m_zone_order;

	// default: 0
	// maximum: 20
	char m_kill_points;

	// default: 1
	// maximum: 20
	char m_juggeraut_kill_points;

	// default: 1
	// maximum: 20
	char m_kill_as_juggernaut_points;

	// default: 1
	// maximum: 20
	char m_destination_arrival_points;

	// default: 1
	// maximum: 20
	char m_suicide_points;

	// default: 1
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 10
	char m_juggernaut_delay;

	c_player_traits m_juggernaut_traits;

	byte pad[2];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_juggernaut_variant) == 0x260);