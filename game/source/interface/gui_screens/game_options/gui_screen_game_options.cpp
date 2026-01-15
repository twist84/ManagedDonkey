#include "interface/gui_screens/game_options/gui_screen_game_options.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"

//.text:00B0AEA0 ; 
//.text:00B0AEC0 ; 
//.text:00B0AEE0 ; 
//.text:00B0AEF0 ; 

c_game_options_category_datasource::c_game_options_category_datasource(e_game_engine_category template_category, e_game_engine_category interface_category, const c_game_variant* edit_variant) :
	c_gui_ordered_data(),
	m_edit_variant(edit_variant),
	m_template_category(template_category),
	m_interface_category(interface_category)
{
	//DECLFUNC(0x00B0AF00, void, __thiscall, c_game_options_category_datasource*, e_game_engine_category, e_game_engine_category, const c_game_variant*)(
	//	this, template_category, interface_category, edit_variant);

	ASSERT(edit_variant != NULL);
	ASSERT(VALID_INDEX(m_template_category, k_number_of_editable_game_engine_category_groups) || m_template_category == k_invalid_category);

	if (m_interface_category > k_number_of_editable_game_engine_category_groups)
	{
		event(_event_warning, "ui: invalid interface settings category '%d' - bad variant editing options UI tags? template-category= '%d' interface-category= '%d' variant-name= '%S'",
			m_interface_category,
			template_category,
			interface_category,
			edit_variant->get_active_variant()->get_metadata()->display_name);
	}
}

c_game_options_parameter_datasource::c_game_options_parameter_datasource(e_game_variant_parameter parameter) :
	c_gui_ordered_data(),
	m_parameter(parameter)
{
	//DECLFUNC(0x00B0AF40, void, __thiscall, c_game_options_parameter_datasource*, e_game_variant_parameter)(
	//	this, parameter);

	ASSERT(VALID_INDEX(parameter, k_number_of_editable_game_variant_parameters));
}

c_gui_screen_game_options::c_gui_screen_game_options(int32 name) :
	c_gui_screen_widget(name),
	m_edit_game_variant(),
	m_revert_game_variant(),
	m_editing_parameter(k_invalid_parameter),
	m_settings_history_stack(),
	m_setting_stack_depth(0),
	m_layered_position(NONE),
	m_asynchronous_operation(_operation_none),
	m_confirmation_action(_invalid_result),
	m_confirmation_result(_result_indeterminate),
	m_variant_on_disk(),
	m_asynchronous_operation_controller(k_no_controller),
	m_keyboard_task(NULL),
	m_virtual_keyboard_results_buffer(),
	m_string_verify_task(),
	m_saved_game_name_buffer(),
	m_saved_game_description_buffer(),
	m_multiplayer_setup(),
	m_create_new_variant_task(),
	m_write_new_variant_task(),
	m_delete_edit_variant_task(),
	m_write_variant_task(),
	m_async_success(),
	m_async_complete(),
	m_content_item_index(),
	m_progress_dialog_start_time_milliseconds()
{
	//DECLFUNC(0x00B0AF70, void, __thiscall, c_gui_screen_game_options*, int32)(
	//	this, name);
}

c_load_game_options_screen_message::c_load_game_options_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, const c_game_variant* edit_game_variant, const c_game_variant* revert_game_variant, e_game_engine_category template_category, e_game_engine_category interface_category, int32 focused_element_handle, s_game_options_history_stack_node* settings_history_stack, int32 history_stack_depth) :
	c_load_screen_message(screen_name, controller, window, layered_position),
	m_edit_game_variant(),
	m_revert_game_variant(),
	m_template_category(template_category),
	m_interface_category(interface_category),
	m_focused_element_handle(focused_element_handle),
	m_settings_history_stack(),
	m_history_stack_depth(history_stack_depth),
	m_layered_position(layered_position)
{
	//DECLFUNC(0x00B0B140, void, __thiscall, c_load_game_options_screen_message*, int32, e_controller_index, e_window_index, int32, const c_game_variant*, const c_game_variant*, e_game_engine_category, e_game_engine_category, int32, s_game_options_history_stack_node*, int32)(
	//	this, screen_name, controller, window, layered_position, edit_game_variant, revert_game_variant, template_category, interface_category, focused_element_handle, settings_history_stack, history_stack_depth);

	ASSERT(edit_game_variant != NULL);

	m_edit_game_variant.copy_from_unsafe(edit_game_variant);
	m_revert_game_variant.copy_from_unsafe(revert_game_variant);

	if (!settings_history_stack || m_interface_category >= k_number_of_editable_game_engine_category_groups)
	{
		csmemset(m_settings_history_stack, NONE, sizeof(s_game_options_history_stack_node) * history_stack_depth);
		return;
	}

	if (!VALID_INDEX(history_stack_depth, NUMBEROF(m_settings_history_stack)))
	{
		event(_event_error, "ui: invalid history stack depth for game options editing ui (%ld) - should be in the range of [0,%ld]",
			history_stack_depth,
			NUMBEROF(m_settings_history_stack) - 1);
		return;
	}

	csmemcpy(m_settings_history_stack, settings_history_stack, sizeof(m_settings_history_stack));
}

//.text:00B0B270 ; 
//.text:00B0B280 ; public: virtual c_game_options_category_datasource::~c_game_options_category_datasource()
//.text:00B0B290 ; public: virtual c_game_options_parameter_datasource::~c_game_options_parameter_datasource()
//.text:00B0B2A0 ; public: virtual c_gui_screen_game_options::~c_gui_screen_game_options()
//.text:00B0B300 ; public: virtual c_load_game_options_screen_message::~c_load_game_options_screen_message()
//.text:00B0B310 ; public: virtual void* c_game_options_category_datasource::`vector deleting destructor'(unsigned int)
//.text:00B0B340 ; public: virtual void* c_game_options_parameter_datasource::`vector deleting destructor'(unsigned int)
//.text:00B0B370 ; public: virtual void* c_gui_screen_game_options::`vector deleting destructor'(unsigned int)
//.text:00B0B3E0 ; public: virtual void* c_load_game_options_screen_message::`vector deleting destructor'(unsigned int)

bool c_gui_screen_game_options::allow_user_to_transition()
{
	return INVOKE_CLASS_MEMBER(0x00B0B410, c_gui_screen_game_options, allow_user_to_transition);
}

//.text:00B0B4D0 ; public: void c_gui_screen_game_options::apply_initial_state(const c_game_variant*, const c_game_variant*, e_game_engine_category, e_game_engine_category, int32, const s_game_options_history_stack_node*, int32, int32)

void c_load_game_options_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	INVOKE_CLASS_MEMBER(0x00B0B590, c_load_game_options_screen_message, apply_initial_state, screen_widget);
}

bool c_gui_screen_game_options::back_out_from_current_editing_category(e_controller_index controller_index)
{
	return INVOKE_CLASS_MEMBER(0x00B0B5D0, c_gui_screen_game_options, back_out_from_current_editing_category, controller_index);
}

void c_gui_screen_game_options::begin_rename_edited_variant(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B0B780, c_gui_screen_game_options, begin_rename_edited_variant, controller_index);
}

void c_gui_screen_game_options::begin_save_as_current_edited_variant(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B0B920, c_gui_screen_game_options, begin_save_as_current_edited_variant, controller_index);
}

void c_gui_screen_game_options::begin_save_current_edited_variant(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B0BAE0, c_gui_screen_game_options, begin_save_current_edited_variant, controller_index);
}

bool __cdecl can_edit_settings()
{
	return INVOKE(0x00B0BC30, can_edit_settings);

	//return !user_interface_squad_exists() || user_interface_squad_local_peer_is_leader();
}

bool __cdecl can_save_settings(e_game_engine_type game_engine)
{
	return INVOKE(0x00B0BC40, can_save_settings, game_engine);

	//return can_edit_settings() && game_engine != _game_engine_type_sandbox;
}

//.text:00B0BC70 ; public: c_flags<e_text_value_pair_flags, uns8, 1>::c_flags<e_text_value_pair_flags, uns8, k_number_of_text_value_pair_flags>()
//.text:00B0BC80 ; protected: void c_gui_screen_game_options::finish_editing_setting()

void c_game_options_category_datasource::get_column_names(string_id* const column_names, int32* column_count)
{
	INVOKE_CLASS_MEMBER(0x00B0BC90, c_game_options_category_datasource, get_column_names, column_names, column_count);

	//*column_count = 0;
	//column_names[(*column_count)++] = STRING_ID(global, value);
}

void c_game_options_parameter_datasource::get_column_names(string_id* const column_names, int32* column_count)
{
	INVOKE_CLASS_MEMBER(0x00B0BCB0, c_game_options_parameter_datasource, get_column_names, column_names, column_count);

	//*column_count = 0;
	//column_names[(*column_count)++] = STRING_ID(global, value);
	//column_names[(*column_count)++] = STRING_ID(global, name);
	//column_names[(*column_count)++] = STRING_ID(global, description);
}

int32 c_game_options_category_datasource::get_current_item_count_internal()
{
	return INVOKE_CLASS_MEMBER(0x00B0BCF0, c_game_options_category_datasource, get_current_item_count_internal);

	//if (!VALID_INDEX(m_interface_category, k_number_of_editable_game_engine_category_groups))
	//{
	//	return 0;
	//}
	//
	//s_variant_setting_edit_reference* master = multiplayer_variant_settings_interface_definition_try_and_get_master(m_interface_category);
	//if (!master)
	//{
	//	return 0;
	//}
	//
	//return master->options.count;
}

int32 c_game_options_parameter_datasource::get_current_item_count_internal()
{
	return INVOKE_CLASS_MEMBER(0x00B0BD20, c_game_options_parameter_datasource, get_current_item_count_internal);

	//s_text_value_pair const* text_value_pair = text_value_pair_try_and_get(m_parameter);
	//if (!text_value_pair)
	//{
	//	return 0;
	//}
	//
	//return text_value_pair->text_value_pair_block.count;
}

e_game_variant_parameter c_gui_screen_game_options::get_editing_parameter() const
{
	return INVOKE_CLASS_MEMBER(0x00B0BD40, c_gui_screen_game_options, get_editing_parameter);

	//return m_editing_parameter;
}

bool c_game_options_category_datasource::get_element(int32 element_handle, void* element, int32 element_size)
{
	return INVOKE_CLASS_MEMBER(0x00B0BD50, c_game_options_category_datasource, get_element, element_handle, element, element_size);
}

bool c_game_options_parameter_datasource::get_integer_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00B0BDC0, c_game_options_parameter_datasource, get_integer_value, element_handle, value_name, value);
}

e_game_engine_category c_gui_screen_game_options::get_interface_setting() const
{
	return INVOKE_CLASS_MEMBER(0x00B0BE10, c_gui_screen_game_options, get_interface_setting);

	//if (!VALID_INDEX(m_setting_stack_depth, NUMBEROF(m_settings_history_stack)))
	//{
	//	return k_invalid_category;
	//}
	//
	//return m_settings_history_stack[m_setting_stack_depth].interface_category;
}

bool c_game_options_category_datasource::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00B0BE30, c_game_options_category_datasource, get_string_id_value, element_handle, value_name, value);
}

bool c_game_options_parameter_datasource::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00B0C010, c_game_options_parameter_datasource, get_string_id_value, element_handle, value_name, value);

	//s_text_value_pair const* text_value_pair = text_value_pair_try_and_get(m_parameter);
	//if (!text_value_pair)
	//{
	//	return false;
	//}
	//
	//switch (value_name)
	//{
	//case STRING_ID(global, value):
	//{
	//	if (text_value_pair->type != 1)
	//	{
	//		break;
	//	}
	//
	//	*value = text_value_pair->string_id_value;
	//	return true;
	//}
	//break;
	//case STRING_ID(global, name):
	//{
	//	*value = text_value_pair->text_label;
	//	return true;
	//}
	//break;
	//case STRING_ID(global, description):
	//{
	//	*value = text_value_pair->description_text;
	//	return true;
	//}
	//break;
	//}
	//
	//return false;
}

e_game_engine_category c_gui_screen_game_options::get_template_setting() const
{
	//return INVOKE_CLASS_MEMBER(0x00B0C090, c_gui_screen_game_options, get_template_setting);

	if (!VALID_INDEX(m_setting_stack_depth, NUMBEROF(m_settings_history_stack)))
	{
		return k_invalid_category;
	}

	return m_settings_history_stack[m_setting_stack_depth].template_category;
}

bool c_gui_screen_game_options::handle_controller_input_message(const c_controller_input_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B0C0B0, c_gui_screen_game_options, handle_controller_input_message, message);

	// $IMPLEMENT

	//if (message->get_event_type() == _event_type_button_press)
	//{
	//	switch (message->get_component())
	//	{
	//	case _controller_component_button_b:
	//	{
	//		if (!c_gui_screen_game_options::allow_user_to_transition())
	//		{
	//			break;
	//		}
	//
	//		if (!c_gui_screen_game_options::back_out_from_current_editing_category(message->get_controller()))
	//		{
	//			break;
	//		}
	//		
	//		return true;
	//	}
	//	break;
	//	case _controller_component_button_x:
	//	{
	//		if (!can_save_settings(m_edit_game_variant.get_game_engine_index()))
	//		{
	//			break;
	//		}
	//
	//		s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();
	//		if (!last_game_setup)
	//		{
	//			break;
	//		}
	//
	//		// $IMPLEMENT
	//	}
	//	break;
	//	case _controller_component_button_back:
	//	{
	//		if (!c_gui_screen_game_options::allow_user_to_transition())
	//		{
	//			break;
	//		}
	//
	//		if (!c_gui_screen_game_options::back_out_from_current_editing_category(message->get_controller()))
	//		{
	//			break;
	//		}
	//
	//		return true;
	//	}
	//	break;
	//	}
	//}
	//
	//return c_gui_screen_widget::handle_controller_input_message(message);
}

bool c_gui_screen_game_options::handle_dialog_result(const c_dialog_result_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B0C200, c_gui_screen_game_options, handle_dialog_result, message);
}

bool c_gui_screen_game_options::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00B0C470, c_gui_screen_game_options, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
}

void c_gui_screen_game_options::initialize()
{
	INVOKE_CLASS_MEMBER(0x00B0CA70, c_gui_screen_game_options, initialize);

	//c_gui_screen_widget::initialize();
	//
	//c_gui_screen_widget::add_game_tag_parser(new (_ui_allocation_marker_dummy) c_magic_string_game_tag_parser(L"<current-value", this, parse_current_selected_value));
	//c_gui_screen_widget::add_game_tag_parser(new (_ui_allocation_marker_dummy) c_magic_string_game_tag_parser(L"<current-name", this, parse_current_selected_name));
}

void c_gui_screen_game_options::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00B0CB20, c_gui_screen_game_options, initialize_datasource);
}

//.text:00B0CD10 ; protected: void c_gui_screen_game_options::load_progress_ui(e_controller_index)

bool __cdecl parse_current_selected_item_nugget(void* this_ptr, int32 nugget_id, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B0CDB0, parse_current_selected_item_nugget, this_ptr, nugget_id, buffer, buffer_length);

	//ASSERT(this_ptr != NULL);
	//
	//c_gui_screen_widget* screen_widget = (c_gui_screen_widget*)this_ptr;
	//
	//c_gui_widget* focused_widget = screen_widget->get_focused_widget();
	//int32 element_handle = NONE;
	//if (focused_widget)
	//{
	//	element_handle = focused_widget->get_element_handle();
	//}
	//
	//if (element_handle == NONE)
	//{
	//	return false;
	//}
	//
	//c_gui_list_widget* parent_list = focused_widget->get_parent_list();
	//if (!parent_list)
	//{
	//	return false;
	//}
	//
	//c_gui_data* parent_list_data = parent_list->get_data();
	//if (!parent_list_data)
	//{
	//	return false;
	//}
	//
	//int32 integer_value = 0;
	//if (parent_list_data->get_integer_value(element_handle, nugget_id, &integer_value))
	//{
	//	usnzprintf(buffer, buffer_length, L"%ld", integer_value);
	//	return true;
	//}
	//
	//int32 string_id_value = 0;
	//if (!parent_list_data->get_string_id_value(element_handle, nugget_id, &string_id_value))
	//{
	//	return false;
	//}
	//
	//c_static_wchar_string<1024> result_buffer;
	//if (!screen_widget->get_string_by_string_id(string_id_value, &result_buffer))
	//{
	//	return false;
	//}
	//
	//ustrnzcpy(buffer, result_buffer.get_string(), buffer_length);
	//return true;
}

bool __cdecl parse_current_selected_name(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B0CEA0, parse_current_selected_name, this_ptr, buffer, buffer_length);

	//return parse_current_selected_item_nugget(this_ptr, STRING_ID(global, name), buffer, buffer_length);
}

bool __cdecl parse_current_selected_value(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B0CEC0, parse_current_selected_value, this_ptr, buffer, buffer_length);

	//return parse_current_selected_item_nugget(this_ptr, STRING_ID(global, value), buffer, buffer_length);
}

void c_gui_screen_game_options::post_initialize()
{
	INVOKE_CLASS_MEMBER(0x00B0CEE0, c_gui_screen_game_options, post_initialize);

	//c_gui_screen_widget::post_initialize();
	//if (!VALID_INDEX(m_setting_stack_depth, NUMBEROF(m_settings_history_stack)))
	//{
	//	return;
	//}
	//
	//int32 focused_element_handle = m_settings_history_stack[m_setting_stack_depth].focused_element_handle;
	//if (focused_element_handle == NONE)
	//{
	//	return;
	//}
	//
	//c_gui_list_widget* game_options_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, game_options));
	//if (!game_options_widget)
	//{
	//	event(_event_warning, "ui: failed to restore focused setting list category - no list widget found!");
	//	return;
	//}
	//
	//c_gui_screen_widget::transfer_focus_to_list(game_options_widget, focused_element_handle, false, false);
}

//.text:00B0CF30 ; protected: void c_gui_screen_game_options::propagate_changes_from_newly_created_game_variant_file(e_controller_index, int32, e_saved_game_file_type)
//.text:00B0D070 ; protected: void c_gui_screen_game_options::propagate_changes_to_currently_selected_game_variant_file(e_controller_index)
//.text:00B0D180 ; protected: void c_gui_screen_game_options::revert_current_edited_variant(e_controller_index)
//.text:00B0D190 ; protected: bool c_gui_screen_game_options::saved_game_variant_file_already_exists(e_controller_index, const wchar_t*) const
//.text:00B0D1C0 ; protected: void c_gui_screen_game_options::set_editing_parameter(e_game_variant_parameter)
//.text:00B0D1D0 ; 
//.text:00B0D1F0 ; private: void c_gui_screen_game_options::setup_game_variant_for_write(s_blffile_game_variant*, const wchar_t*, const c_game_variant*)
//.text:00B0D330 ; private: bool c_gui_screen_game_options::start_create_game_variant(const wchar_t*)

s_text_value_pair const* c_game_options_parameter_datasource::text_value_pair_try_and_get(int32 element_handle) const
{
	return INVOKE_CLASS_MEMBER(0x00B0D3E0, c_game_options_parameter_datasource, text_value_pair_try_and_get, element_handle);
}

void c_gui_screen_game_options::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B0D410, c_gui_screen_game_options, update, current_milliseconds);
}

//.text:00B0D520 ; public: void c_gui_screen_game_options::update_rename_operation()
//.text:00B0D8C0 ; public: void c_gui_screen_game_options::update_revert_changes_operation()
//.text:00B0D900 ; public: void c_gui_screen_game_options::update_save_as_new_operation()
//.text:00B0DED0 ; public: void c_gui_screen_game_options::update_save_operation()
//.text:00B0DFF0 ; protected: bool c_gui_screen_game_options::user_is_editing_setting() const

