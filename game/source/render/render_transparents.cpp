#include "render/render_transparents.hpp"

using transparent_sorted_order_t = c_sorter<s_transparent_types, 768>;

//REFERENCE_DECLARE(0x0191B560, int32, m_current_marker_index);
//REFERENCE_DECLARE(0x0191B590, transparent_sorted_order_t, transparent_sorted_order);
//REFERENCE_DECLARE(0x05216201, bool, m_using_active_camo);
//REFERENCE_DECLARE(0x05216202, bool, m_needs_active_camo_ldr_resolve);
REFERENCE_DECLARE(0x05216204, int16, render_debug_transparent_sort_method);
//REFERENCE_DECLARE_2D_ARRAY(0x0521620C, s_transparency_marker, m_markers, 6, 3);
//REFERENCE_DECLARE(0x05216230, int32, m_total_transparent_count);
//REFERENCE_DECLARE(0x05216234, rectangle2d, m_active_camo_resolve_bounds);
//REFERENCE_DECLARE(0x0521623C, real_vector4d, m_active_camo_distort_bounds);
//REFERENCE_DECLARE_STATIC_ARRAY(0x05216250, s_transparent_types, 768, transparents);

//.text:00A73580 ; public: static void __cdecl c_transparency_renderer::pop_marker()
//.text:00A735D0 ; public: static void __cdecl c_transparency_renderer::push_marker()
//.text:00A73620 ; 

void __cdecl c_transparency_renderer::render(bool depth_test)
{
	INVOKE(0x00A73640, c_transparency_renderer::render, depth_test);
}

//.text:00A736E0 ; public: static void __cdecl c_transparency_renderer::reset()

void __cdecl c_transparency_renderer::set_active_camo_bounds(const rectangle2d* window_pixel_bounds, const rectangle2d* render_pixel_bounds)
{
	INVOKE(0x00A73710, c_transparency_renderer::set_active_camo_bounds, window_pixel_bounds, render_pixel_bounds);
}

//.text:00A73770 ; public: static void __cdecl c_transparency_renderer::resolve_framebuffer_for_active_camo()

void __cdecl c_transparency_renderer::sort()
{
	INVOKE(0x00A73880, c_transparency_renderer::sort);
}

