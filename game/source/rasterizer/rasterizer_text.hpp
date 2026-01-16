#pragma once

#include "cseries/cseries.hpp"

#define INVALID_TEXTURE_REF_HANDLE -1

enum e_bitmap_type;
enum e_character_data_index;
struct bitmap_data;
struct s_font_character;
__interface IDirect3DCubeTexture9;
__interface IDirect3DSurface9;
__interface IDirect3DTexture9;

class c_rasterizer_texture_ref
{
public:
	c_rasterizer_texture_ref() :
		m_datum_ref(INVALID_TEXTURE_REF_HANDLE)
	{
	}

	c_rasterizer_texture_ref(const c_rasterizer_texture_ref& ref) :
		m_datum_ref(ref.m_datum_ref)
	{
	}

	c_rasterizer_texture_ref(int32 datum_ref) :
		m_datum_ref(datum_ref)
	{
		ASSERT(m_datum_ref == INVALID_TEXTURE_REF_HANDLE);
	}

	static void __cdecl allocate(c_rasterizer_texture_ref& texture_ref, int32 width, int32 height, int32 mipmaps, uns32 format, int32 multisample_type, bool use_srgb_curve, e_bitmap_type type, int32 a9);
	static c_rasterizer_texture_ref& __cdecl allocate(c_rasterizer_texture_ref& texture_ref, const bitmap_data* bitmap, const char* name, bool system_memory);
	static void __cdecl dispose();
	uns32 get_d3d_format();
	IDirect3DSurface9* get_d3d_surface(int32 level, int32 cubemap_face);
	IDirect3DTexture9* get_d3d_texture() const;
	int32 get_mipmap_count();
	bool get_width_height(int32 level, int32* width, int32* height);
	static void __cdecl initialize();
	bool is_cubemap() const;
	static void __cdecl release(c_rasterizer_texture_ref& texture_ref);
	void* lock(int32* pitch);
	void unlock();

	bool __cdecl valid();

//protected:
	int32 m_datum_ref;
};
COMPILE_ASSERT(sizeof(c_rasterizer_texture_ref) == 0x4);

struct s_hardware_character
{
	uns32 character_key;
	e_character_data_index character_data_index;
	const s_font_character* font_character;
	int16 prev_character_index;
	int16 next_character_index;
	int16 x0;
	int16 y0;
};
COMPILE_ASSERT(sizeof(s_hardware_character) == 0x14);

struct s_hardware_character_cache_globals
{
	bool initialized;
	bool pad[0x3];
	int32 read_index;
	int32 write_index;
	int32 x0;
	int32 y0;
	int32 maximum_character_height;
	bitmap_data* bitmap;
	c_rasterizer_texture_ref hardware_format;
	real32 one_over_bitmap_width;
	real32 one_over_bitmap_height;
	c_static_array<int16, 512> hash_buckets;
	c_static_array<s_hardware_character, 512> hardware_character_data;
};
COMPILE_ASSERT(sizeof(s_hardware_character_cache_globals) == 0x2C28);

extern s_hardware_character_cache_globals& hardware_character_cache_globals;

extern void __cdecl rasterizer_bitmap_2d_changed_rect(const bitmap_data* bitmap, c_rasterizer_texture_ref hardware_format, const rectangle2d* dirty_rect, bool unused_wait_until_idle);

