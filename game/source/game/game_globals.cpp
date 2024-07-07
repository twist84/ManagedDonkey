#include "game/game_globals.hpp"

#include "cache/cache_files.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

long get_map_minor_version()
{
	if (global_scenario)
		return cache_files_get_header()->minor_version;
	else
		return 0xFFFFFFFF;
}

void s_game_globals::update_reference_names()
{
	UPDATE_REFERENCE_NAME(ai_globals_ref);
	UPDATE_REFERENCE_NAME(multiplayer_globals);
	UPDATE_REFERENCE_NAME(survival_mode_globals);
	UPDATE_REFERENCE_NAME(rasterizer_globals_ref);
	UPDATE_REFERENCE_NAME(default_camera_fx_settings);
	UPDATE_REFERENCE_NAME(podium_definition);
	UPDATE_REFERENCE_NAME(default_wind_settings);
	UPDATE_REFERENCE_NAME(collision_damage_effect);
	UPDATE_REFERENCE_NAME(collision_damage);
	UPDATE_REFERENCE_NAME(effect_globals);
	UPDATE_REFERENCE_NAME(game_progression);
	UPDATE_REFERENCE_NAME(game_achievements);
	UPDATE_REFERENCE_NAME(input_globals);

	UPDATE_BLOCK_REFERENCE_NAMES(havok_cleanup_resources);
	UPDATE_BLOCK_REFERENCE_NAMES(sound_globals);
	UPDATE_BLOCK_REFERENCE_NAMES(ai_globals);
	UPDATE_BLOCK_REFERENCE_NAMES(sounds);
	UPDATE_BLOCK_REFERENCE_NAMES(camera);
	UPDATE_BLOCK_REFERENCE_NAMES(grenades);
	UPDATE_BLOCK_REFERENCE_NAMES(interface_tag_references);
	UPDATE_BLOCK_REFERENCE_NAMES(weapon_list);
	UPDATE_BLOCK_REFERENCE_NAMES(cheat_powerups);
	UPDATE_BLOCK_REFERENCE_NAMES(player_information);
	UPDATE_BLOCK_REFERENCE_NAMES(player_representation);
	UPDATE_BLOCK_REFERENCE_NAMES(player_representation_debug);
	UPDATE_BLOCK_REFERENCE_NAMES(falling_damage);
	UPDATE_BLOCK_REFERENCE_NAMES(materials);
	UPDATE_BLOCK_REFERENCE_NAMES(cinematics_globals);
}

void s_game_globals_havok_cleanup_resources::update_reference_names()
{
	UPDATE_REFERENCE_NAME(object_cleanup_effect);
}

void s_game_globals_camera::update_reference_names()
{
	UPDATE_REFERENCE_NAME(default_unit_camera_track);
}

void s_game_globals_falling_damage::update_reference_names()
{
	UPDATE_REFERENCE_NAME(falling_damage);
	UPDATE_REFERENCE_NAME(jumping_damage);
	UPDATE_REFERENCE_NAME(soft_landing_damage);
	UPDATE_REFERENCE_NAME(hard_landing_damage);
	UPDATE_REFERENCE_NAME(hs_damage);
	UPDATE_REFERENCE_NAME(distance_damage);
}

void s_game_globals_grenade::update_reference_names()
{
	UPDATE_REFERENCE_NAME(throwing_effect);
	UPDATE_REFERENCE_NAME(equipment);
	UPDATE_REFERENCE_NAME(projectile);
}

void s_game_globals_interface_tag_references::update_reference_names()
{
	UPDATE_REFERENCE_NAME(obsolete1);
	UPDATE_REFERENCE_NAME(obsolete2);
	UPDATE_REFERENCE_NAME(screen_color_table);
	UPDATE_REFERENCE_NAME(hud_color_table);
	UPDATE_REFERENCE_NAME(editor_color_table);
	UPDATE_REFERENCE_NAME(dialog_color_table);
	UPDATE_REFERENCE_NAME(motion_sensor_sweep_bitmap);
	UPDATE_REFERENCE_NAME(motion_sensor_sweep_bitmap_mask);
	UPDATE_REFERENCE_NAME(multiplayer_hud_bitmap);
	UPDATE_REFERENCE_NAME(unused);
	UPDATE_REFERENCE_NAME(motion_sensor_blip_bitmap);
	UPDATE_REFERENCE_NAME(interface_goo_map1);
	UPDATE_REFERENCE_NAME(interface_goo_map2);
	UPDATE_REFERENCE_NAME(interface_goo_map3);
	UPDATE_REFERENCE_NAME(mainmenu_ui_globals);
	UPDATE_REFERENCE_NAME(singleplayer_ui_globals);
	UPDATE_REFERENCE_NAME(multiplayer_ui_globals);
	UPDATE_REFERENCE_NAME(chud_globals);

	UPDATE_BLOCK_REFERENCE_NAMES(gfx_ui_strings);
}

void s_game_globals_interface_tag_references::gfx_ui_string_block::update_reference_names()
{
	UPDATE_REFERENCE_NAME(strings);
}

void s_game_globals_player_information::update_reference_names()
{
	UPDATE_REFERENCE_NAME(coop_countdown_sound);
	UPDATE_REFERENCE_NAME(coop_respawn_sound);
	UPDATE_REFERENCE_NAME(coop_respawn_effect);
	UPDATE_REFERENCE_NAME(flashlight_on);
	UPDATE_REFERENCE_NAME(flashlight_off);
	UPDATE_REFERENCE_NAME(default_damage_response);
}

void s_game_globals_player_representation::update_reference_names()
{
	UPDATE_REFERENCE_NAME(first_person_hands);
	UPDATE_REFERENCE_NAME(first_person_body);
	UPDATE_REFERENCE_NAME(first_person_unit);
	UPDATE_REFERENCE_NAME(binoculars_zoom_in_sounds);
	UPDATE_REFERENCE_NAME(binoculars_zoom_out_sounds);
	UPDATE_REFERENCE_NAME(voice);
}

template<tag group_tag>
void s_game_globals_tag_reference<group_tag>::update_reference_names()
{
	get_name();
}

