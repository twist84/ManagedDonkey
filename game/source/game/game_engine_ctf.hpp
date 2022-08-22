#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_ctf_traits.hpp"

struct c_game_engine_ctf_variant : c_game_engine_base_variant
{
	c_flags<e_ctf_variant_flags, char, k_ctf_variant_flags> m_variant_flags;
	c_enum<e_ctf_home_flag_waypoint_settings, char, k_ctf_home_flag_waypoint_settings> m_home_flag_waypoint;
	c_enum<e_ctf_game_type_settings, char, k_ctf_game_type_settings> m_game_type;
	c_enum<e_ctf_respawn_settings, char, k_ctf_respawn_settings> m_respawn;
	c_enum<e_ctf_touch_return_settings, short, k_ctf_touch_return_settings> m_touch_return_timeout;
	c_enum<e_ctf_sudden_death_time, short, k_ctf_sudden_death_times> m_sudden_death_time;

	// default: 5
	// maximum: 50
	short m_score_to_win;

	// halo online specific
	// default: 3
	// maximum: 50
	short m_score_unknown;

	// default: 30
	// maximum: 300
	short m_flag_reset_time; // seconds

	c_player_traits m_carrier_traits;

	byte pad[6];
	byte unused[0x60];
};
static_assert(sizeof(c_game_engine_ctf_variant) == 0x260);