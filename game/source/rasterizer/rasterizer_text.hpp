#pragma once

#include "cseries/cseries.hpp"

struct IDirect3DTexture9;
struct c_rasterizer_texture_ref
{
public:
	static void __cdecl allocate(c_rasterizer_texture_ref& texture_ref, long width, long height, long levels, dword format, long a6, bool a7, long a8, long a9);
	IDirect3DTexture9* __cdecl get_d3d_texture() const;
	static void __cdecl release(c_rasterizer_texture_ref& texture_ref);

	bool __cdecl valid();

//protected:
	dword m_datum_ref;
};
static_assert(sizeof(c_rasterizer_texture_ref) == 0x4);

