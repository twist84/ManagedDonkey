#include "game/cheats.hpp"

#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "input/input_abstraction.hpp"
#include "interface/terminal.hpp"
#include "items/equipment_definitions.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/object_definitions.hpp"
#include "objects/objects.hpp"
#include "physics/collisions.hpp"
#include "scenario/scenario.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "tag_files/string_ids.hpp"
#include "units/bipeds.hpp"
#include "units/vehicle_definitions.hpp"

#include <math.h>
#include <string.h>

cheat_globals cheat = {};

void patch_cheats()
{
	patch_pointer({ .address = 0x01656700 }, cheats_initialize);
	patch_pointer({ .address = 0x01656704 }, cheats_dispose);
	patch_pointer({ .address = 0x01656708 }, cheats_initialize_for_new_map);
	patch_pointer({ .address = 0x0165670C }, cheats_dispose_from_old_map);
}

void __cdecl cheats_initialize()
{
	cheat.lines.clear();
}

void __cdecl cheats_dispose()
{

}

void __cdecl cheats_initialize_for_new_map()
{
	cheats_load();
}

void __cdecl cheats_dispose_from_old_map()
{

}

void __cdecl cheats_load()
{
	FILE* cheats_file;
	if (fopen_s(&cheats_file, "cheats.txt", "r") == 0 && cheats_file)
	{
		char line[200]{};
		for (long controller_button = _controller_button_left_trigger; controller_button < k_controller_button_count && fgets(line, NUMBEROF(line), cheats_file); controller_button++)
		{
			char* line_match = strpbrk(line, "\r\n\t;");
			if (line_match == line)
				continue;

			if (controller_button == _controller_button_back || controller_button == _controller_button_start)
			{
				console_printf("Cannot execute cheats attached to the back or start button");
			}
			else
			{
				*line_match = 0;
				cheat.lines[controller_button].set(line);
			}
		}

		fclose(cheats_file);
	}
}

// #TODO: find used locations and add hooks
bool __cdecl cheats_process_gamepad(long controller_index, s_game_input_state const* input_state)
{
	e_button_action banned_action = static_cast<e_button_action>(game_is_ui_shell() + _button_action_back);
	if (cheat.controller && controller_index != k_no_controller && !game_is_networked() && input_state->get_button(banned_action).down_frames())
	{
		for (long controller_button = _controller_button_left_trigger; controller_button < k_controller_button_count; controller_button++)
		{
			e_button_action controller_action = static_cast<e_button_action>(controller_button);
			if (controller_action != banned_action && !cheat.lines[controller_button].is_empty() && input_state->get_button(controller_action).down_frames() == 1)
			{
				console_printf(cheat.lines[controller_button].get_string());

				// #TODO: add and implement `hs_compile_and_evaluate`
				//hs_compile_and_evaluate(_event_level_message, "cheats", cheat.lines[controller_button].get_string(), true);
				if (cheat.lines[controller_button].is_equal("(set cheat_controller (not cheat_controller))"))
					cheat.controller = !cheat.controller;
			}
		}

		return true;
	}

	return false;
}

bool __cdecl cheat_get_teleport_to_camera_information(long* unit_index, real_point3d* position)
{
	e_output_user_index output_user_index = player_mapping_first_active_output_user();
	if (output_user_index == k_number_of_output_users)
		return false;

	s_observer_result const* result = observer_get_camera(output_user_index);
	ASSERT(result);

	if (result->location.cluster_reference.bsp_index == 0xFF)
	{
		terminal_printf(global_real_argb_orange, "Camera is outside BSP... cannot initiate teleportation...");
		return false;
	}

	*unit_index = object_get_ultimate_parent(player_mapping_get_unit_by_output_user(output_user_index));
	*position = result->focus_point;

	return true;
}

void __cdecl cheat_teleport_to_camera()
{
	long unit_index = NONE;
	real_point3d position{};
	if (cheat_get_teleport_to_camera_information(&unit_index, &position))
	{
		if (game_is_authoritative())
		{
			//simulation_action_unit_debug_teleport(unit_index, &position);
			if (game_is_authoritative() && !game_is_playback())
			{
				if (game_is_distributed())
				{
					if (unit_index == NONE)
					{
						generate_event(_event_level_warning, "networking:simulation:event: attempting to generate a debug teleport event for NONE target unit");
					}
					else
					{
						//simulation_event_generate_for_simulation_queue
					}
				}
				else
				{
					object_debug_teleport(unit_index, &position);
				}
			}

			return;
		}

		//simulation_request_unit_debug_teleport(unit_index, &position);
	}
}

void __cdecl cheat_active_camouflage(bool enable)
{
	e_input_user_index user_index = player_mapping_first_active_input_user();
	long player_index = player_mapping_get_player_by_input_user(user_index);
	cheat_active_camouflage_by_player(player_index, enable);
}

void __cdecl cheat_active_camouflage_by_player(long player_index, bool enable)
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	e_input_user_index user_index = player_mapping_get_input_user(player_index);
	if (VALID_INDEX(user_index, k_number_of_input_users))
	{
		player_datum* player = (player_datum*)datum_get(*player_data, player_index);

		if (player->unit_index != NONE)
		{
			if (enable && !unit_active_camouflage_is_active(player->unit_index))
			{
				unit_active_camouflage_strength(player->unit_index, 1.0f);
				unit_active_camouflage_enable(player->unit_index, 4.0f, -1);
			}
			else
			{
				unit_active_camouflage_strength(player->unit_index, 1.0f);
				unit_active_camouflage_disable(player->unit_index, 4.0f);
			}
		}
	}
}

long __cdecl cheat_player_index()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	c_data_iterator<player_datum> player_iterator;
	player_iterator.begin(*player_data);
	while (player_iterator.next())
	{
		if (player_iterator.get_datum()->unit_index != NONE)
		{
			return player_iterator.get_index();
		}
	}

	return NONE;
}

void __cdecl cheat_objects(s_tag_reference* references, short reference_count)
{
	long player_index = cheat_player_index();
	if (player_index == NONE)
		return;

	real radius = 0.0f;
	for (short reference_index = 0; reference_index < reference_count; reference_index++)
	{
		s_tag_reference& reference = references[reference_index];
		if (reference.index == NONE)
			continue;

		struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, reference.index);
		if (!object_definition)
			continue;

		real bounding_radius = object_definition->object.bounding_radius + 1.5f;
		if (radius <= bounding_radius)
			radius = bounding_radius;
	}

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	player_datum* player = static_cast<player_datum*>(datum_try_and_get(*player_data, player_index));
	if (!player)
		return;

	for (short reference_index = 0; reference_index < reference_count; reference_index++)
	{
		s_tag_reference& reference = references[reference_index];
		if (reference.index == NONE)
			continue;

		real_point3d origin{};
		vector3d forward{};
		vector3d up{};

		object_get_origin(player->unit_index, &origin);
		object_get_orientation(player->unit_index, &forward, &up);

		object_placement_data data{};
		object_placement_data_new(&data, reference.index, NONE, NULL);

		data.position = origin;
		data.forward = forward;
		data.up = up;

		real angle_scaling_factor = atan2f(forward.i, forward.j) + ((TWO_PI * reference_index) / reference_count);
		data.position.x += (cosf(angle_scaling_factor) * radius);
		data.position.y += (sinf(angle_scaling_factor) * radius);
		data.position.z += 0.8f;

		long object_index = object_new(&data);

		if (object_index != NONE)
			simulation_action_object_create(object_index);
	}
}

void __cdecl cheat_all_powerups()
{
	s_game_globals* game_globals = scenario_get_game_globals();
	if (game_globals->cheat_powerups.count())
	{
		cheat_objects(game_globals->cheat_powerups.begin(), (short)game_globals->cheat_powerups.count());
	}
	else
	{
		cheat_objects(NULL, 0);
	}
}

void __cdecl cheat_all_vehicles()
{
	short reference_count = 0;
	s_tag_reference references[16]{};

	tag_iterator iterator{};
	tag_iterator_new(&iterator, VEHICLE_TAG);
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		if (!VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		struct vehicle_definition* vehicle_definition = (struct vehicle_definition*)tag_get(iterator.group_tag, tag_index);
		if (vehicle_definition->unit.powered_seats.count() > 0)
			tag_reference_set(&references[reference_count++], iterator.group_tag, tag_get_name(tag_index));
	}

	cheat_objects(references, reference_count);
}

void __cdecl cheat_all_weapons()
{
	short reference_count = 0;
	s_tag_reference references[128 /* 32 */]{};

	tag_iterator iterator{};
	tag_iterator_new(&iterator, WEAPON_TAG);
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		if (!VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		tag_reference_set(&references[reference_count++], iterator.group_tag, tag_get_name(tag_index));
	}

	cheat_objects(references, reference_count);
}

void __cdecl cheat_all_chars()
{
	short reference_count = 0;
	s_tag_reference references[64]{};

	tag_iterator iterator{};
	tag_iterator_new(&iterator, UNIT_TAG);
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		if (!VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		char const* tag_name = tag_get_name(tag_index);
		if (tag_name && strstr(tag_name, "character"))
			tag_reference_set(&references[reference_count++], iterator.group_tag, tag_name);
	}

	cheat_objects(references, reference_count);
}

void __cdecl cheat_spawn_warthog()
{
	short reference_count = 0;
	s_tag_reference references[16]{};

	tag_iterator iterator{};
	tag_iterator_new(&iterator, VEHICLE_TAG);
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		if (!VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		char const* tag_name = tag_get_name(tag_index);
		if (tag_name && strstr(tag_name, "warthog"))
			tag_reference_set(&references[reference_count++], iterator.group_tag, tag_name);
	}

	cheat_objects(references, reference_count);
}

bool __cdecl cheat_drop_effect(tag group_tag, char const* effect_name, long effect_index, real_point3d* position, vector3d* forward)
{
	if (effect_index == NONE)
	{
		generate_event(_event_level_warning, "cheats: couldn't load effect '%s.effect' to drop it", effect_name);
		return false;
	}

	collision_result collision;
	vector3d scaled_forward{};
	scale_vector3d(forward, 1000.0f, &scaled_forward);

	if (!collision_test_vector(_collision_test_structure_geometry_flags, position, &scaled_forward, NONE, NONE, &collision))
	{
		generate_event(_event_level_warning, "cheats: couldn't find location to drop effect '%s.effect'", effect_name);
		return false;
	}

	real_point3d collision_position = collision.position;
	vector3d normal = collision.plane.n;

	collision_position.x -= forward->i * 0.25f;
	collision_position.y -= forward->j * 0.25f;
	collision_position.z -= forward->k * 0.25f;

	effect_new_from_point_vector(effect_index, &collision_position, forward, &normal, 1, 1, NULL, NULL);

	return true;
}

bool __cdecl cheat_drop_object(tag group_tag, char const* tag_name, tag expected_group_tag, long object_definition_index, long variant_name, long shader, real_point3d const* position, vector3d const* forward, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	char const* tag_group_name = "unknown";

	tag_group_name = tag_group_get_name(group_tag);

	if (game_is_predicted())
		return false;

	if (object_definition_index == NONE)
	{
		if (expected_group_tag == OBJECT_TAG)
			generate_event(_event_level_warning, "cheats: couldn't load object '%s.%s' to drop it", tag_name, tag_group_name);

		return false;
	}

	object_placement_data data{};
	object_placement_data_new(&data, object_definition_index, NONE, NULL);

	struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, object_definition_index);
	real bounding_radius = object_definition->object.bounding_radius + 1.0f;

	if (variant_name != NONE)
		data.model_variant_index = variant_name;

	data.position = *position;
	data.position.x += bounding_radius * forward->i;
	data.position.y += bounding_radius * forward->j;
	data.position.z += bounding_radius * forward->k;

	if (permutations && permutation_count > 0)
	{
		for (long i = 0; i < permutation_count; i++)
		{
			data.model_customization_overrides[i] = permutations[i];
			data.model_customization_override_count++;
		}
	}

	long object_index = object_new(&data);
	if (object_index == NONE)
	{
		generate_event(_event_level_warning, "cheats: couldn't place '%s.%s'", tag_name, tag_group_name);
		return false;
	}

	object_force_inside_bsp(object_index, position, NONE);

	//if (shader != NONE)
	//	object_override_set_shader(object_index, shader);

	if (object_definition->object.type == _object_type_biped && ((biped_datum*)object_get_and_verify_type(object_index, _object_mask_biped))->unit.current_weapon_set.weapon_indices[0] == NONE)
	{
		tag_iterator iterator{};
		tag_iterator_new(&iterator, WEAPON_TAG);
		for (long weapon_definition_index = tag_iterator_next(&iterator); weapon_definition_index != NONE; weapon_definition_index = tag_iterator_next(&iterator))
		{
			object_placement_data weapon_data{};
			object_placement_data_new(&weapon_data, weapon_definition_index, NONE, NULL);
			long weapon_object_index = object_new(&weapon_data);
			if (weapon_object_index != NONE)
			{
				if (unit_add_weapon_to_inventory(object_index, weapon_object_index, 1))
					break;
	
				object_delete(weapon_object_index);
			}
		}
	}

	simulation_action_object_create(object_index);
	console_printf("placed '%s.%s'", tag_name, tag_group_name);

	return true;
}

void __cdecl cheat_drop_tag_safe_hs(long tag_index)
{
	main_cheat_drop_tag(tag_index, NONE, NULL, 0);
}

long __cdecl cheat_get_tag_definition(tag group_tag, char const* tag_name)
{
	return tag_loaded(group_tag, tag_name);
}

long __cdecl cheat_drop_tag(tag group_tag, char const* tag_name, char const* variant_name, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	long variant_id = NONE;

	char const* tag_group_name = tag_group_get_name(group_tag);

	long tag_index = cheat_get_tag_definition(group_tag, tag_name);

	if (variant_name)
		variant_id = string_id_retrieve(variant_name);

	if (tag_index == NONE)
	{
		generate_event(_event_level_warning, "cheats: couldn't load tag '%s.%s' to place", tag_name, tag_group_name);
	}
	else
	{
		main_cheat_drop_tag(tag_index, variant_id, permutations, permutation_count);
	}

	return tag_index;
}

void cheat_get_droppable_tag_types(tag* const out_droppable_tag_types, long* out_droppable_tag_type_count)
{
	tag droppable_tag_types[] = { VEHICLE_TAG, BIPED_TAG, CREATURE_TAG, WEAPON_TAG, CRATE_TAG, EQUIPMENT_TAG, DEVICE_MACHINE_TAG, DEVICE_CONTROL_TAG, PROJECTILE_TAG, DEVICE_TERMINAL_TAG, SOUND_SCENERY_TAG, SCENERY_TAG, EFFECT_SCENERY_TAG, EFFECT_TAG };
	csmemcpy(out_droppable_tag_types, droppable_tag_types, sizeof(droppable_tag_types));
	*out_droppable_tag_type_count = NUMBEROF(droppable_tag_types);
}

void __cdecl cheat_drop_tag_name_with_variant_and_permutations(char const* tag_name, char const* variant_name, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	c_static_string<256> name = tag_name;

	if (char* position = strrchr(name.get_buffer(), '.'))
	{
		char const* group_name = position + 1;
		*position = 0;

		tag group_tag = group_name_to_group_tag(group_name);
		if (group_tag != NONE)
		{
			cheat_drop_tag(group_tag, name.get_string(), variant_name, permutations, permutation_count);
			return;
		}

		if (strlen(group_name))
		{
			generate_event(_event_level_warning, "cheats: unknown tag group '%s'", group_name);
		}
	}

	tag droppable_tag_types[32]{};
	long droppable_tag_type_count = 0;
	cheat_get_droppable_tag_types(droppable_tag_types, &droppable_tag_type_count);

	long droppable_tag_type_index = 0;
	while (cheat_drop_tag(droppable_tag_types[droppable_tag_type_index], name.get_string(), variant_name, permutations, permutation_count) == NONE)
	{
		if (++droppable_tag_type_index >= 14)
		{
			generate_event(_event_level_warning, "cheats: could not find any tags named '%s' to drop", name.get_string());
			return;
		}
	}
}

long __cdecl cheat_get_region_and_permutation_array_from_string(char const* permutation_info, s_model_customization_region_permutation* permutations, long maximum_permutations)
{
	c_static_string<1022> permutations_string = permutation_info;
	c_static_string<1022> name_string;

	long permutation_string_index = 0;
	long permutation_string_next_index = 0;
	long permutation_string_length = permutations_string.length();

	long permutation_index = 0;
	while (permutation_string_next_index < permutation_string_length && permutation_index < maximum_permutations)
	{
		permutation_string_next_index = permutations_string.next_index_of("=", permutation_string_index);
		if (permutation_string_next_index == -1)
		{
			generate_event(_event_level_error, "error dropping permutation: string '%s' is in an unexpected format!", permutation_info);
			break;
		}

		permutations_string.substring(permutation_string_index, permutation_string_next_index - permutation_string_index, name_string);
		long region_name = string_id_retrieve(name_string.get_string());

		permutation_string_index = permutation_string_next_index + 1;
		permutation_string_next_index = permutations_string.next_index_of(",", permutation_string_next_index + 1);

		long index = permutation_string_next_index;
		if (permutation_string_next_index != -1)
			index = permutation_string_length;

		permutations_string.substring(permutation_string_index, index - permutation_string_index, name_string);
		long permutation_name = string_id_retrieve(name_string.get_string());
		permutation_string_index = permutation_string_next_index + 1;

		if (region_name != -1 && permutation_name != -1)
		{
			permutations[permutation_index].region_name = region_name;
			permutations[permutation_index++].permutation_name = permutation_name;
		}
	}

	return permutation_index;
}

void __cdecl cheat_drop_tag_name(char const* tag_name)
{
	cheat_drop_tag_name_with_variant_and_permutations(tag_name, NULL, NULL, 0);
}

void __cdecl cheat_drop_tag_name_with_variant_hs(char const* tag_name, char const* variant_name)
{
	cheat_drop_tag_name_with_variant_and_permutations(tag_name, variant_name, NULL, 0);
}

void __cdecl cheat_drop_tag_name_with_permutation_hs(char const* tag_name, char const* permutation_info)
{
	s_model_customization_region_permutation permutations[16]{};
	long permutation_count = cheat_get_region_and_permutation_array_from_string(permutation_info, permutations, NUMBEROF(permutations));
	cheat_drop_tag_name_with_variant_and_permutations(tag_name, NULL, permutations, permutation_count);
}

void __cdecl cheat_drop_tag_in_main_event_loop(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	if (tag_index == NONE)
		return;

	e_output_user_index user_index = player_mapping_first_active_output_user();
	if (user_index == k_number_of_output_users)
		return;

	s_observer_result const* result = observer_try_and_get_camera(user_index);

	tag group_tag_ = tag_get_group_tag(tag_index);
	tag group_tag = group_tag_;
	switch (group_tag)
	{
	case DEVICE_MACHINE_TAG:
	case DEVICE_CONTROL_TAG:
	case BIPED_TAG:
	case CRATE_TAG:
	case CREATURE_TAG:
	case EFFECT_SCENERY_TAG:
	case EQUIPMENT_TAG:
	case GIANT_TAG:
	case DEVICE_TERMINAL_TAG:
	case PROJECTILE_TAG:
	case SCENERY_TAG:
	case SOUND_SCENERY_TAG:
	case VEHICLE_TAG:
	case WEAPON_TAG:
		group_tag = OBJECT_TAG;
		break;
	}

	//random_seed_allow_use();

	if (result)
	{
		if (group_tag == EFFECT_TAG)
		{
			real_point3d focus_point = result->focus_point;
			vector3d forward = result->forward;
			cheat_drop_effect(group_tag, tag_get_name(tag_index), tag_index, &focus_point, &forward);
		}
		else if (group_tag == OBJECT_TAG)
		{
			cheat_drop_object(group_tag_, tag_get_name(tag_index), OBJECT_TAG, tag_index, variant_name, NONE, &result->focus_point, &result->forward, permutations, permutation_count);
		}
		else
		{
			generate_event(_event_level_warning, "cheats: don't know how to place tags of type '%s'", tag_group_get_name(group_tag_));
		}
	}

	//random_seed_disallow_use();
}

