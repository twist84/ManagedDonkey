#include "rasterizer/rasterizer_render_targets.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_text.hpp"
#include "render/screen_postprocess.hpp"

#include <d3d9.h>

struct c_render_surface
{
	IDirect3DSurface9* m_d3d_surface;
	IDirect3DTexture9* m_d3d_texture;
};

// c_render_surface* c_render_surfaces_interface::get_render_surface_default(c_rasterizer::e_surface)
c_render_surface* __cdecl get_render_surface_default(c_rasterizer::e_surface surface)
{
	return INVOKE(0x0060F8B0, get_render_surface_default, surface);
}

// `sub_A487E0` gets called in `render_bitmap` where ui blur is supposed to be handled
// giving us the `texture_ref` that would be set
// `unknown` is `2` and potentially a surface (that of `display`)
c_rasterizer_texture_ref* __cdecl sub_A487E0(c_rasterizer_texture_ref* texture_ref, long unknown)
{
	c_rasterizer_texture_ref* result = INVOKE(0x00A487E0, sub_A487E0, texture_ref, unknown);

	c_screen_postprocess::setup_rasterizer_for_postprocess(0);
	c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_none);

	// this `blur_display` uses the `screenshot_display` surface and not `display`
	c_rasterizer::e_surface surface = c_screen_postprocess::blur_display();

	// for some reason when using the correct surface `display` no ui renders at all
	//c_rasterizer::set_render_target(0, c_rasterizer::_surface_display, NONE);

	// use the `screenshot_display` surface
	c_rasterizer::set_render_target(0, c_rasterizer::_surface_screenshot_display, NONE);

	c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil);
	c_rasterizer::restore_last_viewport();
	c_rasterizer::restore_last_scissor_rect();
	
	result->m_datum_ref = (dword)get_render_surface_default(surface)->m_d3d_texture;

	return result;
}

//#define ISEXPERIMENTAL_NO_UI_BACKGROUND

#if defined(ISEXPERIMENTAL_NO_UI_BACKGROUND) // ISEXPERIMENTAL_UI_BACKGROUND_BLUR
HOOK_DECLARE_CALL(0x00B17313, sub_A487E0);
#endif // ISEXPERIMENTAL_NO_UI_BACKGROUND

