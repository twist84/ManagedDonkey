#include "interface/c_gui_widget.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
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

c_gui_widget::~c_gui_widget()
{
	DECLFUNC(0x00AB61A0, void, __thiscall, c_gui_widget*)(this);
}

//.text:00AB63A0 ; public: virtual void* c_gui_widget::`scalar deleting destructor'(unsigned int)
//.text:00AB63D0 ; public: void c_gui_widget::add_child_widget(c_gui_widget*)
//.text:00AB64B0 ; public: static void __cdecl c_gui_widget::add_definition_fields(e_gui_widget_type, s_core_widget_definition const*, s_runtime_core_widget_definition*, real_rectangle2d const*, bool)
//.text:00AB6680 ; private: void c_gui_widget::animate(dword)
//.text:00AB6C20 ; protected: void c_gui_widget::animate_recursively(dword)
//.text:00AB6C90 ; void __cdecl apply_color_animation(s_animation_transform*, real const, bool const, s_color_keyframe_block const*, s_color_keyframe_block const*, c_function_definition const*)
//.text:00AB6D40 ; void __cdecl apply_font_animation(s_animation_transform*, real const, bool const, s_font_keyframe_block const*, s_font_keyframe_block const*, c_function_definition const*)
//.text:00AB6D90 ; void __cdecl apply_position_animation(s_animation_transform*, real const, bool const, real_vector2d const*, s_position_keyframe_block const*, s_position_keyframe_block const*, c_function_definition const*)
//.text:00AB6E80 ; 
//.text:00AB6ED0 ; void __cdecl apply_rotation_animation(c_gui_widget*, s_animation_transform*, real const, bool const, s_rotation_keyframe_block const*, s_rotation_keyframe_block const*, c_function_definition const*);
//.text:00AB7050 ; void __cdecl apply_scale_animation(c_gui_widget*, s_animation_transform*, real const, bool const, s_scale_keyframe_block const*, s_scale_keyframe_block const*, c_function_definition const*)
//.text:00AB71C0 ; void __cdecl apply_sprite_animation(s_animation_transform*, real const, bool const, s_sprite_keyframe_block const*, s_sprite_keyframe_block const*, c_function_definition const*)
//.text:00AB7290 ; void __cdecl apply_texture_coordinate_animation(s_animation_transform*, real const, bool const, s_texture_coordinate_keyframe_block const*, s_texture_coordinate_keyframe_block const*, c_function_definition const*)

void c_gui_widget::assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	INVOKE_CLASS_MEMBER(0x00AB7330, c_gui_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	ASSERT(render_data != NULL);
	ASSERT(window_bounds != NULL);

	render_data->type = m_type;

	real_rectangle2d current_bounds{};
	get_current_bounds(&current_bounds);
	get_projected_bounds(window_bounds, &render_data->projected_bounds, apply_translation, apply_scale, apply_rotation);

	render_data->local_controller_index = local_controller_index;
	render_data->flags.clear();
	render_data->flags.set(s_gui_widget_render_data::_render_in_screenshot_bit, get_render_in_screenshot());
}

void c_gui_widget::calculate_animation_transform(e_animation_state animation_state, long start_time_milliseconds, long current_time_milliseconds, s_widget_animation_definition const* animation, s_animation_transform* transform, real_vector2d const* aspect_ratio_scale, bool initialize, bool combinative, bool* finished)
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
//.text:00AB7AC0 ; public: void c_gui_widget::create_and_add_child_list_item_widgets(s_tag_block const*, long)
//.text:00AB7B80 ; public: c_gui_list_widget* c_gui_widget::create_and_add_child_list_widget(s_list_widget_block const*)
//.text:00AB7BC0 ; public: c_gui_model_widget* c_gui_widget::create_and_add_child_model_widget(s_model_widget_block const*)
//.text:00AB7C00 ; public: c_gui_text_widget* c_gui_widget::create_and_add_child_text_widget(s_text_widget_block const*)
//.text:00AB7C90 ; public: void c_gui_widget::create_and_add_children_from_skin_definition(s_gui_skin_definition const*, long, short)
//.text:00AB8160 ; public: c_gui_group_widget* c_gui_widget::create_and_add_group(s_group_widget_block const*)

c_gui_bitmap_widget* c_gui_widget::create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB81A0, c_gui_widget, create_bitmap_widget, definition);
}

c_gui_button_key_widget* c_gui_widget::create_button_key_widget(s_button_key_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8200, c_gui_widget, create_button_key_widget, definition);
}

c_gui_group_widget* c_gui_widget::create_group_widget(s_group_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8260, c_gui_widget, create_group_widget, definition);
}

c_gui_list_item_widget* c_gui_widget::create_list_item_widget(s_list_item_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB82C0, c_gui_widget, create_list_item_widget, definition);
}

c_gui_list_widget* c_gui_widget::create_list_widget(s_list_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8320, c_gui_widget, create_list_widget, definition);
}

c_gui_model_widget* c_gui_widget::create_model_widget(s_model_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB8380, c_gui_widget, create_model_widget, definition);
}

c_gui_text_widget* c_gui_widget::create_text_widget(s_runtime_text_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00AB83E0, c_gui_widget, create_text_widget, definition);
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
//.text:00AB8820 ; private: static long __cdecl c_gui_widget::get_animation_period(s_widget_component_animation_definition const*, long)
//.text:00AB8850 ; private: dword c_gui_widget::get_animation_state_timer(e_animation_state)

e_controller_index c_gui_widget::get_arbitrary_responding_controller() const
{
	return INVOKE_CLASS_MEMBER(0x00AB8900, c_gui_widget, get_arbitrary_responding_controller);
}

real_rectangle2d* c_gui_widget::get_authored_bounds(real_rectangle2d* unanimated_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00AB8A10, c_gui_widget, get_authored_bounds, unanimated_bounds);
}

c_gui_bitmap_widget* c_gui_widget::get_child_bitmap_widget(long name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A40, c_gui_widget, get_child_bitmap_widget, name);

	return (c_gui_bitmap_widget*)get_child_widget(_gui_bitmap, name);
}

c_gui_group_widget* c_gui_widget::get_child_group_widget(long name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A60, c_gui_widget, get_child_group_widget, name);

	return (c_gui_group_widget*)get_child_widget(_gui_group, name);
}

c_gui_list_item_widget* c_gui_widget::get_child_list_item_widget(long name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8A80, c_gui_widget, get_child_list_item_widget, name);

	return (c_gui_list_item_widget*)get_child_widget(_gui_list_item, name);
}

c_gui_list_widget* c_gui_widget::get_child_list_widget(long name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8AA0, c_gui_widget, get_child_list_widget, name);

	return (c_gui_list_widget*)get_child_widget(_gui_list, name);
}

c_gui_model_widget* c_gui_widget::get_child_model_widget(long name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8AC0, c_gui_widget, get_child_model_widget, name);

	return (c_gui_model_widget*)get_child_widget(_gui_model, name);
}

c_gui_text_widget* c_gui_widget::get_child_text_widget(long name)
{
	//return INVOKE_CLASS_MEMBER(0x00AB8AE0, c_gui_widget, get_child_text_widget, name);

	return (c_gui_text_widget*)get_child_widget(_gui_text, name);
}

c_gui_widget* c_gui_widget::get_child_widget(e_gui_widget_type type, long name)
{
	return INVOKE_CLASS_MEMBER(0x00AB8B00, c_gui_widget, get_child_widget, type, name);
}

real_rectangle2d* c_gui_widget::get_container_current_bounds(real_rectangle2d* unanimated_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00AB8BC0, c_gui_widget, get_container_current_bounds, unanimated_bounds);
}

long c_gui_widget::get_controller_mask() const
{
	return INVOKE_CLASS_MEMBER(0x00AB8CA0, c_gui_widget, get_controller_mask);
}

real_argb_color* c_gui_widget::get_cumulative_color_tint(real_argb_color* tint)
{
	return INVOKE_CLASS_MEMBER(0x00AB8CB0, c_gui_widget, get_cumulative_color_tint, tint);
}

real_rectangle2d* c_gui_widget::get_current_bounds(real_rectangle2d* unanimated_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00AB8CE0, c_gui_widget, get_current_bounds, unanimated_bounds);
}

long c_gui_widget::get_datasource_index()
{
	return INVOKE_CLASS_MEMBER(0x00AB8D10, c_gui_widget, get_datasource_index);
}

real_argb_color const* c_gui_widget::get_debug_color()
{
	return INVOKE_CLASS_MEMBER(0x00AB8D30, c_gui_widget, get_debug_color);
}

c_gui_widget* c_gui_widget::get_deepest_widget_that_can_receive_focus()
{
	return INVOKE_CLASS_MEMBER(0x00AB8DA0, c_gui_widget, get_deepest_widget_that_can_receive_focus);
}

long c_gui_widget::get_definition_index()
{
	return INVOKE_CLASS_MEMBER(0x00AB8E20, c_gui_widget, get_definition_index);
}

e_controller_index c_gui_widget::get_driving_controller() const
{
	return INVOKE_CLASS_MEMBER(0x00AB8EB0, c_gui_widget, get_driving_controller);
}

long c_gui_widget::get_element_handle()
{
	return INVOKE_CLASS_MEMBER(0x00AB8F30, c_gui_widget, get_element_handle);
}

long c_gui_widget::get_element_index()
{
	return INVOKE_CLASS_MEMBER(0x00AB8F50, c_gui_widget, get_element_index);
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

long c_gui_widget::get_name() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB94B0, c_gui_widget, get_name);

	return m_name.get_value();
}

c_gui_widget* c_gui_widget::get_previous()
{
	return INVOKE_CLASS_MEMBER(0x00AB94D0, c_gui_widget, get_previous);
}

c_gui_bitmap_widget* c_gui_widget::get_previous_bitmap_widget()
{
	return INVOKE_CLASS_MEMBER(0x00AB9500, c_gui_widget, get_previous_bitmap_widget);
}

c_gui_list_item_widget* c_gui_widget::get_previous_list_item_widget(bool only_consider_valid_items)
{
	return INVOKE_CLASS_MEMBER(0x00AB9510, c_gui_widget, get_previous_list_item_widget, only_consider_valid_items);
}

c_gui_list_widget* c_gui_widget::get_previous_list_widget()
{
	return INVOKE_CLASS_MEMBER(0x00AB9540, c_gui_widget, get_previous_list_widget);
}

c_gui_model_widget* c_gui_widget::get_previous_model_widget()
{
	return INVOKE_CLASS_MEMBER(0x00AB9550, c_gui_widget, get_previous_model_widget);
}

c_gui_text_widget* c_gui_widget::get_previous_text_widget()
{
	return INVOKE_CLASS_MEMBER(0x00AB9560, c_gui_widget, get_previous_text_widget);
}

c_gui_widget* c_gui_widget::get_previous_widget_of_type(e_gui_widget_type type)
{
	return INVOKE_CLASS_MEMBER(0x00AB9570, c_gui_widget, get_previous_widget_of_type, type);
}


gui_real_rectangle2d* c_gui_widget::get_projected_bounds(rectangle2d const* window_bounds, gui_real_rectangle2d* projected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	return INVOKE_CLASS_MEMBER(0x00AB95D0, c_gui_widget, get_projected_bounds, window_bounds, projected_bounds, apply_translation, apply_scale, apply_rotation);
}

//.text:00AB9610 ; public: short c_gui_widget::get_render_depth_bias()

bool c_gui_widget::get_render_in_screenshot()
{
	return INVOKE_CLASS_MEMBER(0x00AB9620, c_gui_widget, get_render_in_screenshot);
}

//.text:00AB96B0 ; 

e_controller_index c_gui_widget::get_single_responding_controller() const
{
	return INVOKE_CLASS_MEMBER(0x00AB96C0, c_gui_widget, get_single_responding_controller);
}

//.text:00AB96D0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<256>::get_string()
//.text:00AB96E0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<48>::get_string()
//.text:00AB96F0 ; public: virtual wchar_t const* __cdecl c_sized_user_interface_text<1024>::get_string()

bool c_gui_widget::get_string_by_string_id(long string_identifier, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00AB9700, c_gui_widget, get_string_by_string_id, string_identifier, buffer);
}

//.text:00AB9710 ; public: virtual unsigned long __cdecl c_gui_sized_text_widget<256>::get_text_buffer_size() const
//.text:00AB9720 ; public: virtual unsigned long __cdecl c_gui_sized_text_widget<48>::get_text_buffer_size() const
//.text:00AB9730 ; public: virtual unsigned long __cdecl c_gui_sized_text_widget<1024>::get_text_buffer_size() const
//.text:00AB9740 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<256>::get_text_internal()
//.text:00AB9750 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<48>::get_text_internal()
//.text:00AB9760 ; private: virtual c_user_interface_text* __cdecl c_gui_sized_text_widget<1024>::get_text_internal()
//.text:00AB9770 ; 

//.text:00AB97C0 ; public: gui_real_rectangle2d* c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d*, bool, bool, bool)
void __thiscall c_gui_widget::get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//HOOK_INVOKE_CLASS_MEMBER(, c_gui_widget, get_unprojected_bounds, unprojected_bounds, apply_translation, apply_scale, apply_rotation);

	real_rectangle2d authored_bounds{};
	get_current_bounds(&authored_bounds);
	unprojected_bounds->set(&authored_bounds);

	if (apply_scale)
		unprojected_bounds->scale_about_local_point(&m_animated_state.local_scale_origin, &m_animated_state.scale);

	if (apply_rotation)
		unprojected_bounds->rotate_about_local_point(&m_animated_state.local_rotation_origin, m_animated_state.sine_rotation_angle, m_animated_state.cosine_rotation_angle);

	if (apply_translation)
		unprojected_bounds->offset(m_animated_state.position.x, m_animated_state.position.y);

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
	return INVOKE_CLASS_MEMBER(0x00AB9980, c_gui_widget, handle_alt_stick, message);
}

bool c_gui_widget::handle_alt_tab(c_controller_input_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB99E0, c_gui_widget, handle_alt_tab, message);
}

bool c_gui_widget::handle_controller_input_message(c_controller_input_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB9A40, c_gui_widget, handle_alt_tab, message);
}

//.text:00AB9AA0 ; c_gui_widget::handle_?

bool c_gui_widget::handle_tab(c_controller_input_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB9B40, c_gui_widget, handle_tab, message);
}

bool c_gui_widget::handle_widget_back_out()
{
	return INVOKE_CLASS_MEMBER(0x00AB9BA0, c_gui_widget, handle_widget_back_out);
}

bool c_gui_widget::handle_widget_selected()
{
	return INVOKE_CLASS_MEMBER(0x00AB9BB0, c_gui_widget, handle_widget_selected);
}

//.text:00AB9BC0 ; 

void c_gui_widget::initialize()
{
	INVOKE_CLASS_MEMBER(0x00AB9BD0, c_gui_widget, initialize);
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
	return INVOKE_CLASS_MEMBER(0x00AB9CC0, c_gui_widget, leaf_node_of_widget, branch_widget);
}

//.text:00AB9D40 ; 
//.text:00AB9D50 ; 
//.text:00AB9D60 ; 

void c_gui_widget::modulate_tint_color(real_argb_color const* modulation)
{
	INVOKE_CLASS_MEMBER(0x00AB9D70, c_gui_widget, modulate_tint_color, modulation);
}

void c_gui_widget::post_initialize()
{
	INVOKE_CLASS_MEMBER(0x00AB9DD0, c_gui_widget, post_initialize);
}

void c_gui_widget::remove_child_widget(c_gui_widget* child)
{
	INVOKE_CLASS_MEMBER(0x00AB9E40, c_gui_widget, remove_child_widget, child);
}

void c_gui_widget::render(long user_index, s_gui_widget_render_data const* render_data, rectangle2d const* window_bounds, bool is_screenshot)
{
	INVOKE(0x00AB9F40, c_gui_widget::render, user_index, render_data, window_bounds, is_screenshot);
}

//.text:00AB9F90 ; 
//.text:00AB9FA0 ; 

void c_gui_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	INVOKE_CLASS_MEMBER(0x00AB9FD0, c_gui_widget, set_animated_state_baseline, transform);
}

//.text:00ABA070 ; 
//.text:00ABA120 ; 

void c_gui_widget::set_child_bitmap_sprite_frame(long widget_name, long sprite_frame_index)
{
	INVOKE_CLASS_MEMBER(0x00ABA1E0, c_gui_widget, set_child_bitmap_sprite_frame, widget_name, sprite_frame_index);
}

void c_gui_widget::set_child_enabled(e_gui_widget_type widget_type, long widget_name, bool enabled)
{
	INVOKE_CLASS_MEMBER(0x00ABA200, c_gui_widget, set_child_enabled, widget_type, widget_name, enabled);
}

void c_gui_widget::set_child_use_alternate_ambient_state(e_gui_widget_type widget_type, long widget_name, bool value)
{
	INVOKE_CLASS_MEMBER(0x00ABA220, c_gui_widget, set_child_use_alternate_ambient_state, widget_type, widget_name, value);
}

void c_gui_widget::set_child_visible(e_gui_widget_type widget_type, long widget_name, bool visible)
{
	INVOKE_CLASS_MEMBER(0x00ABA240, c_gui_widget, set_child_visible, widget_type, widget_name, visible);
}

void c_gui_widget::set_children(c_gui_widget* children)
{
	INVOKE_CLASS_MEMBER(0x00ABA260, c_gui_widget, set_children, children);
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
	INVOKE_CLASS_MEMBER(0x00ABA3C0, c_gui_widget, set_next, next);
}

void c_gui_widget::set_parent(c_gui_widget* parent)
{
	INVOKE_CLASS_MEMBER(0x00ABA3D0, c_gui_widget, set_parent, parent);
}

void c_gui_widget::set_previous(c_gui_widget* previous)
{
	INVOKE_CLASS_MEMBER(0x00ABA3E0, c_gui_widget, set_previous, previous);
}

//.text:00ABA3F0 ; 
//.text:00ABA420 ; 
//.text:00ABA450 ; public: virtual void __cdecl c_sized_user_interface_text<256>::set_string(wchar_t const*, bool, long)
//.text:00ABA550 ; public: virtual void __cdecl c_sized_user_interface_text<48>::set_string(wchar_t const*, bool, long)
//.text:00ABA630 ; public: virtual void __cdecl c_sized_user_interface_text<1024>::set_string(wchar_t const*, bool, long)

void c_gui_widget::set_tint_color_direct(real_argb_color const* color)
{
	INVOKE_CLASS_MEMBER(0x00ABA730, c_gui_widget, set_tint_color_direct, color);
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
	return INVOKE_CLASS_MEMBER(0x00ABA850, c_gui_widget, should_render, add_to_render_list);

	//if (add_to_render_list)
	//{
	//	*add_to_render_list = m_type != _gui_text || m_type == _gui_bitmap || m_type == _gui_model;
	//}
	//return m_visible;
}

void c_gui_widget::start_animation(e_animation_state animation_state, bool recursive)
{
	INVOKE_CLASS_MEMBER(0x00ABA890, c_gui_widget, start_animation, animation_state, recursive);
}

void c_gui_widget::start_animation_at_time(e_animation_state animation_state, dword animation_start_time, bool recursive)
{
	INVOKE_CLASS_MEMBER(0x00ABA8B0, c_gui_widget, start_animation_at_time, animation_state, animation_start_time, recursive);
}

void c_gui_widget::stomp_bounds(real_rectangle2d const* new_bounds)
{
	INVOKE_CLASS_MEMBER(0x00ABAB50, c_gui_widget, stomp_bounds, new_bounds);
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

//.text:00ABAD00 ; public: virtual void __cdecl c_sized_user_interface_text<256>::update(long)
//.text:00ABAE30 ; public: virtual void __cdecl c_sized_user_interface_text<48>::update(long)
//.text:00ABAF40 ; public: virtual void __cdecl c_sized_user_interface_text<1024>::update(long)

void c_gui_widget::update(dword current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00ABB070, c_gui_widget, update, current_milliseconds);

	//for (c_gui_widget* child_widget = get_children(); child_widget; child_widget = child_widget->get_next())
	//{
	//	if (child_widget->m_type != _gui_screen)
	//		child_widget->update(current_milliseconds);
	//}
}

void c_gui_widget::update_animation(dword current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00ABB0E0, c_gui_widget, update_animation, current_milliseconds);
}

void c_gui_widget::update_render_state(dword current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00ABB1A0, c_gui_widget, update_render_state, current_milliseconds);

	//if (m_visible)
	//{
	//	for (c_gui_widget* child_widget = get_children(); child_widget; child_widget = child_widget->get_next())
	//	{
	//		if (child_widget->m_type != _gui_screen)
	//			child_widget->update_render_state(current_milliseconds);
	//	}
	//}
}

//.text:00ABB210 ; 

bool c_gui_widget::verify_animation_period(e_animation_state animation_state, long period)
{
	return INVOKE_CLASS_MEMBER(0x00ABB220, c_gui_widget, verify_animation_period, animation_state, period);
}

//.text:00ABB270 ; public: static char const* __cdecl c_gui_widget::widget_name_to_string(long)
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

