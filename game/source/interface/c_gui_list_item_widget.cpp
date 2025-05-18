#include "interface/c_gui_list_item_widget.hpp"

//.text:00B18260 ; public: __cdecl c_gui_list_item_widget::c_gui_list_item_widget()
//.text:00B18350 ; 

//.text:00B183B0 ; public: virtual void* __cdecl c_gui_list_item_widget::`vector deleting destructor'(unsigned int)
c_gui_list_item_widget::~c_gui_list_item_widget()
{
	DECLFUNC(0x00B183B0, void, __thiscall, c_gui_list_item_widget*)(this);
}

bool c_gui_list_item_widget::can_receive_focus()
{
	return INVOKE_CLASS_MEMBER(0x00B183E0, c_gui_list_item_widget, can_receive_focus);
}

c_gui_bitmap_widget* c_gui_list_item_widget::create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18420, c_gui_list_item_widget, create_bitmap_widget, definition);
}

c_gui_button_key_widget* c_gui_list_item_widget::create_button_key_widget(s_button_key_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18430, c_gui_list_item_widget, create_button_key_widget, definition);
}

c_gui_group_widget* c_gui_list_item_widget::create_group_widget(s_group_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18440, c_gui_list_item_widget, create_group_widget, definition);
}

c_gui_list_item_widget* c_gui_list_item_widget::create_list_item_widget(s_list_item_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18450, c_gui_list_item_widget, create_list_item_widget, definition);
}

c_gui_list_widget* c_gui_list_item_widget::create_list_widget(s_list_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18460, c_gui_list_item_widget, create_list_widget, definition);
}

c_gui_model_widget* c_gui_list_item_widget::create_model_widget(s_model_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18470, c_gui_list_item_widget, create_model_widget, definition);
}

c_gui_text_widget* c_gui_list_item_widget::create_text_widget(s_runtime_text_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B18480, c_gui_list_item_widget, create_text_widget, definition);
}

s_runtime_core_widget_definition* c_gui_list_item_widget::get_core_definition()
{
	return INVOKE_CLASS_MEMBER(0x00B18490, c_gui_list_item_widget, get_core_definition);
}

int32 c_gui_list_item_widget::get_element_handle()
{
	return INVOKE_CLASS_MEMBER(0x00B184A0, c_gui_list_item_widget, get_element_handle);
}

int32 c_gui_list_item_widget::get_item_label()
{
	return INVOKE_CLASS_MEMBER(0x00B184C0, c_gui_list_item_widget, get_item_label);
}

//.text:00B184D0 ; 
//.text:00B184E0 ; public: c_gui_list_item_widget* c_gui_list_item_widget::get_sibling_list_item_widget(bool, int)
//.text:00B18520 ; 

bool c_gui_list_item_widget::handle_widget_back_out()
{
	return INVOKE_CLASS_MEMBER(0x00B18530, c_gui_list_item_widget, handle_widget_back_out);
}

bool c_gui_list_item_widget::handle_widget_selected()
{
	return INVOKE_CLASS_MEMBER(0x00B185F0, c_gui_list_item_widget, handle_widget_selected);
}

void c_gui_list_item_widget::initialize(s_list_item_widget_block const* item_block)
{
	INVOKE_CLASS_MEMBER(0x00B18640, c_gui_list_item_widget, initialize, item_block);
}

//.text:00B186C0 ; public: bool c_gui_list_item_widget::invokes_submenu(int32*)
//.text:00B18760 ; 
//.text:00B18770 ; 
//.text:00B18780 ; 
//.text:00B187A0 ; 
//.text:00B187D0 ; public: void c_gui_list_item_widget::set_list_item_index(int32)

bool c_gui_list_item_widget::should_render(bool* add_to_render_list)
{
	return INVOKE_CLASS_MEMBER(0x00B187E0, c_gui_list_item_widget, should_render, add_to_render_list);

	//return c_gui_widget::should_render(add_to_render_list) & (get_element_handle() != NONE);
}

