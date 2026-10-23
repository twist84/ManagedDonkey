#include "interface/c_gui_text_widget.hpp"

#include "cache/cache_files.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface_constants.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "text/text_group.hpp"
#include "user_interface_utilities.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B18A10, c_gui_text_widget, assemble_render_data_);
HOOK_DECLARE_CLASS_MEMBER(0x00B19220, c_gui_text_widget, set_string_from_string_id_);
HOOK_DECLARE_CLASS_MEMBER(0x00B194D0, c_gui_text_widget, update_);
HOOK_DECLARE_CLASS_MEMBER(0x00B19A90, c_gui_text_widget, update_render_state_);

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

void __thiscall c_gui_text_widget::update_(uns32 current_milliseconds)
{
	c_gui_text_widget::update(current_milliseconds);
}

void __thiscall c_gui_text_widget::update_render_state_(uns32 current_milliseconds)
{
	c_gui_text_widget::update_render_state(current_milliseconds);
}

c_gui_text_widget::c_gui_text_widget() :
	c_gui_widget(_gui_text),
	m_text_source_data(),
	m_definition()
{
	//DECLFUNC(0x00B18810, void, __thiscall, c_gui_text_widget*)(this);
}

c_gui_text_widget::s_text_source_data::s_text_source_data() :
	type(_source_invalid),
	name(_string_id_invalid),
	element_handle(NONE)
{
	//DECLFUNC(0x00B18850, void, __thiscall, c_gui_text_widget::s_text_source_data*)(this);
}

//.text:00B18870 ; public: virtual void * __cdecl c_gui_text_widget::`vector deleting destructor'(unsigned int)

void __cdecl c_gui_text_widget::add_definition_fields(const s_text_widget_definition* source_definition, s_runtime_text_widget_definition* dest_definition, real_rectangle2d const* positioning_bounds, bool was_templated)
{
	//INVOKE(0x00B188A0, c_gui_text_widget::add_definition_fields, source_definition, dest_definition, positioning_bounds, was_templated);

	ASSERT(source_definition != nullptr);
	ASSERT(dest_definition != nullptr);
	ASSERT(positioning_bounds != nullptr);
	
	s_user_interface_shared_globals const* ui_globals = user_interface_shared_tag_globals_try_and_get();
	c_gui_widget::add_definition_fields(_gui_text, source_definition, dest_definition, positioning_bounds, was_templated);
	
	if (source_definition->value_override_list != _string_id_invalid && source_definition->value_override_list != _string_id_empty_string)
	{
		dest_definition->value_override_list = source_definition->value_override_list;
	}
	
	if (source_definition->value_identifier != _string_id_invalid && source_definition->value_identifier != _string_id_empty_string)
	{
		dest_definition->value_identifier = source_definition->value_identifier;
	}
	
	if (ui_globals != nullptr)
	{
		real_argb_color empty_color = real_argb_color();
	
		bool color_is_empty = csmemcmp(&dest_definition->text_color, &empty_color, sizeof(dest_definition->text_color)) == 0;
		if (source_definition->text_color_preset != _string_id_empty_string || color_is_empty)
		{
			if (source_definition->text_color_preset == _string_id_empty_string && ui_globals->color_presets.count > 0)
			{
				dest_definition->text_color = ui_globals->color_presets[0].color;
			}
			else
			{
				for (long color_preset_index = 0; color_preset_index < ui_globals->color_presets.count; color_preset_index++)
				{
					s_color_preset* color_preset = &ui_globals->color_presets[color_preset_index];
					if (source_definition->text_color_preset == color_preset->name)
					{
						dest_definition->text_color = color_preset->color;
						break;
					}
				}
			}
		}
	}
	
	if (source_definition->font > _terminal_font && source_definition->font < k_public_font_id_count)
	{
		dest_definition->font = static_cast<e_font_id>(source_definition->font);
	}
}

void __cdecl c_gui_text_widget::assemble_definition(const s_text_widget_block* template_and_override_block, s_runtime_text_widget_definition* definition, real_rectangle2d* positioning_bounds)
{
	//INVOKE(0x00B189C0, c_gui_text_widget::assemble_definition, template_and_override_block, definition, positioning_bounds);

	bool was_templated = false;

	ASSERT(template_and_override_block != nullptr);
	ASSERT(definition != nullptr);

	s_text_widget_definition const* source_definition;
	if (template_and_override_block->widget_template_reference.index != NONE)
	{
		source_definition = text_widget_definition_get(template_and_override_block->widget_template_reference.index);
		add_definition_fields(source_definition, definition, positioning_bounds, false);

		was_templated = true;
	}
	else
	{
		source_definition = nullptr;
	}

	add_definition_fields(&template_and_override_block->override_definition, definition, positioning_bounds, was_templated);
}

void c_gui_text_widget::assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00B18A10, c_gui_text_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	s_gui_text_widget_extra_large_render_data* text_render_data = static_cast<s_gui_text_widget_extra_large_render_data*>(render_data);

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

bool c_user_interface_text::get_string_was_set() const
{
	return INVOKE_CLASS_MEMBER(0x00B18F20, c_user_interface_text, get_string_was_set);
}

//.text:00B18F40 ; 

void c_gui_text_widget::get_tab_stops(int16* tab_stops, int16* in_out_count)
{
	//INVOKE_CLASS_MEMBER(0x00B18F50, c_gui_text_widget, get_tab_stops, tab_stops, in_out_count);

	get_text_internal()->get_tab_stops(tab_stops, in_out_count);
}

//.text:00B190C0 ; 

const wchar_t* c_gui_text_widget::get_text()
{
	//return INVOKE_CLASS_MEMBER(0x00B18F90, c_gui_text_widget, get_text);

	wchar_t const* text = get_text_internal()->get_string();
	return text;
}

bool c_gui_text_widget::get_text_bounds(c_font_cache_base* font_cache, rectangle2d* bounds)
{
	//return INVOKE_CLASS_MEMBER(0x00B18FA0, c_gui_text_widget, get_text_bounds, font_cache, bounds);

	rectangle2d display_bounds;
	rectangle2d render_bounds;
	interface_get_current_display_settings(&render_bounds, nullptr, &display_bounds, nullptr);

	real_rectangle2d text_bounds;
	compute_text_bounds(&display_bounds,
		m_animated_state.local_rotation_origin,
		m_animated_state.position.x, m_animated_state.position.y, m_animated_state.position.z,
		m_animated_state.scale.i, m_animated_state.scale.j,
		nullptr, &text_bounds, nullptr, nullptr);

	bool result = false;

	if (text_bounds.y1 - text_bounds.y0 > 0.0f && text_bounds.x1 - text_bounds.x0 > 0.0f)
	{
		rectangle2d text_rectangle;
		text_rectangle.n[0] = static_cast<short>(text_bounds.n[2]);
		text_rectangle.n[1] = static_cast<short>(text_bounds.n[0]);
		text_rectangle.n[2] = static_cast<short>(text_bounds.n[3]);
		text_rectangle.n[3] = static_cast<short>(text_bounds.n[1]);
		float depth = static_cast<float>(m_definition.render_depth_bias);

		result = get_text_internal()->compute_text_bounds(font_cache, 0, &text_rectangle, depth, &display_bounds, bounds);
	}

	return result;
}

//.text:00B190C0 ; 

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
	if (value_name == _string_id_invalid || value_name == _string_id_empty_string)
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

void c_gui_text_widget::set_tab_stops(const int16* tab_stops, int32 tab_stop_count)
{
	//INVOKE_CLASS_MEMBER(0x00B192A0, c_gui_text_widget, set_tab_stops, tab_stops, tab_stop_count);

	get_text_internal()->set_tab_stops(tab_stops, tab_stop_count);
}

//.text:00B192D0 ; 

void c_gui_text_widget::set_text(const wchar_t* string)
{
	//INVOKE_CLASS_MEMBER(0x00B19300, c_gui_text_widget, set_text, text);

	if (string != nullptr)
	{
		s_text_source_data source_data;
		source_data.type = s_text_source_data::_source_raw;
		set_text_internal(string, &source_data, true);
	}
}

void c_gui_text_widget::set_text_from_string_id(c_gui_screen_widget* parent_screen, int32 parent_screen_string_id)
{
	//INVOKE_CLASS_MEMBER(0x00B19340, c_gui_text_widget, set_text_from_string_id, parent_screen, parent_screen_string_id);

	c_static_wchar_string<1024> buffer;
	if (parent_screen != nullptr)
	{
		if (parent_screen->get_string_by_string_id(parent_screen_string_id, &buffer))
		{
			set_text(buffer.get_string());
		}
	}
}

void c_gui_text_widget::set_text_internal(const wchar_t* string, const s_text_source_data* source_data, bool forcibly_set)
{
	//INVOKE_CLASS_MEMBER(0x00B19390, c_gui_text_widget, set_text_internal, string, source_data, forcibly_set);

	bool source_dirty = true;
	if (source_data->type == m_text_source_data.type)
	{
		source_dirty = false;
	
		switch (source_data->type)
		{
		case s_text_source_data::_source_invalid:
			break;
	
		case s_text_source_data::_source_raw:
			source_dirty = true;
			break;
	
		case s_text_source_data::_source_string_id:
			source_dirty = source_data->name != m_text_source_data.name;
			break;
	
		case s_text_source_data::_source_exported_data:
			source_dirty = source_data->name != m_text_source_data.name;
			source_dirty = source_dirty || source_data->element_handle != m_text_source_data.element_handle;
			if (!source_dirty)
			{
				c_user_interface_text* internal_text = get_text_internal();
				wchar_t const* temp = internal_text->get_string();
	
				if (ustrcmp(string, temp) == 0)
				{
					source_dirty = false;
				}
				else
				{
					source_dirty = true;
				}
			}
			break;
	
		default:
			UNREACHABLE();
			break;
		}
	}
	
	if (source_dirty || forcibly_set)
	{
		bool string_clipped = false;
		bool parse_xml = false;
	
		c_gui_screen_widget const* screen_widget = get_parent_screen();
		if (screen_widget != nullptr)
		{
			if (string != nullptr)
			{
				wchar_t const* xml_tag_start = ustrchr(string, '<');
	
				wchar_t const* xml_tag_end;
				if (xml_tag_start != nullptr)
				{
					xml_tag_end = ustrchr(xml_tag_start, '>');
				}
				else
				{
					xml_tag_end = nullptr;
				}
	
				parse_xml = xml_tag_end != nullptr;
			}
	
			get_text_internal()->set_string(string, parse_xml, screen_widget->get_screen_index());
	
			if (get_text_internal()->has_overflowed())
			{
				string_clipped = true;
			}
			SET_BIT(m_definition.flags, s_text_widget_definition::_text_clipped_insufficient_buffer_size_bit, string_clipped);
	
			m_text_source_data = *source_data;
		}
	}
}

//.text:00B194B0 ; 

void c_gui_text_widget::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B194D0, c_gui_text_widget, update, current_milliseconds);

	c_gui_widget::update(current_milliseconds);

	if (get_visible())
	{
		c_gui_list_item_widget* parent_list_item = get_parent_list_item();
		c_gui_list_widget* parent_list = get_parent_list();
		s_runtime_core_widget_definition* definition = get_core_definition();

		bool left_justify = TEST_BIT(definition->flags, s_text_widget_definition::_left_justify_text_bit);
		bool right_justify = !left_justify && TEST_BIT(definition->flags, s_text_widget_definition::_right_justify_text_bit);
		bool scrollable = TEST_BIT(definition->flags, s_text_widget_definition::_scrollable_text_bit);
		bool uppercase = TEST_BIT(definition->flags, s_text_widget_definition::_uppercase_text_bit);
		bool use_exported_text = TEST_BIT(definition->flags, s_text_widget_definition::_attach_string_to_exported_text_bit);
		bool use_exported_string_id = TEST_BIT(definition->flags, s_text_widget_definition::_attach_string_to_exported_string_id_bit);
		bool use_exported_global_string_id = TEST_BIT(definition->flags, s_text_widget_definition::_attach_string_to_exported_global_string_id_bit);
		bool use_exported_integer = TEST_BIT(definition->flags, s_text_widget_definition::_attach_string_to_exported_integer_bit);
		bool use_list_item_label = TEST_BIT(definition->flags, s_text_widget_definition::_attach_string_to_parent_list_item_label_bit);

		bool add_brackets_this_frame = TEST_BIT(definition->flags, s_text_widget_definition::_use_backets_to_indicate_focus_bit)
			&& parent_list_item != nullptr
			&& parent_list_item->within_focus_chain()
			&& parent_list != nullptr
			&& parent_list->get_enabled();

		bool wrap_horizontally = !TEST_BIT(definition->flags, s_text_widget_definition::_do_not_wrap_horizontally_bit);
		bool brackets_added_last_frame = TEST_BIT(definition->flags, s_text_widget_definition::_brackets_added_for_focus_current_frame_bit);
		bool string_set = false;

		e_text_justification justification;
		if (left_justify)
		{
			justification = _text_justification_left;
		}
		else if (right_justify)
		{
			justification = _text_justification_right;
		}
		else
		{
			justification = _text_justification_center;
		}

		e_text_drop_shadow_style drop_shadow_style;
		if (TEST_BIT(definition->flags, s_text_widget_definition::_standard_drop_shadow_bit))
		{
			drop_shadow_style = _shadow_drop;
		}
		else if (TEST_BIT(definition->flags, s_text_widget_definition::_no_drop_shadow_bit))
		{
			drop_shadow_style = _shadow_none;
		}
		else
		{
			drop_shadow_style = _shadow_outline;
		}

		get_text_internal()->set_render_uppercase(uppercase);
		get_text_internal()->set_drop_shadow_style(drop_shadow_style);
		get_text_internal()->set_justification(justification);
		get_text_internal()->set_font(m_animated_state.font);
		get_text_internal()->set_wrap_horizontally(wrap_horizontally);

		c_gui_widget* string_list_owner = nullptr;
		if (get_parent() != nullptr)
		{
			if (get_parent()->get_type() == _gui_button_key)
			{
				string_list_owner = get_parent();
			}
			else
			{
				string_list_owner = get_parent_screen();
			}
		}

		if (string_list_owner != nullptr)
		{
			s_text_source_data source_data;
			c_static_wchar_string<1024> buffer;
			bool forcibly_set_text = add_brackets_this_frame || brackets_added_last_frame;

			if (get_debug_name())
			{
				buffer.print(L"#%hs#", string_id_get_string_const(m_name));
				source_data.type = s_text_source_data::_source_raw;
				set_text_internal(buffer.get_string(), &source_data, forcibly_set_text);
				string_set = true;
			}

			if (!string_set && use_list_item_label && parent_list_item != nullptr)
			{
				string_set = set_string_from_string_id(&buffer, string_list_owner, parent_list_item->get_item_label(), forcibly_set_text);
			}

			if (!string_set)
			{
				c_gui_screen_widget* parent_screen = get_parent_screen();
				c_gui_data* datasource = nullptr;
				long element_handle = NONE;

				if (parent_list != nullptr)
				{
					datasource = parent_list->get_data();
					element_handle = get_element_handle();
				}

				if (parent_screen != nullptr && m_definition.value_override_list != _string_id_invalid && m_definition.value_override_list != _string_id_empty_string)
				{
					c_gui_list_widget* override_list = nullptr;
					if (m_definition.value_override_list == STRING_ID(gui, focused_list))
					{
						c_gui_widget* focused_child = parent_screen->get_focused_widget();
						if (focused_child != nullptr)
						{
							override_list = focused_child->get_parent_list();
						}
					}
					else
					{
						override_list = parent_screen->get_child_list_widget(m_definition.value_override_list);
					}

					if (override_list != nullptr)
					{
						datasource = override_list->get_data();
						element_handle = override_list->get_focused_element_handle();
					}
				}

				if (datasource != nullptr)
				{
					string_id value_name = m_definition.value_identifier;

					if (use_exported_integer)
					{
						long value = 0;
						if (datasource->get_integer_value(element_handle, value_name, &value))
						{
							source_data.type = s_text_source_data::_source_exported_data;
							source_data.name = value_name;
							source_data.element_handle = element_handle;
							buffer.print(L"%d", value);
							set_text_internal(buffer.get_string(), &source_data, forcibly_set_text);
							string_set = true;
						}
					}

					if (!string_set && use_exported_text && datasource->get_text_value(element_handle, value_name, &buffer))
					{
						source_data.type = s_text_source_data::_source_exported_data;
						source_data.name = value_name;
						source_data.element_handle = element_handle;
						set_text_internal(buffer.get_string(), &source_data, forcibly_set_text);
						string_set = true;
					}

					if (!string_set && use_exported_string_id)
					{
						string_id result_value = _string_id_invalid;
						if (datasource->get_string_id_value(element_handle, value_name, &result_value))
						{
							string_set = set_string_from_string_id(&buffer, string_list_owner, result_value, forcibly_set_text);
						}
					}

					if (!string_set && use_exported_global_string_id)
					{
						s_user_interface_shared_globals const* ui_shared_globals = user_interface_shared_tag_globals_try_and_get();
						if (ui_shared_globals != nullptr && ui_shared_globals->ui_string_list.index != NONE)
						{
							string_id result_value = _string_id_invalid;
							if (datasource->get_string_id_value(element_handle, value_name, &result_value)
								&& multilingual_unicode_string_list_test_string(ui_shared_globals->ui_string_list.index, result_value)
								&& string_list_get_interface_string(ui_shared_globals->ui_string_list.index, result_value, &buffer))
							{
								source_data.type = s_text_source_data::_source_string_id;
								source_data.name = result_value;
								set_text_internal(buffer.get_string(), &source_data, forcibly_set_text);
								string_set = true;
							}
						}
					}

					if (!string_set && (use_exported_text || use_exported_string_id || use_exported_global_string_id || use_exported_integer))
					{
						source_data.type = s_text_source_data::_source_raw;
						source_data.name = value_name;
						set_text_internal(buffer.get_string(), &source_data, forcibly_set_text);
						string_set = true;
					}
				}
			}

			if (!string_set)
			{
				string_set = set_string_from_string_id(&buffer, string_list_owner, m_definition.value_identifier, forcibly_set_text);
			}

			if (!string_set && !get_text_internal()->get_string_was_set())
			{
				buffer.print(L"!%hs!", string_id_get_string_const(m_name));
				source_data.type = s_text_source_data::_source_raw;
				set_text_internal(buffer.get_string(), &source_data, forcibly_set_text);
				string_set = true;
			}

			if (add_brackets_this_frame && string_set)
			{
				c_static_wchar_string<1024> temporary_buffer;
				c_static_wchar_string<1024> buffer_with_brackets;

				user_interface_global_string_get(STRING_ID(gui, list_selection_left_bracket), &temporary_buffer);
				buffer_with_brackets.append(temporary_buffer.get_string());
				buffer_with_brackets.append(buffer.get_string());
				user_interface_global_string_get(STRING_ID(gui, list_selection_right_bracket), &temporary_buffer);
				buffer_with_brackets.append(temporary_buffer.get_string());

				set_text_internal(buffer_with_brackets.get_string(), &source_data, forcibly_set_text);
			}

			if (add_brackets_this_frame)
			{
				definition->flags |= FLAG(s_text_widget_definition::_brackets_added_for_focus_current_frame_bit);
			}
			else
			{
				definition->flags &= ~FLAG(s_text_widget_definition::_brackets_added_for_focus_current_frame_bit);
			}
		}
	}
}

void c_gui_text_widget::update_render_state(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B19A90, c_gui_text_widget, update_render_state, current_milliseconds);

	c_gui_widget::update_render_state(current_milliseconds);
	
	c_gui_screen_widget const* parent_screen = get_parent_screen();
	if (parent_screen != nullptr)
	{
		if (!parent_screen->transitioning_out())
		{
			get_text_internal()->update(-1);
		}
	}
}

bool c_gui_text_widget::within_focus_chain()
{
	//return INVOKE_CLASS_MEMBER(0x00B19AE0, c_gui_text_widget, within_focus_chain);

	bool in_focus_chain;

	c_gui_list_item_widget* parent_list_item_widget = get_parent_list_item();
	if (parent_list_item_widget != nullptr)
	{
		in_focus_chain = parent_list_item_widget->within_focus_chain();
	}
	else
	{
		in_focus_chain = c_gui_widget::within_focus_chain();
	}

	return in_focus_chain;
}

