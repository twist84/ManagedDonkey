#include "render/render_objects.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE_CLASS(0x00A78410, c_object_renderer, render_albedo);

//.text:00A78140 ; c_object_renderer::initialize
//.text:00A781C0 ; 
//.text:00A781F0 ; c_object_renderer::pop_marker
//.text:00A78230 ; c_object_renderer::push_marker
//.text:00A78280 ; c_object_renderer::render_active_camo_object_context

void __cdecl c_object_renderer::render_albedo(dword flags)
{
	//INVOKE(0x00A78410, c_object_renderer::render_albedo, flags);

	c_rasterizer_profile_scope _objects_albedo(_rasterizer_profile_element_total, L"objects_albedo");

	HOOK_INVOKE_CLASS(, c_object_renderer, render_albedo, decltype(&c_object_renderer::render_albedo), flags);
}

//.text:00A78470 ; c_object_renderer::render_albedo_decals
//.text:00A78480 ; 
//.text:00A784F0 ; c_object_renderer::render_lights

void __cdecl c_object_renderer::submit_and_render_sky(long a1, long player_index)
{
	INVOKE(0x00A79300, c_object_renderer::submit_and_render_sky, a1, player_index);
}

