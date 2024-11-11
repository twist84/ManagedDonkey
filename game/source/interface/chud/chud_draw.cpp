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
REFERENCE_DECLARE(0x0526A085, bool, chud_debug_crosshair);
REFERENCE_DECLARE(0x0526A087, bool, chud_debug_grid);

HOOK_DECLARE(0x00AC7B80, chud_compute_render_data);
HOOK_DECLARE(0x00AC7F80, chud_debug_draw_reticle_labeled);
HOOK_DECLARE(0x00AC8CE0, chud_draw_bitmap_widget);
HOOK_DECLARE(0x00AC9490, chud_draw_text_widget);

bool chud_compute_render_data_result = false;

//bool __cdecl chud_compute_render_data(s_chud_draw_widget_data* draw_widget_data, s_chud_render_data* render_data, bool a3)
bool __cdecl chud_compute_render_data(void* draw_widget_data, void* render_data, bool a3)
{
	//INVOKE(0x00AC7B80, chud_compute_render_data, draw_widget_data, render_data, a3);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(chud_compute_render_data_result =, chud_compute_render_data, draw_widget_data, render_data, a3);

	if (chud_compute_render_data_result)
	{
		static c_static_wchar_string<128> pix_name;
		REFERENCE_DECLARE(offset_pointer(draw_widget_data, 0x10), void*, draw_widget_data10);
		REFERENCE_DECLARE(draw_widget_data10, c_string_id, name_id);
		rasterizer_profile_begin_event(_rasterizer_profile_element_interface_hud, pix_name.print(L"%hs", name_id.get_string()));
	}

	return chud_compute_render_data_result;
}

void __cdecl chud_debug_draw(e_output_user_index output_user_index)
{
	INVOKE(0x00AC7DB0, chud_debug_draw, output_user_index);
}

void __cdecl chud_debug_draw_reticle(e_output_user_index output_user_index, real a2, real_argb_color const* color)
{
	//INVOKE(0x00AC7F50, chud_debug_draw_reticle, output_user_index, a2, color);

	chud_debug_draw_reticle_labeled(output_user_index, a2, NULL, 0.0f, color);
}

void __cdecl chud_debug_draw_reticle_labeled(e_output_user_index output_user_index, real a2, char const* label, real a4, real_argb_color const* color)
{
	//INVOKE(0x00AC7F80, chud_debug_draw_reticle_labeled, output_user_index, a2, label, a4, color);

	real unknown64 = observer_get(output_user_index)->result.__unknown64;
	c_player_view* player_view = c_player_view::get_current();
	render_projection const* rasterizer_projection = player_view->get_rasterizer_projection();

	real v2 = tanf(a2) / CIRCLE_DIVISIONS;
	real_point3d points[CIRCLE_DIVISIONS]{};

	real current_angle = 0.0f;
	for (long i = 0; i < CIRCLE_DIVISIONS; i++)
	{
		set_real_point3d(&points[i], cosf(current_angle) * v2, real(sinf(current_angle) * v2) - real(unknown64 * (1.0f / CIRCLE_DIVISIONS)), -(1.0f / CIRCLE_DIVISIONS));
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
	//		e_output_user_index user_index = player_mapping_first_active_output_user();
	//		if (user_index != k_input_user_none)
	//		{
	//			// #TODO: implement me
	//		}
	//	}
	//
	//	if (chud_debug_crosshair)
	//	{
	//		e_output_user_index user_index = player_mapping_first_active_output_user();
	//		if (user_index != k_input_user_none)
	//			chud_debug_draw(user_index);
	//	}
	//}
}

//.text:00AC80E0 ; 
//.text:00AC80F0 ; void __cdecl chud_draw_begin(long, real, long, bool)

void __cdecl chud_draw_bitmap_widget(long a1, void* a2, bool a3)
{
	//INVOKE(0x00AC8CE0, chud_draw_bitmap_widget, a1, a2, a3);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(, chud_draw_bitmap_widget, a1, a2, a3);

	if (chud_compute_render_data_result)
		rasterizer_profile_end_event();
}

//.text:00AC8E50 ; 
//.text:00AC8E60 ; void __cdecl chud_draw_end(long, bool)
//.text:00AC8E80 ; void __cdecl chud_draw_initialize()
//.text:00AC8E90 ; 

void __cdecl chud_draw_text_widget(long a1, void* a2, bool a3)
{
	//INVOKE(0x00AC9490, chud_draw_text_widget, a1, a2, a3);

	chud_compute_render_data_result = false;

	HOOK_INVOKE(, chud_draw_text_widget, a1, a2, a3);

	if (chud_compute_render_data_result)
		rasterizer_profile_end_event();
}

//.text:00AC9560 ; 
//.text:00AC9570 ; void __cdecl chud_draw_widget(e_output_user_index, s_chud_runtime_widget_datum*, long, long)
//.text:00AC9620 ; void __cdecl chud_draw_widget_geometry(s_widget_geometry*, s_anchor_basis*, bool, bool)
//.text:00AC9AA0 ; void __cdecl chud_expand_pixel32(dword, vector4d*)
//.text:00AC9B50 ; void __cdecl chud_geometry_build_transform(s_widget_geometry_properties const&, real_matrix4x3*)

