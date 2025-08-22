#include "interface/c_gui_text_widget.hpp"

#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B18A10, c_gui_text_widget, assemble_render_data_);
HOOK_DECLARE_CLASS_MEMBER(0x00B19220, c_gui_text_widget, set_string_from_string_id_);

void __thiscall c_gui_text_widget::assemble_render_data_(
	s_gui_widget_render_data* render_data,
	const rectangle2d* window_bounds,
	e_controller_index local_controller_index,
	bool apply_translation,
	bool apply_scale,
	bool apply_rotation)
{
	c_gui_text_widget::assemble_render_data(
		render_data,
		window_bounds,
		local_controller_index,
		apply_translation,
		apply_scale,
		apply_rotation);
}

bool __thiscall c_gui_text_widget::set_string_from_string_id_(c_static_wchar_string<1024>* buffer, c_gui_widget* string_list_owner, int32 value_name, bool forcibly_set_text)
{
	return c_gui_text_widget::set_string_from_string_id(buffer, string_list_owner, value_name, forcibly_set_text);
}

//.text:00B18810 ; public: c_gui_text_widget::c_gui_text_widget()

c_gui_text_widget::s_text_source_data::s_text_source_data() :
	type(_source_invalid),
	name(_string_id_invalid),
	element_handle(NONE)
{
	//DECLFUNC(0x00B18850, void, __thiscall, c_gui_text_widget::s_text_source_data*)(this);
}

//.text:00B18870 ; public: virtual void * __cdecl c_gui_text_widget::`vector deleting destructor'(unsigned int)
//.text:00B188A0 ; public: static void __cdecl c_gui_text_widget::add_definition_fields(const s_text_widget_definition*, s_runtime_text_widget_definition*, real_rectangle2d*, bool)
//.text:00B189C0 ; public: static void __cdecl c_gui_text_widget::assemble_definition(const s_text_widget_block*, s_runtime_text_widget_definition*, real_rectangle2d*)

void c_gui_text_widget::assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00B18A10, c_gui_text_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	s_gui_text_widget_extra_large_render_data* text_render_data = (s_gui_text_widget_extra_large_render_data*)render_data;

	c_gui_widget::assemble_render_data(
		render_data,
		window_bounds,
		local_controller_index,
		apply_translation,
		apply_scale,
		apply_rotation);

	c_user_interface_text* text_internal = get_text_internal();

	real_argb_color tint_color = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (TEST_BIT(m_flags, 5))
	{
		tint_color = *c_gui_widget::get_debug_color();
	}
	else
	{
		get_cumulative_color_tint(&tint_color);
	}

	real_argb_color shadow_color = { 1.0f, 0.0f, 0.0f, 0.0f };
	const s_user_interface_shared_globals* shared_globals = user_interface_shared_tag_globals_try_and_get();
	if (shared_globals && shared_globals->default_text_shadow_color.alpha > 0.0f)
	{
		shadow_color = shared_globals->default_text_shadow_color;
	}
	if (text_internal->m_drop_shadow_style)
	{
		shadow_color.alpha = tint_color.alpha * shadow_color.alpha;
	}
	else
	{
		shadow_color.alpha = 0.0f;
	}

	text_render_data->font = text_internal->m_font;
	text_render_data->style = text_internal->m_text_style;
	text_render_data->justification = text_internal->m_justification;
	text_render_data->drop_shadow_style = text_internal->m_drop_shadow_style;
	text_render_data->color = real_argb_color_to_pixel32(&tint_color);
	text_render_data->shadow_color = real_argb_color_to_pixel32(&shadow_color);
	text_render_data->wrap_horizontally = TEST_BIT(text_internal->m_flags, 2);
	text_render_data->align_vertically = TEST_BIT(text_internal->m_flags, 1);
	text_render_data->tab_stop_count = 16;
	text_render_data->rotation_angle_radians = m_animated_state.rotation_angle_radians;

	int16 tab_stop_count = (int16)text_internal->m_tab_stop_count;
	if (tab_stop_count > text_render_data->tab_stop_count)
	{
		tab_stop_count = text_render_data->tab_stop_count;
	}
	text_render_data->tab_stop_count = tab_stop_count;

	if (tab_stop_count > 0)
	{
		csmemcpy(text_render_data->tab_stops, text_internal->m_tab_stops, sizeof(int16) * tab_stop_count);
	}

	c_gui_text_widget::compute_text_bounds(
		window_bounds,
		m_animated_state.local_rotation_origin,
		m_animated_state.position.x,
		m_animated_state.position.y,
		m_animated_state.position.z,
		m_animated_state.scale.i,
		m_animated_state.scale.j,
		&text_render_data->rotation_origin,
		&text_render_data->bounds_rect,
		&text_render_data->clip_rect,
		&text_render_data->glyph_scale);

	ustrnzcpy(text_render_data->text, get_text(), get_text_buffer_size());
	if (TEST_BIT(get_text_internal()->m_flags, 0))
	{
		ustrnupr(text_render_data->text, NUMBEROF(text_render_data->text));
	}
}

void c_gui_text_widget::compute_text_bounds(const rectangle2d* window_bounds, real_point2d rotation_origin, real32 position_x, real32 position_y, real32 depth, real32 scale_i, real32 scale_j, real_point2d* out_rotation_origin, real_rectangle2d* out_transformed_text_rect, real_rectangle2d* out_transformed_clip_rect, real32* out_glyph_scale)
{
	INVOKE_CLASS_MEMBER(0x00B18C60, c_gui_text_widget, compute_text_bounds, window_bounds, rotation_origin, position_x, position_y, depth, scale_i, scale_j, out_rotation_origin, out_transformed_text_rect, out_transformed_clip_rect, out_glyph_scale);
}

s_runtime_core_widget_definition* c_gui_text_widget::get_core_definition()
{
	return INVOKE_CLASS_MEMBER(0x00B18EB0, c_gui_text_widget, get_core_definition);
}

real_argb_color* c_gui_text_widget::get_cumulative_color_tint(real_argb_color* tint)
{
	return INVOKE_CLASS_MEMBER(0x00B18EC0, c_gui_text_widget, get_cumulative_color_tint, tint);
}

//.text:00B18F20 ; 
//.text:00B18F40 ; 
//.text:00B18F50 ; public: void c_gui_text_widget::get_tab_stops(int16*, int16*)
//.text:00B190C0 ; 

const wchar_t* c_gui_text_widget::get_text()
{
	return INVOKE_CLASS_MEMBER(0x00B18F90, c_gui_text_widget, get_text);
}

void c_gui_text_widget::initialize(const s_text_widget_block* template_and_override_block)
{
	INVOKE_CLASS_MEMBER(0x00B190D0, c_gui_text_widget, initialize, template_and_override_block);
}

//.text:00B19180 ; 

void c_gui_text_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	INVOKE_CLASS_MEMBER(0x00B191B0, c_gui_text_widget, set_animated_state_baseline, transform);
}

//.text:00B191F0 ; 
//.text:00B19200 ; 

bool c_gui_text_widget::set_string_from_string_id(c_static_wchar_string<1024>* buffer, c_gui_widget* string_list_owner, int32 value_name, bool forcibly_set_text)
{
	//INVOKE_CLASS_MEMBER(0x00B19220, c_gui_text_widget, set_string_from_string_id, buffer, string_list_owner, value_name, forcibly_set_text);

	s_text_source_data source_data{};
	if (value_name == _string_id_invalid || value_name == k_string_id_empty_string)
	{
		return false;
	}

	if (string_list_owner->get_string_by_string_id(value_name, buffer))
	{
		source_data.type = s_text_source_data::_source_string_id;
		source_data.name = value_name;
	}
	else
	{
		buffer->print(L"#%hs#", string_id_get_string_const(value_name));
		source_data.type = s_text_source_data::_source_raw;
	}

	c_gui_text_widget::set_text_internal(buffer->get_string(), &source_data, forcibly_set_text);
	return true;
}

//.text:00B192A0 ; public: void c_gui_text_widget::set_tab_stops(const int16*, int32)
//.text:00B192D0 ; 

void c_gui_text_widget::set_text(const wchar_t* text)
{
	INVOKE_CLASS_MEMBER(0x00B19300, c_gui_text_widget, set_text, text);
}

void c_gui_text_widget::set_text_from_string_id(c_gui_screen_widget* screen, int32 id)
{
	INVOKE_CLASS_MEMBER(0x00B19340, c_gui_text_widget, set_text_from_string_id, screen, id);
}

void c_gui_text_widget::set_text_internal(const wchar_t* string, const s_text_source_data* source_data, bool forcibly_set)
{
	INVOKE_CLASS_MEMBER(0x00B19390, c_gui_text_widget, set_text_internal, string, source_data, forcibly_set);
}

//.text:00B194B0 ; 

void c_gui_text_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B194D0, c_gui_text_widget, update, current_milliseconds);
}

void c_gui_text_widget::update_render_state(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B19A90, c_gui_text_widget, update_render_state, current_milliseconds);
}

bool c_gui_text_widget::within_focus_chain()
{
	return INVOKE_CLASS_MEMBER(0x00B19AE0, c_gui_text_widget, within_focus_chain);
}

