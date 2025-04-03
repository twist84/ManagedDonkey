#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "shell/shell.hpp"

struct c_gui_screen_widget;

template<typename t_class>
void ui_track_delete(t_class const* object)
{
}

struct c_message
{
public:
	c_message(e_ui_message_type type, long screen_name, e_controller_index controller, e_window_index window);
	virtual ~c_message();
	virtual void initialize();
	virtual void update();

	void* operator new(unsigned int size);

	e_ui_message_type get_type() const;
	long get_screen_name() const;
	e_controller_index get_controller() const;
	e_window_index get_window() const;
	long get_game_time_at_creation() const;

protected:
	long m_game_time_at_creation;
	e_ui_message_type m_type;
	long m_screen_name;
	e_controller_index m_controller;
	e_window_index m_window;
};
static_assert(sizeof(c_message) == 0x18);

//_ui_message_type_controller_input
struct c_controller_input_message :
	public c_message
{
public:
	c_controller_input_message(long screen_name, e_controller_index controller, e_window_index window, e_event_type event_type, e_controller_component component, long event_value);
	virtual ~c_controller_input_message();

	e_event_type get_event_type() const;
	e_controller_component get_component() const;
	long get_event_value() const;

protected:
	e_event_type m_event_type;
	e_controller_component m_component;
	long m_event_value;
	long m_from_automation;
};
static_assert(sizeof(c_controller_input_message) == sizeof(c_message) + 0x10);

//_ui_message_type_xenon
struct c_xenon_message :
	public c_message
{
	enum e_xenon_message_type;

public:
	c_xenon_message(e_controller_index controller, e_xenon_message_type xenon_message_type, long event_value);
	virtual ~c_xenon_message();

	e_xenon_message_type get_xenon_message_type() const;
	long get_event_value() const;

protected:
	e_xenon_message_type m_xenon_message_type;
	long m_event_value;
};
static_assert(sizeof(c_xenon_message) == sizeof(c_message) + 0x8);

enum e_screen_transition_type;

//_ui_message_type_load_screen
struct c_load_screen_message :
	public c_message
{
public:
	c_load_screen_message(long screen_name, e_controller_index controller, e_window_index window, long layered_position);
	virtual ~c_load_screen_message();

	virtual void apply_initial_state(c_gui_screen_widget* screen) const;

	void set_focus_on_load_by_name(long list_name, long column_name, long column_value);
	void set_transition_type(e_screen_transition_type transition_type);
	void set_focus_on_load(long list_name, long element_handle);
	void set_parent_screen_index(long parent_screen_index);

	e_screen_transition_type get_transition_type() const;
	bool get_respond_to_controller_events() const;
	long get_focus_on_load_list_name() const;
	long get_focus_on_load_element_handle() const;
	long get_focus_on_load_column_name() const;
	long get_focus_on_load_column_value() const;
	long get_parent_screen_index() const;
	long get_layered_position() const;
	bool get_applies_even_to_codeless_screens() const;

protected:
	e_screen_transition_type m_transition_type;
	bool m_respond_to_controller_events;
	long m_focus_on_load_name;
	long m_focus_on_load_element_handle;
	long m_focus_on_load_column_name;
	long m_focus_on_load_column_value;
	long m_parent_screen_index;
	long m_layered_position;
	bool m_applies_even_to_codeless_screens;
};
static_assert(sizeof(c_load_screen_message) == sizeof(c_message) + 0x24);

//_ui_message_type_screen_custom
struct c_screen_custom_message :
	public c_message
{
public:
	c_screen_custom_message(long sub_type, long screen_name, e_controller_index controller, e_window_index window);
	virtual ~c_screen_custom_message();

	long get_sub_type() const;

protected:
	long m_sub_type;
};
static_assert(sizeof(c_screen_custom_message) == sizeof(c_message) + 0x4);

enum e_gui_dialog_choice;

//_ui_message_type_dialog_result
struct c_dialog_result_message :
	public c_message
{
public:
	c_dialog_result_message(long screen_name, e_controller_index controller, e_window_index window, long dialog_name, e_gui_dialog_choice dialog_result);
	virtual ~c_dialog_result_message();

	long get_dialog_name() const;
	e_gui_dialog_choice get_dialog_result() const;
	long get_dispose_on_success_screen_index() const;
	void set_dispose_on_success_screen_index(long dispose_on_success_screen_index);

protected:
	e_gui_dialog_choice m_dialog_result;
	long m_dialog_name;
	long m_dispose_on_success_screen_index;
};
static_assert(sizeof(c_dialog_result_message) == sizeof(c_message) + 0xC);

struct c_load_terminal_screen_message :
	public c_load_screen_message
{
public:
	c_load_terminal_screen_message(e_controller_index controller, e_window_index window, long layered_position, long initial_state);
	virtual ~c_load_terminal_screen_message();

protected:
	long m_initial_state; // apply_initial_state
};
static_assert(sizeof(c_load_terminal_screen_message) == sizeof(c_load_screen_message) + 0x4);

struct c_load_alert_screen_message :
	public c_load_screen_message
{
public:
	c_load_alert_screen_message(long screen_name, e_controller_index controller, e_window_index window, c_gui_queued_error* error);
	virtual ~c_load_alert_screen_message();

protected:
	c_gui_queued_error m_error; // apply_initial_state
};
static_assert(sizeof(c_load_alert_screen_message) == sizeof(c_load_screen_message) + 0x424);

struct c_load_dialog_screen_message :
	public c_load_screen_message
{
public:
	c_load_dialog_screen_message(e_controller_index controller, e_window_index window, long layered_position, long dialog_name, long dialog_invoker);
	virtual ~c_load_dialog_screen_message();

	long get_dialog_screen_name(long dialog_name);
	void set_dispose_on_success_screen_index(long dispose_on_success_screen_index);
	void set_test_mode(bool test_mode);

protected:
	long m_dialog_name;
	long m_dialog_invoker;
	bool m_test_mode;
	long m_dispose_on_success_screen_index;
};
static_assert(sizeof(c_load_dialog_screen_message) == sizeof(c_load_screen_message) + 0x10);

struct c_load_game_browser_screen_message :
	public c_load_screen_message
{
public:
	c_load_game_browser_screen_message(long screen_name, e_controller_index controller, e_window_index window, long layered_position, long search_flags, e_browser_type type);
	virtual ~c_load_game_browser_screen_message();

protected:
	dword_flags m_squad_search_flags;
	c_enum<e_browser_type, long, _browse_system_link, k_number_of_game_browser_types> m_type;
};
static_assert(sizeof(c_load_game_browser_screen_message) == sizeof(c_load_screen_message) + 0x8);

enum e_gui_selected_item_type;
struct c_load_pregame_selection_screen_message :
	public c_load_screen_message
{
public:
	c_load_pregame_selection_screen_message(e_controller_index controller, e_window_index window, long layered_position, e_gui_selected_item_type selection_type);
	virtual ~c_load_pregame_selection_screen_message();
	virtual void apply_initial_state(c_gui_screen_widget*) const;

protected:
	e_gui_selected_item_type m_selection_type;
};
static_assert(sizeof(c_load_pregame_selection_screen_message) == sizeof(c_load_screen_message) + 0x4);

struct c_message_globals
{
	struct s_message_queue_node
	{
		s_message_queue_node* m_next;
		s_message_queue_node* m_prev;
		c_message* m_message;
	};
	
protected:
	s_message_queue_node m_queue[32];
	s_message_queue_node* m_next_read;
	s_message_queue_node* m_prev_read;

	// HANDLE
	//void* m_system_message_notification_handle;
	bool m_xbox_guide_is_active;

public:
	c_message_globals();
	~c_message_globals();

	void initialize();
	void dispose();
	void initialize_for_new_map();
	void dispose_from_old_map();
	void empty_queue();
	c_message* dequeue_node(s_message_queue_node* node, bool unknown);
	bool can_read();
	void queue(c_message* message);
	bool can_write();
	void get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference);
	bool message_match(c_message* message, long screen_name, e_controller_index controller, e_window_index window);
	void dequeue(c_message* message);
	bool get_xbox_guide_is_active();
	void set_xbox_guide_is_active(bool xbox_guide_is_active);
	void* get_system_message_notification_handle();
};
static_assert(sizeof(c_message_globals) == 0x18C);

extern c_message_globals& g_message_globals;

bool __cdecl user_interface_message_queue_is_empty();
void __cdecl user_interface_messaging_dispose();
void __cdecl user_interface_messaging_dispose_from_old_map();
bool __cdecl user_interface_messaging_get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference);
void __cdecl user_interface_messaging_initialize();
void __cdecl user_interface_messaging_initialize_for_new_map();
void __cdecl user_interface_messaging_pop(c_message* message);
void __cdecl user_interface_messaging_post(c_message* message);
void __cdecl user_interface_messaging_update();
bool __cdecl user_interface_xbox_guide_is_active();

