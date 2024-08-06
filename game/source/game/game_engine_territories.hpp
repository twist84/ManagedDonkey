#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_territories_traits.hpp"
#include "game/game_engine_player_traits.hpp"

struct c_bitstream;

struct c_game_engine_territories_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_territories_variant* constructor()
	{
		return DECLFUNC(0x00572C50, c_game_engine_territories_variant*, __thiscall, c_game_engine_territories_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_territories_variant const* variant, bool force);
	//void set(s_game_engine_territories_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_one_sided() const;
	void set_one_sided(bool one_sided);

	bool get_lock_after_first_capture() const;
	void set_lock_after_first_capture(bool lock_after_first_capture);

	e_territories_respawn_on_capture_settings get_respawn_on_capture() const;
	void set_respawn_on_capture(e_territories_respawn_on_capture_settings respawn_on_capture);

	short get_capture_time() const;
	void set_capture_time(short capture_time);

	short get_sudden_death_time() const;
	bool get_sudden_death_enabled() const;
	void set_sudden_death_time(short sudden_death_time);

	c_player_traits* get_defender_traits_writeable();
	c_player_traits const* get_defender_traits() const;
	void set_defender_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_attacker_traits_writeable();
	c_player_traits const* get_attacker_traits() const;
	void set_attacker_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_territories_variant_flags, word_flags, k_territories_variant_flags> m_variant_flags;
	c_enum<e_territories_respawn_on_capture_settings, short, _territories_respawn_on_capture_settings_disabled, k_territories_respawn_on_capture_settings> m_respawn_on_capture;
	c_enum<e_territories_capture_time_settings, short, _territories_capture_time_settings_instant, k_territories_capture_time_settings> m_capture_time;
	c_enum<e_territories_sudden_death_settings, short, _territories_sudden_death_settings_indefinite, k_territories_sudden_death_settings> m_sudden_death_time;
	c_player_traits m_defender_traits;
	c_player_traits m_attacker_traits;
};
static_assert(sizeof(c_game_engine_territories_variant) == 0x210);

struct c_territories_engine :
	c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_territory_data :
	c_area
{
	c_static_array<word, 8> __unknown64;
	c_static_array<byte, 8> __unknown74;
	long __unknown7C;
	long __unknown80;
};
static_assert(sizeof(s_territory_data) == 0x84);

struct s_territories_globals
{
	c_area_set<s_territory_data, 8> area_set;
	long __unknown430;
	long __unknown434;
	long __unknown438;
};
static_assert(sizeof(s_territories_globals) == 0x43C);

extern c_game_engine_territories_variant*& territories_variant;

