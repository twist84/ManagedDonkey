#pragma once

#include "cseries/cseries.hpp"

struct bitmap_data;
__interface IDirect3DTexture9;
struct c_rasterizer_texture_ref
{
public:
	static void __cdecl allocate(c_rasterizer_texture_ref& texture_ref, long width, long height, long levels, dword format, long a6, bool a7, long a8, long a9);
	IDirect3DTexture9* get_d3d_texture() const;
	static void __cdecl initialize();
	static void __cdecl release(c_rasterizer_texture_ref& texture_ref);

	bool __cdecl valid();

//protected:
	dword m_datum_ref;
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
	byte __pad1[0x3];
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	bitmap_data* bitmap;
	c_rasterizer_texture_ref hardware_format;
	real __unknown20;
	real __unknown24;
	c_static_array<short, 512> __unknown28;
	c_static_array<s_hardware_character, 512> hardware_characters;
};
static_assert(sizeof(s_hardware_character_cache_globals) == 0x2C28);

extern s_hardware_character_cache_globals& hardware_character_cache_globals;

extern void __cdecl rasterizer_bitmap_2d_changed_rect(bitmap_data const* bitmap, c_rasterizer_texture_ref texture_ref, short_rectangle2d const* dirty_rect, bool a4);

