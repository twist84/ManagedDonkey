#include "render/render_objects.hpp"

#include "memory/module.hpp"
#include "objects/widgets/widgets.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_mesh.hpp"
#include "render/views/render_view.hpp"

HOOK_DECLARE_CLASS(0x00A78410, c_object_renderer, render_albedo);

//.text:00A78140 ; public: static void __cdecl c_object_renderer::initialize()
//.text:00A781A0 ; public: static void __cdecl c_render_flags::or_window_bit(c_render_flags::e_window_bit, uint32)
//.text:00A781C0 ; void __cdecl override_shield_impact_parameters(long, s_shield_impact_parameters const*)
//.text:00A781F0 ; public: static void __cdecl c_object_renderer::pop_marker()
//.text:00A78230 ; public: static void __cdecl c_object_renderer::push_marker()
//.text:00A78280 ; public: static void __cdecl c_object_renderer::render_active_camo_object_context(void const*, long)

void __cdecl c_object_renderer::render_albedo(uint32 mesh_part_flags)
{
	//INVOKE(0x00A78410, c_object_renderer::render_albedo, flags);

	c_rasterizer_profile_scope _objects_albedo(_rasterizer_profile_element_total, L"objects_albedo");

	c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_write);
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);
	c_rasterizer::set_color_write_enable(0, 15);
	c_rasterizer::set_color_write_enable(1, 15);

	begin_render_mesh_part_only();
	c_object_renderer::render_object_contexts(_entry_point_albedo, mesh_part_flags);
	end_render_mesh_part_only();

	c_object_renderer::render_object_widgets(_entry_point_albedo);

	if (!c_view::get_current_stack_level())
		widgets_render_unattached(_entry_point_albedo);
}

//.text:00A78470 ; public: static void __cdecl c_object_renderer::render_albedo_decals()
//.text:00A78480 ; 
//.text:00A784F0 ; public: static void __cdecl c_object_renderer::render_lights(e_entry_point)
//.text:00A78530 ; public: static void __cdecl c_object_renderer::render_object_context(long, e_entry_point, long, bool)
//.text:00A786B0 ; public: static void __cdecl c_object_renderer::render_object_context_instances(s_context_mesh_part const*, e_entry_point, uint32 const*, s_lod_transparency, long)
//.text:00A78940 ; public: static void __cdecl c_object_renderer::render_object_context_mesh_part(s_context_mesh_part const*, e_entry_point, long, bool)

void __cdecl c_object_renderer::render_object_contexts(e_entry_point entry_point, long mesh_part_mask)
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
//.text:00A78F00 ; public: static void __cdecl c_object_renderer::render_shadows_generate(long)
//.text:00A78F50 ; public: static void __cdecl c_object_renderer::render_shield_impact_mesh_part(void const*, long)
//.text:00A79060 ; public: static void __cdecl c_object_renderer::render_static_lighting(long)
//.text:00A790E0 ; public: static void __cdecl c_object_renderer::render_transparent_object_mesh_part(void const*, long)
//.text:00A79160 ; 
//.text:00A79180 ; public: static void __cdecl c_object_renderer::render_unattached_widgets(e_entry_point)
//.text:00A791A0 ; 
//.text:00A791E0 ; 
//.text:00A79220 ; public: static void __cdecl c_object_renderer::reset()
//.text:00A79260 ; public: static void __cdecl c_object_renderer::setup_shared_state_for_mesh_part(s_context_mesh_part const*, bool)

void __cdecl c_object_renderer::submit_and_render_sky(long entry_point_type, long player_window_index)
{
	INVOKE(0x00A79300, c_object_renderer::submit_and_render_sky, entry_point_type, player_window_index);
}

//.text:00A793E0 ; public: static void __cdecl c_object_renderer::submit_attachments(long)
//.text:00A79470 ; public: static void __cdecl c_object_renderer::submit_object_attachments(long, long)
//.text:00A796E0 ; public: static void __cdecl c_object_renderer::submit_object_hierarchy(long, long, s_lod_transparency, long, uint32)
//.text:00A798B0 ; public: static void __cdecl c_object_renderer::submit_object_hierarchy_for_special_traits_recursive(long*, long*, long, render_lighting*, s_lod_transparency, long, real_point3d*, uint32)
//.text:00A79BA0 ; public: static void __cdecl c_object_renderer::submit_object_hierarchy_recursive(long*, long*, long, s_object_render_context*, long, uint32, uint32*)
//.text:00A79D80 ; public: static void __cdecl c_object_renderer::submit_object_instances(s_object_render_context*, s_visible_object_render_visibility*, long)
//.text:00A79DE0 ; public: static void __cdecl c_object_renderer::submit_object_mesh_parts(s_object_render_context*, s_visible_object_render_visibility*, long, bool, bool, bool, bool)
//.text:00A7A280 ; public: static void __cdecl c_object_renderer::submit_object_mesh_parts_for_active_camo(s_object_render_context*, s_visible_object_render_visibility*, bool, bool)
//.text:00A7A4F0 ; 
//.text:00A7A510 ; public: static void __cdecl c_object_renderer::submit_visibility(uint32)

