#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_player_traits.hpp"

enum e_base_variant_flags
{
	_base_variant_flags_built_in = 0,

	k_base_variant_flags
};

enum e_game_engine_miscellaneous_option_flags
{
	_game_engine_miscellaneous_option_teams_enabled = 0,
	_game_engine_miscellaneous_option_round_reset_players,
	_game_engine_miscellaneous_option_round_reset_map,
	_game_engine_miscellaneous_option_director_allow_player_switching,

	k_game_engine_miscellaneous_option_flags
};

enum e_game_engine_respawn_options_flags
{
	_game_engine_respawn_options_inherit_respawn_time_enabled = 0,
	_game_engine_respawn_options_respawn_with_teammate_enabled,
	_game_engine_respawn_options_respawn_at_location_enabled,
	_game_engine_respawn_options_respawn_on_kills_enabled,

	// "is_auto_respawn_countdown"
	// "is_early_respawn_enabled"
	// "is_early_respawn_requested"
	// auto_respawn_enabled = !TEST_BIT(variant.respawn_options.flags, _game_engine_respawn_options_auto_respawn_disabled)
	_game_engine_respawn_options_auto_respawn_disabled,

	k_game_engine_respawn_options_flags
};

enum e_game_engine_social_options_flags
{
	_game_engine_social_options_friendly_fire_enabled = 0,
	_game_engine_social_options_betrayal_booting_enabled,
	_game_engine_social_options_enemy_voice_enabled,
	_game_engine_social_options_open_channel_voice_enabled,
	_game_engine_social_options_dead_player_voice_enabled,

	// added in halo online
	_game_engine_social_options_spartans_vs_elites_enabled,

	// removed from halo 3: odst
	//_game_engine_social_options_observers_enabled = 0,
	//_game_engine_social_options_team_changing_enabled,
	//_game_engine_social_options_team_changing_balancing_only_enabled,

	// add back these even if the game doesn't use them
	// these bits aren't checked so it should be fine?
	_game_engine_social_options_observers_enabled,
	_game_engine_social_options_team_changing_enabled,
	_game_engine_social_options_team_changing_balancing_only_enabled,

	k_game_engine_social_options_flags
};

enum e_game_engine_map_override_option_flags
{
	_game_engine_map_override_option_grenades_on_map_enabled = 0,
	_game_engine_map_override_option_indestructible_vehicles_enabled,

	k_game_engine_map_override_option_flags
};

struct c_game_engine_miscellaneous_options
{
public:
	void byteswap();

	void set(c_game_engine_miscellaneous_options const* options, bool force);
	//void set(s_game_engine_miscellaneous_options_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_teams_enabled() const;
	void set_teams_enabled(bool teams_enabled);

	bool get_round_reset_players() const;
	void set_round_reset_players(bool round_reset_players);

	bool get_round_reset_map() const;
	void set_round_reset_map(bool round_reset_map);

	uint8 get_round_time_limit_minutes() const;
	void set_round_time_limit_minutes(long round_time_limit_minutes);

	long get_round_time_limit_seconds() const;
	void set_round_time_limit_seconds(long round_time_limit_seconds);

	uint8 get_round_limit() const;
	void set_round_limit(long round_limit);

	uint8 get_early_victory_win_count() const;
	void set_early_victory_win_count(long early_victory_win_count);

protected:
	c_flags<e_game_engine_miscellaneous_option_flags, uint8, k_game_engine_miscellaneous_option_flags> m_flags;
	c_enum<long, uint8, 8, 60> m_round_time_limit_minutes;
	c_enum<long, uint8, 1, 15> m_round_limit;
	c_enum<long, uint8, 2, 15> m_early_victory_win_count;
};
static_assert(sizeof(c_game_engine_miscellaneous_options) == 0x4);

struct c_game_engine_respawn_options
{
public:
	void byteswap();

	void set(c_game_engine_respawn_options const* options, bool force);
	//void set(s_game_engine_respawn_options_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_inherit_respawn_time_enabled() const;
	void set_inherit_respawn_time_enabled(bool respawn_time_enabled);

	bool get_respawn_with_teammate_enabled() const;
	void set_respawn_with_teammate_enabled(bool respawn_with_teammate_enabled);

	bool get_respawn_at_location_enabled() const;
	void set_respawn_at_location_enabled(bool respawn_at_location_enabled);

	bool get_respawn_on_kills_enabled() const;
	void set_respawn_on_kills_enabled(bool respawn_on_kills_enabled);

	bool get_auto_respawn_disabled() const;
	void set_auto_respawn_disabled(bool auto_respawn_disabled);

	uint8 get_lives_per_round() const;
	void set_lives_per_round(long lives_per_round);

	uint8 get_team_lives_per_round() const;
	void set_team_lives_per_round(long team_lives_per_round);

	uint8 get_respawn_time_seconds() const;
	void set_respawn_time_seconds(long respawn_time_seconds);

	uint8 get_suicide_penalty_seconds() const;
	void set_suicide_penalty_seconds(long suicide_penalty_seconds);

	uint8 get_betrayal_penalty_seconds() const;
	void set_betrayal_penalty_seconds(long betrayal_penalty_seconds);

	uint8 get_unknown_penalty_seconds() const;
	void set_unknown_penalty_seconds(long unknown_penalty_seconds);

	uint8 get_respawn_growth_seconds() const;
	void set_respawn_growth_seconds(long respawn_growth_seconds);

	uint8 get_respawn_player_traits_duration_seconds() const;
	void set_respawn_player_traits_duration_seconds(long respawn_player_traits_duration_seconds);

	c_player_traits* get_respawn_player_traits_writeable();
	c_player_traits const* get_respawn_player_traits() const;
	void set_respawn_player_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_game_engine_respawn_options_flags, uint8, k_game_engine_respawn_options_flags> m_flags;
	c_enum<long, uint8, 0, 50> m_lives_per_round;
	c_enum<long, uint8, 0, 100> m_team_lives_per_round;
	c_enum<long, uint8, 5, 100> m_respawn_time_seconds;
	c_enum<long, uint8, 10, 240> m_suicide_penalty_seconds;
	c_enum<long, uint8, 5, 240> m_betrayal_penalty_seconds;
	c_enum<long, uint8, 5, 240> m_unknown_penalty_seconds;
	c_enum<long, uint8, 0, 15> m_respawn_growth_seconds;
	c_enum<long, uint8, 5, 60> m_respawn_player_traits_duration_seconds;

	// woman bound for glory, why you leaving me again?
	uint8 pad[3];

	c_player_traits m_respawn_player_traits;
};
static_assert(sizeof(c_game_engine_respawn_options) == 0x28);

struct c_game_engine_social_options
{
public:
	void byteswap();

	void set(c_game_engine_social_options const* options, bool force);
	//void set(s_game_engine_social_options_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_friendly_fire_enabled() const;
	void set_friendly_fire_enabled(bool friendly_fire_enabled);

	bool get_betrayal_booting_enabled() const;
	void set_betrayal_booting_enabled(bool betrayal_booting_enabled);

	bool get_enemy_voice_enabled() const;
	void set_enemy_voice_enabled(bool enemy_voice_enabled);
	
	bool get_open_channel_voice_enabled() const;
	void set_open_channel_voice_enabled(bool open_channel_voice_enabled);
	
	bool get_dead_player_voice_enabled() const;
	void set_dead_player_voice_enabled(bool dead_player_voice_enabled);

	bool get_spartans_vs_elites_enabled() const;
	void set_spartans_vs_elites_enabled(bool spartans_vs_elites_enabled);

	bool get_observers_enabled() const;
	void set_observers_enabled(bool observers_enabled);

	bool get_team_changing_enabled() const;
	void set_team_changing_enabled(bool team_changing_enabled);

	bool get_team_changing_balancing_only_enabled() const;
	void set_team_changing_balancing_only_enabled(bool team_changing_balancing_only_enabled);

	long get_team_changing_setting() const;
	void set_team_changing_setting(long team_changing);

protected:
	c_flags<e_game_engine_social_options_flags, uint16, k_game_engine_social_options_flags> m_flags;
	c_enum<long, uint16, 0, 2> m_team_changing;
};
static_assert(sizeof(c_game_engine_social_options) == 0x4);

struct c_game_engine_map_override_options
{
public:
	void byteswap();

	void set(c_game_engine_map_override_options const* options, bool force);
	//void set(s_game_engine_map_override_options_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_grenades_on_map_enabled() const;
	void set_grenades_on_map_enabled(bool grenades_on_map_enabled);

	bool get_indestructible_vehicles_enabled() const;
	void set_indestructible_vehicles_enabled(bool indestructible_vehicles_enabled);

	c_player_traits* get_base_player_traits_writeable();
	c_player_traits const* get_base_player_traits() const;
	void set_base_player_traits(c_player_traits const* traits, bool force);

	short get_weapon_set_absolute_index() const;
	void set_weapon_set_absolute_index(short weapon_set_absolute_index);

	short get_vehicle_set_absolute_index() const;
	void set_vehicle_set_absolute_index(short vehicle_set_absolute_index);

	long get_weapon_set() const;
	void set_weapon_set(long weapon_set);
	
	long get_vehicle_set() const;
	void set_vehicle_set(long vehicle_set);

	c_player_traits* get_red_powerup_traits_writeable();
	c_player_traits const* get_red_powerup_traits() const;
	void set_red_powerup_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_blue_powerup_traits_writeable();
	c_player_traits const* get_blue_powerup_traits() const;
	void set_blue_powerup_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_yellow_powerup_traits_writeable();
	c_player_traits const* get_yellow_powerup_traits() const;
	void set_yellow_powerup_traits(c_player_traits const* traits, bool force);

	uint8 get_red_powerup_duration_seconds() const;
	void set_red_powerup_duration_seconds(uint8 red_powerup_duration_seconds);

	uint8 get_blue_powerup_duration_seconds() const;
	void set_blue_powerup_duration_seconds(uint8 blue_powerup_duration_seconds);

	uint8 get_yellow_powerup_duration_seconds() const;
	void set_yellow_powerup_duration_seconds(uint8 yellow_powerup_duration_seconds);

protected:
	c_flags<e_game_engine_map_override_option_flags, uint32, k_game_engine_map_override_option_flags> m_flags;
	c_player_traits m_base_player_traits;
	uint16 m_weapon_set_absolute_index;
	uint16 m_vehicle_set_absolute_index;
	c_player_traits m_red_powerup_traits;
	c_player_traits m_blue_powerup_traits;
	c_player_traits m_yellow_powerup_traits;
	c_enum<long, uint8, 5, 120> m_red_powerup_duration_seconds;
	c_enum<long, uint8, 30, 120> m_blue_powerup_duration_seconds;
	c_enum<long, uint8, 30, 120> m_yellow_powerup_duration_seconds;

	// gonna pack her bags and leave this house of pain
	uint8 pad[1];
};
static_assert(sizeof(c_game_engine_map_override_options) == 0x7C);

