#include "main/console.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game_time.hpp"
#include "hs/hs_compile.hpp"
#include "input/input_abstraction.hpp"
#include "interface/c_controller.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/gui_custom_bitmap_storage.hpp"
#include "interface/terminal.hpp"
#include "main/debug_keys.hpp"
#include "main/main.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "networking/tools/remote_command.hpp"
#include "profiler/profiler.hpp"
#include "render/render_cameras.hpp"
#include "shell/shell.hpp"
#include "sound/sound_manager.hpp"
#include "text/draw_string.hpp"
#include "xbox/xbox.hpp"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

HOOK_DECLARE(0x00605E10, console_execute_initial_commands);

s_console_globals console_globals;

bool console_dump_to_debug_display = false;

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
	bool present_list = false;
	if (console_token_buffer.is_empty())
	{
		console_token_buffer.set(console_get_token());
		console_token_buffer.set_length(console_globals.input_state.edit.insertion_point_index);
		suggestion_current_index = NONE;

		present_list = true;
	}

	char* token = console_get_token();

	const char* matching_items[256]{};
	int16 matching_item_count = hs_tokens_enumerate(console_token_buffer.get_string(), NONE, matching_items, NUMBEROF(matching_items));
	if (matching_item_count)
	{
		ASSERT(matching_items[0]);

		int16 last_similar_character_index = SHRT_MAX;
		bool columnize = matching_item_count > 16;

		c_static_string<1024> print_buffer;
		print_buffer.set("");

		if (present_list)
		{
			int16 item_index = 0;
			const char** matching_item = matching_items;
			for (item_index = 0; item_index < matching_item_count; item_index++, matching_item++)
			{
				int16 matching_item_length_minus_one = int16(strlen_debug(*matching_item)) - 1;
				if (last_similar_character_index > matching_item_length_minus_one)
				{
					last_similar_character_index = matching_item_length_minus_one;
				}

				int16 similar_character_index;
				for (similar_character_index = 0; ; similar_character_index++)
				{
					if (tolower(matching_items[0][similar_character_index]) != tolower(matching_items[item_index][similar_character_index])
						|| similar_character_index > last_similar_character_index)
					{
						break;
					}
				}

				if (similar_character_index)
				{
					if (similar_character_index >= int16(console_token_buffer.length()))
					{
						last_similar_character_index = similar_character_index - 1;
					}
				}

				if (columnize)
				{
					print_buffer.append_print("%s|t", *matching_item);
					if (item_index % 6 == 5)
					{
						console_printf("%s", print_buffer.get_string());
						print_buffer.clear();
					}
				}
				else
				{
					console_printf("%s", *matching_item);
				}
			}

			if (columnize && (item_index - 1) % 6 != 5)
			{
				console_printf("%s", print_buffer.get_string());
			}

			ASSERT(int16(strlen_debug(matching_items[0])) >= (last_similar_character_index + 1));

			csmemcpy(token, matching_items[0], last_similar_character_index + 1);
			token[last_similar_character_index + 1] = 0;

			console_globals.input_state.edit.insertion_point_index = last_similar_character_index + int16(token - console_globals.input_state.result + 1);

			suggestion_current_index = int16(strlen_debug(matching_items[0])) == last_similar_character_index + 1;
		}
		else if (suggestion_current_index == matching_item_count)
		{
			suggestion_current_index = 0;
			console_token_buffer.copy_to(token, 256);
			console_globals.input_state.edit.insertion_point_index = uns16(console_token_buffer.length() + 1);
		}
		else
		{
			csmemcpy(token, matching_items[suggestion_current_index], strlen_debug(matching_items[suggestion_current_index]));

			int16 suggestion_length = int16(strlen_debug(matching_items[suggestion_current_index++]));
			token[suggestion_length] = 0;

			console_globals.input_state.edit.insertion_point_index = int16(token - console_globals.input_state.result) + suggestion_length;
		}
	}
}

void __cdecl console_dispose()
{
	static bool x_once = true;
	if (x_once)
	{
		console_close();
		debug_keys_dispose();
		terminal_dispose();
		x_once = false;
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
	static bool x_once = true;
	if (x_once)
	{
		terminal_initialize();

		console_globals.status_render = true;
		console_globals.input_state.color = { 1.0f, 1.0f, 0.3f, 1.0f };
		csstrnzcpy(console_globals.input_state.prompt, "donkey( ", NUMBEROF(console_globals.input_state.prompt));
		console_globals.input_state.result[0] = 0;
		console_globals.previous_command_count = 0;
		console_globals.newest_previous_command_index = NONE;
		console_globals.selected_previous_command_index = NONE;

		debug_keys_initialize();

		for (int32 index = 0; index < NUMBEROF(g_status_strings); index++)
		{
			s_status_line* line = &g_status_strings[index].line;
			status_lines_initialize(line, NULL, 1);
			line->flags.set(_status_line_left_justify_bit, true);
		}

		x_once = false;
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
	va_list arglist;
	va_start(arglist, format);
	if (is_main_thread())
	{
		char buffer[255];
		cvsnzprintf(buffer, sizeof(buffer), format, arglist);
		terminal_printf(NULL, buffer);
		if (console_dump_to_debug_display)
		{
			display_debug_string(buffer);
		}
	}
	va_end(arglist);
}

void __cdecl console_printf_color(const real_argb_color* color, const char* format, ...)
{
	va_list arglist;
	va_start(arglist, format);
	if (is_main_thread())
	{
		char buffer[255];
		cvsnzprintf(buffer, sizeof(buffer), format, arglist);
		terminal_printf(color, buffer);
		if (console_dump_to_debug_display)
		{
			display_debug_string(buffer);
		}
	}
	va_end(arglist);
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
					{
						console_globals.selected_previous_command_index += 2;
					}

					int16 v4 = console_globals.selected_previous_command_index - 1;
					console_globals.selected_previous_command_index = v4;

					if (v4 <= 0)
					{
						console_globals.selected_previous_command_index = 0;
					}

					if (v4 <= 0)
					{
						v4 = 0;
					}

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
		{
			console_globals.open_timeout_seconds -= shell_seconds_elapsed;
		}
		else
		{
			console_globals.open_timeout_seconds = 0.0f;
		}
	}
}

void __cdecl console_warning(const char* format, ...)
{
	va_list arglist;
	va_start(arglist, format);

	if (is_main_thread())
	{
		char buffer[255]{};
		cvsnzprintf(buffer, sizeof(buffer), format, arglist);
		terminal_printf(global_real_argb_red, "%s", buffer);
	}
	va_end(arglist);
}

bool __cdecl console_process_command(const char* command, bool interactive)
{
	if (strlen_debug(command) >= 255)
	{
		return false;
	}

	if (!command[0] || command[0] == ';')
	{
		return false;
	}

	main_status("console_command", "%s", command);

	int16 command_index = (console_globals.newest_previous_command_index + 1) % NUMBEROF(console_globals.previous_commands);
	console_globals.newest_previous_command_index = command_index;
	console_globals.previous_commands[command_index].set(command);
	if (++console_globals.previous_command_count > NUMBEROF(console_globals.previous_commands))
	{
		console_globals.previous_command_count = NUMBEROF(console_globals.previous_commands);
	}
	console_globals.selected_previous_command_index = NONE;

	bool result = hs_compile_and_evaluate(_event_message, "console_command", command, interactive);

#if 0
	if (!result)
	{
		tokens_t tokens{};
		int32 token_count = 0;
		command_tokenize(command, tokens, &token_count);

		if (token_count == 2 && load_preference(tokens[0]->get_string(), tokens[1]->get_string()))
		{
			return true;
		}

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
		}
	}
#endif

	main_status("console_command", NULL);
	return result;
}

bool __cdecl debugging_system_has_focus()
{
	return console_is_active() || debug_menu_get_active();
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

