#pragma once

#include "cseries/cseries.hpp"

__interface IDirect3DSurface9;
__interface IDirect3DTexture9;

struct c_render_surface
{
	IDirect3DSurface9* m_d3d_surface;
	IDirect3DTexture9* m_d3d_texture;

	void* m_surface_description;
	uint32 m_surface_width;
	uint32 m_surface_height;
	uint8 unknown14[20];
	uint32 m_render_target_format;
	uint8 m_render_state;
	uint8 unknown2C[12];
	uint32 unknown60;
	uint8 unknown64[16];
	IDirect3DSurface9* m_d3d_resolve_surface;
	uint8 unknown54[36];
};

