#include "main/console.hpp"

#include "ai/ai_debug.hpp"
#include "camera/camera_globals.hpp"
#include "cseries/cseries_events.hpp"
#include "cubemaps/cubemap_debug.hpp"
#include "effects/contrails.hpp"
#include "game/campaign_metagame.hpp"
#include "game/cheats.hpp"
#include "hs/hs_runtime.hpp"
#include "input/input_abstraction.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/chud/chud_draw.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/first_person_weapons.hpp"
#include "interface/gui_custom_bitmap_storage.hpp"
#include "interface/terminal.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_hs.hpp"
#include "items/weapons.hpp"
#include "main/debug_keys.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_render.hpp"
#include "main/main_time.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "motor/sync_action.hpp"
#include "multithreading/threads.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"
#include "networking/messages/network_message_gateway.hpp"
#include "networking/tools/remote_command.hpp"
#include "objects/damage.hpp"
#include "objects/object_types.hpp"
#include "physics/collision_debug.hpp"
#include "physics/havok.hpp"
#include "physics/water_physics.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/dx9/rasterizer_dx9_dynamic_geometry.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/old_render_debug.hpp"
#include "render/render.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug_commands.hpp"
#include "render/render_lights.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/render_transparents.hpp"
#include "render/render_water.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_player_view.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation_debug_globals.hpp"
#include "sound/sound_manager.hpp"
#include "text/draw_string.hpp"
#include "units/bipeds.hpp"
#include "visibility/visibility_collection.hpp"
#include "xbox/xbox.hpp"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

HOOK_DECLARE(0x00605E10, console_execute_initial_commands);

s_console_globals console_globals;

bool console_dump_to_debug_display = false;

bool* console_status_render = &console_globals.status_render;

c_static_string<256> console_token_buffer;
int32 suggestion_current_index;

s_status_line* g_status_line_head = NULL;
s_status_line* g_status_line_tail = NULL;

s_status_string g_status_strings[20];

s_status_line::s_status_line() :
	text(),
	flags()
{

}

s_status_string::s_status_string() :
	format_string(),
	line()
{
}

s_string_cache::s_string_cache() :
	string()
{
}

FILE* console_open_init_by_application_type(e_shell_application_type application_type)
{
	FILE* file = NULL;
	c_static_string<256> file_name;

	if (application_type == _shell_application_tool)
	{
		e_shell_tool_type tool_type = shell_tool_type();
		if (tool_type == _shell_tool_interactive)
		{
			file_name.print("guerilla_init.txt");
		}
		else if (tool_type == _shell_tool_command_line)
		{
			file_name.print("tool_init.txt");
		}
	}
	else if (application_type == _shell_application_editor)
	{
		file_name.print("editor_init.txt");
	}
	else
	{
		file_name.print("init.txt");
	}

	fopen_s(&file, file_name.get_string(), "r");
	return file;
}

FILE* console_open_init()
{
	return console_open_init_by_application_type(shell_application_type());
}

char* __cdecl console_get_token()
{
	char* input_text = strrchr(console_globals.input_state.result, ' ') + 1;
	char* v1 = strrchr(console_globals.input_state.result, '(') + 1;
	char* result = strrchr(console_globals.input_state.result, '"') + 1;

	if (console_globals.input_state.result > input_text)
		input_text = console_globals.input_state.result;

	if (input_text > v1)
		v1 = input_text;

	if (v1 > result)
		return v1;

	return result;
}

void __cdecl console_clear()
{
	terminal_clear();
}

void __cdecl console_close()
{
	if (console_is_active())
	{
		terminal_gets_end(&console_globals.input_state);
		console_globals.open_timeout_seconds = 0.1f;
		console_globals.active = false;
	}
	else
	{
		debug_menu_close();
	}
}

void __cdecl console_complete()
{
	bool something = false;
	if (console_token_buffer.is_empty())
	{
		console_token_buffer.set(console_get_token());
		console_token_buffer.set_length(console_globals.input_state.edit.insertion_point_index);
		suggestion_current_index = NONE;

		something = true;
	}

	char* token = console_get_token();

	const char* matching_items[256]{};
	int16 matching_item_count = hs_tokens_enumerate(console_token_buffer.get_string(), NONE, matching_items, NUMBEROF(matching_items));
	if (matching_item_count)
	{
		ASSERT(matching_items[0]);

		int16 last_similar_character_index = SHRT_MAX;
		bool use_rows = matching_item_count > 16;

		c_static_string<1024> matching_item_row;
		matching_item_row.set("");

		if (something)
		{
			console_printf("");

			int16 matching_item_index = 0;
			const char** matching_item = matching_items;
			for (matching_item_index = 0; matching_item_index < matching_item_count; matching_item_index++, matching_item++)
			{
				int16 matching_item_length_minus_one = int16(strlen(*matching_item)) - 1;
				if (last_similar_character_index > matching_item_length_minus_one)
					last_similar_character_index = matching_item_length_minus_one;

				int16 similar_character_index;
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
					if (similar_character_index >= int16(console_token_buffer.length()))
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

			ASSERT(int16(strlen(matching_items[0])) >= (last_similar_character_index + 1));

			csmemcpy(token, matching_items[0], last_similar_character_index + 1);
			token[last_similar_character_index + 1] = 0;

			console_globals.input_state.edit.insertion_point_index = last_similar_character_index + int16(token - console_globals.input_state.result + 1);

			suggestion_current_index = int16(strlen(matching_items[0])) == last_similar_character_index + 1;
		}
		else if (suggestion_current_index == matching_item_count)
		{
			suggestion_current_index = 0;
			console_token_buffer.copy_to(token, k_terminal_gets_state_input_text_size);
			console_globals.input_state.edit.insertion_point_index = uns16(console_token_buffer.length() + 1);
		}
		else
		{
			csmemcpy(token, matching_items[suggestion_current_index], strlen(matching_items[suggestion_current_index]));

			int16 suggestion_length = int16(strlen(matching_items[suggestion_current_index++]));
			token[suggestion_length] = 0;

			console_globals.input_state.edit.insertion_point_index = int16(token - console_globals.input_state.result) + suggestion_length;
		}
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

void __cdecl console_execute_initial_commands()
{
	if (FILE* file = console_open_init())
	{
		char buffer[200]{};
		while (fgets(buffer, NUMBEROF(buffer), file))
		{
			string_terminate_at_first_delimiter(buffer, "\r\n");
			console_process_command(buffer, false);
		}
		fclose(file);
	}
}

void __cdecl console_initialize()
{
	static bool initialize_console = true;
	if (initialize_console)
	{
		terminal_initialize();

		console_globals.status_render = true;
		console_globals.input_state.color = { 1.0f, 1.0f, 0.3f, 1.0f };
		console_globals.input_state.prompt.set("donkey( ");
		console_globals.input_state.result[0] = 0;
		console_globals.previous_command_count = 0;
		console_globals.newest_previous_command_index = NONE;
		console_globals.selected_previous_command_index = NONE;

		debug_keys_initialize();

		for (int32 i = 0; i < NUMBEROF(g_status_strings); i++)
		{
			status_lines_initialize(&g_status_strings[i].line, NULL, 1);
			g_status_strings[i].line.flags.set(_status_line_left_justify_bit, true);
		}

		initialize_console = false;
	}
}

bool __cdecl console_is_active()
{
	return console_globals.active;
}

bool __cdecl console_is_empty()
{
	return console_globals.active && !console_globals.input_state.result[0];
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
			console_globals.input_state.result[0] = 0;
			console_globals.active = terminal_gets_begin(&console_globals.input_state);
		}
	}
}

void __cdecl console_printf(const char* format, ...)
{
	va_list list;
	va_start(list, format);

	if (is_main_thread())
	{
		c_static_string<255> message{};
		message.print_va(format, list);
		const char* message_string = message.get_string();

		terminal_printf(nullptr, message_string);
		c_console::write_line(message_string);

		if (console_dump_to_debug_display)
			display_debug_string(message_string);
	}

	va_end(list);
}

void __cdecl console_printf_color(const real_argb_color* color, const char* format, ...)
{
	va_list list;
	va_start(list, format);

	c_console::write_line(format, list);
	if (is_main_thread())
	{
		c_static_string<255> message{};
		message.print_va(format, list);
		const char* message_string = message.get_string();

		terminal_printf(color, message_string);
		c_console::write_line(message_string);

		if (console_dump_to_debug_display)
			display_debug_string(message_string);
	}

	va_end(list);
}

void __cdecl console_update(real32 shell_seconds_elapsed)
{
	PROFILER(console_update)
	{
		if (console_is_active())
		{
			for (int32 key_index = 0; key_index < console_globals.input_state.key_count; key_index++)
			{
				s_key_state* key = &console_globals.input_state.keys[key_index];
				ASSERT(key->ascii_code != _key_not_a_key);

				if (key->key_type == _key_type_down && (key->ascii_code == _key_backquote || key->ascii_code == _key_f1))
				{
					console_close();
					break;
				}
				else if (key->key_type == _key_type_up && key->ascii_code == _key_tab)
				{
					console_complete();
				}
				else if (TEST_BIT(key->modifier_flags, _key_modifier_flag_control_key_bit) && key->key_type == _key_type_up && key->ascii_code == _key_v)
				{
					char buffer[256]{};
					get_clipboard_as_text(buffer, NUMBEROF(buffer));
					csnzappendf(console_globals.input_state.result, NUMBEROF(console_globals.input_state.result), buffer);

					suggestion_current_index = 0;
					console_token_buffer.clear();
					break;
				}
				else if (key->key_type == _key_type_up && (key->ascii_code == _key_return || key->ascii_code == _keypad_enter))
				{
					if (console_globals.input_state.result[0])
					{
						console_process_command(console_globals.input_state.result, true);
						console_globals.input_state.result[0] = 0;
						edit_text_selection_reset(&console_globals.input_state.edit);
					}
					break;
				}
				else if (key->key_type == _key_type_up && (key->ascii_code == _key_up_arrow || key->ascii_code == _key_down_arrow))
				{
					if (key->ascii_code == _key_up_arrow)
						console_globals.selected_previous_command_index += 2;

					int16 v4 = console_globals.selected_previous_command_index - 1;
					console_globals.selected_previous_command_index = v4;

					if (v4 <= 0)
						console_globals.selected_previous_command_index = 0;

					if (v4 <= 0)
						v4 = 0;

					if (v4 > console_globals.previous_command_count - 1)
					{
						v4 = console_globals.previous_command_count - 1;
						console_globals.selected_previous_command_index = console_globals.previous_command_count - 1;
					}

					if (v4 != NONE)
					{
						decltype(console_globals.input_state.result)& input_text = console_globals.input_state.result;
						decltype(console_globals.previous_commands)& previous_commands = console_globals.previous_commands;
						decltype(console_globals.newest_previous_command_index)& newest_previous_command_index = console_globals.newest_previous_command_index;

						previous_commands[(newest_previous_command_index - v4 + NUMBEROF(previous_commands)) % NUMBEROF(previous_commands)].copy_to(input_text, NUMBEROF(input_text));
						edit_text_selection_reset(&console_globals.input_state.edit);
					}
					break;
				}
				else if (key->vk_code != NONE && key->key_type == _key_type_char)
				{
					csnzappendf(console_globals.input_state.result, NUMBEROF(console_globals.input_state.result), key->character);
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
				if (!key.repeating && !key.modifier_flags && key.key_type == _key_type_down && (key.ascii_code == _key_tilde || key.ascii_code == _key_f1))
				{
					input_get_key(&key, _input_type_game);
					console_open(false);
				}
			}

			debug_keys_update();
		}

		if ((console_globals.open_timeout_seconds - shell_seconds_elapsed) >= 0.0f)
			console_globals.open_timeout_seconds -= shell_seconds_elapsed;
		else
			console_globals.open_timeout_seconds = 0.0f;
	}
}

void __cdecl console_warning(const char* format, ...)
{
	va_list list;
	va_start(list, format);

	c_console::write_line(format, list);
	if (is_main_thread())
	{
		c_static_string<255> message{};
		message.print_va(format, list);
		const char* message_string = message.get_string();

		terminal_printf(global_real_argb_red, message_string);
		c_console::write_line(message_string);

		if (console_dump_to_debug_display)
			display_debug_string(message_string);
	}

	va_end(list);
}

bool __cdecl console_process_command(const char* command, bool interactive)
{
	if (strlen(command) >= 255)
		return false;

	if (!command[0] || command[0] == ';')
		return false;

	main_status("console_command", "%s", command);

	int16 command_index = (console_globals.newest_previous_command_index + 1) % NUMBEROF(console_globals.previous_commands);
	console_globals.newest_previous_command_index = command_index;
	console_globals.previous_commands[command_index].set(command);

	int16 v5 = NUMBEROF(console_globals.previous_commands);
	if (console_globals.previous_command_count + 1 <= NUMBEROF(console_globals.previous_commands))
		v5 = console_globals.previous_command_count + 1;
	console_globals.previous_command_count = v5;

	console_globals.selected_previous_command_index = NONE;

	bool result = false;//hs_compile_and_evaluate(_event_message, "console_command", command, interactive);

	tokens_t tokens{};
	int32 token_count = 0;
	command_tokenize(command, tokens, &token_count);
	if (token_count > 0)
	{
		const char* command_name = tokens[0]->get_string();

		bool command_found = false;
		for (int32 i = 0; i < NUMBEROF(k_registered_commands); i++)
		{
			if (tokens[0]->is_equal(k_registered_commands[i].name))
			{
				command_found = true;

				callback_result_t callback_result = k_registered_commands[i].callback(&k_registered_commands[i], token_count, tokens);

				c_console::write(callback_result.get_string());

				int32 succeeded = callback_result.index_of(": succeeded");
				result = succeeded != NONE || tokens[0]->is_equal("help");

				if (result)
					console_printf("command '%s' succeeded", command_name);
				else
					console_warning("command '%s' failed: %s", command_name, callback_result.get_string());

				return result;
			}
		}

#if defined(ALLOW_CONSOLE_SCRIPT_HS_SCRIPT_BY_NAME)
		if (user_interface_start_hs_script_by_name(command_name) != NONE)
			return true;
#endif

		if (token_count == 2 && load_preference(tokens[0]->get_string(), tokens[1]->get_string()))
			return true;

		callback_result_t callback_result = set_callback(nullptr, token_count, tokens);
		if (callback_result.is_equal("success"))
			return true;

		if (callback_result.is_equal("failure"))
			return false;

		if (!command_found || callback_result.is_equal("not found"))
			console_warning("command '%s' not found", command_name);
	}

	main_status("console_command", NULL);

	return result;
}

bool __cdecl debugging_system_has_focus()
{
	return console_is_active() || debug_menu_get_active();
}

#define CONSOLE_GLOBAL_DECLARE_BOOL(_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_boolean,       .pointer = &_name })
#define CONSOLE_GLOBAL_DECLARE_REAL(_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_real,          .pointer = &_name })
#define CONSOLE_GLOBAL_DECLARE_SHORT(_name, ...) s_console_global({ .name = #_name, .type = _hs_type_short_integer, .pointer = &_name })
#define CONSOLE_GLOBAL_DECLARE_LONG(_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_long_integer,  .pointer = &_name })

#define CONSOLE_GLOBAL_DECLARE_BOOL2(_name, _variable_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_boolean,       .pointer = &_variable_name })
#define CONSOLE_GLOBAL_DECLARE_REAL2(_name, _variable_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_real,          .pointer = &_variable_name })
#define CONSOLE_GLOBAL_DECLARE_SHORT2(_name, _variable_name, ...) s_console_global({ .name = #_name, .type = _hs_type_short_integer, .pointer = &_variable_name })
#define CONSOLE_GLOBAL_DECLARE_LONG2(_name, _variable_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_long_integer,  .pointer = &_variable_name })

#define CONSOLE_GLOBAL_DECLARE_BOOL3(_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_boolean,       .pointer = _name })
#define CONSOLE_GLOBAL_DECLARE_REAL3(_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_real,          .pointer = _name })
#define CONSOLE_GLOBAL_DECLARE_SHORT3(_name, ...) s_console_global({ .name = #_name, .type = _hs_type_short_integer, .pointer = _name })
#define CONSOLE_GLOBAL_DECLARE_LONG3(_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_long_integer,  .pointer = _name })

#define CONSOLE_GLOBAL_DECLARE_BOOL4(_name, _variable_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_boolean,       .pointer = _variable_name })
#define CONSOLE_GLOBAL_DECLARE_REAL4(_name, _variable_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_real,          .pointer = _variable_name })
#define CONSOLE_GLOBAL_DECLARE_SHORT4(_name, _variable_name, ...) s_console_global({ .name = #_name, .type = _hs_type_short_integer, .pointer = _variable_name })
#define CONSOLE_GLOBAL_DECLARE_LONG4(_name, _variable_name, ...)  s_console_global({ .name = #_name, .type = _hs_type_long_integer,  .pointer = _variable_name })

REFERENCE_DECLARE(0x01917CED, bool, g_enable_better_cpu_gpu_sync_hs_setting);

s_console_global const k_console_globals[] =
{
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_no_drawing),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_force_all_player_views_to_default_player),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_render_freeze),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_render_horizontal_splitscreen),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_load_panic_to_main_menu),
	CONSOLE_GLOBAL_DECLARE_BOOL(display_framerate),
	//CONSOLE_GLOBAL_DECLARE_BOOL(display_pulse_rates),
	//CONSOLE_GLOBAL_DECLARE_BOOL(display_throttle_rates),
	//CONSOLE_GLOBAL_DECLARE_BOOL(display_lag_times),
	CONSOLE_GLOBAL_DECLARE_BOOL(display_frame_deltas),
	CONSOLE_GLOBAL_DECLARE_BOOL4(console_status_string_render, console_status_render),
	CONSOLE_GLOBAL_DECLARE_BOOL2(console_pauses_game, debug_console_pauses_game),
	//CONSOLE_GLOBAL_DECLARE_BOOL(framerate_infinite),
	//CONSOLE_GLOBAL_DECLARE_BOOL(framerate_debug),
	//CONSOLE_GLOBAL_DECLARE_BOOL(framerate_use_system_time),
	//CONSOLE_GLOBAL_DECLARE_BOOL(framerate_stabilization),
	//CONSOLE_GLOBAL_DECLARE_BOOL(debug_controller_latency),
	//CONSOLE_GLOBAL_DECLARE_BOOL(debug_physical_memory),
	CONSOLE_GLOBAL_DECLARE_BOOL2(terminal_render, g_terminal_render_enable),
	//CONSOLE_GLOBAL_DECLARE_BOOL(events_debug_spam_render),
	CONSOLE_GLOBAL_DECLARE_BOOL(console_dump_to_debug_display),

	CONSOLE_GLOBAL_DECLARE_REAL2(director_camera_speed_scale, g_director_camera_speed_scale),

	CONSOLE_GLOBAL_DECLARE_REAL2(camera_global_fov, g_camera_globals.camera_field_of_view_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_yaw_scale, g_camera_globals.camera_yaw_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_pitch_scale, g_camera_globals.camera_pitch_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_forward_scale, g_camera_globals.camera_forward_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_side_scale, g_camera_globals.camera_side_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(camera_up_scale, g_camera_globals.camera_up_scale),
	CONSOLE_GLOBAL_DECLARE_REAL2(flying_camera_maximum_boost_speed, g_camera_globals.flying_camera_time_to_maximum_boost),
	CONSOLE_GLOBAL_DECLARE_REAL2(flying_camera_movement_delay, g_camera_globals.flying_cam_movement_delay),

	CONSOLE_GLOBAL_DECLARE_BOOL(net_skip_countdown),
	CONSOLE_GLOBAL_DECLARE_BOOL(net_experimental),

	CONSOLE_GLOBAL_DECLARE_BOOL2(net_show_latency_and_framerate_metrics_on_chud, g_network_interface_show_latency_and_framerate_metrics_on_chud),
	CONSOLE_GLOBAL_DECLARE_BOOL2(net_fake_latency_and_framerate_metrics_on_chud, g_network_interface_fake_latency_and_framerate_metrics_on_chud),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_enable_force_phonebooth_assassinate),

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
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_controller, cheat.controller_enabled),
	CONSOLE_GLOBAL_DECLARE_BOOL2(cheat_chevy, cheat.chevy),

	CONSOLE_GLOBAL_DECLARE_BOOL(render_debug_show_air_probes),
	CONSOLE_GLOBAL_DECLARE_LONG(render_debug_toggle_default_lightmaps_texaccum),
	CONSOLE_GLOBAL_DECLARE_BOOL(render_debug_toggle_default_static_lighting),
	CONSOLE_GLOBAL_DECLARE_BOOL(render_debug_toggle_default_dynamic_lighting),
	CONSOLE_GLOBAL_DECLARE_BOOL(render_debug_toggle_default_sfx),

	CONSOLE_GLOBAL_DECLARE_LONG(render_debug_depth_render),
	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_depth_render_scale_r),
	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_depth_render_scale_g),
	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_depth_render_scale_b),

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

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_damage),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_damage_verbose),

	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_water_proxy),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_spray),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_features),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_phantom_bsp),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_lightmaps),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_geometry_sampling),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flags),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_structure),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_water),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_instanced_geometry),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_bipeds),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_giants),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_effect_scenery),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_vehicles),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_weapons),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_equipment),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_terminals),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_projectiles),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_scenery),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_machines),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_controls),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_sound_scenery),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_crates),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_objects_creatures),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_child_objects),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_nonpathfindable_objects),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_cinematic_objects),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_dead_bipeds),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_passthrough_bipeds),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_front_facing_surfaces),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_back_facing_surfaces),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_two_sided_surfaces),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_invisible_surfaces),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_ignore_breakable_surfaces),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_allow_early_out),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_flag_try_to_keep_location_valid),
	CONSOLE_GLOBAL_DECLARE_BOOL(collision_debug_repeat),
	CONSOLE_GLOBAL_DECLARE_REAL2(collision_debug_point_x, collision_debug_point.x),
	CONSOLE_GLOBAL_DECLARE_REAL2(collision_debug_point_y, collision_debug_point.y),
	CONSOLE_GLOBAL_DECLARE_REAL2(collision_debug_point_z, collision_debug_point.z),
	CONSOLE_GLOBAL_DECLARE_REAL2(collision_debug_vector_i, collision_debug_vector.i),
	CONSOLE_GLOBAL_DECLARE_REAL2(collision_debug_vector_j, collision_debug_vector.j),
	CONSOLE_GLOBAL_DECLARE_REAL2(collision_debug_vector_k, collision_debug_vector.k),
	CONSOLE_GLOBAL_DECLARE_REAL(collision_debug_length),
	CONSOLE_GLOBAL_DECLARE_REAL(collision_debug_width),
	CONSOLE_GLOBAL_DECLARE_REAL(collision_debug_height),
	CONSOLE_GLOBAL_DECLARE_LONG(collision_debug_ignore_object_index),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_camera),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_tangent_space),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_player),
	//CONSOLE_GLOBAL_DECLARE_BOOL(debug_player_control_autoaim_always_active),

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
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_input_abstraction),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_input_mouse_state),

	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_profile_disable, ai_profile.disable_ai),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_profile_random, ai_profile.move_randomly),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show, ai_profile.show),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_stats, ai_profile.show_stats),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_actors, ai_profile.show_actors),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_swarms, ai_profile.show_swarms),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_paths, ai_profile.show_paths),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_line_of_sight, ai_profile.show_line_of_sight),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_prop_types, ai_profile.show_prop_types),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_show_sound_distance, ai_profile.show_sound_distance),

	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render, g_ai_render),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_dialogue_player_weights, g_ai_render_dialogue_player_weights),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_behavior_stack_all, g_ai_render_behavior_stack_all),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_character_names, g_ai_render_character_names),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_stimuli, g_ai_render_stimuli),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_sectors, g_ai_render_sectors),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_object_properties, g_ai_render_object_properties),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_intersection_links, g_ai_render_intersection_links),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_threshold_links, g_ai_render_threshold_links),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_suppress_combat, g_ai_render_suppress_combat),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_vehicle_reservations, g_ai_render_vehicle_reservations),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_objectives, g_ai_render_objectives),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_strength, g_ai_render_strength),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_debug_tracking_data, g_ai_debug_tracking_data),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_debug_perception_data, g_ai_debug_perception_data),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_debug_combat_status, g_ai_debug_combat_status),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_tracked_props_all, g_ai_render_tracked_props_all),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_targets_all, g_ai_render_targets_all),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_command_scripts, g_ai_render_command_scripts),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_dialogue_variants, g_ai_render_dialogue_variants),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_dynamic_firing_positions, g_ai_render_dynamic_firing_positions),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_vehicle_interest, g_ai_render_vehicle_interest),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_player_battle_vector, g_ai_render_player_battle_vector),
	CONSOLE_GLOBAL_DECLARE_BOOL2(ai_render_player_needs_vehicle, g_ai_render_player_needs_vehicle),

	CONSOLE_GLOBAL_DECLARE_REAL(water_physics_velocity_minimum),
	CONSOLE_GLOBAL_DECLARE_REAL(water_physics_velocity_maximum),

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
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_objects_expensive_physics),
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

	CONSOLE_GLOBAL_DECLARE_REAL2(havok_collision_tolerance, g_havok_constants.havok_collision_tolerance),
	CONSOLE_GLOBAL_DECLARE_LONG2(havok_debug_mode, g_havok_constants.havok_debug_mode),
	CONSOLE_GLOBAL_DECLARE_LONG2(havok_thread_count, g_havok_constants.havok_thread_count),
	CONSOLE_GLOBAL_DECLARE_LONG2(havok_environment_type, g_havok_constants.havok_environment_type),
	CONSOLE_GLOBAL_DECLARE_REAL2(havok_shape_radius, g_havok_constants.havok_shape_radius),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_jumping_beans, g_havok_constants.havok_jumping_beans),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_disable_deactivation, g_havok_constants.havok_disable_deactivation),
	CONSOLE_GLOBAL_DECLARE_REAL2(havok_deactivation_reference_distance, g_havok_constants.havok_deactivation_reference_distance),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_weld_environment, g_havok_constants.havok_weld_environment),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_batch_add_entities_disabled, g_havok_constants.havok_batch_add_entities_disabled),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_shape_cache, g_havok_constants.havok_shape_cache),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_shape_cache_debug, g_havok_constants.havok_shape_cache_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_enable_back_stepping, g_havok_constants.havok_enable_back_stepping),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_enable_sweep_shapes, g_havok_constants.havok_enable_sweep_shapes),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_display_stats, g_havok_constants.havok_display_stats),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_initialize_profiling, g_havok_constants.havok_initialize_profiling),
	CONSOLE_GLOBAL_DECLARE_BOOL2(impacts_debug, g_havok_constants.havok_render_impacts),
	CONSOLE_GLOBAL_DECLARE_BOOL2(proxies_debug, g_havok_constants.havok_render_proxies),
	CONSOLE_GLOBAL_DECLARE_BOOL2(collision_damage_debug, g_havok_constants.havok_render_collision_damage),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_play_impact_effects, g_havok_constants.havok_play_impact_effects),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_play_biped_impact_effects, g_havok_constants.havok_play_biped_impact_effects),
	CONSOLE_GLOBAL_DECLARE_BOOL2(physics_debug, g_havok_constants.havok_render_shape_properties),
	CONSOLE_GLOBAL_DECLARE_BOOL2(havok_cleanup_inactive_agents, g_havok_constants.havok_cleanup_inactive_agents),

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

	CONSOLE_GLOBAL_DECLARE_BOOL(chud_enabled),
	CONSOLE_GLOBAL_DECLARE_BOOL(chud_debug_grid),
	CONSOLE_GLOBAL_DECLARE_BOOL(chud_debug_messages),
	//CONSOLE_GLOBAL_DECLARE_BOOL(chud_cortana_debug),
	CONSOLE_GLOBAL_DECLARE_BOOL(chud_debug_crosshair),
	CONSOLE_GLOBAL_DECLARE_BOOL(chud_debug_metagame),

	CONSOLE_GLOBAL_DECLARE_BOOL2(game_time_lock, debug_game_time_lock),
	CONSOLE_GLOBAL_DECLARE_BOOL2(game_time_statistics, debug_game_time_statistics),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_game_save),
	//CONSOLE_GLOBAL_DECLARE_BOOL(recover_saved_games_hack),
	//CONSOLE_GLOBAL_DECLARE_BOOL(game_state_verify_on_write),
	//CONSOLE_GLOBAL_DECLARE_BOOL(game_state_verify_on_read),

	CONSOLE_GLOBAL_DECLARE_BOOL2(static_fp_fov, debug_static_first_person),
	CONSOLE_GLOBAL_DECLARE_BOOL2(centered_crosshair, controller_centered_crosshair),

	CONSOLE_GLOBAL_DECLARE_BOOL2(enable_better_cpu_gpu_sync, g_enable_better_cpu_gpu_sync_hs_setting),
	CONSOLE_GLOBAL_DECLARE_BOOL2(reduce_widescreen_fov_during_cinematics, g_reduce_widescreen_fov_during_cinematics),
	CONSOLE_GLOBAL_DECLARE_REAL(render_debug_aspect_ratio_scale),
	CONSOLE_GLOBAL_DECLARE_BOOL(render_debug_force_4x3_aspect_ratio),
	CONSOLE_GLOBAL_DECLARE_SHORT(render_debug_transparent_sort_method),
	CONSOLE_GLOBAL_DECLARE_BOOL(render_debug_pix_events),
	CONSOLE_GLOBAL_DECLARE_BOOL(render_pc_specular),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_pc_albedo_lighting, c_render_globals::m_render_pc_albedo_lighting),

	CONSOLE_GLOBAL_DECLARE_BOOL2(mean_look_sensitivity, use_mean_look_sensitivity),

	CONSOLE_GLOBAL_DECLARE_LONG2(watermark_enabled, g_watermark_enabled),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_skulls),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_weapons),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_weapons_triggers),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_weapons_barrels),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_weapons_magazines),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_weapons_primary),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_weapons_secondary),

	CONSOLE_GLOBAL_DECLARE_BOOL(debug_animation_fp_sprint_disable),
	CONSOLE_GLOBAL_DECLARE_BOOL(debug_first_person_skeleton),

	CONSOLE_GLOBAL_DECLARE_BOOL2(render_postprocess, c_screen_postprocess::x_editable_settings.m_postprocess),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_accum, c_screen_postprocess::x_editable_settings.m_accum),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_bloom_source, c_screen_postprocess::x_editable_settings.m_bloom_source),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_persist, c_screen_postprocess::x_editable_settings.m_persist),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_bloom, c_screen_postprocess::x_editable_settings.m_bloom),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_bling, c_screen_postprocess::x_editable_settings.m_bling),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_downsample, c_screen_postprocess::x_editable_settings.m_downsample),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_show_alpha, c_screen_postprocess::x_editable_settings.m_display_alpha),
	CONSOLE_GLOBAL_DECLARE_REAL2(render_postprocess_exposure, c_screen_postprocess::x_editable_settings.m_postprocess_exposure),
	CONSOLE_GLOBAL_DECLARE_LONG2(render_accum_filter, c_screen_postprocess::x_editable_settings.m_accum_filter),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_tone_curve, c_screen_postprocess::x_editable_settings.m_tone_curve),
	CONSOLE_GLOBAL_DECLARE_REAL2(render_tone_curve_white, c_screen_postprocess::x_editable_settings.m_tone_curve_white_point),
	CONSOLE_GLOBAL_DECLARE_BOOL2(render_exposure_lock, c_screen_postprocess::x_editable_settings.m_auto_exposure_lock),

	CONSOLE_GLOBAL_DECLARE_BOOL2(gui_custom_bitmaps_enabled, g_gui_custom_bitmaps_enabled),

	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_text_bounds_global),
	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_bitmap_bounds_global),
	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_model_bounds_global),
	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_list_item_bounds_global),
	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_list_bounds_global),
	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_group_bounds_global),
	CONSOLE_GLOBAL_DECLARE_BOOL(gui_debug_screen_bounds_global),

};
int32 const k_console_global_count = NUMBEROF(k_console_globals);

bool string_to_boolean(const char* string, bool* value)
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

bool string_to_real(const char* string, real32* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	real32 const input = *value;
	*value = static_cast<real32>(atof(string));
	return input != *value;
}

bool string_to_short_integer(const char* string, int16* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	int16 const input = *value;
	*value = static_cast<int16>(atol(string));
	return input != *value;
}

bool string_to_long_integer(const char* string, int32* value)
{
	if (!string)
		return true;

	if (!value)
		return false;

	int32 const input = *value;
	*value = atol(string);
	return input != *value;
}

callback_result_t set_callback(const void* userdata, int32 token_count, tokens_t const tokens)
{
	ASSERT(token_count >= 1);

	static callback_result_t result = "not found";

	int32 console_global_index = NONE;
	for (int32 i = 0; i < k_console_global_count; i++)
	{
		if (!tokens[0]->is_equal(k_console_globals[i].name))
		{
			continue;
		}

		console_global_index = i;
		break;
	}

	if (!VALID_INDEX(console_global_index, k_console_global_count))
	{
		return result;
	}

	const s_console_global* console_global = &k_console_globals[console_global_index];

	if (!console_global->pointer)
	{
		return result;
	}

	const char* value_string = nullptr;
	if (token_count >= 2)
	{
		value_string = tokens[1]->get_string();
	}

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
		result = string_to_real(value_string, static_cast<real32*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, "%.6f", *static_cast<real32*>(console_global->pointer));
	}
	break;
	case _hs_type_short_integer:
	{
		result = string_to_short_integer(value_string, static_cast<int16*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, "%hd", *static_cast<int16*>(console_global->pointer));
	}
	break;
	case _hs_type_long_integer:
	{
		result = string_to_long_integer(value_string, static_cast<int32*>(console_global->pointer)) ? "success" : "failure";
		terminal_printf(global_real_argb_white, "%d", *static_cast<int32*>(console_global->pointer));
	}
	break;
	}

	return result;
}

void status_line_add_single(s_status_line* inStatusLine)
{
	ASSERT(NULL == inStatusLine->prev);
	ASSERT(NULL == inStatusLine->next);

	inStatusLine->next = NULL;
	inStatusLine->prev = g_status_line_tail;
	
	if (g_status_line_tail)
		g_status_line_tail->next = inStatusLine;
	else
		g_status_line_head = inStatusLine;

	if (inStatusLine->next)
		inStatusLine->next->prev = inStatusLine;
	else
		g_status_line_tail = inStatusLine;
}

void status_line_draw()
{
	PROFILER(status_line_draw)
	{
		if (!console_globals.status_render/* || !can_use_claws()*/)
			return;

		for (int32 i = 0; i < NUMBEROF(g_status_strings); i++)
		{
			s_status_line& status_line = g_status_strings[i].line;
			if (!status_line.text.is_empty())
			{
				uns32 time = system_milliseconds();
				int32 time_delta = time - g_status_strings[i].time_created;
				if (time_delta > 10000)
				{
					status_line.text.clear();
				}
				else
				{
					if (time_delta > 5000)
					{
						time = time_delta - 5000;
						status_line.alpha = 1.0f - (real32(time) / 5000);
					}
					else
					{
						status_line.alpha = 1.0f;
					}
				}
			}
		}

		c_rasterizer_draw_string draw_string{};
		c_font_cache_mt_safe font_cache{};
		s_string_cache string_cache{};

		string_cache.string.clear();

		for (s_status_line* status_line = g_status_line_head; status_line; status_line = status_line->next)
		{
			if (!status_line_visible(status_line))
				continue;

			e_text_justification justification = e_text_justification(!status_line->flags.test(_status_line_left_justify_bit));

			const char* string = status_line->text.get_string();
			if (status_line->flags.test(_status_line_blink_bit) && system_milliseconds() % 500 < 250)
				string = "|n";

			if (!string_cache_add(&string_cache, string, status_line->alpha, status_line->color, justification))
			{
				string_cache_render(&string_cache, &draw_string, &font_cache);
				string_cache_add(&string_cache, string, status_line->alpha, status_line->color, justification);

				if (status_line->flags.test(_status_line_draw_once_bit))
					status_line->text.clear();
			}
		}

		string_cache_render(&string_cache, &draw_string, &font_cache);
	}
}

void status_line_dump()
{
	for (s_status_line* status_line = g_status_line_head; status_line; status_line = status_line->next)
	{
		if (status_line_visible(status_line))
			event(_event_message, "status_lines: %s", status_line->text.get_string());
	}
}

void status_line_remove_single(s_status_line* status_line)
{
	ASSERT(status_line->next != NULL || status_line == g_status_line_tail);
	ASSERT(status_line->prev != NULL || status_line == g_status_line_head);

	if (status_line->prev)
	{
		status_line->prev->next = status_line->next;
	}
	else
	{
		g_status_line_head = status_line->next;
	}

	if (status_line->next)
	{
		status_line->next->prev = status_line->prev;
	}
	else
	{
		g_status_line_tail = status_line->prev;
	}

	status_line->prev = NULL;
	status_line->next = NULL;
}

bool status_line_visible(const s_status_line* line)
{
	return (!line->in_use || *line->in_use) && !line->text.is_empty() && !line->flags.test(_status_line_inhibit_drawing_bit);
}

void status_lines_clear_text(s_status_line* status_lines, int32 count)
{
	for (int32 i = 0; i < count; i++)
	{
		status_lines[i].text.clear();
	}
}

void status_lines_disable(const char* identifier)
{
	for (s_status_line* status_line = g_status_line_head; status_line; status_line = status_line->next)
	{
		if (status_line->identifier && status_line->in_use && !*status_line->in_use)
		{
			if (csstristr(status_line->identifier, identifier))
			{
				*status_line->in_use = false;
			}
		}
	}
}

void status_lines_dispose(s_status_line* status_lines, int32 count)
{
	for (int32 i = 0; i < count; i++)
	{
		status_line_remove_single(&status_lines[i]);
	}
}

void status_lines_enable(const char* identifier)
{
	for (s_status_line* status_line = g_status_line_head; status_line; status_line = status_line->next)
	{
		if (status_line->identifier && status_line->in_use && !*status_line->in_use)
		{
			if (csstristr(status_line->identifier, identifier))
			{
				*status_line->in_use = true;
			}
		}
	}
}

void status_lines_initialize(s_status_line* status_lines, bool* flag, int32 count)
{
	status_lines_initialize_simple(status_lines, flag, NULL, count);
}

void status_lines_initialize_simple(s_status_line* status_line, bool* flag, const char* identifier, int32 count)
{
	csmemset(status_line, 0, sizeof(s_status_line) * count);

	for (int32 i = 0; i < count; i++)
	{
		s_status_line& line = status_line[i];
		line.in_use = flag;
		line.identifier = identifier;
		line.color = *global_real_rgb_white;
		line.alpha = 1.0f;
		status_line_add_single(&line);
	}
}

void status_lines_set_flags(s_status_line* status_lines, e_status_line_flags flag, bool enable, int32 count)
{
	for (int32 i = 0; i < count; i++)
	{
		status_lines[i].flags.set(flag, enable);
	}
}

void status_printf(const char* format, ...)
{
	if (is_main_thread())
	{
		va_list arglist;
		va_start(arglist, format);
		status_printf_va(format, arglist);
		va_end(arglist);
	}
}

void status_printf_va(const char* format, char* argument_list)
{
	char buffer[1024]{};
	cvsnzprintf(buffer, sizeof(buffer), format, argument_list);
	status_string_internal(format, buffer);
}

void status_string_internal(const char* status, const char* message)
{
	for (int32 i = 0; i < NUMBEROF(g_status_strings); i++)
	{
		s_status_string& status_string = g_status_strings[i];
		if (!status_string.line.text.is_empty() && status_string.format_string.is_equal(status))
		{
			status_string.time_created = system_milliseconds();
			status_string.line.text.set(message);
			return;
		}
	}

	for (int32 i = 0; i < NUMBEROF(g_status_strings); i++)
	{
		s_status_string& status_string = g_status_strings[i];
		if (status_string.line.text.is_empty())
		{
			status_string.time_created = system_milliseconds();
			status_string.line.text.set(message);
			status_string.format_string.set(status);
			break;
		}
	}
}

void status_strings(const char* status, const char* strings)
{
	if (is_main_thread())
	{
		char buffer[1024]{};
		char* data[3]{};
		int32 line = 0;

		csstrnzcpy(buffer, strings, sizeof(buffer));
		for (char* line_end = csstrtok(buffer, "\r\n", 1, data); line_end; line_end = csstrtok(NULL, "\r\n", 1, data))
		{
			c_static_string<256> string;
			string.print("%d%s", line++, status);
			status_string_internal(string.get_string(), line_end);
		}
	}
}

bool string_cache_add(s_string_cache* cache, const char* string, real32 alpha, const real_rgb_color& color, e_text_justification justification)
{
	bool result = false;
	if (cache->string.is_empty())
	{
		cache->color = color;
		cache->alpha = alpha;
		cache->justification = justification;
		result = true;
	}
	else if (cache->alpha == alpha
		&& cache->color.red == color.red
		&& cache->color.green == color.green
		&& cache->color.blue == color.blue
		&& cache->justification == justification)
	{
		result = true;
	}

	if (result)
	{
		cache->string.append(string);
		cache->string.append("|n");
	}

	return result;
}

void string_cache_render(s_string_cache* cache, c_draw_string* draw_string, c_font_cache_base* font_cache)
{
	if (cache->string.is_empty())
		return;

	real_argb_color color{};
	real_argb_color shadow_color{};

	color.rgb = cache->color;
	color.alpha = cache->alpha * 0.5f;

	shadow_color.rgb = *global_real_rgb_black;
	shadow_color.alpha = cache->alpha;

	draw_string->set_justification(cache->justification);
	draw_string->set_color(&color);
	draw_string->set_shadow_color(&shadow_color);
	draw_string->draw(font_cache, cache->string.get_string());

	cache->string.clear();
}

