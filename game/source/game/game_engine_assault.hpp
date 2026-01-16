#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_assault_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_bitstream;

class c_game_engine_assault_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_assault_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572B00, c_game_engine_assault_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_assault_variant* variant, bool force);
	//void set(const s_game_engine_assault_variant_definition* definition, bool force);

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

	int16 get_score_to_win() const;
	void set_score_to_win(int16 score_to_win);

	int16 get_sudden_death_time() const;
	void set_sudden_death_time(int16 sudden_death_time);

	int16 get_bomb_reset_time() const;
	void set_bomb_reset_time(int16 bomb_reset_time);

	int16 get_bomb_arming_time() const;
	void set_bomb_arming_time(int16 bomb_arming_time);

	int16 get_bomb_disarming_time() const;
	void set_bomb_disarming_time(int16 bomb_disarming_time);

	int16 get_bomb_fuse_time() const;
	void set_bomb_fuse_time(int16 bomb_fuse_time);

	c_player_traits* get_carrier_traits_writeable();
	const c_player_traits* get_carrier_traits() const;
	void set_carrier_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_arming_traits_writeable();
	const c_player_traits* get_arming_traits() const;
	void set_arming_traits(const c_player_traits* traits, bool force);

protected:
	c_flags<e_assault_variant_flags, uns16, k_assault_variant_flags> m_variant_flags;
	c_enum<e_assault_respawn_settings, int16, _assault_respawn_settings_disabled, k_assault_respawn_settings> m_respawn;
	c_enum<e_assault_game_type_settings, int16, _assault_game_type_settings_multi_bomb, k_assault_game_type_settings> m_game_type;
	c_enum<e_assault_enemy_bomb_waypoint_settings, int16, _assault_enemy_bomb_waypoint_settings_never, k_assault_enemy_bomb_waypoint_settings> m_enemy_bomb_waypoint;
	c_enum<int32, int16, 0, 50> m_score_to_win;        // default: 3
	c_enum<int32, int16, 0, 50> m_score_unknown0;      // default: 2, halo online specific
	c_enum<int32, int16, 0, 50> m_score_unknown1;      // default: 0, halo online specific
	c_enum<int32, int16, 0, 50> m_score_unknown2;      // default: 0, halo online specific
	c_enum<int32, int16, 0, 50> m_score_unknown3;      // default: 0, halo online specific
	c_enum<e_assault_variant_sudden_death_time, int16, _assault_variant_sudden_death_time_infinite, k_assault_variant_sudden_death_time> m_sudden_death_time;
	c_enum<int32, int16, 0, 90> m_bomb_reset_time;     // default: 30, 0-60 in halo 3
	c_enum<int32, int16, 0, 90> m_bomb_arming_time;    // default: 35, 0-60 in halo 3
	c_enum<int32, int16, 0, 90> m_bomb_disarming_time; // default: 35, 0-60 in halo 3
	c_enum<int32, int16, 0, 90> m_bomb_fuse_time;      // default: 35, 0-60 in halo 3
	c_player_traits m_carrier_traits;
	c_player_traits m_arming_traits;

	byte m_pad1[4];
};
COMPILE_ASSERT(sizeof(c_game_engine_assault_variant) == 0x228);

class c_assault_engine :
	public c_ctf_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_assault_globals : s_ctf_shared_globals
{
	c_static_array<int32, 9> arming_or_disarming_timer;
	c_static_array<int16, 9> bomb_reset_timer;
	c_static_array<uns8, 2> bomb_flags;
	c_static_array<int32, 9> player_that_placed_bomb;
	c_static_array<uns8, 8> __unknown16A8;
	c_static_array<int16, 9> assault_timeout_return_second_counter;
};
COMPILE_ASSERT(sizeof(s_assault_globals) == 0x16C4);

extern c_game_engine_assault_variant*& assault_variant;

