#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_ctf_traits.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_ctf_variant : c_game_engine_base_variant
{
public:
	c_game_engine_ctf_variant* constructor()
	{
		return DECLFUNC(0x00572B40, c_game_engine_ctf_variant*, __thiscall, c_game_engine_ctf_variant*)(this);
	}

	void byteswap();

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
struct c_ctf_engine : c_game_engine
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
};

struct s_ctf_shared_globals
{
	c_static_array<c_area_set<c_area, 3>, 9> area_set0;
	c_static_array<c_area_set<c_area, 3>, 9> area_setB1C;
	byte __data1638[0x12];
	word_flags flags;
};
static_assert(sizeof(s_ctf_shared_globals) == 0x164C);

struct s_ctf_globals
{
	s_ctf_shared_globals shared_globals;
	c_static_array<dword, 9> __unknown164C;
	c_static_array<word, 9> __unknown1670;
	c_static_array<dword, 9> __unknown1684;
	c_static_array<byte, 9> __unknown16A8;
	c_static_array<word, 9> __unknown16B2;
	c_static_array<word, 9> __unknown16C4;
};
static_assert(sizeof(s_ctf_globals) == 0x16D8);

extern c_game_engine_ctf_variant*& ctf_variant;

