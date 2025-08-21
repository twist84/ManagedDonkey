#include "interface/user_interface_messages.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_time.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_difficulty.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_level.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_selection.hpp"
#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "networking/logic/logic_session_tracker.hpp"
#include "tag_files/string_ids.hpp"

REFERENCE_DECLARE(0x052600D0, c_message_globals, g_message_globals);

HOOK_DECLARE(0x00A93390, user_interface_message_queue_is_empty);
HOOK_DECLARE(0x00A933B0, user_interface_messaging_dispose);
HOOK_DECLARE(0x00A933C0, user_interface_messaging_dispose_from_old_map);
HOOK_DECLARE(0x00A933D0, user_interface_messaging_get_next_message);
HOOK_DECLARE(0x00A93410, user_interface_messaging_initialize);
HOOK_DECLARE(0x00A93420, user_interface_messaging_initialize_for_new_map);
HOOK_DECLARE(0x00A93430, user_interface_messaging_pop);
HOOK_DECLARE(0x00A93450, user_interface_messaging_post);
HOOK_DECLARE(0x00A934A0, user_interface_messaging_update);
HOOK_DECLARE(0x00A934B0, user_interface_xbox_guide_is_active);

void* c_message::operator new(unsigned int size)
{
	return user_interface_malloc_tracked(size, __FILE__, __LINE__);
}

template<>
void ui_track_delete<c_message>(const c_message* object)
{
	ASSERT(object != NULL);

	object->~c_message();
	user_interface_free(object);
}

c_message::c_message(e_ui_message_type type, int32 screen_name, e_controller_index controller, e_window_index window) :
	m_game_time_at_creation(game_time_initialized() ? game_time_get() : 0),
	m_type(type),
	m_screen_name(screen_name),
	m_controller(controller),
	m_window(window)
{
}

c_message::~c_message()
{
}

void c_message::initialize()
{
}

void c_message::update()
{
}

//c_controller.obj
e_ui_message_type c_message::get_type() const
{
	return m_type;
}

//user_interface_networking.obj
int32 c_message::get_screen_name() const
{
	return m_screen_name;
}

//user_interface_networking.obj
e_controller_index c_message::get_controller() const
{
	return m_controller;
}

//user_interface_messages.obj
e_window_index c_message::get_window() const
{
	return m_window;
}

//user_interface_window_manager.obj
int32 c_message::get_game_time_at_creation() const
{
	return m_game_time_at_creation;
}

c_controller_input_message::c_controller_input_message(int32 screen_name, e_controller_index controller, e_window_index window, e_event_type event_type, e_controller_component component, int32 event_value) :
	c_message(_message_type_controller_input, screen_name, controller, window),
	m_event_type(event_type),
	m_component(component),
	m_event_value(event_value),
	m_from_automation(false)
{
}

//gui_screen_start_menu.obj
e_event_type c_controller_input_message::get_event_type() const
{
	return m_event_type;
}

//gui_screen_start_menu.obj
e_controller_component c_controller_input_message::get_component() const
{
	return m_component;
}

//gui_screen_scoreboard.obj
int32 c_controller_input_message::get_event_value() const
{
	return m_event_value;
}

c_xenon_message::c_xenon_message(e_controller_index controller, e_xenon_message_type xenon_message_type, int32 event_value) :
	c_message(_message_type_xenon, _string_id_invalid, controller, k_no_window),
	m_xenon_message_type(xenon_message_type),
	m_event_value(event_value)
{
}

//c_controller.obj
c_xenon_message::e_xenon_message_type c_xenon_message::get_xenon_message_type() const
{
	return m_xenon_message_type;
}

//c_controller.obj
int32 c_xenon_message::get_event_value() const
{
	return m_event_value;
}

c_load_screen_message::c_load_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position) :
	c_message(_message_type_load_screen, screen_name, controller, window),
	m_transition_type((e_screen_transition_type)1),
	m_respond_to_controller_events(true),
	m_focus_on_load_name(NONE),
	m_focus_on_load_element_handle(NONE),
	m_focus_on_load_column_name(NONE),
	m_focus_on_load_column_value(NONE),
	m_parent_screen_index(NONE),
	m_layered_position(layered_position),
	m_applies_even_to_codeless_screens(false)
{
	ASSERT(VALID_INDEX(controller, k_number_of_controllers) || controller == k_any_controller);
	ASSERT(VALID_INDEX(window, k_number_of_render_windows));
	ASSERT(m_layered_position != _string_id_invalid);
	if (screen_name == _string_id_invalid)
	{
		event(_event_error, "ui:dialog: dialog %s (%d) does not exist", string_id_get_string_const(_string_id_invalid), _string_id_invalid);
	}
}

void c_load_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
}

//saved_film_director.obj
void c_load_screen_message::set_focus_on_load_by_name(int32 list_name, int32 column_name, int32 column_value)
{
	m_focus_on_load_name = list_name;
	m_focus_on_load_column_name = column_name;
	m_focus_on_load_column_value = column_value;
}

//gui_screen_start_menu.obj
void c_load_screen_message::set_transition_type(e_screen_transition_type transition_type)
{
	m_transition_type = transition_type;
}

//gui_screen_start_menu.obj
void c_load_screen_message::set_focus_on_load(int32 list_name, int32 element_handle)
{
	m_focus_on_load_name = list_name;
	m_focus_on_load_element_handle = element_handle;
}

//gui_screen_start_menu.obj
void c_load_screen_message::set_parent_screen_index(int32 parent_screen_index)
{
	m_parent_screen_index = parent_screen_index;
}

//user_interface_window_manager.obj
e_screen_transition_type c_load_screen_message::get_transition_type() const
{
	return m_transition_type;
}

//user_interface_window_manager.obj
bool c_load_screen_message::get_respond_to_controller_events() const
{
	return m_respond_to_controller_events;
}

//user_interface_window_manager.obj
int32 c_load_screen_message::get_focus_on_load_list_name() const
{
	return m_focus_on_load_name;
}

//user_interface_window_manager.obj
int32 c_load_screen_message::get_focus_on_load_element_handle() const
{
	return m_focus_on_load_element_handle;
}

//user_interface_window_manager.obj
int32 c_load_screen_message::get_focus_on_load_column_name() const
{
	return m_focus_on_load_column_name;
}

//user_interface_window_manager.obj
int32 c_load_screen_message::get_focus_on_load_column_value() const
{
	return m_focus_on_load_column_value;
}

//user_interface_window_manager.obj
int32 c_load_screen_message::get_parent_screen_index() const
{
	return m_parent_screen_index;
}

//user_interface_window_manager.obj
int32 c_load_screen_message::get_layered_position() const
{
	return m_layered_position;
}

//user_interface_window_manager.obj
bool c_load_screen_message::get_applies_even_to_codeless_screens() const
{
	return m_applies_even_to_codeless_screens;
}

c_screen_custom_message::c_screen_custom_message(int32 sub_type, int32 screen_name, e_controller_index controller, e_window_index window) :
	c_message(_message_type_screen_custom, screen_name, controller, window),
	m_sub_type(sub_type)
{
}

//gui_screen_start_menu.obj
int32 c_screen_custom_message::get_sub_type() const
{
	return m_sub_type;
}

c_dialog_result_message::c_dialog_result_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 dialog_name, e_gui_dialog_choice dialog_result) :
	c_message(_message_type_dialog_result, screen_name, controller, window),
	m_dialog_result(dialog_result),
	m_dialog_name(dialog_name),
	m_dispose_on_success_screen_index()
{
}

//user_interface_networking.obj
int32 c_dialog_result_message::get_dialog_name() const
{
	return m_dialog_name;
}

//user_interface_networking.obj
e_gui_dialog_choice c_dialog_result_message::get_dialog_result() const
{
	return m_dialog_result;
}

//gui_pregame_setup_manager.obj
int32 c_dialog_result_message::get_dispose_on_success_screen_index() const
{
	return m_dispose_on_success_screen_index;
}

//gui_screen_dialog.obj
void c_dialog_result_message::set_dispose_on_success_screen_index(int32 dispose_on_success_screen_index)
{
	m_dispose_on_success_screen_index = dispose_on_success_screen_index;
}


c_load_dialog_screen_message::c_load_dialog_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, int32 dialog_name, int32 dialog_invoker) :
	c_load_screen_message(get_dialog_screen_name(dialog_name), controller, window, layered_position),
	m_dialog_name(dialog_name),
	m_dialog_invoker(dialog_invoker),
	m_test_mode(false),
	m_dispose_on_success_screen_index(NONE)
{
}

int32 c_load_dialog_screen_message::get_dialog_screen_name(int32 dialog_name)
{
	// $TODO: iterate through `user_interface_shared_globals->dialog_descriptions`,
	// check `dialog_name` against `dialog_description->dialog_name`
	// and return `(dialog_description->third_item != -1) + STRING_ID(gui, gui_dialog_two_items)`
	// if `dialog_description` doesn't contain `dialog_name` return `_string_id_invalid`
	
	return _string_id_invalid;
}

//gui_pregame_setup_manager.obj
void c_load_dialog_screen_message::set_dispose_on_success_screen_index(int32 dispose_on_success_screen_index)
{
	m_dispose_on_success_screen_index = dispose_on_success_screen_index;
}

//gui_screen_dialog.obj
void c_load_dialog_screen_message::set_test_mode(bool test_mode)
{
	m_test_mode = test_mode;
}

c_load_game_browser_screen_message::c_load_game_browser_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, int32 search_flags, e_browser_type type) :
	c_load_screen_message(screen_name, controller, window, layered_position),
	m_squad_search_flags(search_flags),
	m_type(type)
{
}

c_load_pregame_selection_screen_message::c_load_pregame_selection_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, e_gui_selected_item_type selection_type) :
	c_load_screen_message(STRING_ID(gui, pregame_selection), controller, window, layered_position),
	m_selection_type(selection_type)
{
}

void c_load_pregame_selection_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	if (!screen_widget->running_in_codeless_mode())
		((c_gui_screen_pregame_selection*)screen_widget)->m_selection_type = m_selection_type;
}

c_load_campaign_select_difficulty_screen_message::c_load_campaign_select_difficulty_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, e_gui_campaign_difficulty_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, e_campaign_difficulty_level difficulty) :
	c_load_screen_message(screen_name, controller, window, layered_position),
	m_campaign_setup_mode(campaign_setup_mode),
	m_campaign_id(campaign_id),
	m_map_id(map_id),
	m_difficulty(difficulty)
{
}

void c_load_campaign_select_difficulty_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	((c_gui_screen_campaign_select_difficulty*)screen_widget)->setup(m_campaign_setup_mode, m_campaign_id, m_map_id, m_difficulty);
}

c_load_campaign_select_level_screen_message::c_load_campaign_select_level_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, e_gui_campaign_level_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, int16 campaign_insertion_point) :
	c_load_screen_message(screen_name, controller, window, layered_position),
	m_campaign_setup_mode(campaign_setup_mode),
	m_campaign_id(campaign_id),
	m_map_id(map_id),
	m_campaign_insertion_point(campaign_insertion_point)
{
}

void c_load_campaign_select_level_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	((c_gui_screen_campaign_select_level*)screen_widget)->setup(m_campaign_setup_mode, m_campaign_id, m_map_id, m_campaign_insertion_point);
}

c_message_globals::c_message_globals()
{
	csmemset(m_queue, 0, sizeof(m_queue));
	m_next_read = NULL;
	m_prev_read = NULL;
	m_xbox_guide_is_active = false;
	//m_system_message_notification_handle = NULL;
}

c_message_globals::~c_message_globals()
{
}

void c_message_globals::initialize()
{
	//m_system_message_notification_handle = XNotifyCreateListener(0x6F);
	//XCustomRegisterDynamicActions();
}

void c_message_globals::dispose()
{
	//XCustomUnregisterDynamicActions();
	//if (m_system_message_notification_handle && m_system_message_notification_handle != INVALID_HANDLE_VALUE)
	//{
	//	CloseHandle(m_system_message_notification_handle);
	//	m_system_message_notification_handle = NULL;
	//}
}

void c_message_globals::initialize_for_new_map()
{
	s_message_queue_node* next = NULL;
	s_message_queue_node* prev = NULL;

	for (int32 node_index = 0; node_index < NUMBEROF(m_queue); node_index++)
	{
		if (node_index == NUMBEROF(m_queue) - 1)
		{
			next = m_queue;
		}
		else
		{
			next = &m_queue[node_index + 1];
		}

		if (node_index)
		{
			prev = &m_queue[node_index - 1];
		}
		else
		{
			prev = &m_queue[NUMBEROF(m_queue) - 1];
		}

		m_queue[node_index].m_next = next;
		m_queue[node_index].m_prev = prev;
		m_queue[node_index].m_message = NULL;
	}

	m_prev_read = m_queue;
	m_next_read = m_queue;
}

void c_message_globals::dispose_from_old_map()
{
	empty_queue();
}

void c_message_globals::empty_queue()
{
	while (can_read())
	{
		ui_track_delete<c_message>(dequeue_node(m_next_read, true));
	}
}

c_message* c_message_globals::dequeue_node(s_message_queue_node* node, bool unknown)
{
	//return INVOKE_CLASS_MEMBER(0x00A92EC0, c_message_globals, dequeue_node, node, unknown);

	ASSERT(&m_queue[0] <= node && node <= &m_queue[NUMBEROF(m_queue) - 1]);
	ASSERT(node->m_message != NULL);

	c_message* message = node->m_message;

	if (unknown)
	{
		if (node == m_next_read)
		{
			m_next_read = m_next_read->m_next;
		}

		s_message_queue_node* prev = node->m_prev;
		node->m_next->m_prev = prev;
		prev->m_next = node->m_next;
		node->m_message = 0;

		s_message_queue_node* next = m_prev_read->m_next;
		next->m_prev = node;
		m_prev_read->m_next = node;
		node->m_prev = m_prev_read;
		node->m_next = next;
	}

	return message;
}

bool c_message_globals::can_read()
{
	//return INVOKE_CLASS_MEMBER(0x00A92E10, c_message_globals, can_read);

	return m_next_read->m_message != NULL;
}

void c_message_globals::queue(c_message* message)
{
	ASSERT(message != NULL);
	ASSERT(can_write());
	//ASSERT(user_interface_is_tracked_memory(message));

	for (s_message_queue_node* node = m_next_read; node && node != m_prev_read; node = node->m_next)
	{
		if (!node->m_message || node->m_message != message)
		{
			continue;
		}

		VASSERT("ui: attempting to queue a message that has already been queued");
		break;
	}

	m_prev_read->m_message = message;
	m_prev_read = m_prev_read->m_next;
}

bool c_message_globals::can_write()
{
	return !m_prev_read->m_message && m_prev_read->m_next != m_next_read;
}

void c_message_globals::get_next_message(int32 screen_name, e_controller_index controller, e_window_index window, c_message** message_reference)
{
	ASSERT(message_reference != NULL);

	c_message* next_message = NULL;
	s_message_queue_node* next_node = NULL;

	if (*message_reference)
	{
		for (s_message_queue_node* node = m_next_read; node && node != m_prev_read; node = node->m_next)
		{
			if (!node->m_message || node->m_message != *message_reference)
			{
				continue;
			}

			next_node = node->m_next;
			break;
		}
	}
	else
	{
		ASSERT(m_next_read != NULL);
		next_node = m_next_read;
	}

	while (next_node && next_node != m_prev_read)
	{
		if (!next_node->m_message || !message_match(next_node->m_message, screen_name, controller, window))
		{
			next_node = next_node->m_next;
			continue;
		}

		next_message = next_node->m_message;
		break;
	}

	*message_reference = next_message;
}

bool c_message_globals::message_match(c_message* message, int32 screen_name, e_controller_index controller, e_window_index window)
{
	bool screen_matched = screen_name == _string_id_invalid || message->get_screen_name() == screen_name;
	bool controller_matched = controller == k_any_controller || message->get_controller() == controller;
	bool window_matched = window == k_any_window || window == k_no_window || message->get_window() == window;

	return screen_matched && controller_matched && window_matched;
}

void c_message_globals::dequeue(c_message* message)
{
	for (s_message_queue_node* node = m_next_read; node && node != m_prev_read; node = node->m_next)
	{
		if (!node->m_message || node->m_message != message)
		{
			continue;
		}

		dequeue_node(node, true);
		break;
	}
}

bool c_message_globals::get_xbox_guide_is_active()
{
	return m_xbox_guide_is_active;
}

void c_message_globals::set_xbox_guide_is_active(bool xbox_guide_is_active)
{
	m_xbox_guide_is_active = xbox_guide_is_active;
}

void* c_message_globals::get_system_message_notification_handle()
{
	//return m_system_message_notification_handle;
	return NULL;
}

bool __cdecl user_interface_message_queue_is_empty()
{
	//return INVOKE(0x00A93390, user_interface_message_queue_is_empty);

	return game_in_progress() && !g_message_globals.can_read();
}

void __cdecl user_interface_messaging_dispose()
{
	//INVOKE(0x00A933B0, user_interface_messaging_dispose);

	g_message_globals.dispose();
}

void __cdecl user_interface_messaging_dispose_from_old_map()
{
	//INVOKE(0x00A933C0, user_interface_messaging_dispose_from_old_map);

	g_message_globals.dispose_from_old_map();
}

bool __cdecl user_interface_messaging_get_next_message(int32 screen_name, e_controller_index controller, e_window_index window, c_message** message_reference)
{
	//return INVOKE(0x00A933D0, user_interface_messaging_get_next_message, screen_name, controller, window, message_reference);

	if (!game_in_progress())
	{
		return false;
	}

	g_message_globals.get_next_message(screen_name, controller, window, message_reference);
	return *message_reference != NULL;
}

void __cdecl user_interface_messaging_initialize()
{
	//INVOKE(0x00A93410, user_interface_messaging_initialize);

	g_message_globals.initialize();
}

void __cdecl user_interface_messaging_initialize_for_new_map()
{
	//INVOKE(0x00A93420, user_interface_messaging_initialize_for_new_map);

	g_message_globals.initialize_for_new_map();
}

void __cdecl user_interface_messaging_pop(c_message* message)
{
	//INVOKE(0x00A93430, user_interface_messaging_pop, message);

	if (!game_in_progress())
	{
		return;
	}

	g_message_globals.dequeue(message);
}

void __cdecl user_interface_messaging_post(c_message* message)
{
	//INVOKE(0x00A93450, user_interface_messaging_post, message);

	ASSERT(message != NULL);

	bool message_queued = false;
	if (!game_in_progress())
	{
		event(_event_message, "ui: failed to post message because game is not in progress!");
		ui_track_delete<c_message>(message);
		return;
	}

	if (!g_message_globals.can_write())
	{
		event(_event_message, "ui: failed to post message because message queue is not writeable!");
		ui_track_delete<c_message>(message);
		return;
	}

	g_message_globals.queue(message);
	message_queued = true;
}

void __cdecl user_interface_messaging_update()
{
	//INVOKE(0x00A934A0, user_interface_messaging_update);

	//update_xenon_messages()
}

bool __cdecl user_interface_xbox_guide_is_active()
{
	//return INVOKE(0x00A934B0, user_interface_xbox_guide_is_active);

	return g_message_globals.get_xbox_guide_is_active();
}
