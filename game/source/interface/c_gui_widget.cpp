#include "interface/c_gui_widget.hpp"

#include "interface/interface_constants.hpp"
#include "interface/user_interface_data.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AB97C0, c_gui_widget, get_unprojected_bounds);

struct s_gui_widget_render_data
{
	enum e_widget_render_data_flags
	{
		_render_as_custom_storage_bitmap_bit = 0,
		_render_texture_camera_bit,
		_render_blurred_back_buffer_bit,
		_render_as_player_emblem_bit,
		_emblem_info_valid_bit,
		_render_in_screenshot_bit,
		_render_as_au2_playlist_rating,
		_render_as_au2_rank_tray,

		k_number_of_bitmap_render_data_flags
	};

	e_gui_widget_type type;
	c_flags<e_widget_render_data_flags, long, k_number_of_bitmap_render_data_flags> flags;
	gui_real_rectangle2d projected_bounds;
	e_controller_index local_controller_index;
};

/*
//.text:00AB9F40 ; public: static void __cdecl c_gui_widget::render(long, s_gui_widget_render_data const*, rectangle2d const*, bool)
static void __cdecl gui_widget_render(long user_index, s_gui_widget_render_data const* render_data, rectangle2d const* window_bounds, bool is_screenshot)
{
	c_rasterizer_profile_scope _widget_render(_rasterizer_profile_element_interface_hud, L"c_gui_widget::render");
	INVOKE(0x00AB9F40, gui_widget_render, user_index, render_data, window_bounds, is_screenshot);

	//if (!is_screenshot || render_data->flags.test(s_gui_widget_render_data::_render_in_screenshot_bit))
	//{
	//	switch (render_data->type)
	//	{
	//	case _gui_text:
	//	{
	//		s_user_interface_text_render_data const* text_render_data = (s_user_interface_text_render_data const*)render_data;
	//		c_user_interface_text::render(text_render_data, window_bounds);
	//	}
	//	break;
	//	case _gui_bitmap:
	//	{
	//		s_gui_bitmap_widget_render_data const* bitmap_render_data = (s_gui_bitmap_widget_render_data const*)render_data;
	//		render_bitmap(bitmap_render_data, window_bounds);
	//	}
	//	break;
	//	case _gui_model:
	//	{
	//		s_gui_model_widget_render_data const* model_render_data = (s_gui_model_widget_render_data const*)render_data;
	//		c_gui_model_widget::render(model_render_data, window_bounds);
	//	}
	//	break;
	//	}
	//}
}
HOOK_DECLARE_CALL(0x00AADA93, gui_widget_render);
*/

void* c_gui_widget::destructor(dword a1)
{
	return __vftable->destructor(this, a1);
}

bool c_gui_widget::get_enabled()
{
	return __vftable->get_enabled(this);
}

s_runtime_core_widget_definition* c_gui_widget::get_core_definition()
{
	return __vftable->get_core_definition(this);
}

bool c_gui_widget::should_render(bool* add_to_render_list)
{
	return __vftable->should_render(this, add_to_render_list);
}

bool c_gui_widget::can_receive_focus()
{
	return __vftable->can_receive_focus(this);
}

bool c_gui_widget::within_focus_chain()
{
	return __vftable->within_focus_chain(this);
}

void c_gui_widget::update(dword current_milliseconds)
{
	__vftable->update(this, current_milliseconds);
}

void c_gui_widget::update_render_state(dword current_milliseconds)
{
	__vftable->update_render_state(this, current_milliseconds);
}

bool c_gui_widget::transitioning_in()
{
	return __vftable->transitioning_in(this);
}

bool c_gui_widget::transitioning_out()
{
	return __vftable->transitioning_out(this);
}

void c_gui_widget::assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index controller_index, long projected_bounds, bool offset, bool scale_about_local_point, bool rotate_about_local_point)
{
	__vftable->assemble_render_data(this, render_data, window_bounds, controller_index, projected_bounds, offset, scale_about_local_point, rotate_about_local_point);
}

bool c_gui_widget::handle_tab(c_controller_input_message const* message)
{
	return __vftable->handle_tab(this, message);
}

bool c_gui_widget::handle_controller_input_message(c_controller_input_message const* message)
{
	return __vftable->handle_controller_input_message(this, message);
}

c_gui_bitmap_widget* c_gui_widget::create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition)
{
	return __vftable->create_bitmap_widget(this, definition);
}

c_gui_button_key_widget* c_gui_widget::create_button_key_widget(s_button_key_definition const* definition)
{
	return __vftable->create_button_key_widget(this, definition);
}

c_gui_group_widget* c_gui_widget::create_group_widget(s_group_widget_definition const* definition)
{
	return __vftable->create_group_widget(this, definition);
}

c_gui_list_item_widget* c_gui_widget::create_list_item_widget(s_list_item_widget_block const* definition)
{
	return __vftable->create_list_item_widget(this, definition);
}

c_gui_list_widget* c_gui_widget::create_list_widget(s_list_widget_block const* definition)
{
	return __vftable->create_list_widget(this, definition);
}

c_gui_model_widget* c_gui_widget::create_model_widget(s_model_widget_block const* definition)
{
	return __vftable->create_model_widget(this, definition);
}

//.text:00AB83E0 ; public: virtual c_gui_text_widget* __cdecl c_gui_widget::create_text_widget(s_runtime_text_widget_definition const*)
c_gui_text_widget* c_gui_widget::create_text_widget(s_runtime_text_widget_definition const* definition)
{
	return __vftable->create_text_widget(this, definition);
}

//.text:00AB8590 ; public: void __cdecl c_gui_widget::delete_all_children()

//.text:00AB8620 ; public: virtual void __cdecl c_gui_widget::dispose()
void c_gui_widget::dispose()
{
	__vftable->dispose(this);
}

//.text:00AB8710 ; 

//.text:00AB8720 ; protected: virtual e_animation_state __cdecl c_gui_widget::get_ambient_state()
long c_gui_widget::get_ambient_state()
{
	return __vftable->get_ambient_state(this);
}

//.text:00AB8810 ; 
//.text:00AB8820 ; private: static long __cdecl c_gui_widget::get_animation_period(s_widget_component_animation_definition const*, long)
//.text:00AB8850 ; private: unsigned long __cdecl c_gui_widget::get_animation_state_timer(e_animation_state)
//.text:00AB8900 ; public: e_controller_index __cdecl c_gui_widget::get_arbitrary_responding_controller() const
//.text:00AB8A10 ; public: real_rectangle2d* __cdecl c_gui_widget::get_authored_bounds(real_rectangle2d*)

//.text:00AB8A40 ; public: c_gui_bitmap_widget* __cdecl c_gui_widget::get_child_bitmap_widget(long)
c_gui_bitmap_widget* c_gui_widget::get_child_bitmap_widget(long name)
{
	return (c_gui_bitmap_widget*)get_child_widget(_gui_bitmap, name);
}

//.text:00AB8A60 ; public: c_gui_group_widget* __cdecl c_gui_widget::get_child_group_widget(long)
c_gui_group_widget* c_gui_widget::get_child_group_widget(long name)
{
	return (c_gui_group_widget*)get_child_widget(_gui_group, name);
}

//.text:00AB8A80 ; public: c_gui_list_item_widget* __cdecl c_gui_widget::get_child_list_item_widget(long)
c_gui_list_item_widget* c_gui_widget::get_child_list_item_widget(long name)
{
	return (c_gui_list_item_widget*)get_child_widget(_gui_list_item, name);
}

//.text:00AB8AA0 ; public: c_gui_list_widget* __cdecl c_gui_widget::get_child_list_widget(long)
c_gui_list_widget* c_gui_widget::get_child_list_widget(long name)
{
	return (c_gui_list_widget*)get_child_widget(_gui_list, name);
}

//.text:00AB8AC0 ; public: c_gui_model_widget* __cdecl c_gui_widget::get_child_model_widget(long)
c_gui_model_widget* c_gui_widget::get_child_model_widget(long name)
{
	return (c_gui_model_widget*)get_child_widget(_gui_model, name);
}

//.text:00AB8AE0 ; public: c_gui_text_widget* __cdecl c_gui_widget::get_child_text_widget(long)
c_gui_text_widget* c_gui_widget::get_child_text_widget(long name)
{
	return (c_gui_text_widget*)get_child_widget(_gui_text, name);
}

//.text:00AB8B00 ; public: c_gui_widget* __cdecl c_gui_widget::get_child_widget(e_gui_widget_type, long)
c_gui_widget* c_gui_widget::get_child_widget(e_gui_widget_type type, long name)
{
	return INVOKE_CLASS_MEMBER(0x00AB8B00, c_gui_widget, get_child_widget, type, name);
}

//.text:00AB8BC0 ; public: virtual real_rectangle2d* __cdecl c_gui_widget::get_container_current_bounds(real_rectangle2d*)
real_rectangle2d* c_gui_widget::get_container_current_bounds(real_rectangle2d* unanimated_bounds)
{
	return __vftable->get_container_current_bounds(this, unanimated_bounds);
}

//.text:00AB8CA0 ; protected: long __cdecl c_gui_widget::get_controller_mask() const

//.text:00AB8CB0 ; public: virtual real_argb_color* __cdecl c_gui_widget::get_cumulative_color_tint(real_argb_color*)
real_argb_color* c_gui_widget::get_cumulative_color_tint(real_argb_color* tint)
{
	return __vftable->get_cumulative_color_tint(this, tint);
}

//.text:00AB8CE0 ; public: virtual real_rectangle2d* __cdecl c_gui_widget::get_current_bounds(real_rectangle2d*)
real_rectangle2d* c_gui_widget::get_current_bounds(real_rectangle2d* unanimated_bounds)
{
	return __vftable->get_current_bounds(this, unanimated_bounds);
}


//.text:00AB8D10 ; public: virtual long __cdecl c_gui_widget::get_datasource_index()
long c_gui_widget::get_datasource_index()
{
	return __vftable->get_datasource_index(this);
}

//.text:00AB8D30 ; public: real_argb_color const* __cdecl c_gui_widget::get_debug_color()
//.text:00AB8DA0 ; public: c_gui_widget* __cdecl c_gui_widget::get_deepest_widget_that_can_receive_focus()
//.text:00AB8E20 ; public: long __cdecl c_gui_widget::get_definition_index(void)
//.text:00AB8EB0 ; public: e_controller_index __cdecl c_gui_widget::get_driving_controller() const

//.text:00AB8F30 ; public: virtual long __cdecl c_gui_widget::get_element_handle()
long c_gui_widget::get_element_handle()
{
	return __vftable->get_element_handle(this);
}

//.text:00AB8F50 ; public: long __cdecl c_gui_widget::get_element_index()

//.text:00AB8F80 ; public: c_gui_widget* __cdecl c_gui_widget::get_first_child_widget_by_type(e_gui_widget_type)
c_gui_widget* c_gui_widget::get_first_child_widget_by_type(e_gui_widget_type type)
{
	return INVOKE_CLASS_MEMBER(0x00AB8F80, c_gui_widget, get_first_child_widget_by_type, type);
}

//.text:00AB8FE0 ; 
//.text:00AB8FF0 ; 
//.text:00AB9010 ; public: c_gui_widget* __cdecl c_gui_widget::get_last_child_widget_by_type(e_gui_widget_type)
//.text:00AB9080 ; 
//.text:00AB90C0 ; 
//.text:00AB9100 ; 

c_gui_bitmap_widget* c_gui_widget::get_next_bitmap_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB9220, c_gui_widget, get_next_bitmap_widget);

	return (c_gui_bitmap_widget*)get_next_widget_of_type(_gui_bitmap);
}

c_gui_list_item_widget* c_gui_widget::get_next_list_item_widget(bool only_consider_valid_items)
{
	return INVOKE_CLASS_MEMBER(0x00AB9230, c_gui_widget, get_next_list_item_widget, only_consider_valid_items);
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

//.text:00AB94B0 ; public: virtual long __cdecl c_gui_widget::get_name() const
long c_gui_widget::get_name()
{
	return __vftable->get_name(this);
}

c_gui_widget* c_gui_widget::get_next()
{
	return INVOKE_CLASS_MEMBER(0x00AB94D0, c_gui_widget, get_next);
}

//.text:00AB9500 ; public: c_gui_bitmap_widget* __cdecl c_gui_widget::get_previous_bitmap_widget()
//.text:00AB9510 ; public: c_gui_list_item_widget* __cdecl c_gui_widget::get_previous_list_item_widget(bool)
//.text:00AB9540 ; public: c_gui_list_widget* __cdecl c_gui_widget::get_previous_list_widget()
//.text:00AB9550 ; public: c_gui_model_widget* __cdecl c_gui_widget::get_previous_model_widget()
//.text:00AB9560 ; public: c_gui_text_widget* __cdecl c_gui_widget::get_previous_text_widget()
//.text:00AB9570 ; public: c_gui_widget* __cdecl c_gui_widget::get_previous_widget_of_type(e_gui_widget_type)
//.text:00AB95D0 ; public: gui_real_rectangle2d* __cdecl c_gui_widget::get_projected_bounds(rectangle2d const*, gui_real_rectangle2d*, bool, bool, bool)
//.text:00AB9610 ; public: short __cdecl c_gui_widget::get_render_depth_bias()
//.text:00AB9620 ; public: bool __cdecl c_gui_widget::get_render_in_screenshot()
//.text:00AB96B0 ; 
//.text:00AB96C0 ; public: e_controller_index __cdecl c_gui_widget::get_single_responding_controller() const
//.text:00AB96D0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<256>::get_string()
//.text:00AB96E0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<48>::get_string()
//.text:00AB96F0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<1024>::get_string()

//.text:00AB9700 ; public: virtual bool __cdecl c_gui_widget::get_string_by_string_id(long, c_static_wchar_string<1024>*)
bool c_gui_widget::get_string_by_string_id(long string_identifier, c_static_wchar_string<1024>* buffer)
{
	return __vftable->get_string_by_string_id(this, string_identifier, buffer);
}

//.text:00AB9710 ; public: virtual unsigned long __cdecl c_gui_sized_text_widget<256>::get_text_buffer_size() const
//.text:00AB9720 ; public: virtual unsigned long __cdecl c_gui_sized_text_widget<48>::get_text_buffer_size() const
//.text:00AB9730 ; public: virtual unsigned long __cdecl c_gui_sized_text_widget<1024>::get_text_buffer_size() const
//.text:00AB9740 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<256>::get_text_internal()
//.text:00AB9750 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<48>::get_text_internal()
//.text:00AB9760 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<1024>::get_text_internal()
//.text:00AB9770 ; 

//.text:00AB97C0 ; public: gui_real_rectangle2d* __cdecl c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d*, bool, bool, bool)
void __thiscall c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	HOOK_INVOKE_CLASS_MEMBER(, c_gui_widget, get_unprojected_bounds, unprojected_bounds, apply_translation, apply_scale, apply_rotation);

	// this is more or less what Halo 3 MCC is doing

	rectangle2d display_pixel_bounds;
	interface_get_current_display_settings(NULL, NULL, &display_pixel_bounds, NULL);

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

//.text:00AB9870 ; 
//.text:00AB9880 ; 
//.text:00AB9920 ; public: bool __cdecl c_gui_widget::get_visible() const
//.text:00AB9930 ; public: s_widget_animation_definition const* __cdecl c_gui_widget::get_widget_animation_definition(e_animation_state)

//.text:00AB9980 ; public: virtual bool __cdecl c_gui_widget::handle_alt_tab(c_controller_input_message const*)
bool c_gui_widget::handle_alt_tab(c_controller_input_message const* message)
{
	return __vftable->handle_alt_tab(this, message);
}

//.text:00AB99E0 ; 
//.text:00AB9A40 ; 
//.text:00AB9AA0 ; public: void __cdecl c_gui_widget::set_children(c_gui_widget*)
//.text:00AB9B40 ; public: virtual bool __cdecl c_gui_widget::handle_tab(c_controller_input_message const*)
//.text:00AB9BA0 ; public: virtual bool __cdecl c_gui_widget::handle_widget_back_out()
bool c_gui_widget::handle_widget_back_out()
{
	return __vftable->handle_widget_back_out(this);
}

//.text:00AB9BB0 ; public: virtual bool __cdecl c_gui_widget::handle_widget_selected()
bool c_gui_widget::handle_widget_selected()
{
	return __vftable->handle_widget_selected(this);
}

//.text:00AB9BC0 ; 

//.text:00AB9BD0 ; public: virtual void __cdecl c_gui_widget::initialize()
void c_gui_widget::initialize()
{
	__vftable->initialize(this);
}

//.text:00AB9C10 ; 
//.text:00AB9C50 ; 
//.text:00AB9C70 ; 
//.text:00AB9CB0 ; 
//.text:00AB9CC0 ; public: bool __cdecl c_gui_widget::leaf_node_of_widget(c_gui_widget*)
//.text:00AB9D40 ; 
//.text:00AB9D50 ; 
//.text:00AB9D60 ; 
//.text:00AB9D70 ; public: void __cdecl c_gui_widget::modulate_tint_color(real_argb_color const*)

//.text:00AB9DD0 ; public: virtual void __cdecl c_gui_widget::post_initialize()
void c_gui_widget::post_initialize()
{
	__vftable->post_initialize(this);
}

//.text:00AB9E40 ; public: void __cdecl c_gui_widget::remove_child_widget(c_gui_widget*)
//.text:00AB9F40 ; public: static void __cdecl c_gui_widget::render(long, s_gui_widget_render_data const*, rectangle2d const*, bool)
//.text:00AB9F90 ; 
//.text:00AB9FA0 ; 

//.text:00AB9FD0 ; public: virtual void __cdecl c_gui_widget::set_animated_state_baseline(s_animation_transform*)
void c_gui_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	__vftable->set_animated_state_baseline(this, transform);
}

//.text:00ABA070 ; 
//.text:00ABA120 ; 
//.text:00ABA1E0 ; public: void __cdecl c_gui_widget::set_child_bitmap_sprite_frame(long, long)
//.text:00ABA200 ; public: void __cdecl c_gui_widget::set_child_enabled(e_gui_widget_type, long, bool)
//.text:00ABA220 ; public: void __cdecl c_gui_widget::set_child_use_alternate_ambient_state(e_gui_widget_type, long, bool)
//.text:00ABA240 ; public: void __cdecl c_gui_widget::set_child_visible(e_gui_widget_type, long, bool)
//.text:00ABA260 ; 
//.text:00ABA270 ; 
//.text:00ABA280 ; public: void __cdecl c_gui_widget::set_driving_controller(e_controller_index)

//.text:00ABA2D0 ; public: virtual void __cdecl c_gui_widget::set_enabled(bool)
void c_gui_widget::set_enabled(bool value)
{
	return __vftable->set_enabled(this, value);
}

//.text:00ABA340 ; public: void __cdecl c_gui_widget::set_full_animation_state(const*, bool)
//.text:00ABA3C0 ; 
//.text:00ABA3D0 ; 
//.text:00ABA3E0 ; 
//.text:00ABA3F0 ; 
//.text:00ABA420 ; 
//.text:00ABA450 ; public: virtual void __cdecl c_sized_user_interface_text<256>::set_string(wchar_t const*, bool, long)
//.text:00ABA550 ; public: virtual void __cdecl c_sized_user_interface_text<48>::set_string(wchar_t const*, bool, long)
//.text:00ABA630 ; public: virtual void __cdecl c_sized_user_interface_text<1024>::set_string(wchar_t const*, bool, long)
//.text:00ABA730 ; 

void c_gui_widget::set_use_alternate_ambient_state(bool value)
{
	INVOKE_CLASS_MEMBER(0x00ABA760, c_gui_widget, set_use_alternate_ambient_state, value);
}

//.text:00ABA7D0 ; 

void c_gui_widget::set_visible(bool value)
{
	INVOKE_CLASS_MEMBER(0x00ABA7E0, c_gui_widget, set_visible, value);
}

e_controller_index c_gui_widget::get_any_responding_controller() const
{
	return INVOKE_CLASS_MEMBER(0x00AB8900, c_gui_widget, get_any_responding_controller);
}

