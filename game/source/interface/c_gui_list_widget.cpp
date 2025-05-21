#include "interface/c_gui_list_widget.hpp"

c_gui_list_widget::c_gui_list_widget() :
	c_gui_widget(_gui_list),
	m_definition(),
	m_datasource_name(NONE),
	m_more_items_preceeding_bitmap(NULL),
	m_more_items_following_bitmap(NULL),
	m_focused_item_index(0),
	m_current_scroll_direction(_list_scroll_direction_none),
	m_scroll_position(0),
	m_selectable_item_cap(NONE),
	m_submenu_item(NULL),
	m_is_submenu(false),
	m_is_submenu_needs_disposal(false)
{
	//DECLFUNC(0x00B14890, void, __thiscall, c_gui_list_widget*)(this);
}

//.text:00B14910 ; public: s_list_widget_block::s_list_widget_block()
//.text:00B14970 ; public: s_list_widget_definition::s_list_widget_definition()
//.text:00B149E0 ; public: s_runtime_list_widget_definition::s_runtime_list_widget_definition()
//.text:00B14A70 ; 

//.text:00B14A80 ; public: virtual void* c_gui_list_widget::`scalar deleting destructor'(unsigned int)
c_gui_list_widget::~c_gui_list_widget()
{
	DECLFUNC(0x00B14A80, void, __thiscall, c_gui_list_widget*)(this);
}

//.text:00B14AB0 ; public: void c_gui_list_widget::add_definition_fields(s_list_widget_definition const*, bool)
//.text:00B14B90 ; public: void c_gui_list_widget::close_active_submenu(c_gui_list_widget*)
//.text:00B14C10 ; public: void c_gui_list_widget::create_and_add_additional_items_indicators_bitmaps()
//.text:00B14CE0 ; public: void c_gui_list_widget::dispose_submenu(c_gui_list_widget*)
//.text:00B14D30 ; public: void c_gui_list_widget::get_child_list_item_text_bounds(c_font_cache_base*, rectangle2d*, bool*)
//.text:00B14E90 ; public: void c_gui_list_widget::get_child_list_item_widget_bounds(real_rectangle2d*)

s_runtime_core_widget_definition* c_gui_list_widget::get_core_definition()
{
	return INVOKE_CLASS_MEMBER(0x00B14FD0, c_gui_list_widget, get_core_definition);
}

c_gui_data* c_gui_list_widget::get_data()
{
	return INVOKE_CLASS_MEMBER(0x00B14FE0, c_gui_list_widget, get_data);
}

int32 c_gui_list_widget::get_datasource_index()
{
	return INVOKE_CLASS_MEMBER(0x00B15020, c_gui_list_widget, get_datasource_index);
}

//.text:00B15060 ; public: int32 c_gui_list_widget::get_element_handle_from_list_item_index(int32)
//.text:00B150E0 ; public: int32 c_gui_list_widget::get_focused_element_handle()

int32 c_gui_list_widget::get_focused_item_index()
{
	return INVOKE_CLASS_MEMBER(0x00B15160, c_gui_list_widget, get_focused_item_index);
}

//.text:00B15170 ; public: int32 c_gui_list_widget::get_item_count(bool)
//.text:00B151C0 ; 

int32 c_gui_list_widget::get_scroll_position()
{
	return INVOKE_CLASS_MEMBER(0x00B15250, c_gui_list_widget, get_scroll_position);
}

int32 c_gui_list_widget::get_selectable_item_count()
{
	return INVOKE_CLASS_MEMBER(0x00B15260, c_gui_list_widget, get_selectable_item_count);
}

//.text:00B152C0 ; 
//.text:00B15390 ; 
//.text:00B15450 ; private: bool c_gui_list_widget::handle_grid_tab(c_gui_list_item_widget*, e_event_type)
bool c_gui_list_widget::handle_tab(c_controller_input_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00B156B0, c_gui_list_widget, handle_tab, message);
}

//.text:00B157F0 ; private: bool c_gui_list_widget::handle_tab_direction(c_gui_list_item_widget*, c_gui_list_widget::e_list_scroll_direction)
//.text:00B15B70 ; private: bool c_gui_list_widget::handle_tab_to_list_item(c_gui_list_item_widget*, c_gui_list_item_widget*)

void c_gui_list_widget::initialize(s_list_widget_block const* template_and_override_block)
{
	INVOKE_CLASS_MEMBER(0x00B15BA0, c_gui_list_widget, initialize, template_and_override_block);
}

//.text:00B15C20 ; public: bool c_gui_list_widget::invoke_submenu(int32, int32, bool)
//.text:00B15CD0 ; public: bool c_gui_list_widget::invoke_submenu(s_list_widget_block const*, int32, bool)
//.text:00B15EA0 ; public: bool c_gui_list_widget::is_submenu_that_needs_disposal() const
//.text:00B15EB0 ; public: bool c_gui_list_widget::list_has_more_elements_following()
//.text:00B15F40 ; public: bool c_gui_list_widget::list_has_more_elements_preceeding()
//.text:00B15FB0 ; 
//.text:00B15FC0 ; public: void c_gui_list_widget::mark_as_submenu(bool)
//.text:00B15FD0 ; private: void c_gui_list_widget::offset_horizontal_list_item_indicators()

void c_gui_list_widget::post_initialize()
{
	INVOKE_CLASS_MEMBER(0x00B16130, c_gui_list_widget, post_initialize);
}

//.text:00B16140 ; 
//.text:00B16150 ; 
//.text:00B16160 ; private: void c_gui_list_widget::set_datasource_name(int32)
//.text:00B16170 ; public: bool c_gui_list_widget::set_focused_datasource_element_from_value(int32, int32, bool)
//.text:00B16230 ; public: bool c_gui_list_widget::set_focused_element_handle(int32, bool)

bool c_gui_list_widget::set_focused_item_index(int32 focused_item_index, bool a2)
{
	return INVOKE_CLASS_MEMBER(0x00B162E0, c_gui_list_widget, set_focused_item_index, focused_item_index, a2);
}

void c_gui_list_widget::set_scroll_position(int32 scroll_position)
{
	INVOKE_CLASS_MEMBER(0x00B16440, c_gui_list_widget, set_scroll_position, scroll_position);
}

//.text:00B16490 ; public: c_gui_list_item_widget* c_gui_list_widget::try_and_get_child_list_item_widget_by_element_handle(int32)
//.text:00B164F0 ; public: c_gui_list_item_widget* c_gui_list_widget::try_and_get_focused_child_list_item_widget()

void c_gui_list_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B16510, c_gui_list_widget, update, current_milliseconds);
}

//.text:00B165D0 ; private: void __cdecl c_gui_list_widget::update_indicator_bitmap_widget_animation_states()

void c_gui_list_widget::update_render_state(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B16650, c_gui_list_widget, update_render_state, current_milliseconds);
}

