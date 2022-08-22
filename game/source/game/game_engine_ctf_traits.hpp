#pragma once

enum e_ctf_variant_flags
{
	_ctf_variant_flags_flag_at_home_to_score_flag = 0,

	k_ctf_variant_flags
};

enum e_ctf_home_flag_waypoint_settings
{
	_ctf_home_flag_waypoint_settings_never = 0,
	_ctf_home_flag_waypoint_settings_when_not_carried_by_enemy,
	_ctf_home_flag_waypoint_settings_away_from_home,
	_ctf_home_flag_waypoint_settings_always,

	k_ctf_home_flag_waypoint_settings,
	k_ctf_home_flag_waypoint_settings_default = _ctf_home_flag_waypoint_settings_when_not_carried_by_enemy
};

enum e_ctf_game_type_settings
{
	_ctf_game_type_settings_multi_flag = 0,
	_ctf_game_type_settings_single_flag,
	_ctf_game_type_settings_neutral_flag,

	k_ctf_game_type_settings,
	k_ctf_game_type_settings_default = _ctf_game_type_settings_multi_flag
};

enum e_ctf_respawn_settings
{
	_ctf_respawn_settings_normal = 0,
	_ctf_respawn_settings_on_friendly_capture,
	_ctf_respawn_settings_on_enemy_capture,
	_ctf_respawn_settings_on_any_capture,

	k_ctf_respawn_settings,
	k_ctf_respawn_settings_default = _ctf_respawn_settings_normal
};

enum e_ctf_touch_return_settings
{
	_ctf_touch_return_settings_off = -1,
	_ctf_touch_return_settings_instant = 0,
	_ctf_touch_return_settings_5_seconds = 5,
	_ctf_touch_return_settings_10_seconds = 10,
	_ctf_touch_return_settings_15_seconds = 15,
	_ctf_touch_return_settings_20_seconds = 20,
	_ctf_touch_return_settings_30_seconds = 30,
	_ctf_touch_return_settings_45_seconds = 45,
	_ctf_touch_return_settings_1_minute = 60,

	k_ctf_touch_return_settings,
	k_ctf_touch_return_settings_default = _ctf_touch_return_settings_5_seconds
};

enum e_ctf_sudden_death_time
{
	_ctf_sudden_death_time_infinite = -1,
	_ctf_sudden_death_time_off = 0,
	_ctf_sudden_death_time_15_seconds = 15,
	_ctf_sudden_death_time_1_minute = 60,
	_ctf_sudden_death_time_2_minute = 120,
	_ctf_sudden_death_time_5_minute = 300,

	k_ctf_sudden_death_times,
	k_ctf_sudden_death_times_default = _ctf_sudden_death_time_off
};