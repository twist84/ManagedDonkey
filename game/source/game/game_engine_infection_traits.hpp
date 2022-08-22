#pragma once

enum e_infection_variant_flags
{
	_infection_variant_flags_respawn_on_haven_move = 0,

	k_infection_variant_flags
};

enum e_infection_safe_havens_settings
{
	_infection_safe_havens_off = 0,
	_infection_safe_havens_sequential,
	_infection_safe_havens_random,

	k_infection_safe_havens_settings,
	k_infection_safe_havens_settings_default = _infection_safe_havens_off
};

enum e_infection_next_zombie_settings
{
	_infection_next_zombie_winner = 0,
	_infection_next_zombie_chump,
	_infection_next_zombie_unchanged,
	_infection_next_zombie_random,

	k_infection_next_zombie_settings,
	k_infection_next_zombie_settings_default = _infection_next_zombie_winner
};

enum e_infection_initial_zombie_count_settings
{
	_infection_initial_zombie_count_25_percent = 0,
	_infection_initial_zombie_count_50_percent,
	_infection_initial_zombie_count_1,
	_infection_initial_zombie_count_2,
	_infection_initial_zombie_count_3,
	_infection_initial_zombie_count_4,
	_infection_initial_zombie_count_5,
	_infection_initial_zombie_count_6,
	_infection_initial_zombie_count_7,
	_infection_initial_zombie_count_8,
	_infection_initial_zombie_count_9,
	_infection_initial_zombie_count_10,
	_infection_initial_zombie_count_11,
	_infection_initial_zombie_count_12,
	_infection_initial_zombie_count_13,
	_infection_initial_zombie_count_14,
	_infection_initial_zombie_count_15,

	k_infection_initial_zombie_count_settings,
	k_infection_initial_zombie_count_settings_default = _infection_initial_zombie_count_50_percent
};