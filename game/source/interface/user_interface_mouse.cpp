#include "interface/user_interface_mouse.hpp"

#include "camera/director.hpp"
#include "input/input_windows.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/interface_constants.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "shell/shell_windows.hpp"

s_user_interface_mouse_globals user_interface_mouse_globals{};

void user_interface_mouse_update()
{
	if (window_globals.hWnd == GetForegroundWindow() && director_get(0)->get_camera()->get_type() != _camera_mode_flying)
	{
		user_interface_mouse_update_internal();

		c_gui_screen_widget* topmost_screen = window_manager_get()->get_topmost_screen(_console_window);
		if (!topmost_screen)
		{
			topmost_screen = window_manager_get()->get_topmost_screen(_window0);
		}
		//if (!topmost_screen)
		//{
		//	topmost_screen = window_manager_get()->get_topmost_screen(_window1);
		//}
		//if (!topmost_screen)
		//{
		//	topmost_screen = window_manager_get()->get_topmost_screen(_window2);
		//}
		//if (!topmost_screen)
		//{
		//	topmost_screen = window_manager_get()->get_topmost_screen(_window3);
		//}

		if (topmost_screen && topmost_screen->get_focused_widget())
		{
			if (!user_interface_mouse_handle_screen_widget(topmost_screen))
			{
				user_interface_mouse_globals.last_focused_widget = NULL;
			}
		}
	}
}

void widget_bounds_from_window_bounds(const real_rectangle2d* window_bounds, real_rectangle2d* widget_bounds)
{
	real_vector2d aspect_ratio_scaling = interface_get_aspect_ratio_scaling();

	real32 bounds_x = window_bounds->x1 - window_bounds->x0;
	real32 bounds_y = window_bounds->y1 - window_bounds->y0;
	real32 adjusted_x = real32(bounds_x / 2) + window_bounds->x0;
	real32 adjusted_y = real32(bounds_y / 2) + window_bounds->y0;
	real32 normalized_x = real32(bounds_x / real32(c_rasterizer::render_globals.resolution_width));
	real32 normalized_y = real32(bounds_y / real32(c_rasterizer::render_globals.resolution_height));

	widget_bounds->x0 = ((widget_bounds->x0 * normalized_x) * aspect_ratio_scaling.i) + adjusted_x;
	widget_bounds->x1 = ((widget_bounds->x1 * normalized_x) * aspect_ratio_scaling.i) + adjusted_x;
	widget_bounds->y0 = ((widget_bounds->y0 * normalized_y) * aspect_ratio_scaling.j) + adjusted_y;
	widget_bounds->y1 = ((widget_bounds->y1 * normalized_y) * aspect_ratio_scaling.j) + adjusted_y;
}

void user_interface_mouse_update_tracking()
{
	static bool cursor_shown = false;

	bool screen_active = g_window_manager.m_active_screens->actual_count > 0;
	if (cursor_shown != screen_active)
	{
		cursor_shown = screen_active;
		ShowCursor(cursor_shown ? TRUE : FALSE);
	}

	POINT cursor_position{};
	GetCursorPos(&cursor_position);
	ScreenToClient(window_globals.hWnd, &cursor_position);

	int32 x_delta = abs(cursor_position.x - user_interface_mouse_globals.update_cursor_position.x);
	int32 y_delta = abs(cursor_position.y - user_interface_mouse_globals.update_cursor_position.y);
	user_interface_mouse_globals.update_cursor_position.x = cursor_position.x;
	user_interface_mouse_globals.update_cursor_position.y = cursor_position.y;

	static real32 mouse_move_delta = 0.0f;
	mouse_move_delta += ((real32(y_delta * y_delta + x_delta * x_delta) * 1.0f) / 60.0f);

	if (mouse_move_delta > 0.00001f)
	{
		mouse_move_delta *= 0.4f;
	}
	else
	{
		mouse_move_delta = 0.0f;
	}

	if (mouse_move_delta < 0.01f)
	{
		user_interface_mouse_globals.mouse_move_ticks = 0;
	}
	else if (user_interface_mouse_globals.mouse_move_ticks != 255)
	{
		user_interface_mouse_globals.mouse_move_ticks++;
	}
}

void user_interface_mouse_update_internal()
{
	user_interface_mouse_update_tracking();

	user_interface_mouse_globals.controller_index = _controller0;
	real32 x = real32(c_rasterizer::render_globals.resolution_offset_x);
	real32 y = real32(c_rasterizer::render_globals.resolution_offset_y);
	real32 width = real32(c_rasterizer::render_globals.window_width24);
	real32 height = real32(c_rasterizer::render_globals.window_height28);

	user_interface_mouse_globals.window_bounds.x0 = x;
	user_interface_mouse_globals.window_bounds.x1 = width - x;
	user_interface_mouse_globals.window_bounds.y0 = y;
	user_interface_mouse_globals.window_bounds.y1 = height - y;

	user_interface_mouse_globals.last_window_cursor_position = user_interface_mouse_globals.window_cursor_position;
	user_interface_mouse_globals.window_cursor_position.x = real32(user_interface_mouse_globals.update_cursor_position.x);
	user_interface_mouse_globals.window_cursor_position.y = real32(user_interface_mouse_globals.update_cursor_position.y);

	user_interface_mouse_globals.left_button_frames_down = input_mouse_frames_down(_mouse_button_left_click, _input_type_ui);
	user_interface_mouse_globals.right_button_frames_down = input_mouse_frames_down(_mouse_button_right_click, _input_type_ui);

	user_interface_mouse_globals.mouse_wheel_delta = input_globals.raw_mouse_state.wheel_delta;
	user_interface_mouse_globals.mouse_hwheel_delta = input_globals.raw_mouse_state.hwheel_delta;
}

void user_interface_mouse_compute_widget_bounds(c_gui_widget* widget, real_rectangle2d* accumulated_bounds, bool(*child_filter)(const c_gui_widget*))
{
	real_rectangle2d bounds{};
	for (c_gui_widget* child_widget = widget->get_children();
		child_widget;
		child_widget = child_widget->get_next())
	{
		if (child_filter(child_widget))
		{
			csmemset(&bounds, 0, sizeof(bounds));
			child_widget->get_current_bounds(&bounds);

			if (accumulated_bounds->x0 > bounds.x0 || accumulated_bounds->x0 == 0.0f)
			{
				accumulated_bounds->x0 = bounds.x0;
			}

			if (bounds.x1 > accumulated_bounds->x1 || accumulated_bounds->x1 == 0.0f)
			{
				accumulated_bounds->x1 = bounds.x1;
			}

			if (accumulated_bounds->y0 > bounds.y0 || accumulated_bounds->y0 == 0.0f)
			{
				accumulated_bounds->y0 = bounds.y0;
			}

			if (bounds.y1 > accumulated_bounds->y1 || accumulated_bounds->y1 == 0.0f)
			{
				accumulated_bounds->y1 = bounds.y1;
			}
		}
	}
}

void user_interface_mouse_compute_list_item_bounds(c_gui_list_item_widget* list_item_widget, real_rectangle2d* bounds)
{
	user_interface_mouse_compute_widget_bounds(list_item_widget, bounds, [](const c_gui_widget* widget) -> bool { return widget->m_visible && widget->m_type == _gui_bitmap; });

	real_point2d anchor_position{};
	anchor_position.x = list_item_widget->m_animated_state.position.x;
	anchor_position.y = list_item_widget->m_animated_state.position.y;

	bounds->x0 += anchor_position.x;
	bounds->x1 += anchor_position.x;
	bounds->y0 += anchor_position.y;
	bounds->y1 += anchor_position.y;
}

bool user_interface_mouse_handle_spinner_list_widget_focus(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget)
{
	bool result = false;
	c_gui_widget* focused_widget = screen_widget->get_focused_widget();
	if (list_widget == focused_widget->get_parent_list() && list_widget->get_parent_list() && list_widget->get_parent_list_item())
	{
		if (user_interface_mouse_globals.left_button_frames_down == 1)
		{
			event_manager_button_pressed(user_interface_mouse_globals.controller_index, _button_a);
		}

		c_gui_data* datasource = screen_widget->get_data(list_widget->m_datasource_name.get_value(), NULL);
		int32 current_item_count = datasource->get_current_item_count();
		if (current_item_count)
		{
			int32 index = list_widget->m_focused_item_index - (user_interface_mouse_globals.mouse_wheel_delta / input_globals.mouse_wheel_delta);
			int32 focused_item_index = index + current_item_count;

			if (index >= 0)
			{
				focused_item_index = index;
			}

			if (focused_item_index >= current_item_count)
			{
				focused_item_index -= current_item_count;
			}

			list_widget->set_focused_item_index(focused_item_index, true);
		}

		result = true;
	}
	return result;
}

void user_interface_mouse_handle_scroll_list_widget(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget, int32 scroll_amount)
{
	if (scroll_amount != 0 && list_widget->m_scroll_position != NONE)
	{
		if (c_gui_data* datasource = screen_widget->get_data(list_widget->m_datasource_name.get_value(), NULL))
		{
			int32 current_item_count = datasource->get_current_item_count();
			if (current_item_count >= 1)
			{
				s_runtime_list_widget_definition* definition = (s_runtime_list_widget_definition*)list_widget->get_core_definition();

				int32 max_scroll = current_item_count - definition->list_items.count;
				if (max_scroll < 0)
				{
					max_scroll = 0;
				}

				int32 scroll_position = CLAMP(list_widget->m_scroll_position + scroll_amount, 0, max_scroll);

				if (list_widget->m_scroll_position != scroll_position)
				{
					list_widget->m_focused_item_index = NONE;
				}

				list_widget->m_scroll_position = scroll_position;
			}
		}
	}
}

bool user_interface_mouse_handle_list_widget(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget)
{
	for (c_gui_list_item_widget* child_widget = (c_gui_list_item_widget*)list_widget->get_first_child_widget_by_type(_gui_list_item);
		child_widget;
		child_widget = child_widget->get_next_list_item_widget(true))
	{
		if (child_widget->can_receive_focus())
		{
			real_rectangle2d bounds{};
			user_interface_mouse_compute_list_item_bounds(child_widget, &bounds);
			widget_bounds_from_window_bounds(&user_interface_mouse_globals.window_bounds, &bounds);

			if (point_intersects_rectangle2d(&user_interface_mouse_globals.window_cursor_position, &bounds))
			{
				c_gui_list_item_widget* current_focused_widget = (c_gui_list_item_widget*)screen_widget->get_focused_widget();

				if (user_interface_mouse_globals.mouse_move_ticks && user_interface_mouse_globals.last_focused_widget != child_widget || user_interface_mouse_globals.list_item_selected_dirty)
				{
					user_interface_mouse_globals.list_item_selected_dirty = false;

					if (!current_focused_widget || current_focused_widget->get_parent_list() == list_widget)
					{
						if (current_focused_widget != child_widget)
						{
							user_interface_mouse_globals.last_focused_widget = NULL;
							screen_widget->transfer_focus_to_list(list_widget, child_widget->get_element_handle(), true, true);
						}
					}
					else
					{
						user_interface_mouse_globals.last_focused_widget = NULL;
						screen_widget->transfer_focus(list_widget);
					}
				}

				bool v19 = screen_widget->m_animated_state.state_flags >> 3 || screen_widget->m_animated_state.state_flags >> 5;
				if (user_interface_mouse_globals.left_button_frames_down == 1 && child_widget == current_focused_widget && !v19)
				{
					user_interface_mouse_globals.last_focused_widget = child_widget;
					event_manager_button_pressed(user_interface_mouse_globals.controller_index, _button_a);
				}
			}
			else if (user_interface_mouse_globals.last_focused_widget == child_widget)
			{
				user_interface_mouse_globals.last_focused_widget = NULL;
			}
		}
	}

	return false;
}

bool user_interface_mouse_handle_list_widgets(c_gui_screen_widget* screen_widget, c_gui_widget* parent_widget)
{
	bool result = false;

	if (!parent_widget)
	{
		parent_widget = screen_widget;
	}

	for (c_gui_widget* child_widget = parent_widget->get_children();
		child_widget && !result;
		child_widget = child_widget->get_next())
	{
		if (!child_widget->m_visible)
		{
			continue;
		}

		e_gui_widget_type widget_type = child_widget->m_type;
		switch (widget_type)
		{
		case _gui_list:
		{
			result = user_interface_mouse_handle_list_widget(screen_widget, (c_gui_list_widget*)child_widget);
		}
		break;
		default:
		{
			result = user_interface_mouse_handle_list_widgets(screen_widget, child_widget);
		}
		break;
		}
	}

	return result;
}

bool user_interface_mouse_handle_screen_widget(c_gui_screen_widget* screen_widget)
{
	bool handled = false;
	if (user_interface_mouse_globals.right_button_frames_down == 1)
	{
		event_manager_button_pressed(user_interface_mouse_globals.controller_index, _button_b);
		handled = true;
	}
	else
	{
		c_gui_widget* focused_widget = screen_widget->get_focused_widget();
		if (focused_widget && focused_widget->m_type == _gui_list_item)
		{
			c_gui_list_widget* parent_list_widget = focused_widget->get_parent_list();
			if (parent_list_widget && parent_list_widget->m_type == _gui_list)
			{
				if (user_interface_mouse_handle_spinner_list_widget_focus(screen_widget, parent_list_widget))
				{
					user_interface_mouse_globals.list_item_selected_dirty = true;
					handled = true;
				}
			}
		}

		if (!handled)
		{
			static c_string_id last_screen_widget_name{};
			if (screen_widget->m_name != last_screen_widget_name)
			{
				last_screen_widget_name = screen_widget->m_name;
				user_interface_mouse_globals.list_item_selected_dirty = true;
			}

			if (focused_widget && focused_widget->m_type == _gui_list_item)
			{
				c_gui_list_widget* parent_list_widget = focused_widget->get_parent_list();
				if (parent_list_widget && parent_list_widget->m_type == _gui_list)
				{
					int32 parent_focused_item_index = parent_list_widget->m_focused_item_index;
					user_interface_mouse_handle_scroll_list_widget(screen_widget, parent_list_widget, user_interface_mouse_globals.mouse_wheel_delta / -input_globals.mouse_wheel_delta);

					if (int16 hscroll_ammount = user_interface_mouse_globals.mouse_hwheel_delta / -input_globals.mouse_wheel_delta)
					{
						point2d point = { .x = hscroll_ammount > 0 ? 0x7FFF : -0x7FFF };
						event_manager_tab(0, user_interface_mouse_globals.controller_index, &point, user_interface_milliseconds(), _controller_component_stick_left_thumb);
					}
				}
			}
			handled = user_interface_mouse_handle_list_widgets(screen_widget, NULL) != false;
		}
	}
	return handled;
}

