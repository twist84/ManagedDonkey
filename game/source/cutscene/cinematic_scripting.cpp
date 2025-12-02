#include "cutscene/cinematic_scripting.hpp"

#include "cutscene/cinematics.hpp"
#include "game/players.hpp"
#include "hs/hs_library_external.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "scenario/scenario.hpp"
#include "units/unit_scripting.hpp"

HOOK_DECLARE(0x009EBF40, cinematic_scripting_create_object);
HOOK_DECLARE(0x009EBFF0, cinematic_scripting_destroy_object);
HOOK_DECLARE(0x009EC100, cinematic_scripting_get_object);

bool __cdecl cinematic_index_is_valid(int32 cinematic_index)
{
	return INVOKE(0x009EBD20, cinematic_index_is_valid, cinematic_index);

	// Halo Online
	//return cinematic_index == NONE || cinematic_index > 1000 || global_cinematic_zone_is_active(cinematic_index);

	// Halo 3
	//return cinematic_index > 1000 || global_cinematic_zone_is_active(cinematic_index);
}

//.text:009EBD40 ; void __cdecl cinematic_scripting_clean_up(int32 cinematic_index)
//.text:009EBD50 ; void __cdecl cinematic_scripting_create_and_animate_cinematic_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index, int32 animation_string, bool custom_script_result)
//.text:009EBD60 ; void __cdecl cinematic_scripting_create_and_animate_cinematic_object_no_animation(int32 cinematic_index, int32 scene_index, int32 scene_object_index, bool custom_script_result)
//.text:009EBD80 ; void __cdecl cinematic_scripting_create_and_animate_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index, int32 animation_string, bool custom_script_result)
//.text:009EBEA0 ; void __cdecl cinematic_scripting_create_and_animate_object_no_animation(int32 cinematic_index, int32 scene_index, int32 scene_object_index, bool custom_script_result)
//.text:009EBEC0 ; void __cdecl cinematic_scripting_create_cinematic_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index)

void __cdecl cinematic_scripting_create_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index)
{
	//INVOKE(0x009EBF40, cinematic_scripting_create_object, cinematic_index, scene_index, scene_object_index);

	s_cinematic_scene* scene = get_scene_from_cinematic_or_cortana_effect(cinematic_index, scene_index);
	if (scene && cinematic_index_is_valid(cinematic_index) && VALID_INDEX(scene_object_index, scene->objects.count))
	{
		s_scene_object* scene_object = get_scene_object(scene, scene_object_index);
		if (scene_object)
		{
			int16 object_name_index = scenario_object_name_index_from_string(global_scenario_get(), scene_object->name);
			if (TEST_BIT(scene_object->flags, _scene_object_flags_name_is_function_call))
			{
				hs_object_create_if_necessary(object_name_index);
			}
			else
			{
				hs_object_create_anew(object_name_index);
			}

			int32 object_index = object_index_from_name_index(object_name_index);
			if (object_index != NONE)
			{
				hs_object_hide(object_index, false);
				object_set_always_active(object_index, true);
			}
		}
	}
}

#if 0
void cinematic_scripting_create_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index, short object_name_index) // Halo 3
{
	s_cinematic_scene* scene = get_scene_from_cinematic_or_cortana_effect(cinematic_index, scene_index);
	if (scene && cinematic_index_is_valid(cinematic_index) && VALID_INDEX(scene_object_index, scene->objects.count))
	{
		s_scene_object* scene_object = get_scene_object(scene, scene_object_index);
		if (scene_object)
		{
			if (TEST_BIT(scene_object->flags, _scene_object_flags_name_is_function_call))
			{
				hs_object_create_if_necessary(object_name_index);
			}
			else
			{
				hs_object_create_anew(object_name_index);
			}
	
			int32 object_index = object_index_from_name_index(object_name_index);
			if (object_index != NONE)
			{
				hs_object_hide(object_index, false);
				object_set_always_active(object_index, true);
			}
		}
	}
}
#endif

//.text:009EBFE0 ; void __cdecl cinematic_scripting_destroy_cortana_effect_cinematic()

void __cdecl cinematic_scripting_destroy_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index)
{
	//INVOKE(0x009EBFF0, cinematic_scripting_destroy_object, cinematic_index, scene_index, scene_object_index);

	s_cinematic_scene* scene = get_scene_from_cinematic_or_cortana_effect(cinematic_index, scene_index);
	if (scene && VALID_INDEX(scene_object_index, scene->objects.count))
	{
		s_scene_object* scene_object = get_scene_object(scene, scene_object_index);
		if (scene_object)
		{
			int32 object_index = cinematic_scripting_get_object(cinematic_index, scene_index, scene_object_index);
			if (object_index != NONE)
			{
				object_datum* object = OBJECT_GET(object_index);

				if (!TEST_BIT(scene_object->flags, _scene_object_flags_not_created_dynamically) &&
					!TEST_BIT(scene_object->flags, _scene_object_flags_comes_from_game))
				{
					hs_object_hide(object_index, true);
					object_set_always_active(object_index, false);
					object_cinematic_visibility(object_index, false);
				}

				if (TEST_BIT(_object_mask_unit, object->object.object_identifier.get_type()) &&
					!TEST_BIT(_object_mask_scenery, object->object.object_identifier.get_type()) &&
					!TEST_BIT(_object_mask_effect_scenery, object->object.object_identifier.get_type()))
				{
					unit_scripting_stop_custom_animation(object_index);
					unit_scripting_limit_lipsync_to_mouth_only(object_index, false);
				}
			}
		}
	}
}

//.text:009EC0C0 ; void __cdecl cinematic_scripting_fade_out(real32 fade_red, real32 fade_green, real32 fade_blue, int16 ticks)

int32 __cdecl cinematic_scripting_get_object(int32 cinematic_index, int32 scene_index, int32 scene_object_index)
{
	//return INVOKE(0x009EC100, cinematic_scripting_get_object, cinematic_index, scene_index, scene_object_index);

	int32 result = NONE;

	s_cinematic_scene* scene = get_scene_from_cinematic_or_cortana_effect(cinematic_index, scene_index);
	if (scene && VALID_INDEX(scene_object_index, scene->objects.count))
	{
		s_scene_object* scene_object = get_scene_object(scene, scene_object_index);
		if (scene_object)
		{
			if (!cinematic_get_debug_mode())
			{
				if (TEST_BIT(scene_object->flags, _scene_object_flags_name_is_function_call))
				{
					switch (string_hash(scene_object->name))
					{
					case "player0"_hash:
					{
						result = DATUM_TRY_AND_GET_ABSOLUTE(player_data, player_datum, 0)->unit_index;
					}
					break;
					case "player1"_hash:
					{
						result = DATUM_TRY_AND_GET_ABSOLUTE(player_data, player_datum, 1)->unit_index;
					}
					break;
					case "player2"_hash:
					{
						result = DATUM_TRY_AND_GET_ABSOLUTE(player_data, player_datum, 2)->unit_index;
					}
					break;
					case "player3"_hash:
					{
						result = DATUM_TRY_AND_GET_ABSOLUTE(player_data, player_datum, 3)->unit_index;
					}
					break;
					}
				}
				else if (TEST_BIT(scene_object->flags, _scene_object_flags_comes_from_game))
				{
					result = object_index_from_name_index(
						scenario_object_name_index_from_string(global_scenario_get(), scene_object->name));
				}
				else if (TEST_BIT(scene_object->flags, _scene_object_flags_not_created_dynamically))
				{
					result = object_index_from_name_index(
						scenario_object_name_index_from_string(global_scenario_get(), scene_object->name));
				}
				else
				{
					result = cinematic_object_get(scene_object->name);
				}
			}
			else
			{
				result = cinematic_object_get(scene_object->name);
			}
		}
	}

	return result;
}

//.text:009EC2C0 ; int32 __cdecl cinematic_scripting_get_object_by_name(const char*)
//.text:009EC440 ; bool __cdecl cinematic_scripting_object_coop_flags_valid(int32 cinematic_index, int32 scene_index, int32 object_index)
//.text:009EC4A0 ; void __cdecl cinematic_scripting_play_cortana_effect(int32 cinematic_index, int32 scene_index, int32 shot_index, int32 cortana_effect_index)
//.text:009EC500 ; void __cdecl cinematic_scripting_set_user_input_constraints(int32, int32, int32, int32)
//.text:009EC570 ; bool __cdecl cinematic_scripting_should_reset_object_lighting(const s_cinematic_scene* scene)
//.text:009EC5A0 ; void __cdecl cinematic_scripting_start_animation(int32 cinematic_index, int32 scene_index, int32 scene_object_index, int32 animation_string)
//.text:009EC6C0 ; void __cdecl cinematic_scripting_start_dialogue(int32 cinematic_index, int32 scene_index, int32 shot_index, int32 dialogue_index, int32 source_object_index)
//.text:009EC730 ; void __cdecl cinematic_scripting_start_effect(int32 cinematic_index, int32 scene_index, int32 shot_index, int32 effect_index, int32 object_index)
//.text:009EC790 ; void __cdecl cinematic_scripting_start_music(int32 cinematic_index, int32 scene_index, int32 shot_index, int32 music_index)
//.text:009EC820 ; void __cdecl cinematic_scripting_start_screen_effect(int32, int32, int32, int32)
//.text:009EC880 ; void __cdecl cinematic_scripting_stop_music(int32 cinematic_index, int32 scene_index, int32 shot_index, int32 music_index)
//.text:009EC8E0 ; void __cdecl cinematic_scripting_stop_screen_effect(int32, int32, int32, int32)

s_cinematic_definition* __cdecl get_cinematic(int32 cinematic_index)
{
	return INVOKE(0x009EC940, get_cinematic, cinematic_index);
}

s_cinematic_scene* __cdecl get_scene(s_cinematic_definition* cinematic, int32 scene_index)
{
	return INVOKE(0x009EC980, get_scene, cinematic, scene_index);
}

s_cinematic_scene* __cdecl get_scene_from_cinematic_or_cortana_effect(int32 cinematic_index, int32 scene_index)
{
	return INVOKE(0x009EC9C0, get_scene_from_cinematic_or_cortana_effect, cinematic_index, scene_index);
}

s_scene_object* __cdecl get_scene_object(s_cinematic_scene* scene, int32 object_index)
{
	return INVOKE(0x009ECA80, get_scene_object, scene, object_index);
}

