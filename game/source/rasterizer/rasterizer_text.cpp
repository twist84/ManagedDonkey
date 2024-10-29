#include "rasterizer/rasterizer_text.hpp"

REFERENCE_DECLARE(0x01910748, s_hardware_character_cache_globals, hardware_character_cache_globals);

void __cdecl rasterizer_bitmap_2d_changed_rect(bitmap_data const* bitmap, c_rasterizer_texture_ref texture_ref, short_rectangle2d const* dirty_rect, bool a4)
{
	INVOKE(0x00A271E0, rasterizer_bitmap_2d_changed_rect, bitmap, texture_ref, dirty_rect, a4);
}

void __cdecl c_rasterizer_texture_ref::allocate(c_rasterizer_texture_ref& texture_ref, long width, long height, long levels, dword format, long a6, bool a7, long a8, long a9)
{
	INVOKE(0x00A6DEF0, c_rasterizer_texture_ref::allocate, texture_ref, width, height, levels, format, a6, a7, a8, a9);
}

IDirect3DTexture9* c_rasterizer_texture_ref::get_d3d_texture() const
{
	return INVOKE_CLASS_MEMBER(0x00A6E8E0, c_rasterizer_texture_ref, get_d3d_texture);
}

void __cdecl c_rasterizer_texture_ref::initialize()
{
	INVOKE(0x00A6E9C0, c_rasterizer_texture_ref::initialize);
}

void __cdecl c_rasterizer_texture_ref::release(c_rasterizer_texture_ref& texture_ref)
{
	INVOKE(0x00A6EB10, c_rasterizer_texture_ref::release, texture_ref);
}

bool __cdecl c_rasterizer_texture_ref::valid()
{
	return m_datum_ref && m_datum_ref != NONE;
}

