#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_territories_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_territories_variant : c_game_engine_base_variant
{
	c_flags<e_territories_variant_flags, word_flags, k_territories_variant_flags> m_variant_flags;

	c_enum<e_territories_respawn_on_capture_settings, short, _territories_respawn_on_capture_settings_disabled, k_territories_respawn_on_capture_settings> m_respawn_on_capture;
	c_enum<e_territories_capture_time_settings, short, _territories_capture_time_settings_instant, k_territories_capture_time_settings> m_capture_time;
	c_enum<e_territories_sudden_death_settings, short, _territories_sudden_death_settings_indefinite, k_territories_sudden_death_settings> m_sudden_death_time;

	c_player_traits m_defender_traits;
	c_player_traits m_attacker_traits;

	byte unused[0x50];

	void byteswap();
};
static_assert(sizeof(c_game_engine_territories_variant) == 0x260);

struct c_territories_engine : c_game_engine
{
};

struct s_territory_data : c_area
{
	c_static_array<word, 8> __unknown64;
	c_static_array<byte, 8> __unknown74;
	long __unknown7C;
	long __unknown80;
};
static_assert(sizeof(s_territory_data) == 0x84);

struct s_territories_globals
{
	c_area_set<s_territory_data, 8> area_set;
	long __unknown430;
	long __unknown434;
	long __unknown438;
};
static_assert(sizeof(s_territories_globals) == 0x43C);

