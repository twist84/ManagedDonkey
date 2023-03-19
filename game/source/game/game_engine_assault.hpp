#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_assault_traits.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_assault_variant : c_game_engine_base_variant
{
	c_flags<e_assault_variant_flags, word_flags, k_assault_variant_flags> m_variant_flags;

	// default: `disabled`
	c_enum<e_assault_respawn_settings, short, _assault_respawn_settings_disabled, k_assault_respawn_settings> m_respawn;

	// default: `multi bomb`
	c_enum<e_assault_game_type_settings, short, _assault_game_type_settings_multi_bomb, k_assault_game_type_settings> m_game_type;

	// default: `when not carried by enemy`
	c_enum<e_assault_enemy_bomb_waypoint_settings, short, _assault_enemy_bomb_waypoint_settings_never, k_assault_enemy_bomb_waypoint_settings> m_enemy_bomb_waypoint;

	// default: 3
	// maximum: 50
	short m_score_to_win;

	// halo online specific
	// default: 2
	// maximum: 50
	short m_score_unknown0;

	// halo online specific
	// default: 0
	// maximum: 50
	short m_score_unknown1;

	// halo online specific
	// default: 0
	// maximum: 50
	short m_score_unknown2;

	// halo online specific
	// default: 0
	// maximum: 50
	short m_score_unknown3;

	// default: `off`
	c_enum<e_assault_variant_sudden_death_time, short, _assault_variant_sudden_death_time_infinite, k_assault_variant_sudden_death_time> m_sudden_death_time;

	// default: 30
	// maximum: 90
	short m_bomb_reset_time; // seconds

	// default: 35
	// maximum: 90
	short m_bomb_arming_time; // seconds

	// default: 35
	// maximum: 90
	short m_bomb_disarming_time; // seconds

	// default: 35
	// maximum: 90
	short m_bomb_fuse_time; // seconds

	c_player_traits m_carrier_traits;
	c_player_traits m_arming_traits;

	byte m_pad1[4];
	byte unused[0x38];
};
static_assert(sizeof(c_game_engine_assault_variant) == 0x260);

struct c_assault_engine : c_ctf_engine
{
};

struct s_assault_globals
{
	s_ctf_shared_globals shared_globals;
	c_static_array<dword, 9> __unknown164C;
	c_static_array<word, 9> __unknown1670;
	c_static_array<byte, 2> __unknown1682;
	c_static_array<dword, 9> __unknown1684;
	c_static_array<byte, 8> __unknown16A8;
	c_static_array<word, 9> __unknown16B0;
};
static_assert(sizeof(s_assault_globals) == 0x16C4);

