#pragma once

enum e_vip_variant_flags
{
	_vip_variant_flags_single_vip = 0,
	_vip_variant_flags_destination_zones_enabled,
	_vip_variant_flags_end_round_on_vip_death,

	k_vip_variant_flags,
	k_vip_variant_flags_default
};

enum e_vip_selection_settings
{
	_vip_selection_settings_random,
	_vip_selection_settings_next_respawn,
	_vip_selection_settings_next_kill,
	_vip_selection_settings_unchanged,

	k_vip_selection_settings,
	k_vip_selection_settings_default = _vip_selection_settings_random
};

enum e_vip_zone_movement_settings
{
	_vip_zone_movement_settings_off,
	_vip_zone_movement_settings_10_seconds,
	_vip_zone_movement_settings_15_seconds,
	_vip_zone_movement_settings_30_seconds,
	_vip_zone_movement_settings_1_minute,
	_vip_zone_movement_settings_2_minutes,
	_vip_zone_movement_settings_3_minutes,
	_vip_zone_movement_settings_4_minutes,
	_vip_zone_movement_settings_5_minutes,
	_vip_zone_movement_settings_on_arrival,
	_vip_zone_movement_settings_on_switch,

	k_vip_zone_movement_settings,
	k_vip_zone_movement_settings_default = _vip_zone_movement_settings_off
};

enum e_vip_zone_order_settings
{
	_vip_zone_order_settings_random,
	_vip_zone_order_settings_sequence,

	k_vip_zone_order_settings,
	k_vip_zone_order_settings_default = _vip_zone_order_settings_sequence
};