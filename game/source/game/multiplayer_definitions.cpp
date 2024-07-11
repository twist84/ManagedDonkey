#include "game/multiplayer_definitions.hpp"

#include "cache/cache_files.hpp"
#include "game/game_globals.hpp"
#include "scenario/scenario.hpp"

long __cdecl multiplayer_universal_data_get_random_weapon_definition_index()
{
	return INVOKE(0x0069CC30, multiplayer_universal_data_get_random_weapon_definition_index);
}

long __cdecl multiplayer_universal_data_get_remapped_vehicle_definition_index(long vehicle_tag_index, s_multiplayer_vehicle_set const* vehicle_set)
{
	return INVOKE(0x0069CDC0, multiplayer_universal_data_get_remapped_vehicle_definition_index, vehicle_tag_index, vehicle_set);
}

long __cdecl multiplayer_universal_data_get_remapped_weapon_definition_index(long weapon_tag_index, s_multiplayer_weapon_set const* weapon_set)
{
	return INVOKE(0x0069CE90, multiplayer_universal_data_get_remapped_weapon_definition_index, weapon_tag_index, weapon_set);
}

long __cdecl multiplayer_universal_data_get_vehicle_selection_definition_index(long name)
{
	return INVOKE(0x0069CF70, multiplayer_universal_data_get_vehicle_selection_definition_index, name);
}

short __cdecl multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(long name)
{
	return INVOKE(0x0069CFF0, multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id, name);
}

long __cdecl multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(short absolute_index)
{
	return INVOKE(0x0069D070, multiplayer_universal_data_get_vehicle_set_name_from_absolute_index, absolute_index);
}

long __cdecl multiplayer_universal_data_get_weapon_selection_definition_index(long name)
{
	return INVOKE(0x0069D0E0, multiplayer_universal_data_get_weapon_selection_definition_index, name);
}

short __cdecl multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(long name)
{
	return INVOKE(0x0069D160, multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id, name);
}

long __cdecl multiplayer_universal_data_get_weapon_set_name_from_absolute_index(short absolute_index)
{
	return INVOKE(0x0069D1D0, multiplayer_universal_data_get_weapon_set_name_from_absolute_index, absolute_index);
}

s_multiplayer_vehicle_set const* __cdecl multiplayer_universal_data_vehicle_set_try_and_get(short vehicle_set_index)
{
	return INVOKE(0x0069D250, multiplayer_universal_data_vehicle_set_try_and_get, vehicle_set_index);
}

s_multiplayer_weapon_set const* __cdecl multiplayer_universal_data_weapon_set_try_and_get(short weapon_set_index)
{
	return INVOKE(0x0069D2B0, multiplayer_universal_data_weapon_set_try_and_get, weapon_set_index);
}

s_multiplayer_runtime_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_runtime_data()
{
	return INVOKE(0x0069D310, scenario_multiplayer_globals_try_and_get_runtime_data);
}

s_multiplayer_universal_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_universal_data()
{
	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	if (game_globals)
	{
		s_multiplayer_globals_definition* multiplayer_globals = game_globals->multiplayer_globals.cast_to<s_multiplayer_globals_definition>();
		if (multiplayer_globals)
		{
			s_multiplayer_universal_globals_definition* universal_globals = multiplayer_globals_try_and_get_universal_data(multiplayer_globals);
			return universal_globals;
		}
	}

	return INVOKE(0x0069D340, scenario_multiplayer_globals_try_and_get_universal_data);
}

s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(s_multiplayer_globals_definition* multiplayer_globals)
{
	if (multiplayer_globals && multiplayer_globals->universal.count() > 0)
		return multiplayer_globals->universal.begin();

	return nullptr;
}

void s_multiplayer_globals_definition::update_reference_names()
{
	UPDATE_BLOCK_REFERENCE_NAMES(universal);
	UPDATE_BLOCK_REFERENCE_NAMES(runtime);
}

void s_multiplayer_universal_globals_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(random_player_names);
	UPDATE_REFERENCE_NAME(team_names);
	UPDATE_REFERENCE_NAME(multiplayer_text);
	UPDATE_REFERENCE_NAME(sandbox_text);
	UPDATE_REFERENCE_NAME(sandbox_object_properties_values);
	UPDATE_REFERENCE_NAME(halo3_game_engine_settings);

	UPDATE_BLOCK_REFERENCE_NAMES(customized_spartan_characters);
	UPDATE_BLOCK_REFERENCE_NAMES(customized_elite_characters);
	UPDATE_BLOCK_REFERENCE_NAMES(equipment);
	UPDATE_BLOCK_REFERENCE_NAMES(weapon_selections);
	UPDATE_BLOCK_REFERENCE_NAMES(vehicle_selections);
	UPDATE_BLOCK_REFERENCE_NAMES(grenade_selections);
	UPDATE_BLOCK_REFERENCE_NAMES(podium_animations);
}

void s_multiplayer_customized_model_character::update_reference_names()
{
	UPDATE_BLOCK_REFERENCE_NAMES(customized_areas);
}

void s_multiplayer_customized_model_selection::update_reference_names()
{
	UPDATE_REFERENCE_NAME(third_person_armor_object);
	UPDATE_REFERENCE_NAME(first_person_armor_object);
}

void s_multiplayer_equipment::update_reference_names()
{
	UPDATE_REFERENCE_NAME(object);
}

void s_multiplayer_weapon_selection::update_reference_names()
{
	UPDATE_REFERENCE_NAME(weapon_tag);
}

void s_multiplayer_vehicle_selection::update_reference_names()
{
	UPDATE_REFERENCE_NAME(vehicle_tag);
}

void s_multiplayer_grenade_selection::update_reference_names()
{
	UPDATE_REFERENCE_NAME(grenade_tag);
}

void s_multiplayer_runtime_globals_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(editor_biped);
	UPDATE_REFERENCE_NAME(editor_helper);
	UPDATE_REFERENCE_NAME(flag);
	UPDATE_REFERENCE_NAME(ball);
	UPDATE_REFERENCE_NAME(assault_bomb);
	UPDATE_REFERENCE_NAME(vip_influence_area);
	UPDATE_REFERENCE_NAME(in_game_text);
	UPDATE_REFERENCE_NAME(plant_plasma_on_death_projectile);
	UPDATE_REFERENCE_NAME(vehicle_autoflip_effect);
	UPDATE_REFERENCE_NAME(safety_booster_effect);
	UPDATE_REFERENCE_NAME(default_respawn_sound);
	UPDATE_REFERENCE_NAME(modifier_respawn_sound);
	UPDATE_REFERENCE_NAME(scoreboard_emblem_bitmap);
	UPDATE_REFERENCE_NAME(scoreboard_dead_emblem_bitmap);
	UPDATE_REFERENCE_NAME(hill_shader);
	UPDATE_REFERENCE_NAME(unused_intro_message);
	UPDATE_REFERENCE_NAME(ctf_intro_message);
	UPDATE_REFERENCE_NAME(slayer_intro_message);
	UPDATE_REFERENCE_NAME(oddball_intro_message);
	UPDATE_REFERENCE_NAME(king_intro_message);
	UPDATE_REFERENCE_NAME(sandbox_intro_message);
	UPDATE_REFERENCE_NAME(vip_intro_message);
	UPDATE_REFERENCE_NAME(juggernaut_intro_message);
	UPDATE_REFERENCE_NAME(territories_intro_message);
	UPDATE_REFERENCE_NAME(assault_intro_message);
	UPDATE_REFERENCE_NAME(infection_intro_message);
	UPDATE_REFERENCE_NAME(menu_music_a);
	UPDATE_REFERENCE_NAME(menu_music_b);
	UPDATE_REFERENCE_NAME(menu_music_c);
	UPDATE_REFERENCE_NAME(menu_music_d);

	UPDATE_BLOCK_REFERENCE_NAMES(sounds);
	UPDATE_BLOCK_REFERENCE_NAMES(looping_sounds);
	UPDATE_BLOCK_REFERENCE_NAMES(earn_wp_events);
	UPDATE_BLOCK_REFERENCE_NAMES(general_events);
	UPDATE_BLOCK_REFERENCE_NAMES(flavor_events);
	UPDATE_BLOCK_REFERENCE_NAMES(slayer_events);
	UPDATE_BLOCK_REFERENCE_NAMES(ctf_events);
	UPDATE_BLOCK_REFERENCE_NAMES(oddball_events);
	UPDATE_BLOCK_REFERENCE_NAMES(king_events);
	UPDATE_BLOCK_REFERENCE_NAMES(vip_events);
	UPDATE_BLOCK_REFERENCE_NAMES(juggernaut_events);
	UPDATE_BLOCK_REFERENCE_NAMES(territories_events);
	UPDATE_BLOCK_REFERENCE_NAMES(assault_events);
	UPDATE_BLOCK_REFERENCE_NAMES(infection_events);
	UPDATE_BLOCK_REFERENCE_NAMES(multiplayer_constants);
	UPDATE_BLOCK_REFERENCE_NAMES(state_responses);
}

void s_multiplayer_podium_animation::update_reference_names()
{
	UPDATE_REFERENCE_NAME(animation_tag);

	UPDATE_BLOCK_REFERENCE_NAMES(move_animations);
}

void s_multiplayer_podium_move_animation::update_reference_names()
{
	UPDATE_REFERENCE_NAME(primary_weapon);
	UPDATE_REFERENCE_NAME(secondary_weapon);
}

void s_multiplayer_constants::update_reference_names()
{
	UPDATE_REFERENCE_NAME(hill_bitmap);
	UPDATE_REFERENCE_NAME(bomb_explode_effect);
	UPDATE_REFERENCE_NAME(bomb_explode_secondary_effect);
	UPDATE_REFERENCE_NAME(bomb_explode_dmg_effect);
	UPDATE_REFERENCE_NAME(bomb_defuse_effect);
	UPDATE_REFERENCE_NAME(sandbox_effect);

	UPDATE_BLOCK_REFERENCE_NAMES(weapon_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(vehicle_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(projectile_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(equipment_influencers);
}

void s_game_engine_status_response::update_reference_names()
{
	UPDATE_REFERENCE_NAME(unused);
}

template<tag group_tag>
void s_multiplayer_globals_tag_reference<group_tag>::update_reference_names()
{
	get_name();
}

