#include "interface/c_gui_widget.hpp"

#include "cache/cache_files.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_group_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_model_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/interface_constants.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/old_render_debug.hpp"
#include "text/draw_string.hpp"
#include "user_interface_window_manager.hpp"

constexpr uns64 k_animation_state_ambient = FLAGS_64(
	_focused_ambient,
	_unfocused_ambient,
	_focused_disabled_ambient,
	_unfocused_disabled_ambient,
	_alternate_focused_ambient,
	_alternate_unfocused_ambient,
	_indicator_ambient_additional_items,
	_indicator_ambient_no_additional_items,
	_child_submenu_ambient_focused,
	_child_submenu_ambient_unfocused);
static_assert(0x00300C003Full == k_animation_state_ambient);

constexpr uns64 k_animation_state_screen_transition = FLAGS_64(
	_transition_from_screen,
	_transition_to_screen,
	_transition_back_from_screen,
	_transition_back_to_screen,
	_cycle_in_previous_screen,
	_cycle_in_next_screen,
	_cycle_out_previous_screen,
	_cycle_out_next_screen,
	_custom_screen_transition_in0,
	_custom_screen_transition_out0,
	_custom_screen_transition_in1,
	_custom_screen_transition_out1);
static_assert(0x0F00003FC0ull == k_animation_state_screen_transition);

constexpr uns64 k_animation_state_widget_transition = FLAGS_64(
	_list_display_group_transition_in,
	_list_display_group_transition_out,
	_load_submenu_focused,
	_load_submenu_unfocused,
	_unload_submenu_focused,
	_unload_submenu_unfocused,
	_load_as_submenu,
	_unload_as_submenu,
	_custom_animation0,
	_custom_animation1);
static_assert(0x00CFC0C000ull == k_animation_state_widget_transition);

constexpr uns64 k_animation_state_focus_transfer = FLAGS_64(
	_control_received_focus,
	_control_lost_focus);
static_assert(0x0000030000ull == k_animation_state_focus_transfer);

constexpr uns64 k_animation_state_list_item_indicator_activated = FLAGS_64(
	_indicator_activated_additional_items,
	_indicator_activated_no_additional_items);
static_assert(0x0000300000ull == k_animation_state_list_item_indicator_activated);

//constexpr uns64 k_animation_state_mouse_hover = FLAGS_64(
//	_mouse_enter,
//	_mouse_leave);
//static_assert(0x3000000000ull == k_animation_state_mouse_hover);

HOOK_DECLARE_CLASS_MEMBER(0x00AB81A0, c_gui_widget, create_bitmap_widget_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AB8200, c_gui_widget, create_button_key_widget_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AB8260, c_gui_widget, create_group_widget_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB82C0, c_gui_widget, create_list_item_widget_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB8320, c_gui_widget, create_list_widget_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AB8380, c_gui_widget, create_model_widget_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AB83E0, c_gui_widget, create_text_widget_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB8720, c_gui_widget, get_ambient_state_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AB8B00, c_gui_widget, get_child_widget_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB97C0, c_gui_widget, get_unprojected_bounds_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB9980, c_gui_widget, handle_alt_stick_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB99E0, c_gui_widget, handle_alt_tab_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB9A40, c_gui_widget, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB9B40, c_gui_widget, handle_tab_);

bool g_gui_debug_text_bounds = false;
bool g_gui_debug_bitmap_bounds = false;
bool g_gui_debug_model_bounds = false;
bool g_gui_debug_list_item_bounds = false;
bool g_gui_debug_list_bounds = false;
bool g_gui_debug_group_bounds = false;
bool g_gui_debug_screen_bounds = false;

c_gui_bitmap_widget* __thiscall c_gui_widget::create_bitmap_widget_(const s_runtime_bitmap_widget_definition* definition)
{
	return c_gui_widget::create_bitmap_widget(definition);
}

c_gui_button_key_widget* __thiscall c_gui_widget::create_button_key_widget_(const s_button_key_definition* definition)
{
	return c_gui_widget::create_button_key_widget(definition);
}

c_gui_group_widget* __thiscall c_gui_widget::create_group_widget_(const s_group_widget_definition* definition)
{
	return c_gui_widget::create_group_widget(definition);
}

c_gui_list_item_widget* __thiscall c_gui_widget::create_list_item_widget_(const s_list_item_widget_block* definition)
{
	return c_gui_widget::create_list_item_widget(definition);
}

c_gui_list_widget* __thiscall c_gui_widget::create_list_widget_(const s_list_widget_block* definition)
{
	return c_gui_widget::create_list_widget(definition);
}

c_gui_model_widget* __thiscall c_gui_widget::create_model_widget_(const s_model_widget_block* definition)
{
	return c_gui_widget::create_model_widget(definition);
}

c_gui_text_widget* __thiscall c_gui_widget::create_text_widget_(const s_runtime_text_widget_definition* definition)
{
	return c_gui_widget::create_text_widget(definition);
}

e_animation_state __thiscall c_gui_widget::get_ambient_state_()
{
	return c_gui_widget::get_ambient_state();
}

c_gui_widget* __thiscall c_gui_widget::get_child_widget_(e_gui_widget_type type, int32 name)
{
	return c_gui_widget::get_child_widget(type, name);
}

gui_real_rectangle2d* __thiscall c_gui_widget::get_unprojected_bounds_(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	return c_gui_widget::get_unprojected_bounds(unprojected_bounds, apply_translation, apply_scale, apply_rotation);
}

bool __thiscall c_gui_widget::handle_alt_stick_(const c_controller_input_message* message)
{
	return c_gui_widget::handle_alt_stick(message);
}

bool __thiscall c_gui_widget::handle_alt_tab_(const c_controller_input_message* message)
{
	return c_gui_widget::handle_alt_tab(message);
}

bool __thiscall c_gui_widget::handle_controller_input_message_(const c_controller_input_message* message)
{
	return c_gui_widget::handle_controller_input_message(message);
}

bool __thiscall c_gui_widget::handle_tab_(const c_controller_input_message* message)
{
	return c_gui_widget::handle_tab(message);
}

template<>
void ui_track_delete<c_gui_widget>(const c_gui_widget* object)
{
	ASSERT(object != nullptr);

	object->~c_gui_widget();
	user_interface_free(object);
}

c_gui_widget::c_gui_widget(e_gui_widget_type type) :
	__unknown4(0),
	m_type(type),
	m_visible(true),
	m_enabled(true),
	m_needs_disposal(false),
	m_use_alternate_ambient_state(false),
	m_parent(nullptr),
	m_children(nullptr),
	m_next(nullptr),
	m_previous(nullptr),
	m_name(_string_id_invalid),
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
	//c_window_manager* window_manager = window_manager_get();
	//window_manager->on_widget_delete(this);
}

void c_gui_widget::add_child_widget(c_gui_widget* child)
{
	INVOKE_CLASS_MEMBER(0x00AB63D0, c_gui_widget, add_child_widget, child);
}

void __cdecl c_gui_widget::add_definition_fields(e_gui_widget_type type, const s_core_widget_definition* source_definition, s_runtime_core_widget_definition* dest_definition, const real_rectangle2d* unanimated_bounds, bool was_templated)
{
	return INVOKE(0x00AB64B0, c_gui_widget::add_definition_fields, type, source_definition, dest_definition, unanimated_bounds, was_templated);
}

void c_gui_widget::animate(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AB6680, c_gui_widget, animate, current_milliseconds);
}

void c_gui_widget::animate_recursively(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AB6C20, c_gui_widget, animate_recursively, current_milliseconds);

	animate(current_milliseconds);

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			child->animate_recursively(current_milliseconds);
		}
	}
}

//.text:00AB6C90 ; void __cdecl apply_color_animation(s_animation_transform*, real32 const, bool const, const s_color_keyframe_block*, const s_color_keyframe_block*, const c_function_definition*)
//.text:00AB6D40 ; void __cdecl apply_font_animation(s_animation_transform*, real32 const, bool const, const s_font_keyframe_block*, const s_font_keyframe_block*, const c_function_definition*)
//.text:00AB6D90 ; void __cdecl apply_position_animation(s_animation_transform*, real32 const, bool const, const real_vector2d*, const s_position_keyframe_block*, const s_position_keyframe_block*, const c_function_definition*)
//.text:00AB6E80 ; 
//.text:00AB6ED0 ; void __cdecl apply_rotation_animation(c_gui_widget*, s_animation_transform*, real32 const, bool const, const s_rotation_keyframe_block*, const s_rotation_keyframe_block*, const c_function_definition*);
//.text:00AB7050 ; void __cdecl apply_scale_animation(c_gui_widget*, s_animation_transform*, real32 const, bool const, const s_scale_keyframe_block*, const s_scale_keyframe_block*, const c_function_definition*)
//.text:00AB71C0 ; void __cdecl apply_sprite_animation(s_animation_transform*, real32 const, bool const, const s_sprite_keyframe_block*, const s_sprite_keyframe_block*, const c_function_definition*)
//.text:00AB7290 ; void __cdecl apply_texture_coordinate_animation(s_animation_transform*, real32 const, bool const, const s_texture_coordinate_keyframe_block*, const s_texture_coordinate_keyframe_block*, const c_function_definition*)

void c_gui_widget::assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00AB7330, c_gui_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	ASSERT(render_data != nullptr);
	ASSERT(window_bounds != nullptr);

	render_data->type = get_type();

	real_rectangle2d current_bounds{};
	get_current_bounds(&current_bounds);

	get_projected_bounds(window_bounds, &render_data->projected_bounds, apply_translation, apply_scale, apply_rotation);
	render_data->local_controller_index = local_controller_index;

	render_data->flags.clear();
	render_data->flags.set(s_gui_widget_render_data::_render_in_screenshot_bit, get_render_in_screenshot());

	// >= profile builds
	render_data->name = get_name();

	// >= play builds
	const real_argb_color* debug_color = get_debug_color();
	render_data->debug_color = real_argb_color_to_pixel32(debug_color);

	render_data->animation_state_flags = m_animated_state.state_flags;

	render_data->rotation_origin_with_depth.x = (m_animated_state.position.x + current_bounds.x0) + m_animated_state.local_rotation_origin.x;
	render_data->rotation_origin_with_depth.y = (m_animated_state.position.y + current_bounds.y0) + m_animated_state.local_rotation_origin.y;
	render_data->rotation_origin_with_depth.z = m_animated_state.position.z;
	gui_real_rectangle2d::transform_real_point3d(&render_data->rotation_origin_with_depth, window_bounds);

	render_data->render_debug_name = get_debug_name();
	render_data->render_debug_animation_state = get_debug_animation_state();
	render_data->render_debug_bounds = get_debug_bounds();
	render_data->render_debug_rotation_origin = get_debug_rotation_origin();

	switch (render_data->type)
	{
	case _gui_text:
		render_data->render_debug_name = false;
		render_data->render_debug_bounds |= g_gui_debug_text_bounds;
		break;
	case _gui_bitmap:
		render_data->render_debug_bounds |= g_gui_debug_bitmap_bounds;
		break;
	case _gui_model:
		render_data->render_debug_bounds |= g_gui_debug_model_bounds;
		break;
	case _gui_group:
		render_data->render_debug_bounds |= g_gui_debug_group_bounds;
		break;
	case _gui_list_item:
		render_data->render_debug_bounds |= g_gui_debug_list_item_bounds;
		break;
	case _gui_list:
		render_data->render_debug_bounds |= g_gui_debug_list_bounds;
		break;
	case _gui_screen:
		render_data->render_debug_bounds |= g_gui_debug_screen_bounds;
		break;
	default:
		break;
	}
}

void c_gui_widget::calculate_animation_transform(e_animation_state animation_state, int32 start_time_milliseconds, int32 current_time_milliseconds, const s_widget_animation_definition* animation, s_animation_transform* transform, const real_vector2d* aspect_ratio_scale, bool initialize, bool combinative, bool* finished)
{
	INVOKE_CLASS_MEMBER(0x00AB73B0, c_gui_widget, calculate_animation_transform, animation_state, start_time_milliseconds, current_time_milliseconds, animation, transform, aspect_ratio_scale, initialize, combinative, finished);
}

bool const c_gui_widget::can_all_children_be_disposed()
{
	//return INVOKE_CLASS_MEMBER(0x00AB7850, c_gui_widget, can_all_children_be_disposed);

	bool result = true;
	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			result &= child->can_be_disposed();
		}
	}
	return result;
}

bool const c_gui_widget::can_be_disposed()
{
	//return INVOKE_CLASS_MEMBER(0x00AB78C0, c_gui_widget, can_be_disposed);

	bool result = m_needs_disposal;
	if (result)
	{
		for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
		{
			result &= child->can_be_disposed();
		}
	}
	return result;
}

bool c_gui_widget::can_receive_focus()
{
	//return INVOKE_CLASS_MEMBER(0x00AB7930, c_gui_widget, can_receive_focus);

	return false;
}

bool c_gui_widget::contains_point(const point2d* point)
{
	ASSERT(point != nullptr);

	rectangle2d bounds;
	get_mouse_region(&bounds);

	{
		short temp = bounds.x0;
		if (bounds.x0 > bounds.x1)
		{
			bounds.x0 = bounds.x1;
			bounds.x1 = temp;
		}
	}
	{
		short temp = bounds.y0;
		if (bounds.y0 > bounds.y1)
		{
			bounds.y0 = bounds.y1;
			bounds.y1 = temp;
		}
	}

	bool result = point2d_in_rectangle2d(&bounds, point);
	return result;
}

bool c_gui_widget::controller_can_drive(e_controller_index controller_index)
{
	return INVOKE_CLASS_MEMBER(0x00AB7960, c_gui_widget, controller_can_drive, controller_index);

	//return controller_index == k_any_controller || get_driving_controller() == k_any_controller || get_driving_controller() == controller_index;
}

//.text:00AB79D0 ; public: c_gui_button_key_widget* c_gui_widget::create_and_add_button_key(const s_button_key_block*)

c_gui_bitmap_widget* c_gui_widget::create_and_add_child_bitmap_widget(const s_bitmap_widget_block* bitmap_widget_block)
{
	return INVOKE_CLASS_MEMBER(0x00AB7A30, c_gui_widget, create_and_add_child_bitmap_widget, bitmap_widget_block);

#if 0
	ASSERT(bitmap_widget_block != nullptr);

	c_gui_widget* parent = get_parent();
	ASSERT(parent != nullptr);

	s_runtime_bitmap_widget_definition definition;
	real_rectangle2d positioning_bounds;

	parent->get_authored_bounds(&positioning_bounds);
	c_gui_bitmap_widget::assemble_definition(bitmap_widget_block, &definition, &positioning_bounds);

	c_gui_bitmap_widget* bitmap_widget = create_bitmap_widget(&definition);
	if (bitmap_widget != nullptr)
	{
		add_child_widget(bitmap_widget);
		bitmap_widget->initialize(bitmap_widget_block);
	}
	return bitmap_widget;
#endif
}

void c_gui_widget::create_and_add_child_list_item_widgets(const s_tag_block* list_items_block, int32 gui_skin_tag_index)
{
	INVOKE_CLASS_MEMBER(0x00AB7AC0, c_gui_widget, create_and_add_child_list_item_widgets, list_items_block, gui_skin_tag_index);

#if 0
	ASSERT(list_items_block != nullptr);

	if (gui_skin_tag_index == NONE)
	{
		event(_event_warning, "ui: list widget with no gui skin");
	}
	else
	{
		s_gui_skin_definition const* skin_definition = TAG_GET(GUI_SKIN_DEFINITION_TAG, s_gui_skin_definition, gui_skin_tag_index);
		for (long item_index = 0; item_index < list_items_block->count; item_index++)
		{
			s_list_item_widget_block const* item_block = TAG_BLOCK_GET_ELEMENT(list_items_block, item_index, s_list_item_widget_block);

			c_gui_screen_widget* parent_screen = get_parent_screen();
			c_gui_list_item_widget* item = parent_screen != nullptr ? parent_screen->create_list_item_widget(item_block) : create_list_item_widget(item_block);
			if (item == nullptr)
			{
				break;
			}

			add_child_widget(item);
			item->set_list_item_index(item_index);
			item->initialize(item_block);

			short render_depth_bias = item->get_render_depth_bias();
			long animation_collection_reference_index = item->get_animation_collection_reference_index();
			item->create_and_add_children_from_skin_definition(skin_definition, animation_collection_reference_index, render_depth_bias);
		}
	}
#endif
}

//.text:00AB7B80 ; public: c_gui_list_widget* c_gui_widget::create_and_add_child_list_widget(const s_list_widget_block*)
//.text:00AB7BC0 ; public: c_gui_model_widget* c_gui_widget::create_and_add_child_model_widget(const s_model_widget_block*)
//.text:00AB7C00 ; public: c_gui_text_widget* c_gui_widget::create_and_add_child_text_widget(const s_text_widget_block*)
//.text:00AB7C90 ; public: void c_gui_widget::create_and_add_children_from_skin_definition(const s_gui_skin_definition*, int32, int16)
//.text:00AB8160 ; public: c_gui_group_widget* c_gui_widget::create_and_add_group(const s_group_widget_block*)

c_gui_bitmap_widget* c_gui_widget::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AB81A0, c_gui_widget, create_bitmap_widget, definition);

	c_gui_bitmap_widget* bitmap_widget = new (_ui_allocation_marker_dummy) c_gui_bitmap_widget();
	return bitmap_widget;
}

c_gui_button_key_widget* c_gui_widget::create_button_key_widget(const s_button_key_definition* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8200, c_gui_widget, create_button_key_widget, definition);

	//c_gui_button_key_widget* button_key_widget = new (_ui_allocation_marker_dummy) c_gui_button_key_widget();
	//return button_key_widget;
}

c_gui_group_widget* c_gui_widget::create_group_widget(const s_group_widget_definition* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8260, c_gui_widget, create_group_widget, definition);

	//c_gui_group_widget* group_widget = new (_ui_allocation_marker_dummy) c_gui_group_widget();
	//return group_widget;
}

c_gui_list_item_widget* c_gui_widget::create_list_item_widget(const s_list_item_widget_block* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AB82C0, c_gui_widget, create_list_item_widget, definition);

	c_gui_list_item_widget* list_item_widget = new (_ui_allocation_marker_dummy) c_gui_list_item_widget();
	return list_item_widget;
}

c_gui_list_widget* c_gui_widget::create_list_widget(const s_list_widget_block* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8320, c_gui_widget, create_list_widget, definition);

	c_gui_list_widget* list_widget = new (_ui_allocation_marker_dummy) c_gui_list_widget();
	return list_widget;
}

c_gui_model_widget* c_gui_widget::create_model_widget(const s_model_widget_block* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8380, c_gui_widget, create_model_widget, definition);

	//c_gui_model_widget* model_widget = new (_ui_allocation_marker_dummy) c_gui_model_widget();
	//return model_widget;
}

c_gui_text_widget* c_gui_widget::create_text_widget(const s_runtime_text_widget_definition* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB83E0, c_gui_widget, create_text_widget, definition);

	//c_gui_text_widget* widget;
	//
	//if (TEST_BIT(definition->flags, s_text_widget_definition::_extra_large_text_field_bit))
	//{
	//	widget = new (_ui_allocation_marker_dummy) c_gui_sized_text_widget<1024>();
	//}
	//else if (TEST_BIT(definition->flags, s_text_widget_definition::_large_text_field_bit))
	//{
	//	widget = new (_ui_allocation_marker_dummy) c_gui_sized_text_widget<256>();
	//}
	//else
	//{
	//	widget = new (_ui_allocation_marker_dummy) c_gui_sized_text_widget<48>();
	//}
	//
	//return widget;
}

void c_gui_widget::delete_all_children()
{
	//INVOKE_CLASS_MEMBER(0x00AB8590, c_gui_widget, delete_all_children);

	c_gui_widget* child = get_children();
	set_children(nullptr);

	while (child != nullptr)
	{
		c_gui_widget* next = child->get_next();

		//sub_AB9AA0(child);
		child->set_parent(nullptr);
		child->dispose();
		ui_track_delete<c_gui_widget>(child);

		child = next;
	}
}

void c_gui_widget::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00AB8620, c_gui_widget, dispose);

	c_gui_screen_widget* parent_screen;

	if (get_type() == _gui_screen)
	{
		parent_screen = static_cast<c_gui_screen_widget*>(this);
	}
	else
	{
		parent_screen = get_parent_screen();
	}

	if (parent_screen != nullptr && parent_screen->get_focused_widget() == this)
	{
		for (c_gui_widget* focus_candidate = get_parent(); focus_candidate != nullptr; focus_candidate = focus_candidate->get_parent())
		{
			if (focus_candidate->can_receive_focus())
			{
				parent_screen->transfer_focus(focus_candidate);
				break;
			}
		}
	}

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		if (child->get_type() != _gui_screen)
		{
			child->dispose();
		}
	}
}

//.text:00AB8710 ; 

e_animation_state c_gui_widget::get_ambient_state()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8720, c_gui_widget, get_ambient_state);

	e_animation_state ambient_state;

	c_gui_list_widget* parent_list = c_gui_widget::get_parent_list();
	if (parent_list != nullptr && parent_list->has_active_submenu())
	{
		ambient_state = within_focus_chain() ? _child_submenu_ambient_focused : _child_submenu_ambient_unfocused;
	}
	else
	{
		ambient_state = k_invalid_animation_state;
	}

	if (ambient_state == k_invalid_animation_state)
	{
		bool has_focus = c_gui_widget::within_focus_chain();
		if (!has_focus)
		{
			e_gui_widget_type type = get_type();
			if (type != _gui_list && parent_list != nullptr && parent_list->list_selection_visible_without_focus())
			{
				c_gui_list_item_widget* list_item;

				if (type == _gui_list_item)
				{
					list_item = static_cast<c_gui_list_item_widget*>(this);
				}
				else
				{
					list_item = get_parent_list_item();
				}

				if (list_item != nullptr)
				{
					int32 focused_item_index = parent_list->get_focused_item_index();
					int32 list_item_index = focused_item_index - parent_list->get_scroll_position();
					has_focus = list_item_index == list_item->get_list_item_index();
				}
			}
		}

		//if (get_enabled() && !has_focus && has_highlight())
		//{
		//	ambient_state = _mouse_hover_ambient;
		//}
		//else
		{
			if (!get_enabled())
			{
				ambient_state = has_focus ? _focused_disabled_ambient : _unfocused_disabled_ambient;
			}
			else if (m_use_alternate_ambient_state)
			{
				ambient_state = has_focus ? _alternate_focused_ambient : _alternate_unfocused_ambient;
			}
			else
			{
				ambient_state = has_focus ? _focused_ambient : _unfocused_ambient;
			}
		}
	}

	return ambient_state;
}

//.text:00AB8810 ; 
//.text:00AB8820 ; private: static int32 __cdecl c_gui_widget::get_animation_period(const s_widget_component_animation_definition*, int32)
//.text:00AB8850 ; private: uns32 c_gui_widget::get_animation_state_timer(e_animation_state)

e_controller_index c_gui_widget::get_arbitrary_responding_controller() const
{
	return INVOKE_CLASS_MEMBER(0x00AB8900, c_gui_widget, get_arbitrary_responding_controller);
}

real_rectangle2d* c_gui_widget::get_authored_bounds(real_rectangle2d* unanimated_bounds)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A10, c_gui_widget, get_authored_bounds, unanimated_bounds);

	ASSERT(unanimated_bounds != nullptr);

	s_runtime_core_widget_definition* definition = get_core_definition();
	ASSERT(definition != nullptr);

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

#if 0 // $REVIEW
	c_gui_widget* found_widget = nullptr;

	if (get_type() == type && get_name() == name)
	{
		found_widget = this;
	}
	else
	{
		for (bool search_recursively = false; ; search_recursively = true)
		{
			for (c_gui_widget* test_widget = get_children(); test_widget != nullptr && found_widget == nullptr; test_widget = test_widget->get_next())
			{
				if (search_recursively)
				{
					found_widget = test_widget->get_child_widget(type, name);
				}
				else
				{
					if (test_widget->get_type() == type && test_widget->get_name() == name)
					{
						found_widget = test_widget;
					}
				}
			}

			if (found_widget != nullptr || search_recursively)
			{
				break;
			}
		}
	}

	return found_widget;
#endif
}

real_rectangle2d* c_gui_widget::get_container_current_bounds(real_rectangle2d* unanimated_bounds)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8BC0, c_gui_widget, get_container_current_bounds, unanimated_bounds);

	get_current_bounds(unanimated_bounds);

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			real_rectangle2d child_rectangle;
			child->get_container_current_bounds(&child_rectangle);

			unanimated_bounds->x0 = MIN(child_rectangle.x0, unanimated_bounds->x0);
			unanimated_bounds->y0 = MIN(child_rectangle.y0, unanimated_bounds->y0);
			unanimated_bounds->x1 = MAX(child_rectangle.x1, unanimated_bounds->x1);
			unanimated_bounds->y1 = MAX(child_rectangle.y1, unanimated_bounds->y1);
		}
	}

	return unanimated_bounds;
}

int32 c_gui_widget::get_controller_mask() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB8CA0, c_gui_widget, get_controller_mask);

	return m_flags & MASK(k_number_of_controllers);
}

real_argb_color* c_gui_widget::get_cumulative_color_tint(real_argb_color* tint)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8CB0, c_gui_widget, get_cumulative_color_tint, tint);

	ASSERT(tint != nullptr);

	*tint = m_animated_state.color;
	return tint;
}

real_rectangle2d* c_gui_widget::get_current_bounds(real_rectangle2d* unanimated_bounds)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8CE0, c_gui_widget, get_current_bounds, unanimated_bounds);

	ASSERT(unanimated_bounds != nullptr);

	s_runtime_core_widget_definition* definition = get_core_definition();
	ASSERT(definition != nullptr);

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

bool c_gui_widget::get_debug_animation_state()
{
	return TEST_BIT(m_flags, _debug_animation_state_bit);
}

bool c_gui_widget::get_debug_bounds() const
{
	return TEST_BIT(m_flags, _debug_bounds_bit);
}

const real_argb_color* c_gui_widget::get_debug_color()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8D30, c_gui_widget, get_debug_color);

	const real_argb_color* result = global_real_argb_grey;
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

bool c_gui_widget::get_debug_name() const
{
	return TEST_BIT(m_flags, _debug_name_bit);
}

bool c_gui_widget::get_debug_rotation_origin() const
{
	return TEST_BIT(m_flags, _debug_rotation_origin_bit);
}

c_gui_widget* c_gui_widget::get_deepest_widget_that_can_receive_focus()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8DA0, c_gui_widget, get_deepest_widget_that_can_receive_focus);

	c_gui_widget* widget = nullptr;

	for (c_gui_widget* child = get_children(); child != nullptr && widget == nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			widget = child->get_deepest_widget_that_can_receive_focus();
		}
	}

	if (widget == nullptr && can_receive_focus())
	{
		widget = this;
	}

	return widget;
}

int32 c_gui_widget::get_definition_index()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8E20, c_gui_widget, get_definition_index);

	int32 my_index;

	c_gui_widget* parent = get_parent();
	if (parent != nullptr)
	{
		int32 test_index = 0;
		for (c_gui_widget* child = parent->get_children(); child != this; child = child->get_next())
		{
			test_index++;
		}
		my_index = test_index;
	}
	else
	{
		my_index = NONE;
	}

	return my_index;
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

	int32 element_handle;

	c_gui_list_item_widget* parent_list_item = get_parent_list_item();
	if (parent_list_item != nullptr)
	{
		element_handle = parent_list_item->get_element_handle();
	}
	else
	{
		element_handle = NONE;
	}

	return element_handle;
}

bool c_gui_widget::get_enabled()
{
	//return INVOKE_CLASS_MEMBER(0x00AB8F70, c_gui_widget, get_enabled);

	return m_enabled;
}

c_gui_widget* c_gui_widget::get_first_child_widget_by_type(e_gui_widget_type type)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8F80, c_gui_widget, get_first_child_widget_by_type, type);

	c_gui_widget* child = get_children();
	while (child != nullptr)
	{
		e_gui_widget_type child_type = child->get_type();
		if (child_type == type)
		{
			break;
		}
		child = child->get_next();
	}
	return child;
}

//.text:00AB8FE0 ; 
//.text:00AB8FF0 ; 

c_gui_widget* c_gui_widget::get_last_child_widget_by_type(e_gui_widget_type type)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9010, c_gui_widget, get_last_child_widget_by_type, type);

	c_gui_widget* last_child_of_type = nullptr;

	c_gui_widget* child = get_children();
	while (child != nullptr)
	{
		e_gui_widget_type child_type = child->get_type();
		if (child_type == type)
		{
			last_child_of_type = child;
		}
		child = child->get_next();
	}

	return last_child_of_type;
}

//.text:00AB9080 ; void __cdecl get_local_coordinate_system_position_from_rotation_keyframe(c_gui_widget*, const s_rotation_keyframe_block*, real_point2d*)
//.text:00AB90C0 ; void __cdecl get_local_coordinate_system_position_from_scale_keyframe(c_gui_widget*, const s_scale_keyframe_block*, real_point2d*)
//.text:00AB9100 ; void __cdecl get_local_widget_coordinate_system_position_from_positioning(c_gui_widget*, e_widget_anchoring, real_point2d*)

void c_gui_widget::get_mouse_region(rectangle2d* mouse_region_out)
{
	ASSERT(mouse_region_out);

	rectangle2d null_bounds{};

	c_gui_screen_widget* parent_screen = c_gui_widget::get_parent_screen();
	if (parent_screen && parent_screen->get_render_state())
	{
		s_window_manager_screen_render_data* render_state = parent_screen->get_render_state();
		const rectangle2d* window_bounds = &render_state->built_for_viewport_bounds;

		gui_real_rectangle2d bounds{};
		c_gui_widget::get_projected_bounds(window_bounds, &bounds, true, true, true);
		mouse_region_out->x0 = (int16)bounds.top_left.x;
		mouse_region_out->y0 = (int16)bounds.top_left.y;
		mouse_region_out->x1 = (int16)bounds.bottom_right.x;
		mouse_region_out->y1 = (int16)bounds.bottom_right.y;
	}
	else
	{
		*mouse_region_out = null_bounds;
	}
}

c_gui_bitmap_widget* c_gui_widget::get_next_bitmap_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9220, c_gui_widget, get_next_bitmap_widget);

	return (c_gui_bitmap_widget*)get_next_widget_of_type(_gui_bitmap);
}

c_gui_list_item_widget* c_gui_widget::get_next_list_item_widget(bool only_consider_valid_items)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9230, c_gui_widget, get_next_list_item_widget, only_consider_valid_items);

	c_gui_list_item_widget* next_item = static_cast<c_gui_list_item_widget*>(get_next_widget_of_type(_gui_list_item));
	if (next_item != nullptr && only_consider_valid_items && next_item->get_element_handle() == NONE)
	{
		next_item = nullptr;
	}
	return next_item;
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
	//return INVOKE_CLASS_MEMBER(0x00AB9290, c_gui_widget, get_next_widget_of_type, type);

	c_gui_widget* test = get_next();
	while (test != nullptr)
	{
		e_gui_widget_type test_type = test->get_type();
		if (test_type == type)
		{
			break;
		}
		test = test->get_next();
	}
	return test;
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

	return m_name;
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
		return nullptr;
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
	//return INVOKE_CLASS_MEMBER(0x00AB9570, c_gui_widget, get_previous_widget_of_type, type);

	c_gui_widget* test = get_previous();
	while (test != nullptr)
	{
		e_gui_widget_type test_type = test->get_type();
		if (test_type == type)
		{
			break;
		}
		test = test->get_previous();
	}
	return test;
}

gui_real_rectangle2d* c_gui_widget::get_projected_bounds(const rectangle2d* window_bounds, gui_real_rectangle2d* projected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//return INVOKE_CLASS_MEMBER(0x00AB95D0, c_gui_widget, get_projected_bounds, window_bounds, projected_bounds, apply_translation, apply_scale, apply_rotation);

	ASSERT(window_bounds != NULL);
	ASSERT(projected_bounds != NULL);

	c_gui_widget::get_unprojected_bounds(projected_bounds, apply_translation, apply_scale, apply_rotation);

	projected_bounds->apply_projection_transform(m_animated_state.position.z, window_bounds);
	return projected_bounds;
}

int16 c_gui_widget::get_render_depth_bias()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9610, c_gui_widget, get_render_depth_bias);

	s_runtime_core_widget_definition* definition = get_core_definition();
	return (int16)definition->render_depth_bias;
}

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

//.text:00AB96D0 ; public: virtual const wchar_t* __cdecl c_sized_user_interface_text<256>::get_string()
//.text:00AB96E0 ; public: virtual const wchar_t* __cdecl c_sized_user_interface_text<48>::get_string()
//.text:00AB96F0 ; public: virtual const wchar_t* __cdecl c_sized_user_interface_text<1024>::get_string()

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

e_gui_widget_type c_gui_widget::get_type() const
{
	return m_type;
}

gui_real_rectangle2d* c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
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
	interface_get_current_display_settings(nullptr, nullptr, &render_window_bounds, nullptr);

	real_vector2d scale{};
	scale.i = rectangle2d_width(&render_window_bounds) / 1152.0f;
	scale.j = rectangle2d_height(&render_window_bounds) / 640.0f;
	unprojected_bounds->scale_direct(&scale);

	return unprojected_bounds;
}

//.text:00AB9870 ; 
//.text:00AB9880 ; 

bool c_gui_widget::get_visible() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB9920, c_gui_widget, get_visible);

	return m_visible;
}

//.text:00AB9930 ; public: const s_widget_animation_definition* c_gui_widget::get_widget_animation_definition(e_animation_state)

bool c_gui_widget::handle_alt_stick(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9980, c_gui_widget, handle_alt_stick, message);

	if (!get_parent())
	{
		return false;
	}

	return get_parent()->handle_alt_stick(message);
}

bool c_gui_widget::handle_alt_tab(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB99E0, c_gui_widget, handle_alt_tab, message);

	bool handled;

	c_gui_widget* parent = get_parent();
	if (parent != nullptr)
	{
		handled = parent->handle_alt_tab(message);
	}
	else
	{
		handled = false;
	}

	return handled;
}

bool c_gui_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9A40, c_gui_widget, handle_controller_input_message, message);

	if (!get_parent())
	{
		return false;
	}

	return get_parent()->handle_controller_input_message(message);
}

//.text:00AB9AA0 ; c_gui_widget::handle_?

bool c_gui_widget::handle_tab(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9B40, c_gui_widget, handle_tab, message);

	bool handled;

	c_gui_widget* parent = get_parent();
	if (parent != nullptr)
	{
		handled = parent->handle_tab(message);
	}
	else
	{
		handled = false;
	}

	return handled;
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

	SET_BIT(m_flags, _initializing_bit, true);

	real_rectangle2d widget_bounds;
	get_current_bounds(&widget_bounds);

	s_runtime_core_widget_definition* definition = get_core_definition();
	definition->runtime_bounds = widget_bounds;
}

bool c_gui_widget::is_animation_active(e_animation_state animation_state)
{
	return INVOKE_CLASS_MEMBER(0x00AB9C10, c_gui_widget, is_animation_active, animation_state);

	ASSERT(animation_state != k_invalid_animation_state);

	ASSERT((FLAG_64(animation_state) & k_animation_state_ambient) == 0);
	bool result = TEST_BIT_64(m_animated_state.state_flags, animation_state);
	return result;
}

//.text:00AB9C50 ; 
//.text:00AB9C70 ; 
//.text:00AB9CB0 ; 

bool c_gui_widget::leaf_node_of_widget(c_gui_widget* branch_widget)
{
	//return INVOKE_CLASS_MEMBER(0x00AB9CC0, c_gui_widget, leaf_node_of_widget, branch_widget);

	ASSERT(branch_widget != nullptr);

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

void c_gui_list_widget::mark_as_submenu_that_needs_disposal(bool dispose)
{
	//INVOKE_CLASS_MEMBER(0x00AB9D60, c_gui_list_widget, mark_as_submenu_that_needs_disposal, dispose);

	m_is_submenu_needs_disposal = dispose;
}

void c_gui_widget::modulate_tint_color(const real_argb_color* modulation)
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

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		if (child->get_type() != _gui_screen)
		{
			child->post_initialize();
		}
	}

	SET_BIT(m_flags, _initializing_bit, false);
}

void c_gui_widget::remove_child_widget(c_gui_widget* child)
{
	//INVOKE_CLASS_MEMBER(0x00AB9E40, c_gui_widget, remove_child_widget, child);

	ASSERT(child != nullptr);
	ASSERT(child->get_parent() == this);

	c_gui_widget* previous = child->get_previous();
	c_gui_widget* next = child->get_next();

	if (previous != nullptr)
	{
		previous->set_next(next);
	}
	if (next != nullptr)
	{
		next->set_previous(previous);
	}
	if (get_children() == child)
	{
		set_children(next);
	}
	child->set_parent(nullptr);
}

void c_gui_widget::render(int32 user_index, const s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, bool is_screenshot)
{
	//INVOKE(0x00AB9F40, c_gui_widget::render, user_index, render_data, window_bounds, is_screenshot);

	ASSERT(render_data != nullptr);
	ASSERT(window_bounds != nullptr);

	//c_rasterizer_profile_scope _render(, L"");

	if (!is_screenshot || render_data->flags.test(s_gui_widget_render_data::_render_in_screenshot_bit))
	{
		if (render_data->type == _gui_text)
		{
			c_user_interface_text::render((s_user_interface_text_render_data*)render_data, window_bounds);
		}
		else if (render_data->type == _gui_bitmap)
		{
			render_bitmap((const s_gui_bitmap_widget_render_data*)render_data, window_bounds);
		}
		else if (render_data->type == _gui_model)
		{
			// $IMPLEMENT `c_gui_model_widget`
			//c_gui_model_widget::render((const s_gui_model_widget_render_data*)render_data, window_bounds);
			return;
		}
	}

	if (render_data->render_debug_name)
	{
		c_simple_font_draw_string draw_string;
		real_rectangle2d text_bounds
		{
			.x0 = render_data->projected_bounds.vertex[0].x,
			.x1 = (real32)window_bounds->x1,
			.y0 = render_data->projected_bounds.vertex[0].y,
			.y1 = (real32)window_bounds->y1
		};
		draw_string.set_bounds(&text_bounds);
		draw_string.set_color(render_data->debug_color);

		const char* text = string_id_get_string_const(render_data->name);
		draw_string.draw(nullptr, text);
	}

	if (render_data->render_debug_animation_state)
	{
		// $TODO `c_gui_text_widget::update` for `#string#` text render

		c_static_string<1024> text;
		text.set("");

		for (int32 animation_state = 0; animation_state < k_number_of_ui_animation_states; animation_state++)
		{
			if ((FLAG_64(animation_state) & render_data->animation_state_flags) == 0)
			{
				continue;
			}

			switch (animation_state)
			{
			case _focused_ambient:
				text.append("focused-ambient:");
				break;
			case _unfocused_ambient:
				text.append("unfocused-ambient:");
				break;
			case _focused_disabled_ambient:
				text.append("focused-disabled-ambient:");
				break;
			case _unfocused_disabled_ambient:
				text.append("unfocused-disabled-ambient:");
				break;
			case _alternate_focused_ambient:
				text.append("alt-focused-ambient:");
				break;
			case _alternate_unfocused_ambient:
				text.append("alt-unfocused-ambient:");
				break;
			case _transition_from_screen:
				text.append("transition-from:");
				break;
			case _transition_to_screen:
				text.append("transition-to:");
				break;
			case _transition_back_from_screen:
				text.append("transition-back-from:");
				break;
			case _transition_back_to_screen:
				text.append("transition-back-to:");
				break;
			case _cycle_in_previous_screen:
				text.append("cycle-in-prev-screen:");
				break;
			case _cycle_in_next_screen:
				text.append("cycle-in-next-screen:");
				break;
			case _cycle_out_previous_screen:
				text.append("cycle-out-prev-screen:");
				break;
			case _cycle_out_next_screen:
				text.append("cycle-out-next-screen:");
				break;
			case _list_display_group_transition_in:
				text.append("display-group-in:");
				break;
			case _list_display_group_transition_out:
				text.append("display-group-out:");
				break;
			case _control_received_focus:
				text.append("control-focus:");
				break;
			case _control_lost_focus:
				text.append("control-unfocus:");
				break;
			case _indicator_ambient_additional_items:
				text.append("indicator-ambient-more-items:");
				break;
			case _indicator_ambient_no_additional_items:
				text.append("indicator-ambient-no-more-items:");
				break;
			case _indicator_activated_additional_items:
				text.append("indicator-active-more-items:");
				break;
			case _indicator_activated_no_additional_items:
				text.append("indicator-active-no-more-items:");
				break;
			case _load_submenu_focused:
				text.append("load-submenu-focused:");
				break;
			case _load_submenu_unfocused:
				text.append("load-submenu-unfocused:");
				break;
			case _unload_submenu_focused:
				text.append("unload-submenu-focused:");
				break;
			case _unload_submenu_unfocused:
				text.append("unload-submenu-unfocused:");
				break;
			case _load_as_submenu:
				text.append("load-as-submenu:");
				break;
			case _unload_as_submenu:
				text.append("unload-as-submenu:");
				break;
			case _child_submenu_ambient_focused:
				text.append("child-submenu-ambient-focused:");
				break;
			case _child_submenu_ambient_unfocused:
				text.append("child-submenu-ambient-unfocused:");
				break;
			case _custom_animation0:
				text.append("custom0:");
				break;
			case _custom_animation1:
				text.append("custom1:");
				break;
			case _custom_screen_transition_in0:
				text.append("custom-transition-in0:");
				break;
			case _custom_screen_transition_out0:
				text.append("custom-transition-out0:");
				break;
			case _custom_screen_transition_in1:
				text.append("custom-transition-in1:");
				break;
			case _custom_screen_transition_out1:
				text.append("custom-transition-out1:");
				break;
			case _custom_display_group_in:
				text.append("custom-display-group-in:");
				break;
			case _custom_display_group_out:
				text.append("custom-display-group-out:");
				break;
			case _transition_to_overlaid:
				text.append("transition-to-overlaid:");
				break;
			case _transition_from_overlaid:
				text.append("transition-from-overlaid:");
				break;
			//case _mouse_enter:
			//	text.append("mouse-enter:");
			//	break;
			//case _mouse_leave:
			//	text.append("mouse-leave:");
			//	break;
			//case _mouse_hover_ambient:
			//	text.append("mouse-hover-ambient:");
			//	break;
			default:
			{
				UNREACHABLE();
			}
			break;
			}
		}
		c_simple_font_draw_string draw_string;
		real_rectangle2d text_bounds
		{
			.x0 = render_data->projected_bounds.vertex[0].x,
			.x1 = (real32)window_bounds->x1,
			.y0 = render_data->projected_bounds.vertex[0].y,
			.y1 = (real32)window_bounds->y1
		};
		draw_string.set_bounds(&text_bounds);
		draw_string.set_color(render_data->debug_color);
		draw_string.draw(nullptr, text.get_string());
	}

	if (render_data->render_debug_bounds)
	{
		point2d points[5]{};

		points[0].x = (int16)render_data->projected_bounds.vertex[0].x;
		points[0].y = (int16)render_data->projected_bounds.vertex[0].y;
		points[1].x = (int16)render_data->projected_bounds.vertex[2].x;
		points[1].y = (int16)render_data->projected_bounds.vertex[2].y;
		points[2].x = (int16)render_data->projected_bounds.vertex[3].x;
		points[2].y = (int16)render_data->projected_bounds.vertex[3].y;
		points[3].x = (int16)render_data->projected_bounds.vertex[1].x;
		points[3].y = (int16)render_data->projected_bounds.vertex[1].y;
		points[4].x = (int16)render_data->projected_bounds.vertex[0].x;
		points[4].y = (int16)render_data->projected_bounds.vertex[0].y;

		real_vector2d aspect_ratio_scale = interface_get_aspect_ratio_scaling();
		interface_scale_points_for_xenon_scaler(points, NUMBEROF(points), &aspect_ratio_scale);
		c_rasterizer::draw_debug_linestrip2d(points, NUMBEROF(points), render_data->debug_color);
	}

	if (render_data->render_debug_rotation_origin)
	{
		c_simple_font_draw_string draw_string;
		real_rectangle2d text_bounds
		{
			.x0 = render_data->rotation_origin_with_depth.x - 8.0f,
			.x1 = (real32)window_bounds->x1,
			.y0 = render_data->rotation_origin_with_depth.y - 8.0f,
			.y1 = (real32)window_bounds->y1
		};
		draw_string.set_bounds(&text_bounds);
		draw_string.set_color(render_data->debug_color);
		draw_string.draw(nullptr, "(+)");
	}
}

//.text:00AB9F90 ; 
//.text:00AB9FA0 ; 

void c_gui_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	//INVOKE_CLASS_MEMBER(0x00AB9FD0, c_gui_widget, set_animated_state_baseline, transform);

	set_real_argb_color(&transform->color, 1.0f, 1.0f, 1.0f, 1.0f);
	set_real_point3d(&transform->position, 0.0f, 0.0f, 0.0f);
	set_real_vector2d(&transform->scale, 1.0f, 1.0f);
	set_real_point2d(&transform->local_scale_origin, 0.0f, 0.0f);
	transform->rotation_angle_radians = 0.0f;
	transform->sine_rotation_angle = 0.0f;
	transform->cosine_rotation_angle = 1.0f;
	set_real_point2d(&transform->local_rotation_origin, 0.0f, 0.0f);
	set_real_point2d(&transform->texture_uv, 0.0f, 0.0f);
	transform->bitmap_sprite_sequence = 0;
	transform->bitmap_sprite_frame = 0;
	transform->font = _terminal_font;
}

//.text:00ABA070 ; 
//.text:00ABA120 ; 

void c_gui_widget::set_child_bitmap_sprite_frame(int32 widget_name, int32 sprite_frame_index)
{
	//INVOKE_CLASS_MEMBER(0x00ABA1E0, c_gui_widget, set_child_bitmap_sprite_frame, widget_name, sprite_frame_index);

	c_gui_bitmap_widget* bitmap_widget = get_child_bitmap_widget(widget_name);
	if (bitmap_widget != nullptr)
	{
		bitmap_widget->set_sprite_frame(sprite_frame_index);
	}
}

void c_gui_widget::set_child_enabled(e_gui_widget_type widget_type, int32 widget_name, bool enabled)
{
	//INVOKE_CLASS_MEMBER(0x00ABA200, c_gui_widget, set_child_enabled, widget_type, widget_name, enabled);

	c_gui_widget* widget = get_child_widget(widget_type, widget_name);
	if (widget != nullptr)
	{
		widget->set_enabled(enabled);
	}
}

void c_gui_widget::set_child_use_alternate_ambient_state(e_gui_widget_type widget_type, int32 widget_name, bool value)
{
	//INVOKE_CLASS_MEMBER(0x00ABA220, c_gui_widget, set_child_use_alternate_ambient_state, widget_type, widget_name, value);

	c_gui_widget* widget = get_child_widget(widget_type, widget_name);
	if (widget != nullptr)
	{
		widget->set_use_alternate_ambient_state(value);
	}
}

void c_gui_widget::set_child_visible(e_gui_widget_type widget_type, int32 widget_name, bool visible)
{
	//INVOKE_CLASS_MEMBER(0x00ABA240, c_gui_widget, set_child_visible, widget_type, widget_name, visible);

	c_gui_widget* widget = get_child_widget(widget_type, widget_name);
	if (widget != nullptr)
	{
		widget->set_visible(visible);
	}
}

void c_gui_widget::set_children(c_gui_widget* children)
{
	//INVOKE_CLASS_MEMBER(0x00ABA260, c_gui_widget, set_children, children);

	m_children.set_value(children);
}

void c_gui_widget::set_debug_animation_state(bool activate)
{
	SET_BIT(m_flags, _debug_animation_state_bit, activate);
}

void c_gui_widget::set_debug_bounds(bool activate)
{
	SET_BIT(m_flags, _debug_bounds_bit, activate);
}

void c_gui_widget::set_debug_name(bool activate)
{
	SET_BIT(m_flags, _debug_name_bit, activate);
}

void c_gui_widget::set_debug_rotation_origin(bool activate)
{
	SET_BIT(m_flags, _debug_rotation_origin_bit, activate);
}

void c_gui_widget::set_driving_controller(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00ABA280, c_gui_widget, set_driving_controller, controller_index);
}

void c_gui_group_widget::set_dispose_as_display_group(bool dispose)
{
	//INVOKE_CLASS_MEMBER(0x00ABA270, c_gui_group_widget, set_dispose_as_display_group, dispose);

	m_dispose_as_display_group = dispose;
}

void c_gui_widget::set_enabled(bool value)
{
	//INVOKE_CLASS_MEMBER(0x00ABA2D0, c_gui_widget, set_enabled, value);
	
	m_enabled = value;

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			child->set_enabled(value);
		}
	}
}

void c_gui_widget::set_full_animation_state(const s_animation_transform* transform, bool recursive)
{
	//INVOKE_CLASS_MEMBER(0x00ABA340, c_gui_widget, set_full_animation_state, transform, recursive);

	m_animated_state = *transform;

	if (recursive)
	{
		for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
		{
			child->set_full_animation_state(transform, recursive);
		}
	}
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
//.text:00ABA450 ; public: virtual void __cdecl c_sized_user_interface_text<256>::set_string(const wchar_t*, bool, int32)
//.text:00ABA550 ; public: virtual void __cdecl c_sized_user_interface_text<48>::set_string(const wchar_t*, bool, int32)
//.text:00ABA630 ; public: virtual void __cdecl c_sized_user_interface_text<1024>::set_string(const wchar_t*, bool, int32)

void c_gui_widget::set_tint_color_direct(const real_argb_color* color)
{
	//INVOKE_CLASS_MEMBER(0x00ABA730, c_gui_widget, set_tint_color_direct, color);

	ASSERT(color != nullptr);

	m_animated_state.color = *color;
}

void c_gui_widget::set_use_alternate_ambient_state(bool value)
{
	//INVOKE_CLASS_MEMBER(0x00ABA760, c_gui_widget, set_use_alternate_ambient_state, value);

	m_use_alternate_ambient_state = value;

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		child->set_use_alternate_ambient_state(value);
	}
}

//.text:00ABA7D0 ; public: void c_robust_pointer<c_gui_widget>::set_value(c_gui_widget* value)

void c_gui_widget::set_visible(bool value)
{
	//INVOKE_CLASS_MEMBER(0x00ABA7E0, c_gui_widget, set_visible, value);

	m_visible = value;

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			child->set_visible(value);
		}
	}
}

bool c_gui_widget::should_render(bool* add_to_render_list)
{
	//return INVOKE_CLASS_MEMBER(0x00ABA850, c_gui_widget, should_render, add_to_render_list);

	bool render_me = get_visible();

	e_gui_widget_type type = get_type();
	if (add_to_render_list != nullptr)
	{
		*add_to_render_list = type != _gui_text || type == _gui_bitmap || type == _gui_model;
	}

	return render_me;
}

void c_gui_widget::start_animation(e_animation_state animation_state, bool recursive)
{
	//INVOKE_CLASS_MEMBER(0x00ABA890, c_gui_widget, start_animation, animation_state, recursive);

	start_animation_at_time(animation_state, 0xFFFFFFFF, recursive);
}

void c_gui_widget::start_animation_at_time(e_animation_state animation_state, uns32 animation_start_time, bool recursive)
{
	INVOKE_CLASS_MEMBER(0x00ABA8B0, c_gui_widget, start_animation_at_time, animation_state, animation_start_time, recursive);
}

void c_gui_widget::stomp_bounds(const real_rectangle2d* new_bounds)
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

	c_gui_widget* child = get_children();
	while (child != nullptr)
	{
		c_gui_widget* next = child->get_next();

		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			child->update(current_milliseconds);
		}

		child = next;
	}
}

void c_gui_widget::update_animation(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00ABB0E0, c_gui_widget, update_animation, current_milliseconds);

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type type = child->get_type();
		if (type != _gui_screen)
		{
			child->update_animation(current_milliseconds);
		}
	}

	animate(current_milliseconds);

	if (can_be_disposed() && can_all_children_be_disposed())
	{
		e_gui_widget_type type = get_type();
		if (type == _gui_group || type == _gui_button_key)
		{
			c_gui_group_widget* group_widget = static_cast<c_gui_group_widget*>(this);
			group_widget->set_dispose_as_display_group(true);
		}
		else if (type == _gui_list)
		{
			c_gui_list_widget* list = static_cast<c_gui_list_widget*>(this);
			if (list->is_submenu())
			{
				list->mark_as_submenu_that_needs_disposal(true);
			}
		}
	}
}

void c_gui_widget::update_render_state(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00ABB1A0, c_gui_widget, update_render_state, current_milliseconds);

	if (m_visible)
	{
		for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
		{
			e_gui_widget_type type = child->get_type();
			if (type != _gui_screen)
			{
				child->update_render_state(current_milliseconds);
			}
		}
	}
}

//.text:00ABB210 ; 

bool c_gui_widget::verify_animation_period(e_animation_state animation_state, int32 period)
{
	return INVOKE_CLASS_MEMBER(0x00ABB220, c_gui_widget, verify_animation_period, animation_state, period);
}

const char* __cdecl c_gui_widget::widget_name_to_string(int32 name)
{
	//return INVOKE(0x00ABB270, c_gui_widget::widget_name_to_string, name);

	char const* string = string_id_get_string_const(name);
	return string;
}

const char* __cdecl c_gui_widget::widget_type_to_string(e_gui_widget_type type)
{
	//return INVOKE(0x00ABB280, c_gui_widget::widget_type_to_string, type);

	char const* type_name;
	switch (type)
	{
	case _gui_text:
		type_name = "text";
		break;
	case _gui_bitmap:
		type_name = "bitmap";
		break;
	case _gui_model:
		type_name = "model";
		break;
	case _gui_group:
		type_name = "group";
		break;
	case _gui_button_key:
		type_name = "button_key";
		break;
	case _gui_list_item:
		type_name = "item";
		break;
	case _gui_slider:
		type_name = "slider";
		break;
	case _gui_list:
		type_name = "list";
		break;
	case _gui_screen:
		type_name = "screen";
		break;
	default:
		type_name = "<unknown>";
		break;
	}
	return type_name;
}

bool c_gui_widget::within_focus_chain()
{
	//return INVOKE_CLASS_MEMBER(0x00ABB300, c_gui_widget, within_focus_chain);

	c_gui_screen_widget* screen = (c_gui_screen_widget*)this;
	if (m_type != _gui_screen)
	{
		screen = get_parent_screen();
	}

	if (!screen)
	{
		return false;
	}

	c_gui_widget* focused_widget = screen->get_focused_widget();
	if (!focused_widget)
	{
		return false;
	}

	c_gui_widget* branch_widget = this;
	if (m_type != _gui_list_item && get_parent_list_item())
	{
		branch_widget = get_parent_list_item();
	}

	if (!branch_widget)
	{
		return false;
	}

	return focused_widget->leaf_node_of_widget(branch_widget);
}

