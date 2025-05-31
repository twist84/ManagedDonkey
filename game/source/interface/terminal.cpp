#include "interface/terminal.hpp"

#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "text/draw_string.hpp"

#include <math.h>

s_terminal_globals terminal_globals = { .initialized = false };

bool g_terminal_render_enable = true;

real32 const k_output_total_seconds = 4.0f + 1.0f;
int16 const k_tab_stops[] = { 160, 320, 470, 620, 770 };

void __cdecl terminal_printf(const real_argb_color* color, const char* format, ...)
{
	ASSERT(format != NULL);

	char message[256]{};

	va_list list;
	va_start(list, format);
	cvsnzprintf(message, sizeof(message), format, list);
	va_end(list);

	if (terminal_globals.initialized && !terminal_globals.suppress_output)
	{
		real_argb_color message_color = { 1.0f, 0.7f, 0.7f, 0.7f };
		if (color)
			message_color = *color;

		terminal_new_line(message, &message_color, csstrstr(message, "|t") != 0);

		//if (terminal_globals.console_output)
		//	c_console::write_line(message);

		//telnet_console_print(message);
	}
}

void __cdecl terminal_initialize()
{
	if (terminal_globals.initialized)
		return;

	terminal_globals.output_lines = data_new("terminal output", 32, sizeof(output_line_datum), 0, g_normal_allocation);
	data_make_valid(terminal_globals.output_lines);

	terminal_globals.console_output = false;
	terminal_globals.input_state = nullptr;
	terminal_globals.newest_output_line_index = NONE;
	terminal_globals.oldest_output_line_index = NONE;
	terminal_globals.initialized = true;
}

void __cdecl terminal_dispose()
{
	if (!terminal_globals.initialized)
		return;

	if (terminal_globals.output_lines)
	{
		if (terminal_globals.output_lines->valid)
			data_make_invalid(terminal_globals.output_lines);

		data_dispose(terminal_globals.output_lines);
		terminal_globals.output_lines = nullptr;
	}

	terminal_globals.initialized = false;
}

void __cdecl terminal_clear()
{
	if (!terminal_globals.initialized)
		return;

	c_font_cache_mt_safe font_cache;
	c_critical_section_scope critical_section_scope(k_crit_section_terminal);

	terminal_globals.newest_output_line_index = NONE;
	terminal_globals.oldest_output_line_index = NONE;

	data_delete_all(terminal_globals.output_lines);
}

void terminal_handle_key(s_key_state* key)
{
	ASSERT(terminal_globals.input_state);

	if (terminal_globals.input_state->key_count < 32)
		terminal_globals.input_state->keys[terminal_globals.input_state->key_count++] = *key;

	if (terminal_globals.input_state->edit.buffer)
		edit_text_handle_key(&terminal_globals.input_state->edit, key);

	terminal_globals.insertion_point_visible = true;
	terminal_globals.insertion_point_toggle_timer = 0.0f;
}

bool __cdecl terminal_update_input(real32 shell_seconds_elapsed)
{
	if (terminal_gets_active())
	{
		terminal_globals.input_state->key_count = 0;

		s_key_state key{};
		while (input_get_key(&key, _input_type_game))
			terminal_handle_key(&key);

		input_suppress();

		terminal_globals.insertion_point_toggle_timer += shell_seconds_elapsed;
		if (terminal_globals.insertion_point_toggle_timer > 1.0f)
		{
			terminal_globals.insertion_point_visible = !terminal_globals.insertion_point_visible;
			terminal_globals.insertion_point_toggle_timer = 0.0f;
		}

		int32 cursor_position = terminal_globals.input_state->prompt.length() + terminal_globals.input_state->edit.insertion_point_index;
		int32 scroll_amount = terminal_globals.input_state->horizontal_scroll_amount;

		if (cursor_position > scroll_amount + 59)
			scroll_amount = cursor_position - 59;

		if (cursor_position < scroll_amount + 20)
		{
			scroll_amount = cursor_position - 20;
			if (cursor_position - 20 < 0)
				scroll_amount = 0;
		}

		ASSERT(scroll_amount >= 0);
		terminal_globals.input_state->horizontal_scroll_amount = scroll_amount;

		return true;
	}

	return false;
}

void __cdecl terminal_update_output(real32 shell_seconds_elapsed)
{
	if (!terminal_globals.suppress_output && shell_seconds_elapsed < 0.5f)
	{
		c_font_cache_mt_safe font_cache;
		c_critical_section_scope critical_section_scope(k_crit_section_terminal);

		int32 line_count = NONE;
		for (int32 line_index = terminal_globals.newest_output_line_index; line_index != NONE; line_index = line_count)
		{
			output_line_datum* line = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, line_index);
			line_count = line->line_count;

			line->timer += shell_seconds_elapsed;
			if (line->timer > k_output_total_seconds)
				terminal_remove_line(line_index);
		}
	}
}

bool __cdecl terminal_update(real32 shell_seconds_elapsed)
{
	bool result = false;
	if (terminal_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(k_crit_section_terminal);

		result = terminal_update_input(shell_seconds_elapsed);
		if (!console_is_active())
			terminal_update_output(shell_seconds_elapsed);
	}

	return result;
}

void __cdecl terminal_remove_line(int32 line_index)
{
	c_critical_section_scope critical_section_scope(k_crit_section_terminal);

	output_line_datum* line = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, line_index);

	if (line->line_count == NONE)
	{
		terminal_globals.oldest_output_line_index = line->line_index;
	}
	else
	{
		output_line_datum* temp = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, line->line_count);
		temp->line_index = line->line_index;
	}

	if (line->line_index == NONE)
	{
		terminal_globals.newest_output_line_index = line->line_count;
	}
	else
	{
		output_line_datum* temp = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, line->line_index);
		temp->line_count = line->line_count;
	}

	datum_delete(terminal_globals.output_lines, line_index);
}

int32 __cdecl terminal_new_line(const char* buffer, const real_argb_color* color, bool tabstop)
{
	c_font_cache_mt_safe font_cache;
	c_critical_section_scope critical_section_scope(k_crit_section_terminal);

	if (data_is_full(terminal_globals.output_lines))
		terminal_remove_line(terminal_globals.oldest_output_line_index);

	int32 new_line_index = datum_new(terminal_globals.output_lines);
	ASSERT(new_line_index != NONE);

	output_line_datum* line = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, new_line_index);

	line->line_index = NONE;
	line->line_count = terminal_globals.newest_output_line_index;
	line->tabstop = tabstop;
	line->buffer.set(buffer);
	line->color = *color;
	line->timer = 0.0f;

	terminal_globals.newest_output_line_index = new_line_index;

	if (line->line_count != NONE)
	{
		output_line_datum* temp = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, line->line_count);
		temp->line_index = new_line_index;
	}
	else
	{
		terminal_globals.oldest_output_line_index = new_line_index;
	}

	return new_line_index;
}

void __cdecl terminal_output_to_console(bool console_output)
{
	terminal_globals.console_output = console_output;
}

void __cdecl terminal_suppress_output(bool suppress_output)
{
	terminal_globals.suppress_output = suppress_output;
}

bool __cdecl terminal_gets_active()
{
	return terminal_globals.input_state != nullptr;
}

bool __cdecl terminal_gets_begin(terminal_gets_state* state)
{
	c_critical_section_scope critical_section_scope(k_crit_section_terminal);

	ASSERT(state);

	if (!terminal_gets_active())
	{
		terminal_globals.input_state = state;
		state->edit.buffer = state->result;
		terminal_globals.input_state->edit.maximum_length = NUMBEROF(state->result) - 1;
		terminal_globals.input_state->horizontal_scroll_amount = 0;
		edit_text_new(&terminal_globals.input_state->edit);
		state->key_count = 0;

		return true;
	}

	return false;
}

void __cdecl terminal_gets_end(terminal_gets_state* state)
{
	c_critical_section_scope critical_section_scope(k_crit_section_terminal);

	if (state == terminal_globals.input_state)
		terminal_globals.input_state = nullptr;
}

void __cdecl terminal_draw(rectangle2d* screen_bounds, rectangle2d* frame_bounds)
{
	if (terminal_globals.initialized)
	{
		//c_critical_section_scope critical_section_scope(k_crit_section_terminal);
		c_rasterizer_draw_string draw_string;
		c_font_cache_mt_safe font_cache;

		int16 line_height = draw_string.get_line_height();

		rectangle2d pixel_bounds[4]{};
		interface_get_current_display_settings(&pixel_bounds[0], &pixel_bounds[1], &pixel_bounds[2], &pixel_bounds[3]);
		rectangle2d* pixel_bounds_ = &pixel_bounds[3];

		rectangle2d bounds{};
		if (terminal_gets_active())
		{
			c_static_string<288> buffer;
			buffer.set(terminal_globals.input_state->prompt.get_string());
			buffer.append(terminal_globals.input_state->result);

			bounds.x0 = pixel_bounds_->x0;
			bounds.x1 = pixel_bounds_->x1;
			bounds.y0 = pixel_bounds_->y1 - line_height;
			bounds.y1 = pixel_bounds_->y1;

			// cursor blink
			if (terminal_globals.insertion_point_visible)
			{
				int16 index = int16(terminal_globals.input_state->prompt.length()) + terminal_globals.input_state->edit.insertion_point_index;
				buffer.set_character(index, '_');
			}

			draw_string.set_color(&terminal_globals.input_state->color);
			draw_string.set_bounds(&bounds);
			draw_string.draw(&font_cache, buffer.get_offset(int_pin(terminal_globals.input_state->horizontal_scroll_amount, 0l, buffer.length())));
		}

		if (g_terminal_render_enable)
		{
			real_argb_color shadow_color = *global_real_argb_black;
			int16 line_offset = pixel_bounds_->y1 - line_height;

			int32 line_count = NONE;
			for (int32 line_index = terminal_globals.newest_output_line_index; line_index != NONE && line_offset - line_height > 0; line_index = line_count)
			{
				if (line_offset - line_height <= 0)
					break;

				output_line_datum* line = DATUM_TRY_AND_GET(terminal_globals.output_lines, output_line_datum, line_index);
				line_count = line->line_count;

				real32 alpha_scale = fmaxf(4.0f - line->timer, 0.0f);
				if (alpha_scale >= 1.0f)
					alpha_scale = 1.0f;

				line->color.alpha *= alpha_scale;
				shadow_color.alpha = line->color.alpha;

				bounds.x0 = pixel_bounds_->x0;
				bounds.x1 = pixel_bounds_->x1;
				bounds.y1 = line_offset;
				line_offset -= line_height;
				bounds.y0 = line_offset;

				if (line->tabstop)
					draw_string.set_tab_stops(k_tab_stops, NUMBEROF(k_tab_stops));

				draw_string.set_color(&line->color);
				draw_string.set_shadow_color(&shadow_color);
				draw_string.set_bounds(&bounds);
				draw_string.draw(&font_cache, line->buffer.get_string());

				draw_string.set_tab_stops(k_tab_stops, 0);
			}
		}
	}
}

