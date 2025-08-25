#pragma once

#include "cseries/cseries.hpp"
#include "rasterizer/rasterizer.hpp"

__interface IDirect3DSurface9;
__interface IDirect3DTexture9;

class c_render_surface
{
public:
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

// $TODO move this
struct s_surface_group_description
{
	uns32 flags;
	IDirect3DTexture9* Texture;
	real32 DepthWidth;
	real32 __unknownC;
	uns32 DepthFormat;
	byte __unknown14[4];
	uns32 Srgb;
	uns32 MultiSample;
	real32 __unknown20;
	real32 __unknown24;
	uns32 __unknown28;
	uns32 __unknown2C;
	uns32 __unknown30;
	uns32 __unknown34;
	uns32 __unknown38;
	uns32 __unknown3C;
	uns32 Alias;
	uns32 __unknown44;
	uns32 index_unknown_of_size_54;
};

// $TODO move this
class c_render_surface_group
{
public:
	s_surface_group_description description;
	int32 m_count_specializations;
	c_render_surface* m_surfaces;
};

// $TODO move this
class c_render_surfaces_interface
{
public:
	static c_render_surface* get_render_surface(c_rasterizer::e_surface surface);

	static c_render_surface_group(&m_render_surface_groups)[60];
};

