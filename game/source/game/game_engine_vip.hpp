#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_vip_traits.hpp"

struct c_game_engine_vip_variant : c_game_engine_base_variant
{
	c_enum<long, short, 0, 500> m_score_to_win_round;         // default: 15
	c_enum<long, short, 0, 500> m_score_unknown;              // default: 10, halo online specific
	c_flags<e_vip_variant_flags, word_flags, k_vip_variant_flags> m_variant_flags;
	c_enum<long, char, -10, 10> m_kill_points;                // default: 0
	c_enum<long, char, -10, 10> m_takedown_points;            // default: 0
	c_enum<long, char, -10, 10> m_kill_as_vip_points;         // default: 0
	c_enum<long, char, -10, 10> m_vip_death_points;           // default: 0
	c_enum<long, char, -10, 10> m_destination_arrival_points; // default: 0
	c_enum<long, char, -10, 10> m_suicide_points;             // default: 0
	c_enum<long, char, -10, 10> m_betrayal_points;            // default: 0
	c_enum<long, char, -10, 10> m_vip_suicide_points;         // default: 0
	c_enum<e_vip_selection_settings, char, _vip_selection_settings_random, k_vip_selection_settings> m_vip_selection;
	c_enum<e_vip_zone_movement_settings, char, _vip_zone_movement_settings_off, k_vip_zone_movement_settings> m_zone_movement;
	c_enum<e_vip_zone_order_settings, char, _vip_zone_order_settings_random, k_vip_zone_order_settings> m_zone_order;

	byte m_pad1[1];

	c_enum<long, short, 0, 50> m_influence_radius;
	c_player_traits m_vip_team_traits;
	c_player_traits m_vip_influence_traits;
	c_player_traits m_vip_traits;

	void byteswap();
};
static_assert(sizeof(c_game_engine_vip_variant) == 0x238);

struct c_vip_engine : c_game_engine
{
};

struct c_destination_zone : c_area
{
	byte_flags m_team_designator_flags;
};
static_assert(sizeof(c_destination_zone) == 0x68);

struct s_vip_globals
{
	c_static_array<long, 8> __unknown0;
	dword __unknown20;
	c_static_array<s_multiplayer_object_boundary_geometry_data, 8> geometry_datas;
	c_area_set<c_destination_zone, 12> destination_zones;
};
static_assert(sizeof(s_vip_globals) == 0x794);

extern c_game_engine_vip_variant*& vip_variant;

