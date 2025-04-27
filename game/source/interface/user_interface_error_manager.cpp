#include "interface/user_interface_error_manager.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_mapping.hpp"
#include "interface/user_interface_session.hpp"
#include "tag_files/string_ids.hpp"

#include <search.h>
#include <string.h>

REFERENCE_DECLARE(0x05257C48, c_gui_error_manager, g_gui_error_manager);

c_gui_queued_error::c_gui_queued_error() : 
	m_is_valid(false),
	m_custom_title(),
	m_custom_message()
{
}

void c_gui_queued_error::set(s_gui_alert_description const* alert_description, wchar_t const* custom_title, wchar_t const* custom_message, e_controller_index controller_index, int32 posted_time, bool requires_resolution, bool blocking)
{
	m_error_name = alert_description->error_name;
	m_error_category = alert_description->error_category;
	m_error_icon = alert_description->error_icon;
	m_title = alert_description->title;
	m_message = alert_description->message;
	m_controller_index = controller_index;
	m_auto_dismissable = alert_description->flags.test(_gui_alert_flag_allow_auto_dismissal_bit);
	m_show_spinner = alert_description->flags.test(_gui_alert_flag_show_spinner_bit);
	m_custom_title.set(custom_title ? custom_title : L"");
	m_custom_message.set(custom_message ? custom_message : L"");
	m_is_valid = true;
	m_requires_resolution = requires_resolution;
	m_blocking = blocking;
	m_posted_time = posted_time;
}

bool c_gui_queued_error::match(e_controller_index controller_index, int32 error_name, wchar_t const* custom_message) const
{
	if (is_valid()
		&& get_error_name() == error_name
		&& (controller_index == k_any_controller
			|| get_controller_index() == controller_index
			|| get_controller_index() == k_any_controller))
	{
		if (m_custom_message.length() > 0 || custom_message && wcslen(custom_message))
			return custom_message && m_custom_message.is_equal(custom_message);

		return true;
	}

	return false;
}

bool c_gui_queued_error::is_valid() const
{
	return m_is_valid;
}

void c_gui_queued_error::clear()
{
	m_is_valid = false;
}

bool c_gui_queued_error::get_resolved() const
{
	ASSERT(is_valid());

	return !m_requires_resolution;
}

void c_gui_queued_error::set_resolved(bool resolved)
{
	ASSERT(is_valid());

	m_requires_resolution = !resolved;
}

int32 c_gui_queued_error::get_error_name() const
{
	ASSERT(is_valid());

	return m_error_name.get_value();
}

e_gui_error_category c_gui_queued_error::get_error_category() const
{
	ASSERT(is_valid());

	return m_error_category;
}

bool c_gui_queued_error::get_auto_dismissable() const
{
	ASSERT(is_valid());

	return m_auto_dismissable;
}

bool c_gui_queued_error::get_requires_resolution() const
{
	ASSERT(is_valid());

	return m_requires_resolution;
}

bool c_gui_queued_error::get_blocking() const
{
	ASSERT(is_valid());

	return m_blocking;
}

int32 c_gui_queued_error::get_posted_time() const
{
	ASSERT(is_valid());

	return m_posted_time;
}

void c_gui_queued_error::set_posted_time(int32 posted_time)
{
	m_posted_time = posted_time;
}

wchar_t const* c_gui_queued_error::get_custom_title() const
{
	ASSERT(is_valid());

	return m_custom_title.length() > 0 ? m_custom_title.get_string() : NULL;
}

wchar_t const* c_gui_queued_error::get_custom_message() const
{
	ASSERT(is_valid());

	return m_custom_message.length() > 0 ? m_custom_message.get_string() : NULL;
}

e_controller_index c_gui_queued_error::get_controller_index() const
{
	ASSERT(is_valid());

	return m_controller_index;
}

c_gui_error_manager::c_gui_error_manager() :
	m_errors(),
	m_display_mode(_alert_display_mode_toast),
	__unknown8484(false)
{
}

bool c_gui_error_manager::any_error_active_for_controller(e_controller_index controller_index)
{
	return get_current_for_user(controller_index) != NULL;
}

bool c_gui_error_manager::any_error_active_for_window(e_window_index window_index)
{
	return get_current_for_window(window_index) != NULL;
}

void c_gui_error_manager::clear_all_errors()
{
	for (c_gui_queued_error& error : m_errors)
	{
		error.clear();
	}
}

void c_gui_error_manager::clear_error(int32 error_name, e_controller_index controller_index)
{
	clear_error_with_custom_message(error_name, controller_index, NULL);
}

void c_gui_error_manager::clear_error_with_custom_message(int32 error_name, e_controller_index controller_index, wchar_t const* custom_message)
{
	bool cleared = false;
	for (c_gui_queued_error& error : m_errors)
	{
		if (error.match(controller_index, error_name, custom_message))
		{
			cleared = true;
			error.clear();
			break;
		}
	}

	event(_event_message, "ui:errors: %s error '%s' controller #%d",
		cleared ? "cleared" : "tried to clear nonexistent",
		string_id_get_string_const(error_name),
		controller_index);
}

void c_gui_error_manager::dismiss_auto_dismissable_errors(e_controller_index controller_index)
{
	for (c_gui_queued_error& error : m_errors)
	{
		if (error.is_valid() && (controller_index == k_any_controller || error.get_controller_index() == controller_index))
		{
			error.clear();
		}
	}
}

void c_gui_error_manager::dispose_from_old_map()
{
}

c_gui_queued_error const* c_gui_error_manager::get_error(e_controller_index controller_index, int32 error_name, wchar_t const* custom_message) const
{
	for (c_gui_queued_error const& error : m_errors)
	{
		if (error.match(controller_index, error_name, custom_message))
		{
			return &error;
		}
	}

	return NULL;
}

void c_gui_error_manager::initialize()
{
	for (c_gui_queued_error& error : m_errors)
	{
		error.clear();
	}
}

void c_gui_error_manager::initialize_for_new_map()
{
	if (s_user_interface_shared_globals const* shared_globals = user_interface_shared_tag_globals_try_and_get())
	{
		for (c_gui_queued_error& error : m_errors)
		{
			if (!error.is_valid())
				continue;

			for (s_gui_alert_description const& alert_description : shared_globals->alert_descriptions)
			{
				if (alert_description.error_name.get_value() == error.get_error_name())
				{
					error.set(&alert_description,
						error.get_custom_title(),
						error.get_custom_message(),
						error.get_controller_index(),
						error.get_posted_time(),
						error.get_requires_resolution(),
						error.get_blocking());
					break;
				}
			}
		}
	}

	//if (!__unknown8484 && shell_utility_drive_was_cleared_on_startup())
	//{
	//	post_error(STRING_ID(gui_alert, utility_drive_was_cleared), k_any_controller, false);
	//	__unknown8484 = true;
	//}
}

void c_gui_error_manager::post_error(int32 error_name, e_controller_index controller_index, bool requires_resolution)
{
	post_error_with_custom_message(error_name, NULL, NULL, controller_index, requires_resolution, true);
}

void c_gui_error_manager::post_error_with_custom_message(int32 error_name, wchar_t const* custom_message, e_controller_index controller_index, bool requires_resolution)
{
	post_error_with_custom_message(error_name, NULL, custom_message, controller_index, requires_resolution, true);
}

void c_gui_error_manager::post_toast(int32 error_name)
{
	post_toast_with_custom_message(error_name, NULL, NULL);
}

void c_gui_error_manager::post_toast_with_custom_message(int32 error_name, wchar_t const* custom_title, wchar_t const* custom_message)
{
	post_error_with_custom_message(error_name, custom_title, custom_message, k_any_controller, false, false);

	if (game_options_valid() && !game_is_ui_shell())
		event(_event_message, "ui:errors: posted a toast while ingame - shouldn't be doing that");
}

void c_gui_error_manager::resolve_error(int32 error_name, e_controller_index controller_index)
{
	resolve_error_with_custom_message(error_name, controller_index, NULL);
}

void c_gui_error_manager::resolve_error_with_custom_message(int32 error_name, e_controller_index controller_index, wchar_t const* custom_message)
{
	for (c_gui_queued_error& error : m_errors)
	{
		if (error.match(controller_index, error_name, custom_message))
		{
			error.set_resolved(true);
			break;
		}
	}
}

void c_gui_error_manager::update(uint32 ui_time)
{
	INVOKE_CLASS_MEMBER(0x00A92340, c_gui_error_manager, update, ui_time);

	//e_controller_index controller_index;
	//e_alert_display_mode display_mode = c_gui_error_manager::get_error_display_mode(&controller_index);
	//ASSERT(display_mode == _alert_display_mode_toast || display_mode == _alert_display_mode_in_render_window);
	//
	//struct
	//{
	//	c_gui_queued_error const* error;
	//	string_id name;
	//} alerts[k_number_of_render_windows]{};
	//
	//clean_out_error_queue();
	//sort_queue();
	//
	//if (display_mode == _alert_display_mode_toast)
	//{
	//	ASSERT(VALID_CONTROLLER(controller_index) || controller_index == k_any_controller);
	//
	//	c_gui_queued_error const* error_ = get_current_for_user(controller_index);
	//
	//	alerts[_console_window] =
	//	{
	//		.error = error_,
	//		.name = !error_ || error_->get_blocking() ? STRING_ID(gui, gui_alert_toast) : STRING_ID(gui, gui_alert_nonblocking)
	//	};
	//}
	//for (int32 window_index = 0; window_index < k_number_of_render_windows; window_index++)
	//{
	//	c_gui_queued_error const* error = get_current_for_user(controller_index);
	//
	//	alerts[window_index].error = error;
	//
	//	if (window_index == _console_window)
	//	{
	//		alerts[_console_window].name = !error || error->get_blocking() ? STRING_ID(gui, gui_alert_ingame_full) : STRING_ID(gui, gui_alert_nonblocking);
	//	}
	//	else
	//	{
	//		alerts[window_index].name = (user_interface_local_player_count() > 1) + STRING_ID(gui, gui_alert_ingame_full);
	//	}
	//}
	//
	//for (int32 window_index = 0; window_index < k_number_of_render_windows; window_index++)
	//{
	//	c_gui_queued_error const* error = alerts[window_index].error;
	//
	//	// $TODO: implement this loop
	//}
	//
	//m_display_mode = display_mode;
}

void c_gui_error_manager::clean_out_error_queue()
{
	e_controller_index controller_index;
	e_alert_display_mode display_mode = c_gui_error_manager::get_error_display_mode(&controller_index);
	for (c_gui_queued_error& error : m_errors)
	{
		if (!error.is_valid() || error.get_controller_index() == k_any_controller)
			continue;

		if (controller_get(error.get_controller_index())->in_use())
		{
			if (display_mode == _alert_display_mode_toast
				&& controller_index != k_any_controller
				&& error.get_controller_index() != controller_index
				&& error.get_auto_dismissable())
			{
				error.clear();
			}
		}
		else
		{
			error.clear();
		}
	}
}

c_gui_queued_error const* c_gui_error_manager::get_current_for_user(e_controller_index controller_index)
{
	for (c_gui_queued_error& error : m_errors)
	{
		if (error.is_valid() && (controller_index == k_any_controller || error.get_controller_index() == k_any_controller || error.get_controller_index() == controller_index))
		{
			return &error;
		}
	}

	return NULL;
}

c_gui_queued_error const* c_gui_error_manager::get_current_for_window(e_window_index window_index)
{
	for (c_gui_queued_error& error : m_errors)
	{
		if (error.is_valid())
		{
			e_controller_index controller_index = error.get_controller_index();
			if (controller_index == k_any_controller && window_index == _console_window
				|| controller_index != k_any_controller
				&& user_interface_get_window_for_controller(controller_index) == window_index)
			{
				return &error;
			}
		}
	}

	return NULL;
}

c_gui_error_manager::e_alert_display_mode c_gui_error_manager::get_error_display_mode(e_controller_index* controller_index)
{
	return INVOKE_CLASS_MEMBER(0x00A91CE0, c_gui_error_manager, get_error_display_mode, controller_index);
}

void c_gui_error_manager::post_error_with_custom_message(int32 error_name, wchar_t const* custom_title, wchar_t const* custom_message, e_controller_index controller_index, bool requires_resolution, bool blocking)
{
	//return INVOKE_CLASS_MEMBER(0x00A91FD0, c_gui_error_manager, post_error_with_custom_message, error_name, custom_title, custom_message, controller_index, requires_resolution, blocking);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers) || controller_index == k_any_controller);

	for (c_gui_queued_error& error : m_errors)
	{
		if (error.match(controller_index, error_name, custom_message)
			&& error.get_controller_index() == controller_index
			&& error.get_error_name() == error_name)
		{
			if (requires_resolution && error.get_resolved())
			{
				error.set_resolved(false);
				error.set_posted_time(user_interface_milliseconds());
			}
			return;
		}
	}

	bool can_add_new_error = false;
	for (c_gui_queued_error& error : m_errors)
	{
		if (!error.is_valid())
			can_add_new_error = true;
	}

	if (!can_add_new_error)
	{
		event(_event_warning, "ui:errors: unable to add new error!  too much shit has already hit the fan.");
		return;
	}

	for (c_gui_queued_error& error : m_errors)
	{
		if (!error.is_valid())
		{
			if (s_user_interface_shared_globals const* shared_globals = user_interface_shared_tag_globals_try_and_get())
			{
				bool found_alert_description = false;
				for (s_gui_alert_description const& alert_description : shared_globals->alert_descriptions)
				{
					if (alert_description.error_name.get_value() == error_name)
					{
						error.set(&alert_description, custom_title, custom_message, controller_index, user_interface_milliseconds(), requires_resolution, blocking);

						found_alert_description = true;

						event(_event_message, "automation:errors: new error '%s' posted", string_id_get_string_const(error_name));
						//telnet_console_printf("automation:errors: new error '%s' posted", string_id_get_string_const(error_name));
						break;
					}
				}

				if (!found_alert_description)
				{
					event(_event_warning, "ui:errors: error was unable to be added because we couldn't find the alert description '%s' in ui globals",
						string_id_get_string_const(error_name));
				}
			}
			break;
		}
	}
}

int __cdecl queued_error_sort_proc(void const* a, void const* b)
{
	c_gui_queued_error const* error_a = static_cast<c_gui_queued_error const*>(a);
	c_gui_queued_error const* error_b = static_cast<c_gui_queued_error const*>(b);

	if (!error_a->is_valid())
		return error_b->is_valid();

	if (!error_b->is_valid())
		return NONE;

	if (error_a->get_error_category() > error_b->get_error_category())
		return NONE;

	if (error_a->get_error_category() < error_b->get_error_category())
		return true;

	if (error_a->get_posted_time() < error_b->get_posted_time())
		return NONE;

	return error_a->get_posted_time() > error_b->get_posted_time();
}

void c_gui_error_manager::sort_queue()
{
	qsort(&m_errors, m_errors.get_count(), sizeof(c_gui_queued_error), queued_error_sort_proc);
}

c_gui_error_manager* __cdecl user_interface_error_manager_get()
{
	return &g_gui_error_manager;
}

