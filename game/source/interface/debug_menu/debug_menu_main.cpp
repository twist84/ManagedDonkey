#include "interface/debug_menu/debug_menu_main.hpp"

#include "cache/restricted_memory_regions.hpp"
#include "config/version.hpp"
#include "cseries/cseries.hpp"
#include "game/game_time.hpp"
#include "input/controllers.hpp"
#include "input/input_windows.hpp"
#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_parse.hpp"
#include "interface/debug_menu/debug_menu_scroll.hpp"
#include "interface/user_interface_controller.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "text/draw_string.hpp"

//HOOK_DECLARE(0x00671670, debug_menu_initialize);
//HOOK_DECLARE(0x00671680, debug_menu_initialize_for_new_map);
//HOOK_DECLARE(0x006713B0, debug_menu_dispose);
//HOOK_DECLARE(0x006713C0, debug_menu_dispose_from_old_map);

class c_main_menu :
	public c_debug_menu_scroll
{
public:
	c_main_menu() :
		//c_debug_menu_scroll(NULL, 26, "Main") // Halo 3
		c_debug_menu_scroll(NULL, 26, version_get_full_string()) // version_get_build_and_changelist_string() in Halo 4
	{
	}
};

struct s_debug_menu_globals
{
	enum
	{
		k_str_length = 127
	};

	c_debug_menu* m_main_menu;
	c_debug_menu* m_active_menu;
	char m_caption[DEBUG_MENU_NUM_GLOBAL_CAPTIONS][k_str_length + 1];
	bool m_do_render;
	gamepad_state m_current_gamepad;
	gamepad_state m_last_gamepad;
	int32 open_menu_time;
};
static_assert(sizeof(s_debug_menu_globals) == 0x488);

real_argb_color const instance_debug_real_argb_grey       = { 1.0f,  0.3f, 0.42f, 0.33f };
real_argb_color const instance_debug_real_argb_white      = { 1.0f,  0.9f, 0.9f,  0.8f  };
real_argb_color const instance_debug_real_argb_tv_white   = { 1.0f,  0.8f, 0.8f,  0.75f };
real_argb_color const instance_debug_real_argb_tv_blue    = { 1.0f,  0.2f, 0.2f,  0.45f };
real_argb_color const instance_debug_real_argb_tv_magenta = { 1.0f,  0.7f, 0.05f, 0.7f  };
real_argb_color const instance_debug_real_argb_tv_orange  = { 1.0f,  1.0f, 0.5f,  0.0f  };
real_argb_color const instance_debug_real_argb_tv_green   = { 1.0f, 0.05f, 0.65f, 0.05f };

const real_argb_color* const debug_real_argb_grey       = &instance_debug_real_argb_grey;
const real_argb_color* const debug_real_argb_white      = &instance_debug_real_argb_white;
const real_argb_color* const debug_real_argb_tv_white   = &instance_debug_real_argb_tv_white;
const real_argb_color* const debug_real_argb_tv_blue    = &instance_debug_real_argb_tv_blue;
const real_argb_color* const debug_real_argb_tv_magenta = &instance_debug_real_argb_tv_magenta;
const real_argb_color* const debug_real_argb_tv_orange  = &instance_debug_real_argb_tv_orange;
const real_argb_color* const debug_real_argb_tv_green   = &instance_debug_real_argb_tv_green;

bool debug_menu_enabled = true;
s_debug_menu_globals g_debug_menu_globals = {};

bool g_debug_menu_rebuild_request = false;

c_static_stack<int32, k_debug_menu_stack_size> g_debug_menu_stack;

void debug_menu_draw_rect(int16 x0, int16 y0, int16 x1, int16 y1, real32 alpha, const real_argb_color* color)
{
	point2d points[4]{};

	set_point2d(&points[0], x0, y1);
	set_point2d(&points[1], x1, y1);
	set_point2d(&points[2], x1, y0);
	set_point2d(&points[3], x0, y0);

	rasterizer_quad_screenspace(points, real_argb_color_to_pixel32(color), NULL, 0, false);
}

bool debug_menu_get_active()
{
	return g_debug_menu_globals.m_active_menu != NULL;
}

c_debug_menu* debug_menu_get_root()
{
	return g_debug_menu_globals.m_main_menu;
}

void debug_menu_initialize()
{
}

void debug_menu_dispose()
{
}

void debug_menu_initialize_for_new_map()
{
	g_debug_menu_globals.m_do_render = false;
	g_debug_menu_globals.m_main_menu = NULL;
	g_debug_menu_globals.m_active_menu = NULL;

	g_debug_menu_globals.m_main_menu = DEBUG_MENU_MALLOC(c_main_menu);

	csmemset(&g_debug_menu_globals.m_last_gamepad, 0, sizeof(g_debug_menu_globals.m_last_gamepad));
	csmemset(&g_debug_menu_globals.m_current_gamepad, 0, sizeof(g_debug_menu_globals.m_current_gamepad));

	debug_menu_parse(debug_menu_get_root(), "debug_menu_init.txt");
	debug_menu_parse(debug_menu_get_root(), "debug_menu_user_init.txt");
}

void debug_menu_dispose_from_old_map()
{
	if (g_debug_menu_globals.m_main_menu)
	{
		g_debug_menu_stack.resize(0);
		g_debug_menu_globals.m_main_menu = NULL;
	}
	g_debug_menu_globals.m_active_menu = NULL;

	if (g_user_interface_controller_globals.suppressed)
		g_user_interface_controller_globals.suppressed = false;
}

void debug_menu_rebuild()
{
	g_debug_menu_rebuild_request = true;
}

void debug_menu_update_current_gamepad_state()
{
	csmemset(&g_debug_menu_globals.m_current_gamepad, 0, sizeof(g_debug_menu_globals.m_current_gamepad));
	for (e_controller_index controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
	{
		if (const gamepad_state* state = input_get_gamepad_state(static_cast<int16>(controller_index)))
			xor_buffers(&g_debug_menu_globals.m_current_gamepad, state, sizeof(gamepad_state));
	}

	csmemset(&g_debug_menu_globals.m_current_gamepad.sticks, 0, 8);
	csmemset(&g_debug_menu_globals.m_current_gamepad.analog_buttons, 0, 2);
	csmemset(&g_debug_menu_globals.m_current_gamepad.analog_button_thresholds, 0, 2);
}

void debug_menu_update()
{
	debug_menu_update_current_gamepad_state();

	const gamepad_state& state = debug_menu_get_gamepad_state();
	if (state.button_frames[_gamepad_binary_button_left_thumb])
	{
		g_debug_menu_globals.m_do_render = false;
	}
	else
	{
		bool v2 = false;
		if (debugging_system_has_focus())
		{
			v2 = input_key_frames_down(_key_home, _input_type_ui) == 1
				|| state.button_frames[_gamepad_binary_button_back] == 1;
		}
		else
		{
			v2 = input_key_frames_down(_key_home, _input_type_ui) == 1
				|| state.button_frames[_gamepad_binary_button_back] == 1 && state.button_frames[_gamepad_binary_button_start]
				|| state.button_frames[_gamepad_binary_button_back] && state.button_frames[_gamepad_binary_button_start] == 1;
		}

		if (!debugging_system_has_focus() && !debug_menu_enabled)
			v2 = false;

		if (debugging_system_has_focus())
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

		g_debug_menu_globals.m_do_render = true;
	}

	g_debug_menu_globals.m_last_gamepad = g_debug_menu_globals.m_current_gamepad;
}

void debug_menu_open()
{
	if (debug_menu_get_root() && !debug_menu_get_active())
	{
		debug_menu_set_active_menu(debug_menu_get_root(), false);
		g_debug_menu_globals.open_menu_time = system_milliseconds();
	}
}

void debug_menu_close()
{
	if (debug_menu_get_root() && debug_menu_get_active() &&
		system_milliseconds() - g_debug_menu_globals.open_menu_time > 100)
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
	main_set_single_thread_request_flag(_single_thread_for_user_request, debug_menu_get_active());

	if (restricted_region_locked_for_current_thread(1)
		&& debug_menu_get_active()
		&& g_debug_menu_globals.m_do_render)
	{
		c_font_cache_mt_safe font_cache{};

		point2d position{};
		set_point2d(&position, 180, 60);
		debug_menu_get_active_menu()->render(&font_cache, position);
	}
}

const gamepad_state& debug_menu_get_gamepad_state()
{
	return g_debug_menu_globals.m_current_gamepad;
}

const gamepad_state& debug_menu_get_last_gamepad_state()
{
	return g_debug_menu_globals.m_last_gamepad;
}

real32 debug_menu_get_item_margin()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 10.0f;
}

real32 debug_menu_get_item_width()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 650.0f;
}

real32 debug_menu_get_item_height()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 20.0f;
}

real32 debug_menu_get_title_height()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 20.0f;
}

real32 debug_menu_get_item_indent_x()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 40.0f;
}

real32 debug_menu_get_item_indent_y()
{
	return draw_string_get_glyph_scaling_for_display_settings() * 2.0f;
}

c_debug_menu* debug_menu_get_active_menu()
{
	return g_debug_menu_globals.m_active_menu;
}

void debug_menu_set_active_menu(c_debug_menu* active_menu, bool dont_open)
{
	c_debug_menu* current_active_menu = g_debug_menu_globals.m_active_menu;

	bool menu_is_parent = 0;
	if (g_debug_menu_globals.m_active_menu && active_menu)
	{
		if (c_debug_menu* parent = g_debug_menu_globals.m_active_menu->get_parent())
		{
			if (parent == active_menu)
				menu_is_parent = true;
		}
	}
	g_debug_menu_globals.m_active_menu = active_menu;

	if (active_menu)
	{
		if (!menu_is_parent && !dont_open)
		{
			active_menu->open();
			active_menu = g_debug_menu_globals.m_active_menu;
			g_user_interface_controller_globals.suppressed = true;
		}
		active_menu->notify_activated();
	}
	else if (current_active_menu)
	{
		current_active_menu->notify_closed();
		g_user_interface_controller_globals.suppressed = false;
	}

	for (int16 caption_index = 0; caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS; caption_index++)
		debug_menu_set_caption(caption_index, "");
}

void debug_menu_set_caption(int16 caption_index, const char* caption)
{
	ASSERT(caption_index >= 0 && caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS);

	csstrnzcpy(g_debug_menu_globals.m_caption[caption_index], caption, sizeof(*g_debug_menu_globals.m_caption));
}

const char* debug_menu_get_caption(int16 caption_index)
{
	ASSERT(caption_index >= 0 && caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS);

	return g_debug_menu_globals.m_caption[caption_index];
}

int32 debug_menu_get_time()
{
	return int32(((system_milliseconds() * 30.0f) / 1000.0f) + 0.5f);
}

void* debug_menu_malloc(int32 size)
{
	int32 size_in_count = (size + 3) >> 2;
	g_debug_menu_stack.resize(g_debug_menu_stack.count() + size_in_count);
	void* result = g_debug_menu_stack.get(g_debug_menu_stack.count() - size_in_count);

	return result;
}

void xor_buffers(void* dest, const void* source, int32 count)
{
	for (int32 i = 0; i < count; i++)
		((byte*)dest)[i] ^= ((const byte*)source)[i];
}

