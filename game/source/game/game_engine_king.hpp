#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_koth_traits.hpp"

class c_bitstream;
struct s_file_reference;

class c_game_engine_king_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_king_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572BA0, c_game_engine_king_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_king_variant* variant, bool force);
	//void set(const s_game_engine_king_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_opaque_hill() const;
	void set_opaque_hill(bool opaque_hill);

	int16 get_score_to_win() const;
	void set_score_to_win(int16 score_to_win);

	e_king_moving_hill_settings get_moving_hill() const;
	int16 get_hill_move_in_seconds() const;
	void set_moving_hill(e_king_moving_hill_settings moving_hill);

	e_king_moving_hill_order_settings get_moving_hill_order() const;
	void set_moving_hill_order(e_king_moving_hill_order_settings moving_hill_order);

	int8 get_uncontested_hill_bonus() const;
	void set_uncontested_hill_bonus(int8 uncontested_hill_bonus);

	int8 get_kill_points() const;
	void set_kill_points(int8 kill_points);

	int8 get_inside_hill_points() const;
	void set_inside_hill_points(int8 inside_hill_points);

	int8 get_outside_hill_points() const;
	void set_outside_hill_points(int8 outside_hill_points);

	c_player_traits* get_inside_hill_traits_writeable();
	const c_player_traits* get_inside_hill_traits() const;
	void set_inside_hill_traits(const c_player_traits* traits, bool force);

protected:
	c_flags<e_king_flags_settings, uns32, k_king_flags_settings> m_variant_flags;
	c_enum<int32, int16, 0, 1000> m_score_to_win;          // default: 100
	c_enum<int32, int16, 0, 1000> m_score_unknown;         // default: 90, halo online specific
	c_enum<e_king_moving_hill_settings, int8, _king_moving_hill_settings_off, k_king_moving_hill_settings> m_moving_hill;
	c_enum<e_king_moving_hill_order_settings, int8, _king_moving_hill_order_settings_random, k_king_moving_hill_order_settings> m_moving_hill_order;
	c_enum<int32, int8, -10, 10> m_uncontested_hill_bonus; // default: 0
	c_enum<int32, int8, -10, 10> m_kill_points;            // default: 0
	c_enum<int32, int8, -10, 10> m_inside_hill_points;     // default: 0
	c_enum<int32, int8, -10, 10> m_outside_hill_points;    // default: 0
	c_player_traits m_inside_hill_traits;

	byte m_pad1[6];
};
COMPILE_ASSERT(sizeof(c_game_engine_king_variant) == 0x200);

class c_king_engine :
	public c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_king_globals
{
	c_area_set<c_area, 10> the_hills;
	c_static_array<int32, 8> team_ticks_in_hill;
	c_static_array<int32, 8> team_ticks_outside_hill;
	int32 hill_timer;
	uns16 players_in_hill;
	uns16 pad;
	c_static_array<int32, 16> hill_kills;
};
COMPILE_ASSERT(sizeof(s_king_globals) == 0x480);

extern c_game_engine_king_variant*& king_variant;

