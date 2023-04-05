#include "game/game_engine_default.hpp"

#include "game/game_engine_util.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_options.hpp"

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
	
	// MOVEMENT TRAITS
	
	// APPEARANCE TRAITS
	
	// SENSORS TRAITS

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
	char const* team_changing = active_variant->get_social_options()->get_team_changing_enabled() ? "ENABLED" : "DISABLED";

	file_printf(file, "observers= %s\r\n", active_variant->get_social_options()->get_observers_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "team changing= %s\r\n", active_variant->get_social_options()->get_team_changing_balancing_only_enabled() ? "BALANCING ONLY" : team_changing);
	file_printf(file, "friendly fire= %s\r\n", active_variant->get_social_options()->get_friendly_fire_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "betrayal booting= %s\r\n", active_variant->get_social_options()->get_betrayal_booting_enabled() ? "ENABLED" : "DISABLED");
	//file_printf(file, "enemy voice= %s\r\n", active_variant->get_social_options()->get_enemy_voice_enabled() ? "ENABLED" : "DISABLED");
	//file_printf(file, "open channel voice= %s\r\n", active_variant->get_social_options()->get_open_channel_voice_enabled() ? "ENABLED" : "DISABLED");
	//file_printf(file, "dead player voice= %s\r\n", active_variant->get_social_options()->get_dead_player_voice_enabled() ? "ENABLED" : "DISABLED");
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

