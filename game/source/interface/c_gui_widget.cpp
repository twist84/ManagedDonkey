#include "interface/c_gui_widget.hpp"

#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_model_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/interface_constants.hpp"
#include "interface/user_interface_data.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "text/draw_string.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AB81A0, c_gui_widget, create_bitmap_widget_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB97C0, c_gui_widget, get_unprojected_bounds);
HOOK_DECLARE_CLASS_MEMBER(0x00AB9980, c_gui_widget, handle_alt_stick_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB99E0, c_gui_widget, handle_alt_tab_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB9A40, c_gui_widget, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB9B40, c_gui_widget, handle_tab_);

c_gui_bitmap_widget* __thiscall c_gui_widget::create_bitmap_widget_(s_runtime_bitmap_widget_definition const* definition)
{
	return c_gui_widget::create_bitmap_widget(definition);
}

bool __thiscall c_gui_widget::handle_alt_stick_(c_controller_input_message const* message)
{
	return c_gui_widget::handle_alt_stick(message);
}

bool __thiscall c_gui_widget::handle_alt_tab_(c_controller_input_message const* message)
{
	return c_gui_widget::handle_alt_tab(message);
}

bool __thiscall c_gui_widget::handle_controller_input_message_(c_controller_input_message const* message)
{
	return c_gui_widget::handle_controller_input_message(message);
}

bool __thiscall c_gui_widget::handle_tab_(c_controller_input_message const* message)
{
	return c_gui_widget::handle_tab(message);
}

c_gui_widget::c_gui_widget(e_gui_widget_type type) :
	__unknown4(0),
	m_type(type),
	m_visible(true),
	m_enabled(true),
	m_needs_disposal(false),
	m_use_alternate_ambient_state(false),
	m_parent(NULL),
	m_children(NULL),
	m_next(NULL),
	m_previous(NULL),
	m_name(NULL),
	__unknown44(0),
	m_flags(0),
	__unknown4C(0),
	m_animated_state(),
	m_last_animated_milliseconds(0),
	__unknownDC(0)
{
	//DECLFUNC(0x00AB5BC0, void, __thiscall, c_gui_widget*, e_gui_widget_type)(this, type);

	ASSERT(is_main_thread());
}

//.text:00AB63A0 ; public: virtual void* c_gui_widget::`scalar deleting destructor'(unsigned int)
c_gui_widget::~c_gui_widget()
{
	//DECLFUNC(0x00AB63A0, void, __thiscall, c_gui_widget*)(this);

	delete_all_children();
}

//.text:00AB63D0 ; public: void c_gui_widget::add_child_widget(c_gui_widget*)
//.text:00AB64B0 ; public: static void __cdecl c_gui_widget::add_definition_fields(e_gui_widget_type, s_core_widget_definition const*, s_runtime_core_widget_definition*, real_rectangle2d const*, bool)
//.text:00AB6680 ; private: void c_gui_widget::animate(uns32)
//.text:00AB6C20 ; protected: void c_gui_widget::animate_recursively(uns32)
//.text:00AB6C90 ; void __cdecl apply_color_animation(s_animation_transform*, real32 const, bool const, s_color_keyframe_block const*, s_color_keyframe_block const*, c_function_definition const*)
//.text:00AB6D40 ; void __cdecl apply_font_animation(s_animation_transform*, real32 const, bool const, s_font_keyframe_block const*, s_font_keyframe_block const*, c_function_definition const*)
//.text:00AB6D90 ; void __cdecl apply_position_animation(s_animation_transform*, real32 const, bool const, real_vector2d const*, s_position_keyframe_block const*, s_position_keyframe_block const*, c_function_definition const*)
//.text:00AB6E80 ; 
//.text:00AB6ED0 ; void __cdecl apply_rotation_animation(c_gui_widget*, s_animation_transform*, real32 const, bool const, s_rotation_keyframe_block const*, s_rotation_keyframe_block const*, c_function_definition const*);
//.text:00AB7050 ; void __cdecl apply_scale_animation(c_gui_widget*, s_animation_transform*, real32 const, bool const, s_scale_keyframe_block const*, s_scale_keyframe_block const*, c_function_definition const*)
//.text:00AB71C0 ; void __cdecl apply_sprite_animation(s_animation_transform*, real32 const, bool const, s_sprite_keyframe_block const*, s_sprite_keyframe_block const*, c_function_definition const*)
//.text:00AB7290 ; void __cdecl apply_texture_coordinate_animation(s_animation_transform*, real32 const, bool const, s_texture_coordinate_keyframe_block const*, s_texture_coordinate_keyframe_block const*, c_function_definition const*)

void c_gui_widget::assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00AB7330, c_gui_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	ASSERT(render_data != NULL);
	ASSERT(window_bounds != NULL);

	render_data->type = m_type;

	real_rectangle2d current_bounds{};
	get_current_bounds(&current_bounds);
	get_projected_bounds(window_bounds, &render_data->projected_bounds, apply_translation, apply_scale, apply_rotation);

	render_data->local_controller_index = local_controller_index;
	render_data->flags.clear();
	render_data->flags.set(s_gui_widget_render_data::_render_in_screenshot_bit, get_render_in_screenshot());

	//render_data->name = m_name.get_value();
	//real_argb_color const* debug_color = get_debug_color();
	//render_data->debug_color = real_argb_color_to_pixel32(debug_color);
	//render_data->animation_state_flags = m_animated_state.state_flags;
}

void c_gui_widget::calculate_animation_transform(e_animation_state animation_state, int32 start_time_milliseconds, int32 current_time_milliseconds, s_widget_animation_definition const* animation, s_animation_transform* transform, real_vector2d const* aspect_ratio_scale, bool initialize, bool combinative, bool* finished)
{
	INVOKE_CLASS_MEMBER(0x00AB73B0, c_gui_widget, calculate_animation_transform, animation_state, start_time_milliseconds, current_time_milliseconds, animation, transform, aspect_ratio_scale, initialize, combinative, finished);
}

bool const c_gui_widget::can_all_children_be_disposed()
{
	return INVOKE_CLASS_MEMBER(0x00AB7850, c_gui_widget, can_all_children_be_disposed);
}

bool const c_gui_widget::can_be_disposed()
{
	return INVOKE_CLASS_MEMBER(0x00AB78C0, c_gui_widget, can_be_disposed);
}

bool c_gui_widget::can_receive_focus()
{
	//return INVOKE_CLASS_MEMBER(0x00AB7930, c_gui_widget, can_receive_focus);

	return false;
}

bool c_gui_widget::controller_can_drive(e_controller_index controller_index)
{
	return INVOKE_CLASS_MEMBER(0x00AB7960, c_gui_widget, controller_can_drive, controller_index);
}

//.text:00AB79D0 ; public: c_gui_button_key_widget* c_gui_widget::create_and_add_button_key(s_button_key_block const*)
//.text:00AB7A30 ; public: c_gui_bitmap_widget* c_gui_widget::create_and_add_child_bitmap_widget(s_bitmap_widget_block const*)
//.text:00AB7AC0 ; public: void c_gui_widget::create_and_add_child_list_item_widgets(s_tag_block const*, int32)
//.text:00AB7B80 ; public: c_gui_list_widget* c_gui_widget::create_and_add_child_list_widget(s_list_widget_block const*)
//.text:00AB7BC0 ; public: c_gui_model_widget* c_gui_widget::create_and_add_child_model_widget(s_model_widget_block const*)
//.text:00AB7C00 ; public: c_gui_text_widget* c_gui_widget::create_and_add_child_text_widget(s_text_widget_block const*)
//.text:00AB7C90 ; public: void c_gui_widget::create_and_add_children_from_skin_definition(s_gui_skin_definition const*, int32, int16)
//.text:00AB8160 ; public: c_gui_group_widget* c_gui_widget::create_and_add_group(s_group_widget_block const*)

c_gui_bitmap_widget* c_gui_widget::create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AB81A0, c_gui_widget, create_bitmap_widget, definition);

	c_gui_bitmap_widget* bitmap_widget = new c_gui_bitmap_widget();
	if (!bitmap_widget)
	{
		return NULL;
	}
	return bitmap_widget;
}

c_gui_button_key_widget* c_gui_widget::create_button_key_widget(s_button_key_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8200, c_gui_widget, create_button_key_widget, definition);

	//c_gui_button_key_widget* button_key_widget = new c_gui_button_key_widget();
	//if (!button_key_widget)
	//{
	//	return NULL;
	//}
	//return button_key_widget;
}

c_gui_group_widget* c_gui_widget::create_group_widget(s_group_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8260, c_gui_widget, create_group_widget, definition);

	//c_gui_group_widget* group_widget = new c_gui_group_widget();
	//if (!group_widget)
	//{
	//	return NULL;
	//}
	//return group_widget;
}

c_gui_list_item_widget* c_gui_widget::create_list_item_widget(s_list_item_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB82C0, c_gui_widget, create_list_item_widget, definition);

	//c_gui_list_item_widget* list_item_widget = new c_gui_list_item_widget();
	//if (!list_item_widget)
	//{
	//	return NULL;
	//}
	//return list_item_widget;
}

c_gui_list_widget* c_gui_widget::create_list_widget(s_list_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8320, c_gui_widget, create_list_widget, definition);

	//c_gui_list_widget* list_widget = new c_gui_list_widget();
	//if (!list_widget)
	//{
	//	return NULL;
	//}
	//return list_widget;
}

c_gui_model_widget* c_gui_widget::create_model_widget(s_model_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8380, c_gui_widget, create_model_widget, definition);

	//c_gui_model_widget* model_widget = new c_gui_model_widget();
	//if (!model_widget)
	//{
	//	return NULL;
	//}
	//return model_widget;
}

c_gui_text_widget* c_gui_widget::create_text_widget(s_runtime_text_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB83E0, c_gui_widget, create_text_widget, definition);

	//c_gui_text_widget* result = NULL;
	//if (TEST_BIT(definition->flags, 14))
	//{
	//	result = new c_gui_sized_text_widget<1024>();
	//}
	//else if (TEST_BIT(definition->flags, 13))
	//{
	//	result = new c_gui_sized_text_widget<256>();
	//}
	//else
	//{
	//	result = new c_gui_sized_text_widget<48>();
	//}
	//
	//return result;
}

void c_gui_widget::delete_all_children()
{
	INVOKE_CLASS_MEMBER(0x00AB8590, c_gui_widget, delete_all_children);
}

void c_gui_widget::dispose()
{
	INVOKE_CLASS_MEMBER(0x00AB8620, c_gui_widget, dispose);
}

//.text:00AB8710 ; 

e_animation_state c_gui_widget::get_ambient_state()
{
	return INVOKE_CLASS_MEMBER(0x00AB8720, c_gui_widget, get_ambient_state);
}

//.text:00AB8810 ; 
//.text:00AB8820 ; private: static int32 __cdecl c_gui_widget::get_animation_period(s_widget_component_animation_definition const*, int32)
//.text:00AB8850 ; private: uns32 c_gui_widget::get_animation_state_timer(e_animation_state)

e_controller_index c_gui_widget::get_arbitrary_responding_controller() const
{
	return INVOKE_CLASS_MEMBER(0x00AB8900, c_gui_widget, get_arbitrary_responding_controller);
}

real_rectangle2d* c_gui_widget::get_authored_bounds(real_rectangle2d* unanimated_bounds)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A10, c_gui_widget, get_authored_bounds, unanimated_bounds);

	ASSERT(unanimated_bounds != NULL);

	s_runtime_core_widget_definition* definition = get_core_definition();
	ASSERT(definition != NULL);

	*unanimated_bounds = definition->authored_bounds;
	return unanimated_bounds;
}

c_gui_bitmap_widget* c_gui_widget::get_child_bitmap_widget(int32 name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A40, c_gui_widget, get_child_bitmap_widget, name);

	return (c_gui_bitmap_widget*)get_child_widget(_gui_bitmap, name);
}

c_gui_group_widget* c_gui_widget::get_child_group_widget(int32 name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A60, c_gui_widget, get_child_group_widget, name);

	return (c_gui_group_widget*)get_child_widget(_gui_group, name);
}

c_gui_list_item_widget* c_gui_widget::get_child_list_item_widget(int32 name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A80, c_gui_widget, get_child_list_item_widget, name);

	return (c_gui_list_item_widget*)get_child_widget(_gui_list_item, name);
}

c_gui_list_widget* c_gui_widget::get_child_list_widget(int32 name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8AA0, c_gui_widget, get_child_list_widget, name);

	return (c_gui_list_widget*)get_child_widget(_gui_list, name);
}

c_gui_model_widget* c_gui_widget::get_child_model_widget(int32 name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8AC0, c_gui_widget, get_child_model_widget, name);

	return (c_gui_model_widget*)get_child_widget(_gui_model, name);
}

c_gui_text_widget* c_gui_widget::get_child_text_widget(int32 name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8AE0, c_gui_widget, get_child_text_widget, name);

	return (c_gui_text_widget*)get_child_widget(_gui_text, name);
}

c_gui_widget* c_gui_widget::get_child_widget(e_gui_widget_type type, int32 name)
{
	return INVOKE_CLASS_MEMBER(0x00AB8B00, c_gui_widget, get_child_widget, type, name);
}

real_rectangle2d* c_gui_widget::get_container_current_bounds(real_rectangle2d* unanimated_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00AB8BC0, c_gui_widget, get_container_current_bounds, unanimated_bounds);
}

int32 c_gui_widget::get_controller_mask() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB8CA0, c_gui_widget, get_controller_mask);

	return m_flags & MASK(k_number_of_controllers);
}

real_argb_color* c_gui_widget::get_cumulative_color_tint(real_argb_color* tint)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8CB0, c_gui_widget, get_cumulative_color_tint, tint);

	ASSERT(tint != NULL);

	*tint = m_animated_state.color;
	return tint;
}

real_rectangle2d* c_gui_widget::get_current_bounds(real_rectangle2d* unanimated_bounds)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8CE0, c_gui_widget, get_current_bounds, unanimated_bounds);

	ASSERT(unanimated_bounds != NULL);

	s_runtime_core_widget_definition* definition = get_core_definition();
	ASSERT(definition != NULL);

	*unanimated_bounds = definition->runtime_bounds;
	return unanimated_bounds;
}

int32 c_gui_widget::get_datasource_index()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8D10, c_gui_widget, get_datasource_index);

	c_gui_list_item_widget* parent_list_item = get_parent_list_item();
	if (!parent_list_item)
	{
		return NONE;
	}

	return parent_list_item->get_datasource_index();
}

real_argb_color const* c_gui_widget::get_debug_color()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8D30, c_gui_widget, get_debug_color);

	real_argb_color const* result = global_real_argb_grey;
	switch (m_type)
	{
	case _gui_text:
	{
		result = global_real_argb_white;
	}
	break;
	case _gui_bitmap:
	{
		result = global_real_argb_red;
	}
	break;
	case _gui_model:
	{
		result = global_real_argb_green;
	}
	break;
	case _gui_group:
	case _gui_button_key:
	{
		result = global_real_argb_blue;
	}
	break;
	case _gui_list_item:
	{
		result = global_real_argb_cyan;
	}
	break;
	case _gui_slider:
	{
		result = global_real_argb_yellow;
	}
	break;
	case _gui_list:
	{
		result = global_real_argb_magenta;
	}
	break;
	case _gui_screen:
	{
		result = global_real_argb_orange;
	}
	break;
	}
	return result;
}

c_gui_widget* c_gui_widget::get_deepest_widget_that_can_receive_focus()
{
	return INVOKE_CLASS_MEMBER(0x00AB8DA0, c_gui_widget, get_deepest_widget_that_can_receive_focus);
}

int32 c_gui_widget::get_definition_index()
{
	return INVOKE_CLASS_MEMBER(0x00AB8E20, c_gui_widget, get_definition_index);
}

e_controller_index c_gui_widget::get_driving_controller() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB8EB0, c_gui_widget, get_driving_controller);

	int32 mask_bits = count_bits(get_controller_mask());
	ASSERT(mask_bits == 0 || mask_bits == 1 || mask_bits == 4);
	if (mask_bits == 1)
	{
		return get_single_responding_controller();
	}

	if (mask_bits == 4)
	{
		return k_any_controller;
	}

	return k_no_controller;
}

int32 c_gui_widget::get_element_handle()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8F30, c_gui_widget, get_element_handle);

	c_gui_list_item_widget* parent_list_item = get_parent_list_item();
	if (!parent_list_item)
	{
		return NONE;
	}

	return parent_list_item->get_element_handle();
}

int32 c_gui_widget::get_element_index()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8F50, c_gui_widget, get_element_index);

	c_gui_list_item_widget* parent_list_item = get_parent_list_item();
	if (!parent_list_item)
	{
		return NONE;
	}

	return parent_list_item->get_definition_index();
}

bool c_gui_widget::get_enabled()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8F70, c_gui_widget, get_enabled);

	return m_enabled;
}

c_gui_widget* c_gui_widget::get_first_child_widget_by_type(e_gui_widget_type type)
{
	return INVOKE_CLASS_MEMBER(0x00AB8F80, c_gui_widget, get_first_child_widget_by_type, type);
}

//.text:00AB8FE0 ; 
//.text:00AB8FF0 ; 

c_gui_widget* c_gui_widget::get_last_child_widget_by_type(e_gui_widget_type type)
{
	return INVOKE_CLASS_MEMBER(0x00AB9010, c_gui_widget, get_last_child_widget_by_type, type);
}

//.text:00AB9080 ; void __cdecl get_local_coordinate_system_position_from_rotation_keyframe(c_gui_widget*, s_rotation_keyframe_block const*, real_point2d*)
//.text:00AB90C0 ; void __cdecl get_local_coordinate_system_position_from_scale_keyframe(c_gui_widget*, s_scale_keyframe_block const*, real_point2d*)
//.text:00AB9100 ; void __cdecl get_local_widget_coordinate_system_position_from_positioning(c_gui_widget*, e_widget_anchoring, real_point2d*)

c_gui_bitmap_widget* c_gui_widget::get_next_bitmap_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9220, c_gui_widget, get_next_bitmap_widget);

	return (c_gui_bitmap_widget*)get_next_widget_of_type(_gui_bitmap);
}

c_gui_list_item_widget* c_gui_widget::get_next_list_item_widget(bool only_consider_valid_items)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9230, c_gui_widget, get_next_list_item_widget, only_consider_valid_items);

	c_gui_list_item_widget* result = (c_gui_list_item_widget*)c_gui_widget::get_next_widget_of_type(_gui_list_item);
	if (result && only_consider_valid_items && result->get_element_handle() == NONE)
	{
		return NULL;
	}

	return result;
}

c_gui_list_widget* c_gui_widget::get_next_list_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9260, c_gui_widget, get_next_list_widget);

	return (c_gui_list_widget*)get_next_widget_of_type(_gui_list);
}

c_gui_model_widget* c_gui_widget::get_next_model_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9270, c_gui_widget, get_next_model_widget);

	return (c_gui_model_widget*)get_next_widget_of_type(_gui_model);
}

c_gui_text_widget* c_gui_widget::get_next_text_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9280, c_gui_widget, get_next_text_widget);

	return (c_gui_text_widget*)get_next_widget_of_type(_gui_model);
}

c_gui_widget* c_gui_widget::get_next_widget_of_type(e_gui_widget_type type)
{
	return INVOKE_CLASS_MEMBER(0x00AB9290, c_gui_widget, get_next_widget_of_type, type);
}

c_gui_group_widget* c_gui_widget::get_parent_group()
{
	return INVOKE_CLASS_MEMBER(0x00AB92F0, c_gui_widget, get_parent_group);
}

c_gui_list_widget* c_gui_widget::get_parent_list()
{
	return INVOKE_CLASS_MEMBER(0x00AB9360, c_gui_widget, get_parent_list);
}

c_gui_list_item_widget* c_gui_widget::get_parent_list_item()
{
	return INVOKE_CLASS_MEMBER(0x00AB93D0, c_gui_widget, get_parent_list_item);
}

c_gui_screen_widget* c_gui_widget::get_parent_screen()
{
	return INVOKE_CLASS_MEMBER(0x00AB9440, c_gui_widget, get_parent_screen);
}

int32 c_gui_widget::get_name() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB94B0, c_gui_widget, get_name);

	return m_name.get_value();
}

c_gui_widget* c_gui_widget::get_previous()
{
	//return INVOKE_CLASS_MEMBER(0x00AB94D0, c_gui_widget, get_previous);

	return m_previous.get_value();
}

c_gui_bitmap_widget* c_gui_widget::get_previous_bitmap_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9500, c_gui_widget, get_previous_bitmap_widget);

	return (c_gui_bitmap_widget*)c_gui_widget::get_previous_widget_of_type(_gui_bitmap);
}

c_gui_list_item_widget* c_gui_widget::get_previous_list_item_widget(bool only_consider_valid_items)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9510, c_gui_widget, get_previous_list_item_widget, only_consider_valid_items);

	c_gui_list_item_widget* result = (c_gui_list_item_widget*)c_gui_widget::get_previous_widget_of_type(_gui_list_item);
	if (result && only_consider_valid_items && result->get_element_handle() == NONE)
	{
		return NULL;
	}

	return result;
}

c_gui_list_widget* c_gui_widget::get_previous_list_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9540, c_gui_widget, get_previous_list_widget);

	return (c_gui_list_widget*)c_gui_widget::get_previous_widget_of_type(_gui_list);
}

c_gui_model_widget* c_gui_widget::get_previous_model_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9550, c_gui_widget, get_previous_model_widget);

	return (c_gui_model_widget*)c_gui_widget::get_previous_widget_of_type(_gui_model);
}

c_gui_text_widget* c_gui_widget::get_previous_text_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9560, c_gui_widget, get_previous_text_widget);

	return (c_gui_text_widget*)c_gui_widget::get_previous_widget_of_type(_gui_text);
}

c_gui_widget* c_gui_widget::get_previous_widget_of_type(e_gui_widget_type type)
{
	return INVOKE_CLASS_MEMBER(0x00AB9570, c_gui_widget, get_previous_widget_of_type, type);
}

gui_real_rectangle2d* c_gui_widget::get_projected_bounds(rectangle2d const* window_bounds, gui_real_rectangle2d* projected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//return INVOKE_CLASS_MEMBER(0x00AB95D0, c_gui_widget, get_projected_bounds, window_bounds, projected_bounds, apply_translation, apply_scale, apply_rotation);

	ASSERT(window_bounds != NULL);
	ASSERT(projected_bounds!=NULL);

	c_gui_widget::get_unprojected_bounds(projected_bounds, apply_translation, apply_scale, apply_rotation);
	projected_bounds->apply_projection_transform(m_animated_state.position.z, window_bounds);
	return projected_bounds;
}

//.text:00AB9610 ; public: int16 c_gui_widget::get_render_depth_bias()

bool c_gui_widget::get_render_in_screenshot()
{
	return INVOKE_CLASS_MEMBER(0x00AB9620, c_gui_widget, get_render_in_screenshot);
}

//.text:00AB96B0 ; 

e_controller_index c_gui_widget::get_single_responding_controller() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB96C0, c_gui_widget, get_single_responding_controller);

	return get_arbitrary_responding_controller();
}

//.text:00AB96D0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<256>::get_string()
//.text:00AB96E0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<48>::get_string()
//.text:00AB96F0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<1024>::get_string()

bool c_gui_widget::get_string_by_string_id(int32 string_identifier, c_static_wchar_string<1024>* buffer)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9700, c_gui_widget, get_string_by_string_id, string_identifier, buffer);

	return false;
}

//.text:00AB9710 ; public: virtual uns32 __cdecl c_gui_sized_text_widget<256>::get_text_buffer_size() const
//.text:00AB9720 ; public: virtual uns32 __cdecl c_gui_sized_text_widget<48>::get_text_buffer_size() const
//.text:00AB9730 ; public: virtual uns32 __cdecl c_gui_sized_text_widget<1024>::get_text_buffer_size() const
//.text:00AB9740 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<256>::get_text_internal()
//.text:00AB9750 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<48>::get_text_internal()
//.text:00AB9760 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<1024>::get_text_internal()
//.text:00AB9770 ; 

//.text:00AB97C0 ; public: gui_real_rectangle2d* c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d*, bool, bool, bool)
void __thiscall c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00AB97C0, c_gui_widget, get_unprojected_bounds, unprojected_bounds, apply_translation, apply_scale, apply_rotation);

	real_rectangle2d authored_bounds{};
	get_current_bounds(&authored_bounds);
	unprojected_bounds->set(&authored_bounds);

	if (apply_scale)
	{
		unprojected_bounds->scale_about_local_point(&m_animated_state.local_scale_origin, &m_animated_state.scale);
	}

	if (apply_rotation)
	{
		unprojected_bounds->rotate_about_local_point(&m_animated_state.local_rotation_origin, m_animated_state.sine_rotation_angle, m_animated_state.cosine_rotation_angle);
	}

	if (apply_translation)
	{
		unprojected_bounds->offset(m_animated_state.position.x, m_animated_state.position.y);
	}

	// this is more or less what Halo 3 MCC is doing

	rectangle2d render_window_bounds;
	interface_get_current_display_settings(NULL, NULL, &render_window_bounds, NULL);

	real_vector2d scale{};
	scale.i = (render_window_bounds.x1 - render_window_bounds.x0) / 1152.0f;
	scale.j = (render_window_bounds.y1 - render_window_bounds.y0) / 640.0f;
	unprojected_bounds->scale_direct(&scale);
}

//.text:00AB9870 ; 
//.text:00AB9880 ; 
//.text:00AB9920 ; public: bool c_gui_widget::get_visible() const
//.text:00AB9930 ; public: s_widget_animation_definition const* c_gui_widget::get_widget_animation_definition(e_animation_state)

bool c_gui_widget::handle_alt_stick(c_controller_input_message const* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9980, c_gui_widget, handle_alt_stick, message);

	if (!get_parent())
	{
		return false;
	}

	return get_parent()->handle_alt_stick(message);
}

bool c_gui_widget::handle_alt_tab(c_controller_input_message const* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB99E0, c_gui_widget, handle_alt_tab, message);

	if (!get_parent())
	{
		return false;
	}

	return get_parent()->handle_alt_tab(message);
}

bool c_gui_widget::handle_controller_input_message(c_controller_input_message const* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9A40, c_gui_widget, handle_controller_input_message, message);

	if (!get_parent())
	{
		return false;
	}

	return get_parent()->handle_controller_input_message(message);
}

//.text:00AB9AA0 ; c_gui_widget::handle_?

bool c_gui_widget::handle_tab(c_controller_input_message const* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9B40, c_gui_widget, handle_tab, message);

	if (!get_parent())
	{
		return false;
	}

	return get_parent()->handle_tab(message);
}

bool c_gui_widget::handle_widget_back_out()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9BA0, c_gui_widget, handle_widget_back_out);

	return false;
}

bool c_gui_widget::handle_widget_selected()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9BB0, c_gui_widget, handle_widget_selected);

	return false;
}

//.text:00AB9BC0 ; 

void c_gui_widget::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AB9BD0, c_gui_widget, initialize);

	real_rectangle2d widget_bounds{};

	SET_BIT(m_flags, 4, true);
	get_current_bounds(&widget_bounds);
	get_core_definition()->runtime_bounds = widget_bounds;
}

bool c_gui_widget::is_animation_active(e_animation_state animation_state)
{
	return INVOKE_CLASS_MEMBER(0x00AB9C10, c_gui_widget, is_animation_active, animation_state);
}

//.text:00AB9C50 ; 
//.text:00AB9C70 ; 
//.text:00AB9CB0 ; 

bool c_gui_widget::leaf_node_of_widget(c_gui_widget* branch_widget)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9CC0, c_gui_widget, leaf_node_of_widget, branch_widget);

	ASSERT(branch_widget != NULL);

	if (branch_widget == this)
	{
		return true;
	}

	c_gui_widget* parent_widget = get_parent();
	if (!parent_widget)
	{
		return false;
	}

	while (parent_widget != branch_widget)
	{
		parent_widget = parent_widget->get_parent();
		if (!parent_widget)
		{
			return false;
		}
	}

	return true;
}

//.text:00AB9D40 ; 
//.text:00AB9D50 ; 
//.text:00AB9D60 ; 

void c_gui_widget::modulate_tint_color(real_argb_color const* modulation)
{
	//INVOKE_CLASS_MEMBER(0x00AB9D70, c_gui_widget, modulate_tint_color, modulation);

	m_animated_state.color.alpha *= modulation->alpha;
	m_animated_state.color.red *= modulation->red;
	m_animated_state.color.green *= modulation->green;
	m_animated_state.color.blue *= modulation->blue;
}

void c_gui_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AB9DD0, c_gui_widget, post_initialize);

	for (c_gui_widget* child_widget = get_children(); child_widget; child_widget = child_widget->get_next())
	{
		if (child_widget->m_type == _gui_screen)
		{
			continue;
		}

		child_widget->post_initialize();
	}

	SET_BIT(m_flags, 4, false);
}

void c_gui_widget::remove_child_widget(c_gui_widget* child)
{
	INVOKE_CLASS_MEMBER(0x00AB9E40, c_gui_widget, remove_child_widget, child);
}

void c_gui_widget::render(int32 user_index, s_gui_widget_render_data const* render_data, rectangle2d const* window_bounds, bool is_screenshot)
{
	//INVOKE(0x00AB9F40, c_gui_widget::render, user_index, render_data, window_bounds, is_screenshot);

	ASSERT(render_data != NULL);
	ASSERT(window_bounds != NULL);

	//c_rasterizer_profile_scope _render(, L"");

	if (!is_screenshot || render_data->flags.test(s_gui_widget_render_data::_render_in_screenshot_bit))
	{
		if (render_data->type == _gui_text)
		{
			c_user_interface_text::render((s_user_interface_text_render_data*)render_data, window_bounds);
		}
		else if (render_data->type == _gui_bitmap)
		{
			render_bitmap((s_gui_bitmap_widget_render_data const*)render_data, window_bounds);
		}
		else if (render_data->type == _gui_model)
		{
			c_gui_model_widget::render((s_gui_model_widget_render_data const*)render_data, window_bounds);
		}
	}

	//if (render_data->render_debug_name)
	//{
	//	c_simple_font_draw_string draw_string;
	//	real_rectangle2d text_bounds
	//	{
	//		.x0 = render_data->projected_bounds.vertex[0].x,
	//		.x1 = render_data->projected_bounds.vertex[0].y,
	//		.y0 = (real32)window_bounds->y0,
	//		.y1 = (real32)window_bounds->y1
	//	};
	//	draw_string.set_bounds(&text_bounds);
	//	draw_string.set_color(render_data->debug_color);
	//	draw_string.draw(NULL, string_id_get_string_const(render_data->name));
	//}
	//
	//if (render_data->render_debug_animation_state)
	//{
	//	c_static_string<1024> text;
	//	for (int32 animation_state = 0; animation_state < k_number_of_ui_animation_states; animation_state++)
	//	{
	//		switch (animation_state)
	//		{
	//		// $TODO: add all cases
	//		default:
	//		{
	//			ASSERT2("unreachable");
	//		}
	//		break;
	//		}
	//	}
	//	c_simple_font_draw_string draw_string;
	//	real_rectangle2d text_bounds
	//	{
	//		.x0 = render_data->projected_bounds.vertex[0].x,
	//		.x1 = render_data->projected_bounds.vertex[0].y,
	//		.y0 = (real32)window_bounds->y0,
	//		.y1 = (real32)window_bounds->y1
	//	};
	//	draw_string.set_bounds(&text_bounds);
	//	draw_string.set_color(render_data->debug_color);
	//	draw_string.draw(NULL, text.get_string());
	//}
	//
	//if (render_data->render_debug_bounds)
	//{
	//	point2d points[5]{};
	//
	//	points[0].x = (int16)render_data->projected_bounds.vertex[0].x;
	//	points[0].y = (int16)render_data->projected_bounds.vertex[0].y;
	//	points[1].x = (int16)render_data->projected_bounds.vertex[2].x;
	//	points[1].y = (int16)render_data->projected_bounds.vertex[2].y;
	//	points[2].x = (int16)render_data->projected_bounds.vertex[3].x;
	//	points[2].y = (int16)render_data->projected_bounds.vertex[3].y;
	//	points[3].x = (int16)render_data->projected_bounds.vertex[1].x;
	//	points[3].y = (int16)render_data->projected_bounds.vertex[1].y;
	//	points[4].x = (int16)render_data->projected_bounds.vertex[0].x;
	//	points[4].y = (int16)render_data->projected_bounds.vertex[0].y;
	//
	//	real_vector2d aspect_ratio_scale = interface_get_aspect_ratio_scaling();
	//	interface_scale_points_for_xenon_scaler(points, NUMBEROF(points), &aspect_ratio_scale);
	//	c_rasterizer::draw_debug_linestrip2d(points, NUMBEROF(points), render_data->debug_color);
	//}
	//
	//if (render_data->render_debug_rotation_origin)
	//{
	//	c_simple_font_draw_string draw_string;
	//	real_rectangle2d text_bounds
	//	{
	//		.x0 = render_data->rotation_origin_with_depth.x - 8.0,
	//		.x1 = render_data->rotation_origin_with_depth.y - 8.0,
	//		.y0 = (real32)window_bounds->y0,
	//		.y1 = (real32)window_bounds->y1
	//	};
	//	draw_string.set_bounds(&text_bounds);
	//	draw_string.set_color(render_data->debug_color);
	//	draw_string.draw(NULL, "(+)");
	//}
}

//.text:00AB9F90 ; 
//.text:00AB9FA0 ; 

void c_gui_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	//INVOKE_CLASS_MEMBER(0x00AB9FD0, c_gui_widget, set_animated_state_baseline, transform);

	transform->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	transform->position = { 0.0f, 0.0f, 0.0f };
	transform->scale = { 1.0f, 1.0f };
	transform->local_scale_origin = { 0.0f, 0.0f };
	transform->rotation_angle_radians = 0.0f;
	transform->sine_rotation_angle = 0.0f;
	transform->cosine_rotation_angle = 1.0f;
	transform->local_rotation_origin = { 0.0f, 0.0f };
	transform->texture_uv = { 0.0f, 0.0f };
	transform->bitmap_sprite_sequence = 0;
	transform->bitmap_sprite_frame = 0;
	transform->font = _terminal_font;
}

//.text:00ABA070 ; 
//.text:00ABA120 ; 

void c_gui_widget::set_child_bitmap_sprite_frame(int32 widget_name, int32 sprite_frame_index)
{
	//INVOKE_CLASS_MEMBER(0x00ABA1E0, c_gui_widget, set_child_bitmap_sprite_frame, widget_name, sprite_frame_index);

	if (c_gui_bitmap_widget* child_widget = c_gui_widget::get_child_bitmap_widget(widget_name))
	{
		child_widget->set_sprite_sequence(sprite_frame_index);
	}
}

void c_gui_widget::set_child_enabled(e_gui_widget_type widget_type, int32 widget_name, bool enabled)
{
	//INVOKE_CLASS_MEMBER(0x00ABA200, c_gui_widget, set_child_enabled, widget_type, widget_name, enabled);

	if (c_gui_widget* child_widget = c_gui_widget::get_child_widget(widget_type, widget_name))
	{
		child_widget->set_enabled(enabled);
	}
}

void c_gui_widget::set_child_use_alternate_ambient_state(e_gui_widget_type widget_type, int32 widget_name, bool value)
{
	//INVOKE_CLASS_MEMBER(0x00ABA220, c_gui_widget, set_child_use_alternate_ambient_state, widget_type, widget_name, value);

	if (c_gui_widget* child_widget = c_gui_widget::get_child_widget(widget_type, widget_name))
	{
		child_widget->set_use_alternate_ambient_state(value);
	}
}

void c_gui_widget::set_child_visible(e_gui_widget_type widget_type, int32 widget_name, bool visible)
{
	//INVOKE_CLASS_MEMBER(0x00ABA240, c_gui_widget, set_child_visible, widget_type, widget_name, visible);

	if (c_gui_widget* child_widget = c_gui_widget::get_child_widget(widget_type, widget_name))
	{
		child_widget->set_visible(visible);
	}
}

void c_gui_widget::set_children(c_gui_widget* children)
{
	//INVOKE_CLASS_MEMBER(0x00ABA260, c_gui_widget, set_children, children);

	m_children.set_value(children);
}

//.text:00ABA270 ; 

void c_gui_widget::set_driving_controller(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00ABA280, c_gui_widget, set_driving_controller, controller_index);
}

void c_gui_widget::set_enabled(bool value)
{
	INVOKE_CLASS_MEMBER(0x00ABA2D0, c_gui_widget, set_enabled, value);
}

void c_gui_widget::set_full_animation_state(s_animation_transform const* transform, bool recursive)
{
	INVOKE_CLASS_MEMBER(0x00ABA340, c_gui_widget, set_full_animation_state, transform, recursive);
}

void c_gui_widget::set_next(c_gui_widget* next)
{
	//INVOKE_CLASS_MEMBER(0x00ABA3C0, c_gui_widget, set_next, next);

	m_next.set_value(next);
}

void c_gui_widget::set_parent(c_gui_widget* parent)
{
	//INVOKE_CLASS_MEMBER(0x00ABA3D0, c_gui_widget, set_parent, parent);

	m_parent.set_value(parent);
}

void c_gui_widget::set_previous(c_gui_widget* previous)
{
	//INVOKE_CLASS_MEMBER(0x00ABA3E0, c_gui_widget, set_previous, previous);

	m_previous.set_value(previous);
}

//.text:00ABA3F0 ; 
//.text:00ABA420 ; 
//.text:00ABA450 ; public: virtual void __cdecl c_sized_user_interface_text<256>::set_string(wchar_t const*, bool, int32)
//.text:00ABA550 ; public: virtual void __cdecl c_sized_user_interface_text<48>::set_string(wchar_t const*, bool, int32)
//.text:00ABA630 ; public: virtual void __cdecl c_sized_user_interface_text<1024>::set_string(wchar_t const*, bool, int32)

void c_gui_widget::set_tint_color_direct(real_argb_color const* color)
{
	//INVOKE_CLASS_MEMBER(0x00ABA730, c_gui_widget, set_tint_color_direct, color);

	ASSERT(color != NULL);
	m_animated_state.color = *color;
}

void c_gui_widget::set_use_alternate_ambient_state(bool value)
{
	INVOKE_CLASS_MEMBER(0x00ABA760, c_gui_widget, set_use_alternate_ambient_state, value);
}

//.text:00ABA7D0 ; public: void c_robust_pointer<c_gui_widget>::set_value(c_gui_widget* value)

void c_gui_widget::set_visible(bool value)
{
	INVOKE_CLASS_MEMBER(0x00ABA7E0, c_gui_widget, set_visible, value);
}

bool c_gui_widget::should_render(bool* add_to_render_list)
{
	//return INVOKE_CLASS_MEMBER(0x00ABA850, c_gui_widget, should_render, add_to_render_list);

	if (add_to_render_list)
	{
		*add_to_render_list = m_type != _gui_text || m_type == _gui_bitmap || m_type == _gui_model;
	}
	return m_visible;
}

void c_gui_widget::start_animation(e_animation_state animation_state, bool recursive)
{
	//INVOKE_CLASS_MEMBER(0x00ABA890, c_gui_widget, start_animation, animation_state, recursive);

	c_gui_widget::start_animation_at_time(animation_state, -1, recursive);
}

void c_gui_widget::start_animation_at_time(e_animation_state animation_state, uns32 animation_start_time, bool recursive)
{
	INVOKE_CLASS_MEMBER(0x00ABA8B0, c_gui_widget, start_animation_at_time, animation_state, animation_start_time, recursive);
}

void c_gui_widget::stomp_bounds(real_rectangle2d const* new_bounds)
{
	//INVOKE_CLASS_MEMBER(0x00ABAB50, c_gui_widget, stomp_bounds, new_bounds);

	s_runtime_core_widget_definition* definition = get_core_definition();
	definition->runtime_bounds = *new_bounds;
	definition->authored_bounds = *new_bounds;
}

//.text:00ABAB80 ; 
//.text:00ABAC30 ; 
//.text:00ABAC40 ; 

bool c_gui_widget::transitioning_in()
{
	return INVOKE_CLASS_MEMBER(0x00ABAC60, c_gui_widget, transitioning_in);
}

bool c_gui_widget::transitioning_out()
{
	return INVOKE_CLASS_MEMBER(0x00ABACB0, c_gui_widget, transitioning_in);
}

//.text:00ABAD00 ; public: virtual void __cdecl c_sized_user_interface_text<256>::update(int32)
//.text:00ABAE30 ; public: virtual void __cdecl c_sized_user_interface_text<48>::update(int32)
//.text:00ABAF40 ; public: virtual void __cdecl c_sized_user_interface_text<1024>::update(int32)

void c_gui_widget::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00ABB070, c_gui_widget, update, current_milliseconds);

	for (c_gui_widget* child_widget = get_children(); child_widget; child_widget = child_widget->get_next())
	{
		if (child_widget->m_type == _gui_screen)
		{
			continue;
		}

		child_widget->update(current_milliseconds);
	}
}

void c_gui_widget::update_animation(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00ABB0E0, c_gui_widget, update_animation, current_milliseconds);
}

void c_gui_widget::update_render_state(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00ABB1A0, c_gui_widget, update_render_state, current_milliseconds);

	if (!m_visible)
	{
		return;
	}

	for (c_gui_widget* child_widget = get_children(); child_widget; child_widget = child_widget->get_next())
	{
		if (child_widget->m_type == _gui_screen)
		{
			continue;
		}

		child_widget->update_render_state(current_milliseconds);
	}
}

//.text:00ABB210 ; 

bool c_gui_widget::verify_animation_period(e_animation_state animation_state, int32 period)
{
	return INVOKE_CLASS_MEMBER(0x00ABB220, c_gui_widget, verify_animation_period, animation_state, period);
}

//.text:00ABB270 ; public: static char const* __cdecl c_gui_widget::widget_name_to_string(int32)
//.text:00ABB280 ; public: static char const* __cdecl c_gui_widget::widget_type_to_string(e_gui_widget_type)

bool c_gui_widget::within_focus_chain()
{
	return INVOKE_CLASS_MEMBER(0x00ABB300, c_gui_widget, within_focus_chain);

	//if (c_gui_screen_widget* screen = m_type == _gui_screen ? (c_gui_screen_widget*)this : get_parent_screen())
	//{
	//	if (c_gui_widget* focused_widget = screen->get_focused_widget())
	//	{
	//		c_gui_widget* branch_widget = m_type == _gui_list_item || !get_parent_list_item() ? this : get_parent_list_item();
	//		return leaf_node_of_widget(branch_widget);
	//	}
	//}
	//return false;
}

