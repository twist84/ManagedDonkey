#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_ctf_traits.hpp"
#include "game/game_engine_player_traits.hpp"

struct c_game_engine_ctf_variant :
	c_game_engine_base_variant
{
public:
	c_game_engine_ctf_variant* constructor()
	{
		return DECLFUNC(0x00572B40, c_game_engine_ctf_variant*, __thiscall, c_game_engine_ctf_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_ctf_variant const* variant, bool force);
	//void set(s_game_engine_ctf_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_flag_at_home_to_score() const;
	void set_flag_at_home_to_score(bool flag_at_home_to_score);

	e_ctf_home_flag_waypoint_settings get_home_flag_waypoint() const;
	void set_home_flag_waypoint(e_ctf_home_flag_waypoint_settings home_flag_waypoint);

	e_ctf_game_type_settings get_game_type() const;
	void set_game_type(e_ctf_game_type_settings game_type);

	e_ctf_respawn_settings get_respawn() const;
	void set_respawn(e_ctf_respawn_settings respawn);

	short get_touch_return_timeout() const;
	void set_touch_return_timeout(short touch_return_timeout);

	short get_sudden_death_time() const;
	void set_sudden_death_time(short sudden_death_time);

	short get_score_to_win() const;
	void set_score_to_win(short score_to_win);

	short get_flag_reset_time() const;
	void set_flag_reset_time(short flag_reset_time);

	c_player_traits* get_carrier_traits_writeable();
	c_player_traits const* get_carrier_traits() const;
	void set_carrier_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_ctf_variant_flags, char, k_ctf_variant_flags> m_variant_flags;
	c_enum<e_ctf_home_flag_waypoint_settings, char, _ctf_home_flag_waypoint_settings_never, k_ctf_home_flag_waypoint_settings> m_home_flag_waypoint;
	c_enum<e_ctf_game_type_settings, char, _ctf_game_type_settings_multi_flag, k_ctf_game_type_settings> m_game_type;
	c_enum<e_ctf_respawn_settings, char, _ctf_respawn_settings_normal, k_ctf_respawn_settings> m_respawn;
	c_enum<e_ctf_touch_return_settings, short, _ctf_touch_return_settings_off, k_ctf_touch_return_settings> m_touch_return_timeout;
	c_enum<e_ctf_sudden_death_time, short, _ctf_sudden_death_time_infinite, k_ctf_sudden_death_times> m_sudden_death_time;
	c_enum<long, short, 0, 50> m_score_to_win;     // default: 5
	c_enum<long, short, 0, 50> m_score_unknown;    // default: 3, halo online specific
	c_enum<long, short, 0, 300> m_flag_reset_time; // default: 30
	c_player_traits m_carrier_traits;

	byte m_pad1[6];
};
static_assert(sizeof(c_game_engine_ctf_variant) == 0x200);

struct s_static_spawn_zone;
struct c_ctf_engine :
	c_game_engine
{
public:
	virtual bool static_spawn_zone_is_valid_for_player(long, s_static_spawn_zone const*) const;
	virtual long get_object_definition_index(void) const;
	virtual void process_player_holding_object(long, long) const;
	virtual bool update_object(struct s_multiplayer_weapon_tracker const*) const;
	virtual void initialize_object_data(long) const;
	virtual short get_sudden_death_time(void) const;
	virtual void emit_object_returned_event(long, bool) const;
	virtual bool object_should_exist(long) const;

	// unknown function, also exists in halo 3 mcc
	virtual void* function88(void*, ...);

	virtual bool verify_state(void) const;

	void dump_settings(s_file_reference* file) const;
};

struct s_ctf_shared_globals
{
	c_static_array<c_area_set<c_area, 3>, 9> area_set0;
	c_static_array<c_area_set<c_area, 3>, 9> area_setB1C;
	bool __unknown1638;
	bool __unknown1639;
	byte __data163A[0x2];
	long defensive_team;
	long __unknown1640;
	byte __data1644[0x4];
	word_flags helper_flags;
	word_flags flags;
};
static_assert(sizeof(s_ctf_shared_globals) == 0x164C);

struct s_ctf_globals
{
	s_ctf_shared_globals shared_globals;
	c_static_array<dword, 9> touch_return_timers;
	c_static_array<word, 9> flag_reset_timers;
	c_static_array<dword, 9> __unknown1684;
	c_static_array<byte_flags, 9> flag_weapon_flags;
	c_static_array<word, 9> __unknown16B2;
	c_static_array<word, 9> __unknown16C4;
};
//static_assert(sizeof(s_ctf_globals) == 0x16D8);

extern c_game_engine_ctf_variant*& ctf_variant;

