#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_juggernaut_traits.hpp"
#include "game/game_engine_player_traits.hpp"

struct c_bitstream;
struct s_file_reference;

struct c_game_engine_juggernaut_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_juggernaut_variant* constructor()
	{
		return DECLFUNC(0x00572B80, c_game_engine_juggernaut_variant*, __thiscall, c_game_engine_juggernaut_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_juggernaut_variant const* variant, bool force);
	//void set(s_game_engine_juggernaut_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_allied_against_juggernaut() const;
	void set_allied_against_juggernaut(bool allied_against_juggernaut);

	bool get_respawn_on_lone_juggernaut() const;
	void set_respawn_on_lone_juggernaut(bool respawn_on_lone_juggernaut);

	bool get_destination_zones_enabled() const;
	void set_destination_zones_enabled(bool destination_zones_enabled);

	void set_score_to_win_round(short score_to_win_round);

	e_juggernaut_initial_juggernaut_settings get_initial_juggernaut() const;
	void set_initial_juggernaut(e_juggernaut_initial_juggernaut_settings initial_juggernaut);

	e_juggernaut_next_juggernaut_settings get_next_juggernaut() const;
	void set_next_juggernaut(e_juggernaut_next_juggernaut_settings next_juggernaut);

	e_juggernaut_zone_movement_settings get_zone_movement() const;
	long get_zone_movement_time_in_seconds() const;
	void set_zone_movement(e_juggernaut_zone_movement_settings zone_movement);

	e_juggernaut_zone_order_settings get_zone_order() const;
	void set_zone_order(e_juggernaut_zone_order_settings zone_order);

	char get_kill_points() const;
	void set_kill_points(char kill_points);

	char get_juggernaut_kill_points() const;
	void set_juggernaut_kill_points(char juggernaut_kill_points);

	char get_kill_as_juggernaut_points() const;
	void set_kill_as_juggernaut_points(char kill_as_juggernaut_points);

	char get_destination_arrival_points() const;
	void set_destination_arrival_points(char destination_arrival_points);

	char get_suicide_points() const;
	void set_suicide_points(char suicide_points);

	char get_betrayal_points() const;
	void set_betrayal_points(char betrayal_points);

	byte get_juggernaut_delay() const;
	void set_juggernaut_delay(byte juggernaut_delay);

	c_player_traits* get_juggernaut_traits_writeable();
	c_player_traits const* get_juggernaut_traits() const;
	void set_juggernaut_traits(c_player_traits const* traits, bool force);

protected:
	c_enum<long, short, 0, 500> m_score_to_win_round;         // default: 15
	c_enum<long, short, 0, 500> m_score_unknown;              // default: 13, halo online specific

	byte m_pad[2];

	c_enum<e_juggernaut_initial_juggernaut_settings, byte, _juggernaut_initial_juggernaut_settings_random, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, byte, _juggernaut_next_juggernaut_settings_on_killing_juggernaut, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_variant_flags, byte_flags, k_juggernaut_variant_flags> m_variant_flags;
	c_enum<e_juggernaut_zone_movement_settings, byte, _juggernaut_zone_movement_settings_off, k_number_of_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, byte, _juggernaut_zone_order_settings_random, k_juggernaut_zone_order_settings> m_zone_order;
	c_enum<long, char, -10, 10> m_kill_points;                // default: 0
	c_enum<long, char, -10, 10> m_juggernaut_kill_points;     // default: 1
	c_enum<long, char, -10, 10> m_kill_as_juggernaut_points;  // default: 1
	c_enum<long, char, -10, 10> m_destination_arrival_points; // default: 1
	c_enum<long, char, -10, 10> m_suicide_points;             // default: 1
	c_enum<long, char, -10, 10> m_betrayal_points;            // default: 1
	c_enum<long, byte, 0, 10> m_juggernaut_delay;             // default: 0
	c_player_traits m_juggernaut_traits;

	byte m_pad1[2];
};
static_assert(sizeof(c_game_engine_juggernaut_variant) == 0x200);

struct c_juggernaut_engine :
	c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
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

