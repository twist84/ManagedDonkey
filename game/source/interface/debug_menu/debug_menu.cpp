#include "interface/debug_menu/debug_menu.hpp"

#include "cseries/cseries.hpp"
#include "input/input.hpp"
#include "input/input_abstraction.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"

#include <string.h>


c_debug_menu::~c_debug_menu()
{
	clear();
}

void c_debug_menu::update()
{
	if (get_selection() >= 0 && get_selection() < get_num_items() && !get_item(get_selection())->get_active())
	{
		short selection = get_selection();
		short num_items = get_num_items();
		for (short i = 1; i < get_num_items(); i++)
		{
			short item_index = (selection + i) % num_items;
			if (get_item(item_index)->get_active())
			{
				set_selection(item_index);
				break;
			}
		}
	}

	for (short item_index = 0; item_index < get_num_items(); item_index++)
		get_item(item_index)->update();

	gamepad_state const& state = debug_menu_get_gamepad_state();
	gamepad_state const& last_state = debug_menu_get_last_gamepad_state();
	s_game_input_state* input_state;
	input_abstraction_get_input_state(_controller_index0, &input_state);
	if (get_enabled())
	{
		if (input_key_frames_down(_key_code_up, _input_type_ui) == 1 || state.buttons_down_frames[0])
		{
			input_state->abstract_buttons[_gamepad_button_dpad_up].latch();
			notify_up();
		}
		else if (input_key_frames_down(_key_code_down, _input_type_ui) == 1 || state.buttons_down_frames[1])
		{
			input_state->abstract_buttons[_gamepad_button_dpad_down].latch();
			notify_down();
		}
		else if (input_key_frames_down(_key_code_left, _input_type_ui) == 1 || state.buttons_down_frames[2])
		{
			input_state->abstract_buttons[_gamepad_button_dpad_left].latch();
			try_left();
		}
		else if (input_key_frames_down(_key_code_right, _input_type_ui) == 1 || state.buttons_down_frames[3])
		{
			input_state->abstract_buttons[_gamepad_button_dpad_right].latch();
			try_right();
		}
		else
		{
			m_up_time = 0;
			m_down_time = 0;
			m_left_time = 0;
			m_right_time = 0;
		}
	}

	if (get_enabled()
		&& get_num_items() > 0
		&& get_item(get_selection())->get_active()
		&& (state.buttons_down_frames[8] == 1
			|| input_key_frames_down(_key_code_keypad_enter, _input_type_ui) == 1
			|| input_key_frames_down(_key_code_enter, _input_type_ui) == 1))
	{
		input_state->abstract_buttons[_gamepad_button_a].latch();
		get_item(get_selection())->notify_selected();
		notify_selected(get_selection());
	}
	else if (!state.buttons_down_frames[9] &&
		last_state.buttons_down_frames[9] || input_key_frames_down(_key_code_end, _input_type_ui) == 1)
	{
		input_state->abstract_buttons[_gamepad_button_b].latch();
		close(false);
	}
	else
	{
		e_key_code letter_keys[26]
		{
			_key_code_a,
			_key_code_b,
			_key_code_c,
			_key_code_d,
			_key_code_e,
			_key_code_f,
			_key_code_g,
			_key_code_h,
			_key_code_i,
			_key_code_j,
			_key_code_k,
			_key_code_l,
			_key_code_m,
			_key_code_n,
			_key_code_o,
			_key_code_p,
			_key_code_q,
			_key_code_r,
			_key_code_s,
			_key_code_t,
			_key_code_u,
			_key_code_v,
			_key_code_w,
			_key_code_x,
			_key_code_y,
			_key_code_z,
		};

		short selection = k_key_code_none;
		for (short i = _key_code_1; i <= _key_code_0; i++)
		{
			if (input_key_frames_down(e_key_code(i), _input_type_ui) == 1)
			{
				if (i == _key_code_0)
					selection = _key_code_f9;
				else
					selection = i - _key_code_1;
				break;
			}
		}

		for (short i = _key_code_keypad_0; i <= _key_code_keypad_9; i++)
		{
			if (input_key_frames_down(e_key_code(i), _input_type_ui) == 1)
			{
				if (i == _key_code_keypad_0)
					selection = _key_code_f9;
				else
					selection = i - _key_code_keypad_1;
				break;
			}
		}

		for (short i = 0; i < NUMBEROF(letter_keys); i++)
		{
			if (input_key_frames_down(letter_keys[i], _input_type_ui) == 1)
			{
				selection = i - _key_code_f10;
				break;
			}
		}

		if (selection != k_key_code_none)
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

void c_debug_menu::render(c_font_cache_base* font_cache, int16_point2d* point)
{
	//render_background(font_cache, point);
	//render_title(font_cache, point);
	//render_caption(font_cache, point);
	//render_global_caption(font_cache, point);
	//render_items(font_cache, point, 0 /* start_index */, get_num_items() - 1 /* end_index */);
}

void c_debug_menu::game_render()
{
}

void c_debug_menu::notify_selected(short selected_value)
{
}

void c_debug_menu::open()
{
	for (short item_index = 0; item_index < get_num_items(); item_index++)
		get_item(item_index)->open();

	set_selection(0);
	set_enabled(true);
}

char const* c_debug_menu::get_caption()
{
	return m_caption ? m_caption : "";
}

void c_debug_menu::notify_closed()
{
}

void c_debug_menu::notify_activated()
{
}

long c_debug_menu::get_num_items_to_render()
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

short c_debug_menu::get_value_width()
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

	if (debug_menu_get_time() - m_up_time < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	short selection = get_selection();
	m_up_time = debug_menu_get_time();
	
	c_debug_menu_item* item = NULL;
	do
	{
		short current_selection = get_selection();
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

	if (debug_menu_get_time() - m_down_time < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	short selection = get_selection();
	m_down_time = debug_menu_get_time();
	
	c_debug_menu_item* item = NULL;
	do
	{
		short current_selection = get_selection();
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
	for (short item_index = 0; item_index < get_num_items(); item_index++)
	{
		if (get_item(item_index)->get_active())
			return true;
	}

	return false;
};

short c_debug_menu::get_menu_rate()
{
	return 5;
};

short c_debug_menu::get_max_active_captions()
{
	short max_active_captions = *get_caption() != 0;
	for (short caption_index = 0; caption_index < DEBUG_MENU_NUM_GLOBAL_CAPTIONS; caption_index++)
	{
		if (*debug_menu_get_caption(caption_index))
			max_active_captions = caption_index + 2;
	}

	return max_active_captions;
}

void c_debug_menu::try_left()
{
	ASSERT(get_selection() >= 0);

	if (debug_menu_get_time() - m_left_time < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	m_left_time = debug_menu_get_time();
	notify_left();

	if (get_selection() >= get_num_items())
		return;

	get_item(get_selection())->notify_left();
}

void c_debug_menu::try_right()
{
	ASSERT(get_selection() >= 0);

	if (debug_menu_get_time() - m_right_time < get_menu_rate())
		return;

	if (!get_num_items())
		return;

	m_right_time = debug_menu_get_time();
	notify_right();

	if (get_selection() >= get_num_items())
		return;

	get_item(get_selection())->notify_right();
}

c_debug_menu::c_debug_menu(c_debug_menu* parent, char const* name) :
	m_name(),
	m_caption(),
	m_parent(parent)
{
	set_name(name ? name : "Menu???");
	set_num_items(0);
	set_selection(0);
	set_enabled(true);
}

void c_debug_menu::clear()
{
	for (short item_index = 0; item_index < get_num_items(); item_index++)
	{
		//unreachable

		if (c_debug_menu_item* item = m_items[item_index])
			item->~c_debug_menu_item();
		m_items[item_index] = NULL;
	}

	set_num_items(0);
}

short c_debug_menu::get_num_items()
{
	ASSERT(m_num_items >= 0 && m_num_items <= k_max_items);

	return m_num_items;
}

void c_debug_menu::set_num_items(short num_items)
{
	ASSERT(num_items >= 0 && num_items <= k_max_items);

	m_num_items = num_items;
}

short c_debug_menu::get_selection()
{
	return m_selection;
}

void c_debug_menu::set_selection(short selection)
{
	ASSERT(selection >= 0 && (get_num_items() == 0 || selection < get_num_items()));

	m_selection = selection;
}

c_debug_menu_item* c_debug_menu::get_item(short item_index)
{
	ASSERT(get_num_items() >= 0 && get_num_items() < k_max_items);
	ASSERT(m_items[item_index] != NULL);
	ASSERT(VALID_INDEX(item_index, get_num_items()));

	return m_items[item_index];
}

void c_debug_menu::add_item(c_debug_menu_item* item)
{
	ASSERT(get_num_items() < k_max_items);

	m_items[get_num_items()] = item;
	set_num_items(get_num_items() + 1);
}

char const* c_debug_menu::get_name()
{
	return m_name;
}

void c_debug_menu::set_name(char const* name)
{
	long name_size = strlen(name) + 1;

	ASSERT(m_name == NULL);
	m_name = static_cast<char*>(debug_menu_malloc(name_size));
	csstrnzcpy(m_name, name, name_size);
}

void c_debug_menu::set_caption(char const* caption)
{
	long caption_size = strlen(caption) + 1;

	ASSERT(m_caption == NULL);
	m_caption = static_cast<char*>(debug_menu_malloc(caption_size));
	csstrnzcpy(m_caption, caption, caption_size);
}

c_debug_menu* c_debug_menu::get_parent()
{
	return m_parent;
}

bool c_debug_menu::get_enabled()
{
	return m_enabled;
}

void c_debug_menu::set_enabled(bool enable)
{
	m_enabled = enable;
}

