#include "interface/c_gui_widget.hpp"

#include "interface/interface_constants.hpp"
#include "interface/user_interface_data.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AB97C0, c_gui_widget, get_unprojected_bounds);

long __cdecl c_gui_widget::get_ambient_state()
{
	return __vftable->get_ambient_state(this);
}

void* __cdecl c_gui_widget::destructor(dword a1)
{
	return __vftable->destructor(this, a1);
}

void __cdecl c_gui_widget::initialize()
{
	__vftable->initialize(this);
}

void __cdecl c_gui_widget::post_initialize()
{
	__vftable->post_initialize(this);
}

void __cdecl c_gui_widget::dispose()
{
	__vftable->dispose(this);
}

long __cdecl c_gui_widget::get_name()
{
	return __vftable->get_name(this);
}

long __cdecl c_gui_widget::get_element_handle()
{
	return __vftable->get_element_handle(this);
}

long __cdecl c_gui_widget::get_datasource_index()
{
	return __vftable->get_datasource_index(this);
}

bool __cdecl c_gui_widget::get_enabled()
{
	return __vftable->get_enabled(this);
}

void __cdecl c_gui_widget::set_enabled(bool a1)
{
	return __vftable->set_enabled(this, a1);
}

s_runtime_core_widget_definition* __cdecl c_gui_widget::get_core_definition()
{
	return __vftable->get_core_definition(this);
}

real_rectangle2d* __cdecl c_gui_widget::get_current_bounds(real_rectangle2d* a1)
{
	return __vftable->get_current_bounds(this, a1);
}

real_rectangle2d* __cdecl c_gui_widget::get_container_current_bounds(real_rectangle2d* a1)
{
	return __vftable->get_container_current_bounds(this, a1);
}

bool __cdecl c_gui_widget::should_render(bool* a1)
{
	return __vftable->should_render(this, a1);
}

real_argb_color* __cdecl c_gui_widget::get_cumulative_color_tint(real_argb_color* a1)
{
	return __vftable->get_cumulative_color_tint(this, a1);
}

bool __cdecl c_gui_widget::can_receive_focus()
{
	return __vftable->can_receive_focus(this);
}

bool __cdecl c_gui_widget::within_focus_chain()
{
	return __vftable->within_focus_chain(this);
}

c_gui_text_widget* __cdecl c_gui_widget::create_text_widget(s_runtime_text_widget_definition const* a1)
{
	return __vftable->create_text_widget(this, a1);
}

c_gui_bitmap_widget* __cdecl c_gui_widget::create_bitmap_widget(s_runtime_bitmap_widget_definition const* a1)
{
	return __vftable->create_bitmap_widget(this, a1);
}

c_gui_group_widget* __cdecl c_gui_widget::create_group_widget(s_group_widget_definition const* a1)
{
	return __vftable->create_group_widget(this, a1);
}

c_gui_button_key_widget* __cdecl c_gui_widget::create_button_key_widget(s_button_key_definition const* a1)
{
	return __vftable->create_button_key_widget(this, a1);
}

c_gui_list_widget* __cdecl c_gui_widget::create_list_widget(s_list_widget_block const* a1)
{
	return __vftable->create_list_widget(this, a1);
}

c_gui_list_item_widget* __cdecl c_gui_widget::create_list_item_widget(s_list_item_widget_block const* a1)
{
	return __vftable->create_list_item_widget(this, a1);
}

c_gui_model_widget* __cdecl c_gui_widget::create_model_widget(s_model_widget_block const* a1)
{
	return __vftable->create_model_widget(this, a1);
}

void __cdecl c_gui_widget::update(dword a1)
{
	__vftable->update(this, a1);
};

void __cdecl c_gui_widget::update_render_state(dword a1)
{
	__vftable->update_render_state(this, a1);
};

void __cdecl c_gui_widget::set_animated_state_baseline(s_animation_transform* a1)
{
	__vftable->set_animated_state_baseline(this, a1);
};

void __cdecl c_gui_widget::assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index controller_index, long projected_bounds, bool offset, bool scale_about_local_point, bool rotate_about_local_point)
{
	__vftable->assemble_render_data(this, render_data, window_bounds, controller_index, projected_bounds, offset, scale_about_local_point, rotate_about_local_point);
}

bool __cdecl c_gui_widget::handle_widget_back_out()
{
	return __vftable->handle_widget_back_out(this);
}

bool __cdecl c_gui_widget::handle_tab(c_controller_input_message const* input_message)
{
	return __vftable->handle_tab(this, input_message);
}

bool __cdecl c_gui_widget::get_string_by_string_id(string_id name, c_static_wchar_string<1024>* out_string)
{
	return __vftable->get_string_by_string_id(this, name, out_string);
}

c_gui_list_item_widget* __cdecl c_gui_widget::get_next_list_item_widget(bool a1)
{
	return INVOKE_CLASS_MEMBER(0x00AB9230, c_gui_widget::get_next_list_item_widget, a1);
}

void __cdecl c_gui_widget::set_visible(bool is_visible)
{
	INVOKE_CLASS_MEMBER(0x00ABA7E0, c_gui_widget::set_visible, is_visible);
}

c_gui_bitmap_widget* __cdecl c_gui_widget::get_child_bitmap_widget(string_id name)
{
	return (c_gui_bitmap_widget*)get_child_widget(_gui_widget_type_bitmap_widget, name);
}

c_gui_group_widget* __cdecl c_gui_widget::get_child_group_widget(string_id name)
{
	return (c_gui_group_widget*)get_child_widget(_gui_widget_type_group_widget, name);
}

c_gui_list_item_widget* __cdecl c_gui_widget::get_child_list_item_widget(string_id name)
{
	return (c_gui_list_item_widget*)get_child_widget(_gui_widget_type_list_item_widget, name);
}

c_gui_list_widget* __cdecl c_gui_widget::get_child_list_widget(string_id name)
{
	return (c_gui_list_widget*)get_child_widget(_gui_widget_type_list_widget, name);
}

c_gui_model_widget* __cdecl c_gui_widget::get_child_model_widget(string_id name)
{
	return (c_gui_model_widget*)get_child_widget(_gui_widget_type_model_widget, name);
}

c_gui_text_widget* __cdecl c_gui_widget::get_child_text_widget(string_id name)
{
	return (c_gui_text_widget*)get_child_widget(_gui_widget_type_text_widget, name);
}

c_gui_widget* __cdecl c_gui_widget::get_child_widget(e_gui_widget_type widget_type, string_id name)
{
	return INVOKE_CLASS_MEMBER(0x00AB8B00, c_gui_widget::get_child_widget, widget_type, name);
}

c_gui_widget* __cdecl c_gui_widget::get_first_child_widget_by_type(e_gui_widget_type widget_type)
{
	return INVOKE_CLASS_MEMBER(0x00AB8F80, c_gui_widget::get_first_child_widget_by_type, widget_type);
}

void __thiscall c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool a3, bool a4, bool a5)
{
	HOOK_INVOKE_CLASS_MEMBER(, c_gui_widget, get_unprojected_bounds, unprojected_bounds, a3, a4, a5);

	// this is more or less what Halo 3 MCC is doing

	short_rectangle2d display_pixel_bounds;
	interface_get_current_display_settings(nullptr, nullptr, &display_pixel_bounds, nullptr);

	real width_scale_factor = (display_pixel_bounds.x1 - display_pixel_bounds.x0) / 1152.0f;
	real height_scale_factor = (display_pixel_bounds.y1 - display_pixel_bounds.y0) / 640.0f;

	unprojected_bounds->x0_0 *= width_scale_factor;
	unprojected_bounds->y0_0 *= height_scale_factor;
	unprojected_bounds->x0_1 *= width_scale_factor;
	unprojected_bounds->y0_1 *= height_scale_factor;
	unprojected_bounds->x1_0 *= width_scale_factor;
	unprojected_bounds->y0_2 *= height_scale_factor;
	unprojected_bounds->x1_1 *= width_scale_factor;
	unprojected_bounds->y1_0 *= height_scale_factor;
}

