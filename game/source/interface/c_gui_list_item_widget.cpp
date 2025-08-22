#include "interface/c_gui_list_item_widget.hpp"

#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"

c_gui_list_item_widget::c_gui_list_item_widget() :
	c_gui_widget(_gui_list_item),
	m_definition(),
	m_list_item_index(NONE),
	__unknown11C(false),
	__string11E()
{
	//DECLFUNC(0x00B18260, void, __thiscall, c_gui_list_item_widget*)(this);
}

s_runtime_list_item_widget_definition::s_runtime_list_item_widget_definition()
{
	DECLFUNC(0x00B18350, void, __thiscall, s_runtime_list_item_widget_definition*)(this);
}

//.text:00B183B0 ; public: virtual void* __cdecl c_gui_list_item_widget::`vector deleting destructor'(unsigned int)

bool c_gui_list_item_widget::can_receive_focus()
{
	return INVOKE_CLASS_MEMBER(0x00B183E0, c_gui_list_item_widget, can_receive_focus);
}

c_gui_bitmap_widget* c_gui_list_item_widget::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18420, c_gui_list_item_widget, create_bitmap_widget, definition);

	return c_gui_widget::get_parent_screen()->create_bitmap_widget(definition);
}

c_gui_button_key_widget* c_gui_list_item_widget::create_button_key_widget(const s_button_key_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18430, c_gui_list_item_widget, create_button_key_widget, definition);

	return c_gui_widget::get_parent_screen()->create_button_key_widget(definition);
}

c_gui_group_widget* c_gui_list_item_widget::create_group_widget(const s_group_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18440, c_gui_list_item_widget, create_group_widget, definition);

	return c_gui_widget::get_parent_screen()->create_group_widget(definition);
}

c_gui_list_item_widget* c_gui_list_item_widget::create_list_item_widget(const s_list_item_widget_block* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18450, c_gui_list_item_widget, create_list_item_widget, definition);

	return c_gui_widget::get_parent_screen()->create_list_item_widget(definition);
}

c_gui_list_widget* c_gui_list_item_widget::create_list_widget(const s_list_widget_block* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18460, c_gui_list_item_widget, create_list_widget, definition);

	return c_gui_widget::get_parent_screen()->create_list_widget(definition);
}

c_gui_model_widget* c_gui_list_item_widget::create_model_widget(const s_model_widget_block* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18470, c_gui_list_item_widget, create_model_widget, definition);

	return c_gui_widget::get_parent_screen()->create_model_widget(definition);
}

c_gui_text_widget* c_gui_list_item_widget::create_text_widget(const s_runtime_text_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B18480, c_gui_list_item_widget, create_text_widget, definition);

	return c_gui_widget::get_parent_screen()->create_text_widget(definition);
}

s_runtime_core_widget_definition* c_gui_list_item_widget::get_core_definition()
{
	//return INVOKE_CLASS_MEMBER(0x00B18490, c_gui_list_item_widget, get_core_definition);

	return &m_definition;
}

int32 c_gui_list_item_widget::get_element_handle()
{
	//return INVOKE_CLASS_MEMBER(0x00B184A0, c_gui_list_item_widget, get_element_handle);

	return c_gui_widget::get_parent_list()->get_element_handle_from_list_item_index(m_list_item_index);
}

int32 c_gui_list_item_widget::get_item_label()
{
	//return INVOKE_CLASS_MEMBER(0x00B184C0, c_gui_list_item_widget, get_item_label);

	return m_definition.item_text_identifier.get_value();
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

void c_gui_list_item_widget::initialize(const s_list_item_widget_block* item_block)
{
	//INVOKE_CLASS_MEMBER(0x00B18640, c_gui_list_item_widget, initialize, item_block);

	if (item_block)
	{
		real_rectangle2d positioning_bounds{};
		get_parent()->get_authored_bounds(&positioning_bounds);
		c_gui_widget::add_definition_fields(m_type, item_block, &m_definition, &positioning_bounds, false);
		m_name = m_definition.widget_identifier;
		m_definition.item_text_identifier = item_block->item_text_identifier;
	}

	c_gui_widget::initialize();
}

bool c_gui_list_item_widget::invokes_submenu(int32* submenu_name)
{
	return INVOKE_CLASS_MEMBER(0x00B186C0, c_gui_list_item_widget, invokes_submenu, submenu_name);
}

//.text:00B18760 ; 
//.text:00B18770 ; 
//.text:00B18780 ; 
//.text:00B187A0 ; 

void c_gui_list_item_widget::set_list_item_index(int32 list_item_index)
{
	INVOKE_CLASS_MEMBER(0x00B187D0, c_gui_list_item_widget, set_list_item_index, list_item_index);
}

bool c_gui_list_item_widget::should_render(bool* add_to_render_list)
{
	return INVOKE_CLASS_MEMBER(0x00B187E0, c_gui_list_item_widget, should_render, add_to_render_list);

	//return c_gui_widget::should_render(add_to_render_list) & (get_element_handle() != NONE);
}

