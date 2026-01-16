#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_territories_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_bitstream;

class c_game_engine_territories_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_territories_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572C50, c_game_engine_territories_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_territories_variant* variant, bool force);
	//void set(const s_game_engine_territories_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_one_sided() const;
	void set_one_sided(bool one_sided);

	bool get_lock_after_first_capture() const;
	void set_lock_after_first_capture(bool lock_after_first_capture);

	e_territories_respawn_on_capture_settings get_respawn_on_capture() const;
	void set_respawn_on_capture(e_territories_respawn_on_capture_settings respawn_on_capture);

	int16 get_capture_time() const;
	void set_capture_time(int16 capture_time);

	int16 get_sudden_death_time() const;
	bool get_sudden_death_enabled() const;
	void set_sudden_death_time(int16 sudden_death_time);

	c_player_traits* get_defender_traits_writeable();
	const c_player_traits* get_defender_traits() const;
	void set_defender_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_attacker_traits_writeable();
	const c_player_traits* get_attacker_traits() const;
	void set_attacker_traits(const c_player_traits* traits, bool force);

protected:
	c_flags<e_territories_variant_flags, uns16, k_territories_variant_flags> m_variant_flags;
	c_enum<e_territories_respawn_on_capture_settings, int16, _territories_respawn_on_capture_settings_disabled, k_territories_respawn_on_capture_settings> m_respawn_on_capture;
	c_enum<e_territories_capture_time_settings, int16, _territories_capture_time_settings_instant, k_territories_capture_time_settings> m_capture_time;
	c_enum<e_territories_sudden_death_settings, int16, _territories_sudden_death_settings_indefinite, k_territories_sudden_death_settings> m_sudden_death_time;
	c_player_traits m_defender_traits;
	c_player_traits m_attacker_traits;
};
COMPILE_ASSERT(sizeof(c_game_engine_territories_variant) == 0x210);

class c_territories_engine :
	public c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

struct s_territory_data :
	c_area
{
	c_static_array<uns16, 8> team_capture_time;
	c_static_array<uns8, 8> team_capture_time_in_seconds;
	int32 controlling_team_designator;
	int32 state;
};
COMPILE_ASSERT(sizeof(s_territory_data) == 0x84);

struct s_territories_globals
{
	c_area_set<s_territory_data, 8> territories;
	int32 defending_team_index;
	int32 sudden_death_time_left;
	int32 grace_period_time_left;
};
COMPILE_ASSERT(sizeof(s_territories_globals) == 0x43C);

extern c_game_engine_territories_variant*& territories_variant;

