#include "main/console.hpp"

#include "cseries/cseries_console.hpp"
#include "interface/terminal.hpp"
#include "main/debug_keys.hpp"
#include "multithreading/threads.hpp"

#include <string.h>

s_console_globals console_globals;

bool console_dump_to_debug_display = false;

void __cdecl console_printf(char const* format, ...)
{
	va_list list;
	va_start(list, format);

	c_console::write_line(format, list);
	if (is_main_thread())
	{
		c_static_string<255> message;
		message.vprint(format, list);
		terminal_printf(nullptr, "%s", message.get_string());
		c_console::write_line(message.get_string());

		if (console_dump_to_debug_display)
			display_debug_string(message.get_string());
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
		c_static_string<255> message;
		message.vprint(format, list);
		terminal_printf(global_real_argb_red, "%s", message.get_string());
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
		console_globals.input_state.prompt_color = { 1.0f, 1.0f, 0.30000001f, 1.0f };
		console_globals.input_state.prompt_text.set("donkey( ");
		console_globals.input_state.input_text.clear();
		console_globals.input_state.__unknown11F4 = 0;
		console_globals.input_state.command_count = -1;
		console_globals.input_state.__unknown11F8 = -1;

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
	return console_globals.is_active && console_globals.input_state.input_text.empty();
}

void __cdecl console_open()
{
	if (!console_is_active())
	{
		console_globals.input_state.input_text.clear();
		console_globals.is_active = terminal_gets_begin(&console_globals.input_state);
	}
}

void __cdecl console_close()
{
	if (console_globals.is_active)
	{
		terminal_gets_end(&console_globals.input_state);
		console_globals.__time4 = 0.1f;
		console_globals.is_active = false;
	}
}

void __cdecl console_clear()
{
	terminal_clear();
}

char const* __cdecl console_get_token()
{
	char const* input_text = strrchr(console_globals.input_state.input_text.get_string(), ' ') + 1;
	char const* v1 = strrchr(console_globals.input_state.input_text.get_string(), '(') + 1;
	char const* result = strrchr(console_globals.input_state.input_text.get_string(), '"') + 1;

	if (console_globals.input_state.input_text.get_string() > input_text)
		input_text = console_globals.input_state.input_text.get_string();

	if (input_text > v1)
		v1 = input_text;

	if (v1 > result)
		return v1;

	return result;
}

char console_token_buffer[256]{};
long suggestion_current_index;

void __cdecl console_complete()
{
	bool something = false;
	if (!console_token_buffer[0])
	{
		csstrnzcpy(console_token_buffer, console_get_token(), NUMBEROF(console_token_buffer));
		if (console_globals.input_state.edit.selection_index6 <= 254)
			console_token_buffer[console_globals.input_state.edit.selection_index6] = 0;

		suggestion_current_index = -1;

		something = true;
	}
}

bool __cdecl console_process_command(char const* command, bool a2)
{
	if (strlen(command) >= 255)
		return false;

	c_console::write_line("console_command: %s", command);

	short command_index = (console_globals.input_state.command_count + 1) % 16;
	console_globals.input_state.command_count = command_index;
	console_globals.input_state.commands[command_index].set(command);

	short v5 = 16;
	if (console_globals.input_state.__unknown11F4 <= 15)
		v5 = console_globals.input_state.__unknown11F4 + 1;
	console_globals.input_state.__unknown11F4 = v5;

	console_globals.input_state.__unknown11F8 = -1;

	bool result = true;//hs_compile_and_evaluate(_event_level_message, "console_command", command, a2);
	c_console::write_line("console_command: ");

	return result;
}

void __cdecl console_update(real shell_seconds_elapsed)
{
	if (!console_is_active())
	{
		s_key_state key{};
		if (input_peek_key(&key, _input_type_game))
		{
			if (!key.was_key_down && !key.modifier && key.key_code == _key_code_backquote)
			{
				input_get_key(&key, _input_type_game);
				console_open();
			}
		}

		debug_keys_update();

		if ((console_globals.__time4 - shell_seconds_elapsed) >= 0.0f)
			console_globals.__time4 -= shell_seconds_elapsed;
		else
			console_globals.__time4 = 0.0f;

		return;
	}

	if (console_globals.input_state.key_count <= 0)
	{
		if ((console_globals.__time4 - shell_seconds_elapsed) >= 0.0f)
			console_globals.__time4 -= shell_seconds_elapsed;
		else
			console_globals.__time4 = 0.0f;

		return;
	}

	bool console_closed = false;

	short key_index = 0;
	while (!console_closed)
	{
		s_key_state* key = &console_globals.input_state.keys[key_index];
		ASSERT(key->key_code != NONE);

		e_key_code key_code = key->key_code;
		switch (key_code)
		{
		case _key_code_backquote:
		{
			console_close();
			console_closed = true;
		}
		break;
		case _key_code_tab:
		{
			console_complete();
		}
		break;
		case _key_code_enter:
		case _key_code_keypad_enter:
			if (!console_globals.input_state.input_text.empty())
			{
				console_process_command(console_globals.input_state.input_text.get_string(), true);
				console_globals.input_state.input_text.clear();
				edit_text_selection_reset(&console_globals.input_state.edit);
			}
			else
			{
				console_close();
				console_closed = true;
			}
			break;
		case _key_code_x:
		case _key_code_c:
		case _key_code_v:
		{
			if (!console_globals.input_state.keys[key_index].modifier.test(_key_modifier_flag_control_key_bit))
			{
				suggestion_current_index = 0;
				console_token_buffer[0] = 0;
			}
		}
		break;
		case _key_code_up:
			console_globals.input_state.__unknown11F8 += 2;
		case _key_code_down:
		{
			short v4 = console_globals.input_state.__unknown11F8 - 1;
			console_globals.input_state.__unknown11F8 = v4;

			if (v4 <= 0)
				console_globals.input_state.__unknown11F8 = 0;

			if (v4 <= 0)
				v4 = 0;

			short v5;
			if (v4 <= console_globals.input_state.__unknown11F4 - 1)
			{
				v5 = v4;
			}
			else
			{
				v5 = console_globals.input_state.__unknown11F4 - 1;
				console_globals.input_state.__unknown11F8 = console_globals.input_state.__unknown11F4 - 1;
			}

			if (v5 != -1)
			{
				console_globals.input_state.input_text.set(console_globals.input_state.commands[(console_globals.input_state.command_count - v5 + 16) % 16].get_string());
				edit_text_selection_reset(&console_globals.input_state.edit);
			}
		}
		break;
		default:
		{
			suggestion_current_index = 0;
			console_token_buffer[0] = 0;
		}
		break;
		}
	}

	if (++key_index >= console_globals.input_state.key_count)
	{
		if (console_closed)
			return;

		if ((console_globals.__time4 - shell_seconds_elapsed) >= 0.0f)
			console_globals.__time4 -= shell_seconds_elapsed;
		else
			console_globals.__time4 = 0.0f;

		return;
	}
}

