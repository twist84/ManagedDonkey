#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_infection_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_bitstream;
struct s_file_reference;

class c_game_engine_infection_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_infection_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572B60, c_game_engine_infection_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_infection_variant* variant, bool force);
	//void set(const s_game_engine_infection_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_respawn_on_haven_move() const;
	void set_respawn_on_haven_move(bool respawn_on_haven_move);

	e_infection_safe_havens_settings get_safe_havens() const;
	void set_safe_havens(e_infection_safe_havens_settings safe_havens);

	e_infection_next_zombie_settings get_next_zombie() const;
	void set_next_zombie(e_infection_next_zombie_settings next_zombie);

	e_infection_initial_zombie_count_settings get_initial_zombie_count() const;
	void set_initial_zombie_count(e_infection_initial_zombie_count_settings initial_zombie_count);

	int16 get_safe_haven_movement_time() const;
	void set_safe_haven_movement_time(int16 safe_haven_movement_time);

	int8 get_zombie_kill_points() const;
	void set_zombie_kill_points(int8 zombie_kill_points);

	int8 get_infection_points() const;
	void set_infection_points(int8 infection_points);

	int8 get_safe_haven_arrival_points() const;
	void set_safe_haven_arrival_points(int8 safe_haven_arrival_points);

	int8 get_suicide_points() const;
	void set_suicide_points(int8 suicide_points);

	int8 get_betrayal_points() const;
	void set_betrayal_points(int8 betrayal_points);

	int8 get_last_man_bonus_points() const;
	void set_last_man_bonus_points(int8 last_man_bonus_points);

	c_player_traits* get_zombie_traits_writeable();
	const c_player_traits* get_zombie_traits() const;
	void set_zombie_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_first_zombie_traits_writeable();
	const c_player_traits* get_first_zombie_traits() const;
	void set_first_zombie_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_safe_haven_defender_traits_writeable();
	const c_player_traits* get_safe_haven_defender_traits() const;
	void set_safe_haven_defender_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_last_human_traits_writeable();
	const c_player_traits* get_last_human_traits() const;
	void set_last_human_traits(const c_player_traits* traits, bool force);

protected:
	c_flags<e_infection_variant_flags, uns8, k_infection_variant_flags> m_variant_flags;
	c_enum<e_infection_safe_havens_settings, int8, _infection_safe_havens_off, k_infection_safe_havens_settings> m_safe_havens;
	c_enum<e_infection_next_zombie_settings, int8, _infection_next_zombie_winner, k_infection_next_zombie_settings> m_next_zombie;
	c_enum<e_infection_initial_zombie_count_settings, int8, _infection_initial_zombie_count_25_percent, k_infection_initial_zombie_count_settings> m_initial_zombie_count;
	c_enum<int32, int16, 0, 120> m_safe_haven_movement_time;  // default: 30
	c_enum<int32, int8, -10, 10> m_zombie_kill_points;        // default: 1
	c_enum<int32, int8, -10, 10> m_infection_points;          // default: 0
	c_enum<int32, int8, -10, 10> m_safe_haven_arrival_points; // default: 0
	c_enum<int32, int8, -10, 10> m_suicide_points;            // default: -1
	c_enum<int32, int8, -10, 10> m_betrayal_points;           // default: -1
	c_enum<int32, int8, -10, 10> m_last_man_bonus_points;     // default: 0
	c_player_traits m_zombie_traits;
	c_player_traits m_first_zombie_traits;
	c_player_traits m_safe_haven_defender_traits;
	c_player_traits m_last_human_traits;

	byte m_pad1[4];
};
COMPILE_ASSERT(sizeof(c_game_engine_infection_variant) == 0x250);

class c_infection_engine :
	public c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_infection_globals
{
	struct
	{
		uns16 zombie_bitvector;
		uns16 first_zombie_bitvector;
		int32 safe_haven_timer;
		uns16 zombie_team_bitvector;
		uns16 players_in_safe_haven_bitvector;
	} networked;

	struct
	{
		c_area_set<c_area, 12> safe_havens;
		c_static_array<int32, 16> zombie_kills;
		c_static_array<int32, 16> human_kills;
		bool emitted_last_man_standing_event;
	} local;
};
COMPILE_ASSERT(sizeof(s_infection_globals) == 0x550);

extern c_game_engine_infection_variant*& infection_variant;

