#include "rasterizer/rasterizer_text.hpp"

REFERENCE_DECLARE(0x01910748, s_hardware_character_cache_globals, hardware_character_cache_globals);

void __cdecl rasterizer_bitmap_2d_changed_rect(bitmap_data const* bitmap, c_rasterizer_texture_ref hardware_format, rectangle2d const* dirty_rect, bool unused_wait_until_idle)
{
	INVOKE(0x00A271E0, rasterizer_bitmap_2d_changed_rect, bitmap, hardware_format, dirty_rect, unused_wait_until_idle);
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

uns32 c_rasterizer_texture_ref::get_d3d_format()
{
	return INVOKE_CLASS_MEMBER(0x00A6E820, c_rasterizer_texture_ref, get_d3d_format);
}

IDirect3DSurface9* c_rasterizer_texture_ref::get_d3d_surface(int32 level, int32 cubemap_face)
{
	return INVOKE_CLASS_MEMBER(0x00A6E860, c_rasterizer_texture_ref, get_d3d_surface, level, cubemap_face);
}

IDirect3DTexture9* c_rasterizer_texture_ref::get_d3d_texture() const
{
	return INVOKE_CLASS_MEMBER(0x00A6E8E0, c_rasterizer_texture_ref, get_d3d_texture);
}

int32 c_rasterizer_texture_ref::get_mipmap_count()
{
	return INVOKE_CLASS_MEMBER(0x00A6E920, c_rasterizer_texture_ref, get_mipmap_count);
}

bool c_rasterizer_texture_ref::get_width_height(int32 level, int32* width, int32* height)
{
	return INVOKE_CLASS_MEMBER(0x00A6E950, c_rasterizer_texture_ref, get_width_height, level, width, height);
}

void __cdecl c_rasterizer_texture_ref::initialize()
{
	INVOKE(0x00A6E9C0, c_rasterizer_texture_ref::initialize);
}

bool c_rasterizer_texture_ref::is_cubemap() const
{
	return INVOKE_CLASS_MEMBER(0x00A6EAB0, c_rasterizer_texture_ref, is_cubemap);
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

