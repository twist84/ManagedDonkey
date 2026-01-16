#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_gui_alert_description
{
	c_string_id error_name;
	c_flags<e_gui_alert_flags, uns8, k_gui_alert_flags> flags;
	c_enum<e_gui_error_category, uns8, _gui_error_category_default, k_gui_error_category_count> error_category;
	c_enum<e_gui_error_icon, uns8, _gui_error_icon_default_alert, k_gui_error_icon_count> error_icon;
	byte pad0[1];
	c_string_id title;
	c_string_id message;
};
COMPILE_ASSERT(sizeof(s_gui_alert_description) == 0x10);

class c_gui_queued_error
{
public:
	c_gui_queued_error();

	void set(const s_gui_alert_description* alert_description, const wchar_t* custom_title, const wchar_t* custom_message, e_controller_index controller_index, int32 posted_time, bool requires_resolution, bool blocking);
	bool match(e_controller_index controller_index, int32 error_name, const wchar_t* custom_message) const;
	bool is_valid() const;
	void clear();
	bool get_resolved() const;
	void set_resolved(bool resolved);
	int32 get_error_name() const;
	e_gui_error_category get_error_category() const;
	bool get_auto_dismissable() const;
	bool get_requires_resolution() const;
	bool get_blocking() const;
	int32 get_posted_time() const;
	void set_posted_time(int32 posted_time);
	const wchar_t* get_custom_title() const;
	const wchar_t* get_custom_message() const;
	e_controller_index get_controller_index() const;

protected:
	bool m_is_valid;
	bool m_requires_resolution;
	bool m_auto_dismissable;
	bool m_show_spinner;
	c_string_id m_error_name;
	c_enum<e_gui_error_category, int32, _gui_error_category_default, k_gui_error_category_count> m_error_category;
	c_enum<e_gui_error_icon, int32, _gui_error_icon_default_alert, k_gui_error_icon_count> m_error_icon;
	int32 m_posted_time;
	c_string_id m_title;
	c_string_id m_message;
	c_enum<e_controller_index, int32, _controller0, k_number_of_controllers> m_controller_index;
	c_static_wchar_string<256> m_custom_title;
	c_static_wchar_string<256> m_custom_message;
	bool m_blocking;
};
COMPILE_ASSERT(sizeof(c_gui_queued_error) == 0x424);

class c_gui_error_manager
{
public:
	c_gui_error_manager();

	bool any_error_active_for_controller(e_controller_index controller_index);
	bool any_error_active_for_window(e_window_index window_index);
	void clear_all_errors();
	void clear_error(int32 error_name, e_controller_index controller_index);
	void clear_error_with_custom_message(int32 error_name, e_controller_index controller_index, const wchar_t* custom_message);
	void dismiss_auto_dismissable_errors(e_controller_index controller_index);
	void dispose_from_old_map();
	const c_gui_queued_error* get_error(e_controller_index controller_index, int32 error_name, const wchar_t* custom_message) const;
	void initialize();
	void initialize_for_new_map();
	void post_error(int32 error_name, e_controller_index controller_index, bool requires_resolution);
	void post_error_with_custom_message(int32 error_name, const wchar_t* custom_message, e_controller_index controller_index, bool requires_resolution);
	void post_toast(int32 error_name);
	void post_toast_with_custom_message(int32 error_name, const wchar_t* custom_title, const wchar_t* custom_message);
	void resolve_error(int32 error_name, e_controller_index controller_index);
	void resolve_error_with_custom_message(int32 error_name, e_controller_index controller_index, const wchar_t* custom_message);
	void update(uns32 current_milliseconds);
	
private:
	enum e_alert_display_mode
	{
		_alert_display_mode_popup = 0,
		_alert_display_mode_toast,
		_alert_display_mode_in_render_window,

		k_alert_display_mode_count
	};

	void clean_out_error_queue();
	const c_gui_queued_error* get_current_for_user(e_controller_index controller_index);
	const c_gui_queued_error* get_current_for_window(e_window_index window_index);
	e_alert_display_mode get_error_display_mode(e_controller_index* controller_index);
	void post_error_with_custom_message(int32 error_name, const wchar_t* custom_title, const wchar_t* custom_message, e_controller_index controller_index, bool requires_resolution, bool blocking);
	void sort_queue();

protected:
	c_gui_queued_error m_error_queue[32];
	e_alert_display_mode m_last_error_display_mode;
	bool m_utility_drive_cleared_message_shown;
};
COMPILE_ASSERT(sizeof(c_gui_error_manager) == 0x8488);

extern c_gui_error_manager& g_gui_error_manager;

extern int __cdecl queued_error_sort_proc(const void* a, const void* b);
extern c_gui_error_manager* __cdecl user_interface_error_manager_get();

