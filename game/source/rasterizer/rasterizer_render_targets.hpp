#pragma once

#include "cseries/cseries.hpp"

__interface IDirect3DSurface9;
__interface IDirect3DTexture9;

struct c_render_surface
{
	IDirect3DSurface9* m_d3d_surface;
	IDirect3DTexture9* m_d3d_texture;

	void* m_surface_description;
	dword m_surface_width;
	dword m_surface_height;
	byte unknown14[20];
	dword m_render_target_format;
	byte m_render_state;
	byte unknown2C[12];
	dword unknown60;
	byte unknown64[16];
	IDirect3DSurface9* m_d3d_resolve_surface;
	byte unknown54[36];
};

