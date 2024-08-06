#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_koth_traits.hpp"

struct c_bitstream;
struct s_file_reference;

struct c_game_engine_king_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_king_variant* constructor()
	{
		return DECLFUNC(0x00572BA0, c_game_engine_king_variant*, __thiscall, c_game_engine_king_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_king_variant const* variant, bool force);
	//void set(s_game_engine_king_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_opaque_hill() const;
	void set_opaque_hill(bool opaque_hill);

	short get_score_to_win() const;
	void set_score_to_win(short score_to_win);

	e_king_moving_hill_settings get_moving_hill() const;
	short get_hill_move_in_seconds() const;
	void set_moving_hill(e_king_moving_hill_settings moving_hill);

	e_king_moving_hill_order_settings get_moving_hill_order() const;
	void set_moving_hill_order(e_king_moving_hill_order_settings moving_hill_order);

	char get_uncontested_hill_bonus() const;
	void set_uncontested_hill_bonus(char uncontested_hill_bonus);

	char get_kill_points() const;
	void set_kill_points(char kill_points);

	char get_inside_hill_points() const;
	void set_inside_hill_points(char inside_hill_points);

	char get_outside_hill_points() const;
	void set_outside_hill_points(char outside_hill_points);

	c_player_traits* get_inside_hill_traits_writeable();
	c_player_traits const* get_inside_hill_traits() const;
	void set_inside_hill_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_king_flags_settings, dword_flags, k_king_flags_settings> m_variant_flags;
	c_enum<long, short, 0, 1000> m_score_to_win;          // default: 100
	c_enum<long, short, 0, 1000> m_score_unknown;         // default: 90, halo online specific
	c_enum<e_king_moving_hill_settings, char, _king_moving_hill_settings_off, k_king_moving_hill_settings> m_moving_hill;
	c_enum<e_king_moving_hill_order_settings, char, _king_moving_hill_order_settings_random, k_king_moving_hill_order_settings> m_moving_hill_order;
	c_enum<long, char, -10, 10> m_uncontested_hill_bonus; // default: 0
	c_enum<long, char, -10, 10> m_kill_points;            // default: 0
	c_enum<long, char, -10, 10> m_inside_hill_points;     // default: 0
	c_enum<long, char, -10, 10> m_outside_hill_points;    // default: 0
	c_player_traits m_inside_hill_traits;

	byte m_pad1[6];
};
static_assert(sizeof(c_game_engine_king_variant) == 0x200);

struct c_king_engine :
	c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_king_globals
{
	c_area_set<c_area, 10> area_set;
	byte __data3F8[0x88];
};
static_assert(sizeof(s_king_globals) == 0x480);

extern c_game_engine_king_variant*& king_variant;

