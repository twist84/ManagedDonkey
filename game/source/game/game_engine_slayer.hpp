#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_slayer_traits.hpp"
#include "game/game_engine_player_traits.hpp"

struct c_bitstream;
struct s_file_reference;

struct c_game_engine_slayer_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_slayer_variant* constructor()
	{
		return DECLFUNC(0x00572C20, c_game_engine_slayer_variant*, __thiscall, c_game_engine_slayer_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_slayer_variant const* variant, bool force);
	//void set(s_game_engine_slayer_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	short get_score_to_win() const;
	void set_score_to_win(short score_to_win);

	char get_kill_points() const;
	void set_kill_points(char kill_points);

	char get_assist_points() const;
	void set_assist_points(char assist_points);

	char get_death_points() const;
	void set_death_points(char death_points);

	char get_suicide_points() const;
	void set_suicide_points(char suicide_points);

	char get_betrayal_points() const;
	void set_betrayal_points(char betrayal_points);

	char get_leader_killed_points() const;
	void set_leader_killed_points(char leader_killed_points);

	char get_elimination_points() const;
	void set_elimination_points(char elimination_points);

	char get_assassination_points() const;
	void set_assassination_points(char assassination_points);

	char get_headshot_points() const;
	void set_headshot_points(char headshot_points);

	char get_melee_points() const;
	void set_melee_points(char melee_points);

	char get_sticky_points() const;
	void set_sticky_points(char sticky_points);

	char get_splatter_points() const;
	void set_splatter_points(char splatter_points);

	char get_killing_spree_points() const;
	void set_killing_spree_points(char killing_spree_points);

	c_player_traits* get_leader_traits_writeable();
	c_player_traits const* get_leader_traits() const;
	void set_leader_traits(c_player_traits const* traits, bool force);

protected:
	c_enum<long, short, -1, 250> m_score_to_win;        // default: 25
	c_enum<long, short, 0, 250> m_score_unknown;        // default: 20, halo online specific
	c_enum<long, short, -10, 10> m_kill_points;         // default: 1
	c_enum<long, char, -10, 10> m_assist_points;        // default: 0
	c_enum<long, char, -10, 10> m_death_points;         // default: 0
	c_enum<long, char, -10, 10> m_suicide_points;       // default: -1
	c_enum<long, char, -10, 10> m_betrayal_points;      // default: -1
	c_enum<long, char, -10, 10> m_leader_killed_points; // default: 0
	c_enum<long, char, -10, 10> m_elimination_points;   // default: 0
	c_enum<long, char, -10, 10> m_assassination_points; // default: 0
	c_enum<long, char, -10, 10> m_headshot_points;      // default: 0
	c_enum<long, char, -10, 10> m_melee_points;         // default: 0
	c_enum<long, char, -10, 10> m_sticky_points;        // default: 0
	c_enum<long, char, -10, 10> m_splatter_points;      // default: 0
	c_enum<long, char, -10, 10> m_killing_spree_points; // default: 0
	c_player_traits m_leader_traits;

	byte m_pad1[2];
};
static_assert(sizeof(c_game_engine_slayer_variant) == 0x200);

struct c_slayer_engine :
	c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

extern c_game_engine_slayer_variant*& slayer_variant;

