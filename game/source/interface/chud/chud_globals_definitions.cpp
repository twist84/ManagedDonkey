#include "interface/chud/chud_globals_definitions.hpp"

void s_chud_globals_definition::update_reference_names()
{
	UPDATE_BLOCK_REFERENCE_NAMES(skins);
	UPDATE_BLOCK_REFERENCE_NAMES(shaders);
	UPDATE_BLOCK_REFERENCE_NAMES(cortana_configs);

	UPDATE_REFERENCE_NAME(metagame_emblems);
	UPDATE_REFERENCE_NAME(metagame_medals);
	UPDATE_REFERENCE_NAME(metagame_medal_animation);
	UPDATE_REFERENCE_NAME(directional_damage_microtexture);
	UPDATE_REFERENCE_NAME(blip_texture_reference);
	UPDATE_REFERENCE_NAME(grunt_birthday_effect);
	UPDATE_REFERENCE_NAME(tentacle_porn);
	UPDATE_REFERENCE_NAME(flood_goo);
	UPDATE_REFERENCE_NAME(parallax_data);
	UPDATE_REFERENCE_NAME(survival_mode_multiplayer_intro);
}

void s_chud_curvature_info::update_reference_names()
{
	UPDATE_REFERENCE_NAME(health_effect);
	UPDATE_REFERENCE_NAME(third_person_health_effect);
	UPDATE_REFERENCE_NAME(shield_effect);
	UPDATE_REFERENCE_NAME(third_person_shield_effect);
}

void s_chud_sound_character_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(sound);
}

void chud_sound_element_definition::update_reference_names()
{
	UPDATE_BLOCK_REFERENCE_NAMES(characters);
}

void s_chud_skin_info::update_reference_names()
{
	UPDATE_BLOCK_REFERENCE_NAMES(curvature_infos);
	UPDATE_BLOCK_REFERENCE_NAMES(hud_sounds);

	UPDATE_REFERENCE_NAME(banned_vehicle_entrance_sound);
	UPDATE_REFERENCE_NAME(grenade_switch_frag);
	UPDATE_REFERENCE_NAME(grenade_switch_plasma);
	UPDATE_REFERENCE_NAME(grenade_switch_claymore);
	UPDATE_REFERENCE_NAME(grenade_switch_firebomb);
	UPDATE_REFERENCE_NAME(microtexture_reference);
	UPDATE_REFERENCE_NAME(noise_texture_reference);
	UPDATE_REFERENCE_NAME(damage_arrow_reference);
	UPDATE_REFERENCE_NAME(grenade_waypoint_reference);
	UPDATE_REFERENCE_NAME(unknown_reference);
	UPDATE_REFERENCE_NAME(navpoints_reference);
	UPDATE_REFERENCE_NAME(player_navpoints_reference);
	UPDATE_REFERENCE_NAME(mini_scoreboard_reference);
	UPDATE_REFERENCE_NAME(meta_scoreboard_reference);
	UPDATE_REFERENCE_NAME(survival_scoreboard_reference);
	UPDATE_REFERENCE_NAME(null_scoreboard_reference);
	UPDATE_REFERENCE_NAME(saved_film_reference);
	UPDATE_REFERENCE_NAME(sandbox_reference);
	UPDATE_REFERENCE_NAME(chud_text);
	UPDATE_REFERENCE_NAME(medals_texture_reference);
	UPDATE_REFERENCE_NAME(multiplayer_medal_animation);
	UPDATE_REFERENCE_NAME(medal_animation);
	UPDATE_REFERENCE_NAME(test_bitmap[0]);
	UPDATE_REFERENCE_NAME(test_bitmap[1]);
	UPDATE_REFERENCE_NAME(test_bitmap[2]);
	UPDATE_REFERENCE_NAME(jammer_damage);
	UPDATE_REFERENCE_NAME(jammer_damage_sound);
}

void s_chud_shader::update_reference_names()
{
	UPDATE_REFERENCE_NAME(vertex_shader);
	UPDATE_REFERENCE_NAME(pixel_shader);
}

void s_cortana_effect_heading_config::update_reference_names()
{
	UPDATE_REFERENCE_NAME(sound_reference);
}

void s_cortana_effect_distance_config::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(facing);
	UPDATE_STRUCT_REFERENCE_NAMES(oblique);
}

void s_cortana_effect_config::update_reference_names()
{
	UPDATE_BLOCK_REFERENCE_NAMES(distance_configs);
}
