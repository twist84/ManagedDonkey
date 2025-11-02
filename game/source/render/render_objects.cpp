#include "render/render_objects.hpp"

#include "memory/module.hpp"
#include "objects/widgets/widgets.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_mesh.hpp"
#include "render/views/render_view.hpp"

HOOK_DECLARE_CLASS(0x00A78410, c_object_renderer, render_albedo);

//.text:00A78140 ; public: static void __cdecl c_object_renderer::initialize()
//.text:00A781A0 ; public: static void __cdecl c_render_flags::or_window_bit(c_render_flags::e_window_bit, uns32)
//.text:00A781C0 ; void __cdecl override_shield_impact_parameters(int32, const s_shield_impact_parameters*)

void __cdecl c_object_renderer::pop_marker()
{
	INVOKE(0x00A781F0, c_object_renderer::pop_marker);
}

void __cdecl c_object_renderer::push_marker()
{
	INVOKE(0x00A78230, c_object_renderer::push_marker);
}

//.text:00A78280 ; public: static void __cdecl c_object_renderer::render_active_camo_object_context(const void*, int32)

void __cdecl c_object_renderer::render_albedo(uns32 mesh_part_flags)
{
	//INVOKE(0x00A78410, c_object_renderer::render_albedo, flags);

	c_rasterizer_profile_scope _objects_albedo(_rasterizer_profile_element_total, L"objects_albedo");

	c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_write);
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);
	c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_all);
	c_rasterizer::set_color_write_enable(1, c_rasterizer::_color_write_enable_all);

	begin_render_mesh_part_only();
	c_object_renderer::render_object_contexts(_entry_point_albedo, mesh_part_flags);
	end_render_mesh_part_only();

	c_object_renderer::render_object_widgets(_entry_point_albedo);

	if (!c_view::get_current_stack_level())
		widgets_render_unattached(_entry_point_albedo);
}

void __cdecl c_object_renderer::render_albedo_decals()
{
	INVOKE(0x00A78470, c_object_renderer::render_albedo_decals);
}

//.text:00A78480 ; 
//.text:00A784F0 ; public: static void __cdecl c_object_renderer::render_lights(e_entry_point)
//.text:00A78530 ; public: static void __cdecl c_object_renderer::render_object_context(int32, e_entry_point, int32, bool)
//.text:00A786B0 ; public: static void __cdecl c_object_renderer::render_object_context_instances(const s_context_mesh_part*, e_entry_point, const uns32*, s_lod_transparency, int32)
//.text:00A78940 ; public: static void __cdecl c_object_renderer::render_object_context_mesh_part(const s_context_mesh_part*, e_entry_point, int32, bool)

void __cdecl c_object_renderer::render_object_contexts(e_entry_point entry_point, int32 mesh_part_mask)
{
	INVOKE(0x00A78A50, c_object_renderer::render_object_contexts, entry_point, mesh_part_mask);
}

void __cdecl c_object_renderer::render_object_widgets(e_entry_point entry_point)
{
	INVOKE(0x00A78C40, c_object_renderer::render_object_widgets, entry_point);
}

//.text:00A78CD0 ; public: static void __cdecl c_object_renderer::render_occlusions()
//.text:00A78D10 ; 
//.text:00A78EC0 ; public: static void __cdecl c_object_renderer::render_shadows_apply()
//.text:00A78F00 ; public: static void __cdecl c_object_renderer::render_shadows_generate(int32)
//.text:00A78F50 ; public: static void __cdecl c_object_renderer::render_shield_impact_mesh_part(const void*, int32)
//.text:00A79060 ; public: static void __cdecl c_object_renderer::render_static_lighting(int32)
//.text:00A790E0 ; public: static void __cdecl c_object_renderer::render_transparent_object_mesh_part(const void*, int32)
//.text:00A79160 ; 
//.text:00A79180 ; public: static void __cdecl c_object_renderer::render_unattached_widgets(e_entry_point)
//.text:00A791A0 ; 
//.text:00A791E0 ; 
//.text:00A79220 ; public: static void __cdecl c_object_renderer::reset()
//.text:00A79260 ; public: static void __cdecl c_object_renderer::setup_shared_state_for_mesh_part(const s_context_mesh_part*, bool)

void __cdecl c_object_renderer::submit_and_render_sky(int32 entry_point_type, int32 player_window_index)
{
	INVOKE(0x00A79300, c_object_renderer::submit_and_render_sky, entry_point_type, player_window_index);
}

//.text:00A793E0 ; public: static void __cdecl c_object_renderer::submit_attachments(int32)
//.text:00A79470 ; public: static void __cdecl c_object_renderer::submit_object_attachments(int32, int32)
//.text:00A796E0 ; public: static void __cdecl c_object_renderer::submit_object_hierarchy(int32, int32, s_lod_transparency, int32, uns32)
//.text:00A798B0 ; public: static void __cdecl c_object_renderer::submit_object_hierarchy_for_special_traits_recursive(int32*, int32*, int32, render_lighting*, s_lod_transparency, int32, real_point3d*, uns32)
//.text:00A79BA0 ; public: static void __cdecl c_object_renderer::submit_object_hierarchy_recursive(int32*, int32*, int32, s_object_render_context*, int32, uns32, uns32*)
//.text:00A79D80 ; public: static void __cdecl c_object_renderer::submit_object_instances(s_object_render_context*, s_visible_object_render_visibility*, int32)
//.text:00A79DE0 ; public: static void __cdecl c_object_renderer::submit_object_mesh_parts(s_object_render_context*, s_visible_object_render_visibility*, int32, bool, bool, bool, bool)
//.text:00A7A280 ; public: static void __cdecl c_object_renderer::submit_object_mesh_parts_for_active_camo(s_object_render_context*, s_visible_object_render_visibility*, bool, bool)
//.text:00A7A4F0 ; 

void __cdecl c_object_renderer::submit_visibility(uns32 submit_visibility_flags)
{
	INVOKE(0x00A7A510, c_object_renderer::submit_visibility, submit_visibility_flags);
}

