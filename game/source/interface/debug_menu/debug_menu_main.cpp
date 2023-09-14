#include "interface/debug_menu/debug_menu_main.hpp"

#include "cache/restricted_memory_regions.hpp"
#include "cseries/cseries.hpp"
#include "input/input.hpp"
#include "interface/debug_menu/debug_menu.hpp"
#include "main/main.hpp"
#include "math/color_math.hpp"
#include "rasterizer/rasterizer.hpp"
#include "text/draw_string.hpp"

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

void debug_menu_draw_rect(short a1, short a2, short a3, short a4, float a5, real_argb_color const* color)
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

class c_main_menu
{
	byte __data[0x12C];
};

void debug_menu_initialize_for_new_map()
{
	g_debug_menu_globals.render = false;
	g_debug_menu_globals.root_menu = NULL;
	g_debug_menu_globals.active_menu = NULL;

	//c_main_menu* main_menu = static_cast<c_main_menu*>(debug_menu_malloc(sizeof(c_main_menu)));
	//g_debug_menu_globals.root_menu = main_menu;

	csmemset(&g_debug_menu_globals.last_gamepad_state, 0, sizeof(g_debug_menu_globals.last_gamepad_state));
	csmemset(&g_debug_menu_globals.current_gamepad_state, 0, sizeof(g_debug_menu_globals.current_gamepad_state));

	debug_menu_parse(debug_menu_get_root(), "debug_menu_init.txt");
	debug_menu_parse(debug_menu_get_root(), "debug_menu_user_init.txt.txt");
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

void debug_menu_update()
{
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

void debug_menu_parse(c_debug_menu* root_menu, char const* file_name)
{
	ASSERT(file_name != NULL);
	ASSERT(root_menu != NULL);

	FILE* menu_file;
	if (fopen_s(&menu_file, file_name, "rt") == 0 && menu_file)
	{
		char error_buffer[1024]{};

		long line_count = 1;
		int c = fgetc(menu_file);
		//debug_menu_build_recursive(menu_file, &c, root_menu, &line_count, error_buffer, sizeof(error_buffer));
		fclose(menu_file);
	}
}

