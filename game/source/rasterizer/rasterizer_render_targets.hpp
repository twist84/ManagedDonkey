#pragma once

#include "cseries/cseries.hpp"

__interface IDirect3DSurface9;
__interface IDirect3DTexture9;

struct c_render_surface
{
	IDirect3DSurface9* m_d3d_surface;
	IDirect3DTexture9* m_d3d_texture;

	void* m_surface_description;
	uns32 m_surface_width;
	uns32 m_surface_height;
	uns8 unknown14[20];
	uns32 m_render_target_format;
	uns8 m_render_state;
	uns8 unknown2C[12];
	uns32 unknown60;
	uns8 unknown64[16];
	IDirect3DSurface9* m_d3d_resolve_surface;
	uns8 unknown54[36];
};

