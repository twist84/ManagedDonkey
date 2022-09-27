#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct c_message
{
public:
	virtual void* destructor(dword);
	virtual void initialize();
	virtual void update();

	long get_screen_name();
	e_controller_index get_controller();
	e_window_index get_window();

protected:
	dword m_game_time_at_creation;
	long m_type;
	long m_screen_name;
	e_controller_index m_controller;
	e_window_index m_window;
};
static_assert(sizeof(c_message) == 0x18);

class c_gui_screen_widget;
struct c_load_screen_message : c_message
{
	virtual void apply_initial_state(class c_gui_screen_widget*) const;

protected:
	long m_transition_type;
	bool m_respond_to_controller_events;
	long m_focus_on_load_list_name;
	long m_focus_on_load_element_handle;
	long m_focus_on_load_column_name;
	long m_focus_on_load_column_value;
	long m_parent_screen_index;
	long m_layered_position;
	bool m_applies_even_to_codeless_screens;
};
static_assert(sizeof(c_load_screen_message) == 0x3C);

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

extern void __cdecl user_interface_messaging_initialize();
extern void __cdecl user_interface_messaging_dispose();
extern void __cdecl user_interface_messaging_initialize_for_new_map();
extern void __cdecl user_interface_messaging_dispose_from_old_map();
extern void __cdecl user_interface_messaging_update();
extern void __cdecl user_interface_messaging_post(c_message* message);
extern bool __cdecl user_interface_messaging_get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference);
extern void __cdecl user_interface_messaging_pop(c_message* message);
extern bool __cdecl user_interface_message_queue_is_empty();

extern c_load_screen_message* load_screen_message_ctor(c_load_screen_message* message, long screen_name, e_controller_index controller, e_window_index window, long layered_position);