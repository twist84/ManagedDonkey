#include "game/cheats.hpp"

#include "camera/observer.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "input/input_abstraction.hpp"
#include "interface/terminal.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"

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
			if (controller_button != banned_button && !cheat.lines[controller_button].empty() && input_state->abstract_buttons[controller_button].down_frames() == 1)
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