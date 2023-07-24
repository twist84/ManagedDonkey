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
	UPDATE_REFERENCE_NAMES(universal);
	UPDATE_REFERENCE_NAMES(runtime);
}

void s_multiplayer_universal_globals_definition::update_reference_names()
{
	random_player_names.get_name();
	team_names.get_name();
	multiplayer_text.get_name();
	sandbox_text.get_name();
	sandbox_object_properties_values.get_name();
	halo3_game_engine_settings.get_name();
}

void s_multiplayer_runtime_globals_definition::update_reference_names()
{
	editor_biped.get_name();
	editor_helper.get_name();
	flag.get_name();
	ball.get_name();
	assault_bomb.get_name();
	vip_influence_area.get_name();
	in_game_text.get_name();
	__reference70_projectile.get_name();
	__reference80_effect.get_name();
	__reference90_effect.get_name();
	default_respawn_sound.get_name();
	modifier_respawn_sound.get_name();
	scoreboard_emblem_bitmap.get_name();
	scoreboard_dead_emblem_bitmap.get_name();
	hill_shader.get_name();
	unused_intro_message.get_name();
	ctf_intro_message.get_name();
	slayer_intro_message.get_name();
	oddball_intro_message.get_name();
	king_intro_message.get_name();
	sandbox_intro_message.get_name();
	vip_intro_message.get_name();
	juggernaut_intro_message.get_name();
	territories_intro_message.get_name();
	assault_intro_message.get_name();
	infection_intro_message.get_name();
	menu_music_a.get_name();
	menu_music_b.get_name();
	menu_music_c.get_name();
	menu_music_d.get_name();
}

