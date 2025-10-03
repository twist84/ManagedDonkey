#include "interface/c_gui_bitmap_widget.hpp"

#include "bitmaps/bitmap_group.hpp"
#include "cache/cache_files.hpp"
#include "gui_custom_bitmap_storage.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/interface_constants.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_utilities.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/screen_postprocess.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B167B0, c_gui_bitmap_widget, assemble_render_data_);
HOOK_DECLARE(0x00B170F0, render_bitmap);

void __thiscall c_gui_bitmap_widget::assemble_render_data_(
	s_gui_widget_render_data* render_data,
	const rectangle2d* window_bounds,
	e_controller_index local_controller_index,
	bool apply_translation,
	bool apply_scale,
	bool apply_rotation)
{
	c_gui_bitmap_widget::assemble_render_data(
		render_data,
		window_bounds,
		local_controller_index,
		apply_translation,
		apply_scale,
		apply_rotation);
}

c_gui_bitmap_widget::c_gui_bitmap_widget() :
	c_gui_widget(_gui_bitmap),
	m_override_sprite_bitmap_index(NONE),
	m_override_sprite_frame(NONE),
	m_override_sprite_sequence(NONE),
	m_definition()
{
	//DECLFUNC(0x00B16670, void, __thiscall, c_gui_bitmap_widget*)(this);
}

//.text:00B166B0 ; public: virtual void* c_gui_bitmap_widget::`vector deleting destructor'(unsigned int)

void __cdecl c_gui_bitmap_widget::add_definition_fields(const s_bitmap_widget_definition* source_definition, s_runtime_bitmap_widget_definition* dest_definition, real_rectangle2d* positioning_bounds, bool was_templated)
{
	INVOKE(0x00B166E0, c_gui_bitmap_widget::add_definition_fields, source_definition, dest_definition, positioning_bounds, was_templated);
}

//.text:00B16760 ; public: static void __cdecl c_gui_bitmap_widget::assemble_definition(const s_bitmap_widget_block*, s_runtime_bitmap_widget_definition*, real_rectangle2d*)

void c_gui_bitmap_widget::assemble_render_data(
	s_gui_widget_render_data* render_data,
	const rectangle2d* window_bounds,
	e_controller_index local_controller_index,
	bool apply_translation,
	bool apply_scale,
	bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00B167B0, c_gui_bitmap_widget, assemble_render_data,
	//	render_data,
	//	window_bounds,
	//	local_controller_index,
	//	apply_translation,
	//	apply_scale,
	//	apply_rotation);

	c_gui_widget::assemble_render_data(
		render_data,
		window_bounds,
		local_controller_index,
		apply_translation,
		apply_scale,
		apply_rotation);

	s_gui_bitmap_widget_render_data* bitmap_render_data = (s_gui_bitmap_widget_render_data*)render_data;

	bool render_blurred_back_buffer = TEST_BIT(m_definition.flags, 4);
	bitmap_render_data->flags.set(s_gui_widget_render_data::_render_blurred_back_buffer_bit, render_blurred_back_buffer);

	bool render_as_player_emblem = renders_as_player_emblem();
	bitmap_render_data->flags.set(s_gui_widget_render_data::_render_as_player_emblem_bit, render_as_player_emblem);

	if (m_override_sprite_bitmap_index != NONE)
	{
		bitmap_render_data->bitmap_definition_index = m_override_sprite_bitmap_index;
	}
	else
	{
		bitmap_render_data->bitmap_definition_index = m_definition.bitmap_reference_index;
	}

	if (render_blurred_back_buffer)
	{
		real_rectangle2d pin_bounds
		{
			.x0 = (real32)window_bounds->x0,
			.x1 = (real32)window_bounds->x1,
			.y0 = (real32)window_bounds->y0,
			.y1 = (real32)window_bounds->y1,
		};
		bitmap_render_data->projected_bounds.pin(&pin_bounds);
	}
	else if (render_as_player_emblem)
	{
		c_gui_screen_widget* parent_screen = get_parent_screen();
		ASSERT(parent_screen != NULL);

		bool emblem_info_valid = parent_screen->try_and_get_render_data_emblem_info(this, &bitmap_render_data->source.emblem);
		if (!emblem_info_valid)
		{
			s_player_appearance player_appearance{};

			if (get_element_handle() != NONE)
			{
				if (c_gui_list_widget* parent_list = get_parent_list())
				{
					if (c_gui_data* data = parent_list->get_data())
					{
						emblem_info_valid = data->get_player_appearance(get_element_handle(), &player_appearance);
					}
				}
			}

			if (emblem_info_valid)
			{
				bitmap_render_data->source.emblem = player_appearance.emblem_info;
			}
			else if (VALID_CONTROLLER(bitmap_render_data->local_controller_index))
			{
				bitmap_render_data->source.emblem = controller_get(bitmap_render_data->local_controller_index)->get_player_profile_interface()->get_emblem_info();
				emblem_info_valid = true;
			}
		}

		bitmap_render_data->flags.set(s_gui_widget_render_data::_emblem_info_valid_bit, emblem_info_valid);
	}
	else
	{
		bitmap_render_data->source.sprite.sequence = m_animated_state.bitmap_sprite_sequence;
		bitmap_render_data->source.sprite.frame = m_animated_state.bitmap_sprite_frame;
	}

	real_argb_color argb_tint_color{};
	bitmap_render_data->argb_tint = real_argb_color_to_pixel32(get_cumulative_color_tint(&argb_tint_color));
	bitmap_render_data->texture_uv_offset = m_animated_state.texture_uv;
	bitmap_render_data->frame_buffer_blend_function = PIN(m_definition.render_blend_mode, 0, 12);
	bitmap_render_data->explicit_shader_index = m_definition.explicit_shader_reference_index;
}

bool c_gui_bitmap_widget::can_receive_focus()
{
	//return INVOKE_CLASS_MEMBER(0x00B168F0, c_gui_bitmap_widget, can_receive_focus);

	return TEST_BIT(m_definition.flags, 10);
}

e_animation_state c_gui_bitmap_widget::get_ambient_state()
{
	return INVOKE_CLASS_MEMBER(0x00B16900, c_gui_bitmap_widget, get_ambient_state);
}

static bitmap_data* get_bitmap_and_hardware_format(int32 bitmap_group_index, int32 sequences_index, int32 bitmap_index, c_rasterizer_texture_ref* bitmap_hardware_format)
{
	return INVOKE(0x00B16970, get_bitmap_and_hardware_format, bitmap_group_index, sequences_index, bitmap_index, bitmap_hardware_format);
}

//.text:00B16A20 ; 

s_runtime_core_widget_definition* c_gui_bitmap_widget::get_core_definition()
{
	//return INVOKE_CLASS_MEMBER(0x00B16AB0, c_gui_bitmap_widget, get_core_definition);
	
	return &m_definition;
}

const bitmap_data* c_gui_bitmap_widget::get_current_bitmap() const
{
	return INVOKE_CLASS_MEMBER(0x00B16AC0, c_gui_bitmap_widget, get_current_bitmap);
}

//.text:00B16B70 ; public: const bitmap_data* c_gui_bitmap_widget::get_current_bitmap_hardware_format(c_rasterizer_texture_ref*) const

real_rectangle2d* c_gui_bitmap_widget::get_current_bounds(real_rectangle2d* unanimated_bounds)
{
	real_rectangle2d* result = INVOKE_CLASS_MEMBER(0x00B16C40, c_gui_bitmap_widget, get_current_bounds, unanimated_bounds);
	return result;
}

//.text:00B16E10 ; 
//.text:00B16E40 ; 
//.text:00B16F00 ; 
//.text:00B16F20 ; 
//.text:00B16FC0 ; 

void c_gui_bitmap_widget::initialize(const s_bitmap_widget_block* template_and_override_block)
{
	//INVOKE_CLASS_MEMBER(0x00B16FE0, c_gui_bitmap_widget, initialize, template_and_override_block);

	if (template_and_override_block)
	{
		real_rectangle2d positioning_bounds{};
		m_parent.get_value()->get_authored_bounds(&positioning_bounds);
		
		bool was_templated = false;
		if (template_and_override_block->widget_template_reference.index != NONE)
		{
			s_bitmap_widget_definition* bitmap_widget_definition = TAG_GET(GUI_BITMAP_WIDGET_DEFINITION_TAG, s_bitmap_widget_definition, template_and_override_block->widget_template_reference.index);
			c_gui_bitmap_widget::add_definition_fields(bitmap_widget_definition, &m_definition, &positioning_bounds, false);
			was_templated = true;
		}
		c_gui_bitmap_widget::add_definition_fields(&template_and_override_block->override_definition, &m_definition, &positioning_bounds, was_templated);
		m_name = m_definition.widget_identifier;
	}

	if (m_definition.bitmap_reference_index > 0)
	{
		user_interface_precache_bitmaps_from_tag(m_definition.bitmap_reference_index);
	}

	c_gui_widget::initialize();
}

void c_gui_bitmap_widget::__func39(c_tag_resource_demand_collector* demand_collector)
{
	INVOKE_CLASS_MEMBER(0x00B170A0, c_gui_bitmap_widget, __func39, demand_collector);
}

void c_gui_bitmap_widget::__func40(c_tag_resource_demand_collector* demand_collector)
{
	INVOKE_CLASS_MEMBER(0x00B170D0, c_gui_bitmap_widget, __func40, demand_collector);
}

struct s_dynamic_render_target
{
	int16 identifier;
	int32 frame_allocated;
	int32 target_type;
	c_rasterizer_texture_ref accumulation_surface;
	c_rasterizer_texture_ref albedo_surface;
	c_rasterizer_texture_ref normal_surface;
	c_rasterizer_texture_ref depth_stencil_surface;
};
static_assert(sizeof(s_dynamic_render_target) == 0x1C);

void __cdecl render_bitmap(const s_gui_bitmap_widget_render_data* render_data, const rectangle2d* window_bounds)
{
	//INVOKE(0x00B170F0, render_bitmap, render_data, window_bounds);

	ASSERT(render_data != NULL);

	real32 width = render_data->projected_bounds.get_width();
	real32 height = render_data->projected_bounds.get_height();
	int32 x0 = window_bounds->x0;
	int32 y0 = window_bounds->y0;

	if (width < _real_epsilon || height < _real_epsilon)
	{
		return;
	}

	rectangle2d render_bounds{};
	rectangle2d out_display_bounds{};
	interface_get_current_display_settings(&render_bounds, NULL, &out_display_bounds, NULL);
	
	real32 render_width = (real32)rectangle2d_width(&render_bounds);
	real32 render_height = (real32)rectangle2d_height(&render_bounds);

	int32 explicit_shader_index = NONE;// render_data->explicit_shader_index;

	real32 left_x = 0.0f;
	real32 left_y = 0.0f;
	real32 right_x = 1.0f;
	real32 right_y = 1.0f;

	bool flipped_axes = false;
	
	c_rasterizer_texture_ref hardware_format_primary;
	c_rasterizer_texture_ref hardware_format_secondary;
	
	if (render_data->flags.test(s_gui_widget_render_data::_render_texture_camera_bit))
	{
		//if (s_dynamic_render_target* target = c_dynamic_render_targets::get_target_from_target_type(_dynamic_render_target_type_texture_camera))
		//{
		//	int32 accumulation_surface_width;
		//	int32 accumulation_surface_height;
		//	if (target->accumulation_surface.get_width_height(0, &accumulation_surface_width, &accumulation_surface_height))
		//	{
		//		real32 ratio = real32(accumulation_surface_width * width) / real32(accumulation_surface_height * height);
		//		if (render_width > _real_epsilon && render_height > _real_epsilon)
		//		{
		//			if (render_width > render_height)
		//			{
		//				ratio *= (render_height / render_width);
		//				left_x += 0.5f * (1.0f - ratio);
		//				right_x -= 0.5f * (1.0f - ratio);
		//			}
		//			else if (render_height > render_width)
		//			{
		//				ratio *= (render_width / render_height);
		//				left_y += 0.5f * (1.0f - ratio);
		//				right_y -= 0.5f * (1.0f - ratio);
		//			}
		//		}
		//
		//		hardware_format_primary = target->accumulation_surface;
		//	}
		//}
	}
	else if (render_data->flags.test(s_gui_widget_render_data::_render_blurred_back_buffer_bit))
	{
		if (render_width >= _real_epsilon && render_height >= _real_epsilon)
		{
			c_rasterizer::e_surface surface = c_rasterizer::_surface_display;

			//real32 display_width = (real32)rectangle2d_width(&out_display_bounds);
			//real32 display_height = (real32)rectangle2d_height(&out_display_bounds);
			//
			//left_x = render_data->projected_bounds.vertex[0].x / display_width;
			//left_y = render_data->projected_bounds.vertex[2].y / display_height;
			//right_x = render_data->projected_bounds.vertex[3].x / display_width;
			//right_y = render_data->projected_bounds.vertex[3].y / display_height;
			//
			//real32 horizontal_blur_factor = 1.5f;
			//real32 vertical_blur_factor = 1.5f;
			//
			//if (const s_user_interface_shared_globals* user_interface_shared_globals = user_interface_shared_tag_globals_try_and_get())
			//{
			//	horizontal_blur_factor = user_interface_shared_globals->horizontal_blur_factor;
			//	vertical_blur_factor = user_interface_shared_globals->vertical_blur_factor;
			//}
			//
			//c_screen_postprocess::setup_rasterizer_for_postprocess(false);
			//c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_none);
			//
			//surface = c_screen_postprocess::blur_display(/*horizontal_blur_factor, vertical_blur_factor*/);
			//c_rasterizer::set_render_target(0, c_rasterizer::_surface_display, 0xFFFFFFFF);
			//
			//c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil);
			//c_rasterizer::restore_last_viewport();
			//c_rasterizer::restore_last_scissor_rect();

			hardware_format_primary = c_rasterizer::get_surface_texture(surface);
		}
	}
	else if (render_data->flags.test(s_gui_widget_render_data::_render_as_player_emblem_bit))
	{
		if (render_data->flags.test(s_gui_widget_render_data::_emblem_info_valid_bit) && emblem_set_render_constants_from_user_interface(&render_data->source.emblem))
		{
			get_bitmap_and_hardware_format(render_data->bitmap_definition_index, 0, render_data->source.emblem.background_emblem_index, &hardware_format_primary);
			get_bitmap_and_hardware_format(render_data->bitmap_definition_index, 0, render_data->source.emblem.foreground_emblem_index, &hardware_format_secondary);
			explicit_shader_index = c_rasterizer_globals::_shader_player_emblem_screen;
		}
	}
	else if (render_data->flags.test(s_gui_widget_render_data::_render_as_custom_storage_bitmap_bit))
	{
		if (render_data->source.custom_bitmap.storage_index != NONE)
		{
			const c_gui_custom_bitmap_storage_item* bitmap = c_gui_custom_bitmap_storage_manager::get()->get_bitmap(render_data->source.custom_bitmap.storage_index);
			if (bitmap && bitmap->m_bitmap_ready && &bitmap->m_bitmap_data)
			{
				hardware_format_primary = bitmap->m_bitmap_data.internal_hardware_format;
				if (bitmap->m_use_shader_custom_gamma_correct) // added by us
				{
					explicit_shader_index = c_rasterizer_globals::_shader_custom_gamma_correct;
				}
			}
		}
	}
	else
	{
		if (render_data->bitmap_definition_index != NONE)
		{
			bitmap_data* bitmap = get_bitmap_and_hardware_format(
				render_data->bitmap_definition_index,
				render_data->source.sprite.sequence,
				render_data->source.sprite.frame,
				&hardware_format_primary);
	
			if (render_data->source.sprite.sequence != 0xFFFFi16 && render_data->source.sprite.frame != 0xFFFFi16)
			{
				const real_rectangle2d* bounding_rect = bitmap_group_get_bounding_rect_from_sequence(
					render_data->bitmap_definition_index,
					render_data->source.sprite.sequence,
					render_data->source.sprite.frame);
				if (bounding_rect)
				{
					left_x = bounding_rect->x0;
					left_y = bounding_rect->y0;
					right_x = bounding_rect->x1;
					right_y = bounding_rect->y1;
				}
			}

			flipped_axes = bitmap && bitmap->flags.test(_bitmap_flipped_axes_bit);
	
			if (render_data->flags.test(s_gui_widget_render_data::_render_as_au2_playlist_rating))
			{
				explicit_shader_index = c_rasterizer_globals::_shader_hack_au2_playlist_rating;
			}
			else if (render_data->flags.test(s_gui_widget_render_data::_render_as_au2_rank_tray))
			{
				explicit_shader_index = c_rasterizer_globals::_shader_hack_au2_rank_tray;
			}
		}
	}
	
	//if (v15 && v66)
	{
		rasterizer_dynamic_screen_geometry_parameters parameters{};
		rasterizer_vertex_screen vertices[4]{};
	
		gui_real_rectangle2d projected_bounds = render_data->projected_bounds;
		projected_bounds.offset((real32)x0, (real32)y0);
	
		csmemset(&parameters, 0, sizeof(rasterizer_dynamic_screen_geometry_parameters));

		vertices[0].position = projected_bounds.vertex[0];
		vertices[0].texcoord.i =render_data->texture_uv_offset.x + left_x;
		vertices[0].texcoord.j =render_data->texture_uv_offset.y + left_y;
		vertices[0].color = render_data->argb_tint;
	
		vertices[1].position = projected_bounds.vertex[2];
		vertices[2 * flipped_axes + 1].texcoord.i = render_data->texture_uv_offset.x + right_x;
		vertices[2 * flipped_axes + 1].texcoord.j = render_data->texture_uv_offset.y + left_y;
		vertices[1].color = render_data->argb_tint;
	
		vertices[2].position = projected_bounds.vertex[3];
		vertices[2].texcoord.i = render_data->texture_uv_offset.x + right_x;
		vertices[2].texcoord.j = render_data->texture_uv_offset.y + right_y;
		vertices[2].color = render_data->argb_tint;
	
		vertices[3].position = projected_bounds.vertex[1];
		vertices[2 * !flipped_axes + 1].texcoord.i = render_data->texture_uv_offset.x + left_x;
		vertices[2 * !flipped_axes + 1].texcoord.j = render_data->texture_uv_offset.y + right_y;
		vertices[3].color = render_data->argb_tint;
	
		real_vector2d aspect_ratio_scale = interface_get_aspect_ratio_scaling();
		interface_scale_screenspace_vertices_for_xenon_scaler(vertices, NUMBEROF(vertices), &aspect_ratio_scale);
	
		uns32 samplers_flag = 0;
	
		parameters.map_texture_scale[0] = { 1.0f, 1.0f };
		parameters.map_wrapped[0] = render_data->texture_uv_offset.x != 0.0f || render_data->texture_uv_offset.y != 0.0f;
		parameters.map_scale[0] = { 1.0f, 1.0f };
	
		if (hardware_format_primary.valid())
		{
			parameters.hardware_formats[0] = hardware_format_primary;
			samplers_flag = FLAG(0);
		}
	
		if (hardware_format_secondary.valid())
		{
			parameters.hardware_formats[1] = hardware_format_secondary;
			parameters.map_wrapped[1] = parameters.map_wrapped[0];
			parameters.map_texture_scale[1] = parameters.map_texture_scale[0];
			samplers_flag |= FLAG(1);
		}
	
		parameters.framebuffer_blend_function = int_pin((int16)render_data->frame_buffer_blend_function, 0, 12);
		parameters.explicit_override_index = explicit_shader_index;
	
		rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);
		c_rasterizer::clear_sampler_textures(samplers_flag);
	}
}

bool c_gui_bitmap_widget::renders_as_player_emblem() const
{
	//return INVOKE_CLASS_MEMBER(0x00B17640, c_gui_bitmap_widget, renders_as_player_emblem);

	return TEST_BIT(m_definition.flags, 5);
}

void c_gui_bitmap_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	//INVOKE_CLASS_MEMBER(0x00B17650, c_gui_bitmap_widget, set_animated_state_baseline, transform);

	c_gui_widget::set_animated_state_baseline(transform);
	transform->bitmap_sprite_sequence = m_definition.initial_sprite_sequence;
	transform->bitmap_sprite_frame = m_definition.initial_sprite_frame;
}

void c_gui_bitmap_widget::set_sprite_frame(int32 sprite_frame)
{
	m_override_sprite_frame = sprite_frame;
}

void c_gui_bitmap_widget::set_sprite_sequence(int32 sprite_sequence)
{
	m_override_sprite_sequence = sprite_sequence;
}

void c_gui_bitmap_widget::update_render_state(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B17690, c_gui_bitmap_widget, update_render_state, current_milliseconds);
}

bool c_gui_bitmap_widget::within_focus_chain()
{
	//return INVOKE_CLASS_MEMBER(0x00B17810, c_gui_bitmap_widget, within_focus_chain);

	if (c_gui_list_item_widget* parent_list_item = get_parent_list_item())
	{
		return parent_list_item->within_focus_chain();
	}

	return c_gui_widget::within_focus_chain();
}

