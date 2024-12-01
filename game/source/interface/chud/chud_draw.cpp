#include "interface/chud/chud_draw.hpp"

#include "camera/observer.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "text/unicode.hpp"

#include <cmath>

REFERENCE_DECLARE(0x0191CC64, bool, chud_enabled);
REFERENCE_DECLARE(0x052604C1, bool, chud_debug_messages);
REFERENCE_DECLARE(0x05269FC8, s_chud_draw_globals, g_chud_draw_globals);
REFERENCE_DECLARE(0x0526A085, bool, chud_debug_crosshair);
REFERENCE_DECLARE(0x0526A087, bool, chud_debug_grid);

HOOK_DECLARE(0x00AC7B80, chud_compute_render_data);
HOOK_DECLARE(0x00AC7F80, chud_debug_draw_reticle_labeled);
HOOK_DECLARE(0x00AC8CE0, chud_draw_bitmap_widget);
HOOK_DECLARE(0x00AC9490, chud_draw_text_widget);

//.text:00AC59F0 ; bool __cdecl chud_apply_render_data(s_chud_render_data* render_data, bool fancy)
//.text:00AC5B40 ; void __cdecl chud_apply_widget_animation(s_chud_draw_widget_data* draw_widget_data, s_anchor_basis* anchor_basis, s_widget_geometry* widget_geometry, s_chud_render_data* render_data)
//.text:00AC5EB0 ; void __cdecl chud_apply_widget_animation_definition(real timer_msec, s_chud_animation_definition* animation_definition, s_anchor_basis* anchor_basis, s_widget_geometry* widget_geometry, s_chud_aux_animation_data* aux_animation_data)
//.text:00AC6220 ; bool __cdecl chud_bitmap_widget_compute_bitmap_info(long bitmap_group_index, long sequence_index, long sprite_index, s_chud_bitmap_info* bitmap_info)
//.text:00AC63F0 ; bool __cdecl chud_bitmap_widget_compute_geometry(s_chud_draw_widget_data* draw_widget_data, s_widget_geometry* result_geometry)
//.text:00AC67C0 ; void __cdecl chud_build_custom_render_data(s_chud_render_data* render_data, long chud_shader_index)
//.text:00AC6870 ; 
//.text:00AC69E0 ; real __cdecl chud_build_geometry_from_navpoint(long user_index, long sequence_index, long sprite_index, real origin_offset, real scale, s_widget_geometry* widget_geometry)
//.text:00AC6B80 ; bool __cdecl chud_build_text_geometry(s_widget_geometry* result_geometry, wchar_t const* text, e_font_id text_widget_font, bool center, real character_scale, real_rectangle2d* pixel_clip)
//.text:00AC6D60 ; bool __cdecl chud_compute_anchor_basis(long user_index, s_chud_draw_widget_data* draw_widget_data, long anchor_type, s_anchor_basis* basis)
//.text:00AC76A0 ; bool __cdecl chud_compute_anchor_basis_from_tracker(c_chud_point_tracker* tracker, s_anchor_basis* basis, bool rotate_if_offscreen)
//.text:00AC78D0 ; s_chud_curvature_info* __cdecl chud_compute_curvature_info(long user_index, long current_resolution)
//.text:00AC7940 ; void __cdecl chud_compute_geometry_common(s_chud_widget_placement_data* placement_data, s_widget_geometry* result_geometry)
//.text:00AC7A10 ; void __cdecl chud_compute_geometry_common_internal(s_widget_geometry* result_geometry, real origin_x, real origin_y, real offset_x, real offset_y, real scale_x, real scale_y)
//.text:00AC7AA0 ; bool __cdecl chud_compute_parent_transform(long user_index, s_chud_draw_widget_data* draw_widget_data, real_matrix4x3* parent_transform)

bool chud_compute_render_data_result = false;

//bool __cdecl chud_compute_render_data(s_chud_draw_widget_data* draw_widget_data, s_chud_render_data* out_render_data, bool is_draw_turbulence)
bool __cdecl chud_compute_render_data(void* draw_widget_data, void* out_render_data, bool is_draw_turbulence)
{
	//INVOKE(0x00AC7B80, chud_compute_render_data, draw_widget_data, out_render_data, is_draw_turbulence);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(chud_compute_render_data_result =, chud_compute_render_data, draw_widget_data, out_render_data, is_draw_turbulence);

	if (chud_compute_render_data_result)
	{
		static c_static_wchar_string<128> pix_name;
		REFERENCE_DECLARE(offset_pointer(draw_widget_data, 0x10), void*, draw_widget_data10);
		REFERENCE_DECLARE(draw_widget_data10, c_string_id, name_id);
		rasterizer_profile_begin_event(_rasterizer_profile_element_interface_hud, pix_name.print(L"%hs", name_id.get_string()));
	}

	return chud_compute_render_data_result;
}

void __cdecl chud_debug_draw(long user_index)
{
	INVOKE(0x00AC7DB0, chud_debug_draw, user_index);
}

void __cdecl chud_debug_draw_reticle(long user_index, real angle, real_argb_color const* color)
{
	//INVOKE(0x00AC7F50, chud_debug_draw_reticle, user_index, angle, color);

	chud_debug_draw_reticle_labeled(user_index, angle, NULL, 0.0f, color);
}

void __cdecl chud_debug_draw_reticle_labeled(long user_index, real angle, char const* label, real a4, real_argb_color const* color)
{
	//INVOKE(0x00AC7F80, chud_debug_draw_reticle_labeled, user_index, angle, label, a4, color);

	real magic_crosshair_offset = observer_get(user_index)->result.magic_crosshair_offset;
	c_player_view* player_view = c_player_view::get_current();
	render_projection const* rasterizer_projection = player_view->get_rasterizer_projection();

	real v2 = tanf(angle) / CIRCLE_DIVISIONS;
	real_point3d points[CIRCLE_DIVISIONS]{};

	real current_angle = 0.0f;
	for (long i = 0; i < CIRCLE_DIVISIONS; i++)
	{
		set_real_point3d(&points[i], cosf(current_angle) * v2, real(sinf(current_angle) * v2) - real(magic_crosshair_offset * (1.0f / CIRCLE_DIVISIONS)), -(1.0f / CIRCLE_DIVISIONS));
		matrix4x3_transform_point(&rasterizer_projection->view_to_world, &points[i], &points[i]);
	
		current_angle += (TWO_PI / CIRCLE_DIVISIONS);
	}

	for (long i = 0; i < CIRCLE_DIVISIONS; i++)
		render_debug_line(false, &points[i], &points[(i + 1) % CIRCLE_DIVISIONS], color);

	if (label)
		render_debug_string_at_point(&points[long((a4 / TWO_PI) * CIRCLE_DIVISIONS)], label, color);
}

void __cdecl chud_debug_render()
{
	INVOKE(0x00AC8070, chud_debug_render);

	//if (chud_globals)
	//{
	//	if (chud_debug_grid)
	//	{
	//		long user_index = player_mapping_first_active_output_user();
	//		if (user_index != NONE)
	//		{
	//			// #TODO: implement me
	//		}
	//	}
	//
	//	if (chud_debug_crosshair)
	//	{
	//		long user_index = player_mapping_first_active_output_user();
	//		if (user_index != NONE)
	//			chud_debug_draw(user_index);
	//	}
	//}
}

//.text:00AC80E0 ; void __cdecl chud_draw_alpha_watermark(long user_index, qword magic_bit_pattern)
//.text:00AC80F0 ; void __cdecl chud_draw_begin(long user_index, real global_hud_alpha, bool setup_cortana_effect, bool drawing_saved_film)
//.text:00AC8C10 ; 

void __cdecl chud_draw_bitmap_widget(long user_index, void* draw_widget_data, bool is_draw_turbulence)
{
	//INVOKE(0x00AC8CE0, chud_draw_bitmap_widget, user_index, draw_widget_data, is_draw_turbulence);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(, chud_draw_bitmap_widget, user_index, draw_widget_data, is_draw_turbulence);

	if (chud_compute_render_data_result)
		rasterizer_profile_end_event();
}

//.text:00AC8E50 ; void __cdecl chud_draw_debug(long user_index)
//.text:00AC8E60 ; void __cdecl chud_draw_end(long user_index, bool resolve_cortana_effect)
//.text:00AC8E80 ; void __cdecl chud_draw_initialize()
//.text:00AC8E90 ; 

void __cdecl chud_draw_text_widget(long user_index, void* draw_widget_data, bool is_draw_turbulence)
{
	//INVOKE(0x00AC9490, chud_draw_text_widget, user_index, draw_widget_data, is_draw_turbulence);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(, chud_draw_text_widget, user_index, draw_widget_data, is_draw_turbulence);

	if (chud_compute_render_data_result)
		rasterizer_profile_end_event();
}

//.text:00AC9560 ; void __cdecl chud_draw_training_text(long user_index)
//.text:00AC9570 ; void __cdecl chud_draw_widget(long user_index, s_chud_runtime_widget_datum* widget, long chud_definition_index, long is_draw_turbulence)
//.text:00AC9620 ; void __cdecl chud_draw_widget_geometry(s_widget_geometry* geometry, s_anchor_basis* anchor_basis, bool mirror_x, bool mirror_y)
//.text:00AC9AA0 ; void __cdecl chud_expand_pixel32(dword value, vector4d* result)
//.text:00AC9B50 ; void __cdecl chud_geometry_build_transform(s_widget_geometry const* geometry, real_matrix4x3* widget_transform)
//.text:00AC9C70 ; void __cdecl chud_get_crosshair_screen_position(long user_index, real_point2d* position)
//.text:00AC9DD0 ; long __cdecl chud_get_current_resolution(long user_index)
//.text:00AC9ED0 ; 
//.text:00AC9F50 ; 
//.text:00AC9FD0 ; 
//.text:00ACA020 ; void __cdecl chud_render_data_fetch_color_output(long user_index, short color_output_type, s_chud_widget_render_data* render_data, real_vector4d* result)
//.text:00ACA300 ; real __cdecl chud_render_data_fetch_scalar_output(short scalar_output_type, s_chud_widget_render_data* render_data, s_chud_runtime_widget_datum* widget)
//.text:00ACA380 ; bool __cdecl chud_text_widget_compute_geometry(s_chud_draw_widget_data* draw_widget_data, s_widget_geometry* result_geometry, s_chud_render_data* render_data)
//.text:00ACA420 ; bool __cdecl chud_text_widget_compute_string(s_chud_draw_widget_data* draw_widget_data, s_chud_render_data* render_data, c_static_wchar_string<256>* parsed_text)
//.text:00ACAD00 ; void __cdecl chud_widget_build_quads_from_bounds(real_rectangle2d* actual_geometry_bounds, real_rectangle2d* extended_geometry_bounds, real_rectangle2d* texture_bounds, s_widget_geometry* result_geometry, real scale_x, real scale_y)
//.text:00ACAFA0 ; void __cdecl chud_widget_geometry_add_character(s_widget_geometry* geometry, real_rectangle2d const* src_rectangle, real_rectangle2d const* dest_rectangle, real character_scale)
//.text:00ACB0E0 ; 

