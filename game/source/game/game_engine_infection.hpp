#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_infection_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_infection_variant : c_game_engine_base_variant
{
	c_flags<e_infection_variant_flags, byte_flags, k_infection_variant_flags> m_variant_flags;
	c_enum<e_infection_safe_havens_settings, char, k_infection_safe_havens_settings> m_safe_havens;
	c_enum<e_infection_next_zombie_settings, char, k_infection_next_zombie_settings> m_next_zombie;
	c_enum<e_infection_initial_zombie_count_settings, char, k_infection_initial_zombie_count_settings> m_initial_zombie_count;

	// default: 30
	// maximum: 120
	short m_safe_haven_movement_time; // 0 is no movement

	// default: 1
	// maximum: 20
	char m_zombie_kill_points;

	// default: 0
	// maximum: 20
	char m_infection_points;

	// default: 0
	// maximum: 20
	char m_safe_haven_arrival_points;

	// default: -1
	// maximum: 20
	char m_suicide_points;

	// default: -1
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 20
	char m_last_man_bonus_points;

	c_player_traits m_zombie_traits;
	c_player_traits m_first_zombie_traits;
	c_player_traits m_safe_haven_defender_traits;
	c_player_traits m_last_human_traits;

	byte pad[4];
	byte unused[0x10];
};
static_assert(sizeof(c_game_engine_infection_variant) == 0x260);

struct c_infection_engine : c_game_engine
{
};