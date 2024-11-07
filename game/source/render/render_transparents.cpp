#include "render/render_transparents.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x05216204, short, render_debug_transparent_sort_method);

//.text:00A73580 ; public: static void __cdecl c_transparency_renderer::pop_marker()
//.text:00A735D0 ; public: static void __cdecl c_transparency_renderer::push_marker()
//.text:00A73620 ; 

void __cdecl c_transparency_renderer::render(bool a1)
{
	INVOKE(0x00A73640, c_transparency_renderer::render, a1);
}

//.text:00A736E0 ; public: static void __cdecl c_transparency_renderer::reset()

void __cdecl c_transparency_renderer::set_active_camo_bounds(short_rectangle2d const* rect_a, short_rectangle2d const* rect_b)
{
	INVOKE(0x00A73710, c_transparency_renderer::set_active_camo_bounds, rect_a, rect_b);
}

//.text:00A73770 ; public: static void __cdecl c_transparency_renderer::resolve_framebuffer_for_active_camo()

void __cdecl c_transparency_renderer::sort()
{
	INVOKE(0x00A73880, c_transparency_renderer::sort);
}

