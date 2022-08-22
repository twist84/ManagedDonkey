#pragma once

enum e_territories_variant_flags
{
	_territories_variant_flags_one_sided = 0,
	_territories_variant_flags_lock_after_first_capture,

	k_territories_variant_flags
};

enum e_territories_respawn_on_capture_settings
{
	_territories_respawn_on_capture_settings_disabled,
	_territories_respawn_on_capture_settings_friendly,
	_territories_respawn_on_capture_settings_enemy,
	_territories_respawn_on_capture_settings_all,

	k_territories_respawn_on_capture_settings,
	k_territories_respawn_on_capture_settings_default = _territories_respawn_on_capture_settings_disabled
};

enum e_territories_capture_time_settings
{
	_territories_capture_time_settings_instant = 0,
	_territories_capture_time_settings_5_seconds = 5,
	_territories_capture_time_settings_10_seconds = 10,
	_territories_capture_time_settings_15_seconds = 15,
	_territories_capture_time_settings_20_seconds = 20,
	_territories_capture_time_settings_30_seconds = 30,
	_territories_capture_time_settings_45_seconds = 45,
	_territories_capture_time_settings_1_minute = 60,
	_territories_capture_time_settings_2_minutes = 120,

	k_territories_capture_time_settings,
	k_territories_capture_time_settings_default = _territories_capture_time_settings_30_seconds
};

enum e_territories_sudden_death_settings
{
	_territories_sudden_death_settings_indefinite = -1,
	_territories_sudden_death_settings_off = 0,
	_territories_sudden_death_settings_15_seconds = 15,
	_territories_sudden_death_settings_30_seconds = 30,
	_territories_sudden_death_settings_1_minute = 60,
	_territories_sudden_death_settings_2_minutes = 120,
	_territories_sudden_death_settings_5_minutes = 300,

	k_territories_sudden_death_settings,
	k_territories_sudden_death_settings_default = _territories_sudden_death_settings_indefinite
};