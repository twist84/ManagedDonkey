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
	c_flags<e_game_engine_miscellaneous_option_flags, byte_flags, k_game_engine_miscellaneous_option_flags> m_flags;

	// default: 8
	// maximum: 60
	byte m_round_time_limit; // minutes

	// default: 1
	// maximum: 15
	byte m_number_of_rounds;

	// default: 2
	// maximum: 15
	byte m_early_victory_win_count;
};
static_assert(sizeof(c_game_engine_miscellaneous_options) == 0x4);

struct c_game_engine_respawn_options
{
	c_flags<e_game_engine_respawn_options_flags, byte_flags, k_game_engine_respawn_options_flags> m_flags;

	// default: 0
	// maximum: 50
	byte m_lives_per_round;

	// default: 0
	// maximum: 100
	byte m_team_lives_per_round;

	// default: 5
	// maximum: 240
	byte m_respawn_time; // seconds

	// default: 10
	// maximum: 240
	byte m_suicide_penalty; // seconds

	// default: 5
	// maximum: 240
	byte m_betrayal_penalty; // seconds

	// halo online specific
	// default: 5
	// maximum: 240
	byte m_unknown_penalty; // seconds

	// default: 0
	// maximum: 15
	byte m_respawn_growth;

	// default: 5
	// maximum: 60
	byte m_respawn_player_traits_duration_seconds;

	// woman bound for glory, why you leaving me again?
	byte pad[3];

	c_player_traits m_respawn_player_traits;
};
static_assert(sizeof(c_game_engine_respawn_options) == 0x28);

struct c_game_engine_social_options
{
	c_flags<e_game_engine_social_options_flags, word_flags, k_game_engine_social_options_flags> m_flags;

	// default: 0
	// maximum: 2
	word m_team_changing;
};
static_assert(sizeof(c_game_engine_social_options) == 0x4);

struct c_game_engine_map_override_options
{
	c_flags<e_game_engine_map_override_options_flags, dword_flags, k_game_engine_map_override_options_flags> m_flags;

	c_player_traits m_base_player_traits;

	word m_weapon_set_absolute_index;
	word m_vehicle_set_absolute_index;

	c_player_traits m_red_powerup_traits;
	c_player_traits m_blue_powerup_traits;
	c_player_traits m_yellow_powerup_traits;

	// default: 5
	// maximum: 120
	byte m_red_powerup_traits_duration; // seconds

	// default: 30
	// maximum: 120
	byte m_blue_powerup_traits_duration; // seconds

	// default: 30
	// maximum: 120
	byte m_yellow_powerup_traits_duration; // seconds

	// gonna pack her bags and leave this house of pain
	byte pad[1];
};
static_assert(sizeof(c_game_engine_map_override_options) == 0x7C);