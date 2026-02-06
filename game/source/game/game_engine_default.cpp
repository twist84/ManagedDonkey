#include "game/game_engine_default.hpp"

#include "game/game_engine.hpp"
#include "game/game_engine_util.hpp"
#include "game/game_engine_variant.hpp"
#include "main/levels.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

void c_game_engine::dump_player_trait_settings(const char* traits_name, const c_player_traits* traits, s_file_reference* file) const
{
	ASSERT(traits_name != nullptr);
	ASSERT(traits != nullptr);
	ASSERT(file != nullptr);

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
	const char* vehicle_usage = "**INVALID SETTING**";
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
	const char* active_camo = "**INVALID SETTING**";
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
	
	const char* waypoint = "**INVALID SETTING**";
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
	
	const char* aura = "**INVALID SETTING**";
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
	
	const char* forced_change_color = "**INVALID SETTING**";
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
	const char* motion_tracker = "**INVALID SETTING**";
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
	
	const char* motion_tracker_range = "**INVALID SETTING**";
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
	ASSERT(file != nullptr);

	const c_game_engine_base_variant* active_variant = current_game_variant()->get_active_variant();

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

	int16 team_scoring_method = get_team_scoring_method();

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

	int16 team_scoring_method = static_cast<int16>(packet->read_integer("team-scoring-method", 3));

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
	bswap_uns16_inplace(m_flags);
	bswap_uns16_inplace(m_team_scoring_method);
}

void c_game_engine_base_variant::set(const c_game_engine_base_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	set_name(variant->get_name());
	set_description(variant->get_description());
	m_miscellaneous_options.set(variant->get_miscellaneous_options(), force);
	m_respawn_options.set(variant->get_respawn_options(), force);
	m_social_options.set(variant->get_social_options(), force);
	m_map_override_options.set(variant->get_map_override_options(), force);
	set_team_scoring_method(variant->get_team_scoring_method());
}

//void c_game_engine_base_variant::set(const s_game_engine_base_variant_definition* definition, bool force)
//{
//}

s_saved_game_item_metadata* c_game_engine_base_variant::get_metadata()
{
	return &m_metadata;
}

const s_saved_game_item_metadata* c_game_engine_base_variant::get_metadata() const
{
	return &m_metadata;
}

void c_game_engine_base_variant::get_game_engine_name(c_static_wchar_string<1024>* out_game_engine_name) const
{
	game_engine_get_multiplayer_string(get_game_engine_name_string_id(), out_game_engine_name);
}

void c_game_engine_base_variant::get_game_engine_description(c_static_wchar_string<1024>* game_engine_description) const
{
	game_engine_get_multiplayer_string(get_game_engine_default_description_string_id(), game_engine_description);
}

const char* c_game_engine_base_variant::get_name() const
{
	return m_name;
}

void c_game_engine_base_variant::set_name(const char* name)
{
	csstrnzcpy(m_name, name, 32);
}

const char* c_game_engine_base_variant::get_description() const
{
	return m_metadata.description;
}

void c_game_engine_base_variant::set_description(const char* description)
{
	csstrnzcpy(m_metadata.description, description, 128);
}

c_game_engine_miscellaneous_options* c_game_engine_base_variant::get_miscellaneous_options_writeable()
{
	return &m_miscellaneous_options;
}

const c_game_engine_miscellaneous_options* c_game_engine_base_variant::get_miscellaneous_options() const
{
	return &m_miscellaneous_options;
}

c_game_engine_respawn_options* c_game_engine_base_variant::get_respawn_options_writeable()
{
	return &m_respawn_options;
}

const c_game_engine_respawn_options* c_game_engine_base_variant::get_respawn_options() const
{
	return &m_respawn_options;
}

c_game_engine_social_options* c_game_engine_base_variant::get_social_options_writeable()
{
	return &m_social_options;
}

const c_game_engine_social_options* c_game_engine_base_variant::get_social_options() const
{
	return &m_social_options;
}

c_game_engine_map_override_options* c_game_engine_base_variant::get_map_override_options_writeable()
{
	return &m_map_override_options;
}

const c_game_engine_map_override_options* c_game_engine_base_variant::get_map_override_options() const
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

int16 c_game_engine_base_variant::get_team_scoring_method() const
{
	return m_team_scoring_method;
}

void c_game_engine_base_variant::set_team_scoring_method(int16 team_scoring_method)
{
	m_team_scoring_method = team_scoring_method;
}

//int32 c_game_engine::get_type() const
//{
//	//return DECLFUNC(0x00749ED0, int32, __thiscall, const c_game_engine*)(this);
//
//	return 0;
//}
//
//int32 c_game_engine::get_score_to_win_round() const
//{
//	//return DECLFUNC(0x00749E90, int32, __thiscall, const c_game_engine*)(this);
//
//	if (current_game_variant())
//		return current_game_variant()->get_active_variant()->get_score_to_win_round();
//
//	return 0;
//}
//
//int32 c_game_engine::get_score_to_win_round_early() const
//{
//	//return DECLFUNC(0x00749E50, int32, __thiscall, const c_game_engine*)(this);
//
//	if (current_game_variant())
//		return current_game_variant()->get_active_variant()->get_score_to_win_round_early();
//
//	return 0;
//}
//
//void c_game_engine::recompute_team_score(e_game_team game_team, int32 a1, e_team_scoring_method team_scoring_method) const
//{
//	DECLFUNC(0x0074A450, void, __thiscall, const c_game_engine*, e_game_team, int32, e_team_scoring_method)(this, game_team, a1, team_scoring_method);
//}
//
//void c_game_engine::get_score_string(int32 score, c_static_wchar_string<256>* score_string) const
//{
//	//DECLFUNC(0x00749E70, void, __thiscall, const c_game_engine*, int32, c_static_wchar_string<256>*)(this, score, score_string);
//
//	score_string->print(L"%d", score);
//}
//
//void c_game_engine::get_hud_interface_state(int32 a1, game_engine_interface_state* hud_interface_state) const
//{
//	//DECLFUNC(0x00749DB0, void, __thiscall, const c_game_engine*, int32, game_engine_interface_state*)(this, a1, hud_interface_state);
//}
//
//bool c_game_engine::initialize_for_new_map() const
//{
//	//return DECLFUNC(0x00749F00, bool, __thiscall, const c_game_engine*)(this);
//
//	return true;
//}
//
//void c_game_engine::dispose_from_old_map() const
//{
//	//DECLFUNC(0x00749BA0, void, __thiscall, const c_game_engine*)(this);
//}
//
//bool c_game_engine::initialize_for_new_round() const
//{
//	//return DECLFUNC(0x00749F10, bool, __thiscall, const c_game_engine*)(this);
//
//	return true;
//}
//
//void c_game_engine::stats_reset_for_round_switch() const
//{
//	//DECLFUNC(0x0074A670, void, __thiscall, const c_game_engine*)(this);
//}
//
//bool c_game_engine::validate_team_designator_for_new_map(e_multiplayer_team_designator team_designator) const
//{
//	//return DECLFUNC(0x0074A7A0, bool, __thiscall, const c_game_engine*, e_multiplayer_team_designator)(this, team_designator);
//
//	s_level_datum multiplayer_level{};
//	return !levels_try_and_get_multiplayer_map(global_scenario_get()->map_id, &multiplayer_level) || team_designator < multiplayer_level.engine_maximum_teams[get_type()];
//}
//
//void c_game_engine::player_added(int32 player_index) const
//{
//	//DECLFUNC(0x00749FB0, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::player_activated(int32 player_index) const
//{
//	//DECLFUNC(0x00749FA0, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::player_left(int32 player_index) const
//{
//	//DECLFUNC(0x0074A1D0, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::player_rejoined(int32 player_index) const
//{
//	//DECLFUNC(0x0074A1F0, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::player_changed_indices(int32 player_index_a, int32 player_index_b) const
//{
//	//DECLFUNC(0x00749FD0, void, __thiscall, const c_game_engine*, int32, int32)(this, player_index_a, player_index_b);
//}
//
//void c_game_engine::player_changed_teams(int32 player_index) const
//{
//	//DECLFUNC(0x00749FE0, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::player_about_to_spawn(int32 player_index) const
//{
//	//DECLFUNC(0x00749F90, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::player_just_spawned(int32 player_index) const
//{
//	//DECLFUNC(0x0074A010, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::game_ending() const
//{
//	//DECLFUNC(0x00749D80, void, __thiscall, const c_game_engine*)(this);
//}
//
//void c_game_engine::game_starting() const
//{
//	//DECLFUNC(0x00749D90, void, __thiscall, const c_game_engine*)(this);
//}
//
//void c_game_engine::render(int32 user_index) const
//{
//	//DECLFUNC(0x0074A540, void, __thiscall, const c_game_engine*, int32)(this, user_index);
//}
//
//void c_game_engine::render_debug(int32 user_index) const
//{
//	//DECLFUNC(0x0074A550, void, __thiscall, const c_game_engine*, int32)(this, user_index);
//}
//
//void c_game_engine::submit_nav_points(int32 user_index, int32 player_index) const
//{
//	DECLFUNC(0x0074A680, void, __thiscall, const c_game_engine*, int32, int32)(this, user_index, player_index);
//}
//
//bool c_game_engine::build_player_nav_point(int32 a1, int32 a2, int32 a3, bool a4, s_chud_navpoint* chud_navpoint) const
//{
//	return DECLFUNC(0x0074A680, bool, __thiscall, const c_game_engine*, int32, int32, int32, bool, s_chud_navpoint*)(this, a1, a2, a3, a4, chud_navpoint);
//}
//
//bool c_game_engine::should_draw_nav_point(int32 user_index, int32 player_index) const
//{
//	return DECLFUNC(0x0074A5B0, bool, __thiscall, const c_game_engine*, int32, int32)(this, user_index, player_index);
//}
//
//void c_game_engine::update() const
//{
//	//DECLFUNC(0x0074A790, void, __thiscall, const c_game_engine*)(this);
//}
//
//void c_game_engine::player_update(int32 player_index) const
//{
//	DECLFUNC(0x0074A200, void, __thiscall, const c_game_engine*, int32)(this, player_index);
//}
//
//void c_game_engine::apply_baseline_traits_for_player(int32 player_index, c_player_traits* player_traits) const
//{
//	//DECLFUNC(0x00748940, void, __thiscall, const c_game_engine*, int32, c_player_traits*)(this, player_index, player_traits);
//
//	player_traits->set(current_game_variant()->get_active_variant()->get_map_override_options()->get_base_player_traits(), false);
//}
//
//void c_game_engine::apply_game_engine_traits_for_player(int32 player_index, c_player_traits* player_traits) const
//{
//	//DECLFUNC(0x00748960, void, __thiscall, const c_game_engine*, int32, c_player_traits*)(this, player_index, player_traits);
//}
//
//void c_game_engine::assemble_spawn_influencers_for_player(int32 a1, s_netgame_goal_influencer* influencer, int32* a3) const
//{
//	//DECLFUNC(0x00748F60, void, __thiscall, const c_game_engine*, int32, s_netgame_goal_influencer*, int32*)(this, a1, influencer, a3);
//
//	*a3 = 0;
//}
//
//int32 c_game_engine::compare_players(int32 player_index_a, int32 player_index_b) const
//{
//	//return DECLFUNC(0x00749B40, int32, __thiscall, const c_game_engine*, int32, int32)(this, player_index_a, player_index_b);
//
//	return 0;
//}
//
//int32 c_game_engine::compare_teams(e_game_team team_a, e_game_team team_b) const
//{
//	//return DECLFUNC(0x00749B50, int32, __thiscall, const c_game_engine*, int32, int32)(this, team_a, team_b);
//
//	return 0;
//}
//
//bool c_game_engine::allow_weapon_pickup(int32 player_index, int32 weapon_index) const
//{
//	//return DECLFUNC(0x00748900, bool, __thiscall, const c_game_engine*, int32, int32)(this, player_index, weapon_index);
//
//	bool result = game_engine_in_round();
//	if (result)
//		result = player_data[player_index].multiplayer.player_traits.get_weapons_traits()->get_weapon_pickup_allowed();
//	return result;
//}
//
//bool c_game_engine::should_auto_pickup_weapon(int32 player_index, int32 weapon_index) const
//{
//	//return DECLFUNC(0x0074A5A0, bool, __thiscall, const c_game_engine*, int32, int32)(this, player_index, weapon_index);
//
//	return false;
//}
//
//void c_game_engine::player_nearby_multiplayer_weapon(int32 a1, int32 a2) const
//{
//	//DECLFUNC(0x0074A1E0, void, __thiscall, const c_game_engine*, int32, int32)(this, a1, a2);
//}
//
//int32 c_game_engine::object_get_emblem_player(int32 object_index) const
//{
//	//return DECLFUNC(0x00749F70, int32, __thiscall, const c_game_engine*, int32)(this, object_index);
//
//	return NONE;
//}
//
//real32 c_game_engine::compute_object_function(int32 object_index, int32 name) const
//{
//	//return DECLFUNC(0x00749B60, real32, __thiscall, const c_game_engine*, int32, int32)(this, object_index, name);
//
//	return 0.0f;
//}
//
//void c_game_engine::multiplayer_weapon_register(int32 weapon_index) const
//{
//	//DECLFUNC(0x00749F60, void, __thiscall, const c_game_engine*, int32)(this, weapon_index);
//}
//
//void c_game_engine::multiplayer_weapon_deregister(int32 weapon_index) const
//{
//	//DECLFUNC(0x00749F30, void, __thiscall, const c_game_engine*, int32)(this, weapon_index);
//}
//
//void c_game_engine::multiplayer_weapon_picked_up(int32 weapon_index, int32 unit_index) const
//{
//	//DECLFUNC(0x00749F50, void, __thiscall, const c_game_engine*, int32, int32)(this, weapon_index, unit_index);
//}
//
//void c_game_engine::multiplayer_weapon_dropped(int32 weapon_index, int32 unit_index) const
//{
//	//DECLFUNC(0x00749F40, void, __thiscall, const c_game_engine*, int32, int32)(this, weapon_index, unit_index);
//}
//
//void c_game_engine::handle_deleted_object(int32 object_index) const
//{
//	//DECLFUNC(0x00749EF0, void, __thiscall, const c_game_engine*, int32)(this, object_index);
//}

