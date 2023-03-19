#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_infection_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_infection_variant : c_game_engine_base_variant
{
	c_flags<e_infection_variant_flags, byte_flags, k_infection_variant_flags> m_variant_flags;
	c_enum<e_infection_safe_havens_settings, char, _infection_safe_havens_off, k_infection_safe_havens_settings> m_safe_havens;
	c_enum<e_infection_next_zombie_settings, char, _infection_next_zombie_winner, k_infection_next_zombie_settings> m_next_zombie;
	c_enum<e_infection_initial_zombie_count_settings, char, _infection_initial_zombie_count_25_percent, k_infection_initial_zombie_count_settings> m_initial_zombie_count;

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

	byte m_pad1[4];
	byte unused[0x10];

	void byteswap();
};
static_assert(sizeof(c_game_engine_infection_variant) == 0x260);

struct c_infection_engine : c_game_engine
{
};

struct s_infection_globals
{
	dword __unknown0;
	word __unknown4;
	byte __unknown6;
	dword __unknown8;
	c_area_set<c_area, 12> area_set;
	c_static_array<long, 16> __unknown44C;
	c_static_array<long, 16> __unknown50C;
	byte __unknown54C;
};
static_assert(sizeof(s_infection_globals) == 0x550);

