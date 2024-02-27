#include "main/console.hpp"

#include "camera/camera_globals.hpp"
#include "cseries/cseries_events.hpp"
#include "cubemaps/cubemap_debug.hpp"
#include "effects/contrails.hpp"
#include "game/cheats.hpp"
#include "hs/hs_runtime.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/terminal.hpp"
#include "interface/user_interface.hpp"
#include "main/debug_keys.hpp"
#include "main/main.hpp"
#include "main/main_time.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/threads.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"
#include "networking/messages/network_message_gateway.hpp"
#include "networking/tools/remote_command.hpp"
#include "objects/object_types.hpp"
#include "physics/havok.hpp"
#include "physics/water_physics.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/dx9/rasterizer_dx9_dynamic_geometry.hpp"
#include "render/old_render_debug.hpp"
#include "render/render_cameras.hpp"
#include "render/render_lights.hpp"
#include "render/render_visibility.hpp"
#include "render/render_water.hpp"
#include "render/views/render_player_view.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "simulation/simulation_debug_globals.hpp"
#include "sound/sound_manager.hpp"
#include "units/bipeds.hpp"
#include "xbox/xbox.hpp"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

HOOK_DECLARE(0x00605E10, console_execute_initial_commands);

s_console_globals console_globals;

bool console_dump_to_debug_display = false;

bool __cdecl debugging_system_has_focus()
{
	return console_is_active() || debug_menu_get_active();
}

void __cdecl console_printf(char const* format, ...)
{
	va_list list;
	va_start(list, format);

	if (is_main_thread())
	{
		c_static_string<255> message{};
		message.vprint(format, list);
		char const* message_string = message.get_string();

		terminal_printf(nullptr, message_string);
		c_console::write_line(message_string);

		if (console_dump_to_debug_display)
			display_debug_string(message_string);
	}

	va_end(list);
}

void __cdecl console_printf_color(real_argb_color const* color, char const* format, ...)
{
	va_list list;
	va_start(list, format);

	c_console::write_line(format, list);
	if (is_main_thread())
	{
		c_static_string<255> message{};
		message.vprint(format, list);
		char const* message_string = message.get_string();

		terminal_printf(color, message_string);
		c_console::write_line(message_string);

		if (console_dump_to_debug_display)
			display_debug_string(message_string);
	}

	va_end(list);
}

void __cdecl console_warning(char const* format, ...)
{
	va_list list;
	va_start(list, format);

	c_console::write_line(format, list);
	if (is_main_thread())
	{
		c_static_string<255> message{};
		message.vprint(format, list);
		char const* message_string = message.get_string();

		terminal_printf(global_real_argb_red, message_string);
		c_console::write_line(message_string);

		if (console_dump_to_debug_display)
			display_debug_string(message_string);
	}

	va_end(list);
}

void __cdecl console_initialize()
{
	static bool initialize_console = true;
	if (initialize_console)
	{
		terminal_initialize();

		console_globals.status_render = false;
		console_globals.input_state.prompt_color = { 1.0f, 1.0f, 0.3f, 1.0f };
		console_globals.input_state.prompt_text.set("donkey( ");
		console_globals.input_state.input_text[0] = 0;
		console_globals.input_state.__unknown11F4 = 0;
		console_globals.input_state.previous_inputs_count = NONE;
		console_globals.input_state.__unknown11F8 = NONE;

		debug_keys_initialize();

		initialize_console = false;
	}
}

void __cdecl console_dispose()
{
	static bool dispose_console = true;
	if (dispose_console)
	{
		console_close();
		debug_keys_dispose();
		terminal_dispose();

		dispose_console = false;
	}
}

bool __cdecl console_is_active()
{
	return console_globals.is_active;
}

bool __cdecl console_is_empty()
{
	return console_globals.is_active && !console_globals.input_state.input_text[0];
}

void __cdecl console_open(bool debug_menu)
{
	if (!console_is_active() && !debug_menu_get_active())
	{
		if (debug_menu)
		{
			debug_menu_open();
		}
		else
		{
			console_globals.input_state.input_text[0] = 0;
			console_globals.is_active = terminal_gets_begin(&console_globals.input_state);
		}
	}
}

void __cdecl console_close()
{
	if (console_is_active())
	{
		terminal_gets_end(&console_globals.input_state);
		console_globals.__time4 = 0.1f;
		console_globals.is_active = false;
	}
	else
	{
		debug_menu_close();
	}
}

void __cdecl console_clear()
{
	terminal_clear();
}

char* __cdecl console_get_token()
{
	char* input_text = strrchr(console_globals.input_state.input_text, ' ') + 1;
	char* v1 = strrchr(console_globals.input_state.input_text, '(') + 1;
	char* result = strrchr(console_globals.input_state.input_text, '"') + 1;

	if (console_globals.input_state.input_text > input_text)
		input_text = console_globals.input_state.input_text;

	if (input_text > v1)
		v1 = input_text;

	if (v1 > result)
		return v1;

	return result;
}

c_static_string<256> console_token_buffer;
long suggestion_current_index;

void __cdecl console_complete()
{
	bool something = false;
	if (console_token_buffer.is_empty())
	{
		console_token_buffer.set(console_get_token());
		console_token_buffer.set_length(console_globals.input_state.edit.cursor_selection_index);
		suggestion_current_index = NONE;

		something = true;
	}

	char* token = console_get_token();

	char const* matching_items[256]{};
	short matching_item_count = hs_tokens_enumerate(console_token_buffer.get_string(), NONE, matching_items, NUMBEROF(matching_items));
	if (matching_item_count)
	{
		ASSERT(matching_items[0]);

		short last_similar_character_index = SHRT_MAX;
		bool use_rows = matching_item_count > 16;

		c_static_string<1024> matching_item_row;
		matching_item_row.set("");

		if (something)
		{
			console_printf("");

			short matching_item_index = 0;
			char const** matching_item = matching_items;
			for (matching_item_index = 0; matching_item_index < matching_item_count; matching_item_index++, matching_item++)
			{
				short matching_item_length_minus_one = short(strlen(*matching_item)) - 1;
				if (last_similar_character_index > matching_item_length_minus_one)
					last_similar_character_index = matching_item_length_minus_one;

				short similar_character_index;
				for (similar_character_index = 0; ; similar_character_index++)
				{
					if (tolower(matching_items[0][similar_character_index]) != tolower(matching_items[matching_item_index][similar_character_index])
						|| similar_character_index > last_similar_character_index)
					{
						break;
					}
				}

				if (similar_character_index)
				{
					if (similar_character_index >= short(console_token_buffer.length()))
						last_similar_character_index = similar_character_index - 1;
				}

				if (use_rows)
				{
					matching_item_row.append_print("%s|t", *matching_item);
					if (matching_item_index % 6 == 5)
					{
						console_printf("%s", matching_item_row.get_string());
						matching_item_row.clear();
					}
				}
				else
				{
					console_printf("%s", *matching_item);
				}
			}

			if (use_rows && (matching_item_index - 1) % 6 != 5)
				console_printf("%s", matching_item_row.get_string());

			ASSERT(short(strlen(matching_items[0])) >= (last_similar_character_index + 1));

			csmemcpy(token, matching_items[0], last_similar_character_index + 1);
			token[last_similar_character_index + 1] = 0;

			console_globals.input_state.edit.cursor_selection_index = last_similar_character_index + short(token - console_globals.input_state.input_text + 1);

			suggestion_current_index = short(strlen(matching_items[0])) == last_similar_character_index + 1;
		}
		else if (suggestion_current_index == matching_item_count)
		{
			suggestion_current_index = 0;
			console_token_buffer.copy_to(token, k_terminal_gets_state_input_text_size);
			console_globals.input_state.edit.cursor_selection_index = word(console_token_buffer.length() + 1);
		}
		else
		{
			csmemcpy(token, matching_items[suggestion_current_index], strlen(matching_items[suggestion_current_index]));

			short suggestion_length = short(strlen(matching_items[suggestion_current_index++]));
			token[suggestion_length] = 0;

			console_globals.input_state.edit.cursor_selection_index = short(token - console_globals.input_state.input_text) + suggestion_length;
		}
	}
}

void __cdecl console_update(real shell_seconds_elapsed)
{
	PROFILER(console_update)
	{
		if (console_is_active())
		{
			for (long key_index = 0; key_index < console_globals.input_state.key_count; key_index++)
			{
				s_key_state* key = &console_globals.input_state.keys[key_index];
				ASSERT(key->key_code != NONE);

				if (key->key_type == _key_type_down && (key->key_code == _key_code_backquote || key->key_code == _key_code_f1))
				{
					console_close();
					break;
				}
				else if (key->key_type == _key_type_up && key->key_code == _key_code_tab)
				{
					console_complete();
				}
				else if (key->modifier.test(_key_modifier_flag_control_key_bit) && key->key_type == _key_type_up && key->key_code == _key_code_v)
				{
					char buffer[256]{};
					get_clipboard_as_text(buffer, NUMBEROF(buffer));
					csnzappendf(console_globals.input_state.input_text, NUMBEROF(console_globals.input_state.input_text), buffer);

					suggestion_current_index = 0;
					console_token_buffer.clear();
					break;
				}
				else if (key->key_type == _key_type_up && (key->key_code == _key_code_enter || key->key_code == _key_code_keypad_enter))
				{
					if (console_globals.input_state.input_text[0])
					{
						console_process_command(console_globals.input_state.input_text, true);
						console_globals.input_state.input_text[0] = 0;
						edit_text_selection_reset(&console_globals.input_state.edit);
					}
					break;
				}
				else if (key->key_type == _key_type_up && (key->key_code == _key_code_up || key->key_code == _key_code_down))
				{
					if (key->key_code == _key_code_up)
						console_globals.input_state.__unknown11F8 += 2;

					short v4 = console_globals.input_state.__unknown11F8 - 1;
					console_globals.input_state.__unknown11F8 = v4;

					if (v4 <= 0)
						console_globals.input_state.__unknown11F8 = 0;

					if (v4 <= 0)
						v4 = 0;

					if (v4 > console_globals.input_state.__unknown11F4 - 1)
					{
						v4 = console_globals.input_state.__unknown11F4 - 1;
						console_globals.input_state.__unknown11F8 = console_globals.input_state.__unknown11F4 - 1;
					}

					if (v4 != NONE)
					{
						decltype(console_globals.input_state.input_text)& input_text = console_globals.input_state.input_text;
						decltype(console_globals.input_state.previous_inputs)& previous_inputs = console_globals.input_state.previous_inputs;
						decltype(console_globals.input_state.previous_inputs_count)& previous_inputs_count = console_globals.input_state.previous_inputs_count;

						previous_inputs[(previous_inputs_count - v4 + NUMBEROF(previous_inputs)) % NUMBEROF(previous_inputs)].copy_to(input_text, NUMBEROF(input_text));
						edit_text_selection_reset(&console_globals.input_state.edit);
					}
					break;
				}
				else if (key->vk_code != NONE && key->key_type == _key_type_char)
				{
					csnzappendf(console_globals.input_state.input_text, NUMBEROF(console_globals.input_state.input_text), key->character);
					break;
				}
				else
				{
					suggestion_current_index = 0;
					console_token_buffer.clear();
				}
			}
		}
		else if (!debugging_system_has_focus())
		{
			s_key_state key{};
			if (input_peek_key(&key, _input_type_game))
			{
				if (!key.was_key_down && !key.modifier && key.key_type == _key_type_down && (key.key_code == _key_code_backquote || key.key_code == _key_code_f1))
				{
					input_get_key(&key, _input_type_game);
					console_open(false);
				}
			}

			debug_keys_update();
		}

		if ((console_globals.__time4 - shell_seconds_elapsed) >= 0.0f)
			console_globals.__time4 -= shell_seconds_elapsed;
		else
			console_globals.__time4 = 0.0f;
	}
}

char const* console_get_init_file_name(e_shell_application_type shell_application_type)
{
	switch (shell_application_type)
	{
	case _shell_application_type_editor:
		return "editor_init.txt";
	default:
		switch (shell_tool_type())
		{
		case _shell_tool_type_guerilla:
			return "guerilla_init.txt";
		case _shell_tool_type_tool:
			return "tool_init.txt";
		}
		return "init.txt";
	}

	return "";
}

FILE* console_open_init()
{
	FILE* file = nullptr;
	fopen_s(&file, console_get_init_file_name(shell_application_type()), "r");
	return file;
}

void console_execute_commands_from_file(FILE* file)
{
	if (file)
	{
		char buffer[200]{};
		while (fgets(buffer, NUMBEROF(buffer), file))
		{
			string_terminate_at_first_delimiter(buffer, "\r\n");
			console_process_command(buffer, false);
		}
	}
}

void __cdecl console_execute_initial_commands()
{
	if (FILE* file = console_open_init())
	{
		console_execute_commands_from_file(file);
		fclose(file);
	}
}

bool __cdecl console_process_command(char const* command, bool a2)
{
	if (strlen(command) >= 255)
		return false;

	if (!command[0] || command[0] == ';')
		return false;

	generate_event(_event_level_message, "console_command: %s", command);

	short command_index = (console_globals.input_state.previous_inputs_count + 1) % NUMBEROF(console_globals.input_state.previous_inputs);
	console_globals.input_state.previous_inputs_count = command_index;
	console_globals.input_state.previous_inputs[command_index].set(command);

	short v5 = NUMBEROF(console_globals.input_state.previous_inputs);
	if (console_globals.input_state.__unknown11F4 + 1 <= NUMBEROF(console_globals.input_state.previous_inputs))
		v5 = console_globals.input_state.__unknown11F4 + 1;
	console_globals.input_state.__unknown11F4 = v5;

	console_globals.input_state.__unknown11F8 = NONE;

	bool result = false;//hs_compile_and_evaluate(_event_level_message, "console_command", command, a2);

	tokens_t tokens{};
	long token_count = 0;
	command_tokenize(command, tokens, &token_count);
	if (token_count > 0)
	{
		char const* command_name = tokens[0]->get_string();

		bool command_found = false;
		for (long i = 0; i < NUMBEROF(k_registered_commands); i++)
		{
			if (tokens[0]->is_equal(k_registered_commands[i].name))
			{
				command_found = true;

				callback_result_t callback_result = k_registered_commands[i].callback(&k_registered_commands[i], token_count, tokens);

				c_console::write(callback_result.get_string());

				long succeeded = callback_result.index_of(": succeeded");
				result = succeeded != NONE || tokens[0]->is_equal("help");

				if (result)
					console_printf("command '%s' succeeded", command_name);
				else
					console_warning("command '%s' failed: %s", command_name, callback_result.get_string());

				return result;
			}
		}


		callback_result_t callback_result = set_callback(nullptr, token_count, tokens);
		if (callback_result.is_equal("success"))
			return true;

		if (callback_result.is_equal("failure"))
			return false;

		if (!command_found || callback_result.is_equal("not found"))
			console_warning("command '%s' not found", command_name);
	}

	return result;
}

#define CONSOLE_GLOBAL_DECLARE_BOOL(_name, ...)  new s_console_global({ .name = #_name, .type = _hs_type_boolean,       .pointer = &_name })
#define CONSOLE_GLOBAL_DECLARE_REAL(_name, ...)  new s_console_global({ .name = #_name, .type = _hs_type_real,          .pointer = &_name })
#define CONSOLE_GLOBAL_DECLARE_SHORT(_name, ...) new s_console_global({ .name = #_name, .type = _hs_type_short_integer, .pointer = &_name })
#define CONSOLE_GLOBAL_DECLARE_LONG(_name, ...)  new s_console_global({ .name = #_name, .type = _hs_type_long_integer,  .pointer = &_name })

#define CONSOLE_GLOBAL_DECLARE_BOOL2(_name, _variable_name, ...)  new s_console_global({ .name = #_name, .type = _hs_type_boolean,       .pointer = &_variable_name })
#define CONSOLE_GLOBAL_DECLARE_REAL2(_name, _variable_name, ...)  new s_console_global({ .name = #_name, .type = _hs_type_real,          .pointer = &_variable_name })
#define CONSOLE_GLOBAL_DECLARE_SHORT2(_name, _variable_name, ...) new s_console_global({ .name = #_name, .type = _hs_type_short_integer, .pointer = &_variable_name })
#define CONSOLE_GLOBAL_DECLARE_LONG2(_name, _variable_name, ...)  new s_console_global({ .name = #_name, .type = _hs_type_long_integer,  .pointer = &_variable_name })

s_console_global const* const k_console_globals[] =
{
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_no_drawing),

	CONSOLE_GLOBAL_DECLARE_BOOL2(terminal_render, g_terminal_render_enable),
	CONSOLE_GLOBAL_DECLARE_REAL2(director_camera_speed_scale, g_director_camera_speed_scale),

	CONSOLE_GLOBAL_DECLARE_REAL2(camera_fov, g_camera_globals.field_of_view),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_yaw_scale, g_camera_globals.yaw_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_pitch_scale, g_camera_globals.pitch_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_forward_scale, g_camera_globals.forward_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_side_scale, g_camera_globals.side_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_up_scale, g_camera_globals.up_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(flying_camera_maximum_boost_speed, g_camera_globals.maximum_boost_speed),
	CONSOLE_GLOBAL_DECLARE_REAL2(flying_camera_movement_delay, g_camera_globals.flying_cam_movement_delay),

	CONSOLE_GLOBAL_DECLARE_BOOL(display_framerate),
	CONSOLE_GLOBAL_DECLARE_BOOL(display_frame_deltas),

	CONSOLE_GLOBAL_DECLARE_BOOL(net_skip_countdown),
	CONSOLE_GLOBAL_DECLARE_BOOL(net_experimental),

	CONSOLE_GLOBAL_DECLARE_BOOL2(net_show_latency_and_framerate_metrics_on_chud, g_network_interface_show_latency_and_framerate_metrics_on_chud),
	CONSOLE_GLOBAL_DECLARE_BOOL2(net_fake_latency_and_framerate_metrics_on_chud, g_network_interface_fake_latency_and_framerate_metrics_on_chud),

	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_deathless_player, cheat.deathless_player),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_valhalla, cheat.valhalla),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_jetpack, cheat.jetpack),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_infinite_ammo, cheat.infinite_ammo),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_bottomless_clip, cheat.bottomless_clip),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_bump_possession, cheat.bump_possession),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_super_jump, cheat.super_jump),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_reflexive_damage_effects, cheat.reflexive_damage_effects),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_medusa, cheat.medusa),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_omnipotent, cheat.omnipotent),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_controller, cheat.controller),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_chevy, cheat.chevy),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_porcupine, cheat.porcupine),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_infinite_equipment_energy, cheat.infinite_equipment_energy),

	CONSOLE_GLOBAL_DECLARE_BOOL(console_dump_to_debug_display),
	CONSOLE_GLOBAL_DECLARE_BOOL2(console_pauses_game, debug_console_pauses_game),

	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_depth_render_scale_r),
	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_depth_render_scale_g),
	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_depth_render_scale_b),
	CONSOLE_GLOBAL_DECLARE_LONG(render_debug_depth_render),

	CONSOLE_GLOBAL_DECLARE_BOOL(render_default_lighting),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_trigger_volumes),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_pvs_activation),

	//CONSOLE_GLOBAL_DECLARE_LONG(debug_object_index),
	//CONSOLE_GLOBAL_DECLARE_LONG(debug_objects_type_mask),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_player_only),

	CONSOLE_GLOBAL_DECLARE_BOOL2(render_water, render_water_enabled),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_water_tessellated, render_water_tessellation_enabled),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_water_interaction, render_water_interaction_enabled),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_water_wireframe, render_water_wireframe_enabled),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_markers),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_surface_references),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_soft_ceilings),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_soft_kill),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_slip_surfaces),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_cluster_skies),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_invisible),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_automatic),
	//CONSOLE_GLOBAL_DECLARE_LONG(debug_plane_index),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_unique_colors),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_complexity),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_seam_edges),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_instanced_geometry),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_instanced_geometry_bounding_spheres),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_instanced_geometry_names),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_instanced_geometry_vertex_counts),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_instanced_geometry_collision_geometry),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_zone_set_critical_portals),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_structure_water),
	CONSOLE_GLOBAL_DECLARE_BOOL(water_physics_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_input),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_early_movers),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_sound_spheres),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_indices),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_programmer),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_garbage),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_names),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_full_names),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_active_nodes),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_animation_times),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_functions),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_position_velocity),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_origin),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_root_node),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_bounding_spheres),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_attached_bounding_spheres),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_dynamic_render_bounding_spheres),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_model_targets),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_collision_models),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_profile_times),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_water_physics),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_physics_models),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_contact_points),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_constraints),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_vehicle_physics),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_mass),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_pathfinding),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_node_bounds),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_animation),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_physics_control_node),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_biped_autoaim_pills),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_ground_plane),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_movement_mode),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_biped_throttle),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_pathfinding_surface),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_pendulum),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_vectors),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_seats),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_mouth_apeture),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_firing),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_acceleration),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_unit_camera),

	//CONSOLE_GLOBAL_DECLARE_BOOL(debug_sound_class_totals),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_sound_timing),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_duckers),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_sound_listeners),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_sound),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_sound_manager_channels),

	CONSOLE_GLOBAL_DECLARE_REAL2(havok_collision_tolerance, g_havok_constants.collision_tolerance),
	CONSOLE_GLOBAL_DECLARE_LONG2(havok_debug_mode, g_havok_constants.debug_mode),
	CONSOLE_GLOBAL_DECLARE_LONG2(havok_thread_count, g_havok_constants.thread_count),
	CONSOLE_GLOBAL_DECLARE_LONG2(havok_environment_type, g_havok_constants.environment_type),
	CONSOLE_GLOBAL_DECLARE_REAL2(havok_shape_radius, g_havok_constants.shape_radius),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_jumping_beans, g_havok_constants.jumping_beans),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_disable_deactivation, g_havok_constants.disable_deactivation),
	CONSOLE_GLOBAL_DECLARE_REAL2(havok_deactivation_reference_distance, g_havok_constants.deactivation_reference_distance),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_weld_environment, g_havok_constants.weld_environment),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_batch_add_entities_disabled, g_havok_constants.batch_add_entities_disabled),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_shape_cache, g_havok_constants.shape_cache),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_shape_cache_debug, g_havok_constants.shape_cache_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_enable_back_stepping, g_havok_constants.enable_back_stepping),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_enable_sweep_shapes, g_havok_constants.enable_sweep_shapes),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_display_stats, g_havok_constants.display_stats),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_initialize_profiling, g_havok_constants.initialize_profiling),
	CONSOLE_GLOBAL_DECLARE_BOOL2(impacts_debug, g_havok_constants.impacts_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL2(proxies_debug, g_havok_constants.proxies_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL2(collision_damage_debug, g_havok_constants.collision_damage_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_play_impact_effects, g_havok_constants.play_impact_effects),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_play_biped_impact_effects, g_havok_constants.play_biped_impact_effects),
	CONSOLE_GLOBAL_DECLARE_BOOL2(physics_debug, g_havok_constants.physics_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_cleanup_inactive_agents, g_havok_constants.cleanup_inactive_agents),

	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_memory_always_system, g_havok_memory_always_system),

	CONSOLE_GLOBAL_DECLARE_BOOL(contrail_render_enable),
	CONSOLE_GLOBAL_DECLARE_BOOL(soft_ceilings_disable),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cubemap_debug, c_cubemap_debug::g_render),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_camera_projection),

	CONSOLE_GLOBAL_DECLARE_REAL2(render_screenspace_center, g_screenspace_pixel_center),
	CONSOLE_GLOBAL_DECLARE_REAL2(ui_time_scale, g_ui_time_scale),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_thread_enable, g_render_thread_user_setting),

	CONSOLE_GLOBAL_DECLARE_BOOL2(game_paused, debug_pause_game),
	CONSOLE_GLOBAL_DECLARE_REAL2(game_speed, debug_game_speed),
	CONSOLE_GLOBAL_DECLARE_BOOL2(game_time_lock, debug_game_time_lock),
	CONSOLE_GLOBAL_DECLARE_BOOL2(game_time_statistics, debug_game_time_statistics),

};
long const k_console_global_count = NUMBEROF(k_console_globals);

bool string_to_boolean(char const* string, bool* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	bool const input = *value;
	if (IN_RANGE_INCLUSIVE(*string, '0', '1'))
	{
		*value = !!atol(string);
	}
	else if (csstricmp(string, "true") == 0)
	{
		*value = true;
	}
	else if (csstricmp(string, "false") == 0)
	{
		*value = false;
	}
	return input != *value;
}

bool string_to_real(char const* string, real* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	real const input = *value;
	*value = static_cast<real>(atof(string));
	return input != *value;
}

bool string_to_short_integer(char const* string, short* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	short const input = *value;
	*value = static_cast<short>(atol(string));
	return input != *value;
}

bool string_to_long_integer(char const* string, long* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	long const input = *value;
	*value = atol(string);
	return input != *value;
}

callback_result_t set_callback(void const* userdata, long token_count, tokens_t const tokens)
{
	ASSERT(token_count >= 1);

	static callback_result_t result = "not found";

	long console_global_index = NONE;
	for (long i = 0; i < k_console_global_count; i++)
	{
		if (!tokens[0]->is_equal(k_console_globals[i]->name))
			continue;

		console_global_index = i;
		break;
	}

	if (!VALID_INDEX(console_global_index, k_console_global_count))
		return result;

	s_console_global const* console_global = k_console_globals[console_global_index];

	if (!console_global->pointer)
		return result;

	char const* value_string = nullptr;
	if (token_count >= 2)
		value_string = tokens[1]->get_string();

	e_hs_type type = console_global->type;
	switch (type)
	{
	case _hs_type_boolean:
	{
		result = string_to_boolean(value_string, static_cast<bool*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, *static_cast<bool*>(console_global->pointer) ? "true" : "false");
	}
	break;
	case _hs_type_real:
	{
		result = string_to_real(value_string, static_cast<real*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, "%.6f", *static_cast<real*>(console_global->pointer));
	}
	break;
	case _hs_type_short_integer:
	{
		result = string_to_short_integer(value_string, static_cast<short*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, "%hd", *static_cast<short*>(console_global->pointer));
	}
	break;
	case _hs_type_long_integer:
	{
		result = string_to_long_integer(value_string, static_cast<long*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, "%d", *static_cast<long*>(console_global->pointer));
	}
	break;
	}

	return result;
}

