#include "interface/debug_menu/debug_menu.hpp"

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"
#include "input/input_abstraction.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface_constants.hpp"
#include "text/draw_string.hpp"

#include <string.h>

c_debug_menu::~c_debug_menu()
{
	clear();
}

void c_debug_menu::update()
{
	if (get_selection() >= 0 && get_selection() < get_num_items() && !get_item(get_selection())->get_active())
	{
		int16 selection = get_selection();
		int16 num_items = get_num_items();
		for (int16 i = 1; i < get_num_items(); i++)
		{
			int16 item_index = (selection + i) % num_items;
			if (get_item(item_index)->get_active())
			{
				set_selection(item_index);
				break;
			}
		}
	}

	for (int16 item_index = 0; item_index < get_num_items(); item_index++)
		get_item(item_index)->update();

	const gamepad_state& state = debug_menu_get_gamepad_state();
	const gamepad_state& last_state = debug_menu_get_last_gamepad_state();
	s_game_input_state* input_state;
	input_abstraction_get_input_state(_controller0, &input_state);
	if (get_enabled())
	{
		if (input_key_frames_down(_key_up_arrow, _input_type_ui) == 1 || state.button_frames[_gamepad_binary_button_dpad_up])
		{
			input_state->get_button(_button_dpad_up).latch();
			notify_up();
		}
		else if (input_key_frames_down(_key_down_arrow, _input_type_ui) == 1 || state.button_frames[_gamepad_binary_button_dpad_down])
		{
			input_state->get_button(_button_dpad_down).latch();
			notify_down();
		}
		else if (input_key_frames_down(_key_left_arrow, _input_type_ui) == 1 || state.button_frames[_gamepad_binary_button_dpad_left])
		{
			input_state->get_button(_button_dpad_left).latch();
			try_left();
		}
		else if (input_key_frames_down(_key_right_arrow, _input_type_ui) == 1 || state.button_frames[_gamepad_binary_button_dpad_right])
		{
			input_state->get_button(_button_dpad_right).latch();
			try_right();
		}
		else
		{
			m_last_up = 0;
			m_last_down = 0;
			m_last_left = 0;
			m_last_right = 0;
		}
	}

	if (get_enabled()
		&& get_num_items() > 0
		&& get_item(get_selection())->get_active()
		&& (state.button_frames[_gamepad_binary_button_a] == 1
			|| input_key_frames_down(_keypad_enter, _input_type_ui) == 1
			|| input_key_frames_down(_key_return, _input_type_ui) == 1))
	{
		input_state->get_button(_button_a).latch();
		get_item(get_selection())->notify_selected();
		notify_selected(get_selection());
	}
	else if (!state.button_frames[_gamepad_binary_button_b] &&
		last_state.button_frames[_gamepad_binary_button_b] || input_key_frames_down(_key_end, _input_type_ui) == 1)
	{
		input_state->get_button(_button_b).latch();
		close(false);
	}
	else
	{
		e_input_key_code letter_keys[26]
		{
			_key_a,
			_key_b,
			_key_c,
			_key_d,
			_key_e,
			_key_f,
			_key_g,
			_key_h,
			_key_i,
			_key_j,
			_key_k,
			_key_l,
			_key_m,
			_key_n,
			_key_o,
			_key_p,
			_key_q,
			_key_r,
			_key_s,
			_key_t,
			_key_u,
			_key_v,
			_key_w,
			_key_x,
			_key_y,
			_key_z,
		};

		int16 selection = _key_not_a_key;
		for (int16 i = _key_1; i <= _key_0; i++)
		{
			if (input_key_frames_down(e_input_key_code(i), _input_type_ui) == 1)
			{
				if (i == _key_0)
					selection = _key_f9;
				else
					selection = i - _key_1;
				break;
			}
		}

		for (int16 i = _keypad_0; i <= _keypad_9; i++)
		{
			if (input_key_frames_down(e_input_key_code(i), _input_type_ui) == 1)
			{
				if (i == _keypad_0)
					selection = _key_f9;
				else
					selection = i - _keypad_1;
				break;
			}
		}

		for (int16 i = 0; i < NUMBEROF(letter_keys); i++)
		{
			if (input_key_frames_down(letter_keys[i], _input_type_ui) == 1)
			{
				selection = i + _key_f10;
				break;
			}
		}

		if (selection != _key_not_a_key)
		{
			if (selection < get_num_items())
			{
				set_selection(selection);
				get_item(get_selection())->notify_selected();
				notify_selected(get_selection());
			}
		}
	}
}

void c_debug_menu::render(c_font_cache_base* font_cache, const point2d& point)
{
	render_background(font_cache, point);
	render_title(font_cache, point);
	render_caption(font_cache, point);
	render_global_caption(font_cache, point);
	render_items(font_cache, point, 0, get_num_items() - 1);
}

void c_debug_menu::game_render()
{
}

void c_debug_menu::notify_selected(int16 selected_value)
{
}

void c_debug_menu::open()
{
	for (int16 item_index = 0; item_index < get_num_items(); item_index++)
		get_item(item_index)->open();

	set_selection(0);
	set_enabled(true);
}

const char* c_debug_menu::get_caption()
{
	return m_caption ? m_caption : "";
}

void c_debug_menu::notify_closed()
{
}

void c_debug_menu::notify_activated()
{
}

int32 c_debug_menu::get_num_items_to_render()
{
	return get_num_items();
}

void c_debug_menu::close(bool closed)
{
	debug_menu_set_active_menu(get_parent(), false);
	notify_closed();

	if (get_parent())
	{
		if (closed)
			get_parent()->notify_selection_closed();
		else
			get_parent()->notify_selection_exited();
	}
}

int16 c_debug_menu::get_value_width()
{
	return 0;
}

void c_debug_menu::notify_selection_closed()
{
}

void c_debug_menu::notify_selection_exited()
{
}

void c_debug_menu::notify_up()
{
	if (!is_active_menu())
		return;

	if (debug_menu_get_time() - m_last_up < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	int16 selection = get_selection();
	m_last_up = debug_menu_get_time();
	
	c_debug_menu_item* item = nullptr;
	do
	{
		int16 current_selection = get_selection();
		if (get_selection() <= 0)
			current_selection = get_num_items();
		set_selection(current_selection - 1);
	
		item = get_item(get_selection());
	} while (!item->get_active());
	
	if (!get_item(get_selection())->get_active())
		set_selection(selection);
}

void c_debug_menu::notify_down()
{
	if (!is_active_menu())
		return;

	if (debug_menu_get_time() - m_last_down < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	int16 selection = get_selection();
	m_last_down = debug_menu_get_time();
	
	c_debug_menu_item* item = nullptr;
	do
	{
		int16 current_selection = get_selection();
		if (current_selection >= get_num_items() - 1)
			set_selection(0);
		else
			set_selection(get_selection() + 1);
	
		item = get_item(get_selection());
	} while (!item->get_active());
	
	if (!get_item(get_selection())->get_active())
		set_selection(selection);
}

void c_debug_menu::notify_left()
{
}

void c_debug_menu::notify_right()
{
}

bool c_debug_menu::is_active_menu()
{
	for (int16 item_index = 0; item_index < get_num_items(); item_index++)
	{
		if (get_item(item_index)->get_active())
			return true;
	}

	return false;
};

int16 c_debug_menu::get_menu_rate()
{
	return 5;
};

int16 c_debug_menu::get_max_active_captions()
{
	int16 max_active_captions = *get_caption() != 0;
	for (int16 caption_index = 0; caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS; caption_index++)
	{
		if (*debug_menu_get_caption(caption_index))
			max_active_captions = caption_index + 2;
	}

	return max_active_captions;
}

void c_debug_menu::render_background(c_font_cache_base* font_cache, const point2d& point)
{
	real32 item_margin = get_value_width() ? debug_menu_get_item_margin() : 0.0f;

	int16 x0 = int16((((point.x - debug_menu_get_item_margin()) - get_num_items_to_render()) - item_margin) - 60.0);
	int16 y0 = point.y;
	int16 x1 = int16((point.x + debug_menu_get_item_width()) + debug_menu_get_item_margin());
	int16 y1 = int16((point.y + get_title_height()) + (get_num_items_to_render() + get_max_active_captions()) * get_item_height());
	real32 alpha = get_enabled() ? 0.7f : 0.1f;

	debug_menu_draw_rect(x0, y0, x1, y1, alpha, debug_real_argb_tv_blue);
}

void c_debug_menu::render_title(c_font_cache_base* font_cache, const point2d& point)
{
	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);

	int16 x0 = point.x;
	int16 y0 = int16(point.y + debug_menu_get_item_indent_y());
	int16 x1 = int16(point.x + debug_menu_get_item_width());
	int16 y1 = int16((point.y + get_item_height()) - (2.0f * debug_menu_get_item_indent_y()));
	real32 alpha = get_enabled() ? 0.7f : 0.1f;

	debug_menu_draw_rect(x0, y0, x1, y1, alpha, debug_real_argb_grey);

	set_rectangle2d(&bounds, point.x, point.y, int16(point.x + debug_menu_get_item_width()), bounds.y1);
	draw_string.set_bounds(&bounds);
	draw_string.set_color(debug_real_argb_tv_magenta);
	draw_string.draw(font_cache, m_name);
}

void c_debug_menu::render_caption(c_font_cache_base* font_cache, const point2d& point)
{
	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);
	if (*get_caption())
	{
		int16 x0 = point.x;
		int16 y0 = int16(point.y + debug_menu_get_item_indent_y());
		int16 x1 = int16(point.x + debug_menu_get_item_width());
		int16 y1 = int16(((point.y + get_title_height()) + get_num_items_to_render() * get_item_height()) - (2.0f * debug_menu_get_item_indent_y()));
		real32 alpha = get_enabled() ? 0.7f : 0.1f;

		debug_menu_draw_rect(x0, y0, x1, y1, alpha, debug_real_argb_grey);
	}

	set_rectangle2d(&bounds, point.x, int16((point.y + get_title_height()) + get_num_items_to_render() * get_item_height()), int16(point.x + debug_menu_get_item_width()), bounds.y1);
	draw_string.draw(font_cache, get_caption());
}

void c_debug_menu::render_global_caption(c_font_cache_base* font_cache, const point2d& point)
{
	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);

	set_rectangle2d(&bounds, point.x, int16((point.y + get_title_height()) * (get_num_items_to_render() + 1) * get_item_height()), int16(point.x + debug_menu_get_item_width()), bounds.y1);
	draw_string.set_color(debug_real_argb_tv_magenta);

	for (int16 caption_index = 0; caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS; caption_index++)
	{
		if (*debug_menu_get_caption(caption_index))
		{
			int16 x0 = point.x;
			int16 y0 = int16(((point.y + get_title_height()) + ((caption_index + 1) + get_num_items_to_render()) * get_item_height()) + debug_menu_get_item_indent_y());
			int16 x1 = int16(point.x + debug_menu_get_item_width());
			int16 y1 = int16(((point.y + get_title_height()) + ((caption_index + 2) + get_num_items_to_render()) * get_item_height()) - (2.0f * debug_menu_get_item_indent_y()));
			real32 alpha = get_enabled() ? 0.7f : 0.1f;

			debug_menu_draw_rect(x0, y0, x1, y1, alpha, debug_real_argb_grey);
		}

		if (rectangle2d_width(&bounds) > 0 && rectangle2d_height(&bounds) > 0)
		{
			draw_string.set_bounds(&bounds);
			draw_string.draw(font_cache, debug_menu_get_caption(caption_index));
		}

		debug_menu_set_caption(caption_index, "");
		bounds.y0 += get_item_height();
	}
}

void c_debug_menu::render_items(c_font_cache_base* font_cache, const point2d& point, int16 start_index, int16 end_index)
{
	ASSERT(start_index >= 0);
	ASSERT(start_index <= end_index);

	for (int16 item_index = start_index; item_index <= end_index && item_index < get_num_items(); item_index++)
	{
		c_debug_menu_item* item = get_item(item_index);
		if (item->get_active())
		{
			int16 x0 = point.x;
			int16 y0 = int16(((point.y + get_title_height()) + (item_index - start_index) * get_item_height()) + debug_menu_get_item_indent_y());
			int16 x1 = int16(point.x + debug_menu_get_item_width());
			int16 y1 = int16((((point.y + get_title_height()) + (item_index - start_index + 1) * get_item_height())) - (2.0f * debug_menu_get_item_indent_y()));
			real32 alpha = get_enabled() ? 0.7f : 0.1f;

			debug_menu_draw_rect(x0, y0, x1, y1, alpha, item->get_background_color());
		}

		point2d item_point{};
		set_point2d(&item_point, point.x, point.y + (get_title_height() + (item_index - start_index) * get_item_height()));
		item->render(font_cache, item_point);
	}
}

void c_debug_menu::try_left()
{
	ASSERT(get_selection() >= 0);

	if (debug_menu_get_time() - m_last_left < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	m_last_left = debug_menu_get_time();
	notify_left();

	if (get_selection() >= get_num_items())
		return;

	get_item(get_selection())->notify_left();
}

void c_debug_menu::try_right()
{
	ASSERT(get_selection() >= 0);

	if (debug_menu_get_time() - m_last_right < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	m_last_right = debug_menu_get_time();
	notify_right();

	if (get_selection() >= get_num_items())
		return;

	get_item(get_selection())->notify_right();
}

c_debug_menu::c_debug_menu(c_debug_menu* parent, const char* name) :
	m_name(nullptr),
	m_caption(nullptr),
	m_parent_ref(parent)
{
	set_name(name ? name : "Menu???");
	set_num_items(0);
	set_selection(0);
	set_enabled(true);
}

void c_debug_menu::clear()
{
	for (int16 item_index = 0; item_index < get_num_items(); item_index++)
	{
		//unreachable

		if (c_debug_menu_item* item = m_items[item_index])
			item->~c_debug_menu_item();
		m_items[item_index] = nullptr;
	}

	set_num_items(0);
}

int16 c_debug_menu::get_num_items()
{
	ASSERT(m_num_items >= 0 && m_num_items <= k_max_items);

	return m_num_items;
}

void c_debug_menu::set_num_items(int16 num_items)
{
	ASSERT(num_items >= 0 && num_items <= k_max_items);

	m_num_items = num_items;
}

int16 c_debug_menu::get_selection()
{
	return m_selection;
}

void c_debug_menu::set_selection(int16 selection)
{
	ASSERT(selection >= 0 && (get_num_items() == 0 || selection < get_num_items()));

	m_selection = selection;
}

c_debug_menu_item* c_debug_menu::get_item(int16 item_index)
{
	ASSERT(get_num_items() >= 0 && get_num_items() < k_max_items);
	ASSERT(m_items[item_index] != nullptr);
	ASSERT(VALID_INDEX(item_index, get_num_items()));

	return m_items[item_index];
}

void c_debug_menu::add_item(c_debug_menu_item* item)
{
	ASSERT(get_num_items() < k_max_items);

	m_items[get_num_items()] = item;
	set_num_items(get_num_items() + 1);
}

const char* c_debug_menu::get_name()
{
	return m_name;
}

void c_debug_menu::set_name(const char* name)
{
	int32 name_size = strlen_debug(name) + 1;

	ASSERT(m_name == nullptr);
	m_name = static_cast<char*>(debug_menu_malloc(name_size));
	csstrnzcpy(m_name, name, name_size);
}

void c_debug_menu::set_caption(const char* caption)
{
	int32 caption_size = strlen_debug(caption) + 1;

	//ASSERT(m_caption == nullptr);
	m_caption = static_cast<char*>(debug_menu_malloc(caption_size));
	csstrnzcpy(m_caption, caption, caption_size);
}

c_debug_menu* c_debug_menu::get_parent()
{
	return m_parent_ref;
}

bool c_debug_menu::get_enabled()
{
	return m_enabled;
}

void c_debug_menu::set_enabled(bool enable)
{
	m_enabled = enable;
}

int16 c_debug_menu::get_title_height()
{
	return static_cast<int16>(debug_menu_get_title_height());
}

int16 c_debug_menu::get_item_indent()
{
	return static_cast<int16>(debug_menu_get_item_indent_x());
}

int16 c_debug_menu::get_item_height()
{
	return static_cast<int16>(debug_menu_get_item_height());
}

