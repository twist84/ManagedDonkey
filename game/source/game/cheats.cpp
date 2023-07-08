#include "game/cheats.hpp"

#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "input/input_abstraction.hpp"
#include "interface/terminal.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "scenario/scenario.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"

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
	//REFERENCE_DECLARE(0x04FE67A0, long, mainmenu_spartan_unit_index);
	//return mainmenu_spartan_unit_index;

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

		byte* object = (byte*)tag_get('obje', reference.index);

		real bounding_radius = *reinterpret_cast<real*>(object + 4) + 1.5f;
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
		data.multiplayer_object_properties.game_engine_flags = 0;
		data.multiplayer_object_properties.spawn_flags = 0;

		real angle_offset = atan2f(forward.i, forward.j) + (real(TWO_PI * reference_index) / reference_count);
		object_placement_data_new(&data, reference.index, NONE, nullptr);

		data.position = origin;
		data.forward = forward;
		data.up = up;

		data.position.x += (cosf(angle_offset) * radius);
		data.position.y += (sinf(angle_offset) * radius);
		data.position.z += 0.8f;

		long lock = tag_resources_lock_game();
		long object_index = object_new(&data);
		tag_resources_unlock_game(lock);

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
	s_tag_reference references[16];

	tag_iterator iterator{};
	tag_iterator_new(&iterator, 'vehi');
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		if (tag_index == NONE || !VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		byte* vehicle = (byte*)tag_get('vehi', tag_index);
		s_tag_block& powered_seats = *reinterpret_cast<s_tag_block*>(vehicle + 0x358);
		if (powered_seats.count > 0)
		{
			char const* name = tag_get_name(tag_index);
			references[reference_count].group_tag = 'vehi';
			references[reference_count++].index = tag_index;
		}
	}

	cheat_objects(references, reference_count);
}

void __cdecl cheat_all_weapons()
{
	short reference_count = 0;
	s_tag_reference references[128 /* 32 */];

	tag_iterator iterator{};
	tag_iterator_new(&iterator, 'weap');
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		if (tag_index == NONE || !VALID_INDEX(reference_count, NUMBEROF(references)))
			break;

		char const* name = tag_get_name(tag_index);
		references[reference_count].group_tag = 'weap';
		references[reference_count++].index = tag_index;
	}

	cheat_objects(references, reference_count);
}

