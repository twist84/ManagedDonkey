#pragma once

#include "render/render_cameras.hpp"
// 0165DB98
struct c_view
{
public:
	virtual void __cdecl render();
	virtual long __cdecl render_setup();
	virtual long __cdecl compute_visibility();
	virtual long __cdecl render_submit_visibility();

	static long& g_view_stack_top;
	static c_view*(&g_view_stack)[4];

	render_camera m_rasterizer_camera;
	render_projection m_rasterizer_projection;
	render_camera m_render_camera;
	render_projection m_render_projection;
};
static_assert(sizeof(c_view) == 0x294);

