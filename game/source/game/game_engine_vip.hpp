#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_vip_traits.hpp"
#include "game/game_engine_player_traits.hpp"

class c_bitstream;
struct s_file_reference;

class c_game_engine_vip_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_vip_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572C70, c_game_engine_vip_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_vip_variant* variant, bool force);
	//void set(const s_game_engine_vip_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_single_vip() const;
	void set_single_vip(bool single_vip);

	bool get_destination_zones_enabled() const;
	void set_destination_zones_enabled(bool destination_zones_enabled);

	bool get_should_end_round_on_vip_death() const;
	void set_end_round_on_vip_death(bool end_round_on_vip_death);

	//virtual int32 get_score_to_win_round() override;?
	void set_score_to_win_round(int16 score_to_win_round);

	int8 get_kill_points() const;
	void set_kill_points(int8 kill_points);

	int8 get_takedown_points() const;
	void set_takedown_points(int8 takedown_points);

	int8 get_kill_as_vip_points() const;
	void set_kill_as_vip_points(int8 kill_as_vip_points);

	int8 get_vip_death_points() const;
	void set_vip_death_points(int8 vip_death_points);

	int8 get_destination_arrival_points() const;
	void set_destination_arrival_points(int8 destination_arrival_points);

	int8 get_suicide_points() const;
	void set_suicide_points(int8 suicide_points);

	int8 get_betrayal_points() const;
	void set_betrayal_points(int8 betrayal_points);

	int8 get_vip_suicide_points() const;
	void set_vip_suicide_points(int8 vip_suicide_points);

	e_vip_vip_selection_settings get_vip_selection() const;
	void set_vip_selection(e_vip_vip_selection_settings vip_selection);

	e_vip_zone_movement_settings get_zone_movement() const;
	int32 get_zone_movement_time_in_seconds() const;
	void set_zone_movement(e_vip_zone_movement_settings zone_movement);

	e_vip_zone_order_settings get_zone_order() const;
	void set_zone_order(e_vip_zone_order_settings zone_order);

	int16 get_influence_radius() const;
	void set_influence_radius(int16 influence_radius);

	c_player_traits* get_vip_team_traits_writeable();
	const c_player_traits* get_vip_team_traits() const;
	void set_vip_team_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_vip_influence_traits_writeable();
	const c_player_traits* get_vip_influence_traits() const;
	void set_vip_influence_traits(const c_player_traits* traits, bool force);

	c_player_traits* get_vip_traits_writeable();
	const c_player_traits* get_vip_traits() const;
	void set_vip_traits(const c_player_traits* traits, bool force);

protected:
	c_enum<int32, int16, 0, 500> m_score_to_win_round;         // default: 15
	c_enum<int32, int16, 0, 500> m_score_unknown;              // default: 10, halo online specific
	c_flags<e_vip_variant_flags, uns16, k_vip_variant_flags> m_variant_flags;
	c_enum<int32, int8, -10, 10> m_kill_points;                // default: 0
	c_enum<int32, int8, -10, 10> m_takedown_points;            // default: 0
	c_enum<int32, int8, -10, 10> m_kill_as_vip_points;         // default: 0
	c_enum<int32, int8, -10, 10> m_vip_death_points;           // default: 0
	c_enum<int32, int8, -10, 10> m_destination_arrival_points; // default: 0
	c_enum<int32, int8, -10, 10> m_suicide_points;             // default: 0
	c_enum<int32, int8, -10, 10> m_betrayal_points;            // default: 0
	c_enum<int32, int8, -10, 10> m_vip_suicide_points;         // default: 0
	c_enum<e_vip_vip_selection_settings, uns8, _vip_vip_selection_settings_random, k_vip_vip_selection_settings> m_vip_selection;
	c_enum<e_vip_zone_movement_settings, uns8, _vip_zone_movement_settings_off, k_number_of_vip_zone_movement_settings> m_zone_movement;
	c_enum<e_vip_zone_order_settings, uns8, _vip_zone_order_settings_random, k_vip_zone_order_settings> m_zone_order;

	byte m_pad1[1];

	c_enum<int32, int16, 0, 50> m_influence_radius;
	c_player_traits m_vip_team_traits;
	c_player_traits m_vip_influence_traits;
	c_player_traits m_vip_traits;
};
COMPILE_ASSERT(sizeof(c_game_engine_vip_variant) == 0x238);

class c_vip_engine :
	public c_game_engine
{
public:
	void dump_settings(s_file_reference* file) const;
};

class c_destination_zone :
	public c_area
{
public:
	uns8 m_team_has_visited;
};
COMPILE_ASSERT(sizeof(c_destination_zone) == 0x68);

struct s_vip_globals
{
	c_static_array<int32, 8> vip_player_index;
	int32 defending_team_index;
	c_static_array<s_multiplayer_object_boundary_geometry_data, 8> vip_influence_area;
	c_area_set<c_destination_zone, 12> zones;
};
COMPILE_ASSERT(sizeof(s_vip_globals) == 0x794);

extern c_game_engine_vip_variant*& vip_variant;

