#include "rasterizer/rasterizer_text.hpp"

void __cdecl c_rasterizer_texture_ref::allocate(c_rasterizer_texture_ref& texture_ref, long width, long height, long levels, dword format, long a6, bool a7, long a8, long a9)
{
	INVOKE(0x00A6DEF0, c_rasterizer_texture_ref::allocate, texture_ref, width, height, levels, format, a6, a7, a8, a9);
}

IDirect3DTexture9* __cdecl c_rasterizer_texture_ref::get_d3d_texture() const
{
	return INVOKE_CLASS_MEMBER(0x00A6E8E0, c_rasterizer_texture_ref::get_d3d_texture);
}

void __cdecl c_rasterizer_texture_ref::release(c_rasterizer_texture_ref& texture_ref)
{
	INVOKE(0x00A6EB10, c_rasterizer_texture_ref::release, texture_ref);
}

bool __cdecl c_rasterizer_texture_ref::valid()
{
	return m_datum_ref && m_datum_ref != -1;
}

