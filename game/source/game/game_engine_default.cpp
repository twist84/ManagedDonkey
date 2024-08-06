#include "game/game_engine_default.hpp"

#include "game/game_engine.hpp"
#include "game/game_engine_util.hpp"
#include "game/game_engine_variant.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

void c_game_engine::dump_player_trait_settings(char const* traits_name, c_player_traits const* traits, s_file_reference* file) const
{
	ASSERT(traits_name != NULL);
	ASSERT(traits != NULL);
	ASSERT(file != NULL);

	file_printf(file, "---------- '%s' player traits:\r\n", traits_name);

	// SHIELD VITALITY TRAITS
	file_printf(file, "dmg resistance= %g\r\n", traits->get_shield_vitality_traits()->get_damage_resistance_percentage());
	file_printf(file, "shield recharge rate= %g\r\n", traits->get_shield_vitality_traits()->get_shield_recharge_rate_percentage());
	file_printf(file, "vampirism= %g\r\n", traits->get_shield_vitality_traits()->get_vampirism_percentage());
	file_printf(file, "headshot immunity= %s\r\n", traits->get_shield_vitality_traits()->get_headshot_immunity() ? "ENABLED" : "DISABLED");
	file_printf(file, "shield multiplier= %dX\r\n", traits->get_shield_vitality_traits()->get_shield_multiplier());

	// WEAPONS TRAITS
	file_printf(file, "damage modifier= %g\r\n", traits->get_weapons_traits()->get_damage_modifier_percentage());
	file_printf(file, "initial primary weapon absolute index= %d\r\n", traits->get_weapons_traits()->get_initial_primary_weapon_absolute_index());
	file_printf(file, "initial secondary weapon absolute index= %d\r\n", traits->get_weapons_traits()->get_initial_secondary_weapon_absolute_index());
	file_printf(file, "initial grenade count setting= %d\r\n", traits->get_weapons_traits()->get_initial_grenade_count());
	file_printf(file, "recharging grenades= %s\r\n", traits->get_weapons_traits()->get_recharging_grenades() ? "ENABLED" : "DISABLED");
	file_printf(file, "infinite ammo= %s\r\n", traits->get_weapons_traits()->get_infinite_ammo_setting() ? "ENABLED" : "DISABLED");
	file_printf(file, "weapon pickup= %s\r\n", traits->get_weapons_traits()->get_weapon_pickup_allowed() ? "ENABLED" : "DISABLED");
	
	// MOVEMENT TRAITS
	char const* vehicle_usage = "**INVALID SETTING**";
	switch (traits->get_movement_traits()->get_vehicle_usage_setting())
	{
	case _vehicle_usage_setting_unchanged:
		vehicle_usage = "UNCHANGED";
		break;
	case _vehicle_usage_setting_none:
		vehicle_usage = "NONE";
		break;
	
	case _vehicle_usage_setting_passenger_only:
		vehicle_usage = "PASSENGER ONLY";
		break;
	
	case _vehicle_usage_setting_full:
		vehicle_usage = "FULL";
		break;
	}
	
	file_printf(file, "speed= %g\r\n", traits->get_movement_traits()->get_speed());
	file_printf(file, "gravity= %g\r\n", traits->get_movement_traits()->get_gravity());
	file_printf(file, "vehicle use= %s\r\n", vehicle_usage);
	
	// APPEARANCE TRAITS
	char const* active_camo = "**INVALID SETTING**";
	switch (traits->get_appearance_traits()->get_active_camo_setting())
	{
	case _active_camo_setting_unchanged:
		active_camo = "UNCHANGED";
		break;
	case _active_camo_setting_off:
		active_camo = "OFF";
		break;
	case _active_camo_setting_poor:
		active_camo = "POOR";
		break;
	case _active_camo_setting_good:
		active_camo = "GOOD";
		break;
	case _active_camo_setting_invisible:
		active_camo = "INVISIBLE";
		break;
	}
	
	char const* waypoint = "**INVALID SETTING**";
	switch (traits->get_appearance_traits()->get_waypoint_setting())
	{
	case _waypoint_setting_unchanged:
		waypoint = "UNCHANGED";
		break;
	case _waypoint_setting_off:
		waypoint = "OFF";
		break;
	case _waypoint_setting_allies:
		waypoint = "ALLIES";
		break;
	case _waypoint_setting_all:
		waypoint = "ALL";
		break;
	}
	
	char const* aura = "**INVALID SETTING**";
	switch (traits->get_appearance_traits()->get_aura_setting())
	{
	case _aura_setting_unchanged:
		aura = "UNCHANGED";
		break;
	case _aura_setting_off:
		aura = "OFF";
		break;
	case _aura_setting_team_color:
		aura = "TEAM COLOR";
		break;
	case _aura_setting_black:
		aura = "BLACK";
		break;
	case _aura_setting_white:
		aura = "WHITE";
		break;
	}
	
	char const* forced_change_color = "**INVALID SETTING**";
	switch (traits->get_appearance_traits()->get_forced_change_color_setting())
	{
	case _forced_change_color_setting_unchanged:
		forced_change_color = "UNCHANGED";
		break;
	case _forced_change_color_setting_off:
		forced_change_color = "OFF";
		break;
	case _forced_change_color_setting_mp_team_red:
		forced_change_color = "RED";
		break;
	case _forced_change_color_setting_mp_team_blue:
		forced_change_color = "BLUE";
		break;
	case _forced_change_color_setting_mp_team_green:
		forced_change_color = "GREEN";
		break;
	case _forced_change_color_setting_mp_team_yellow:
		forced_change_color = "YELLOW";
		break;
	case _forced_change_color_setting_mp_team_purple:
		forced_change_color = "PURPLE";
		break;
	case _forced_change_color_setting_mp_team_orange:
		forced_change_color = "ORANGE";
		break;
	case _forced_change_color_setting_mp_team_brown:
		forced_change_color = "BROWN";
		break;
	case _forced_change_color_setting_mp_team_grey:
		forced_change_color = "GREY";
		break;
	case _forced_change_color_setting_extra1:
		forced_change_color = "XTRA 1";
		break;
	case _forced_change_color_setting_extra2:
		forced_change_color = "XTRA 2";
		break;
	case _forced_change_color_setting_extra3:
		forced_change_color = "XTRA 3";
		break;
	case _forced_change_color_setting_extra4:
		forced_change_color = "XTRA 4";
		break;
	}
	
	file_printf(file, "active camo= %s\r\n", active_camo);
	file_printf(file, "waypoint= %s\r\n", waypoint);
	file_printf(file, "aura= %s\r\n", aura);
	file_printf(file, "forced change color= %s\r\n", forced_change_color);
	
	// SENSORS TRAITS
	char const* motion_tracker = "**INVALID SETTING**";
	switch (traits->get_sensor_traits()->get_motion_tracker_setting())
	{
	case _motion_tracker_setting_unchanged :
		motion_tracker = "UNCHANGED";
		break;
	case _motion_tracker_setting_off:
		motion_tracker = "OFF";
		break;
	case _motion_tracker_setting_moving_friendly_bipeds_moving_neutral_vehicles:
		motion_tracker = "FRIENDLY ONLY";
		break;
	case _motion_tracker_setting_moving_bipeds_moving_vehicles:
		motion_tracker = "NORMAL";
		break;
	case _motion_tracker_setting_all_bipeds_moving_vehicles:
		motion_tracker = "ALWAYS";
		break;
	}
	
	char const* motion_tracker_range = "**INVALID SETTING**";
	switch (traits->get_sensor_traits()->get_motion_tracker_range_setting())
	{
	case _motion_tracker_range_setting_unchanged :
		motion_tracker_range = "UNCHANGED";
		break;
	case _motion_tracker_range_setting_10_meters:
		motion_tracker_range = "10m";
		break;
	case _motion_tracker_range_setting_15_meters:
		motion_tracker_range = "15m";
		break;
	case _motion_tracker_range_setting_25_meters:
		motion_tracker_range = "25m";
		break;
	case _motion_tracker_range_setting_50_meters:
		motion_tracker_range = "50m";
		break;
	case _motion_tracker_range_setting_75_meters:
		motion_tracker_range = "75m";
		break;
	case _motion_tracker_range_setting_100_meters:
		motion_tracker_range = "100m";
		break;
	case _motion_tracker_range_setting_150_meters:
		motion_tracker_range = "150m";
		break;
	}
	
	file_printf(file, "motion tracker= %s\r\n", motion_tracker);
	file_printf(file, "motion tracker range= %s\r\n", motion_tracker_range);

	file_printf(file, "---------- end player traits\r\n");
}

void c_game_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine_base_variant const* active_variant = current_game_variant()->get_active_variant();

	file_printf(file, "---------- BASE variant settings\r\n");

	file_printf(file, "---------- name= '%s'\r\n", active_variant->get_name());
	file_printf(file, "---------- description= '%s'\r\n", active_variant->get_description());

	// MISCELLANEOUS OPTIONS
	file_printf(file, "teams= %s\r\n", active_variant->get_miscellaneous_options()->get_teams_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "round reset players= %s\r\n", active_variant->get_miscellaneous_options()->get_round_reset_players() ? "ENABLED" : "DISABLED");
	file_printf(file, "round reset map= %s\r\n", active_variant->get_miscellaneous_options()->get_round_reset_map() ? "ENABLED" : "DISABLED");
	file_printf(file, "round time limit= %d minutes\r\n", active_variant->get_miscellaneous_options()->get_round_time_limit_minutes());
	file_printf(file, "round limit= %d\r\n", active_variant->get_miscellaneous_options()->get_round_limit());
	file_printf(file, "early victory win count= %d\r\n", active_variant->get_miscellaneous_options()->get_early_victory_win_count());

	// RESPAWN OPTIONS
	file_printf(file, "inherit respawn time= %s\r\n", active_variant->get_respawn_options()->get_inherit_respawn_time_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "respawn with teammate= %s\r\n", active_variant->get_respawn_options()->get_respawn_with_teammate_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "respawn at location= %s\r\n", active_variant->get_respawn_options()->get_respawn_at_location_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "respawn on kills= %s\r\n", active_variant->get_respawn_options()->get_respawn_on_kills_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "lives per round= %d\r\n", active_variant->get_respawn_options()->get_lives_per_round());
	file_printf(file, "team lives per round= %d\r\n", active_variant->get_respawn_options()->get_team_lives_per_round());
	file_printf(file, "respawn time= %d seconds\r\n", active_variant->get_respawn_options()->get_respawn_time_seconds());
	file_printf(file, "suicide penalty= %d seconds\r\n", active_variant->get_respawn_options()->get_suicide_penalty_seconds());
	file_printf(file, "betrayal penalty= %d seconds\r\n", active_variant->get_respawn_options()->get_betrayal_penalty_seconds());
	file_printf(file, "respawn growth= %d seconds\r\n", active_variant->get_respawn_options()->get_respawn_growth_seconds());
	file_printf(file, "respawn traits duration= %d seconds\r\n", active_variant->get_respawn_options()->get_respawn_player_traits_duration_seconds());
	dump_player_trait_settings("respawn", active_variant->get_respawn_options()->get_respawn_player_traits(), file);
	
	// SOCIAL OPTIONS
	file_printf(file, "observers= %s\r\n", "DISABLED"); // active_variant->get_social_options()->get_observers_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "team changing= %d\r\n", active_variant->get_social_options()->get_team_changing_setting());
	file_printf(file, "friendly fire= %s\r\n", active_variant->get_social_options()->get_friendly_fire_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "betrayal booting= %s\r\n", active_variant->get_social_options()->get_betrayal_booting_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "enemy voice= %s\r\n", active_variant->get_social_options()->get_enemy_voice_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "open channel voice= %s\r\n", active_variant->get_social_options()->get_open_channel_voice_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "dead player voice= %s\r\n", active_variant->get_social_options()->get_dead_player_voice_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "spartans vs elites= %s\r\n", active_variant->get_social_options()->get_spartans_vs_elites_enabled() ? "ENABLED" : "DISABLED");
	
	// MAP-OVERRIDE OPTIONS
	file_printf(file, "grenades on map= %s\r\n", active_variant->get_map_override_options()->get_grenades_on_map_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "indestructible vehicles= %s\r\n", active_variant->get_map_override_options()->get_indestructible_vehicles_enabled() ? "ENABLED" : "DISABLED");
	dump_player_trait_settings("base", active_variant->get_map_override_options()->get_base_player_traits(), file);
	file_printf(file, "weapon set absolute index= %d\r\n", active_variant->get_map_override_options()->get_weapon_set_absolute_index());
	file_printf(file, "vehicle set absolute index= %d\r\n", active_variant->get_map_override_options()->get_vehicle_set_absolute_index());
	dump_player_trait_settings("red powerup", active_variant->get_map_override_options()->get_red_powerup_traits(), file);
	dump_player_trait_settings("blue powerup", active_variant->get_map_override_options()->get_blue_powerup_traits(), file);
	dump_player_trait_settings("yellow powerup", active_variant->get_map_override_options()->get_yellow_powerup_traits(), file);
	file_printf(file, "red powerup duration= %d seconds\r\n", active_variant->get_map_override_options()->get_red_powerup_duration_seconds());
	file_printf(file, "blue powerup duration= %d seconds\r\n", active_variant->get_map_override_options()->get_blue_powerup_duration_seconds());
	file_printf(file, "yellow powerup duration= %d seconds\r\n", active_variant->get_map_override_options()->get_yellow_powerup_duration_seconds());

	file_printf(file, "---------- END BASE variant settings\r\n");
}

void c_game_engine_base_variant::encode_to_mcc(c_bitstream* packet) const
{
	m_metadata.encode_to_mcc(packet);

	bool built_in = get_built_in();

	packet->write_bool("variant-built-in", built_in);

	get_miscellaneous_options()->encode_to_mcc(packet);
	get_respawn_options()->encode_to_mcc(packet);
	get_social_options()->encode_to_mcc(packet);
	get_map_override_options()->encode_to_mcc(packet);

	short team_scoring_method = get_team_scoring_method();

	packet->write_integer("team-scoring-method", team_scoring_method, 3);
}

void c_game_engine_base_variant::decode_from_mcc(c_bitstream* packet)
{
	initialize();
	m_metadata.decode_from_mcc(packet);

	bool built_in = packet->read_bool("variant-built-in");

	get_miscellaneous_options_writeable()->decode_from_mcc(packet);
	get_respawn_options_writeable()->decode_from_mcc(packet);
	get_social_options_writeable()->decode_from_mcc(packet);
	get_map_override_options_writeable()->decode_from_mcc(packet);

	short team_scoring_method = static_cast<short>(packet->read_integer("team-scoring-method", 3));

	set_built_in(built_in);

	set_team_scoring_method(team_scoring_method);
}

void c_game_engine_base_variant::byteswap()
{
	m_metadata.byteswap();
	m_miscellaneous_options.byteswap();
	m_respawn_options.byteswap();
	m_social_options.byteswap();
	m_map_override_options.byteswap();
	bswap_word_inplace(m_flags);
	bswap_word_inplace(m_team_scoring_method);
}

void c_game_engine_base_variant::set(c_game_engine_base_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	set_name(variant->get_name());
	set_description(variant->get_description());
	m_miscellaneous_options.set(variant->get_miscellaneous_options(), force);
	m_respawn_options.set(variant->get_respawn_options(), force);
	m_social_options.set(variant->get_social_options(), force);
	m_map_override_options.set(variant->get_map_override_options(), force);
	set_team_scoring_method(variant->get_team_scoring_method());
}

//void c_game_engine_base_variant::set(s_game_engine_base_variant_definition const* definition, bool force)
//{
//}

void c_game_engine_base_variant::get_game_engine_name(c_static_wchar_string<1024>* out_game_engine_name) const
{
	game_engine_get_multiplayer_string(get_game_engine_name_string_id(), out_game_engine_name);
}

void c_game_engine_base_variant::get_game_engine_description(c_static_wchar_string<1024>* game_engine_description) const
{
	game_engine_get_multiplayer_string(get_game_engine_default_description_string_id(), game_engine_description);
}

char const* c_game_engine_base_variant::get_name() const
{
	return m_name;
}

void c_game_engine_base_variant::set_name(char const* name)
{
	csstrnzcpy(m_name, name, 32);
}

char const* c_game_engine_base_variant::get_description() const
{
	return m_metadata.description;
}

void c_game_engine_base_variant::set_description(char const* description)
{
	csstrnzcpy(m_metadata.description, description, 128);
}

c_game_engine_miscellaneous_options* c_game_engine_base_variant::get_miscellaneous_options_writeable()
{
	return &m_miscellaneous_options;
}

c_game_engine_miscellaneous_options const* c_game_engine_base_variant::get_miscellaneous_options() const
{
	return &m_miscellaneous_options;
}

c_game_engine_respawn_options* c_game_engine_base_variant::get_respawn_options_writeable()
{
	return &m_respawn_options;
}

c_game_engine_respawn_options const* c_game_engine_base_variant::get_respawn_options() const
{
	return &m_respawn_options;
}

c_game_engine_social_options* c_game_engine_base_variant::get_social_options_writeable()
{
	return &m_social_options;
}

c_game_engine_social_options const* c_game_engine_base_variant::get_social_options() const
{
	return &m_social_options;
}

c_game_engine_map_override_options* c_game_engine_base_variant::get_map_override_options_writeable()
{
	return &m_map_override_options;
}

c_game_engine_map_override_options const* c_game_engine_base_variant::get_map_override_options() const
{
	return &m_map_override_options;
}

bool c_game_engine_base_variant::get_built_in() const
{
	return m_flags.test(_base_variant_flags_built_in);
}

void c_game_engine_base_variant::set_built_in(bool built_in)
{
	m_flags.set(_base_variant_flags_built_in, built_in);
}

short c_game_engine_base_variant::get_team_scoring_method() const
{
	return m_team_scoring_method;
}

void c_game_engine_base_variant::set_team_scoring_method(short team_scoring_method)
{
	m_team_scoring_method = team_scoring_method;
}

