#include "interface/gui_screens/campaign/gui_screen_campaign_select_skulls.hpp"

#include "tag_files/string_ids.hpp"

c_gui_primary_skulls_data::c_gui_primary_skulls_data() :
	c_gui_ordered_data(),
	m_skull_state()
{
	//DECLFUNC(0x00AFE470, void, __thiscall, c_gui_primary_skulls_data*)(this);

	c_gui_primary_skulls_data::update();
}

//.text:00AFE4C0 ; public: c_gui_screen_campaign_select_skulls::c_gui_screen_campaign_select_skulls(int32)

c_gui_secondary_skulls_data::c_gui_secondary_skulls_data() :
	c_gui_ordered_data(),
	m_skull_state()
{
	//DECLFUNC(0x00AFE510, void, __thiscall, c_gui_secondary_skulls_data*)(this);

	c_gui_secondary_skulls_data::update();
}

//.text:00AFE560 ; 
//.text:00AFE570 ; 
//.text:00AFE580 ; 
//.text:00AFE590 ; public: virtual void* c_gui_primary_skulls_data::`vector deleting destructor'(unsigned int)
//.text:00AFE5C0 ; public: virtual void* c_gui_screen_campaign_select_skulls::`scalar deleting destructor'(unsigned int)
//.text:00AFE5F0 ; public: virtual void* c_gui_secondary_skulls_data::`scalar deleting destructor'(unsigned int)

void c_gui_primary_skulls_data::get_column_names(string_id* const column_names, int32* column_count)
{
	//INVOKE_CLASS_MEMBER(0x00AFE620, c_gui_primary_skulls_data, get_column_names, column_names, column_count);

	*column_count = 0;
	column_names[*column_count++] = STRING_ID(gui, primary_skull_image);
	column_names[*column_count++] = STRING_ID(gui, primary_skull_state);
	column_names[*column_count++] = STRING_ID(gui, skull_status);
	column_names[*column_count++] = STRING_ID(gui, skull_description);
	column_names[*column_count++] = STRING_ID(gui, skull_name);
}

void c_gui_secondary_skulls_data::get_column_names(string_id* const column_names, int32* column_count)
{
	//INVOKE_CLASS_MEMBER(0x00AFE680, c_gui_secondary_skulls_data, get_column_names, column_names, column_count);

	*column_count = 0;
	column_names[*column_count++] = STRING_ID(gui, secondary_skull_image);
	column_names[*column_count++] = STRING_ID(gui, secondary_skull_state);
}

int32 c_gui_primary_skulls_data::get_current_item_count_internal()
{
	//return INVOKE_CLASS_MEMBER(0x00AFE6E0, c_gui_primary_skulls_data, get_current_item_count_internal);

	return k_campaign_skull_primary_count;
}

int32 c_gui_secondary_skulls_data::get_current_item_count_internal()
{
	//return INVOKE_CLASS_MEMBER(0x00AFE6F0, c_gui_secondary_skulls_data, get_current_item_count_internal);

	return k_campaign_skull_secondary_count;
}

bool c_gui_primary_skulls_data::get_integer_value(int32 element_handle, string_id value_name, int32* value)
{
	//return INVOKE_CLASS_MEMBER(0x00AFE700, c_gui_primary_skulls_data, get_integer_value, element_handle, value_name, value);

	switch (value_name)
	{
	case STRING_ID(gui, primary_skull_state):
	{
		if (element_handle < k_campaign_skull_primary_count)
		{
			ASSERT(VALID_INDEX(m_skull_state[element_handle], k_skull_state_count));

			*value = m_skull_state[element_handle];
			return true;
		}
	}
	break;
	case STRING_ID(gui, primary_skull_image):
	{
		if (element_handle < k_campaign_skull_primary_count)
		{
			ASSERT(VALID_INDEX(m_skull_state[element_handle], k_skull_state_count));

			*value = 0;
			if (m_skull_state[element_handle] != _skull_locked)
			{
				*value = element_handle + 1;
			}
			return true;
		}
	}
	break;
	}

	return false;
}

bool c_gui_secondary_skulls_data::get_integer_value(int32 element_handle, string_id value_name, int32* value)
{
	//return INVOKE_CLASS_MEMBER(0x00AFE770, c_gui_secondary_skulls_data, get_integer_value, element_handle, value_name, value);

	switch (value_name)
	{
	case STRING_ID(gui, secondary_skull_state):
	{
		if (element_handle < k_campaign_skull_secondary_count)
		{
			ASSERT(VALID_INDEX(m_skull_state[element_handle], k_skull_state_count));

			*value = m_skull_state[element_handle];
			return true;
		}
	}
	break;
	case STRING_ID(gui, secondary_skull_image):
	{
		if (element_handle < k_campaign_skull_secondary_count)
		{
			ASSERT(VALID_INDEX(m_skull_state[element_handle], k_skull_state_count));

			*value = 0;
			if (m_skull_state[element_handle] != _skull_locked)
			{
				*value = element_handle + 1;
			}
			return true;
		}
	}
	break;
	}

	return false;
}

bool c_gui_primary_skulls_data::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AFE7D0, c_gui_primary_skulls_data, get_string_id_value, element_handle, value_name, value);
}

bool c_gui_secondary_skulls_data::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AFE890, c_gui_secondary_skulls_data, get_string_id_value, element_handle, value_name, value);
}

//.text:00AFE960 ; public: virtual bool c_gui_screen_campaign_select_skulls::handle_controller_input_message(const c_controller_input_message*)
//.text:00AFED80 ; public: virtual bool c_gui_screen_campaign_select_skulls::handle_dialog_result(const c_dialog_result_message*)
//.text:00AFEE20 ; public: virtual bool c_gui_screen_campaign_select_skulls::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AFEF60 ; public: virtual void c_gui_screen_campaign_select_skulls::initialize()
//.text:00AFEF90 ; public: virtual void c_gui_screen_campaign_select_skulls::initialize_datasource()
//.text:00AFF0D0 ; public: void c_gui_screen_campaign_select_skulls::set_focus(c_gui_screen_campaign_select_skulls::e_focus_state, int16)
//.text:00AFF270 ; 
//.text:00AFF290 ; 

void c_gui_primary_skulls_data::update()
{
	INVOKE_CLASS_MEMBER(0x00AFF2B0, c_gui_primary_skulls_data, update);
}

//.text:00AFF460 ; public: virtual void c_gui_screen_campaign_select_skulls::update(uns32)

void c_gui_secondary_skulls_data::update()
{
	INVOKE_CLASS_MEMBER(0x00AFF650, c_gui_secondary_skulls_data, update);
}

