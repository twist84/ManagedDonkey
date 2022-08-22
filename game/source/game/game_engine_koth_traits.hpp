#pragma once

enum e_king_team_scoring_settings
{
	_king_team_scoring_settings_sum = 0,
	_king_team_scoring_settings_minimum,
	_king_team_scoring_settings_maximum,
	_king_team_scoring_settings_team_score,

	k_king_team_scoring_settings
};

enum e_king_variant_flags
{
	_king_variant_flags_opaque_hill = 0,

	k_king_variant_flags
};

enum e_king_moving_hill_settings
{
	_king_moving_hill_settings_off = 0,
	_king_moving_hill_settings_10_seconds,
	_king_moving_hill_settings_15_seconds,
	_king_moving_hill_settings_30_seconds,
	_king_moving_hill_settings_1_minute,
	_king_moving_hill_settings_2_minutes,
	_king_moving_hill_settings_3_minutes,
	_king_moving_hill_settings_4_minutes,
	_king_moving_hill_settings_5_minutes,

	k_king_moving_hill_settings,
	k_king_moving_hill_settings_default = _king_moving_hill_settings_30_seconds
};

enum e_king_moving_hill_order_settings
{
	_king_moving_hill_order_settings_random = 0,
	_king_moving_hill_order_settings_sequence,

	k_king_moving_hill_order_settings,
	k_king_moving_hill_order_settings_default = _king_moving_hill_order_settings_sequence
};