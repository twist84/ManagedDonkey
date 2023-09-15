#include "interface/debug_menu/debug_menu_main.hpp"

#include "cache/restricted_memory_regions.hpp"
#include "cseries/cseries.hpp"
#include "input/controllers.hpp"
#include "input/input.hpp"
#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_scroll.hpp"
#include "interface/debug_menu/debug_menu_parse.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "shell/shell.hpp"
#include "text/draw_string.hpp"

void patch_debug_menu()
{
	patch_pointer({ .address = 0x01656850 }, debug_menu_initialize);
	patch_pointer({ .address = 0x01656854 }, debug_menu_dispose);
	patch_pointer({ .address = 0x01656858 }, debug_menu_initialize_for_new_map);
	patch_pointer({ .address = 0x0165685C }, debug_menu_dispose_from_old_map);
}

class c_main_menu :
	public c_debug_menu_scroll
{
public:
	c_main_menu() :
		c_debug_menu_scroll(NULL, 26, "Main")
	{
	}

	void* operator new(unsigned int size)
	{
		return debug_menu_malloc(size);
	}
};

struct s_debug_menu_globals
{
	c_debug_menu* root_menu;
	c_debug_menu* active_menu;
	char captions[DEBUG_MENU_NUM_GLOBAL_CAPTIONS][128];
	bool render;
	gamepad_state current_gamepad_state;
	gamepad_state last_gamepad_state;
	dword open_time;
};
static_assert(sizeof(s_debug_menu_globals) == 0x488);

real_argb_color const* const debug_real_argb_grey       = new real_argb_color({ 1.0f, {  0.3f, 0.42f, 0.33f } });
real_argb_color const* const debug_real_argb_white      = new real_argb_color({ 1.0f, {  0.9f, 0.9f,  0.8f  } });
real_argb_color const* const debug_real_argb_tv_white   = new real_argb_color({ 1.0f, {  0.8f, 0.8f,  0.75f } });
real_argb_color const* const debug_real_argb_tv_blue    = new real_argb_color({ 1.0f, {  0.2f, 0.2f,  0.45f } });
real_argb_color const* const debug_real_argb_tv_magenta = new real_argb_color({ 1.0f, {  0.7f, 0.05f, 0.7f  } });
real_argb_color const* const debug_real_argb_tv_orange  = new real_argb_color({ 1.0f, {  1.0f, 0.5f,  0.0f  } });
real_argb_color const* const debug_real_argb_tv_green   = new real_argb_color({ 1.0f, { 0.05f, 0.65f, 0.5f  } });

bool debug_menu_enabled = true;
s_debug_menu_globals g_debug_menu_globals = {};
c_static_stack<long, 64000> g_debug_menu_stack;

void debug_menu_draw_rect(short a1, short a2, short a3, short a4, real a5, real_argb_color const* color)
{
	int16_point2d points[4]{};

	set_point2d(&points[0], a1, a4);
	set_point2d(&points[1], a3, a4);
	set_point2d(&points[2], a3, a2);
	set_point2d(&points[3], a1, a2);

	rasterizer_quad_screenspace(points, real_argb_color_to_pixel32(color), nullptr, 0, false);
}

bool debug_menu_get_active()
{
	return g_debug_menu_globals.active_menu != NULL;
}

c_debug_menu* debug_menu_get_root()
{
	return g_debug_menu_globals.root_menu;
}

void debug_menu_initialize()
{
}

void debug_menu_dispose()
{
}

void debug_menu_initialize_for_new_map()
{
	g_debug_menu_globals.render = false;
	g_debug_menu_globals.root_menu = NULL;
	g_debug_menu_globals.active_menu = NULL;

	g_debug_menu_globals.root_menu = new c_main_menu();

	csmemset(&g_debug_menu_globals.last_gamepad_state, 0, sizeof(g_debug_menu_globals.last_gamepad_state));
	csmemset(&g_debug_menu_globals.current_gamepad_state, 0, sizeof(g_debug_menu_globals.current_gamepad_state));

	debug_menu_parse(debug_menu_get_root(), "debug_menu_init.txt");
	debug_menu_parse(debug_menu_get_root(), "debug_menu_user_init.txt");
}

void debug_menu_dispose_from_old_map()
{
	if (g_debug_menu_globals.root_menu)
	{
		g_debug_menu_stack.resize(0);
		g_debug_menu_globals.root_menu = NULL;
	}
	g_debug_menu_globals.active_menu = NULL;
}

void debug_menu_update_current_gamepad_state()
{
	csmemset(&g_debug_menu_globals.current_gamepad_state, 0, sizeof(g_debug_menu_globals.current_gamepad_state));
	for (long controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
	{
		//if (player_index_from_controller_index(controller_index) != k_no_controller)
		//{
		//	if (gamepad_state const* state = input_get_gamepad_state(controller_index))
		//		xor_buffers(&g_debug_menu_globals.current_gamepad_state, state, sizeof(gamepad_state));
		//}
	}

	csmemset(&g_debug_menu_globals.current_gamepad_state.thumb_left, 0, 8);
	csmemset(&g_debug_menu_globals.current_gamepad_state.trigger_down_amount, 0, 2);
	csmemset(&g_debug_menu_globals.current_gamepad_state.max_trigger_down_amount, 0, 2);
}

bool debugging_system_has_focus()
{
	return console_is_active() || debug_menu_get_active();
}

bool g_debug_menu_rebuild_request = false;

void debug_menu_update()
{
	debug_menu_update_current_gamepad_state();

	gamepad_state const& state = debug_menu_get_gamepad_state();
	if (state.buttons_down_frames[6])
	{
		g_debug_menu_globals.render = false;
	}
	else
	{
		bool v2 = input_key_frames_down(_key_code_home, _input_type_ui) == 1
			|| state.buttons_down_frames[5] == 1 && state.buttons_down_frames[4]
			|| state.buttons_down_frames[5] && state.buttons_down_frames[4] == 1;

		if (console_is_active())
		{
			v2 = input_key_frames_down(_key_code_home, _input_type_ui) == 1
				|| state.buttons_down_frames[5] == 1;
		}

		if (!debugging_system_has_focus() && !debug_menu_enabled)
			v2 = false;

		if (console_is_active())
		{
			if (v2)
				debug_menu_close();
		}
		else if (v2)
		{
			debug_menu_open();
		}

		if (debug_menu_get_active())
		{
			if (g_debug_menu_rebuild_request)
			{
				debug_menu_close();
				debug_menu_dispose_from_old_map();
				debug_menu_initialize_for_new_map();
				debug_menu_open();

				g_debug_menu_rebuild_request = false;
			}

			debug_menu_get_active_menu()->update();
		}

		g_debug_menu_globals.render = true;
	}

	g_debug_menu_globals.last_gamepad_state = g_debug_menu_globals.current_gamepad_state;
}

void debug_menu_open()
{
	if (debug_menu_get_root() && !debug_menu_get_active())
	{
		debug_menu_set_active_menu(debug_menu_get_root(), false);
		g_debug_menu_globals.open_time = system_milliseconds();
	}
}

void debug_menu_close()
{
	if (debug_menu_get_root() && debug_menu_get_active() &&
		system_milliseconds() - g_debug_menu_globals.open_time > 100)
	{
		debug_menu_set_active_menu(NULL, false);
	}
}

void render_debug_debug_menu_game()
{
	if (debug_menu_get_active())
		debug_menu_get_active_menu()->game_render();
}

void render_debug_debug_menu()
{
	main_set_single_thread_request_flag(0, debug_menu_get_active());

	if (restricted_region_locked_for_current_thread(1)
		&& debug_menu_get_active()
		&& g_debug_menu_globals.render)
	{
		c_font_cache_mt_safe font_cache{};

		int16_point2d point{};
		set_point2d(&point, 180, 60);
		debug_menu_get_active_menu()->render(&font_cache, &point);
	}
}

gamepad_state const& debug_menu_get_gamepad_state()
{
	return g_debug_menu_globals.current_gamepad_state;;
}

gamepad_state const& debug_menu_get_last_gamepad_state()
{
	return g_debug_menu_globals.last_gamepad_state;
}

real debug_menu_get_item_margin()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 10.0f;
}

real debug_menu_get_item_width()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 650.0f;
}

real debug_menu_get_item_height()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 20.0f;
}

real debug_menu_get_title_height()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 20.0f;
}

real debug_menu_get_item_indent_x()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 40.0f;
}

real debug_menu_get_item_indent_y()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 2.0f;
}

c_debug_menu* debug_menu_get_active_menu()
{
	return g_debug_menu_globals.active_menu;
}

void debug_menu_set_active_menu(c_debug_menu* menu, bool active)
{
	c_debug_menu* active_menu = g_debug_menu_globals.active_menu;

	bool v4 = g_debug_menu_globals.active_menu
		&& menu
		&& g_debug_menu_globals.active_menu->get_parent()
		&& g_debug_menu_globals.active_menu->get_parent() == menu;

	g_debug_menu_globals.active_menu = menu;

	if (menu)
	{
		if (!v4 && !active)
		{
			menu->open();
			menu = g_debug_menu_globals.active_menu;
		}
		menu->notify_activated();
	}
	else if (active_menu)
	{
		active_menu->notify_closed();
	}

	for (short caption_index = 0; caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS; caption_index++)
		debug_menu_set_caption(caption_index, "");
}

void debug_menu_set_caption(short caption_index, char const* caption)
{
	ASSERT(caption_index >= 0 && caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS);

	csstrnzcpy(g_debug_menu_globals.captions[caption_index], caption, sizeof(*g_debug_menu_globals.captions));
}

char const* debug_menu_get_caption(short caption_index)
{
	ASSERT(caption_index >= 0 && caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS);

	return g_debug_menu_globals.captions[caption_index];
}

long debug_menu_get_time()
{
	return long(((system_milliseconds() * 30.0f) / 1000.0f) + 0.5f);
}

void* debug_menu_malloc(long size)
{
	long size_in_count = (size + 3) >> 2;
	g_debug_menu_stack.resize(g_debug_menu_stack.count() + size_in_count);
	void* result = g_debug_menu_stack.get(g_debug_menu_stack.count() - size_in_count);

	return result;
}

void xor_buffers(void* destination, void const* source, long buffer_size)
{
	for (long i = 0; i < buffer_size; i++)
		((byte*)destination)[i] ^= ((byte const*)source)[i];
}

