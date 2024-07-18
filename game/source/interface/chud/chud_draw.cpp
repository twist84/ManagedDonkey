#include "interface/chud/chud_draw.hpp"

#include "camera/observer.hpp"
#include "memory/module.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"

#include <cmath>

REFERENCE_DECLARE(0x0191CC64, bool, chud_enabled);
REFERENCE_DECLARE(0x052604C1, bool, chud_debug_messages);
REFERENCE_DECLARE(0x0526A085, bool, chud_debug_crosshair);
REFERENCE_DECLARE(0x0526A087, bool, chud_debug_grid);

HOOK_DECLARE(0x00AC7F80, chud_debug_draw_reticle_labeled);

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

