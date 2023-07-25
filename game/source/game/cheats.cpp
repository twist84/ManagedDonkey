#include "game/cheats.hpp"

#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "input/input_abstraction.hpp"
#include "interface/terminal.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/object_definitions.hpp"
#include "objects/objects.hpp"
#include "scenario/scenario.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "tag_files/string_ids.hpp"

#include <string.h>

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
		for (long controller_button = 0; controller_button < k_controller_button_count && fgets(line, NUMBEROF(line), cheats_file); controller_button++)
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
	long banned_button = game_is_ui_shell() + _controller_button_back;
	if (cheat.controller && controller_index != k_no_controller && !game_is_networked() && input_state->abstract_buttons[banned_button].down_frames())
	{
		for (long controller_button = _controller_button_left_trigger; controller_button < k_controller_button_count; controller_button++)
		{
			if (controller_button != banned_button && !cheat.lines[controller_button].is_empty() && input_state->abstract_buttons[controller_button].down_frames() == 1)
			{
				console_printf(cheat.lines[controller_button].get_string());

				// #TODO: add and implement `hs_compile_and_evaluate`
				//hs_compile_and_evaluate(_event_level_message, "cheats", cheat.lines[controller_button].get_string(), true);
				if (cheat.lines[controller_button].equals("(set cheat_controller (not cheat_controller))"))
					cheat.controller = !cheat.controller;
			}
		}

		return true;
	}

	return false;
}

bool __cdecl cheat_get_teleport_to_camera_information(long* unit_index, real_point3d* position)
{
	long output_user = player_mapping_first_active_output_user();
	if (output_user == NONE)
		return false;

	s_observer_result const* result = observer_get_camera(output_user);
	ASSERT(result);

	if (result->location.cluster_reference.bsp_index == 0xFF)
	{
		terminal_printf(global_real_argb_orange, "Camera is outside BSP... cannot initiate teleportation...");
		return false;
	}

	*unit_index = object_get_ultimate_parent(player_mapping_get_unit_by_output_user(output_user));
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
						c_console::write_line("networking:simulation:event: attempting to generate a debug teleport event for NONE target unit");
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
long __cdecl cheat_player_index()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	long index = NONE;
	c_player_with_unit_iterator player_iterator(player_data);
	while (player_iterator.next())
	{
		if (player_iterator.get_datum()->unit_index == NONE)
			continue;

		index = player_iterator.get_index();
		break;
	}

	return index;
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

		_object_definition* object = static_cast<_object_definition*>(tag_get(OBJECT_TAG, reference.index));

		real bounding_radius = object->bounding_radius + 1.5f;
		if (radius <= bounding_radius)
			radius = bounding_radius;
	}

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);
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
		data.multiplayer_properties.game_engine_flags = 0;
		data.multiplayer_properties.spawn_flags = 0;

		real angle_offset = atan2f(forward.i, forward.j) + (real(TWO_PI * reference_index) / reference_count);
		object_placement_data_new(&data, reference.index, NONE, nullptr);

		data.position = origin;
		data.forward = forward;
		data.up = up;

		data.position.x += (cosf(angle_offset) * radius);
		data.position.y += (sinf(angle_offset) * radius);
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
		cheat_objects(nullptr, 0);
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
		if (tag_index == NONE || !VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		byte* vehicle = (byte*)tag_get(iterator.group_tag, tag_index);
		s_tag_block& powered_seats = *reinterpret_cast<s_tag_block*>(vehicle + 0x358);
		if (powered_seats.count > 0)
		{
			char const* name = tag_get_name(tag_index);
			references[reference_count].group_tag = iterator.group_tag;
			references[reference_count++].index = tag_index;
		}
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
		if (tag_index == NONE || !VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		char const* name = tag_get_name(tag_index);
		references[reference_count].group_tag = iterator.group_tag;
		references[reference_count++].index = tag_index;
	}

	cheat_objects(references, reference_count);
}

bool __cdecl cheat_drop_object(tag group_tag, char const* tag_name, tag expected_group_tag, long tag_index, long variant_name, long shader, real_point3d const* position, vector3d const* forward, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	char const* tag_group_name = "unknown";

	tag_group_name = tag_group_get_name(group_tag);

	if (game_is_predicted())
		return false;

	if (tag_index == NONE)
	{
		if (expected_group_tag == OBJECT_TAG)
			c_console::write_line("cheats: couldn't load object '%s.%s' to drop it", tag_name, tag_group_name);

		return false;
	}

	object_placement_data data{};
	data.multiplayer_properties.game_engine_flags = 0;
	data.multiplayer_properties.spawn_flags = 0;

	_object_definition* object = static_cast<_object_definition*>(tag_get(OBJECT_TAG, tag_index));
	object_placement_data_new(&data, tag_index, NONE, nullptr);
	real bounding_radius = object->bounding_radius + 1.0f;

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
		c_console::write_line("cheats: couldn't place '%s.%s'", tag_name, tag_group_name);
		return false;
	}

	//object_force_inside_bsp(object_index, position, NONE);
	//if (shader != NONE)
	//	object_override_set_shader(object_index, shader);

	//if (!object->runtime_object_type && object_get_and_verify_type(object_index, 1)->item.__data17C[0xE6] == NONE)
	//{
	//	tag_iterator iterator{};
	//	tag_iterator_new(&iterator, WEAPON_TAG);
	//	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	//	{
	//		object_placement_data weapon_data{};
	//		weapon_data.multiplayer_properties.game_engine_flags = 0;
	//		weapon_data.multiplayer_properties.spawn_flags = 0;
	//		long weapon_object_index = object_new(&weapon_data);
	//		if (weapon_object_index != NONE)
	//		{
	//			if (unit_add_weapon_to_inventory(object_index, weapon_object_index, 1))
	//				break;
	//
	//			object_delete(weapon_object_index);
	//		}
	//	}
	//}

	simulation_action_object_create(object_index);
	console_printf("placed '%s.%s'", tag_name, tag_group_name);

	return true;
}

void __cdecl cheat_drop_tag_safe_hs(long tag_index)
{
	main_cheat_drop_tag(tag_index, NONE, nullptr, 0);
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
		c_console::write_line("cheats: couldn't load tag '%s.%s' to place", tag_name, tag_group_name);
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
	char name_buffer[256]{};
	csstrnzcpy(name_buffer, tag_name, 256);

	if (char* position = strrchr(name_buffer, '.'))
	{
		char const* group_name = position + 1;
		*position = 0;

		tag group_tag = group_name_to_group_tag(group_name);
		if (group_tag != NONE)
		{
			cheat_drop_tag(group_tag, name_buffer, variant_name, permutations, permutation_count);
			return;
		}
		
		if (strlen(group_name))
		{
			c_console::write_line("cheats: unknown tag group '%s'", group_name);
		}
	}

	tag droppable_tag_types[32]{};
	long droppable_tag_type_count = 0;
	cheat_get_droppable_tag_types(droppable_tag_types, &droppable_tag_type_count);

	long droppable_tag_type_index = 0;
	while (cheat_drop_tag(droppable_tag_types[droppable_tag_type_index], name_buffer, variant_name, permutations, permutation_count) == NONE)
	{
		if (++droppable_tag_type_index >= 14)
		{
			c_console::write_line("cheats: could not find any tags named '%s' to drop", name_buffer);
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
			c_console::write_line("error dropping permutation: string '%s' is in an unexpected format!", permutation_info);
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
	cheat_drop_tag_name_with_variant_and_permutations(tag_name, nullptr, nullptr, 0);
}

void __cdecl cheat_drop_tag_name_with_variant_hs(char const* tag_name, char const* variant_name)
{
	cheat_drop_tag_name_with_variant_and_permutations(tag_name, variant_name, nullptr, 0);
}

void __cdecl cheat_drop_tag_name_with_permutation_hs(char const* tag_name, char const* permutation_info)
{
	s_model_customization_region_permutation permutations[16]{};
	long permutation_count = cheat_get_region_and_permutation_array_from_string(permutation_info, permutations, NUMBEROF(permutations));
	cheat_drop_tag_name_with_variant_and_permutations(tag_name, nullptr, permutations, permutation_count);
}

void __cdecl cheat_drop_tag_in_main_event_loop(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	if (tag_index == NONE)
		return;

	long active_user = players_first_active_user();
	if (active_user == NONE)
		return;

	s_observer_result const* result = observer_try_and_get_camera(active_user);

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
			//cheat_drop_effect(group_tag, tag_get_name(tag_index), tag_index, &result->focus_point, &result->forward);
		}
		else if (group_tag == OBJECT_TAG)
		{
			cheat_drop_object(group_tag_, tag_get_name(tag_index), OBJECT_TAG, tag_index, variant_name, NONE, &result->focus_point, &result->forward, permutations, permutation_count);
		}
		else
		{
			c_console::write_line("cheats: don't know how to place tags of type '%s'", tag_group_get_name(group_tag_));
		}
	}

	//random_seed_disallow_use();
}

