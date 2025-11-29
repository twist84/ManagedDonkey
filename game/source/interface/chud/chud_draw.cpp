#include "interface/chud/chud_draw.hpp"

#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "game/game.hpp"
#include "interface/chud/chud.hpp"
#include "interface/chud/chud_definitions.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "text/unicode.hpp"

#include <cmath>

REFERENCE_DECLARE(0x0191CC64, bool, chud_enabled) = true;
REFERENCE_DECLARE(0x052604C1, bool, chud_debug_messages) = false;
REFERENCE_DECLARE(0x05269FC8, s_chud_draw_globals, g_chud_draw_globals);
REFERENCE_DECLARE(0x0526A085, bool, chud_debug_crosshair) = false;
REFERENCE_DECLARE(0x0526A087, bool, chud_debug_grid) = false;
REFERENCE_DECLARE(0x0526A088, bool, chud_contain_turbulence) = false;

HOOK_DECLARE(0x00AC7B80, chud_compute_render_data);
HOOK_DECLARE(0x00AC7F80, chud_debug_draw_reticle_labeled);
//HOOK_DECLARE(0x00AC8CE0, chud_draw_bitmap_widget);
//HOOK_DECLARE(0x00AC9490, chud_draw_text_widget);
HOOK_DECLARE(0x00AC9570, chud_draw_widget);

//.text:00AC59F0 ; bool __cdecl chud_apply_render_data(s_chud_render_data* render_data, bool fancy)
//.text:00AC5B40 ; void __cdecl chud_apply_widget_animation(s_chud_draw_widget_data* draw_widget_data, s_anchor_basis* anchor_basis, s_widget_geometry* widget_geometry, s_chud_render_data* render_data)
//.text:00AC5EB0 ; void __cdecl chud_apply_widget_animation_definition(real32 timer_msec, s_chud_animation_definition* animation_definition, s_anchor_basis* anchor_basis, s_widget_geometry* widget_geometry, s_chud_aux_animation_data* aux_animation_data)
//.text:00AC6220 ; bool __cdecl chud_bitmap_widget_compute_bitmap_info(int32 bitmap_group_index, int32 sequence_index, int32 sprite_index, s_chud_bitmap_info* bitmap_info)
//.text:00AC63F0 ; bool __cdecl chud_bitmap_widget_compute_geometry(s_chud_draw_widget_data* draw_widget_data, s_widget_geometry* result_geometry)
//.text:00AC67C0 ; void __cdecl chud_build_custom_render_data(s_chud_render_data* render_data, int32 chud_shader_index)
//.text:00AC6870 ; 
//.text:00AC69E0 ; real32 __cdecl chud_build_geometry_from_navpoint(int32 user_index, int32 sequence_index, int32 sprite_index, real32 origin_offset, real32 scale, s_widget_geometry* widget_geometry)
//.text:00AC6B80 ; bool __cdecl chud_build_text_geometry(s_widget_geometry* result_geometry, const wchar_t* text, e_font_id text_widget_font, bool center, real32 character_scale, real_rectangle2d* pixel_clip)
//.text:00AC6D60 ; bool __cdecl chud_compute_anchor_basis(int32 user_index, s_chud_draw_widget_data* draw_widget_data, int32 anchor_type, s_anchor_basis* basis)
//.text:00AC76A0 ; bool __cdecl chud_compute_anchor_basis_from_tracker(c_chud_point_tracker* tracker, s_anchor_basis* basis, bool rotate_if_offscreen)
//.text:00AC78D0 ; s_chud_curvature_info* __cdecl chud_compute_curvature_info(int32 user_index, int32 current_resolution)
//.text:00AC7940 ; void __cdecl chud_compute_geometry_common(s_chud_widget_placement_data* placement_data, s_widget_geometry* result_geometry)
//.text:00AC7A10 ; void __cdecl chud_compute_geometry_common_internal(s_widget_geometry* result_geometry, real32 origin_x, real32 origin_y, real32 offset_x, real32 offset_y, real32 scale_x, real32 scale_y)
//.text:00AC7AA0 ; bool __cdecl chud_compute_parent_transform(int32 user_index, s_chud_draw_widget_data* draw_widget_data, real_matrix4x3* parent_transform)

bool chud_compute_render_data_result = false;

bool __cdecl chud_compute_render_data(s_chud_draw_widget_data* draw_widget_data, s_chud_render_data* out_render_data, bool is_draw_turbulence)
{
	//INVOKE(0x00AC7B80, chud_compute_render_data, draw_widget_data, out_render_data, is_draw_turbulence);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(chud_compute_render_data_result =, chud_compute_render_data, draw_widget_data, out_render_data, is_draw_turbulence);

	if (chud_compute_render_data_result)
	{
		c_string_id artist_name = draw_widget_data->widget_base->artist_name;

		static c_static_wchar_string<128> pix_name;
		rasterizer_profile_begin_event(_rasterizer_profile_element_interface_hud, pix_name.print(L"%hs", artist_name.get_string()));
	}

	return chud_compute_render_data_result;
}

void __cdecl chud_debug_draw(int32 user_index)
{
	INVOKE(0x00AC7DB0, chud_debug_draw, user_index);
}

void __cdecl chud_debug_draw_reticle(int32 user_index, real32 angle, const real_argb_color* color)
{
	//INVOKE(0x00AC7F50, chud_debug_draw_reticle, user_index, angle, color);

	chud_debug_draw_reticle_labeled(user_index, angle, NULL, 0.0f, color);
}

void __cdecl chud_debug_draw_reticle_labeled(int32 user_index, real32 angle, const char* label, real32 a4, const real_argb_color* color)
{
	//INVOKE(0x00AC7F80, chud_debug_draw_reticle_labeled, user_index, angle, label, a4, color);

	real32 magic_crosshair_offset = observer_get(user_index)->result.magic_crosshair_offset;
	c_player_view* player_view = c_player_view::get_current();
	const render_projection* rasterizer_projection = player_view->get_rasterizer_projection();

	real32 v2 = tanf(angle) / CIRCLE_DIVISIONS;
	real_point3d points[CIRCLE_DIVISIONS]{};

	real32 current_angle = 0.0f;
	for (int32 i = 0; i < CIRCLE_DIVISIONS; i++)
	{
		set_real_point3d(&points[i], cosine(current_angle) * v2, real32(sine(current_angle) * v2) - real32(magic_crosshair_offset * (1.0f / CIRCLE_DIVISIONS)), -(1.0f / CIRCLE_DIVISIONS));
		matrix4x3_transform_point(&rasterizer_projection->view_to_world, &points[i], &points[i]);
	
		current_angle += (TWO_PI / CIRCLE_DIVISIONS);
	}

	for (int32 i = 0; i < CIRCLE_DIVISIONS; i++)
		render_debug_line(false, &points[i], &points[(i + 1) % CIRCLE_DIVISIONS], color);

	if (label)
		render_debug_string_at_point(&points[int32((a4 / TWO_PI) * CIRCLE_DIVISIONS)], label, color);
}

void __cdecl chud_debug_render()
{
	INVOKE(0x00AC8070, chud_debug_render);

	//if (chud_globals)
	//{
	//	if (chud_debug_grid)
	//	{
	//		int32 user_index = player_mapping_first_active_output_user();
	//		if (user_index != NONE)
	//		{
	//			// $IMPLEMENT
	//		}
	//	}
	//
	//	if (chud_debug_crosshair)
	//	{
	//		int32 user_index = player_mapping_first_active_output_user();
	//		if (user_index != NONE)
	//			chud_debug_draw(user_index);
	//	}
	//}
}

//.text:00AC80E0 ; void __cdecl chud_draw_alpha_watermark(int32 user_index, uns64 magic_bit_pattern)

bool __cdecl chud_draw_begin(int32 user_index, real32 global_hud_alpha, bool setup_cortana_effect, bool drawing_saved_film)
{
	return INVOKE(0x00AC80F0, chud_draw_begin, user_index, global_hud_alpha, setup_cortana_effect, drawing_saved_film);
}

//.text:00AC8C10 ; 

void __cdecl chud_draw_bitmap_widget(int32 user_index, s_chud_draw_widget_data* draw_widget_data, bool is_draw_turbulence)
{
	chud_compute_render_data_result = false;

	INVOKE(0x00AC8CE0, chud_draw_bitmap_widget, user_index, draw_widget_data, is_draw_turbulence);

	if (chud_compute_render_data_result)
	{
		rasterizer_profile_end_event();
	}
}

//.text:00AC8E50 ; void __cdecl chud_draw_debug(int32 user_index)

void __cdecl chud_draw_end(int32 user_index, bool resolve_cortana_effect)
{
	INVOKE(0x00AC8E60, chud_draw_end, user_index, resolve_cortana_effect);
}

//.text:00AC8E80 ; void __cdecl chud_draw_initialize()
//.text:00AC8E90 ; 

void __cdecl chud_draw_text_widget(int32 user_index, s_chud_draw_widget_data* draw_widget_data, bool is_draw_turbulence)
{
	chud_compute_render_data_result = false;

	INVOKE(0x00AC9490, chud_draw_text_widget, user_index, draw_widget_data, is_draw_turbulence);

	if (chud_compute_render_data_result)
	{
		rasterizer_profile_end_event();
	}

	//s_chud_widget_base* widget_base = draw_widget_data->widget_base;
	//if (widget_base->placement_data.count)
	//{
	//	s_chud_widget_placement_data* placement_data = TAG_BLOCK_GET_ELEMENT(&widget_base->placement_data, 0, s_chud_widget_placement_data);
	//	if (!TEST_BIT(draw_widget_data->widget->flags, 0))
	//	{
	//		s_widget_geometry result_geometry{};
	//		result_geometry.texture_reference = c_rasterizer_texture_ref();
	//
	//		s_chud_render_data render_data{};
	//		if (chud_compute_render_data(draw_widget_data, &render_data, is_draw_turbulence))
	//		{
	//			s_anchor_basis basis{};
	//			chud_compute_anchor_basis(user_index, draw_widget_data, placement_data->anchor_type, &basis);
	//			if (chud_text_widget_compute_geometry(draw_widget_data, &result_geometry, &render_data))
	//			{
	//				chud_apply_widget_animation(draw_widget_data, &basis, &result_geometry, &render_data);
	//				if (chud_apply_render_data(&render_data, false))
	//				{
	//					chud_draw_widget_geometry(&result_geometry, &basis, false, false);
	//				}
	//			}
	//
	//			rasterizer_profile_end_event();
	//		}
	//	}
	//}
}

//.text:00AC9560 ; void __cdecl chud_draw_training_text(int32 user_index)

void __cdecl chud_draw_widget(int32 user_index, s_chud_runtime_widget_datum* widget, int32 chud_definition_index, int32 is_draw_turbulence)
{
	//INVOKE(0x00AC9570, chud_draw_widget, user_index, widget, chud_definition_index, is_draw_turbulence);

	s_chud_definition* widget_definition = TAG_GET(CHUD_DEFINITION_TAG, s_chud_definition, chud_definition_index);
	s_chud_widget_collection* widget_collection = TAG_BLOCK_GET_ELEMENT(&widget_definition->widget_collections, widget->collection_index, s_chud_widget_collection);

	bool is_text_widget = false;
	s_chud_widget_base* widget_base = widget_collection->get_widget_by_index(widget->widget_index, &is_text_widget);

	if (!game_is_playback()
		|| widget_collection->scripting_class != _chud_scripting_class_crosshair
		|| game_playback_should_display_crosshair(user_index))
	{
		s_chud_draw_widget_data draw_widget_data
		{
			.user_index = user_index,
			.widget = widget,
			.widget_definition = widget_definition,
			.widget_collection = widget_collection,
			.widget_base = widget_base,
		};

		if (is_text_widget)
		{
			chud_draw_text_widget(user_index, &draw_widget_data, is_draw_turbulence);
		}
		else
		{
			chud_draw_bitmap_widget(user_index, &draw_widget_data, is_draw_turbulence);
		}
	}
}

//.text:00AC9620 ; void __cdecl chud_draw_widget_geometry(s_widget_geometry* geometry, s_anchor_basis* anchor_basis, bool mirror_x, bool mirror_y)
//.text:00AC9AA0 ; void __cdecl chud_expand_pixel32(uns32 value, real_vector4d* result)
//.text:00AC9B50 ; void __cdecl chud_geometry_build_transform(const s_widget_geometry* geometry, real_matrix4x3* widget_transform)
//.text:00AC9C70 ; void __cdecl chud_get_crosshair_screen_position(int32 user_index, real_point2d* position)
//.text:00AC9DD0 ; int32 __cdecl chud_get_current_resolution(int32 user_index)
//.text:00AC9ED0 ; 
//.text:00AC9F50 ; 
//.text:00AC9FD0 ; 
//.text:00ACA020 ; void __cdecl chud_render_data_fetch_color_output(int32 user_index, int16 color_output_type, s_chud_widget_render_data* render_data, real_vector4d* result)
//.text:00ACA300 ; real32 __cdecl chud_render_data_fetch_scalar_output(int16 scalar_output_type, s_chud_widget_render_data* render_data, s_chud_runtime_widget_datum* widget)
//.text:00ACA380 ; bool __cdecl chud_text_widget_compute_geometry(s_chud_draw_widget_data* draw_widget_data, s_widget_geometry* result_geometry, s_chud_render_data* render_data)
//.text:00ACA420 ; bool __cdecl chud_text_widget_compute_string(s_chud_draw_widget_data* draw_widget_data, s_chud_render_data* render_data, c_static_wchar_string<256>* parsed_text)
//.text:00ACAD00 ; void __cdecl chud_widget_build_quads_from_bounds(real_rectangle2d* actual_geometry_bounds, real_rectangle2d* extended_geometry_bounds, real_rectangle2d* texture_bounds, s_widget_geometry* result_geometry, real32 scale_x, real32 scale_y)
//.text:00ACAFA0 ; void __cdecl chud_widget_geometry_add_character(s_widget_geometry* geometry, const real_rectangle2d* src_rectangle, const real_rectangle2d* dest_rectangle, real32 character_scale)
//.text:00ACB0E0 ; 

// $TODO find me a home
bool __cdecl game_playback_should_display_crosshair(int32 user_index)
{
	return INVOKE(0x00AD0F50, game_playback_should_display_crosshair, user_index);
}

