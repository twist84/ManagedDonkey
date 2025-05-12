#include "rasterizer/rasterizer_text.hpp"

REFERENCE_DECLARE(0x01910748, s_hardware_character_cache_globals, hardware_character_cache_globals);

void __cdecl rasterizer_bitmap_2d_changed_rect(bitmap_data const* bitmap, c_rasterizer_texture_ref texture_ref, rectangle2d const* dirty_rect, bool a4)
{
	INVOKE(0x00A271E0, rasterizer_bitmap_2d_changed_rect, bitmap, texture_ref, dirty_rect, a4);
}

void __cdecl c_rasterizer_texture_ref::allocate(c_rasterizer_texture_ref& texture_ref, int32 width, int32 height, int32 mipmaps, uns32 format, int32 multisample_type, bool use_srgb_curve, e_bitmap_type type, int32 a9)
{
	//INVOKE(0x00A6DEF0, c_rasterizer_texture_ref::allocate, texture_ref, width, height, mipmaps, format, multisample_type, use_srgb_curve, type, a9);
	DECLFUNC(0x00A6DEF0, void, __cdecl, c_rasterizer_texture_ref&, int32, int32, int32, uns32, int32, bool, int32, int32)(
		texture_ref,
		width,
		height,
		mipmaps,
		format,
		multisample_type,
		use_srgb_curve,
		type,
		a9);
}

c_rasterizer_texture_ref& __cdecl c_rasterizer_texture_ref::allocate(c_rasterizer_texture_ref& texture_ref, bitmap_data const* bitmap, char const* name, bool system_memory)
{
	//return INVOKE_CLASS_MEMBER(0x00A6DFF0, c_rasterizer_texture_ref::allocate, bitmap, name, system_memory);
	return DECLFUNC(0x00A6DFF0, c_rasterizer_texture_ref&, __cdecl, c_rasterizer_texture_ref&, bitmap_data const*, char const*, bool)(
		texture_ref,
		bitmap,
		name,
		system_memory);
}

void __cdecl c_rasterizer_texture_ref::dispose()
{
	INVOKE(0x00A6E7B0, c_rasterizer_texture_ref::dispose);
}

IDirect3DTexture9* c_rasterizer_texture_ref::get_d3d_texture() const
{
	return INVOKE_CLASS_MEMBER(0x00A6E8E0, c_rasterizer_texture_ref, get_d3d_texture);
}

bool c_rasterizer_texture_ref::get_width_height(int32 level, int32* width, int32* height)
{
	return INVOKE_CLASS_MEMBER(0x00A6E950, c_rasterizer_texture_ref, get_width_height, level, width, height);
}

void __cdecl c_rasterizer_texture_ref::initialize()
{
	INVOKE(0x00A6E9C0, c_rasterizer_texture_ref::initialize);
}

void __cdecl c_rasterizer_texture_ref::release(c_rasterizer_texture_ref& texture_ref)
{
	INVOKE(0x00A6EB10, c_rasterizer_texture_ref::release, texture_ref);
}

void* c_rasterizer_texture_ref::lock(int32* pitch)
{
	return INVOKE_CLASS_MEMBER(0x00A74150, c_rasterizer_texture_ref, lock, pitch);
}

void c_rasterizer_texture_ref::unlock()
{
	INVOKE_CLASS_MEMBER(0x00A74210, c_rasterizer_texture_ref, unlock);
}

bool __cdecl c_rasterizer_texture_ref::valid()
{
	return m_datum_ref && m_datum_ref != NONE;
}

