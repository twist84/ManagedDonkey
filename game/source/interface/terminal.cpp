#include "interface/terminal.hpp"

#include "cseries/cseries_console.hpp"
#include "input/input.hpp"
#include "interface/interface_constants.hpp"
#include "memory/module.hpp"
#include "text/draw_string.hpp"

s_terminal_globals terminal_globals = { .initialized = false };

bool g_terminal_render_enable = true;

real const k_output_total_seconds = 4.0f + 1.0f;
short const k_tab_stops[]{ 160, 320, 470, 620, 770 };

REFERENCE_DECLARE(0x0189CDE0, c_allocation_base*, g_normal_allocation);

// #TODO: find a home
struct c_critical_section_scope
{
	c_critical_section_scope(long critical_section_id) :
		m_critical_section_id(),
		m_critical_section_entered()
	{
		DECLFUNC(0x0052C4A0, void, __cdecl, long)(critical_section_id);
	}

	c_critical_section_scope(long critical_section_id, long time_step, bool* out_lock_acquired) :
		m_critical_section_id(),
		m_critical_section_entered()
	{
		DECLFUNC(0x0052BEE0, void, __thiscall, c_critical_section_scope*, long, long, bool*)(this, critical_section_id, time_step, out_lock_acquired);
	}

	~c_critical_section_scope()
	{
		DECLFUNC(0x0052C4E0, void, __cdecl, long)(m_critical_section_id);
	}

	volatile long m_critical_section_id;
	volatile bool m_critical_section_entered;
};

void __cdecl terminal_initialize()
{
	if (terminal_globals.initialized)
		return;

	terminal_globals.output_data = (c_smart_data_array<terminal_output_datum>*)data_new("terminal output", 32, sizeof(terminal_output_datum), 0, g_normal_allocation);
	data_make_valid(terminal_globals.output_data);

	terminal_globals.output_to_console = false;
	terminal_globals.input_state = nullptr;
	terminal_globals.line_count = NONE;
	terminal_globals.line_index = NONE;
	terminal_globals.initialized = true;
}

void __cdecl terminal_dispose()
{
	if (!terminal_globals.initialized)
		return;

	if (terminal_globals.output_data)
	{
		if (terminal_globals.output_data->valid)
			data_make_invalid(terminal_globals.output_data);

		data_dispose(terminal_globals.output_data);
		terminal_globals.output_data = nullptr;
	}

	terminal_globals.initialized = false;
}

void __cdecl terminal_clear()
{
	if (!terminal_globals.initialized)
		return;

	c_font_cache_mt_safe font_cache;
	c_critical_section_scope critical_section_scope(3);

	terminal_globals.line_count = NONE;
	terminal_globals.line_index = NONE;

	data_delete_all(terminal_globals.output_data);
}

void terminal_handle_key(s_key_state* key)
{
	ASSERT(terminal_globals.input_state);

	// #TODO: map out `terminal_gets_state`

	terminal_globals.should_draw = true;
	terminal_globals.draw_time = 0.0;
}

bool __cdecl terminal_update_input(real shell_seconds_elapsed)
{
	// #TODO: map out `terminal_gets_state`
	//if (terminal_gets_active())
	{
		//s_key_state key;
		//while (input_get_key(&key, _input_type_game))
		//	terminal_handle_key(&key);

		//input_suppress();

		terminal_globals.draw_time += shell_seconds_elapsed;
		if (terminal_globals.draw_time > 1.0f)
		{
			terminal_globals.should_draw = !terminal_globals.should_draw;
			terminal_globals.draw_time = 0.0f;
		}

		return true;
	}

	//return false;
}

void __cdecl terminal_update_output(real shell_seconds_elapsed)
{
	if (!terminal_globals.suppress_output && shell_seconds_elapsed < 0.5f)
	{
		c_font_cache_mt_safe font_cache;
		c_critical_section_scope critical_section_scope(3);

		long line_count = NONE;
		for (long line_index = terminal_globals.line_count; line_index != NONE; line_index = line_count)
		{
			terminal_output_datum* line = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, line_index);
			line_count = line->line_count;

			line->shadow_color_alpha_scale += shell_seconds_elapsed;
			if (line->shadow_color_alpha_scale > k_output_total_seconds)
				terminal_remove_line(line_index);
		}
	}
}

bool __cdecl debugging_system_has_focus()
{
	return false; // console_is_active() || debug_menu_get_active();
}

bool __cdecl terminal_update(real shell_seconds_elapsed)
{
	bool result = false;
	if (terminal_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(3);

		result = terminal_update_input(shell_seconds_elapsed);
		if (!debugging_system_has_focus())
			terminal_update_output(shell_seconds_elapsed);
	}

	return result;
}
HOOK_DECLARE_CALL(0x00505CCD, terminal_update);

void __cdecl terminal_remove_line(long line_index)
{
	c_critical_section_scope critical_section_scope(3);

	terminal_output_datum* line = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, line_index);

	if (line->line_count == NONE)
	{
		terminal_globals.line_index = line->line_index;
	}
	else
	{
		terminal_output_datum* temp = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, line->line_count);
		temp->line_index = line->line_index;
	}

	if (line->line_index == NONE)
	{
		terminal_globals.line_count = line->line_count;
	}
	else
	{
		terminal_output_datum* temp = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, line->line_index);
		temp->line_count = line->line_count;
	}

	datum_delete(terminal_globals.output_data, line_index);
}

long __cdecl terminal_new_line(char const* message, real_argb_color const* message_color, bool tab_stop)
{
	c_font_cache_mt_safe font_cache;
	c_critical_section_scope critical_section_scope(3);

	if (data_is_full(terminal_globals.output_data))
		terminal_remove_line(terminal_globals.line_index);

	long new_line_index = datum_new(terminal_globals.output_data);
	ASSERT(new_line_index != NONE);

	terminal_output_datum* line = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, new_line_index);

	line->line_index = NONE;
	line->line_count = terminal_globals.line_count;
	line->tab_stop = tab_stop;
	line->message.set(message);
	line->message_color = *message_color;
	line->shadow_color_alpha_scale = 0.0f;

	terminal_globals.line_count = new_line_index;

	if (line->line_count != NONE)
	{
		terminal_output_datum* temp = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, line->line_count);
		temp->line_index = new_line_index;
	}
	else
	{
		terminal_globals.line_index = new_line_index;
	}

	return new_line_index;
}

void __cdecl terminal_printf(real_argb_color const* color, char const* format, ...)
{
	ASSERT(format != NULL);

	va_list list;
	va_start(list, format);

	if (terminal_globals.initialized && !terminal_globals.suppress_output)
	{
		real_argb_color message_color = { 1.0f, 0.69999999f, 0.69999999f, 0.69999999f };
		if (color)
			message_color = *color;

		char message[255]{};
		cvsnzprintf(message, 255, format, list);
		terminal_new_line(message, &message_color, csstrstr(message, "|t") != 0);
		c_console::write_line(message); //telnet_console_print(message);
	}

	va_end(list);
}

void __cdecl terminal_draw()
{
	if (!terminal_globals.initialized)
		return;

	c_font_cache_mt_safe font_cache;
	c_rasterizer_draw_string rasterizer_draw_string;
	short line_height = rasterizer_draw_string.get_line_height();

	short_rectangle2d pixel_bounds[4]{};
	interface_get_current_display_settings(&pixel_bounds[0], &pixel_bounds[1], &pixel_bounds[2], &pixel_bounds[3]);
	short_rectangle2d* pixel_bounds_ = &pixel_bounds[3];

	if (terminal_gets_active())
	{
		// #TODO: map out `terminal_gets_state`
	}

	if (g_terminal_render_enable)
	{
		real_argb_color shadow_color = *global_real_argb_black;
		short y0 = pixel_bounds_->y1 - line_height;

		long line_count = NONE;
		for (long line_index = terminal_globals.line_count; line_index != NONE && y0 - line_height > 0; line_index = line_count)
		{
			if (y0 - line_height <= 0)
				break;

			terminal_output_datum* line = (terminal_output_datum*)datum_try_and_get(terminal_globals.output_data, line_index);
			line_count = line->line_count;

			real alpha_scale = fmaxf(4.0f - line->shadow_color_alpha_scale, 0.0f);
			if (alpha_scale >= 1.0f)
				alpha_scale = 1.0f;

			line->message_color.alpha *= alpha_scale;
			shadow_color.alpha = line->message_color.alpha;

			short_rectangle2d bounds{};
			bounds.x0 = pixel_bounds_->x0;
			bounds.x1 = pixel_bounds_->x1;
			bounds.y1 = y0;
			y0 -= line_height;
			bounds.y0 = y0;

			if (line->tab_stop)
				rasterizer_draw_string.set_tab_stops(k_tab_stops, NUMBEROF(k_tab_stops));

			rasterizer_draw_string.set_color(&line->message_color);
			rasterizer_draw_string.set_shadow_color(&shadow_color);
			rasterizer_draw_string.set_bounds(&bounds);
			rasterizer_draw_string.draw(&font_cache, line->message.get_string());

			rasterizer_draw_string.set_tab_stops(k_tab_stops, 0);
		}
	}
}

void __cdecl terminal_output_to_console(bool output_to_console)
{
	terminal_globals.output_to_console = output_to_console;
}

void __cdecl terminal_suppress_output(bool suppress_output)
{
	terminal_globals.suppress_output = suppress_output;
}

bool __cdecl terminal_gets_active()
{
	return terminal_globals.input_state != nullptr;
}

