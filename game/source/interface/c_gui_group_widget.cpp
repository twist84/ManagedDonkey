#include "interface/c_gui_group_widget.hpp"

//.text:00B13EE0 ; public: c_gui_button_key_widget::c_gui_button_key_widget(void)
//.text:00B13F10 ; public: c_gui_group_widget::c_gui_group_widget(e_gui_widget_type type)
//.text:00B13F40 ; public: c_gui_group_widget::c_gui_group_widget(void)
//.text:00B13F70 ; public: s_runtime_group_widget_definition::s_runtime_group_widget_definition(void)
//.text:00B14030 ; 
//.text:00B14040 ; 
//.text:00B14050 ; public: virtual void* c_gui_button_key_widget::`scalar deleting destructor'(unsigned int)
//.text:00B14080 ; public: virtual void* c_gui_group_widget::`scalar deleting destructor'(unsigned int)
//.text:00B140B0 ; public: void c_gui_button_key_widget::add_definition_fields(const s_button_key_definition* definition, bool was_templated)
//.text:00B14160 ; public: void c_gui_group_widget::add_definition_fields(const s_group_widget_definition* definition, bool was_templated)

bool c_gui_group_widget::contains_submenu_control_definition(string_id submenu_name, s_list_widget_block* submenu_definition)
{
	return INVOKE_CLASS_MEMBER(0x00B14250, c_gui_group_widget, contains_submenu_control_definition, submenu_name, submenu_definition);
}

//.text:00B14320 ; protected: void c_gui_group_widget::create_and_add_children_from_definition(void)
//.text:00B14440 ; public: virtual c_gui_bitmap_widget* c_gui_group_widget::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
//.text:00B14450 ; public: virtual c_gui_button_key_widget* c_gui_group_widget::create_button_key_widget(const s_button_key_definition* definition)
//.text:00B14460 ; public: virtual c_gui_group_widget* c_gui_group_widget::create_group_widget(const s_group_widget_definition* definition)
//.text:00B14470 ; public: virtual c_gui_list_item_widget* c_gui_group_widget::create_list_item_widget(const s_list_item_widget_block* definition)
//.text:00B14480 ; public: virtual c_gui_list_widget* c_gui_group_widget::create_list_widget(const s_list_widget_block* definition)
//.text:00B14490 ; public: virtual c_gui_model_widget* c_gui_group_widget::create_model_widget(const s_model_widget_block* definition)
//.text:00B144A0 ; public: virtual c_gui_text_widget* c_gui_group_widget::create_text_widget(const s_runtime_text_widget_definition* definition)
//.text:00B144B0 ; public: virtual s_runtime_core_widget_definition* c_gui_group_widget::get_core_definition(void)
//.text:00B144C0 ; public: virtual bool c_gui_button_key_widget::get_string_by_string_id(string_id string_identifier, c_static_wchar_string<1024>* buffer)
//.text:00B14500 ; public: static bool c_gui_group_widget::group_widget_loaded_on_initialization(struct s_group_widget_block const *)
//.text:00B14540 ; 
//.text:00B145B0 ; 
//.text:00B14610 ; 
//.text:00B146E0 ; 
//.text:00B147B0 ; public: virtual void c_gui_button_key_widget::initialize(const s_button_key_block*, bool)
//.text:00B14800 ; public: virtual void c_gui_group_widget::initialize(const s_group_widget_block*, bool)
//.text:00B14870 ; 

