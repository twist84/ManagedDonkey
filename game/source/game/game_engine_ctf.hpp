#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_ctf_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_game_engine_ctf_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_ctf_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572B40, c_game_engine_ctf_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_ctf_variant* variant, bool force);
	//void set(const s_game_engine_ctf_variant_definition* definition, bool force);

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

	int16 get_touch_return_timeout() const;
	void set_touch_return_timeout(int16 touch_return_timeout);

	int16 get_sudden_death_time() const;
	void set_sudden_death_time(int16 sudden_death_time);

	int16 get_score_to_win() const;
	void set_score_to_win(int16 score_to_win);

	int16 get_flag_reset_time() const;
	void set_flag_reset_time(int16 flag_reset_time);

	c_player_traits* get_carrier_traits_writeable();
	const c_player_traits* get_carrier_traits() const;
	void set_carrier_traits(const c_player_traits* traits, bool force);

protected:
	c_flags<e_ctf_variant_flags, int8, k_ctf_variant_flags> m_variant_flags;
	c_enum<e_ctf_home_flag_waypoint_settings, int8, _ctf_home_flag_waypoint_settings_never, k_ctf_home_flag_waypoint_settings> m_home_flag_waypoint;
	c_enum<e_ctf_game_type_settings, int8, _ctf_game_type_settings_multi_flag, k_ctf_game_type_settings> m_game_type;
	c_enum<e_ctf_respawn_settings, int8, _ctf_respawn_settings_normal, k_ctf_respawn_settings> m_respawn;
	c_enum<e_ctf_touch_return_settings, int16, _ctf_touch_return_settings_off, k_ctf_touch_return_settings> m_touch_return_timeout;
	c_enum<e_ctf_sudden_death_time, int16, _ctf_sudden_death_time_infinite, k_ctf_sudden_death_times> m_sudden_death_time;
	c_enum<int32, int16, 0, 50> m_score_to_win;     // default: 5
	c_enum<int32, int16, 0, 50> m_score_unknown;    // default: 3, halo online specific
	c_enum<int32, int16, 0, 300> m_flag_reset_time; // default: 30
	c_player_traits m_carrier_traits;

	byte m_pad1[6];
};
COMPILE_ASSERT(sizeof(c_game_engine_ctf_variant) == 0x200);

struct s_static_spawn_zone;
class c_ctf_engine :
	public c_game_engine
{
public:
	virtual bool static_spawn_zone_is_valid_for_player(int32, const s_static_spawn_zone*) const;
	virtual int32 get_object_definition_index(void) const;
	virtual void process_player_holding_object(int32, int32) const;
	virtual bool update_object(const struct s_multiplayer_weapon_tracker*) const;
	virtual void initialize_object_data(int32) const;
	virtual int16 get_sudden_death_time(void) const;
	virtual void emit_object_returned_event(int32, bool) const;
	virtual bool object_should_exist(int32) const;

	// unknown function, also exists in halo 3 mcc
	virtual void* function88(void*, ...);

	virtual bool verify_state(void) const;

	void dump_settings(s_file_reference* file) const;
};

struct s_ctf_shared_globals
{
	c_static_array<c_area_set<c_area, 3>, 9> goal_areas_by_team_designator;
	c_static_array<c_area_set<c_area, 3>, 9> spawn_areas_by_team_designator;
	bool supress_reset_message;
	bool object_returned_by_player;
	int32 defensive_team_index;
	int32 sudden_death_ticks;
	int32 grace_period_ticks;
	uns16 helper_flags;
	uns16 flags;
};
COMPILE_ASSERT(sizeof(s_ctf_shared_globals) == 0x164C);

struct s_ctf_globals : s_ctf_shared_globals
{
	c_static_array<int32, 9> touch_return_timer;
	c_static_array<int16, 9> flag_reset_timer;
	c_static_array<int32, 9> player_emblem_for_flag;
	c_static_array<uns8, 9> flag_weapon_flags;
	c_static_array<int16, 9> failure_event_timer;
	c_static_array<int16, 9> timeout_return_second_counter;
};
COMPILE_ASSERT(sizeof(s_ctf_globals) == 0x16D8);

extern c_game_engine_ctf_variant*& ctf_variant;

