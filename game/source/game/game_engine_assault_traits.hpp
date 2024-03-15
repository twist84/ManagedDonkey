#pragma once

enum e_assault_variant_flags
{
	_assault_variant_flags_reset_bomb_on_disarm = 0,
	_assault_variant_flags_siege_mode,

	k_assault_variant_flags
};

enum e_assault_respawn_settings
{
	_assault_respawn_settings_disabled = 0,
	_assault_respawn_settings_on_friendly_detonation,
	_assault_respawn_settings_on_enemy_detonation,
	_assault_respawn_settings_on_any_detonation,

	k_assault_respawn_settings,
	k_assault_respawn_settings_default = _assault_respawn_settings_disabled
};

enum e_assault_game_type_settings
{
	_assault_game_type_settings_multi_bomb = 0,
	_assault_game_type_settings_one_bomb,
	_assault_game_type_settings_neutral_bomb,

	k_assault_game_type_settings,
	k_assault_game_type_settings_default = _assault_game_type_settings_multi_bomb
};

enum e_assault_enemy_bomb_waypoint_settings
{
	_assault_enemy_bomb_waypoint_settings_never = 0,
	_assault_enemy_bomb_waypoint_settings_when_not_carried_by_enemy,
	_assault_enemy_bomb_waypoint_settings_when_armed,
	_assault_enemy_bomb_waypoint_settings_alway,

	k_assault_enemy_bomb_waypoint_settings,
	k_assault_enemy_bomb_waypoint_settings_default = _assault_enemy_bomb_waypoint_settings_when_not_carried_by_enemy
};

enum e_assault_variant_sudden_death_time
{
	_assault_variant_sudden_death_time_infinite = -1,
	_assault_variant_sudden_death_time_off = 0,
	_assault_variant_sudden_death_time_15_seconds = 15,
	_assault_variant_sudden_death_time_1_minute = 60,
	_assault_variant_sudden_death_time_2_minute = 120,
	_assault_variant_sudden_death_time_5_minute = 300,

	k_assault_variant_sudden_death_time,
	k_assault_variant_sudden_death_time_default = _assault_variant_sudden_death_time_off
};

