#pragma once

#include "cseries/cseries.hpp"

#define INVALID_TEXTURE_REF_HANDLE -1

struct bitmap_data;
__interface IDirect3DTexture9;
struct c_rasterizer_texture_ref
{
public:
	c_rasterizer_texture_ref() :
		m_datum_ref(INVALID_TEXTURE_REF_HANDLE)
	{
	}

	c_rasterizer_texture_ref(c_rasterizer_texture_ref const& ref) :
		m_datum_ref(ref.m_datum_ref)
	{
	}

	c_rasterizer_texture_ref(long datum_ref) :
		m_datum_ref(datum_ref)
	{
		ASSERT(m_datum_ref == INVALID_TEXTURE_REF_HANDLE);
	}

	static void __cdecl allocate(c_rasterizer_texture_ref& texture_ref, long width, long height, long levels, uint32 format, long a6, bool a7, long a8, long a9);
	static c_rasterizer_texture_ref& __cdecl allocate(c_rasterizer_texture_ref& texture_ref, bitmap_data const* bitmap, char const* name, bool system_memory);
	static void __cdecl dispose();
	IDirect3DTexture9* get_d3d_texture() const;
	bool get_width_height(long level, long* width, long* height);
	static void __cdecl initialize();
	static void __cdecl release(c_rasterizer_texture_ref& texture_ref);
	void* lock(long* pitch);
	void unlock();

	bool __cdecl valid();

//protected:
	long m_datum_ref;
};
static_assert(sizeof(c_rasterizer_texture_ref) == 0x4);

struct s_hardware_character
{
	long character_data_index;
	long __unknown4;
	long __unknown8;
	short __unknownC;
	short __unknownE;
	short __unknown10;
	short __unknown12;
};
static_assert(sizeof(s_hardware_character) == 0x14);

struct s_hardware_character_cache_globals
{
	bool initialized;
	uint8 __pad1[0x3];
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	bitmap_data* bitmap;
	c_rasterizer_texture_ref hardware_format;
	real32 __unknown20;
	real32 __unknown24;
	c_static_array<short, 512> __unknown28;
	c_static_array<s_hardware_character, 512> hardware_characters;
};
static_assert(sizeof(s_hardware_character_cache_globals) == 0x2C28);

extern s_hardware_character_cache_globals& hardware_character_cache_globals;

extern void __cdecl rasterizer_bitmap_2d_changed_rect(bitmap_data const* bitmap, c_rasterizer_texture_ref texture_ref, rectangle2d const* dirty_rect, bool a4);

