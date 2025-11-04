#include "game/game_globals.hpp"

#include "cache/cache_files.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

rgb_color const k_player_colors[]
{
	{ .value = 0x6E6E6E }, // Steel
	{ .value = 0xB9B9B9 }, // Silver
	{ .value = 0xE3E3E3 }, // White
	{ .value = 0xA73B3B }, // Red
	{ .value = 0xE07373 }, // Mauve
	{ .value = 0xF28D8D }, // Salmon
	{ .value = 0xDF9600 }, // Orange
	{ .value = 0xFBB862 }, // Coral
	{ .value = 0xFFD2A7 }, // Peach
	{ .value = 0xD4B632 }, // Gold
	{ .value = 0xF0CD35 }, // Yellow
	{ .value = 0xFFDF84 }, // Pale
	{ .value = 0x63801C }, // Sage
	{ .value = 0x9BB06C }, // Green
	{ .value = 0xDAF1A9 }, // Olive
	{ .value = 0x388489 }, // Teal
	{ .value = 0x55C4C9 }, // Aqua
	{ .value = 0x9CEFEF }, // Cyan
	{ .value = 0x3B659E }, // Blue
	{ .value = 0x6094DF }, // Cobalt
	{ .value = 0xA3BFF6 }, // Sapphire
	{ .value = 0x60479B }, // Violet
	{ .value = 0x9C81E9 }, // Orchid
	{ .value = 0xD0C4FF }, // Lavender
	{ .value = 0x900051 }, // Crimson
	{ .value = 0xD8458F }, // Rubine
	{ .value = 0xFF96C3 }, // Pink
	{ .value = 0x5D4016 }, // Brown
	{ .value = 0xB69679 }, // Tan
	{ .value = 0xE4C6AC }, // Khaki
};

c_typed_tag_block<rgb_color> s_game_globals::profile_colors{};

e_language get_map_language()
{
	e_language result = k_first_language;
	if (global_scenario && VALID_INDEX(global_game_globals->language, k_language_count))
	{
		result = global_game_globals->language;
	}
	return result;
}

int32 get_map_minor_version()
{
	int32 result = NONE;
	if (global_scenario)
	{
		result = cache_files_get_header()->minor_version_number;
	}
	return result;
}

void s_game_globals::update_static_runtime_data()
{
	profile_colors.count = NUMBEROF(k_player_colors);
	profile_colors.address = (void*)&k_player_colors;
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
	UPDATE_REFERENCE_NAME(item);
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
	UPDATE_REFERENCE_NAME(unit_sounds[0]);
	UPDATE_REFERENCE_NAME(unit_sounds[1]);
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

template<tag ...k_group_tags>
void s_game_globals_tag_reference<k_group_tags...>::update_reference_names()
{
	get_name();
}

