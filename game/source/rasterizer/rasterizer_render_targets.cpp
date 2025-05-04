#include "rasterizer/rasterizer_render_targets.hpp"


c_render_surface* c_render_surfaces_interface::get_render_surface(c_rasterizer::e_surface surface)
{
	return INVOKE(0x0060F8B0, c_render_surfaces_interface::get_render_surface, surface);

	//return m_render_surface_groups[surface].m_surfaces;
}

