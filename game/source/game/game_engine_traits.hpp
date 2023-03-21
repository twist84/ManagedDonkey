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
	_game_engine_social_options_observers_enabled = 0,
	_game_engine_social_options_team_changing_enabled,
	_game_engine_social_options_team_changing_balancing_only,
	_game_engine_social_options_friendly_fire_enabled,
	_game_engine_social_options_betrayal_booting_enabled,

	// removed from halo online
	//_game_engine_social_options_enemy_voice_enabled,
	//_game_engine_social_options_open_channel_voice_enabled,
	//_game_engine_social_options_dead_player_voice_enabled,

	// added in halo online
	_game_engine_social_options_spartans_vs_elites_enabled,

	k_game_engine_social_options_flags
};

enum e_game_engine_map_override_options_flags
{
	_game_engine_map_override_options_grenades_on_map = 0,
	_game_engine_map_override_options_indestructible_vehicles,

	k_game_engine_map_override_options_flags
};

struct c_game_engine_miscellaneous_options
{
	void byteswap();

	bool get_teams_enabled() const;
	void set_teams_enabled(bool teams_enabled);

	bool get_round_reset_players() const;
	void set_round_reset_players(bool round_reset_players);

	bool get_round_reset_map() const;
	void set_round_reset_map(bool round_reset_map);

	byte get_round_time_limit_minutes() const;
	void set_round_time_limit_minutes(long round_time_limit_minutes);

	long get_round_time_limit_seconds() const;
	void set_round_time_limit_seconds(long round_time_limit_seconds);

	byte get_round_limit() const;
	void set_round_limit(long round_limit);

	byte get_early_victory_win_count() const;
	void set_early_victory_win_count(long early_victory_win_count);
	c_flags<e_game_engine_miscellaneous_option_flags, byte_flags, k_game_engine_miscellaneous_option_flags> m_flags;
	c_enum<long, byte, 8, 60> m_round_time_limit_minutes;
	c_enum<long, byte, 1, 15> m_round_limit;
	c_enum<long, byte, 2, 15> m_early_victory_win_count;
};
static_assert(sizeof(c_game_engine_miscellaneous_options) == 0x4);

struct c_game_engine_respawn_options
{
	void byteswap();

	c_flags<e_game_engine_respawn_options_flags, byte_flags, k_game_engine_respawn_options_flags> m_flags;
	c_enum<long, byte, 0, 50> m_lives_per_round;
	c_enum<long, byte, 0, 100> m_team_lives_per_round;
	c_enum<long, byte, 5, 100> m_respawn_time_seconds;
	c_enum<long, byte, 10, 240> m_suicide_penalty_seconds;
	c_enum<long, byte, 5, 240> m_betrayal_penalty_seconds;
	c_enum<long, byte, 5, 240> m_unknown_penalty_seconds;
	c_enum<long, byte, 0, 15> m_respawn_growth_seconds;
	c_enum<long, byte, 5, 60> m_respawn_player_traits_duration_seconds;

	// woman bound for glory, why you leaving me again?
	byte pad[3];

	c_player_traits m_respawn_player_traits;
};
static_assert(sizeof(c_game_engine_respawn_options) == 0x28);

struct c_game_engine_social_options
{
	void byteswap();

	c_flags<e_game_engine_social_options_flags, word_flags, k_game_engine_social_options_flags> m_flags;
	c_enum<long, word, 0, 2> m_team_changing;
};
static_assert(sizeof(c_game_engine_social_options) == 0x4);

struct c_game_engine_map_override_options
{
	void byteswap();

	c_flags<e_game_engine_map_override_options_flags, dword_flags, k_game_engine_map_override_options_flags> m_flags;
	c_player_traits m_base_player_traits;
	word m_weapon_set_absolute_index;
	word m_vehicle_set_absolute_index;
	c_player_traits m_red_powerup_traits;
	c_player_traits m_blue_powerup_traits;
	c_player_traits m_yellow_powerup_traits;
	c_enum<long, byte, 5, 120> m_red_powerup_duration_seconds;
	c_enum<long, byte, 30, 120> m_blue_powerup_duration_seconds;
	c_enum<long, byte, 30, 120> m_yellow_powerup_duration_seconds;

	// gonna pack her bags and leave this house of pain
	byte pad[1];
};
static_assert(sizeof(c_game_engine_map_override_options) == 0x7C);

