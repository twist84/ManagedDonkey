#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"

#include "cseries/cseries_events.hpp"
#include "cutscene/cinematics.hpp"
#include "game/game.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_screens/start_menu/panes/settings/start_menu_settings.hpp"
#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "networking/online/online_service_record.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "simulation/simulation.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE03C0, c_start_menu_screen_widget, dispose_);
HOOK_DECLARE_CLASS_MEMBER(0x00AE05B0, c_start_menu_screen_widget, handle_controller_input_message_);
HOOK_DECLARE_CLASS(0x00AE0720, c_start_menu_screen_widget, handle_global_start_button_press_);
HOOK_DECLARE_CLASS_MEMBER(0x00AE07A0, c_start_menu_screen_widget, initialize_);

byte const handle_global_start_button_press_patch_bytes[] = { 0xEB, 0x10 };
DATA_PATCH_DECLARE(0x00AAC890, handle_global_start_button_press, handle_global_start_button_press_patch_bytes);

void __thiscall c_start_menu_screen_widget::back_out_current_pane_()
{
	c_start_menu_screen_widget::back_out_current_pane();
}

void __thiscall c_start_menu_screen_widget::dispose_()
{
	c_start_menu_screen_widget::dispose();
}

void __thiscall c_start_menu_screen_widget::initialize_()
{
	c_start_menu_screen_widget::initialize();
}

bool __thiscall c_start_menu_screen_widget::handle_controller_input_message_(c_controller_input_message* message)
{
	return c_start_menu_screen_widget::handle_controller_input_message(message);
}

bool __cdecl c_start_menu_screen_widget::handle_global_start_button_press_(c_controller_input_message* message)
{
	return c_start_menu_screen_widget::handle_global_start_button_press(message);
}

void c_start_menu_screen_widget::back_out_current_pane()
{
	//INVOKE_CLASS_MEMBER(0x00AE01D0, c_start_menu_screen_widget, back_out_current_pane);

	if (m_breadcrumbs.count() > 1)
	{
		c_start_menu_settings* current_pane = (c_start_menu_settings*)c_start_menu_screen_widget::get_current_pane();
		if (current_pane)
		{
			current_pane->transition_out_with_transition_type(_transition_out_normal, _screen_transition_type_custom1);
		}

		m_breadcrumbs.pop();
		m_breadcrumb_to_load_valid = true;
		m_breadcrumb_to_load = *m_breadcrumbs.get_top();

		real32 appearance_camera_zoom = 0.0f;
		if (current_pane && gui_screen_widget_is_start_menu_pane_screen(current_pane))
		{
			appearance_camera_zoom = current_pane->m_appearance_camera_zoom;
		}

		real32 appearance_camera_yaw = 0.0f;
		if (current_pane && gui_screen_widget_is_start_menu_pane_screen(current_pane))
		{
			appearance_camera_yaw = current_pane->m_appearance_camera_yaw;
		}

		c_start_menu_screen_widget::load_pane_direct(
			m_breadcrumb_to_load.screen_id,
			_screen_transition_type_custom1,
			STRING_ID(gui, sidebar),
			m_breadcrumb_to_load.element_handle,
			appearance_camera_zoom,
			appearance_camera_yaw);
	}
	else
	{
		c_gui_screen_widget::transition_out(_transition_out_back_out);
	}
}

//.text:00AE02C0 ; private: void c_start_menu_screen_widget::clean_up_old_panes()
//.text:00AE0340 ; 
//.text:00AE0350 ; public: static void __cdecl c_start_menu_screen_widget::close_start_menu()
//.text:00AE0390 ; 

bool c_start_menu_screen_widget::current_pane_is_root_pane() const
{
	//return INVOKE_CLASS_MEMBER(0x00AE03A0, c_start_menu_screen_widget, current_pane_is_root_pane);

	return m_is_rooted && m_breadcrumbs.count() == 1;
}

void c_start_menu_screen_widget::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00AE03C0, c_start_menu_screen_widget, dispose);

	c_gui_screen_widget::dispose();

	if (game_is_playback())
	{
		saved_film_manager_set_playback_game_speed(m_old_film_playback_speed);
	}
	if (game_time_get_paused_for_reason(_game_time_pause_ui))
	{
		game_time_set_paused(false, _game_time_pause_ui);
	}
}

//.text:00AE03F0 ; 
//.text:00AE0400 ; 
//.text:00AE0420 ; 
//.text:00AE0430 ; 
//.text:00AE0440 ; public: virtual void c_start_menu_screen_widget_sidebar_items_datasource::get_column_names(int32* const column_names, int32* column_count)

int32 __cdecl c_start_menu_screen_widget::get_current_game_pane_name()
{
	//return INVOKE(0x00AE0470, c_start_menu_screen_widget::get_current_game_pane_name);

	string_id result = _string_id_invalid;
	if (game_in_progress())
	{
		if (game_is_playback())
		{
			result = STRING_ID(gui, start_menu_game_saved_film);
		}
		else if (game_is_campaign())
		{
			result = STRING_ID(gui, start_menu_game_campaign);
		}
		else if (game_engine_is_sandbox())
		{
			result = STRING_ID(gui, start_menu_game_editor);
		}
		else if (game_is_multiplayer())
		{
			result = STRING_ID(gui, start_menu_game_multiplayer);
		}
	}
	return result;
}

//.text:00AE04C0 ; protected: virtual int32 c_start_menu_screen_widget_sidebar_items_datasource::get_current_item_count_internal()

c_gui_screen_widget* c_start_menu_screen_widget::get_current_pane()
{
	//return INVOKE_CLASS_MEMBER(0x00AE04D0, c_start_menu_screen_widget, get_current_pane);

	c_gui_screen_widget* current_pane = NULL;
	if (m_requested_pane != NONE)
	{
		current_pane = (c_gui_screen_widget*)c_gui_widget::get_child_widget(_gui_screen, m_requested_pane);
	}
	return current_pane;
}

e_render_data_size c_start_menu_screen_widget::get_render_data_size()
{
	//return INVOKE_CLASS_MEMBER(0x00AE04F0, c_start_menu_screen_widget, get_render_data_size);

	return k_render_data_size_small;
}

//.text:00AE0500 ; public: virtual bool c_start_menu_screen_widget_sidebar_items_datasource::get_string_id_value(int32, int32, int32*)
//.text:00AE0580 ; 
//.text:00AE0590 ; 
//.text:00AE05A0 ; 

bool c_start_menu_screen_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	//return HOOK_INVOKE_CLASS_MEMBER(0x00AE05B0, c_start_menu_screen_widget, handle_controller_input_message, message);

	// idk where this should be
	//c_gui_list_item_widget* list_item_widget;

	bool handled = false;

	c_gui_widget* focused_widget = c_gui_screen_widget::get_focused_widget();

	e_event_type event_type = message->get_event_type();
	e_controller_component component = message->get_component();
	switch (event_type)
	{
	case _event_type_button_press:
	{
		switch (component)
		{
		case _controller_component_button_b:
		{
			if (m_breadcrumbs.count() != 1)
			{
				c_gui_screen_widget::transition_out(_transition_out_back_out);
			}
			else
			{
				c_start_menu_screen_widget::back_out_current_pane();
			}

			handled = true;
		}
		break;
		case _controller_component_button_start:
		{
			if (!c_start_menu_screen_widget::transitioning_out())
			{
				c_gui_screen_widget::transition_out(_transition_out_back_out);
			}

			handled = true;
		}
		break;
		}
	}
	break;
	case _event_type_tab_left:
	case _event_type_tab_right:
	{
		if (c_start_menu_screen_widget::current_pane_is_root_pane())
		{
			c_gui_list_widget* list_widget = focused_widget->get_parent_list();
			int32 element_handle = list_widget->get_focused_element_handle();
			ASSERT(element_handle != NONE);

			c_gui_data* data = list_widget->get_data();
			int32 target_element_handle = NONE;
			e_screen_transition_type transition_type = _screen_transition_type_none;
			if (event_type == _event_type_tab_right)
			{
				target_element_handle = data->get_next_element_handle(element_handle);
				transition_type = _screen_transition_type_cycle_next;
			}
			else
			{
				target_element_handle = data->get_previous_element_handle(element_handle);
				transition_type = _screen_transition_type_cycle_previous;
			}

			string_id pane_to_show = NONE;
			if (data->get_string_id_value(target_element_handle, STRING_ID(gui, target), &pane_to_show)
				&& pane_to_show != _string_id_invalid && pane_to_show != k_string_id_empty_string)
			{
				c_start_menu_settings* current_pane = (c_start_menu_settings*)c_start_menu_screen_widget::get_current_pane();

				real32 appearance_camera_zoom = 0.0f;
				if (current_pane && gui_screen_widget_is_start_menu_pane_screen(current_pane))
				{
					appearance_camera_zoom = current_pane->m_appearance_camera_zoom;
				}

				real32 appearance_camera_yaw = 0.0f;
				if (current_pane && gui_screen_widget_is_start_menu_pane_screen(current_pane))
				{
					appearance_camera_yaw = current_pane->m_appearance_camera_yaw;
				}

				c_start_menu_screen_widget::load_pane(
					pane_to_show,
					true,
					transition_type,
					NONE,
					NONE,
					appearance_camera_zoom,
					appearance_camera_yaw);
			}
		}
	}
	break;
	}

	if (!handled)
	{
		handled = c_gui_screen_widget::handle_controller_input_message(message);
	}
	return handled;
}

bool __cdecl c_start_menu_screen_widget::handle_global_start_button_press(const c_controller_input_message* message)
{
	//return INVOKE(0x00AE0720, c_start_menu_screen_widget::handle_global_start_button_press, message);

	bool handled = false;
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_start)
	{
		e_controller_index controller_index = message->get_controller();
		c_controller_interface const* controller = controller_get(controller_index);

		if (simulation_starting_up())
		{
			event(_event_status, "ui:start_menu: Can't show start menu -- simulation starting up.");
		}
		else if (controller->in_use() && controller->get_user_index() != INT16_MAX)
		{
			unsigned long long target_player_xuid = controller->get_player_xuid();

			s_player_identifier target_player_id;
			controller->get_player_identifier(&target_player_id);
			handled = c_start_menu_screen_widget::load_start_menu(controller_index, &target_player_id, &target_player_xuid, nullptr, nullptr, 0);
		}
		else
		{
			event(_event_status, "ui:start_menu: Can't show start menu -- simulation starting up.");
		}
	}
	return handled;
}

void c_start_menu_screen_widget::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AE07A0, c_start_menu_screen_widget, initialize);

	c_gui_screen_widget::initialize();

	const c_controller_interface* controller = controller_get(c_gui_widget::get_single_responding_controller());

	controller->get_player_identifier(&m_owner_player_id);
	if (m_owner_player_id.is_empty())
	{
		transition_out(_transition_out_normal);
	}
	controller->get_player_identifier(&m_target_player_id);
	if (m_target_player_id.is_empty())
	{
		transition_out(_transition_out_normal);
	}
	if (m_target_player_xuid == 0ULL)
	{
		m_target_player_xuid = controller->get_player_xuid();
	}

	if (game_is_playback())
	{
		m_old_film_playback_speed = saved_film_manager_get_playback_game_speed();
		saved_film_manager_set_playback_game_speed(0.0f);
	}
	if (game_is_campaign())
	{
		m_wants_to_pause_game_time = !game_is_cooperative() || user_interface_squad_get_machine_count() <= 1;
	}
}

void c_start_menu_screen_widget::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00AE0870, c_start_menu_screen_widget, initialize_datasource);

	//c_gui_screen_widget::initialize_datasource();
	//
	//c_gui_data* datasource = new(_ui_allocation_marker_dummy) c_start_menu_screen_widget_sidebar_items_datasource();
	//if (datasource != nullptr)
	//{
	//	if (datasource->initialize(STRING_ID(gui, top_menu)))
	//	{
	//		add_datasource(datasource);
	//	}
	//	else
	//	{
	//		ui_track_delete<c_gui_data>(datasource);
	//	}
	//}
}

bool c_start_menu_screen_widget::__funcs53()
{
	return INVOKE_CLASS_MEMBER(0x00AE0920, c_start_menu_screen_widget, __funcs53);
}

void c_start_menu_screen_widget::load_pane(int32 pane_to_load, bool is_top_level_pane, e_screen_transition_type transition_type, int32 focus_on_load_list_name, int32 focus_on_load_element_handle, real32 appearance_camera_zoom, real32 appearance_camera_yaw)
{
	//INVOKE_CLASS_MEMBER(0x00AE09A0, c_start_menu_screen_widget, load_pane, pane_to_load, is_top_level_pane, transition_type, focus_on_load_list_name, focus_on_load_element_handle, appearance_camera_zoom, appearance_camera_yaw);

	c_gui_screen_widget* current_pane = get_current_pane();

	if (m_breadcrumbs.count() < s_start_menu_breadcrumb::k_maximum_breadcrumbs && pane_to_load != m_requested_pane)
	{
		s_start_menu_breadcrumb breadcrumb;
		breadcrumb.screen_id = pane_to_load;
		breadcrumb.element_handle = NONE;

		if (is_top_level_pane)
		{
			m_breadcrumbs.clear();
		}
		else if (m_breadcrumbs.count() > 0)
		{
			s_start_menu_breadcrumb* previous_breadcrumb = m_breadcrumbs.get_top();

			long element_handle = NONE;
			if (current_pane)
			{
				c_gui_widget* focused_widget = get_focused_widget();
				if (focused_widget)
				{
					c_gui_list_item_widget* focused_list_item = static_cast<c_gui_list_item_widget*>(focused_widget);
					if (focused_widget->get_type() != _gui_list_item)
					{
						focused_list_item = focused_widget->get_parent_list_item();
					}

					if (focused_list_item)
					{
						element_handle = focused_list_item->get_element_handle();
					}
				}
			}

			previous_breadcrumb->element_handle = element_handle;
		}

		m_breadcrumbs.push_back(breadcrumb);

		c_start_menu_screen_widget::load_pane_direct(pane_to_load, transition_type, focus_on_load_list_name, focus_on_load_element_handle, appearance_camera_zoom, appearance_camera_yaw);
	}
}

void c_start_menu_screen_widget::load_pane_direct(int32 pane_to_load, e_screen_transition_type transition_type, int32 focus_on_load_list_name, int32 focus_on_load_element_handle, real32 camera_zoom, real32 camera_yaw)
{
	INVOKE_CLASS_MEMBER(0x00AE0A80, c_start_menu_screen_widget, load_pane_direct, pane_to_load, transition_type, focus_on_load_list_name, focus_on_load_element_handle, camera_zoom, camera_yaw);
}

bool __cdecl c_start_menu_screen_widget::load_start_menu(e_controller_index controller_index, const s_player_identifier* player_identifier, const uns64* player_xuid, const s_service_record_identity* identity, const s_start_menu_breadcrumb* breadcrumbs, int32 breadcrumb_count)
{
	return INVOKE(0x00AE0BE0, load_start_menu, controller_index, player_identifier, player_xuid, identity, breadcrumbs, breadcrumb_count);
}

//.text:00AE0C10 ; public: static bool __cdecl c_start_menu_screen_widget::load_start_menu_for_file_share(e_controller_index, const s_player_identifier*, const uns64*, const s_service_record_identity*, int32)
//.text:00AE0D10 ; public: static bool __cdecl c_start_menu_screen_widget::load_start_menu_for_service_record(e_controller_index, const s_player_identifier*, const uns64*, const s_service_record_identity*)
//.text:00AE0DF0 ; private: static bool __cdecl c_start_menu_screen_widget::load_start_menu_internal(e_controller_index, const s_player_identifier*, const uns64*, const s_service_record_identity*, const s_start_menu_breadcrumb*, int32, bool)
//.text:00AE0EF0 ; 

void c_start_menu_screen_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AE0F00, c_start_menu_screen_widget, post_initialize);

	const c_controller_interface* controller = controller_get(c_gui_widget::get_single_responding_controller());

	c_gui_text_widget* player_name_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(global, player_name));
	c_gui_bitmap_widget* ring_of_light_bitmap = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, ring_of_light));

	c_gui_screen_widget::post_initialize();
	c_start_menu_screen_widget::update_pane();

	if (player_name_text_widget)
	{
		player_name_text_widget->set_text(controller->m_display_name);
	}
	if (ring_of_light_bitmap)
	{
		ring_of_light_bitmap->set_sprite_frame(controller->get_controller_index());
	}
}

bool c_start_menu_screen_widget::process_message(const c_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00AE0F80, c_start_menu_screen_widget, process_message, message);
}

//.text:00AE10A0 ; 
//.text:00AE10B0 ; 

void c_start_menu_screen_widget::reconstruct()
{
	//INVOKE_CLASS_MEMBER(0x00AE10D0, c_start_menu_screen_widget, reconstruct);

	m_requested_pane = NONE;
	m_breadcrumb_to_load.reset();
	m_breadcrumb_to_load_valid = false;
	m_breadcrumbs.clear();

	c_gui_screen_widget::reconstruct();
}

//.text:00AE1110 ; 
//.text:00AE1130 ; 
//.text:00AE1140 ; private: static void __cdecl c_start_menu_screen_widget::reset_start_menu_global_load_data()
//.text:00AE1150 ; 
//.text:00AE1170 ; public: static void __cdecl c_gui_start_menu_hq_service_record_file_share::set_next_load_target_slot_index(int32)
//.text:00AE1180 ; 
//.text:00AE11A0 ; 
//.text:00AE11C0 ; 
//.text:00AE11E0 ; 
//.text:00AE11F0 ; 
//.text:00AE1270 ; public: bool c_start_menu_screen_widget::should_handle_controller_input_message(const c_controller_input_message*)
//.text:00AE12F0 ; 

void c_start_menu_screen_widget::transition_out_with_transition_type(e_transition_out_type transition_out_type, e_screen_transition_type transition_type)
{
	//INVOKE_CLASS_MEMBER(0x00AE1300, c_start_menu_screen_widget, transition_out_with_transition_type, transition_out_type, transition_type);

	for (c_gui_screen_widget* screen = (c_gui_screen_widget*)this->get_first_child_widget_by_type(_gui_screen);
		screen != NULL;
		screen = (c_gui_screen_widget*)screen->get_next_widget_of_type(_gui_screen))
	{
		if (!screen->transitioning_out())
		{
			screen->transition_out_with_transition_type(transition_out_type, transition_type);
		}
	}
	c_gui_screen_widget::transition_out_with_transition_type(transition_out_type, transition_type);
}

void c_start_menu_screen_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AE1360, c_start_menu_screen_widget, update, current_milliseconds);

	//{
	//	c_load_screen_message* load_screen_message;
	//}
	//{
	//	const c_controller_interface* controller;
	//	s_player_identifier owner_player_id;
	//	event(...);
	//	event(...);
	//}
	//{
	//	const bool pause_game_allowed;
	//	event(...);
	//}
}

void c_start_menu_screen_widget::update_pane()
{
	INVOKE_CLASS_MEMBER(0x00AE14C0, c_start_menu_screen_widget, update_pane);

	//c_gui_screen_widget* current_pane;
	//{
	//	string_id pane_to_load;
	//	{
	//		c_gui_list_widget* top_menu_widget;
	//		{
	//			string_id requested_pane;
	//			c_gui_data* datasource;
	//			int32 element_handle;
	//		}
	//	}
	//	{
	//		real32 appearance_camera_yaw;
	//		real32 appearance_camera_zoom;
	//	}
	//}
	//{
	//	c_gui_list_item_widget* list_item;
	//}
}

void c_start_menu_screen_widget::update_render_state(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AE1660, c_start_menu_screen_widget, update_render_state, current_milliseconds);

	//c_controller_interface* controller;
	//c_gui_text_widget* player_name_text;
	//e_player_color_index player_change_color_index;
	//const c_player_profile_interface* player_profile;
	//{
	//	c_gui_widget* child_widget;
	//	{
	//		c_gui_bitmap_widget* base_color_bitmap;
	//		{
	//			c_gui_bitmap_widget* test_widget;
	//		}
	//	}
	//}
}

