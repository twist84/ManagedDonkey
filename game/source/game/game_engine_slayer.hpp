#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_slayer_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_bitstream;
struct s_file_reference;

class c_game_engine_slayer_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_slayer_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572C20, c_game_engine_slayer_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_slayer_variant* variant, bool force);
	//void set(const s_game_engine_slayer_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	int16 get_score_to_win() const;
	void set_score_to_win(int16 score_to_win);

	int8 get_kill_points() const;
	void set_kill_points(int8 kill_points);

	int8 get_assist_points() const;
	void set_assist_points(int8 assist_points);

	int8 get_death_points() const;
	void set_death_points(int8 death_points);

	int8 get_suicide_points() const;
	void set_suicide_points(int8 suicide_points);

	int8 get_betrayal_points() const;
	void set_betrayal_points(int8 betrayal_points);

	int8 get_leader_killed_points() const;
	void set_leader_killed_points(int8 leader_killed_points);

	int8 get_elimination_points() const;
	void set_elimination_points(int8 elimination_points);

	int8 get_assassination_points() const;
	void set_assassination_points(int8 assassination_points);

	int8 get_headshot_points() const;
	void set_headshot_points(int8 headshot_points);

	int8 get_melee_points() const;
	void set_melee_points(int8 melee_points);

	int8 get_sticky_points() const;
	void set_sticky_points(int8 sticky_points);

	int8 get_splatter_points() const;
	void set_splatter_points(int8 splatter_points);

	int8 get_killing_spree_points() const;
	void set_killing_spree_points(int8 killing_spree_points);

	c_player_traits* get_leader_traits_writeable();
	const c_player_traits* get_leader_traits() const;
	void set_leader_traits(const c_player_traits* traits, bool force);

protected:
	c_enum<int32, int16, -1, 250> m_score_to_win;        // default: 25
	c_enum<int32, int16, 0, 250> m_score_unknown;        // default: 20, halo online specific
	c_enum<int32, int16, -10, 10> m_kill_points;         // default: 1
	c_enum<int32, int8, -10, 10> m_assist_points;        // default: 0
	c_enum<int32, int8, -10, 10> m_death_points;         // default: 0
	c_enum<int32, int8, -10, 10> m_suicide_points;       // default: -1
	c_enum<int32, int8, -10, 10> m_betrayal_points;      // default: -1
	c_enum<int32, int8, -10, 10> m_leader_killed_points; // default: 0
	c_enum<int32, int8, -10, 10> m_elimination_points;   // default: 0
	c_enum<int32, int8, -10, 10> m_assassination_points; // default: 0
	c_enum<int32, int8, -10, 10> m_headshot_points;      // default: 0
	c_enum<int32, int8, -10, 10> m_melee_points;         // default: 0
	c_enum<int32, int8, -10, 10> m_sticky_points;        // default: 0
	c_enum<int32, int8, -10, 10> m_splatter_points;      // default: 0
	c_enum<int32, int8, -10, 10> m_killing_spree_points; // default: 0
	c_player_traits m_leader_traits;

	byte m_pad1[2];
};
COMPILE_ASSERT(sizeof(c_game_engine_slayer_variant) == 0x200);

class c_slayer_engine :
	public c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

extern c_game_engine_slayer_variant*& slayer_variant;

