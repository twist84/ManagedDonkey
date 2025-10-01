#include "shell/shell.hpp"

#include "cache/cache_files.hpp"
#include "cache/security_functions.hpp"
#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/runtime_state.hpp"
#include "input/input_windows.hpp"
#include "main/global_preferences.hpp"
#include "main/main.hpp"
#include "main/main_time.hpp"
#include "memory/module.hpp"
#include "networking/network_utilities.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer.hpp"
#include "saved_games/game_state.hpp"
#include "shell/shell_windows.hpp"
#include "sound/sound_manager.hpp"
#include "text/font_loading.hpp"

//#include <crtdbg.h>

REFERENCE_DECLARE(0x0199C000, bool, shell_application_paused);

//int __cdecl microsoft_crt_report_hook(int report_type, char* message, int* return_value)
//{
//	if (report_type == _CRT_ASSERT)
//	{
//		VASSERT(message);
//	}
//	else
//	{
//		event((e_event_level)(report_type != _CRT_WARN + _event_warning), "system:crt: %s", message);
//	}
//
//	return 1;
//}

bool __cdecl shell_application_is_paused()
{
	//return INVOKE(0x0042E310, shell_application_is_paused);

	return shell_application_paused;
}

void __cdecl shell_application_pause(bool pause)
{
	//INVOKE(0x0042E320, shell_application_pause, pause);

	if (shell_application_is_paused() == pause)
		return;

	shell_application_paused = pause;
	shell_screen_pause(pause);

	if (!pause)
		main_time_reset();
}

e_shell_application_type __cdecl shell_application_type()
{
	//return INVOKE(0x0042E350, shell_application_type);

	return _shell_application_game;
}

bool __cdecl shell_build_number_is_compatible(int32 build_number)
{
	return INVOKE(0x0042E360, shell_build_number_is_compatible, build_number);
}

bool __cdecl shell_build_string_is_compatible(const char* build_string)
{
	return INVOKE(0x0042E390, shell_build_string_is_compatible, build_string);
}

void __cdecl shell_dispose()
{
	//INVOKE(0x0042E410, shell_dispose);

	game_state_shell_dispose();
	sound_dispose();
	input_dispose();
	real_math_dispose();
	tag_files_close();
	font_dispose();
	security_dispose();
	async_dispose();
	network_remote_reporting_dispose();
	c_rasterizer::shell_dispose();
	shell_platform_dispose();
	runtime_state_shell_dispose();
	cseries_dispose();
}

const char* __cdecl shell_get_target()
{
	//return INVOKE(0x0042E470, shell_get_target);

	return "blam";
}

const char* __cdecl shell_get_target_variant()
{
	//return INVOKE(0x0042E480, shell_get_target_variant);

	return "cache";
}

void __cdecl shell_halt_on_pure_virtual_call()
{
	INVOKE(0x0042E4A0, shell_halt_on_pure_virtual_call);

	//VASSERT(0, "calling pure virtual function!");
}

void __cdecl shell_halt_with_message(const char* message)
{
	INVOKE(0x0042E4B0, shell_halt_with_message, message);

	//const wchar_t* spinner_states[] = { L"-", L"\\", L"||", L"/" };
	//while (true)
	//{
	//	c_static_wchar_string<256> status_message;
	//	ascii_string_to_wchar_string(message, status_message.get_buffer(), status_message.element_count, NULL);
	//	status_message.print(L"|n%s", spinner_states[(system_milliseconds() / 100) & 3]);
	//	main_loop_status_message(status_message.get_string());
	//}
}

_VCRTIMP _purecall_handler __cdecl set_purecall_handler(_purecall_handler _Handler)
{
	return INVOKE(0x00BED579, set_purecall_handler, _Handler);

	//return _set_purecall_handler(_Handler);
}

bool __cdecl shell_initialize(bool windowed)
{
	//return INVOKE(0x0042E540, shell_initialize, windowed);

	bool shell_initialized = false;
	set_purecall_handler(shell_halt_on_pure_virtual_call);
	//_CrtSetReportHook2(_CRT_RPTHOOK_INSTALL, microsoft_crt_report_hook);

	cseries_initialize();

	PROFILER(shell_initialize)
	{
		//errors_initialize();
		events_initialize();
		game_state_shell_gobble_first_physical_allocation();
		runtime_state_shell_initialize();
		if (shell_platform_initialize())
		{
			real_math_initialize();
			async_initialize();
			security_initialize();
			network_remote_reporting_initialize();
			global_preferences_initialize();
			font_initialize();
			tag_files_open();
			game_state_shell_initialize();
			c_rasterizer::shell_initialize(false, windowed);
			if (rasterizer_initialized())
			{
				input_initialize();
				sound_initialize();
				shell_initialized = true;
			}
			shell_platform_verify();
		}
	}
	return shell_initialized;
}

bool __cdecl sub_42E5C0()
{
	return INVOKE(0x0042E5C0, sub_42E5C0);
}

bool __cdecl sub_42E5D0() // disable rendering
{
	return INVOKE(0x0042E5D0, sub_42E5D0);
}

bool __cdecl sub_42E5E0() // disable audio
{
	return INVOKE(0x0042E5E0, sub_42E5E0);
}

bool __cdecl game_is_bot_client()
{
	return INVOKE(0x0042E5F0, game_is_bot_client);
}

bool __cdecl game_is_dedicated_server()
{
	return INVOKE(0x0042E600, game_is_dedicated_server);
}

bool __cdecl game_is_client()
{
	return INVOKE(0x0042E610, game_is_client);
}

bool __cdecl game_is_guerilla()
{
	return INVOKE(0x0042E620, game_is_guerilla);
}

bool __cdecl game_is_halo3()
{
	return INVOKE(0x0042E630, game_is_halo3);
}

bool __cdecl game_is_sapien()
{
	return INVOKE(0x0042E640, game_is_sapien);
}

bool __cdecl game_is_tool()
{
	return INVOKE(0x0042E650, game_is_tool);
}

e_shell_tool_type __cdecl shell_tool_type()
{
	//return INVOKE(0x0042E660, shell_tool_type);

	return _shell_tool_invalid;
}

const char* quality_setting_get_name(e_quality_setting quality_setting)
{
	if (quality_setting < _quality_setting_low || quality_setting >= k_quality_setting_count)
		return "<invalid 'quality_setting'>";

	const char* names[k_quality_setting_count]
	{
		"low",
		"medium",
		"high",
	};

	return names[quality_setting];
}

e_quality_setting quality_setting_from_string(const char* str)
{
	const char* names[k_quality_setting_count]
	{
		"low",
		"medium",
		"high",
	};

	e_quality_setting quality_setting = e_quality_setting(-1);
	for (int32 i = _quality_setting_low; i < k_quality_setting_count; i++)
	{
		if (csstricmp(str, names[i]) != 0)
			continue;

		quality_setting = e_quality_setting(i);
	}

	return quality_setting;
}

const char* subtitle_setting_get_name(e_subtitle_setting subtitle_setting)
{
	if (subtitle_setting < _subtitle_setting_automatic || subtitle_setting >= k_number_of_subtitle_settings)
		return "<invalid 'subtitle_setting'>";

	return global_subtitle_setting_names[subtitle_setting];
}

e_subtitle_setting subtitle_setting_from_string(const char* str)
{
	e_subtitle_setting subtitle_setting = e_subtitle_setting(-1);
	for (int32 i = _subtitle_setting_automatic; i < k_number_of_subtitle_settings; i++)
	{
		if (csstricmp(str, global_subtitle_setting_names[i]) != 0)
			continue;

		subtitle_setting = e_subtitle_setting(i);
	}

	return subtitle_setting;
}

const char* campaign_difficulty_level_get_name(e_campaign_difficulty_level difficulty)
{
	if (difficulty < _campaign_difficulty_level_easy || difficulty >= k_campaign_difficulty_levels_count)
		return "<invalid 'difficulty'>";

	return global_campaign_difficulty_level_names[difficulty];
}

e_campaign_difficulty_level campaign_difficulty_level_from_string(const char* str)
{
	e_campaign_difficulty_level difficulty = _campaign_difficulty_level_normal;
	for (int32 i = _campaign_difficulty_level_easy; i < k_campaign_difficulty_levels_count; i++)
	{
		if (csstricmp(str, global_campaign_difficulty_level_names[i]) != 0)
			continue;

		difficulty = e_campaign_difficulty_level(i);
	}

	return difficulty;
}

const char* network_session_mode_get_name(int32 session_mode)
{
	if (session_mode < _network_session_mode_none || session_mode >= k_network_session_mode_count)
		return "<invalid 'network_session_mode'>";

	return k_network_session_mode_names[session_mode];
}

const char* ui_game_mode_get_name(int32 ui_game_mode)
{
	if (ui_game_mode < _ui_game_mode_campaign || ui_game_mode >= k_gui_game_setup_mode_count)
		return "<invalid 'ui_game_mode'>";

	return k_ui_game_mode_names[ui_game_mode];
}

const char* gui_network_session_advertisement_mode_get_name(int32 advertisement_mode)
{
	if (advertisement_mode < _network_advertise_xbox_live_public || advertisement_mode >= k_network_session_advertisement_mode_count)
		return "<invalid 'ui_game_mode'>";

	return k_gui_network_session_advertisement_mode_names[advertisement_mode];
}

const char* game_variant_parameter_get_name(int32 parameter)
{
	if (parameter < _game_variant_base_miscellaneous_teams || parameter >= k_number_of_editable_game_variant_parameters)
		return "<invalid 'parameter'>";

	return k_game_variant_parameter_names[parameter];
}

const char* k_network_session_mode_names[k_network_session_mode_count]
{
	"none",
	"idle",
	"setup",
	"in-game",
	"end-game",
	"post-game",
	"matchmaking-start",
	"matchmaking-searching",
	"matchmaking-gathering",
	"matchmaking-slave",
	"matchmaking-disbanding",
	"matchmaking-arbitrating",
	"matchmaking-choosing-game"
};

const char* k_ui_game_mode_names[k_gui_game_setup_mode_count]
{
	"campaign",
	"matchmaking",
	"multiplayer",
	"map_editor",
	"theater",
	"survival"
};

const char* k_gui_network_session_advertisement_mode_names[k_network_session_advertisement_mode_count]
{
	"open_to_public",
	"open_to_friends",
	"invite_only",
	"system_link",
	"offline"
};

const char* k_game_variant_parameter_names[k_number_of_editable_game_variant_parameters]
{
	"game_misc_teams",
	"game_misc_unknown1",
	"game_misc_round_reset_map",
	"game_misc_round_time_limit",
	"game_misc_round_limit",
	"game_misc_early_win_count",

	"game_respawn_inherit_timer",
	"game_respawn_with_teammate",
	"game_respawn_at_location",
	"game_respawn_on_kill",
	"game_respawn_lives_per_round",
	"game_respawn_team_lives_per_round",
	"game_respawn_respawn_time",
	"game_respawn_suicide_penalty",
	"game_respawn_betrayal_penalty",
	"game_respawn_growth",
	"game_respawn_traits_duration",

	"respawn_shield_damage_resistance",
	"respawn_shield_multiplier",
	"respawn_shield_recharge_rate",
	"respawn_headshot_immunity",
	"respawn_vampirism",
	"respawn_damage_modifier_percentage",
	"respawn_initial_primary_weapon",
	"respawn_initial_secondary_weapon",
	"respawn_initial_grenade_count",
	"respawn_recharging_grenades",
	"respawn_infinite_ammo",
	"respawn_weapon_pickup",
	"respawn_walking_speed",
	"respawn_personal_gravity",
	"respawn_vehicle_use",
	"respawn_motion_tracker",
	"respawn_motion_tracker_range",
	"respawn_waypoint",
	"respawn_active_camo",
	"respawn_visual",
	"respawn_forced_change_colors",

	"game_social_observers",
	"game_social_team_changing",
	"game_social_friendly_fire",
	"game_social_betrayal_booting",
	"game_social_enemy_voice",
	"game_social_open_channel_voice",
	"game_social_dead_player_voice",

	"game_map_grenades",
	"game_map_indestructible_vehicles",

	"player_shield_damage_resistance",
	"player_shield_multiplier",
	"player_shield_recharge_rate",
	"player_headshot_immunity",
	"player_vampirism",
	"player_damage_modifier_percentage",
	"player_initial_primary_weapon",
	"player_initial_secondary_weapon",
	"player_initial_grenade_count",
	"player_recharging_grenades",
	"player_infinite_ammo",
	"player_weapon_pickup",
	"player_walking_speed",
	"player_personal_gravity",
	"player_vehicle_use",
	"player_motion_tracker",
	"player_motion_tracker_range",
	"player_waypoint",
	"player_active_camo",
	"player_visual",
	"player_forced_change_colors",

	"game_map_weapon_set",
	"game_map_vehicle_set",

	"red_powerup_shield_damage_resistance",
	"red_powerup_shield_multiplier",
	"red_powerup_shield_recharge_rate",
	"red_powerup_headshot_immunity",
	"red_powerup_vampirism",
	"red_powerup_damage_modifier_percentage",
	"red_powerup_initial_primary_weapon",
	"red_powerup_initial_secondary_weapon",
	"red_powerup_initial_grenade_count",
	"red_powerup_recharging_grenades",
	"red_powerup_infinite_ammo",
	"red_powerup_weapon_pickup",
	"red_powerup_walking_speed",
	"red_powerup_personal_gravity",
	"red_powerup_vehicle_use",
	"red_powerup_motion_tracker",
	"red_powerup_motion_tracker_range",
	"red_powerup_waypoint",
	"red_powerup_active_camo",
	"red_powerup_visual",
	"red_powerup_forced_change_colors",

	"blue_powerup_shield_damage_resistance",
	"blue_powerup_shield_multiplier",
	"blue_powerup_shield_recharge_rate",
	"blue_powerup_headshot_immunity",
	"blue_powerup_vampirism",
	"blue_powerup_damage_modifier_percentage",
	"blue_powerup_initial_primary_weapon",
	"blue_powerup_initial_secondary_weapon",
	"blue_powerup_initial_grenade_count",
	"blue_powerup_recharging_grenades",
	"blue_powerup_infinite_ammo",
	"blue_powerup_weapon_pickup",
	"blue_powerup_walking_speed",
	"blue_powerup_personal_gravity",
	"blue_powerup_vehicle_use",
	"blue_powerup_motion_tracker",
	"blue_powerup_motion_tracker_range",
	"blue_powerup_waypoint",
	"blue_powerup_active_camo",
	"blue_powerup_visual",
	"blue_powerup_forced_change_colors",

	"yellow_powerup_shield_damage_resistance",
	"yellow_powerup_shield_multiplier",
	"yellow_powerup_shield_recharge_rate",
	"yellow_powerup_headshot_immunity",
	"yellow_powerup_vampirism",
	"yellow_powerup_damage_modifier_percentage",
	"yellow_powerup_initial_primary_weapon",
	"yellow_powerup_initial_secondary_weapon",
	"yellow_powerup_initial_grenade_count",
	"yellow_powerup_recharging_grenades",
	"yellow_powerup_infinite_ammo",
	"yellow_powerup_weapon_pickup",
	"yellow_powerup_walking_speed",
	"yellow_powerup_personal_gravity",
	"yellow_powerup_vehicle_use",
	"yellow_powerup_motion_tracker",
	"yellow_powerup_motion_tracker_range",
	"yellow_powerup_waypoint",
	"yellow_powerup_active_camo",
	"yellow_powerup_visual",
	"yellow_powerup_forced_change_colors",

	"game_map_red_powerup_duration",
	"game_map_blue_powerup_duration",
	"game_map_yellow_powerup_duration",

	"slayer_team_scoring",
	"slayer_score_to_win",
	"slayer_kill_points",
	"slayer_assist_points",
	"slayer_death_points",
	"slayer_suicide_points",
	"slayer_betrayal_points",
	"slayer_leader_killed_points",
	"slayer_elimination_points",
	"slayer_assassination_points",
	"slayer_headshot_points",
	"slayer_melee_points",
	"slayer_sticky_points",
	"slayer_splatter_points",
	"slayer_killing_spree_points",

	"slayer_leader_shield_damage_resistance",
	"slayer_leader_shield_multiplier",
	"slayer_leader_shield_recharge_rate",
	"slayer_leader_headshot_immunity",
	"slayer_leader_vampirism",
	"slayer_leader_damage_modifier_percentage",
	"slayer_leader_initial_primary_weapon",
	"slayer_leader_initial_secondary_weapon",
	"slayer_leader_initial_grenade_count",
	"slayer_leader_recharging_grenades",
	"slayer_leader_infinite_ammo",
	"slayer_leader_weapon_pickup",
	"slayer_leader_walking_speed",
	"slayer_leader_personal_gravity",
	"slayer_leader_vehicle_use",
	"slayer_leader_motion_tracker",
	"slayer_leader_motion_tracker_range",
	"slayer_leader_waypoint",
	"slayer_leader_active_camo",
	"slayer_leader_visual",
	"slayer_leader_forced_change_colors",

	"oddball_auto_ball_pickup",
	"oddball_ball_effect",
	"oddball_team_scoring",
	"oddball_waypoints",
	"oddball_score_to_win",
	"oddball_carrying_points",
	"oddball_kill_points",
	"oddball_ball_kill_points",
	"oddball_carrier_kill_points",
	"oddball_ball_count",
	"oddball_ball_spawn_delay",
	"oddball_inactive_respawn_delay",

	"oddball_carrier_shield_damage_resistance",
	"oddball_carrier_shield_multiplier",
	"oddball_carrier_shield_recharge_rate",
	"oddball_carrier_headshot_immunity",
	"oddball_carrier_vampirism",
	"oddball_carrier_damage_modifier_percentage",
	"oddball_carrier_initial_primary_weapon",
	"oddball_carrier_initial_secondary_weapon",
	"oddball_carrier_initial_grenade_count",
	"oddball_carrier_recharging_grenades",
	"oddball_carrier_infinite_ammo",
	"oddball_carrier_weapon_pickup",
	"oddball_carrier_walking_speed",
	"oddball_carrier_personal_gravity",
	"oddball_carrier_vehicle_use",
	"oddball_carrier_motion_tracker",
	"oddball_carrier_motion_tracker_range",
	"oddball_carrier_waypoint",
	"oddball_carrier_active_camo",
	"oddball_carrier_visual",
	"oddball_carrier_forced_change_colors",

	"ctf_touch_return_timeout",
	"ctf_flag_at_home_to_score",
	"ctf_bomb_sticky_arming",
	"ctf_home_flag_waypoint",
	"ctf_enemy_bomb_waypoint",
	"ctf_game_type",
	"ctf_respawn",
	"ctf_score_to_win",
	"ctf_sudden_death_time",
	"ctf_flag_reset_time",
	"ctf_bomb_arming_time",
	"ctf_bomb_fuse_time",

	"ctf_carrier_shield_damage_resistance",
	"ctf_carrier_shield_multiplier",
	"ctf_carrier_shield_recharge_rate",
	"ctf_carrier_headshot_immunity",
	"ctf_carrier_vampirism",
	"ctf_carrier_damage_modifier_percentage",
	"ctf_carrier_initial_primary_weapon",
	"ctf_carrier_initial_secondary_weapon",
	"ctf_carrier_initial_grenade_count",
	"ctf_carrier_recharging_grenades",
	"ctf_carrier_infinite_ammo",
	"ctf_carrier_weapon_pickup",
	"ctf_carrier_walking_speed",
	"ctf_carrier_personal_gravity",
	"ctf_carrier_vehicle_use",
	"ctf_carrier_motion_tracker",
	"ctf_carrier_motion_tracker_range",
	"ctf_carrier_waypoint",
	"ctf_carrier_active_camo",
	"ctf_carrier_visual",
	"ctf_carrier_forced_change_colors",

	"infection_respawn_on_haven_move",
	"infection_safe_haven_setting",
	"infection_next_zombie",
	"infection_initial_zombie_count",
	"infection_safe_haven_movement_time",
	"infection_zombie_kill_points",
	"infection_points",
	"infection_safe_haven_arrive_points",
	"infection_suicide_points",
	"infection_betrayal_points",
	"infection_last_man_standing_bonus_points",

	"zombie_shield_damage_resistance",
	"zombie_shield_multiplier",
	"zombie_shield_recharge_rate",
	"zombie_headshot_immunity",
	"zombie_vampirism",
	"zombie_damage_modifier_percentage",
	"zombie_initial_primary_weapon",
	"zombie_initial_secondary_weapon",
	"zombie_initial_grenade_count",
	"zombie_recharging_grenades",
	"zombie_infinite_ammo",
	"zombie_weapon_pickup",
	"zombie_walking_speed",
	"zombie_personal_gravity",
	"zombie_vehicle_use",
	"zombie_motion_tracker",
	"zombie_motion_tracker_range",
	"zombie_waypoint",
	"zombie_active_camo",
	"zombie_visual",
	"zombie_forced_change_colors",

	"1st_zombie_shield_damage_resistance",
	"1st_zombie_shield_multiplier",
	"1st_zombie_shield_recharge_rate",
	"1st_zombie_headshot_immunity",
	"1st_zombie_vampirism",
	"1st_zombie_damage_modifier_percentage",
	"1st_zombie_initial_primary_weapon",
	"1st_zombie_initial_secondary_weapon",
	"1st_zombie_initial_grenade_count",
	"1st_zombie_recharging_grenades",
	"1st_zombie_infinite_ammo",
	"1st_zombie_weapon_pickup",
	"1st_zombie_walking_speed",
	"1st_zombie_personal_gravity",
	"1st_zombie_vehicle_use",
	"1st_zombie_motion_tracker",
	"1st_zombie_motion_tracker_range",
	"1st_zombie_waypoint",
	"1st_zombie_active_camo",
	"1st_zombie_visual",
	"1st_zombie_forced_change_colors",

	"haven_defender_shield_damage_resistance",
	"haven_defender_shield_multiplier",
	"haven_defender_shield_recharge_rate",
	"haven_defender_headshot_immunity",
	"haven_defender_vampirism",
	"haven_defender_damage_modifier_percentage",
	"haven_defender_initial_primary_weapon",
	"haven_defender_initial_secondary_weapon",
	"haven_defender_initial_grenade_count",
	"haven_defender_recharging_grenades",
	"haven_defender_infinite_ammo",
	"haven_defender_weapon_pickup",
	"haven_defender_walking_speed",
	"haven_defender_personal_gravity",
	"haven_defender_vehicle_use",
	"haven_defender_motion_tracker",
	"haven_defender_motion_tracker_range",
	"haven_defender_waypoint",
	"haven_defender_active_camo",
	"haven_defender_visual",
	"haven_defender_forced_change_colors",

	"last_human_shield_damage_resistance",
	"last_human_shield_multiplier",
	"last_human_shield_recharge_rate",
	"last_human_headshot_immunity",
	"last_human_vampirism",
	"last_human_damage_modifier_percentage",
	"last_human_initial_primary_weapon",
	"last_human_initial_secondary_weapon",
	"last_human_initial_grenade_count",
	"last_human_recharging_grenades",
	"last_human_infinite_ammo",
	"last_human_weapon_pickup",
	"last_human_walking_speed",
	"last_human_personal_gravity",
	"last_human_vehicle_use",
	"last_human_motion_tracker",
	"last_human_motion_tracker_range",
	"last_human_waypoint",
	"last_human_active_camo",
	"last_human_visual",
	"last_human_forced_change_colors",

	"king_opaque_hill",
	"king_score_to_win",
	"king_team_scoring",
	"king_moving_hill",
	"king_moving_hill_order",
	"king_inside_hill_points",
	"king_outside_hill_points",
	"king_uncontested_hill_points",
	"king_kill_points",

	"in_hill_shield_damage_resistance",
	"in_hill_shield_multiplier",
	"in_hill_shield_recharge_rate",
	"in_hill_headshot_immunity",
	"in_hill_vampirism",
	"in_hill_damage_modifier_percentage",
	"in_hill_initial_primary_weapon",
	"in_hill_initial_secondary_weapon",
	"in_hill_initial_grenade_count",
	"in_hill_recharging_grenades",
	"in_hill_infinite_ammo",
	"in_hill_weapon_pickup",
	"in_hill_walking_speed",
	"in_hill_personal_gravity",
	"in_hill_vehicle_use",
	"in_hill_motion_tracker",
	"in_hill_motion_tracker_range",
	"in_hill_waypoint",
	"in_hill_active_camo",
	"in_hill_visual",
	"in_hill_forced_change_colors",

	"territories_capture_time",
	"territories_one_sided",
	"territories_lock_after_capture",
	"territories_sudden_death",
	"territories_respawn_on_capture",

	"territory_defender_shield_damage_resistance",
	"territory_defender_shield_multiplier",
	"territory_defender_shield_recharge_rate",
	"territory_defender_headshot_immunity",
	"territory_defender_vampirism",
	"territory_defender_damage_modifier_percentage",
	"territory_defender_initial_primary_weapon",
	"territory_defender_initial_secondary_weapon",
	"territory_defender_initial_grenade_count",
	"territory_defender_recharging_grenades",
	"territory_defender_infinite_ammo",
	"territory_defender_weapon_pickup",
	"territory_defender_walking_speed",
	"territory_defender_personal_gravity",
	"territory_defender_vehicle_use",
	"territory_defender_motion_tracker",
	"territory_defender_motion_tracker_range",
	"territory_defender_waypoint",
	"territory_defender_active_camo",
	"territory_defender_visual",
	"territory_defender_forced_change_colors",

	"territory_attacker_shield_damage_resistance",
	"territory_attacker_shield_multiplier",
	"territory_attacker_shield_recharge_rate",
	"territory_attacker_headshot_immunity",
	"territory_attacker_vampirism",
	"territory_attacker_damage_modifier_percentage",
	"territory_attacker_initial_primary_weapon",
	"territory_attacker_initial_secondary_weapon",
	"territory_attacker_initial_grenade_count",
	"territory_attacker_recharging_grenades",
	"territory_attacker_infinite_ammo",
	"territory_attacker_weapon_pickup",
	"territory_attacker_walking_speed",
	"territory_attacker_personal_gravity",
	"territory_attacker_vehicle_use",
	"territory_attacker_motion_tracker",
	"territory_attacker_motion_tracker_range",
	"territory_attacker_waypoint",
	"territory_attacker_active_camo",
	"territory_attacker_visual",
	"territory_attacker_forced_change_colors",

	"vip_score_to_win",
	"vip_single_vip",
	"vip_destination_zones_enabled",
	"vip_end_round_on_vip_death",
	"vip_kill_points",
	"vip_takedown_points",
	"vip_kill_as_vip_points",
	"vip_death_points",
	"vip_zone_arrival_points",
	"vip_suicide_points",
	"vip_vip_friendly_death_points",
	"vip_betrayal_points",
	"vip_selection",
	"vip_zone_movement",
	"vip_zone_order",
	"vip_influence_radius",

	"vip_team_shield_damage_resistance",
	"vip_team_shield_multiplier",
	"vip_team_shield_recharge_rate",
	"vip_team_headshot_immunity",
	"vip_team_vampirism",
	"vip_team_damage_modifier_percentage",
	"vip_team_initial_primary_weapon",
	"vip_team_initial_secondary_weapon",
	"vip_team_initial_grenade_count",
	"vip_team_recharging_grenades",
	"vip_team_infinite_ammo",
	"vip_team_weapon_pickup",
	"vip_team_walking_speed",
	"vip_team_personal_gravity",
	"vip_team_vehicle_use",
	"vip_team_motion_tracker",
	"vip_team_motion_tracker_range",
	"vip_team_waypoint",
	"vip_team_active_camo",
	"vip_team_visual",
	"vip_team_forced_change_colors",

	"vip_influence_shield_damage_resistance",
	"vip_influence_shield_multiplier",
	"vip_influence_shield_recharge_rate",
	"vip_influence_headshot_immunity",
	"vip_influence_vampirism",
	"vip_influence_damage_modifier_percentage",
	"vip_influence_initial_primary_weapon",
	"vip_influence_initial_secondary_weapon",
	"vip_influence_initial_grenade_count",
	"vip_influence_recharging_grenades",
	"vip_influence_infinite_ammo",
	"vip_influence_weapon_pickup",
	"vip_influence_walking_speed",
	"vip_influence_personal_gravity",
	"vip_influence_vehicle_use",
	"vip_influence_motion_tracker",
	"vip_influence_motion_tracker_range",
	"vip_influence_waypoint",
	"vip_influence_active_camo",
	"vip_influence_visual",
	"vip_influence_forced_change_colors",

	"vip_shield_damage_resistance",
	"vip_shield_multiplier",
	"vip_shield_recharge_rate",
	"vip_headshot_immunity",
	"vip_vampirism",
	"vip_damage_modifier_percentage",
	"vip_initial_primary_weapon",
	"vip_initial_secondary_weapon",
	"vip_initial_grenade_count",
	"vip_recharging_grenades",
	"vip_infinite_ammo",
	"vip_weapon_pickup",
	"vip_walking_speed",
	"vip_personal_gravity",
	"vip_vehicle_use",
	"vip_motion_tracker",
	"vip_motion_tracker_range",
	"vip_waypoint",
	"vip_active_camo",
	"vip_visual",
	"vip_forced_change_colors",

	"juggernaut_score_to_win",
	"juggernaut_kill_points",
	"juggernaut_kill_juggernaut_poins",
	"juggernaut_kill_as_juggernaut_points",
	"juggernaut_zone_arrival_points",
	"juggernaut_suicide_points",
	"juggernaut_betrayal_points",
	"juggernaut_initial_juggernaut",
	"juggernaut_next_juggernaut",
	"juggernaut_allied_against_juggernaut",
	"juggernaut_delay",
	"juggernaut_zones_enabled",
	"juggernaut_zone_movement",
	"juggernaut_zone_order",
	"juggernaut_respawn_on_lone_juggernaut",
	"juggernaut_shield_damage_resistance",
	"juggernaut_shield_multiplier",
	"juggernaut_shield_recharge_rate",
	"juggernaut_headshot_immunity",
	"juggernaut_vampirism",
	"juggernaut_damage_modifier_percentage",
	"juggernaut_initial_primary_weapon",
	"juggernaut_initial_secondary_weapon",
	"juggernaut_initial_grenade_count",
	"juggernaut_recharging_grenades",
	"juggernaut_infinite_ammo",
	"juggernaut_weapon_pickup",
	"juggernaut_walking_speed",
	"juggernaut_personal_gravity",
	"juggernaut_vehicle_use",
	"juggernaut_motion_tracker",
	"juggernaut_motion_tracker_range",
	"juggernaut_waypoint",
	"juggernaut_active_camo",
	"juggernaut_visual",
	"juggernaut_forced_change_colors",

	"assault_arming_shield_damage_resistance",
	"assault_arming_shield_multiplier",
	"assault_arming_shield_recharge_rate",
	"assault_arming_headshot_immunity",
	"assault_arming_vampirism",
	"assault_arming_damage_modifier_percentage",
	"assault_arming_initial_primary_weapon",
	"assault_arming_initial_secondary_weapon",
	"assault_arming_initial_grenade_count",
	"assault_arming_recharging_grenades",
	"assault_arming_infinite_ammo",
	"assault_arming_weapon_pickup",
	"assault_arming_walking_speed",
	"assault_arming_personal_gravity",
	"assault_arming_vehicle_use",
	"assault_arming_motion_tracker",
	"assault_arming_motion_tracker_range",
	"assault_arming_waypoint",
	"assault_arming_active_camo",
	"assault_arming_visual",
	"assault_arming_forced_change_colors",

	"assault_bomb_arming_time",
	"assault_bomb_disarming_time",
	"assault_bomb_fuse_time",
	"assault_bomb_reset_time",

	"assault_carrier_shield_damage_resistance",
	"assault_carrier_shield_multiplier",
	"assault_carrier_shield_recharge_rate",
	"assault_carrier_headshot_immunity",
	"assault_carrier_vampirism",
	"assault_carrier_damage_modifier_percentage",
	"assault_carrier_initial_primary_weapon",
	"assault_carrier_initial_secondary_weapon",
	"assault_carrier_initial_grenade_count",
	"assault_carrier_recharging_grenades",
	"assault_carrier_infinite_ammo",
	"assault_carrier_weapon_pickup",
	"assault_carrier_walking_speed",
	"assault_carrier_personal_gravity",
	"assault_carrier_vehicle_use",
	"assault_carrier_motion_tracker",
	"assault_carrier_motion_tracker_range",
	"assault_carrier_waypoint",
	"assault_carrier_active_camo",
	"assault_carrier_visual",
	"assault_carrier_forced_change_colors",
	"assault_unknown571",
	"assault_game_type",
	"assault_reset_bomb_on_disarm",
	"assault_respawn",
	"assault_enemy_bomb_waypoint",
	"assault_score_to_win",
	"assault_unknown577",
	"assault_unknown578",
	"assault_unknown579",
	"assault_sudden_death_time",

	"sandbox_open_voice_channel",
	"sandbox_enter_editor_mode",
	"sandbox_respawn_time",
	"sandbox_editor_shield_damage_resistance",
	"sandbox_editor_shield_multiplier",
	"sandbox_editor_shield_recharge_rate",
	"sandbox_editor_headshot_immunity",
	"sandbox_editor_vampirism",
	"sandbox_editor_damage_modifier_percentage",
	"sandbox_editor_initial_primary_weapon",
	"sandbox_editor_initial_secondary_weapon",
	"sandbox_editor_initial_grenade_count",
	"sandbox_editor_recharging_grenades",
	"sandbox_editor_infinite_ammo",
	"sandbox_editor_weapon_pickup",
	"sandbox_editor_walking_speed",
	"sandbox_editor_personal_gravity",
	"sandbox_editor_vehicle_use",
	"sandbox_editor_motion_tracker",
	"sandbox_editor_motion_tracker_range",
	"sandbox_editor_waypoint",
	"sandbox_editor_active_camo",
	"sandbox_editor_visual",
	"sandbox_editor_forced_change_colors",

	"template_shield_damage_resistance",
	"template_shield_multiplier",
	"template_shield_recharge_rate",
	"template_headshot_immunity",
	"template_vampirism",
	"template_damage_modifier_percentage",
	"template_initial_primary_weapon",
	"template_initial_secondary_weapon",
	"template_initial_grenade_count",
	"template_recharging_grenades",
	"template_infinite_ammo",
	"template_weapon_pickup",
	"template_walking_speed",
	"template_personal_gravity",
	"template_vehicle_use",
	"template_motion_tracker",
	"template_motion_tracker_range",
	"template_waypoint",
	"template_active_camo",
	"template_visual",
	"template_forced_change_colors",
};

// 0189DF28
const char* global_campaign_difficulty_level_names[k_campaign_difficulty_levels_count]
{
	"easy",
	"normal",
	"heroic",
	"legendary"
};

// 018BE868
const char* global_campaign_team_names[k_campaign_team_count]
{
	"default",
	"player",
	"human",
	"covenant",
	"flood",
	"sentinel",
	"heretic",
	"prophet",
	"guilty",
	"unused9",
	"unused10",
	"unused11",
	"unused12",
	"unused13",
	"unused14",
	"unused15"
};

// 0189EECC
const char* global_multiplayer_team_names[k_multiplayer_team_count]
{
	"mp_team_red",
	"mp_team_blue",
	"mp_team_green",
	"mp_team_yellow",
	"mp_team_purple",
	"mp_team_orange",
	"mp_team_brown",
	"mp_team_grey"
};

// 0191CB00
const char* global_controller_index_names[k_number_of_controllers]
{
	"controller1",
	"controller2",
	"controller3",
	"controller4"
};

// 0191CB10
const char* global_button_preset_names[k_button_presets_count]
{
	"standard",
	"south_paw",
	"boxer",
	"green_thumb",
	"professional",
	"walkie-talkie"
};

// 0191CB28
const char* global_joystick_preset_names[k_joystick_presets_count]
{
	"standard",
	"south_paw",
	"legacy",
	"legacy_south_paw"
};

// 0189DD38
const char* global_player_model_choice_names[k_player_model_choice_count]
{
	"spartan",
	"elite"
};

// 0191CB38
const char* global_voice_output_setting_names[k_number_of_voice_output_settings]
{
	"default",
	"headset"
};

// 0191CB40
const char* global_voice_mask_names[k_number_of_voice_masks]
{
	"none",
	"anonymous"
};

// 0191CB48
const char* global_subtitle_setting_names[k_number_of_subtitle_settings]
{
	"automatic",
	"enabled",
	"disabled"
};

// 01992AF8
const char* global_actor_type_names[k_number_of_actor_types]
{
	"elite",
	"jackal",
	"grunt",
	"hunter",
	"engineer",
	"assassin",
	"player",
	"marine",
	"crew",
	"combat_form",
	"infection_form",
	"carrier_form",
	"monitor",
	"sentinel",
	"none",
	"mounted_weapon",
	"brute",
	"prophet",
	"bugger",
	"juggernaut",
	"pure_form_stealth",
	"pure_form_tank",
	"pure_form_ranged",
	"scarab",
	"guardian"
};

// 018ECC80
const char* global_model_state_names[k_number_of_model_states]
{
	"standard",
	"minor damage",
	"medium damage",
	"major damage",
	"destroyed"
};

// 018ECCEC
const char* global_character_physics_override_names[k_total_character_physics_overrides]
{
	"NONE",
	"player",
	"biped"
};

// 018BAE24
const char* global_primary_skull_names[k_campaign_skull_primary_count]
{
	"skull_iron",
	"skull_black_eye",
	"skull_tough_luck",
	"skull_catch",
	"skull_fog",
	"skull_famine",
	"skull_thunderstorm",
	"skull_tilt",
	"skull_mythic"
};

// 018BAE48
const char* global_secondary_skull_names[k_campaign_skull_secondary_count]
{
	"skull_assassin",
	"skull_blind",
	"skull_superman",
	"skull_birthday_party",
	"skull_daddy",
	"skull_third_person",
	"skull_directors_cut"
};

