#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_error_manager.hpp"

class c_gui_screen_widget;

template<typename t_class>
void ui_track_delete(const t_class* object);

class c_message
{
public:
	virtual ~c_message();
	virtual void initialize();
	virtual void update();

public:
	c_message(e_ui_message_type type, int32 screen_name, e_controller_index controller, e_window_index window);

	e_ui_message_type get_type() const;
	int32 get_screen_name() const;
	e_controller_index get_controller() const;
	e_window_index get_window() const;
	int32 get_game_time_at_creation() const;

protected:
	int32 m_game_time_at_creation;
	e_ui_message_type m_type;
	int32 m_screen_name;
	e_controller_index m_controller;
	e_window_index m_window;
};
static_assert(sizeof(c_message) == 0x18);

//_ui_message_type_controller_input
class c_controller_input_message :
	public c_message
{
public:
	virtual ~c_controller_input_message() = default;

public:
	c_controller_input_message(int32 screen_name, e_controller_index controller, e_window_index window, e_event_type event_type, e_controller_component component, int32 event_value);

	e_event_type get_event_type() const;
	e_controller_component get_component() const;
	int32 get_event_value() const;

protected:
	e_event_type m_event_type;
	e_controller_component m_component;
	int32 m_event_value;
	int32 m_from_automation;
};
static_assert(sizeof(c_controller_input_message) == sizeof(c_message) + 0x10);

//_ui_message_type_xenon
class c_xenon_message :
	public c_message
{
	enum e_xenon_message_type;

public:
	virtual ~c_xenon_message() = default;

public:
	c_xenon_message(e_controller_index controller, e_xenon_message_type xenon_message_type, int32 event_value);

	e_xenon_message_type get_xenon_message_type() const;
	int32 get_event_value() const;

protected:
	e_xenon_message_type m_xenon_message_type;
	int32 m_event_value;
};
static_assert(sizeof(c_xenon_message) == sizeof(c_message) + 0x8);

//_ui_message_type_load_screen
class c_load_screen_message :
	public c_message
{
public:
	virtual ~c_load_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const;

public:
	c_load_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position);

	void set_focus_on_load_by_name(int32 list_name, int32 column_name, int32 column_value);
	void set_transition_type(e_screen_transition_type transition_type);
	void set_focus_on_load(int32 list_name, int32 element_handle);
	void set_parent_screen_index(int32 parent_screen_index);

	e_screen_transition_type get_transition_type() const;
	bool get_respond_to_controller_events() const;
	int32 get_focus_on_load_list_name() const;
	int32 get_focus_on_load_element_handle() const;
	int32 get_focus_on_load_column_name() const;
	int32 get_focus_on_load_column_value() const;
	int32 get_parent_screen_index() const;
	int32 get_layered_position() const;
	bool get_applies_even_to_codeless_screens() const;

protected:
	e_screen_transition_type m_transition_type;
	bool m_respond_to_controller_events;
	int32 m_focus_on_load_name;
	int32 m_focus_on_load_element_handle;
	int32 m_focus_on_load_column_name;
	int32 m_focus_on_load_column_value;
	int32 m_parent_screen_index;
	int32 m_layered_position;
	bool m_applies_even_to_codeless_screens;
};
static_assert(sizeof(c_load_screen_message) == 0x3C);
static_assert(sizeof(c_load_screen_message) == sizeof(c_message) + 0x24);

//_ui_message_type_screen_custom
class c_screen_custom_message :
	public c_message
{
public:
	virtual ~c_screen_custom_message() = default;

public:
	c_screen_custom_message(int32 sub_type, int32 screen_name, e_controller_index controller, e_window_index window);

	int32 get_sub_type() const;

protected:
	int32 m_sub_type;
};
static_assert(sizeof(c_screen_custom_message) == sizeof(c_message) + 0x4);

//_ui_message_type_dialog_result
class c_dialog_result_message :
	public c_message
{
public:
	virtual ~c_dialog_result_message() = default;

public:
	c_dialog_result_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 dialog_name, e_gui_dialog_choice dialog_result);

	int32 get_dialog_name() const;
	e_gui_dialog_choice get_dialog_result() const;
	int32 get_dispose_on_success_screen_index() const;
	void set_dispose_on_success_screen_index(int32 dispose_on_success_screen_index);

protected:
	e_gui_dialog_choice m_dialog_result;
	int32 m_dialog_name;
	int32 m_dispose_on_success_screen_index;
};
static_assert(sizeof(c_dialog_result_message) == sizeof(c_message) + 0xC);

class c_load_terminal_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_terminal_screen_message() = default;

public:
	c_load_terminal_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, int32 initial_state);

protected:
	int32 m_initial_state; // apply_initial_state
};
static_assert(sizeof(c_load_terminal_screen_message) == sizeof(c_load_screen_message) + 0x4);

class c_load_alert_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_alert_screen_message() = default;

public:
	c_load_alert_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, c_gui_queued_error* error);

protected:
	c_gui_queued_error m_error; // apply_initial_state
};
static_assert(sizeof(c_load_alert_screen_message) == sizeof(c_load_screen_message) + 0x424);

class c_load_dialog_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_dialog_screen_message() = default;

public:
	c_load_dialog_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, int32 dialog_name, int32 dialog_invoker);

	int32 get_dialog_screen_name(int32 dialog_name);
	void set_dispose_on_success_screen_index(int32 dispose_on_success_screen_index);
	void set_test_mode(bool test_mode);

protected:
	int32 m_dialog_name;
	int32 m_dialog_invoker;
	bool m_test_mode;
	int32 m_dispose_on_success_screen_index;
};
static_assert(sizeof(c_load_dialog_screen_message) == sizeof(c_load_screen_message) + 0x10);

class c_load_game_browser_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_game_browser_screen_message() = default;

public:
	c_load_game_browser_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, int32 search_flags, e_browser_type type);

protected:
	uns32 m_squad_search_flags;
	c_enum<e_browser_type, int32, _browse_system_link, k_number_of_game_browser_types> m_type;
};
static_assert(sizeof(c_load_game_browser_screen_message) == sizeof(c_load_screen_message) + 0x8);

class c_load_pregame_selection_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_pregame_selection_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const override;

public:
	c_load_pregame_selection_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, e_gui_selected_item_type selection_type);

protected:
	e_gui_selected_item_type m_selection_type;
};
static_assert(sizeof(c_load_pregame_selection_screen_message) == sizeof(c_load_screen_message) + 0x4);

class c_load_campaign_select_difficulty_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_campaign_select_difficulty_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const override;

public:
	c_load_campaign_select_difficulty_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, e_gui_campaign_difficulty_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, e_campaign_difficulty_level difficulty);

protected:
	e_gui_campaign_difficulty_setup_mode m_campaign_setup_mode;
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	e_campaign_difficulty_level m_difficulty;
};
static_assert(sizeof(c_load_campaign_select_difficulty_screen_message) == sizeof(c_load_screen_message) + 0x10);

class c_load_campaign_select_level_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_campaign_select_level_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const override;

public:
	c_load_campaign_select_level_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, e_gui_campaign_level_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, int16 campaign_insertion_point);

protected:
	e_gui_campaign_level_setup_mode m_campaign_setup_mode;
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	int16 m_campaign_insertion_point;
};
static_assert(sizeof(c_load_campaign_select_level_screen_message) == sizeof(c_load_screen_message) + 0x10);

class c_message_globals
{
public:
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
	void get_next_message(int32 screen_name, e_controller_index controller, e_window_index window, c_message** message_reference);
	bool message_match(c_message* message, int32 screen_name, e_controller_index controller, e_window_index window);
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
bool __cdecl user_interface_messaging_get_next_message(int32 screen_name, e_controller_index controller, e_window_index window, c_message** message_reference);
void __cdecl user_interface_messaging_initialize();
void __cdecl user_interface_messaging_initialize_for_new_map();
void __cdecl user_interface_messaging_pop(c_message* message);
void __cdecl user_interface_messaging_post(c_message* message);
void __cdecl user_interface_messaging_update();
bool __cdecl user_interface_xbox_guide_is_active();

