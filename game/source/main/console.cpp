#include "main/console.hpp"

#include "cseries/cseries_console.hpp"
#include "interface/terminal.hpp"
#include "main/debug_keys.hpp"
#include "main/main.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/threads.hpp"
#include "networking/tools/remote_command.hpp"
#include "xbox/xbox.hpp"

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
		console_globals.input_state.input_text[0] = '\0';
		console_globals.input_state.__unknown11F4 = 0;
		console_globals.input_state.previous_inputs_count = -1;
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
	return console_globals.is_active && !console_globals.input_state.input_text[0];
}

void __cdecl console_open()
{
	if (!console_is_active())
	{
		console_globals.input_state.input_text[0] = '\0';
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
	char const* input_text = strrchr(console_globals.input_state.input_text, ' ') + 1;
	char const* v1 = strrchr(console_globals.input_state.input_text, '(') + 1;
	char const* result = strrchr(console_globals.input_state.input_text, '"') + 1;

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
		suggestion_current_index = -1;

		something = true;
	}
}

bool __cdecl console_process_command(char const* command, bool a2)
{
	if (strlen(command) >= 255)
		return false;

	c_console::write_line("console_command: %s", command);

	short command_index = (console_globals.input_state.previous_inputs_count + 1) % NUMBEROF(console_globals.input_state.previous_inputs);
	console_globals.input_state.previous_inputs_count = command_index;
	console_globals.input_state.previous_inputs[command_index].set(command);

	short v5 = NUMBEROF(console_globals.input_state.previous_inputs);
	if (console_globals.input_state.__unknown11F4 + 1 <= NUMBEROF(console_globals.input_state.previous_inputs))
		v5 = console_globals.input_state.__unknown11F4 + 1;
	console_globals.input_state.__unknown11F4 = v5;

	console_globals.input_state.__unknown11F8 = -1;

	bool result = false;//hs_compile_and_evaluate(_event_level_message, "console_command", command, a2);
	c_console::write_line("console_command: ");

	tokens_t tokens{};
	long token_count = 0;
	command_tokenize(command, tokens, &token_count);
	if (token_count > 0)
	{
		for (long i = 0; i < NUMBEROF(k_registered_commands); i++)
		{
			if (tokens[0]->equals(k_registered_commands[i].name))
			{
				long succeeded = k_registered_commands[i].callback(&k_registered_commands[i], token_count, tokens).index_of(": succeeded");
				result = succeeded != -1 || tokens[0]->equals("help");

				if (result)
					console_printf("command '%s' succeeded", tokens[0]);
			}
		}

		if (!result)
			console_warning("command '%s' not found", tokens[0]);
	}

	return result;
}

void __cdecl console_update(real shell_seconds_elapsed)
{
	if (!console_is_active())
	{
		s_key_state key{};
		if (input_peek_key(&key, _input_type_game))
		{
			if (!key.was_key_down && !key.modifier && key.key_code == _key_code_backquote && key.key_type == _key_type_down)
			{
				input_get_key(&key, _input_type_game);
				console_open();
			}
		}
		else
		{
			debug_keys_update();
		}
	}
	else
	{
		input_globals.suppressed = true;

		for (long key_index = 0; key_index < console_globals.input_state.key_count; key_index++)
		{
			s_key_state* key = &console_globals.input_state.keys[key_index];
			ASSERT(key->key_code != NONE);

			if (key->key_type == _key_type_down && key->key_code == _key_code_backquote)
			{
				console_close();
				break;
			}
			else if (key->modifier.test(_key_modifier_flag_control_key_bit) && key->key_type == _key_type_up && key->key_code == _key_code_v)
			{
				char buffer[256]{};
				get_clipboard_as_text(buffer, NUMBEROF(buffer));
				csnzappendf(console_globals.input_state.input_text, NUMBEROF(console_globals.input_state.input_text), buffer);
				break;
			}
			else if (key->key_type == _key_type_up && (key->key_code == _key_code_enter || key->key_code == _key_code_keypad_enter))
			{
				if (console_globals.input_state.input_text[0])
				{
					if (console_process_command(console_globals.input_state.input_text, true))
					{
						console_globals.input_state.input_text[0] = '\0';
						edit_text_selection_reset(&console_globals.input_state.edit);
						console_close();
					}
				}
				break;
			}
			else if (key->vk_code != 0xFFFF && key->key_type == _key_type_char)
			{
				csnzappendf(console_globals.input_state.input_text, NUMBEROF(console_globals.input_state.input_text), key->character);
			}

			// #TODO: 
			if (csstrnlen(console_globals.input_state.input_text, NUMBEROF(console_globals.input_state.input_text)) > 50)
			{
				console_globals.input_state.input_text[0] = '\0';
				console_close();
				console_warning("console: too many characters '50'");
			}

		}
	}

	TLS_REFERENCE(players_globals);
	players_globals->mostly_inhibit = input_globals.suppressed;

	if ((console_globals.__time4 - shell_seconds_elapsed) >= 0.0f)
		console_globals.__time4 -= shell_seconds_elapsed;
	else
		console_globals.__time4 = 0.0f;
}

