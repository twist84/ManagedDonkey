#include "game/game_globals.hpp"

#include "cache/cache_files.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

game_globals_storage* game_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	return game_globals;
}

long get_map_minor_version()
{
	if (global_scenario)
		return cache_files_get_header()->minor_version;
	else
		return 0xFFFFFFFF;
}

void s_game_globals::update_reference_names()
{
	ai_globals_ref.get_name();
	multiplayer_globals.get_name();
	survival_mode_globals.get_name();
	rasterizer_globals_ref.get_name();
	default_camera_fx_settings.get_name();
	podium_definition.get_name();
	default_wind_settings.get_name();
	collision_damage_effect.get_name();
	collision_damage.get_name();
	effect_globals.get_name();
	game_progression.get_name();
	game_achievements.get_name();
	input_globals.get_name();

	UPDATE_REFERENCE_NAMES(havok_cleanup_resources);
	UPDATE_REFERENCE_NAMES(sound_globals);
	UPDATE_REFERENCE_NAMES(ai_globals);
	UPDATE_REFERENCE_NAMES(sounds);
	UPDATE_REFERENCE_NAMES(camera);
	UPDATE_REFERENCE_NAMES(grenades);
	UPDATE_REFERENCE_NAMES(interface_tags);
	UPDATE_REFERENCE_NAMES(cheat_weapons);
	UPDATE_REFERENCE_NAMES(cheat_powerups);
	UPDATE_REFERENCE_NAMES(player_information);
	UPDATE_REFERENCE_NAMES(player_representation);
	UPDATE_REFERENCE_NAMES(player_representation_debug);
	UPDATE_REFERENCE_NAMES(falling_damage);
	UPDATE_REFERENCE_NAMES(materials);
	UPDATE_REFERENCE_NAMES(cinematics_globals);
}

void s_game_globals_havok_cleanup_resources::update_reference_names()
{
	object_cleanup_effect.get_name();
}

void s_game_globals::sound_block::update_reference_names()
{
	sound.get_name();
}

void s_game_globals_camera::update_reference_names()
{
	default_unit_camera_track.get_name();
}

void s_game_globals_falling_damage::update_reference_names()
{
	falling_damage.get_name();
	jumping_damage.get_name();
	soft_landing_damage.get_name();
	hard_landing_damage.get_name();
	hs_damage.get_name();
	distance_damage.get_name();
}

void s_game_globals_grenade::update_reference_names()
{
	throwing_effect.get_name();
	equipment.get_name();
	projectile.get_name();
}

void s_game_globals_interface_tag_references::update_reference_names()
{
	obsolete1.get_name();
	obsolete2.get_name();
	screen_color_table.get_name();
	hud_color_table.get_name();
	editor_color_table.get_name();
	dialog_color_table.get_name();
	motion_sensor_sweep_bitmap.get_name();
	motion_sensor_sweep_bitmap_mask.get_name();
	multiplayer_hud_bitmap.get_name();
	unused.get_name();
	motion_sensor_blip_bitmap.get_name();
	interface_goo_map1.get_name();
	interface_goo_map2.get_name();
	interface_goo_map3.get_name();
	mainmenu_ui_globals.get_name();
	singleplayer_ui_globals.get_name();
	multiplayer_ui_globals.get_name();
	chud_globals.get_name();

	UPDATE_REFERENCE_NAMES(gfx_ui_strings);
}

void s_game_globals_interface_tag_references::gfx_ui_string_block::update_reference_names()
{
	strings.get_name();
}

void s_game_globals_player_information::update_reference_names()
{
	coop_countdown_sound.get_name();
	coop_respawn_sound.get_name();
	coop_respawn_effect.get_name();
	flashlight_on.get_name();
	flashlight_off.get_name();
	default_damage_response.get_name();
}

void s_game_globals_player_representation::update_reference_names()
{
	first_person_hands.get_name();
	first_person_body.get_name();
	first_person_unit.get_name();
	binoculars_zoom_in_sounds.get_name();
	binoculars_zoom_out_sounds.get_name();
	voice.get_name();
}

template<tag group_tag>
void s_game_globals_tag_reference<group_tag>::update_reference_names()
{
	get_name();
}

