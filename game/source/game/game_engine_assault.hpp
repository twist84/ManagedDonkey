#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_assault_traits.hpp"
#include "game/game_engine_player_traits.hpp"

struct c_bitstream;

struct c_game_engine_assault_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_assault_variant* constructor()
	{
		return DECLFUNC(0x00572B00, c_game_engine_assault_variant*, __thiscall, c_game_engine_assault_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_assault_variant const* variant, bool force);
	//void set(s_game_engine_assault_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_reset_bomb_on_disarm() const;
	void set_reset_bomb_on_disarm(bool reset_bomb_on_disarm);
	
	bool get_siege_mode() const;
	void set_siege_mode(bool siege_mode);

	e_assault_respawn_settings get_respawn() const;
	void set_respawn(e_assault_respawn_settings respawn);

	e_assault_game_type_settings get_game_type() const;
	void set_game_type(e_assault_game_type_settings game_type);

	e_assault_enemy_bomb_waypoint_settings get_enemy_bomb_waypoint() const;
	void set_enemy_bomb_waypoint(e_assault_enemy_bomb_waypoint_settings enemy_bomb_waypoint);

	short get_score_to_win() const;
	void set_score_to_win(short score_to_win);

	short get_sudden_death_time() const;
	void set_sudden_death_time(short sudden_death_time);

	short get_bomb_reset_time() const;
	void set_bomb_reset_time(short bomb_reset_time);

	short get_bomb_arming_time() const;
	void set_bomb_arming_time(short bomb_arming_time);

	short get_bomb_disarming_time() const;
	void set_bomb_disarming_time(short bomb_disarming_time);

	short get_bomb_fuse_time() const;
	void set_bomb_fuse_time(short bomb_fuse_time);

	c_player_traits* get_carrier_traits_writeable();
	c_player_traits const* get_carrier_traits() const;
	void set_carrier_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_arming_traits_writeable();
	c_player_traits const* get_arming_traits() const;
	void set_arming_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_assault_variant_flags, word_flags, k_assault_variant_flags> m_variant_flags;
	c_enum<e_assault_respawn_settings, short, _assault_respawn_settings_disabled, k_assault_respawn_settings> m_respawn;
	c_enum<e_assault_game_type_settings, short, _assault_game_type_settings_multi_bomb, k_assault_game_type_settings> m_game_type;
	c_enum<e_assault_enemy_bomb_waypoint_settings, short, _assault_enemy_bomb_waypoint_settings_never, k_assault_enemy_bomb_waypoint_settings> m_enemy_bomb_waypoint;
	c_enum<long, short, 0, 50> m_score_to_win;        // default: 3
	c_enum<long, short, 0, 50> m_score_unknown0;      // default: 2, halo online specific
	c_enum<long, short, 0, 50> m_score_unknown1;      // default: 0, halo online specific
	c_enum<long, short, 0, 50> m_score_unknown2;      // default: 0, halo online specific
	c_enum<long, short, 0, 50> m_score_unknown3;      // default: 0, halo online specific
	c_enum<e_assault_variant_sudden_death_time, short, _assault_variant_sudden_death_time_infinite, k_assault_variant_sudden_death_time> m_sudden_death_time;
	c_enum<long, short, 0, 90> m_bomb_reset_time;     // default: 30, 0-60 in halo 3
	c_enum<long, short, 0, 90> m_bomb_arming_time;    // default: 35, 0-60 in halo 3
	c_enum<long, short, 0, 90> m_bomb_disarming_time; // default: 35, 0-60 in halo 3
	c_enum<long, short, 0, 90> m_bomb_fuse_time;      // default: 35, 0-60 in halo 3
	c_player_traits m_carrier_traits;
	c_player_traits m_arming_traits;

	byte m_pad1[4];
};
static_assert(sizeof(c_game_engine_assault_variant) == 0x228);

struct c_assault_engine :
	c_ctf_engine
{
public:
	void dump_settings(s_file_reference* file) const;
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
//static_assert(sizeof(s_assault_globals) == 0x16C4);

extern c_game_engine_assault_variant*& assault_variant;

