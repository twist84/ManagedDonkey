#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_juggernaut_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_bitstream;
struct s_file_reference;

class c_game_engine_juggernaut_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_juggernaut_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572B80, c_game_engine_juggernaut_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_juggernaut_variant* variant, bool force);
	//void set(const s_game_engine_juggernaut_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_allied_against_juggernaut() const;
	void set_allied_against_juggernaut(bool allied_against_juggernaut);

	bool get_respawn_on_lone_juggernaut() const;
	void set_respawn_on_lone_juggernaut(bool respawn_on_lone_juggernaut);

	bool get_destination_zones_enabled() const;
	void set_destination_zones_enabled(bool destination_zones_enabled);

	void set_score_to_win_round(int16 score_to_win_round);

	e_juggernaut_initial_juggernaut_settings get_initial_juggernaut() const;
	void set_initial_juggernaut(e_juggernaut_initial_juggernaut_settings initial_juggernaut);

	e_juggernaut_next_juggernaut_settings get_next_juggernaut() const;
	void set_next_juggernaut(e_juggernaut_next_juggernaut_settings next_juggernaut);

	e_juggernaut_zone_movement_settings get_zone_movement() const;
	int32 get_zone_movement_time_in_seconds() const;
	void set_zone_movement(e_juggernaut_zone_movement_settings zone_movement);

	e_juggernaut_zone_order_settings get_zone_order() const;
	void set_zone_order(e_juggernaut_zone_order_settings zone_order);

	int8 get_kill_points() const;
	void set_kill_points(int8 kill_points);

	int8 get_juggernaut_kill_points() const;
	void set_juggernaut_kill_points(int8 juggernaut_kill_points);

	int8 get_kill_as_juggernaut_points() const;
	void set_kill_as_juggernaut_points(int8 kill_as_juggernaut_points);

	int8 get_destination_arrival_points() const;
	void set_destination_arrival_points(int8 destination_arrival_points);

	int8 get_suicide_points() const;
	void set_suicide_points(int8 suicide_points);

	int8 get_betrayal_points() const;
	void set_betrayal_points(int8 betrayal_points);

	uns8 get_juggernaut_delay() const;
	void set_juggernaut_delay(uns8 juggernaut_delay);

	c_player_traits* get_juggernaut_traits_writeable();
	const c_player_traits* get_juggernaut_traits() const;
	void set_juggernaut_traits(const c_player_traits* traits, bool force);

protected:
	c_enum<int32, int16, 0, 500> m_score_to_win_round;         // default: 15
	c_enum<int32, int16, 0, 500> m_score_unknown;              // default: 13, halo online specific

	byte m_pad[0x2];

	c_enum<e_juggernaut_initial_juggernaut_settings, uns8, _juggernaut_initial_juggernaut_settings_random, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, uns8, _juggernaut_next_juggernaut_settings_on_killing_juggernaut, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_variant_flags, uns8, k_juggernaut_variant_flags> m_variant_flags;
	c_enum<e_juggernaut_zone_movement_settings, uns8, _juggernaut_zone_movement_settings_off, k_number_of_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, uns8, _juggernaut_zone_order_settings_random, k_juggernaut_zone_order_settings> m_zone_order;
	c_enum<int32, int8, -10, 10> m_kill_points;                // default: 0
	c_enum<int32, int8, -10, 10> m_juggernaut_kill_points;     // default: 1
	c_enum<int32, int8, -10, 10> m_kill_as_juggernaut_points;  // default: 1
	c_enum<int32, int8, -10, 10> m_destination_arrival_points; // default: 1
	c_enum<int32, int8, -10, 10> m_suicide_points;             // default: 1
	c_enum<int32, int8, -10, 10> m_betrayal_points;            // default: 1
	c_enum<int32, uns8, 0, 10> m_juggernaut_delay;             // default: 0
	c_player_traits m_juggernaut_traits;

	byte m_pad1[2];
};
COMPILE_ASSERT(sizeof(c_game_engine_juggernaut_variant) == 0x200);

class c_juggernaut_engine :
	public c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_juggernaut_globals
{
	int32 juggernaut_player_index;
	int16 juggernaut_delay_seconds;
	bool juggernaut_visited_zone;
	int32 juggernaut_delay_ticks;
	c_area_set<c_area, 12> zones;
	c_static_array<int32, 16> juggernaut_kills;
	int32 juggernaut_player_index_at_game_update_start;
};
COMPILE_ASSERT(sizeof(s_juggernaut_globals) == 0x510);

extern c_game_engine_juggernaut_variant*& juggernaut_variant;

