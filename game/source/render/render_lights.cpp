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

//.text:00A67150 ; 
//.text:00A67160 ; 
//.text:00A67170 ; public: virtual void c_lights_view::render_setup()
//.text:00A671B0 ; public: virtual void c_lights_view::render_submit_visibility()
//.text:00A671D0 ; public: void c_lights_view::setup_clip_planes()
//.text:00A67340 ; public: bool c_lights_view::setup_shadow_camera_and_projection(const light_state*, const light_geometry*, int32, int32)
//.text:00A67B30 ; public: static void __cdecl c_lights_view::submit_attachments(int32)
//.text:00A67B90 ; public: static void __cdecl c_lights_view::submit_light_draw_list_to_shader(const s_simple_light*, int32)

void c_lights_view::submit_simple_light_draw_list_to_vertex_shader(int32 light_count_integer_register, int32 light_data_float_register_start)
{
	INVOKE_CLASS_MEMBER(0x00A67BE0, c_lights_view, submit_simple_light_draw_list_to_vertex_shader, light_count_integer_register, light_data_float_register_start);
}

