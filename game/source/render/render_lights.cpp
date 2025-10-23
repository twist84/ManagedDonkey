#include "render/render_lights.hpp"

#include "cseries/cseries.hpp"
#include "render/views/render_view.hpp"

REFERENCE_DECLARE(0x05115B3C, bool, render_default_lighting);

void c_lights_view::submit_simple_light_draw_list_to_shader() const
{
	INVOKE_CLASS_MEMBER(0x00A3BF70, c_lights_view, submit_simple_light_draw_list_to_shader);
}

void c_lights_view::build_simple_light_draw_list(int32 a1)
{
	INVOKE_CLASS_MEMBER(0x00A669E0, c_lights_view, build_simple_light_draw_list, a1);
}

void c_lights_view::clear_simple_light_draw_list(int32 a1)
{
	INVOKE_CLASS_MEMBER(0x00A66A20, c_lights_view, clear_simple_light_draw_list, a1);
}

void __cdecl render_light_set_cinematic_shadow(bool enable, int32 object_index, int32 marker_id, real32 radius)
{
	//INVOKE(0x00A67140, render_light_set_cinematic_shadow, enable, object_index, marker_id, radius);
}

