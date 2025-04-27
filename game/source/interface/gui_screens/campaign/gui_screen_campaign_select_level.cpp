#include "interface/gui_screens/campaign/gui_screen_campaign_select_level.hpp"

#include "interface/gui_selected_items_level.hpp"
#include "interface/user_interface_data.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AFD9B0, c_gui_screen_campaign_select_level, post_initialize_);

//.text:00AFCA00 ; public: c_gui_screen_campaign_select_level::c_gui_screen_campaign_select_level(int32)
//.text:00AFCAE0 ; public: virtual c_gui_screen_campaign_select_level::~c_gui_screen_campaign_select_level()
//.text:00AFCBA0 ; public: virtual void* c_gui_screen_campaign_select_level::`vector deleting destructor'(unsigned int)
//.text:00AFCC00 ; public: virtual c_gui_bitmap_widget* c_gui_screen_campaign_select_level::create_bitmap_widget(s_runtime_bitmap_widget_definition const*)
//.text:00AFCE00 ; public: virtual bool c_gui_screen_campaign_select_level::handle_controller_input_message(c_controller_input_message const*)
//.text:00AFCF20 ; public: virtual bool c_gui_screen_campaign_select_level::handle_dialog_result(c_dialog_result_message const*)
//.text:00AFCF90 ; public: virtual bool c_gui_screen_campaign_select_level::handle_list_item_chosen(c_controller_input_message const*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AFD180 ; public: virtual void c_gui_screen_campaign_select_level::initialize()
//.text:00AFD600 ; public: virtual void c_gui_screen_campaign_select_level::initialize_datasource()
//.text:00AFD6F0 ; private: void c_gui_screen_campaign_select_level::level_chosen_immediate()
//.text:00AFD850 ; 

void __thiscall c_gui_screen_campaign_select_level::post_initialize_()
{
	//c_gui_screen_widget::post_initialize();
	INVOKE_CLASS_MEMBER(0x00AB18E0, c_gui_screen_widget, post_initialize);

	c_gui_data* level_data = get_data(STRING_ID(global, level), NULL);
	c_gui_list_widget* level_list_widget = get_child_list_widget(STRING_ID(global, level));
	if (level_list_widget && level_data)
	{
		DECLARE_LAMBDA2(get_element_handle, int32, c_gui_screen_campaign_select_level* this_ptr, c_gui_data* level_data)
		{
			for (int32 element_handle = level_data->get_first_element_handle();
				element_handle != NONE;
				element_handle = level_data->get_next_element_handle(element_handle))
			{
				c_gui_selected_item const* selected_item = level_data->get_gui_selected_item(element_handle);
				if (selected_item && selected_item->m_selection_type == _gui_selection_type_level)
				{
					c_gui_level_selected_item* level_selected_item = (c_gui_level_selected_item*)selected_item;
					if (level_selected_item->m_campaign_id == this_ptr->m_campaign_id &&
						level_selected_item->m_map_id == this_ptr->m_map_id)
					{
						return element_handle;
					}
				}
			}

			return NONE;
		};

		int32 element_handle = get_element_handle(this, level_data);
		if (element_handle != NONE)
			c_gui_screen_widget::transfer_focus_to_list(level_list_widget, element_handle, false, false);
	}
}

void c_gui_screen_campaign_select_level::setup(e_gui_campaign_level_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, int16 campaign_insertion_point)
{
	//INVOKE_CLASS_MEMBER(0x00AFDBA0, c_gui_screen_campaign_select_level, setup, campaign_setup_mode, campaign_id, map_id, campaign_insertion_point);

	m_campaign_setup_mode = campaign_setup_mode;
	m_campaign_id = campaign_id;
	m_map_id = map_id;
	m_campaign_insertion_point = campaign_insertion_point;
}

