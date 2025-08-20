#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"

#include "cseries/cseries_events.hpp"
#include "cutscene/cinematics.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/gui_screens/start_menu/panes/settings/start_menu_settings.hpp"
#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "networking/online/online_service_record.hpp"
#include "simulation/simulation.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE05B0, c_start_menu_screen_widget, handle_controller_input_message_);
HOOK_DECLARE_CLASS(0x00AE0720, c_start_menu_screen_widget, handle_global_start_button_press_);

byte const handle_global_start_button_press_patch_bytes[] = { 0xEB, 0x10 };
DATA_PATCH_DECLARE(0x00AAC890, handle_global_start_button_press, handle_global_start_button_press_patch_bytes);

void __thiscall c_start_menu_screen_widget::back_out_current_pane_()
{
	c_start_menu_screen_widget::back_out_current_pane();
}

bool __thiscall c_start_menu_screen_widget::handle_controller_input_message_(c_controller_input_message* message)
{
	e_event_type event_type = message->get_event_type();
	switch (event_type)
	{
	case _event_type_button_press:
	{
		e_controller_component component = message->get_component();
		switch (component)
		{
		case _controller_component_button_b:
		{
			if (m_breadcrumbs.count() == 1)
			{
				c_gui_screen_widget::transition_out(_transition_out_back_out);
			}
			else
			{
				c_start_menu_screen_widget::back_out_current_pane();
			}

			return true;
		}
		break;
		case _controller_component_button_start:
		{
			if (!c_start_menu_screen_widget::transitioning_out())
			{
				c_gui_screen_widget::transition_out(_transition_out_back_out);
			}

			return true;
		}
		break;
		}
	}
	break;
	case _event_type_tab_left:
	case _event_type_tab_right:
	{
		if (!m_is_rooted || m_breadcrumbs.count() != 1)
		{
			break;
		}

		c_gui_widget* focused_widget = c_gui_screen_widget::get_focused_widget();
		c_gui_list_widget* parent_list = focused_widget->get_parent_list();
		int32 element_handle = parent_list->get_focused_element_handle();
		ASSERT(element_handle != NONE);

		c_gui_data* data = parent_list->get_data();
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

		int32 pane_to_show = NONE;
		if (!data->get_string_id_value(target_element_handle, STRING_ID(gui, target), &pane_to_show)
			|| pane_to_show == NONE
			|| pane_to_show == k_string_id_empty_string)
		{
			break;
		}

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
	break;
	}

	return c_gui_screen_widget::handle_controller_input_message(message);
}

bool __cdecl c_start_menu_screen_widget::handle_global_start_button_press_(c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_start)
	{
		e_controller_index controller_index = message->get_controller();
		const c_controller_interface* controller = controller_get(controller_index);

		if (simulation_starting_up())
		{
			event(_event_status, "ui:start_menu: Can't show start menu -- simulation starting up.");
			return false;
		}
		if (!controller->in_use() || controller->get_user_index() == NONE)
		{
			event(_event_error, "ui:start_menu: controller not in use.");
			return false;
		}

		s_player_identifier player_identifier{};
		controller->get_player_identifier(&player_identifier);
		uns64 const player_xuid = controller->get_player_xuid();
		return c_start_menu_screen_widget::load_start_menu(
			controller_index,
			&player_identifier,
			&player_xuid,
			NULL,
			NULL,
			0);
	}

	return false;
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
//.text:00AE03A0 ; private: bool c_start_menu_screen_widget::current_pane_is_root_pane() const
//.text:00AE03C0 ; public: virtual void c_start_menu_screen_widget::dispose()
//.text:00AE03F0 ; 
//.text:00AE0400 ; 
//.text:00AE0420 ; 
//.text:00AE0430 ; 
//.text:00AE0440 ; public: virtual void c_start_menu_screen_widget_sidebar_items_datasource::get_column_names(int32* const, int32*)
//.text:00AE0470 ; public: static int32 __cdecl c_start_menu_screen_widget::get_current_game_pane_name()
//.text:00AE04C0 ; protected: virtual int32 c_start_menu_screen_widget_sidebar_items_datasource::get_current_item_count_internal()

c_gui_screen_widget* c_start_menu_screen_widget::get_current_pane()
{
	//return INVOKE_CLASS_MEMBER(0x00AE04D0, c_start_menu_screen_widget, get_current_pane);

	if (m_requested_pane == NONE)
	{
		return NULL;
	}

	return (c_gui_screen_widget*)c_gui_widget::get_child_widget(_gui_screen, m_requested_pane);
}

//.text:00AE04F0 ; protected: virtual e_render_data_size c_start_menu_screen_widget::get_render_data_size()
//.text:00AE0500 ; public: virtual bool c_start_menu_screen_widget_sidebar_items_datasource::get_string_id_value(int32, int32, int32*)
//.text:00AE0580 ; 
//.text:00AE0590 ; 
//.text:00AE05A0 ; 
//.text:00AE05B0 ; public: virtual bool c_start_menu_screen_widget::handle_controller_input_message(const c_controller_input_message*)
//.text:00AE0720 ; public: static bool __cdecl c_start_menu_screen_widget::handle_global_start_button_press(const c_controller_input_message*)
//.text:00AE07A0 ; public: virtual void c_start_menu_screen_widget::initialize()
//.text:00AE0870 ; public: virtual void c_start_menu_screen_widget::initialize_datasource()
//.text:00AE0920 ; public: virtual void c_start_menu_screen_widget::transition_out_with_transition_type(e_screen_transition_type)

void c_start_menu_screen_widget::load_pane(int32 pane_to_load, bool is_top_level_pane, e_screen_transition_type transition_type, int32 focus_on_load_list_name, int32 focus_on_load_element_handle, real32 appearance_camera_zoom, real32 appearance_camera_yaw)
{
	INVOKE_CLASS_MEMBER(0x00AE09A0, c_start_menu_screen_widget, load_pane, pane_to_load, is_top_level_pane, transition_type, focus_on_load_list_name, focus_on_load_element_handle, appearance_camera_zoom, appearance_camera_yaw);
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
//.text:00AE0F00 ; public: virtual void c_start_menu_screen_widget::post_initialize()
//.text:00AE0F80 ; public: virtual bool c_start_menu_screen_widget::process_message(const c_message*)
//.text:00AE10A0 ; 
//.text:00AE10B0 ; 
//.text:00AE10D0 ; public: virtual void c_start_menu_screen_widget::reconstruct()
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
//.text:00AE1300 ; public: virtual void c_start_menu_screen_widget::transition_out_with_transition_type(e_transition_out_type, e_screen_transition_type)
//.text:00AE1360 ; public: virtual void c_start_menu_screen_widget::update(uns32)
//.text:00AE14C0 ; private: void c_start_menu_screen_widget::update_pane()
//.text:00AE1660 ; public: virtual void c_start_menu_screen_widget::update_render_state(uns32)

